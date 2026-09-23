#include "memory.h"

#include <cstring>

#include "../util/log.h"
#include "../util/text.h"

namespace game {
namespace {

constexpr Ptr kPreferredBase = 0xA0000000;
Ptr g_base = 0;
Ptr g_imageEnd = 0;

struct Signature {
    Ptr va;
    const char* name;
    unsigned char bytes[8];
    unsigned char len;  // compared bytes; relocated operands are left out
    bool ok;
};

Signature g_signatures[] = {
    {0xa01428e2, "cCTGFramework::GetSystemService", {0x55, 0x8b, 0xec, 0x51, 0x53, 0x56}, 6, false},
    {0xa0972f48, "cGZWinMgrW32::DoModalWin", {0x83, 0xec, 0x24, 0x53, 0x8b, 0xd9}, 6, false},
    {0xa09357ef, "cGZWinBtn::MouseDownL", {0x53, 0x56, 0x57}, 3, false},
    {0xa09359f1, "cGZWinBtn::MouseUpL", {0x53, 0x55, 0x56}, 3, false},
    {0xa078d6c3, "cTSUserToolBase::Pick", {0x8b, 0x49, 0x04, 0x53, 0x85, 0xc9}, 6, false},
    {0xa067f31a, "cTSUserToolObject::SetFits", {0x53, 0x8b, 0x5c, 0x24, 0x08, 0x56}, 6, false},
    {0xa067be89, "cTSUserToolObject::IsHolding", {0x56, 0x8b, 0xf1, 0xe8}, 4, false},
    {0xa067cb69, "cTSUserToolObject::HeldObject", {0x56, 0x8b, 0xf1, 0xe8}, 4, false},
    {0xa078d789, "cTSUserToolBase::ObjectPick", {0x55, 0x8b, 0xec, 0x8b, 0x49, 0x04, 0x85, 0xc9}, 8, false},
    {0xa078ec6d, "cTSUserToolBase::MouseLeave", {0x8b, 0x01, 0xff, 0xa0, 0xf0, 0x00, 0x00, 0x00}, 8, false},
};

bool RawRead(Ptr addr, void* out, size_t n) {
    __try {
        memcpy(out, (const void*)addr, n);
        return true;
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        return false;
    }
}

}  // namespace

void InitMemory(HMODULE exe) {
    g_base = (Ptr)exe;
    auto* dos = (IMAGE_DOS_HEADER*)g_base;
    auto* nt = (IMAGE_NT_HEADERS*)(g_base + dos->e_lfanew);
    g_imageEnd = g_base + nt->OptionalHeader.SizeOfImage;
    Log("Sims2EP9.exe base %p, image end %p", (void*)g_base, (void*)g_imageEnd);
    for (Signature& sig : g_signatures) {
        unsigned char got[8] = {};
        sig.ok = Read(Va(sig.va), got, sig.len) && memcmp(got, sig.bytes, sig.len) == 0;
        Log("signature %s at 0x%06x: %s", sig.name, (unsigned)sig.va, sig.ok ? "ok" : "MISMATCH");
    }
}

Ptr Va(Ptr preferredVa) { return preferredVa - kPreferredBase + g_base; }

bool InImage(Ptr p) { return p >= g_base && p < g_imageEnd; }

bool Read(Ptr addr, void* out, size_t n) {
    if (addr < 0x10000) return false;
    return RawRead(addr, out, n);
}

bool WriteU32(Ptr addr, uint32_t value) {
    __try {
        *(volatile uint32_t*)addr = value;
        return true;
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        return false;
    }
}

bool WriteU8(Ptr addr, uint8_t value) {
    return Guarded([&] { *(volatile uint8_t*)addr = value; });
}

std::string ReadStdString(Ptr addr) {
    uint32_t size = Rd<uint32_t>(addr + 0x10);
    uint32_t cap = Rd<uint32_t>(addr + 0x14);
    if (size == 0 || size > 8192 || cap < 15 || size > cap) return "";
    Ptr data = cap > 15 ? Rd<Ptr>(addr) : addr;  // inline buffer while capacity <= 15
    std::string s(size, '\0');
    if (!Read(data, s.data(), size)) return "";
    return s;
}

std::string ReadStdStringUtf8(Ptr addr) { return ReadStdString(addr); }  // Sims 2 text is UTF-8

bool FunctionOk(Ptr preferredVa) {
    for (const Signature& sig : g_signatures)
        if (sig.va == preferredVa) return sig.ok;
    return false;
}

}  // namespace game
