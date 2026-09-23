#include "game_strings.h"

#include <windows.h>

#include <cstdint>
#include <cstring>
#include <map>
#include <vector>

#include "../util/game_settings.h"
#include "../util/log.h"

namespace game {
namespace {

constexpr uint32_t kTypeDir = 0xE86B1EEF;
constexpr uint32_t kTypeStr = 0x53545223;  // STR#

std::vector<uint8_t> ReadFileBytes(const std::wstring& path) {
    std::vector<uint8_t> out;
    HANDLE f = CreateFileW(path.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
    if (f == INVALID_HANDLE_VALUE) return out;
    LARGE_INTEGER size = {};
    GetFileSizeEx(f, &size);
    if (size.QuadPart > 0 && size.QuadPart < 256 * 1024 * 1024) {
        out.resize((size_t)size.QuadPart);
        DWORD got = 0;
        if (!ReadFile(f, out.data(), (DWORD)out.size(), &got, nullptr) || got != out.size()) out.clear();
    }
    CloseHandle(f);
    return out;
}

uint32_t U32(const std::vector<uint8_t>& d, size_t at) {
    if (at + 4 > d.size()) return 0;
    uint32_t v;
    memcpy(&v, d.data() + at, 4);
    return v;
}

std::vector<uint8_t> RefPack(const uint8_t* p, size_t n, size_t outSize) {
    std::vector<uint8_t> out;
    out.reserve(outSize);
    size_t i = 0;
    while (i < n) {
        uint8_t c0 = p[i];
        size_t plain = 0, copy = 0, off = 0;
        if (c0 < 0x80) {
            if (i + 1 >= n) break;
            plain = c0 & 3;
            copy = ((c0 >> 2) & 7) + 3;
            off = ((c0 & 0x60) << 3) + p[i + 1] + 1;
            i += 2;
        } else if (c0 < 0xC0) {
            if (i + 2 >= n) break;
            plain = (p[i + 1] >> 6) & 3;
            copy = (c0 & 0x3F) + 4;
            off = ((p[i + 1] & 0x3F) << 8) + p[i + 2] + 1;
            i += 3;
        } else if (c0 < 0xE0) {
            if (i + 3 >= n) break;
            plain = c0 & 3;
            copy = ((c0 & 0x0C) << 6) + p[i + 3] + 5;
            off = ((c0 & 0x10) << 12) + (p[i + 1] << 8) + p[i + 2] + 1;
            i += 4;
        } else if (c0 < 0xFC) {
            plain = ((c0 & 0x1F) << 2) + 4;
            i += 1;
        } else {
            plain = c0 & 3;
            i += 1;
        }
        for (size_t k = 0; k < plain && i < n; k++) out.push_back(p[i++]);
        for (size_t k = 0; k < copy; k++) {
            if (off > out.size()) return out;
            out.push_back(out[out.size() - off]);
        }
        if (c0 >= 0xFC || out.size() >= outSize) break;
    }
    return out;
}

// One STR# resource of a package, decompressed; empty when absent.
std::vector<uint8_t> FindStr(const std::vector<uint8_t>& d, uint32_t instance) {
    if (d.size() < 0x60 || memcmp(d.data(), "DBPF", 4) != 0) return {};
    uint32_t count = U32(d, 0x24), index = U32(d, 0x28), minor = U32(d, 0x3c);
    size_t esz = minor == 2 ? 24 : 20;
    struct Entry {
        uint32_t type, group, inst, off, size;
    };
    std::vector<Entry> entries;
    for (uint32_t k = 0; k < count; k++) {
        size_t p = index + k * esz;
        Entry e = {U32(d, p), U32(d, p + 4), U32(d, p + 8), U32(d, p + esz - 8), U32(d, p + esz - 4)};
        entries.push_back(e);
    }
    std::map<uint32_t, uint32_t> compressed;  // instance -> uncompressed size, for STR#
    for (const Entry& e : entries) {
        if (e.type != kTypeDir) continue;
        size_t step = minor == 2 ? 20 : 16;
        for (size_t q = e.off; q + step <= (size_t)e.off + e.size; q += step)
            if (U32(d, q) == kTypeStr) compressed[U32(d, q + 8)] = U32(d, q + step - 4);
    }
    for (const Entry& e : entries) {
        if (e.type != kTypeStr || e.group != 0xFFFFFFFF || e.inst != instance) continue;
        if ((size_t)e.off + e.size > d.size()) return {};
        const uint8_t* raw = d.data() + e.off;
        if (compressed.count(instance) && e.size > 9 && raw[4] == 0x10 && raw[5] == 0xFB) {
            size_t usz = (raw[6] << 16) | (raw[7] << 8) | raw[8];
            return RefPack(raw + 9, e.size - 9, usz);
        }
        return std::vector<uint8_t>(raw, raw + e.size);
    }
    return {};
}

// Entries of one language, in order.
std::vector<std::string> ParseStr(const std::vector<uint8_t>& b, int language) {
    std::vector<std::string> out;
    if (b.size() < 68 || b[64] != 0xFD || b[65] != 0xFF) return out;
    int count = b[66] | (b[67] << 8);
    size_t p = 68;
    for (int k = 0; k < count && p < b.size(); k++) {
        int lang = b[p++];
        size_t e = p;
        while (e < b.size() && b[e]) e++;
        std::string value((const char*)b.data() + p, e - p);
        p = e + 1;
        e = p;
        while (e < b.size() && b[e]) e++;
        p = e + 1;  // the comment
        if (lang == language) out.push_back(value);
    }
    return out;
}

std::wstring InstallDir() {
    wchar_t exe[MAX_PATH];
    GetModuleFileNameW(nullptr, exe, MAX_PATH);
    std::wstring dir(exe);
    for (int up = 0; up < 3; up++) dir = dir.substr(0, dir.find_last_of(L"\\/"));  // ...\EP9\TSBin\x.exe
    return dir;
}

std::map<std::string, std::vector<std::string>> g_cache;  // "file/instance/language" -> entries

const std::vector<std::string>& Table(const char* file, unsigned instance, int language) {
    std::string key = std::string(file) + "/" + std::to_string(instance) + "/" + std::to_string(language);
    auto it = g_cache.find(key);
    if (it != g_cache.end()) return it->second;
    std::vector<std::string> entries;
    // The newest pack first: expansions carry the complete, updated tables.
    static const wchar_t* const kPacks[] = {L"EP9", L"SP7", L"EP8", L"SP6", L"EP7", L"SP5", L"SP4", L"EP6", L"EP5",
                                             L"EP4", L"SP2", L"SP1", L"EP3", L"EP2", L"EP1", L"Base"};
    std::wstring root = InstallDir();
    std::wstring wfile(file, file + strlen(file));
    for (const wchar_t* pack : kPacks) {
        std::vector<uint8_t> data = ReadFileBytes(root + L"\\" + pack + L"\\TSData\\Res\\Text\\" + wfile);
        if (data.empty()) continue;
        std::vector<uint8_t> str = FindStr(data, instance);
        if (str.empty()) continue;
        entries = ParseStr(str, language);
        if (!entries.empty()) break;
    }
    Log("game strings %s #%u, language %d: %d entries", file, instance, language, (int)entries.size());
    return g_cache.emplace(key, std::move(entries)).first->second;
}

}  // namespace

int StringLanguageCode() {
    switch (util::ReadGameLanguageNumber()) {
    case 2: return 3;    // French
    case 3: return 4;    // German
    case 4: return 5;    // Italian
    case 5: return 6;    // Spanish
    case 6: return 9;    // Swedish
    case 7: return 11;   // Finnish
    case 8: return 7;    // Dutch
    case 9: return 8;    // Danish
    case 10: return 35;  // Brazilian Portuguese
    case 11: return 26;  // Czech
    case 14: return 15;  // Japanese
    case 15: return 20;  // Korean
    case 16: return 13;  // Russian
    case 17: return 17;  // Simplified Chinese
    case 18: return 18;  // Traditional Chinese
    case 20: return 16;  // Polish
    case 21: return 19;  // Thai
    case 22: return 10;  // Norwegian
    case 23: return 14;  // Portuguese
    case 25: return 28;  // Hungarian
    default: return 1;   // English
    }
}

std::string GameString(const char* file, unsigned instance, int index) {
    const auto& t = Table(file, instance, StringLanguageCode());
    if (index >= 0 && index < (int)t.size() && !t[index].empty()) return t[index];
    const auto& en = Table(file, instance, 1);
    return index >= 0 && index < (int)en.size() ? en[index] : std::string();
}

}  // namespace game
