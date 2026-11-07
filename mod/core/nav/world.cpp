#include "world.h"

#include "../game/buy.h"
#include "../game/hooks.h"
#include "../game/mouse.h"
#include "collect.h"
#include "model.h"
#include "placement.h"
#include "reach.h"

#include <windows.h>

#include <algorithm>
#include <cmath>
#include <vector>

#include "../game/addresses.h"
#include "../game/game_strings.h"
#include "../game/gz.h"
#include "../game/objects.h"
#include "../game/rtti.h"
#include "../speech/speech.h"
#include "../util/log.h"
#include "../util/strings.h"
#include "../util/text.h"
#include "buffers.h"

namespace nav::world {
namespace {

using game::LotObject;
using game::Ptr;

// Buy mode's function sort flags (OBJD, game/addresses.h) with the index of their name in
// UIText.package STR# 150, in the order the buy catalog shows them.
constexpr uint16_t kSims = 0, kOther = 0xFFFF, kProblems = 0xFFFE, kPortals = 0xFFFD;
struct Category {
    uint16_t flag;   // or one of the four above, the mod's own kinds
    int nameIndex;   // in STR# 150, or -1 for the mod's own name
};
const Category kCategories[] = {
    {kSims, -1},
    {kProblems, -1},  // what wants attention
    {0x0001, 8},     // Comfort
    {0x0002, 9},     // Surfaces
    {0x0020, 10},    // Decorative
    {0x0008, 11},    // Electronics
    {0x0004, 12},    // Appliances
    {0x0010, 13},    // Plumbing
    {0x0080, 14},    // Lighting
    {0x0040, 15},    // Miscellaneous
    {0x0100, 16},    // Hobbies
    {kOther, -1},    // other objects (plants, the mailbox...)
    {kPortals, -1},  // doors, windows and stairs
};
constexpr int kCategoryCount = sizeof(kCategories) / sizeof(kCategories[0]);
constexpr unsigned kCategoryNames = 150;

int g_focusId = -1;               // the thing focus was last on
int g_openKind = 0;               // the kind open in the "things by kind" group (its number), 0: none
int g_openRoom = 0;               // the room open in the "things by room" group (its number), 0: none
Ptr g_lastGroup = kKindsGroup;    // the group a kind or room was opened in last: PageUp/PageDown's
ULONGLONG g_menuAskedAt = 0;  // Interact() posted a menu request at this time
ULONGLONG g_walkAskedAt = 0;  // WalkTo() clicked the ground at this time
bool g_walkRun = false;
bool g_walkClickPending = false;  // waiting for Ctrl and Shift to come up (else a Ctrl+click)

std::string CategoryName(int c) {
    if (kCategories[c].flag == kSims) return S(Str::CategorySims);
    if (kCategories[c].flag == kProblems) return S(Str::CategoryProblems);
    if (kCategories[c].flag == kPortals) return S(Str::CategoryPortals);
    if (kCategories[c].nameIndex < 0) return S(Str::CategoryOther);
    std::string n = game::GameString("UIText.package", kCategoryNames, kCategories[c].nameIndex);
    return n.empty() ? S(Str::CategoryOther) : n;
}

// A Sim's full name; else from its internal one ("E001_User00013 - Carlos" -> "Carlos").
std::string SimName(const LotObject& o) {
    std::string first, last;
    if (game::SimName(o.id, &first, &last)) return last.empty() ? first : first + " " + last;
    size_t dash = o.name.rfind(" - ");
    return dash == std::string::npos ? o.name : o.name.substr(dash + 3);
}

}  // namespace

namespace {

// Messes: objects outside the catalog (a "*" name or none), so the mod has their names.
Str MessName(const std::string& internal) {
    static const struct {
        const char* word;
        size_t length;  // 0: the whole name
        Str name;
    } kMesses[] = {
        {"Puddle", 6, Str::ObjPuddle},      {"Ashes", 0, Str::ObjAshes},      {"Fire", 0, Str::ObjFire},
        {"Roaches", 0, Str::ObjRoaches},    {"Trash - Trash Pile", 0, Str::ObjTrashPile},
        {"Trash - Furniture Debris Pile", 0, Str::ObjTrashPile},
        {"Trash - Electronic Debris Pile", 0, Str::ObjTrashPile},
        {"Dirt Pile", 9, Str::ObjDigPile},
    };
    for (const auto& m : kMesses)
        if (m.length ? internal.compare(0, m.length, m.word) == 0 : internal == m.word) return m.name;
    return Str::Count;
}

bool IsMess(const std::string& internal) { return MessName(internal) != Str::Count; }

}  // namespace

std::string DisplayName(const LotObject& o) {
    if (o.person) return SimName(o);
    if (IsMess(o.name)) return S(MessName(o.name));
    std::string n = util::CleanForSpeech(o.catalogName.empty() ? o.name : o.catalogName);
    return n;
}

namespace {

// Variable 39 also keeps how overgrown a garden plant is: not something a Sim cleans.
bool IsGardenPlant(const std::string& internal) {
    for (const char* word : {"Shrub", "Flower", "Plant", "Hedge", "Tree"})
        if (internal.rfind(word, 0) == 0) return true;
    return false;
}

bool IsDirty(const LotObject& o) {
    return !o.person && o.dirt >= game::addr::objvar::kDirtyFrom && !IsGardenPlant(o.name) && !IsMess(o.name);
}

// What wants someone's attention: what the game's repairman and maid would go for, and messes.
bool IsProblem(const LotObject& o) { return !o.person && (o.broken || IsDirty(o) || IsMess(o.name)); }

// Doors and windows (OBJD types 8 and 9), and every piece of a staircase.
bool IsPortal(const LotObject& o) {
    return !o.person && (o.type == 8 || o.type == 9 || o.name.rfind("Stair", 0) == 0);
}

// What the browser lists: placed objects with a name a player would know, and the Sims.
bool Listed(const LotObject& o) {
    if (o.x < 0 || o.y < 0) return false;  // controllers and other objects kept off the lot
    if (o.person || IsMess(o.name)) return true;
    if (o.catalogName.empty() || o.catalogName[0] == '*') return false;  // internal objects
    if (o.name == "PlumbBob") return false;  // the selected Sim's marker
    return true;
}

bool InCategory(const LotObject& o, int c) {
    uint16_t flag = kCategories[c].flag;
    if (flag == kSims) return o.person;
    if (o.person) return false;
    if (flag == kProblems) return IsProblem(o);
    if (flag == kPortals) return IsPortal(o);
    if (flag == kOther) return o.functionFlags == 0 && !IsPortal(o) && !IsMess(o.name);
    return (o.functionFlags & flag) != 0;
}

// A room's name from what stands in it: each object votes for the buy mode rooms it is sold for
// (an object meant for many rooms votes less), the most votes wins. Room 0 is outside.
std::string RoomName(int level, int room, const std::vector<LotObject>& all) {
    static const struct {
        uint16_t flag;
        int nameIndex;  // UIText.package STR# 150
    } kRooms[] = {{0x0004, 5}, {0x0001, 4}, {0x0002, 2}, {0x0100, 17}, {0x0080, 3},
                  {0x0020, 1}, {0x0008, 0}, {0x0010, 6}, {0x0040, 7}};
    if (room == 0) return game::GameString("UIText.package", kCategoryNames, 6);
    double best = 0;
    int bestIndex = 0;  // living room when nothing tells
    for (const auto& r : kRooms) {
        double votes = 0;
        for (const LotObject& o : all) {
            if (o.person || o.level != level || o.room != room || !(o.roomFlags & r.flag)) continue;
            int bits = 0;
            for (uint16_t f = o.roomFlags; f; f &= f - 1) bits++;
            votes += 1.0 / bits;
        }
        if (votes > best + 1e-9) {
            best = votes;
            bestIndex = r.nameIndex;
        }
    }
    return game::GameString("UIText.package", kCategoryNames, bestIndex);
}

bool SimPosition(LotObject* sim) {
    int id = game::SelectedSimId();
    return id && game::ReadLotObject(id, sim);
}

double Distance(const LotObject& a, const LotObject& b) { return std::hypot(a.x - b.x, a.y - b.y); }

// "3 casillas al noreste" from the selected Sim; -y is north, +x east (the lot's own axes).
std::string Where(const LotObject& o, const LotObject* sim) {
    if (!sim) return "";
    // Level 0 is the terrain, 1 the floor of a house on a foundation: both the ground floor.
    int floor = std::max(o.level, 1), simFloor = std::max(sim->level, 1);
    if (floor != simFloor) return util::Format(S(Str::FloorNumber), floor);
    double dx = o.x - sim->x, dy = o.y - sim->y;
    int tiles = (int)std::lround(std::hypot(dx, dy));
    if (tiles == 0) return S(Str::HereNow);
    static const Str kDirs[] = {Str::DirEast, Str::DirSouthEast, Str::DirSouth, Str::DirSouthWest,
                                Str::DirWest, Str::DirNorthWest, Str::DirNorth, Str::DirNorthEast};
    double angle = std::atan2(dy, dx);  // 0 east, pi/2 south (+y)
    int octant = ((int)std::lround(angle / (3.14159265358979 / 4)) + 8) % 8;
    const char* dir = S(kDirs[octant]);
    return tiles == 1 ? util::Format(S(Str::OneTileAway), dir) : util::Format(S(Str::TilesAway), tiles, dir);
}

// A thing's line: "Nevera, cocina, 6 casillas al sur" (in a room's own list, without the room).
std::string Line(const LotObject& o, const LotObject* sim, bool roomMode, const std::vector<LotObject>& all) {
    std::string where = Where(o, sim);
    std::string line = DisplayName(o);
    if (o.broken && !o.person) line += std::string(", ") + S(Str::ConditionBroken);
    else if (IsDirty(o)) line += std::string(", ") + S(Str::ConditionDirty);
    // What no Sim can get to from the lot's entrance (in the Sims 1 mod a tester's Sim never
    // reached the chair he had placed).
    if (!o.person && !IsPortal(o) && !IsMess(o.name) && reach::Unreachable(o.id))
        line += std::string(", ") + S(Str::NobodyReaches);
    if (!roomMode && o.room >= 0) line += ", " + RoomName(o.level, o.room, all);
    if (o.person && sim && o.id == sim->id) line += std::string(", ") + S(Str::SelectedSimSuffix);
    if (!where.empty()) line += ", " + where;
    return line;
}

}  // namespace

std::string WhereFromSim(float x, float y, int level) {
    LotObject simObj;
    if (!SimPosition(&simObj)) return "";
    LotObject spot;
    spot.x = x;
    spot.y = y;
    spot.level = level;
    return Where(spot, &simObj);
}

namespace {
std::string LookWords(int objectId);  // below, with the lists
}  // namespace

std::string SimLookLine(int objectId) {
    LotObject o;
    if (!game::ReadLotObject(objectId, &o) || !o.person) return "";
    std::string words = LookWords(objectId);
    std::string name = DisplayName(o);
    return words.empty() ? util::Format(S(Str::SimLookUnknown), name.c_str()) : name + ": " + words;
}

void SayLayout(int x, int y, int level) {
    int room = game::RoomAt(x + 0.5f, y + 0.5f, level);
    if (room < 0) {
        speech::Say(S(Str::LayoutNoRoom));
        return;
    }
    std::vector<LotObject> all = game::LotObjects();
    std::string name = RoomName(level, room, all);
    // The room's tiles (off-lot tiles belong to no room of this level).
    int count = 0, x0 = 1 << 20, y0 = 1 << 20, x1 = -1, y1 = -1;
    for (int tx = 0; tx < 80; tx++)
        for (int ty = 0; ty < 80; ty++) {
            if (game::RoomAt(tx + 0.5f, ty + 0.5f, level) != room) continue;
            count++;
            x0 = std::min(x0, tx);
            y0 = std::min(y0, ty);
            x1 = std::max(x1, tx);
            y1 = std::max(y1, ty);
        }
    // Doors and windows on its walls, and what is on their other side.
    static const int kDx[] = {-1, 0, 1, 0}, kDy[] = {0, 1, 0, -1};           // 0 -x, 2 +y, 4 +x, 6 -y
    static const Str kFront[] = {Str::DirWest, Str::DirSouth, Str::DirEast, Str::DirNorth};
    static const Str kBehind[] = {Str::DirEast, Str::DirNorth, Str::DirWest, Str::DirSouth};
    int doors = 0, windows = 0;
    std::vector<std::string> lines;
    for (const game::WallObject& w : game::WallObjects()) {
        if (w.level != level) continue;
        int d = (w.direction / 2) & 3;
        float fx = w.x, fy = w.y, bx = w.x - kDx[d], by = w.y - kDy[d];
        int front = game::RoomAt(fx, fy, level), behind = game::RoomAt(bx, by, level);
        Str side;
        int other;
        float ox, oy;
        if (front == room && behind != room) {
            side = kBehind[d];
            other = behind;
            ox = bx;
            oy = by;
        } else if (behind == room && front != room) {
            side = kFront[d];
            other = front;
            ox = fx;
            oy = fy;
        } else {
            continue;
        }
        (w.type == 8 ? doors : windows)++;
        std::string otherName = other >= 0 ? RoomName(level, other, all) : "";
        std::string line = util::Format(S(w.type == 8 ? Str::LayoutDoor : Str::LayoutWindow), util::CleanForSpeech(w.name).c_str(),
                                        S(side), otherName.c_str(), (int)ox, (int)oy);
        if (w.name.empty()) {  // no name: "puerta , pared" -> "puerta, pared"
            size_t gap = line.find(" ,");
            if (gap != std::string::npos) line.erase(gap, 1);
        }
        lines.push_back(line);
    }
    std::string summary = util::Format(S(Str::LayoutSummary), name.c_str(), std::max(level, 1), count, x0, y0, x1, y1);
    summary += ", " + util::Format(S(Str::LayoutOpenings), doors, windows);
    std::vector<std::string> details = {summary};
    for (const std::string& l : lines) details.push_back(l);
    buffers::SetDetails(details);
    speech::Say(summary);
    Log("layout of room %d floor %d: %d tiles, %d doors, %d windows", room, level, count, doors, windows);
}

std::string RoomNameAt(float x, float y, int level) {
    int room = game::RoomAt(x, y, level);
    return room < 0 ? "" : RoomName(level, room, game::LotObjects());
}

bool LotViewActive() {
    // The live panel's proc or the buy/build panels: the lot view is up.
    Ptr main = game::MainWindow();
    std::vector<Ptr> stack = {main};
    int visited = 0;
    while (!stack.empty() && visited < 400) {
        Ptr w = stack.back();
        stack.pop_back();
        visited++;
        if (!game::Visible(w)) continue;
        std::string proc = game::ProcClass(w);
        if (proc == "cTSWinProcLivePanel" || proc == "cTSWinProcBuyPanel" || proc == "cTSWinProcBuildPanel" ||
            proc == "cTSWinProcSellPanel")  // buy mode while an object of the lot is held
            return true;
        for (Ptr c : game::Children(w)) stack.push_back(c);
    }
    return false;
}

void Reset() {
    g_focusId = -1;
    g_openKind = 0;
    g_openRoom = 0;
    g_lastGroup = kKindsGroup;
}

namespace {

// The rooms with something listed in them, as (level, room): the Sim's own first, then by floor
// and id.
std::vector<std::pair<int, int>> ListedRooms(const std::vector<LotObject>& all, const LotObject* sim) {
    std::vector<std::pair<int, int>> rooms;
    for (const LotObject& o : all)
        if (Listed(o) && o.room >= 0) {
            std::pair<int, int> r = {o.level, o.room};
            if (std::find(rooms.begin(), rooms.end(), r) == rooms.end()) rooms.push_back(r);
        }
    std::sort(rooms.begin(), rooms.end());
    if (sim && sim->room >= 0) {
        auto own = std::find(rooms.begin(), rooms.end(), std::make_pair(sim->level, sim->room));
        if (own != rooms.end()) std::rotate(rooms.begin(), own, own + 1);
    }
    return rooms;
}

// A room's item number: never 0 (0 is "no number" for the navigator).
int RoomNumber(int level, int room) { return level * 10000 + room + 1; }

void SortNearest(std::vector<LotObject>& list, const LotObject* sim) {
    std::stable_sort(list.begin(), list.end(), [&](const LotObject& a, const LotObject& b) {
        if (!sim) return a.id < b.id;
        bool sa = a.level == sim->level, sb = b.level == sim->level;
        if (sa != sb) return sa;
        return Distance(a, *sim) < Distance(b, *sim);
    });
}

std::string JoinWords(const std::vector<std::string>& parts) {
    std::string out;
    for (const std::string& p : parts) out += (out.empty() ? "" : ", ") + p;
    return out;
}

// What a Sim looks like, in the game's own words where it has them (Create a Sim's labels, the
// turn-ons' phrases): "Adulto, Mujer, Cabello rubio, Tono de piel: claro, Color de ojos: Azul,
// Gafas, en forma, Ropa: Cotidiano". Empty when the game says nothing (a pet has no outfit data).
std::string LookWords(int objectId) {
    game::SimLooks l;
    if (!game::SimLook(objectId, &l)) return "";
    std::vector<std::string> parts;
    auto add = [&](const std::string& w) {
        if (!w.empty()) parts.push_back(util::CleanForSpeech(w));
    };
    auto labeled = [&](const std::string& label, const std::string& value) {
        if (!value.empty()) add(label.empty() ? value : util::Format(S(Str::LabelValue), label.c_str(), value.c_str()));
    };
    // The age stage as Create a Sim names it (person data 58 in days, the person interface's slot 22).
    std::string age;
    if (l.ageDays <= 1) age = game::GameString("eCAS.package", 1, 72);                // baby
    else if (l.ageDays == 2) age = game::GameString("CAS.package", 0x85, 2);          // toddler
    else if (l.ageDays <= 15) age = game::GameString("CAS.package", 0x85, 3);         // child
    else if (l.ageDays <= 18) age = game::GameString("CAS.package", 0x85, 4);         // teen
    else if (l.ageDays <= 50 && l.youngAdult) age = game::GameString("CAS_Shared.package", 0xc8, 0);
    else if (l.ageDays <= 50) age = game::GameString("CAS.package", 0x85, 5);         // adult
    else age = game::GameString("CAS.package", 0x85, 6);                              // elder
    if (l.human) add(age);
    add(game::GameString("CAS.package", 0x85, l.gender == 1 ? 1 : 0));
    if (!l.human) return parts.empty() ? "" : JoinWords(parts);
    // Hair: the turn-ons' phrases (UIText STR# 0xe1: 14 blond ... 19 grey).
    static const int kHair[] = {-1, 18, 17, 16, 14, 15, 19};
    if (l.hair >= 1 && l.hair <= 6) add(game::GameString("UIText.package", 0xe1, kHair[l.hair]));
    else if (l.hair == 7) add(S(Str::LookHairDyed));
    static const Str kSkin[] = {Str::LookSkinCustom, Str::LookSkinLight, Str::LookSkinMedium, Str::LookSkinMediumDark,
                                Str::LookSkinDark, Str::LookSkinAlien, Str::LookSkinZombie};
    if (l.skin >= 1 && l.skin <= 7) labeled(game::GameString("CAS.package", 0x85, 7), S(kSkin[l.skin - 1]));
    // Eyes: Create a Sim's colours (CAS STR# 0x87: 12 brown, 13 blue, 14 grey, 15 green, 16 light blue).
    std::string eyes;
    static const int kEyes[] = {-1, -1, 15, 12, 16, 13, 14};
    if (l.eyes >= 2 && l.eyes <= 6) eyes = game::GameString("CAS.package", 0x87, kEyes[l.eyes]);
    else if (l.eyes == 1) eyes = S(Str::LookEyesCustom);
    else if (l.eyes == 7) eyes = S(Str::LookEyesAlien);
    labeled(game::GameString("eCAS.package", 1, 23), eyes);
    // What is worn on the face and head: the turn-ons' words.
    if (l.glasses) add(game::GameString("UIText.package", 0xe1, 9));
    if (l.facialHair) add(game::GameString("UIText.package", 0xe1, 8));
    if (l.fullMakeup) add(game::GameString("UIText.package", 0xe1, 11));
    else if (l.makeup) add(game::GameString("UIText.package", 0xe1, 10));
    if (l.hat) add(game::GameString("UIText.package", 0xe1, 12));
    if (l.jewelry) add(game::GameString("UIText.package", 0xe1, 13));
    if (l.bodyFlags & 1) add(S(Str::LookBodyHeavy));
    else if (l.bodyFlags & 0x10) add(S(Str::LookBodyFit));
    // The outfit worn, as Create a Sim's clothing tabs name it (CAS STR# 0x89, 0x9b).
    std::string outfit;
    switch (l.outfit) {
    case 0: case 7: case 9: case 10: case 12: outfit = game::GameString("CAS.package", 0x89, 4); break;
    case 1: outfit = S(Str::LookOutfitNaked); break;
    case 2: outfit = game::GameString("CAS.package", 0x89, 8); break;
    case 3: outfit = game::GameString("CAS.package", 0x89, 9); break;
    case 4: outfit = game::GameString("CAS.package", 0x89, 5); break;
    case 5: outfit = game::GameString("CAS.package", 0x89, 7); break;
    case 8: outfit = S(Str::LookOutfitMaternity); break;
    case 11: outfit = game::GameString("CAS.package", 0x89, 6); break;
    case 13: outfit = game::GameString("CAS.package", 0x9b, 2); break;
    }
    labeled(game::GameString("CAS.package", 0x89, 2), outfit);
    return JoinWords(parts);
}

void AddThings(Group& g, const std::vector<LotObject>& list, const LotObject* sim, bool roomMode,
               const std::vector<LotObject>& all) {
    for (const LotObject& o : list) {
        Item it;
        it.role = Role::LotObject;
        it.number = o.id;
        it.label = Line(o, sim, roomMode, all);
        if (!o.person && o.price > 0) it.detail = util::Money(o.price);
        if (o.person) it.detail = LookWords(o.id);  // what the Sim looks like (Control+Up)
        g.items.push_back(it);
    }
}

// The things of a kind or a room, nearest to the selected Sim first, as items.
Group ThingsGroup(Ptr key, const std::string& title, bool roomMode, int category, int level, int room) {
    Group g;
    g.key = key;
    g.name = title;
    LotObject simObj;
    const LotObject* sim = SimPosition(&simObj) ? &simObj : nullptr;
    std::vector<LotObject> all = game::LotObjects();
    std::vector<LotObject> list;
    for (const LotObject& o : all)
        if (Listed(o) && (roomMode ? (o.level == level && o.room == room) : InCategory(o, category))) list.push_back(o);
    SortNearest(list, sim);
    AddThings(g, list, sim, roomMode, all);
    return g;
}

// Ctrl+F: what the search finds: names starting with the text first, then with a word starting
// with it, then containing it; within each the nearest first.
std::string g_findFolded;  // the text, folded (util::FoldForSearch)
std::string g_findLabel;   // ... as it was typed

std::vector<LotObject> FindMatches(const std::vector<LotObject>& all, const LotObject* sim, const std::string& folded) {
    std::vector<LotObject> ranked[3];
    for (const LotObject& o : all) {
        if (!Listed(o)) continue;
        std::string name = util::FoldForSearch(DisplayName(o));
        size_t at = name.find(folded);
        if (at == std::string::npos) continue;
        ranked[at == 0 ? 0 : name.find(" " + folded) != std::string::npos ? 1 : 2].push_back(o);
    }
    std::vector<LotObject> out;
    for (auto& r : ranked) {
        SortNearest(r, sim);
        out.insert(out.end(), r.begin(), r.end());
    }
    return out;
}

Group FindGroup() {
    Group g;
    g.key = kKindsGroup;
    g.name = util::Format(S(Str::FindResults), g_findLabel.c_str());
    LotObject simObj;
    const LotObject* sim = SimPosition(&simObj) ? &simObj : nullptr;
    std::vector<LotObject> all = game::LotObjects();
    AddThings(g, FindMatches(all, sim, g_findFolded), sim, false, all);
    return g;
}

}  // namespace

int SetFindQuery(const std::string& utf8) {
    LotObject simObj;
    const LotObject* sim = SimPosition(&simObj) ? &simObj : nullptr;
    std::string folded = util::FoldForSearch(utf8);
    int count = (int)FindMatches(game::LotObjects(), sim, folded).size();
    if (count > 0) {  // with none, the list stays as it was
        g_findFolded = folded;
        g_findLabel = utf8;
    }
    return count;
}

Group KindsGroup() {
    if (g_openKind == kFindKind) return FindGroup();
    if (g_openKind > 0) return ThingsGroup(kKindsGroup, CategoryName(g_openKind - 1), false, g_openKind - 1, 0, 0);
    Group g;
    g.key = kKindsGroup;
    g.name = S(Str::ThingsByKind);
    std::vector<LotObject> all = game::LotObjects();
    for (int c = 0; c < kCategoryCount; c++) {
        int n = 0;
        for (const LotObject& o : all)
            if (Listed(o) && InCategory(o, c)) n++;
        if (!n) continue;
        Item it;
        it.role = Role::Level;
        it.number = c + 1;
        it.label = n == 1 ? util::Format(S(Str::GroupSummaryOne), CategoryName(c).c_str())
                          : util::Format(S(Str::GroupSummary), CategoryName(c).c_str(), n);
        g.items.push_back(it);
    }
    return g;
}

Group RoomsGroup() {
    std::vector<LotObject> all = game::LotObjects();
    if (g_openRoom > 0) {
        int level = (g_openRoom - 1) / 10000, room = (g_openRoom - 1) % 10000;
        return ThingsGroup(kRoomsGroup, RoomName(level, room, all), true, 0, level, room);
    }
    Group g;
    g.key = kRoomsGroup;
    g.name = S(Str::ThingsByRoom);
    LotObject simObj;
    const LotObject* sim = SimPosition(&simObj) ? &simObj : nullptr;
    for (const auto& r : ListedRooms(all, sim)) {
        int n = 0;
        for (const LotObject& o : all)
            if (Listed(o) && o.level == r.first && o.room == r.second) n++;
        std::string name = RoomName(r.first, r.second, all) + ", " +
                           util::Format(S(Str::FloorNumber), std::max(r.first, 1));
        Item it;
        it.role = Role::Level;
        it.number = RoomNumber(r.first, r.second);
        it.label = n == 1 ? util::Format(S(Str::RoomObjectsOne), name.c_str())
                          : util::Format(S(Str::RoomObjects), name.c_str(), n);
        g.items.push_back(it);
    }
    return g;
}

void Open(Ptr group, int number) {
    g_lastGroup = group;
    if (group == kKindsGroup) g_openKind = number;
    if (group == kRoomsGroup) g_openRoom = number;
}

int Step(Ptr group, int delta) {
    std::vector<LotObject> all = game::LotObjects();
    std::vector<int> numbers;  // the kinds or rooms that have things, in their list's order
    if (group == kKindsGroup) {
        for (int c = 0; c < kCategoryCount; c++)
            for (const LotObject& o : all)
                if (Listed(o) && InCategory(o, c)) {
                    numbers.push_back(c + 1);
                    break;
                }
    } else {
        LotObject simObj;
        for (const auto& r : ListedRooms(all, SimPosition(&simObj) ? &simObj : nullptr))
            numbers.push_back(RoomNumber(r.first, r.second));
    }
    if (numbers.empty()) return 0;
    int open = group == kKindsGroup ? g_openKind : g_openRoom;
    int n = (int)numbers.size(), at = -1;
    for (int i = 0; i < n; i++)
        if (numbers[i] == open) at = i;
    // From the list itself: the first kind (the last going back), or the selected Sim's room.
    if (at < 0) return group == kRoomsGroup || delta > 0 ? numbers.front() : numbers.back();
    return numbers[((at + delta) % n + n) % n];
}

int OpenNumber(Ptr group) { return group == kKindsGroup ? g_openKind : group == kRoomsGroup ? g_openRoom : 0; }

Ptr LastGroup() { return g_lastGroup; }

int FocusedId() { return g_focusId; }

int Close(Ptr group) {
    int* open = group == kKindsGroup ? &g_openKind : group == kRoomsGroup ? &g_openRoom : nullptr;
    if (!open || !*open) return 0;
    int was = *open;
    *open = 0;
    return was;
}

void Focus(int objectId) { g_focusId = objectId; }

bool HasFocus() { return g_focusId >= 0; }

void Interact() {
    LotObject o;
    if (g_focusId < 0 || !game::ReadLotObject(g_focusId, &o)) {
        speech::Say(S(Str::NoObjects));
        return;
    }
    // Buy mode, and build mode's hand: Enter picks the object up to move (or sell) it.
    std::string mode = game::CurrentModeClass();
    if (mode == "cTSUIModeBuy" || (mode == "cTSUIModeBuild" && game::IsKindOf(game::CurrentTool(), "cTSUserToolObject"))) {
        if (o.person) {
            speech::Say(util::Format(S(Str::PlaceCannotPick), DisplayName(o).c_str()));
            return;
        }
        placement::BeginMove(o.ptr, o.guid, DisplayName(o), o.x, o.y, o.level);
        return;
    }
    Log("interactions of object %d (%s)", o.id, DisplayName(o).c_str());
    if (game::ShowInteractions(o.id)) g_menuAskedAt = GetTickCount64();
}

bool RemoveFocusedPortal() {
    LotObject o;
    if (g_focusId < 0 || !game::ReadLotObject(g_focusId, &o) || (o.type != 8 && o.type != 9)) return false;
    std::string mode = game::CurrentModeClass();
    if (mode != "cTSUIModeBuy" && !(mode == "cTSUIModeBuild" && game::IsKindOf(game::CurrentTool(), "cTSUserToolObject"))) {
        speech::Say(S(Str::PortalNeedsHand));
        return true;
    }
    placement::BeginRemove(o.ptr, o.guid, DisplayName(o), o.x, o.y, o.level);
    return true;
}

void WalkTo(bool run) {
    LotObject o;
    if (g_focusId < 0 || !game::ReadLotObject(g_focusId, &o) || o.x < 0) {
        speech::Say(S(Str::NoObjects));
        return;
    }
    if (!game::SelectedSimId()) {
        speech::Say(S(Str::NoSimSelected));
        return;
    }
    // A click on the object's tile with nothing under the cursor: the game offers "go here" and
    // "run here" (and routes to the nearest place it can reach). The click waits for Ctrl to be
    // released: the game tracks it from the key messages, and a Ctrl+click opens no menu.
    Log("walk to %s at %.1f,%.1f floor %d", DisplayName(o).c_str(), o.x, o.y, o.level);
    game::SetPickOverride(true, o.x, o.y, o.level);
    game::ForceNoObjectPick();
    g_walkClickPending = true;
    g_walkAskedAt = GetTickCount64();
    g_walkRun = run;
}

bool IsListed(int objectId) {
    LotObject o;
    return game::ReadLotObject(objectId, &o) && Listed(o);
}

std::vector<LotObject> ThingsAt(int x, int y, int level) {
    std::vector<LotObject> out;
    std::vector<int> seen;
    for (const game::PlacedPart& p : game::PlacedParts()) {
        if (p.level != level || p.tileX != x || p.tileY != y || !p.lead || p.type == 8 || p.type == 9) continue;
        if (std::find(seen.begin(), seen.end(), p.lead) != seen.end()) continue;
        seen.push_back(p.lead);
        LotObject o;
        if (game::ReadLotObject(p.lead, &o) && Listed(o)) out.push_back(o);
    }
    std::stable_partition(out.begin(), out.end(), [](const LotObject& o) { return o.person; });
    return out;
}

void WalkToSpot(float x, float y, int level, bool run) {
    if (!game::SelectedSimId()) {
        speech::Say(S(Str::NoSimSelected));
        return;
    }
    Log("walk to the spot %.1f,%.1f floor %d", x, y, level);
    game::SetPickOverride(true, x, y, level);
    game::ForceNoObjectPick();
    g_walkClickPending = true;
    g_walkAskedAt = GetTickCount64();
    g_walkRun = run;
}

void Tick() {
    if (g_walkClickPending) {
        if (game::ModifierHeld(VK_CONTROL) || game::ModifierHeld(VK_SHIFT)) {
            if (GetTickCount64() - g_walkAskedAt < 5000) return;
        }
        g_walkClickPending = false;
        game::MouseClick(1280, 640);
        g_walkAskedAt = GetTickCount64();
    } else if (g_walkAskedAt) {
        Ptr modal = game::ModalWindow();
        bool menu = modal && game::IsKindOf(modal, "cTSWinPieMenu");
        if (menu || GetTickCount64() - g_walkAskedAt > 1500) {
            game::SetPickOverride(false);
            game::ClearObjectPick();
            g_walkAskedAt = 0;
            Group pie = menu ? PieGroup(modal) : Group();
            size_t choice = g_walkRun ? 1 : 0;
            if (pie.items.size() > choice) {
                Log("walk: choosing \"%s\"", pie.items[choice].label.c_str());
                game::ClickWindow(pie.items[choice].widget);
                speech::Say(S(Str::WalkingThere));
            } else {
                speech::Say(S(Str::CannotWalk));
            }
        }
    }
    if (!g_menuAskedAt) return;
    Ptr modal = game::ModalWindow();
    if (modal && game::IsKindOf(modal, "cTSWinPieMenu")) {
        g_menuAskedAt = 0;
        return;
    }
    if (GetTickCount64() - g_menuAskedAt > 1500) {
        g_menuAskedAt = 0;
        speech::Say(S(Str::NoInteractions));
    }
}

}  // namespace nav::world
