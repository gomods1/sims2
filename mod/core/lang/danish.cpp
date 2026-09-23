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
    {Str::GroupControlPanel, "kontrolpanel"},
    {Str::GroupLivePanel, "Sim-panel"},
    {Str::GroupFamily, "familie"},
    {Str::GroupWantsFears, "ønsker og frygt"},
    {Str::GroupNeeds, "behov"},
    {Str::GroupControls, "kontroller"},
    {Str::GroupBuyPanel, "købetilstand"},
    {Str::GroupCatalog, "katalog"},
    // Text fields
    {Str::TextField, "%s %s, tekstfelt"},
    {Str::Blank, "tomt"},
    {Str::Space, "mellemrum"},
    // Values
    {Str::ValueOfMax, "%d af %d"},
    {Str::SliderRange, "%1$s til %2$s"},
    {Str::PointsLeft, "%d point tilbage"},
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
    // Neighborhood: lots and families
    {Str::GroupLots, "grunde"},
    {Str::LotFamily, "familien %s"},
    {Str::LotForSale, "til salg"},
    {Str::LotCommunity, "fællesgrund"},
    {Str::LotApartmentsEmpty, "lejlighedsbygning, ingen lejere"},
    {Str::LotApartmentsWith, "lejlighedsbygning: %s"},
    {Str::FamilySummary, "%1$s: %2$d medlemmer, penge %3$s"},
    // Money
    {Str::Simoleons, "simoleons"},
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
    // Narrator: the family watch
    {Str::ActionStarted, "%1$s: %2$s"},
    {Str::ActionFinished, "%1$s er færdig med: %2$s"},
    {Str::NeedLow, "%1$s, lavt behov: %2$s"},
    {Str::NeedUrgent, "%1$s, akut behov: %2$s"},
    // Lot: selecting Sims
    {Str::SimSelectedNow, "%s, valgt"},
    {Str::NoSuchSim, "ingen Sim %d"},
    // Narrator: visitors
    {Str::VisitorArrived, "%s er kommet"},
    {Str::VisitorLeft, "%s er gået"},
    // Lot: rooms
    {Str::RoomObjects, "%s, %d objekter"},
    {Str::RoomObjectsOne, "%s, 1 objekt"},
    {Str::FundsChanged, "%1$s, tilbage %2$s"},
    // Buy mode
    {Str::PlaceBegin,
     "%s: Page Down finder pladser, hvor den kan stå, piletasterne flytter den, Enter stiller den, Escape "
     "annullerer"},
    {Str::PlaceTile, "felt %1$d, %2$d"},
    {Str::PlaceOffLot, "uden for grunden"},
    {Str::PlaceFits, "kan stå her"},
    {Str::PlaceBlocked, "kan ikke stå her"},
    {Str::PlacePlaced, "%s stillet"},
    {Str::PlaceFacing, "vendt mod %s"},
    {Str::PlaceNoSpot, "vendt sådan kan den ikke stå nogen steder på grunden"},
    {Str::PlaceRoomSpots, "%1$s, %2$d pladser"},
    {Str::PlaceMoving,
     "flyt %s: Page Down finder pladser, Enter stiller den, Delete sælger den, Escape stiller den tilbage"},
    {Str::PlaceCannotPick, "%s kan ikke samles op"},
    {Str::PlaceSold, "%s solgt"},
};

}  // namespace

extern const Translations kDanish = {kEntries, std::size(kEntries)};

}  // namespace lang
