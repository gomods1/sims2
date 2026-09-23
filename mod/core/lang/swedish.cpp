// Swedish: the game's language 6. Written by tools/mod_strings.py from a translation of
// util/strings_table.h; keep the order and groups of the English table.
#include "translations.h"

#include <iterator>

namespace lang {
namespace {

const Translation kEntries[] = {
    // Startup
    {Str::ModLoaded, "Tillgänglighet för The Sims 2 laddad"},
    {Str::UnsupportedBuild, "Tillgänglighet för The Sims 2: spelversionen stöds inte, modden är avstängd"},
    // Key help (Ctrl+F1)
    {Str::KeysScreen,
     "Piltangenterna flyttar inom en grupp, Tabb och Skift+Tabb byter grupp, Home och End går till början och "
     "slutet, Enter aktiverar, R upprepar. Kontroll+Pil upp och Kontroll+Pil ned läser detaljerna om det som "
     "sades, Kontroll+Pil vänster och Kontroll+Pil höger växlar mellan detaljer och händelser."},
    // Control state
    {Str::Checked, "markerad"},
    {Str::NotChecked, "inte markerad"},
    {Str::Selected, "vald"},
    {Str::TabSelected, "vald"},
    {Str::Unavailable, "inte tillgänglig"},
    {Str::UnlabeledButton, "knapp utan namn"},
    {Str::Unnamed, "namnlös"},
    // Control roles: what a control is, said after its name
    {Str::RoleButton, "knapp"},
    {Str::RoleTab, "flik"},
    {Str::RoleCheckBox, "kryssruta"},
    {Str::RoleSlider, "skjutreglage"},
    {Str::RolePicker, "väljare"},
    {Str::NothingHere, "här finns inget att navigera"},
    // Review buffers
    {Str::BufferDetails, "detaljer"},
    {Str::BufferEvents, "händelser"},
    {Str::BufferEmpty, "%s, tomt"},
    {Str::PositionInGroup, "%d av %d, %s"},
    // Groups
    {Str::GroupSummary, "%s, %d objekt"},
    {Str::GroupSummaryOne, "%s, 1 objekt"},
    {Str::GroupButtons, "knappar"},
    {Str::GroupText, "text"},
    {Str::GroupControls, "kontroller"},
    // Text fields
    {Str::TextField, "%s %s, textfält"},
    {Str::Blank, "tomt"},
    {Str::Space, "mellanslag"},
    // Values
    {Str::ValueOfMax, "%d av %d"},
    {Str::LabelValue, "%s: %s"},
    {Str::NextPage, "nästa sida"},
    {Str::PreviousPage, "föregående sida"},
    // Interaction menu
    {Str::PieMenu, "interaktioner"},
    {Str::NoInteractions, "inga interaktioner"},
    {Str::Cancelled, "avbrutet"},
    // Cheat console
    {Str::CheatConsole, "fuskkonsol"},
    {Str::CheatRan, "fusk: %s"},
    {Str::CheatEmpty, "skriv eller välj ett fusk först"},
    // Money
    {Str::FundsNow, "tillgångar %s, %s"},
    {Str::FundsChanged, "%1$s, kvar %2$s"},
};

}  // namespace

extern const Translations kSwedish = {kEntries, std::size(kEntries)};

}  // namespace lang
