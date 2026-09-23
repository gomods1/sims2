// Hungarian: the game's language 25. Written by tools/mod_strings.py from a translation of
// util/strings_table.h; keep the order and groups of the English table.
#include "translations.h"

#include <iterator>

namespace lang {
namespace {

const Translation kEntries[] = {
    // Startup
    {Str::ModLoaded, "The Sims 2 akadálymentesítés betöltve"},
    {Str::UnsupportedBuild, "The Sims 2 akadálymentesítés: nem támogatott játékverzió, a mod kikapcsolva"},
    // Key help (Ctrl+F1)
    {Str::KeysScreen,
     "A nyilak egy csoporton belül mozognak, a Tab és a Shift+Tab csoportot vált, a Home és az End a végekre "
     "ugrik, az Enter megnyom, az R megismétel. A Ctrl+Fel és a Ctrl+Le felolvassa az elhangzottak részleteit, a "
     "Ctrl+Balra és a Ctrl+Jobbra a részletek és az események között vált."},
    // Control state
    {Str::Checked, "bejelölve"},
    {Str::NotChecked, "nincs bejelölve"},
    {Str::Selected, "kiválasztva"},
    {Str::TabSelected, "kiválasztva"},
    {Str::Unavailable, "nem elérhető"},
    {Str::UnlabeledButton, "felirat nélküli gomb"},
    {Str::Unnamed, "névtelen"},
    // Control roles: what a control is, said after its name
    {Str::RoleButton, "gomb"},
    {Str::RoleTab, "fül"},
    {Str::RoleCheckBox, "jelölőnégyzet"},
    {Str::RoleSlider, "csúszka"},
    {Str::RolePicker, "értékválasztó"},
    {Str::NothingHere, "itt nincs mit bejárni"},
    // Review buffers
    {Str::BufferDetails, "részletek"},
    {Str::BufferEvents, "események"},
    {Str::BufferEmpty, "%s, üres"},
    {Str::PositionInGroup, "%d / %d, %s"},
    // Groups
    {Str::GroupSummary, "%s, %d elem"},
    {Str::GroupSummaryOne, "%s, 1 elem"},
    {Str::GroupButtons, "gombok"},
    {Str::GroupText, "szöveg"},
    {Str::GroupControls, "vezérlők"},
    // Text fields
    {Str::TextField, "%s %s, szövegmező"},
    {Str::Blank, "üres"},
    {Str::Space, "szóköz"},
    // Values
    {Str::ValueOfMax, "%d / %d"},
    {Str::LabelValue, "%s: %s"},
    {Str::NextPage, "következő oldal"},
    {Str::PreviousPage, "előző oldal"},
    // Interaction menu
    {Str::PieMenu, "interakciók"},
    {Str::NoInteractions, "nincs interakció"},
    {Str::Cancelled, "megszakítva"},
    // Cheat console
    {Str::CheatConsole, "csalókonzol"},
    {Str::CheatRan, "csalás: %s"},
    {Str::CheatEmpty, "előbb írj be vagy válassz egy csalást"},
    // Money
    {Str::FundsNow, "pénz %s, %s"},
    // Narrator: the family watch
    {Str::FundsChanged, "%1$s, maradt %2$s"},
};

}  // namespace

extern const Translations kHungarian = {kEntries, std::size(kEntries)};

}  // namespace lang
