#include "describe.h"

#include "../game/gz.h"
#include "../util/strings.h"
#include "../util/text.h"
#include "buffers.h"
#include "nhood_view.h"

namespace nav {

std::string Describe(const Item& it) {
    std::string label = it.label.empty() ? S(Str::UnlabeledButton) : it.label;
    switch (it.role) {
    case Role::Text: return label;
    case Role::Lot: return label;
    case Role::PieItem: return label;
    case Role::QueuedAction: return label + ", " + S(Str::RoleButton);
    case Role::ListRow:
        return it.on ? label + ", " + S(Str::Selected) : label;
    case Role::Slider:
        return label + ", " + S(Str::RoleSlider) + ", " + util::Format(S(Str::ValueOfMax), it.step, it.steps);
    case Role::Product: return label + ", " + S(Str::RoleButton) + (it.value.empty() ? "" : ", " + it.value);
    case Role::TextField: return util::Format(S(Str::TextField), label.c_str(), "");
    case Role::Toggle: {
        std::string s = label + ", " + S(Str::RoleButton);
        if (game::ButtonOn(it.widget)) s += std::string(", ") + S(Str::Selected);
        if (!game::Enabled(it.widget)) s += std::string(", ") + S(Str::Unavailable);
        return s;
    }
    case Role::Button:
    default: {
        std::string s = label + ", " + S(Str::RoleButton);
        if (!game::Enabled(it.widget)) s += std::string(", ") + S(Str::Unavailable);
        return s;
    }
    }
}

std::string GroupName(const Group& g) {
    if (!g.name.empty()) return g.name;
    // Panels known by their proc.
    static const struct {
        const char* proc;
        Str name;
    } kPanels[] = {
        {"cTSWinProcUCP", Str::GroupControlPanel},
        {"cTSWinProcLivePanel", Str::GroupLivePanel},
        {"cTSWinProcSimChooser", Str::GroupFamily},
        {"cTSWinProcPanelWantsAndFears", Str::GroupWantsFears},
        {"cTSWinProcPanelMotives", Str::GroupNeeds},
        {"cTSWinProcNeighborhoodUCP", Str::GroupControlPanel},
        {"cTSWinProcBuyPanel", Str::GroupBuyPanel},
        {"cTSWinProcBuildPanel", Str::GroupBuildPanel},
        {"cTSWinProcCatalog", Str::GroupCatalog},
    };
    for (const auto& p : kPanels)
        if (g.proc == p.proc) return S(p.name);
    // Containers known by their design-time caption (Create a Sim has no panel procs).
    static const struct {
        const char* caption;
        Str name;
    } kContainers[] = {
        {"Zodiac", Str::GroupStarSign},   {"TAE Aspirations", Str::GroupAspiration},
        {"full wizard", Str::GroupSteps}, {"Age", Str::GroupAge},
        {"Gender", Str::GroupSex},        {"CAS PUC", Str::GroupControlPanel},
    };
    if (g.key > 0x10000) {
        std::string caption = game::Caption(g.key);
        for (const auto& c : kContainers)
            if (caption == c.caption) return S(c.name);
    }
    for (const Item& it : g.items)
        if (it.role == Role::Text && it.label.size() < 60) return it.label;
    return S(IsTextOnly(g) ? Str::GroupText : Str::GroupButtons);
}

std::string GroupSummary(const Group& g) {
    std::string name = GroupName(g);
    return g.items.size() == 1 ? util::Format(S(Str::GroupSummaryOne), name.c_str())
                               : util::Format(S(Str::GroupSummary), name.c_str(), (int)g.items.size());
}

std::vector<std::string> DetailLines(const Item& it, const Group& g, int index) {
    std::vector<std::string> lines;
    if (it.role == Role::Lot) {
        lines = neighborhood::LotDetails(it);
        lines.push_back(util::Format(S(Str::PositionInGroup), index + 1, (int)g.items.size(), GroupName(g).c_str()));
        return lines;
    }
    lines.push_back(Describe(it));
    for (const std::string& s : buffers::Sentences(it.detail)) lines.push_back(s);
    lines.push_back(util::Format(S(Str::PositionInGroup), index + 1, (int)g.items.size(), GroupName(g).c_str()));
    return lines;
}

}  // namespace nav
