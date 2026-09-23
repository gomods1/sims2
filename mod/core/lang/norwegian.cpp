// Norwegian: the game's language 22. Written by tools/mod_strings.py from a translation of
// util/strings_table.h; keep the order and groups of the English table.
#include "translations.h"

#include <iterator>

namespace lang {
namespace {

const Translation kEntries[] = {
    // Startup
    {Str::ModLoaded, "Sims 2-tilgjengelighet lastet inn"},
    {Str::UnsupportedBuild, "Sims 2-tilgjengelighet: spillversjonen støttes ikke, modden er deaktivert"},
    // Key help (Ctrl+F1)
    {Str::KeysScreen,
     "Piltastene flytter innenfor en gruppe, Tab og Skift+Tab bytter gruppe, Home og End går til endene, Enter "
     "aktiverer, R gjentar. Kontroll+Pil opp og Kontroll+Pil ned leser detaljene om det som ble sagt, "
     "Kontroll+Venstre pil og Kontroll+Høyre pil bytter mellom detaljer og hendelser."},
    // Control state
    {Str::Checked, "avmerket"},
    {Str::NotChecked, "ikke avmerket"},
    {Str::Selected, "valgt"},
    {Str::TabSelected, "valgt"},
    {Str::Unavailable, "utilgjengelig"},
    {Str::UnlabeledButton, "knapp uten navn"},
    {Str::Unnamed, "uten navn"},
    // Control roles: what a control is, said after its name
    {Str::RoleButton, "knapp"},
    {Str::RoleTab, "fane"},
    {Str::RoleCheckBox, "avmerkingsboks"},
    {Str::RoleSlider, "glidebryter"},
    {Str::RolePicker, "verdivelger"},
    {Str::NothingHere, "ingenting å navigere i her"},
    // Review buffers
    {Str::BufferDetails, "detaljer"},
    {Str::BufferEvents, "hendelser"},
    {Str::BufferEmpty, "%s, tom"},
    {Str::PositionInGroup, "%d av %d, %s"},
    // Groups
    {Str::GroupSummary, "%s, %d elementer"},
    {Str::GroupSummaryOne, "%s, 1 element"},
    {Str::GroupButtons, "knapper"},
    {Str::GroupText, "tekst"},
    {Str::GroupControls, "kontroller"},
    // Text fields
    {Str::TextField, "%s %s, tekstfelt"},
    {Str::Blank, "tomt"},
    {Str::Space, "mellomrom"},
    // Values
    {Str::ValueOfMax, "%d av %d"},
    {Str::LabelValue, "%s: %s"},
    {Str::NextPage, "neste side"},
    {Str::PreviousPage, "forrige side"},
    // Interaction menu
    {Str::PieMenu, "handlinger"},
    {Str::NoInteractions, "ingen handlinger"},
    {Str::Cancelled, "avbrutt"},
    // Cheat console
    {Str::CheatConsole, "juksekonsoll"},
    {Str::CheatRan, "juksekode: %s"},
    {Str::CheatEmpty, "skriv eller velg en juksekode først"},
    // Money
    {Str::FundsNow, "midler %s, %s"},
    {Str::FundsChanged, "%1$s, igjen %2$s"},
};

}  // namespace

extern const Translations kNorwegian = {kEntries, std::size(kEntries)};

}  // namespace lang
