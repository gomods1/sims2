#include "collect.h"

#include <algorithm>
#include <map>

#include "../game/gz.h"
#include "../game/hooks.h"
#include "../game/rtti.h"
#include "../util/text.h"

namespace nav {
namespace {

// "Title|Description" -> title, description. Tooltips of the Sims 2 UI scripts use this form.
void SplitTip(const std::string& tip, std::string* title, std::string* rest) {
    size_t bar = tip.find('|');
    *title = util::CleanForSpeech(bar == std::string::npos ? tip : tip.substr(0, bar));
    *rest = bar == std::string::npos ? "" : util::CleanForSpeech(tip.substr(bar + 1));
}

bool Overlaps(const RECT& a, const RECT& b) {
    RECT r;
    return IntersectRect(&r, &a, &b) != 0;
}

struct Walker {
    std::map<Ptr, Group> groups;
    std::vector<Ptr> order;  // group keys in discovery order

    void Add(Ptr groupKey, Item it) {
        auto found = groups.find(groupKey);
        if (found == groups.end()) {
            order.push_back(groupKey);
            found = groups.emplace(groupKey, Group{groupKey, {}, {}}).first;
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

    void Walk(Ptr w, int depth) {
        if (depth > 40) return;
        std::vector<Ptr> kids = game::Children(w);
        for (Ptr k : kids) {
            if (!game::Visible(k)) continue;
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
        if (game::IsButton(k)) {
            std::string title, rest;
            SplitTip(game::Tooltip(k), &title, &rest);
            it.label = title.empty() ? util::CleanForSpeech(game::Caption(k)) : title;
            it.detail = rest;
            it.role = game::ButtonToggles(k) ? Role::Toggle : Role::Button;
            if (it.label.empty()) return;  // icon without a name: logged by the navigator later
            Add(parent, it);
        } else if (game::IsTextEdit(k)) {
            it.label = util::CleanForSpeech(game::TextEditText(k));
            it.role = game::FocusWindow() == k ? Role::TextField : Role::Text;
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

}  // namespace

__declspec(noinline) int FirstControlOrNone(const Group& g) {
    const Item* items = g.items.data();
    size_t n = g.items.size();
    for (size_t i = 0; i < n; i++)
        if (items[i].role != Role::Text) return (int)i;
    return -1;
}

Ptr InputRoot() {
    const auto& modals = game::ModalStack();
    if (!modals.empty()) return modals.back();
    return game::MainWindow();
}

std::vector<Group> Collect(Ptr root) {
    std::vector<Group> out;
    if (!root || !game::Visible(root)) return out;
    Walker walker;
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
        g.bounds = g.items[0].rect;
        for (const Item& it : g.items) UnionRect(&g.bounds, &g.bounds, &it.rect);
        out.push_back(std::move(g));
    }
    std::stable_sort(out.begin(), out.end(), [](const Group& a, const Group& b) {
        if (a.bounds.top / 16 != b.bounds.top / 16) return a.bounds.top < b.bounds.top;
        return a.bounds.left < b.bounds.left;
    });
    return out;
}

}  // namespace nav
