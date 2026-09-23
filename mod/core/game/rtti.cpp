#include "rtti.h"

#include <unordered_map>
#include <vector>

namespace game {
namespace {

struct ClassInfo {
    std::string name;                // empty: vtable without usable RTTI
    std::vector<std::string> bases;  // the class itself and every base
};

std::unordered_map<Ptr, ClassInfo> g_classes;  // by vtable address

// TypeDescriptor name ".?AVcTSWinBtn@@" -> "cTSWinBtn", ".?AVcAnimatable@nTSSG@@" ->
// "nTSSG::cAnimatable" (as re/classes.tsv writes them); templates keep their decorated name.
std::string TypeName(Ptr typeDescriptor) {
    char raw[128] = {};
    if (!Read(typeDescriptor + 8, raw, sizeof raw - 1)) return "";
    std::string s(raw);
    if (s.rfind(".?AV", 0) == 0 || s.rfind(".?AU", 0) == 0) s = s.substr(4);
    size_t end = s.find("@@");
    if (end != std::string::npos) s = s.substr(0, end);
    if (s.rfind("?$", 0) == 0) return s;
    std::string out;
    size_t start = 0;
    while (true) {  // innermost name first: prepend each scope
        size_t at = s.find('@', start);
        std::string part = s.substr(start, at == std::string::npos ? std::string::npos : at - start);
        out = out.empty() ? part : part + "::" + out;
        if (at == std::string::npos) break;
        start = at + 1;
    }
    return out;
}

// x86 layout: vtable[-1] -> CompleteObjectLocator {signature 0, offset, cdOffset,
// TypeDescriptor*, ClassHierarchyDescriptor*}; the hierarchy lists BaseClassDescriptor*,
// each starting with its TypeDescriptor*.
ClassInfo ReadClassInfo(Ptr vtable) {
    ClassInfo info;
    Ptr col = Rd<Ptr>(vtable - 4);
    if (!InImage(col) || Rd<uint32_t>(col) != 0) return info;
    info.name = TypeName(Rd<Ptr>(col + 12));
    Ptr hierarchy = Rd<Ptr>(col + 16);
    uint32_t count = Rd<uint32_t>(hierarchy + 8);
    Ptr bases = Rd<Ptr>(hierarchy + 12);
    for (uint32_t i = 0; i < count && i < 32; i++) info.bases.push_back(TypeName(Rd<Ptr>(Rd<Ptr>(bases + i * 4))));
    return info;
}

const ClassInfo* ClassOf(Ptr obj) {
    Ptr vtable = Rd<Ptr>(obj);
    if (!InImage(vtable)) return nullptr;
    auto it = g_classes.find(vtable);
    if (it == g_classes.end()) it = g_classes.emplace(vtable, ReadClassInfo(vtable)).first;
    return it->second.name.empty() ? nullptr : &it->second;
}

}  // namespace

std::string ClassName(Ptr obj) {
    const ClassInfo* c = ClassOf(obj);
    return c ? c->name : std::string();
}

bool IsKindOf(Ptr obj, const char* cls) {
    const ClassInfo* c = ClassOf(obj);
    if (!c) return false;
    for (const std::string& base : c->bases)
        if (base == cls) return true;
    return false;
}

void ClearRttiCache() { g_classes.clear(); }

}  // namespace game
