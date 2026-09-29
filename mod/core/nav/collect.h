// Builds the navigable model of what is on screen from the live window tree.
#pragma once

#include <vector>

#include "model.h"

namespace nav {

// The window that owns input: the innermost modal dialog, else the main window.
Ptr InputRoot();

// Every navigable item under root (visible windows only), grouped by the panel or container
// that holds it; groups in the window tree's order, items inside a group top to bottom, left to
// right (NAVIGATION.md: position never groups nor orders the groups).
std::vector<Group> Collect(Ptr root);

// The entries of an interaction (pie) menu window, in the game's order.
Group PieGroup(Ptr menu);

}  // namespace nav
