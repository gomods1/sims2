// The game's own texts, read from its .package files (DBPF 1.x, RefPack-compressed resources,
// STR# string tables with a language byte per entry), in the language the game runs in. For
// labels the game shows but keeps nowhere the mod can reach at runtime (buy mode categories).
#pragma once

#include <cstdint>
#include <string>

namespace game {

// The STR# language code of the game's language (1 English, 6 Spanish, 13 Russian, 35 Brazilian
// Portuguese...), from Steam's language number (util/language.h).
int StringLanguageCode();

// A floor's "surfacetype/subsort" ("cpet/carpet", "marble/stone", "lino/tile"...) by its GUID,
// from every pack's floor definitions (TSData\Res\Catalog\Patterns); empty when unknown. The
// first call reads them all (9 MB, a fraction of a second).
std::string FloorSurfaceType(uint32_t guid);
// A catalog floor's name in the game's language ("Baldosa de granito claro"), empty when unknown.
std::string FloorCatalogName(uint32_t guid);

// Entry `index` of STR# `instance` (group 0xFFFFFFFF) in TSData\Res\Text\<file> of the newest
// expansion that has it, in the game's language (English when the entry is empty). UTF-8; empty
// when missing.
std::string GameString(const char* file, unsigned instance, int index);

// Entry `index` of STR# `instance` in group `group` of the objects' package (objects.package of the
// newest pack), in the game's language (or English), falling back to English. UTF-8; empty when
// missing. The first call indexes the package (a fraction of a second).
std::string ObjectString(uint32_t group, unsigned instance, int index, bool english = false);

}  // namespace game
