#include "nhood_view.h"

#include <algorithm>
#include <map>

#include "../game/gz.h"
#include "../game/neighborhood.h"
#include "../game/rtti.h"
#include "../util/strings.h"
#include "../util/text.h"
#include "buffers.h"

namespace nav::neighborhood {
namespace {

using game::FamilyEntry;
using game::LotEntry;
using game::LotType;

bool IsLotType(const LotEntry& l, LotType t) { return l.type == (int)t; }

// The families living on a lot: on it, or in an apartment unit of the building.
std::vector<FamilyEntry> Residents(const LotEntry& lot, const std::vector<LotEntry>& lots,
                                   const std::vector<FamilyEntry>& families) {
    std::vector<FamilyEntry> out;
    for (const FamilyEntry& f : families) {
        if (!f.lot) continue;
        if (f.lot == lot.id) out.push_back(f);
        else if (IsLotType(lot, LotType::ApartmentBuilding))
            for (const LotEntry& unit : lots)
                if (unit.id == f.lot && IsLotType(unit, LotType::ApartmentUnit) && unit.x == lot.x && unit.y == lot.y)
                    out.push_back(f);
    }
    return out;
}

std::string FamilyNames(const std::vector<FamilyEntry>& fams) {
    std::string out;
    for (const FamilyEntry& f : fams) {
        if (!out.empty()) out += ", ";
        out += util::Format(S(Str::LotFamily), f.name.c_str());
    }
    return out;
}

std::string Status(const LotEntry& lot, const std::vector<FamilyEntry>& residents) {
    if (IsLotType(lot, LotType::ApartmentBuilding))
        return residents.empty() ? S(Str::LotApartmentsEmpty)
                                 : util::Format(S(Str::LotApartmentsWith), FamilyNames(residents).c_str());
    if (IsLotType(lot, LotType::Community)) return S(Str::LotCommunity);
    if (!residents.empty()) return FamilyNames(residents);
    return S(Str::LotForSale);
}

bool HasProcShown(Ptr w, const char* procClass, int depth) {
    if (!w || depth > 12 || !game::Visible(w)) return false;
    if (game::IsKindOf(game::Rd<Ptr>(w + 0x12c), procClass)) return true;
    for (Ptr c : game::Children(w))
        if (HasProcShown(c, procClass, depth + 1)) return true;
    return false;
}

}  // namespace

bool ViewActive() { return HasProcShown(game::MainWindow(), "cTSWinProcNeighborhoodUCP", 0); }

Group LotsGroup() {
    Group g;
    g.key = kLotsGroup;
    g.name = S(Str::GroupLots);
    std::vector<LotEntry> lots = game::Lots();
    std::vector<FamilyEntry> families = game::Families();
    struct Row {
        Item item;
        bool home;
        std::string sortKey;
    };
    std::vector<Row> rows;
    for (const LotEntry& lot : lots) {
        if (IsLotType(lot, LotType::ApartmentUnit) || lot.name.empty()) continue;
        std::vector<FamilyEntry> residents = Residents(lot, lots, families);
        Row r;
        r.item.role = Role::Lot;
        r.item.number = lot.id;
        r.item.family = residents.empty() ? 0 : residents[0].id;
        r.item.label = lot.name + ", " + Status(lot, residents);
        r.home = !residents.empty();
        r.sortKey = r.home ? residents[0].name : lot.name;
        rows.push_back(r);
    }
    std::stable_sort(rows.begin(), rows.end(), [](const Row& a, const Row& b) {
        if (a.home != b.home) return a.home;
        return a.sortKey < b.sortKey;
    });
    for (const Row& r : rows) g.items.push_back(r.item);
    return g;
}

std::vector<std::string> LotDetails(const Item& it) {
    std::vector<std::string> lines;
    std::vector<LotEntry> lots = game::Lots();
    std::vector<FamilyEntry> families = game::Families();
    for (const LotEntry& lot : lots) {
        if (lot.id != it.number) continue;
        lines.push_back(it.label);
        for (const FamilyEntry& f : Residents(lot, lots, families)) {
            lines.push_back(util::Format(S(Str::FamilySummary), f.name.c_str(), f.members, util::Money(f.funds).c_str()));
            for (const std::string& s : buffers::Sentences(f.description)) lines.push_back(s);
        }
    }
    return lines;
}

void OpenLot(const Item& it) { game::ShowLotInfo(it.number, it.family, 0); }

}  // namespace nav::neighborhood
