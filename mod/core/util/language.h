// The language the game runs in: which translation of the mod's strings to speak (util/strings.h)
// and how the game encodes its own text.
//
// The Sims 2 takes its language from the registry value Steam writes (HKCU\Software\Electronic
// Arts\The Sims 2 Ultimate Collection 25\1.0, "language": Steam's numbering, 5 Spanish...). All
// of its text is UTF-8.
#pragma once

#include <string>

namespace util {

enum class Language {
    English,
    French,
    German,
    Italian,
    Spanish,
    Dutch,
    Danish,
    Swedish,
    Norwegian,
    Finnish,
    Portuguese,
    Japanese,
    Polish,
    SimplifiedChinese,
    TraditionalChinese,
    Thai,
    Korean,
    Czech,
    Hungarian,
    Russian,
};

// Before game memory can be read: the language the registry names, else Windows' own.
void InitLanguage();

// The game's language number once the game can be asked (game::LanguageNumber()): it wins over
// the registry, which Steam's language or a switch may have overridden. 0 (unknown) is ignored.
void SetGameLanguage(int number);

Language CurrentLanguage();

// The manual's file name for the current language ("en", "es", "zh-hans"...): tools/build_docs.py
// makes Sims2Access\docs\<tag>.html from docs/manual/<tag>.md.
const char* DocTag();

// The Windows code page of the game's text: always UTF-8 in The Sims 2.
unsigned GameCodePage();

// What separates thousands in amounts of money the mod writes: "1,200", "1.200", or nothing
// where the language groups digits with spaces (a synthesizer can read "1 200" as two numbers).
const char* DigitGroupSeparator();

}  // namespace util
