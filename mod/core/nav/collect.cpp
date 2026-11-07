#include "collect.h"

#include <algorithm>
#include <map>

#include "../game/buy.h"
#include "../game/gz.h"
#include "../game/hooks.h"
#include "../game/rtti.h"
#include "../util/strings.h"
#include "../util/text.h"
#include "live.h"
#include "nhood_view.h"
#include "world.h"

namespace nav {
namespace {

// "Title|Description" -> title, description. Tooltips of the Sims 2 UI scripts use this form.
void SplitTip(const std::string& tip, std::string* title, std::string* rest) {
    size_t bar = tip.find('|');
    *title = util::CleanForSpeech(bar == std::string::npos ? tip : tip.substr(0, bar));
    *rest = bar == std::string::npos ? "" : util::CleanForSpeech(tip.substr(bar + 1));
    // Some tooltips carry resource ids after the text ("Moda||0xab203135||0x4906502e").
    size_t ids = rest->find("0x");
    if (ids != std::string::npos) rest->erase(ids);
    while (!rest->empty() && (rest->back() == '|' || rest->back() == ' ')) rest->pop_back();
    while (!rest->empty() && (rest->front() == '|' || rest->front() == ' ')) rest->erase(0, 1);
}

bool Overlaps(const RECT& a, const RECT& b) {
    RECT r;
    return IntersectRect(&r, &a, &b) != 0;
}

// Procs that make a window a panel of its own: everything else (thumbnails, meters, the scene
// view, the main window) groups with the panel around it.
bool IsPanelProc(const std::string& proc) {
    if (proc.empty()) return false;
    static const char* const kNotPanels[] = {"Thumbnail", "FillBar", "Dummy", "SceneGraph", "cTSWinProcMain",
                                             "MotiveBar", "Grid"};
    for (const char* k : kNotPanels)
        if (proc.find(k) != std::string::npos) return false;
    return true;
}

// A container that wraps a single control (a turn-on slot's frame): its control belongs with
// the windows around the container.
bool Wrapper(Ptr w) {
    int controls = 0;
    for (Ptr c : game::Children(w))
        if (game::Visible(c) && (game::IsButton(c) || game::IsText(c) || game::IsTextEdit(c))) controls++;
    return controls == 1;
}

// The group an item belongs to: the nearest panel around it, else its container (past wrappers).
Ptr GroupOf(Ptr parent, Ptr root) {
    int depth = 0;
    for (Ptr p = parent; p && depth < 40; p = game::Parent(p), depth++) {
        if (IsPanelProc(game::ProcClass(p))) return p;
        if (p == root) break;
    }
    Ptr p = parent;
    while (p != root && Wrapper(p) && game::Parent(p) && !game::ProcClass(p).size()) p = game::Parent(p);
    return p;
}

// A visible text or text field next to w shows this text.
bool SiblingShows(Ptr w, const std::string& text) {
    if (text.empty()) return false;
    for (Ptr s : game::Children(game::Parent(w))) {
        if (s == w || !game::Visible(s)) continue;
        std::string t = game::IsTextEdit(s) ? game::TextEditText(s) : game::IsText(s) ? game::Caption(s) : "";
        if (util::CleanForSpeech(t) == text) return true;
    }
    return false;
}

// The trait names on a step bar's row (personality bars have two), empty for a plain meter.
std::string StepBarLabels(Ptr bar) {
    RECT r = game::Area(bar);
    std::string left, right;
    for (Ptr s : game::Children(game::Parent(bar))) {
        if (!game::Visible(s) || !game::IsKindOf(s, "cTSWinTextLabel")) continue;
        RECT l = game::Area(s);
        if (l.bottom <= r.top || l.top >= r.bottom) continue;
        if (l.right <= r.left + 4) left = game::Caption(s);
        else if (l.left >= r.right - 4) right = game::Caption(s);
    }
    return left.empty() || right.empty() ? "" : left + right;
}

// The meter a label names: the step bar on its row, else the only one beside it (personality bars,
// named by two labels, are not meters). On the job panel's skills each 16-pixel label overlaps two
// 14-pixel rows of bars: the row it overlaps most is its own (the last one overlapping gave each
// skill its neighbour's value).
Ptr MeterFor(Ptr label) {
    RECT l = game::Area(label);
    Ptr sameRow = 0, only = 0;
    int count = 0, bestOverlap = 0;
    for (Ptr s : game::Children(game::Parent(label))) {
        int step, steps;
        if (s == label || !game::Visible(s) || !game::StepBar(s, &step, &steps) || !StepBarLabels(s).empty()) continue;
        count++;
        only = s;
        RECT b = game::Area(s);
        int overlap = std::min(l.bottom, b.bottom) - std::max(l.top, b.top);
        if (overlap > bestOverlap) {
            bestOverlap = overlap;
            sameRow = s;
        }
    }
    return sameRow ? sameRow : count == 1 ? only : 0;
}

// The job panel's mark on a skill bar (its child 5, a 9-pixel bitmap): the level the next promotion
// asks for, drawn at (level - 1) tenths of the bar and hidden when it asks nothing (the panel's
// update 0xa065a98d; shown only with a job below its top level). 0 when there is none.
int PromotionNeeds(Ptr bar, int steps) {
    RECT b = game::Area(bar);
    int width = b.right - b.left;
    if (steps <= 0 || width <= 0) return 0;
    for (Ptr c : game::Children(bar)) {
        if (game::WinId(c) != 5 || !game::Visible(c)) continue;
        RECT m = game::Area(c);
        int cell = width / steps;
        return cell > 0 ? (m.left + cell / 2) / cell + 1 : 0;
    }
    return 0;
}

// The nearest text above a window, overlapping it horizontally, among the windows beside it or
// beside its container (screen coordinates); empty when none is close.
std::string HeadingAbove(Ptr w) {
    RECT r = game::ScreenRect(w);
    std::string best;
    int bestGap = 120;
    Ptr parent = game::Parent(w);
    for (Ptr scope : {parent, game::Parent(parent)}) {
        if (!scope) continue;
        for (Ptr s : game::Children(scope)) {
            if (s == w || s == parent || !game::Visible(s) || !game::IsText(s)) continue;
            RECT t = game::ScreenRect(s);
            int gap = r.top - t.bottom;
            if (gap < -4 || gap > bestGap || t.right <= r.left || t.left >= r.right) continue;
            std::string text = util::CleanForSpeech(game::Caption(s));
            if (text.empty() || text.size() > 40) continue;
            best = text;
            bestGap = gap;
        }
    }
    return best;
}

// A plain button that is the only one on among at least four alike (same flags but the on bit):
// the chosen one of a set (Create a Sim's aspiration and star sign, the options page shown).
// Plain buttons are also on for other reasons (Accept and Cancel after a press), hence the count.
bool ChosenAmongOthers(Ptr w) {
    if (!game::ButtonOn(w)) return false;
    uint32_t family = game::ButtonFlags(w) | 1;
    int alike = 0, on = 0;
    for (Ptr s : game::Children(game::Parent(w))) {
        if (!game::Visible(s) || !game::IsButton(s) || (game::ButtonFlags(s) | 1) != family) continue;
        alike++;
        if (game::ButtonOn(s)) on++;
    }
    return alike >= 4 && on == 1;
}

struct Walker {
    Ptr root = 0;
    std::map<Ptr, Group> groups;
    std::vector<Ptr> order;  // group keys in discovery order

    void Add(Ptr container, Item it) {
        Ptr groupKey = GroupOf(container, root);
        auto found = groups.find(groupKey);
        if (found == groups.end()) {
            order.push_back(groupKey);
            Group g;
            g.key = groupKey;
            g.proc = game::ProcClass(groupKey);
            found = groups.emplace(groupKey, g).first;
        }
        found->second.items.push_back(std::move(it));
    }

    // A thumbnail: a container holding one button and one name (text or text field), such as a
    // neighborhood or a family in a picker. The button reads as the name, its tooltip as detail.
    bool Thumbnail(Ptr w, const std::vector<Ptr>& kids, Ptr groupKey) {
        Ptr button = 0, name = 0;
        int buttons = 0, names = 0;
        for (Ptr k : kids) {
            if (!game::Visible(k)) continue;
            if (game::IsButton(k)) { button = k; buttons++; }
            else if (game::IsTextEdit(k) || game::IsText(k)) { name = k; names++; }
        }
        if (buttons != 1 || names != 1) return false;
        std::string text = game::IsTextEdit(name) ? game::TextEditText(name) : game::Caption(name);
        text = util::CleanForSpeech(text);
        if (text.empty() || !Overlaps(game::Area(button), game::Area(name))) return false;
        Item it;
        it.widget = button;
        it.role = Role::Button;
        it.label = text;
        std::string title, rest;
        SplitTip(game::Tooltip(button), &title, &rest);
        it.detail = rest.empty() ? title : title + ". " + rest;
        it.rect = game::ScreenRect(w);
        Add(groupKey, it);
        return true;
    }

    // A need on the needs panel (cTSWinProcMotiveBar): its label and its meter's fill.
    bool Motive(Ptr w, const std::vector<Ptr>& kids, Ptr groupKey) {
        if (game::ProcClass(w) != "cTSWinProcMotiveBar") return false;
        std::string name;
        int percent = -1;
        for (Ptr k : kids) {
            if (game::IsKindOf(k, "cTSWinTextLabel")) name = util::CleanForSpeech(game::Caption(k));
            int p;
            if (game::MeterPercent(k, &p)) percent = p;
        }
        if (name.empty() || percent < 0) return false;
        Item it;
        it.widget = w;
        it.role = Role::Text;
        it.label = util::Format(S(Str::MotiveValue), name.c_str(), percent);
        it.rect = game::ScreenRect(w);
        Add(groupKey, it);
        return true;
    }

    // A catalog product (a thumbnail of the buy mode catalog): its name, price and ratings; the
    // description goes to the details. Enter presses its button (picks it up).
    bool Product(Ptr w, const std::vector<Ptr>& kids, Ptr groupKey) {
        if (game::ProcClass(w) != "cTSWinProcCatalogThumbnail") return false;
        game::Product p;
        if (!game::ReadProduct(w, &p)) return true;
        Item it;
        it.role = Role::Product;
        for (Ptr k : kids)
            if (game::Visible(k) && game::IsButton(k)) it.widget = k;
        if (!it.widget) return true;
        it.label = util::CleanForSpeech(p.name.empty() ? p.internalName : p.name);
        it.value = util::CleanForSpeech(game::Tooltip(it.widget));
        for (int i = 0; i < game::kRatingCount; i++)
            if (p.ratings[i]) it.value += ", " + live::RatingName(i) + " " + std::to_string(p.ratings[i]);
        it.detail = util::CleanForSpeech(p.description);
        it.buyObject = p.isObject;
        it.rect = game::ScreenRect(w);
        Add(groupKey, it);
        return true;
    }

    // A grid of choices without names (Create a Sim's heads, hair, clothes: thumbnails whose
    // captions are resource keys): "3 of 6", selected or not.
    bool Grid(Ptr k, Ptr groupKey) {
        if (!game::IsKindOf(k, "cGZWinGrid")) return false;
        std::vector<Ptr> buttons;
        for (Ptr cell : game::Children(k)) {
            if (!game::Visible(cell)) continue;
            if (game::IsButton(cell)) buttons.push_back(cell);
            else
                for (Ptr c : game::Children(cell))
                    if (game::Visible(c) && game::IsButton(c)) buttons.push_back(c);
        }
        // A cell may show its name in a text field under the thumbnail (a pet's breed).
        auto cellName = [](Ptr button) -> std::string {
            Ptr cell = game::Parent(button);
            if (!cell) return "";
            for (Ptr c : game::Children(cell))
                if (game::Visible(c) && game::IsTextEdit(c)) return util::CleanForSpeech(game::TextEditText(c));
            return "";
        };
        if (buttons.empty()) return false;
        std::sort(buttons.begin(), buttons.end(), [](Ptr a, Ptr b) {
            RECT ra = game::ScreenRect(a), rb = game::ScreenRect(b);
            return ra.top / 8 != rb.top / 8 ? ra.top < rb.top : ra.left < rb.left;
        });
        for (size_t i = 0; i < buttons.size(); i++) {
            Item it;
            it.widget = buttons[i];
            it.role = Role::Toggle;
            it.rect = game::ScreenRect(buttons[i]);
            std::string title, rest;
            SplitTip(game::Tooltip(buttons[i]), &title, &rest);
            it.label = util::Format(S(Str::ValueOfMax), (int)i + 1, (int)buttons.size());
            if (title.empty()) title = cellName(buttons[i]);
            if (!title.empty()) it.label = title + ", " + it.label;
            it.detail = rest;
            Add(groupKey, it);
        }
        return true;
    }

    // A personality bar: its value, named by the two traits beside it ("de Gruñón a Cordial").
    // The "total" bar (the points left) is read with the bars, not on its own.
    bool StepBarItem(Ptr k, Ptr parent, Ptr groupKey) {
        int step, steps;
        if (!game::StepBar(k, &step, &steps)) return false;
        if (game::Caption(k) == "total") return true;
        RECT r = game::Area(k);
        std::string left, right, detail;
        for (Ptr s : game::Children(parent)) {
            if (!game::Visible(s) || !game::IsKindOf(s, "cTSWinTextLabel")) continue;
            RECT l = game::Area(s);
            if (l.bottom <= r.top || l.top >= r.bottom) continue;  // another row
            std::string title, rest;
            SplitTip(game::Tooltip(s), &title, &rest);
            if (title.empty()) title = util::CleanForSpeech(game::Caption(s));
            if (l.right <= r.left + 4) left = title;
            else if (l.left >= r.right - 4) right = title;
            util::AppendSentence(detail, rest);
        }
        // A bar without both trait names is a meter (the age's days) or a part of another bar.
        if (left.empty() || right.empty()) return true;
        // Only Create a Sim (where the points left show on a "total" bar) lets them change; the
        // Sim panel's personality page shows them read-only.
        bool editable = false;
        for (Ptr s : game::Children(parent))
            if (game::Visible(s) && game::Caption(s) == "total" && game::ProcClass(s) == "cTSWinProcStepFillBar") editable = true;
        Item it;
        it.widget = k;
        it.role = editable ? Role::Slider : Role::Text;
        it.step = step;
        it.steps = steps;
        it.label = util::Format(S(Str::SliderRange), left.c_str(), right.c_str());
        if (!editable) it.label = util::Format(S(Str::LabelValue), it.label.c_str(), util::Format(S(Str::ValueOfMax), step, steps).c_str());
        it.detail = detail;
        it.rect = game::ScreenRect(k);
        Add(groupKey, it);
        return true;
    }

    // A list box (destinations, saved games...): every row, scrolled out of view too; Enter
    // selects one (a press on its row, as a click).
    bool ListBox(Ptr k, Ptr groupKey) {
        std::vector<game::ListRow> rows;
        if (!game::ListBoxRows(k, &rows)) return false;
        for (const game::ListRow& r : rows) {
            if (r.text.empty()) continue;
            Item it;
            it.widget = r.window;
            it.role = Role::ListRow;
            it.label = util::CleanForSpeech(r.text);
            it.on = r.selected;
            it.rect = game::ScreenRect(k);  // all rows share it: the stable sort keeps their order
            Add(groupKey, it);
        }
        return true;
    }

    // A person of the relationships panel: the name (its button's tooltip) and the two numbers the
    // thumbnail shows, the daily relationship above the bars and the lifetime one below.
    bool Relationship(Ptr k, Ptr groupKey) {
        if (game::ProcClass(k) != "cTSWinProcRelationshipThumbnail") return false;
        Ptr button = 0;
        std::vector<std::pair<int, std::string>> numbers;  // by height
        for (Ptr c : game::Children(k)) {
            if (!game::Visible(c)) continue;
            if (game::IsButton(c)) button = c;
            else if (game::IsText(c)) numbers.push_back({game::Area(c).top, util::CleanForSpeech(game::Caption(c))});
        }
        if (!button) return true;
        std::sort(numbers.begin(), numbers.end());
        std::string title, rest;
        SplitTip(game::Tooltip(button), &title, &rest);
        Item it;
        it.widget = button;
        it.role = Role::Button;
        it.label = title;
        if (numbers.size() >= 2)
            it.label = util::Format(S(Str::RelationshipValues), title.c_str(), numbers[0].second.c_str(),
                                    numbers[1].second.c_str());
        it.detail = rest;
        it.rect = game::ScreenRect(k);
        Add(groupKey, it);
        return true;
    }

    // A row of unnamed choices between two trait names (a pet's personality: Genio ... Bobo,
    // cTSWinProcRadioGroup): the ends take the names, the others lie "between" them.
    bool RadioRow(Ptr k, Ptr groupKey) {
        if (game::ProcClass(k) != "cTSWinProcRadioGroup") return false;
        std::vector<Ptr> buttons;
        std::vector<std::pair<int, std::string>> names;
        for (Ptr c : game::Children(k)) {
            if (!game::Visible(c)) continue;
            if (game::IsKindOf(c, "cTSWinTextLabel")) {
                std::string title, rest;
                SplitTip(game::Tooltip(c), &title, &rest);
                if (title.empty()) title = util::CleanForSpeech(game::Caption(c));
                names.push_back({game::Area(c).left, title});
            } else if (game::IsButton(c) && game::Tooltip(c).empty()) {
                buttons.push_back(c);
            }
        }
        if (names.size() != 2 || buttons.size() < 2) return false;
        std::sort(names.begin(), names.end());
        std::sort(buttons.begin(), buttons.end(), [](Ptr a, Ptr b) { return game::Area(a).left < game::Area(b).left; });
        for (size_t i = 0; i < buttons.size(); i++) {
            Item it;
            it.widget = buttons[i];
            it.role = Role::Toggle;
            it.rect = game::ScreenRect(buttons[i]);
            if (i == 0) it.label = names[0].second;
            else if (i + 1 == buttons.size()) it.label = names[1].second;
            else it.label = util::Format(S(Str::RadioBetween), names[0].second.c_str(), names[1].second.c_str());
            Add(groupKey, it);
        }
        return true;
    }

    void Walk(Ptr w, int depth) {
        if (depth > 40) return;
        std::vector<Ptr> kids = game::Children(w);
        for (Ptr k : kids) {
            if (!game::Visible(k)) continue;
            // The needs panel repeats the lot view's own needs group (from the Sim's data).
            if (game::ProcClass(k) == "cTSWinProcPanelMotives") continue;
            std::vector<Ptr> motiveKids = game::Children(k);
            if (Motive(k, motiveKids, w)) continue;
            if (Product(k, motiveKids, w)) continue;
            if (Grid(k, w)) continue;
            if (ListBox(k, w)) continue;
            if (Relationship(k, w)) continue;
            if (RadioRow(k, w)) continue;
            if (StepBarItem(k, w, w)) continue;
            std::vector<Ptr> grandKids = game::Children(k);
            if (!grandKids.empty() && Thumbnail(k, grandKids, w)) continue;
            Visit(k, w);
            Walk(k, depth + 1);
        }
    }

    void Visit(Ptr k, Ptr parent) {
        Item it;
        it.widget = k;
        it.rect = game::ScreenRect(k);
        if (it.rect.right <= it.rect.left || it.rect.bottom <= it.rect.top) return;
        if (game::IsKindOf(k, "cTSWinTextLabel")) {
            // The trait names beside a personality bar are read with the bar.
            for (Ptr s : game::Children(parent)) {
                RECT l = game::Area(k), b = game::Area(s);
                if (game::ProcClass(s) == "cTSWinProcStepFillBar" && game::Visible(s) && l.bottom > b.top &&
                    l.top < b.bottom && !StepBarLabels(s).empty())
                    return;
            }
            // A value shown as a label (net worth, funds): its tooltip names it.
            std::string title, rest;
            SplitTip(game::Tooltip(k), &title, &rest);
            std::string value = util::CleanForSpeech(game::Caption(k));
            // A label beside a meter of steps (an interest, a skill): the meter is its value.
            if (Ptr bar = MeterFor(k)) {
                int step, steps;
                if (game::StepBar(bar, &step, &steps)) {
                    // Without a tooltip the label's own text names the meter (a hobby's enthusiasm).
                    if (title.empty()) title = value;
                    value = util::Format(S(Str::ValueOfMax), step, steps);
                    // "the next promotion needs 4" (in the Sims 1 mod a tester asked for it).
                    int needs = PromotionNeeds(bar, steps);
                    if (needs > step) value += ", " + util::Format(S(Str::SkillNeededForPromotion), needs);
                }
            }
            if (value.empty()) return;
            // A letter standing for its tooltip (the job's workdays: "L" for Lunes) reads as the word.
            if (!title.empty() && value.size() <= 2 && !MeterFor(k)) value.clear();
            it.label = title.empty() ? value : value.empty() ? title : util::Format(S(Str::LabelValue), title.c_str(), value.c_str());
            // A dimmed label (a day off among the workdays).
            if (!game::Enabled(k)) it.label += std::string(", ") + S(Str::Unavailable);
            it.detail = rest;
            it.role = Role::Text;
            Add(parent, it);
        } else if (game::IsButton(k)) {
            std::string title, rest;
            SplitTip(game::Tooltip(k), &title, &rest);
            std::string caption = util::CleanForSpeech(game::Caption(k));
            // No tooltip and a caption that is not drawn: the caption is a design-time name
            // ("influence popup / button"), unless a text beside the button shows it.
            if (title.empty() && !game::CaptionShown(k) && !SiblingShows(k, caption)) caption.clear();
            // An unnamed box under a heading ("Me pone" over the turn-on slots) takes the heading.
            if (title.empty() && caption.empty()) title = HeadingAbove(k);
            if (title.empty() && caption.empty()) return;
            it.label = util::WithoutHotkey(title.empty() ? caption : title);
            it.detail = it.label != title && !title.empty() ? title + (rest.empty() ? "" : ". " + rest) : rest;
            // A value drawn on the button (the funds, the clock): "Fondos: 83.947 simoleones".
            if (game::CaptionShown(k) && !caption.empty() && !title.empty() && caption != title)
                it.label = util::Format(S(Str::LabelValue), it.label.c_str(), caption.c_str());
            // A value drawn on a disabled button (the clock, the funds) is text to read.
            if (game::CaptionShown(k) && !game::Enabled(k)) it.role = Role::Text;
            it.role = game::ButtonToggles(k) || ChosenAmongOthers(k) ? Role::Toggle : Role::Button;
            if (it.label.empty()) return;  // icon without a name: logged by the navigator later
            Add(parent, it);
        } else if (game::IsTextEdit(k)) {
            // The cheat console ("Debug Console"): its output is read as it grows (console.cpp),
            // its input is named.
            if (game::Caption(parent) == "Debug Console") {
                RECT r = game::Area(k);
                if (r.bottom - r.top > 40) return;  // the output: read as it grows
                it.label = util::Format(S(Str::LabelValue), S(Str::CheatConsole),
                                        util::CleanForSpeech(game::TextEditText(k)).c_str());
                it.role = Role::TextField;
                Add(parent, it);
                return;
            }
            std::string raw = game::TextEditText(k);
            it.label = util::CleanForSpeech(raw);
            it.role = game::TextEditEditable(k) ? Role::TextField : Role::Text;
            // An empty field shows its prompt as its text ("Ponle un nombre"), which the first
            // key replaces: the text first seen is the field's name once typing has replaced it
            // (text typed after a real value extends it instead).
            static std::map<Ptr, std::string> s_first;
            if (it.role == Role::TextField) {
                auto first = s_first.emplace(k, it.label).first;
                const std::string& prompt = first->second;
                if (!prompt.empty() && it.label != prompt && it.label.rfind(prompt, 0) != 0)
                    it.label = util::Format(S(Str::LabelValue), prompt.c_str(),
                                            it.label.empty() ? S(Str::Blank) : it.label.c_str());
            }
            if (it.label.empty() && it.role == Role::Text) return;
            Add(parent, it);
        } else if (game::IsText(k)) {
            it.label = util::CleanForSpeech(game::Caption(k));
            if (it.label.empty()) return;
            it.role = Role::Text;
            Add(parent, it);
        }
    }
};

bool TopToBottom(const Item& a, const Item& b) {
    if (a.rect.top / 8 != b.rect.top / 8) return a.rect.top < b.rect.top;
    return a.rect.left < b.rect.left;
}

// A panel in columns with a column of read-only values (the job page: the job on the left, the
// skills on the right) reads one column after the other, not row by row across them. Columns are
// separated by vertical gaps no item crosses; the order changes only when one column holds four
// values or more, so grids of buttons (a catalog page) keep their rows.
void ColumnsFirst(std::vector<Item>& items) {
    if (items.size() < 8) return;
    std::vector<std::pair<LONG, LONG>> spans;
    for (const Item& it : items) spans.push_back({it.rect.left, it.rect.right});
    std::sort(spans.begin(), spans.end());
    std::vector<LONG> starts = {spans[0].first};  // each column's left edge
    LONG reach = spans[0].second;
    for (size_t i = 1; i < spans.size(); i++) {
        if (spans[i].first >= reach + 2) starts.push_back(spans[i].first);
        reach = std::max(reach, spans[i].second);
    }
    if (starts.size() < 2) return;
    auto column = [&starts](const Item& it) {
        int c = 0;
        while (c + 1 < (int)starts.size() && it.rect.left >= starts[c + 1]) c++;
        return c;
    };
    std::vector<int> count(starts.size()), texts(starts.size());
    for (const Item& it : items) {
        count[column(it)]++;
        if (it.role == Role::Text) texts[column(it)]++;
    }
    bool values = false;
    for (size_t c = 0; c < starts.size(); c++)
        if (count[c] >= 4 && texts[c] == count[c]) values = true;
    if (!values) return;
    std::stable_sort(items.begin(), items.end(), [&column](const Item& a, const Item& b) {
        int ca = column(a), cb = column(b);
        if (ca != cb) return ca < cb;
        return TopToBottom(a, b);
    });
}

}  // namespace

__declspec(noinline) int FirstControlOrNone(const Group& g) {
    const Item* items = g.items.data();
    size_t n = g.items.size();
    for (size_t i = 0; i < n; i++)
        if (items[i].role != Role::Text) return (int)i;
    return -1;
}

Ptr InputRoot() {
    Ptr modal = game::ModalWindow();
    if (modal && game::Visible(modal)) return modal;
    return game::MainWindow();
}

// The interaction menu: its entries (cPieMenuItemWinProc windows, ids 1000 + n in the game's
// order), each named by its text.
Group PieGroup(Ptr menu) {
    Group g;
    g.key = kPieGroup;
    g.name = S(Str::PieMenu);
    std::vector<std::pair<uint32_t, Item>> entries;
    for (Ptr k : game::Children(menu)) {
        if (!game::Visible(k) || game::ProcClass(k) != "cPieMenuItemWinProc") continue;
        Item it;
        it.widget = k;
        it.role = Role::PieItem;
        it.rect = game::ScreenRect(k);
        for (Ptr c : game::Children(k))
            if (game::IsText(c)) it.label = util::CleanForSpeech(game::Caption(c));
        if (!it.label.empty()) entries.push_back({game::WinId(k), it});
    }
    std::sort(entries.begin(), entries.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
    for (auto& e : entries) g.items.push_back(e.second);
    return g;
}

std::vector<Group> Collect(Ptr root) {
    std::vector<Group> out;
    if (!root || !game::Visible(root)) return out;
    if (game::IsKindOf(root, "cTSWinPieMenu")) {
        Group pie = PieGroup(root);
        if (!pie.items.empty()) out.push_back(std::move(pie));
        return out;
    }
    Walker walker;
    walker.root = root;
    walker.Walk(root, 0);
    for (Ptr key : walker.order) {
        Group g = walker.groups[key];
        // A text that only repeats a button's label ("Aprender a jugar" under its button).
        std::vector<Item> kept;
        for (const Item& it : g.items) {
            bool duplicate = false;
            if (it.role == Role::Text)
                for (const Item& other : g.items)
                    if (&other != &it && other.role != Role::Text && other.label == it.label) duplicate = true;
            if (!duplicate) kept.push_back(it);
        }
        g.items = kept;
        if (g.items.empty()) continue;
        std::stable_sort(g.items.begin(), g.items.end(), TopToBottom);
        ColumnsFirst(g.items);
        // A text the game wrapped over several windows (the next piece starts in lowercase) is one item.
        std::vector<Item> merged;
        for (const Item& it : g.items) {
            if (!merged.empty() && it.role == Role::Text && merged.back().role == Role::Text && !it.steps &&
                !merged.back().steps && util::StartsLowercase(it.label)) {
                util::AppendSentence(merged.back().label, it.label);
                UnionRect(&merged.back().rect, &merged.back().rect, &it.rect);
            } else {
                merged.push_back(it);
            }
        }
        g.items = merged;
        // Controls that share a label (four "Tono de piel" swatches, two body shapes) are told
        // apart by their order: "Tono de piel 2".
        std::map<std::string, int> counts, seen;
        for (const Item& it : g.items)
            if (it.role != Role::Text) counts[it.label]++;
        for (Item& it : g.items)
            if (it.role != Role::Text && counts[it.label] > 1) it.label += " " + std::to_string(++seen[it.label]);
        g.bounds = g.items[0].rect;
        for (const Item& it : g.items) UnionRect(&g.bounds, &g.bounds, &it.rect);
        out.push_back(std::move(g));
    }
    // The groups stay in the order the game built its windows (walker.order: the window tree),
    // never sorted by where they are drawn (NAVIGATION.md); position only orders the items
    // inside one group (above).
    // The lot view: the selected Sim's needs and action queue first (from the Sim's data), then
    // the lot's things by kind and by room.
    if (root == game::MainWindow() && world::LotViewActive()) {
        // The camera's zoom and facing, with the clock and the funds.
        std::string camera = live::CameraLine();
        for (Group& g : out) {
            if (g.proc != "cTSWinProcUCP" || camera.empty()) continue;
            Item it;
            it.role = Role::Text;
            it.number = 1;  // a virtual item is found again by its number
            it.label = camera;
            g.items.push_back(std::move(it));
            break;
        }
        Group rooms = world::RoomsGroup();
        if (!rooms.items.empty()) out.insert(out.begin(), std::move(rooms));
        Group kinds = world::KindsGroup();
        if (!kinds.items.empty()) out.insert(out.begin(), std::move(kinds));
        Group queue = live::QueueGroup();
        if (!queue.items.empty()) out.insert(out.begin(), std::move(queue));
        Group needs = live::NeedsGroup();
        if (!needs.items.empty()) out.insert(out.begin(), std::move(needs));
    }
    // The neighborhood view: its lots first (the map is 3D, the houses are no windows).
    if (root == game::MainWindow() && neighborhood::ViewActive()) {
        Group lots = neighborhood::LotsGroup();
        if (!lots.items.empty()) out.insert(out.begin(), std::move(lots));
    }
    // The main screen ends with the mod's help: this screen's keys (Control+F1) and the manual
    // (Control+Shift+F1). Not on dialogs (their text and buttons are all there is to them) nor on
    // a screen whose windows are not there yet.
    if (root == game::MainWindow() && !out.empty()) {
        Group help;
        help.key = kModHelpGroup;
        help.name = S(Str::GroupModHelp);
        for (int n : {1, 2}) {  // never 0: a virtual item is found again by its number
            Item it;
            it.role = Role::ModManual;
            it.number = n;
            it.label = S(n == 1 ? Str::KeysButton : Str::ManualButton);
            help.items.push_back(std::move(it));
        }
        out.push_back(std::move(help));
    }
    return out;
}

}  // namespace nav
