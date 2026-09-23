// Builds the navigable model of what is on screen from the live window tree.
#pragma once

#include <vector>

#include "model.h"

namespace nav {

// The window that owns input: the innermost modal dialog, else the main window.
Ptr InputRoot();

// Every navigable item under root (visible windows only), grouped by container and ordered top
// to bottom, left to right.
std::vector<Group> Collect(Ptr root);

}  // namespace nav
