// The game's own texts, read from its .package files (DBPF 1.x, RefPack-compressed resources,
// STR# string tables with a language byte per entry), in the language the game runs in. For
// labels the game shows but keeps nowhere the mod can reach at runtime (buy mode categories).
#pragma once

#include <string>

namespace game {

// The STR# language code of the game's language (1 English, 6 Spanish, 13 Russian, 35 Brazilian
// Portuguese...), from Steam's language number (util/language.h).
int StringLanguageCode();

// Entry `index` of STR# `instance` (group 0xFFFFFFFF) in TSData\Res\Text\<file> of the newest
// expansion that has it, in the game's language (English when the entry is empty). UTF-8; empty
// when missing.
std::string GameString(const char* file, unsigned instance, int index);

}  // namespace game
