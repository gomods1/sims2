// How items and groups read.
#pragma once

#include <string>
#include <vector>

#include "model.h"

namespace nav {

std::string Describe(const Item& it);          // "Salir, botón"; toggles add their state
std::string GroupName(const Group& g);
// While set, build mode's catalog is named after the tool that offers it ("Puertas": its styles).
void SetCatalogName(const std::string& name);         // a title found in the group, else its kind
std::string GroupSummary(const Group& g);      // "botones, 3 elementos"
std::vector<std::string> DetailLines(const Item& it, const Group& g, int index);

}  // namespace nav
