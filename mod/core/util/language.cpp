#include "language.h"

#include <windows.h>

#include "../lang/translations.h"
#include "game_settings.h"
#include "log.h"
#include "strings.h"

namespace util {
namespace {

struct LanguageInfo {
    Language language;
    const char* name;                    // for the log
    unsigned codePage;                   // the game's text in this language
    const char* digitGroupSeparator;
    const lang::Translations* strings;   // nullptr: English, the source
};

// The Sims 2 stores all its text as UTF-8 (STR# resources in the .package files), in every
// language; codePage is only kept for the log.
const LanguageInfo kLanguages[] = {
    {Language::English, "English", CP_UTF8, ",", nullptr},
    {Language::French, "French", CP_UTF8, "", &lang::kFrench},
    {Language::German, "German", CP_UTF8, ".", &lang::kGerman},
    {Language::Italian, "Italian", CP_UTF8, ".", &lang::kItalian},
    {Language::Spanish, "Spanish", CP_UTF8, ".", &lang::kSpanish},
    {Language::Dutch, "Dutch", CP_UTF8, ".", &lang::kDutch},
    {Language::Danish, "Danish", CP_UTF8, ".", &lang::kDanish},
    {Language::Swedish, "Swedish", CP_UTF8, "", &lang::kSwedish},
    {Language::Norwegian, "Norwegian", CP_UTF8, "", &lang::kNorwegian},
    {Language::Finnish, "Finnish", CP_UTF8, "", &lang::kFinnish},
    {Language::Portuguese, "Portuguese", CP_UTF8, ".", &lang::kPortuguese},
    {Language::Japanese, "Japanese", CP_UTF8, ",", &lang::kJapanese},
    {Language::Polish, "Polish", CP_UTF8, "", &lang::kPolish},
    {Language::SimplifiedChinese, "Simplified Chinese", CP_UTF8, ",", &lang::kSimplifiedChinese},
    {Language::TraditionalChinese, "Traditional Chinese", CP_UTF8, ",", &lang::kTraditionalChinese},
    {Language::Thai, "Thai", CP_UTF8, ",", &lang::kThai},
    {Language::Korean, "Korean", CP_UTF8, ",", &lang::kKorean},
    {Language::Czech, "Czech", CP_UTF8, "", &lang::kCzech},
    {Language::Hungarian, "Hungarian", CP_UTF8, "", &lang::kHungarian},
    {Language::Russian, "Russian", CP_UTF8, "", &lang::kRussian},
};

// Steam's language numbers for this game (3314070_install.vdf writes them to the registry).
// European Portuguese (23) uses the Brazilian translation of the mod's strings.
struct GameLanguage {
    int number;
    Language language;
};

const GameLanguage kGameLanguages[] = {
    {1, Language::English},    {2, Language::French},     {3, Language::German},
    {4, Language::Italian},    {5, Language::Spanish},    {6, Language::Swedish},
    {7, Language::Finnish},    {8, Language::Dutch},      {9, Language::Danish},
    {10, Language::Portuguese}, {11, Language::Czech},    {14, Language::Japanese},
    {15, Language::Korean},    {16, Language::Russian},   {17, Language::SimplifiedChinese},
    {18, Language::TraditionalChinese}, {20, Language::Polish}, {21, Language::Thai},
    {22, Language::Norwegian}, {23, Language::Portuguese}, {25, Language::Hungarian},
};

// Windows' UI language, for a game without its registry settings.
Language WindowsLanguage() {
    switch (PRIMARYLANGID(GetUserDefaultUILanguage())) {
    case LANG_FRENCH: return Language::French;
    case LANG_GERMAN: return Language::German;
    case LANG_ITALIAN: return Language::Italian;
    case LANG_SPANISH: return Language::Spanish;
    case LANG_DUTCH: return Language::Dutch;
    case LANG_DANISH: return Language::Danish;
    case LANG_SWEDISH: return Language::Swedish;
    case LANG_NORWEGIAN: return Language::Norwegian;
    case LANG_FINNISH: return Language::Finnish;
    case LANG_PORTUGUESE: return Language::Portuguese;
    case LANG_JAPANESE: return Language::Japanese;
    case LANG_POLISH: return Language::Polish;
    case LANG_CHINESE:
        return SUBLANGID(GetUserDefaultUILanguage()) == SUBLANG_CHINESE_SIMPLIFIED ? Language::SimplifiedChinese
                                                                                   : Language::TraditionalChinese;
    case LANG_THAI: return Language::Thai;
    case LANG_KOREAN: return Language::Korean;
    case LANG_CZECH: return Language::Czech;
    case LANG_HUNGARIAN: return Language::Hungarian;
    case LANG_RUSSIAN: return Language::Russian;
    default: return Language::English;
    }
}

const LanguageInfo* g_current = &kLanguages[0];

const LanguageInfo& InfoOf(Language language) {
    for (const LanguageInfo& info : kLanguages)
        if (info.language == language) return info;
    return kLanguages[0];
}

void Use(Language language, const char* source) {
    g_current = &InfoOf(language);
    LoadStrings(g_current->strings);
    Log("language: %s (%s), code page %u", g_current->name, source, g_current->codePage);
}

}  // namespace

void InitLanguage() {
    int number = ReadGameLanguageNumber();
    for (const GameLanguage& g : kGameLanguages) {
        if (g.number == number) {
            Use(g.language, "registry");
            return;
        }
    }
    Use(WindowsLanguage(), number ? "unknown language number, Windows' language" : "no language setting, Windows' language");
}

void SetGameLanguage(int number) {
    if (number == 0) return;
    for (const GameLanguage& g : kGameLanguages) {
        if (g.number != number) continue;
        if (g.language != g_current->language) Use(g.language, "the game's choice");
        return;
    }
    Log("unknown game language number %d; keeping %s", number, g_current->name);
}

Language CurrentLanguage() { return g_current->language; }

unsigned GameCodePage() { return g_current->codePage; }

const char* DigitGroupSeparator() { return g_current->digitGroupSeparator; }

}  // namespace util
