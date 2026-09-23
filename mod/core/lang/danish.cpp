// Danish: the game's language 9. Written by tools/mod_strings.py from a translation of
// util/strings_table.h; keep the order and groups of the English table.
#include "translations.h"

#include <iterator>

namespace lang {
namespace {

const Translation kEntries[] = {
    // Startup
    {Str::ModLoaded, "Sims 2-tilgængelighed indlæst"},
    {Str::UnsupportedBuild, "Sims 2-tilgængelighed: spilversionen understøttes ikke, mod deaktiveret"},
    // Key help (Ctrl+F1)
    {Str::KeysScreen,
     "Piletasterne flytter inden for en gruppe, Tab og Skift+Tab skifter gruppe, Home og End går til enderne, "
     "Enter aktiverer, R gentager. Kontrol+Pil op og Kontrol+Pil ned læser detaljerne om det, der blev sagt, "
     "Kontrol+Venstre pil og Kontrol+Højre pil skifter mellem detaljer og hændelser."},
    // Control state
    {Str::Checked, "markeret"},
    {Str::NotChecked, "ikke markeret"},
    {Str::Selected, "valgt"},
    {Str::TabSelected, "valgt"},
    {Str::Unavailable, "ikke tilgængelig"},
    {Str::UnlabeledButton, "knap uden navn"},
    {Str::Unnamed, "uden navn"},
    // Control roles: what a control is, said after its name
    {Str::RoleButton, "knap"},
    {Str::RoleTab, "fane"},
    {Str::RoleCheckBox, "afkrydsningsfelt"},
    {Str::RoleSlider, "skyder"},
    {Str::RolePicker, "værdivælger"},
    {Str::NothingHere, "her er intet at navigere i"},
    // Review buffers
    {Str::BufferDetails, "detaljer"},
    {Str::BufferEvents, "hændelser"},
    {Str::BufferEmpty, "%s, tom"},
    {Str::PositionInGroup, "%d af %d, %s"},
    // Groups
    {Str::GroupSummary, "%s, %d elementer"},
    {Str::GroupSummaryOne, "%s, 1 element"},
    {Str::GroupButtons, "knapper"},
    {Str::GroupText, "tekst"},
    {Str::GroupControls, "kontroller"},
    // Text fields
    {Str::TextField, "%s %s, tekstfelt"},
    {Str::Blank, "tomt"},
    {Str::Space, "mellemrum"},
    // Values
    {Str::ValueOfMax, "%d af %d"},
    {Str::LabelValue, "%s: %s"},
    {Str::NextPage, "næste side"},
    {Str::PreviousPage, "forrige side"},
    // Interaction menu
    {Str::PieMenu, "handlinger"},
    {Str::NoInteractions, "ingen handlinger"},
    {Str::Cancelled, "annulleret"},
    // Cheat console
    {Str::CheatConsole, "snydekonsol"},
    {Str::CheatRan, "snydekode: %s"},
    {Str::CheatEmpty, "skriv eller vælg en snydekode først"},
    // Money
    {Str::FundsNow, "penge %s, %s"},
    // Lot: object browser (PageUp/PageDown)
    {Str::CategorySims, "Simmere"},
    {Str::CategoryOther, "andre objekter"},
    {Str::NoObjects, "ingen objekter her"},
    {Str::FloorNumber, "etage %d"},
    {Str::NotOnLot, "ikke på en grund"},
    {Str::HereNow, "lige her"},
    {Str::TilesAway, "%d fliser mod %s"},
    {Str::OneTileAway, "1 flise mod %s"},
    {Str::DirNorth, "nord"},
    {Str::DirNorthEast, "nordøst"},
    {Str::DirEast, "øst"},
    {Str::DirSouthEast, "sydøst"},
    {Str::DirSouth, "syd"},
    {Str::DirSouthWest, "sydvest"},
    {Str::DirWest, "vest"},
    {Str::DirNorthWest, "nordvest"},
    {Str::OtherFloor, "på en anden etage"},
    {Str::SelectedSimSuffix, "valgt"},
    {Str::NoInteractionsAtAll, "ingen handlinger: man kan ikke gøre noget med denne genstand"},
    {Str::WalkingThere, "går derhen"},
    {Str::CannotWalk, "kan ikke gå derhen"},
    {Str::NoSimSelected, "ingen Sim valgt"},
    // Needs
    {Str::MotiveValue, "%s, %d procent"},
    // Lot: status (Ctrl+E) and action queue
    {Str::GroupActionQueue, "handlingskø"},
    {Str::StatusIdle, "laver ingenting"},
    {Str::StatusNeedsLow, "lave behov: %s"},
    {Str::StatusNeedsFine, "behov i orden"},
    {Str::FundsChanged, "%1$s, tilbage %2$s"},
};

}  // namespace

extern const Translations kDanish = {kEntries, std::size(kEntries)};

}  // namespace lang
