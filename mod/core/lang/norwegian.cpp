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
    {Str::GroupControlPanel, "kontrollpanel"},
    {Str::GroupLivePanel, "Sim-panel"},
    {Str::GroupFamily, "familie"},
    {Str::GroupWantsFears, "ønsker og frykt"},
    {Str::GroupNeeds, "behov"},
    {Str::GroupControls, "kontroller"},
    {Str::GroupBuyPanel, "kjøpemodus"},
    {Str::GroupBuildPanel, "byggemodus"},
    {Str::GroupCatalog, "katalog"},
    {Str::GroupStarSign, "stjernetegn"},
    {Str::GroupAspiration, "ambisjon"},
    {Str::GroupSteps, "trinn"},
    {Str::GroupAge, "alder"},
    {Str::GroupSex, "kjønn"},
    // Text fields
    {Str::TextField, "%s %s, tekstfelt"},
    {Str::Blank, "tomt"},
    {Str::Space, "mellomrom"},
    // Values
    {Str::ValueOfMax, "%d av %d"},
    {Str::SliderRange, "%1$s til %2$s"},
    {Str::PointsLeft, "%d poeng igjen"},
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
    // Neighborhood: lots and families
    {Str::GroupLots, "tomter"},
    {Str::LotFamily, "familien %s"},
    {Str::LotForSale, "til salgs"},
    {Str::LotCommunity, "felles tomt"},
    {Str::LotApartmentsEmpty, "leilighetsbygg, ingen leietakere"},
    {Str::LotApartmentsWith, "leilighetsbygg: %s"},
    {Str::FamilySummary, "%1$s: %2$d medlemmer, penger %3$s"},
    // Money
    {Str::Simoleons, "simoleons"},
    {Str::FundsNow, "midler %s, %s"},
    // Lot: object browser (PageUp/PageDown)
    {Str::CategorySims, "simmer"},
    {Str::CategoryOther, "andre objekter"},
    {Str::NoObjects, "ingen objekter her"},
    {Str::FloorNumber, "etasje %d"},
    {Str::NotOnLot, "ikke på en tomt"},
    {Str::HereNow, "akkurat her"},
    {Str::TilesAway, "%d fliser mot %s"},
    {Str::OneTileAway, "1 flis mot %s"},
    {Str::DirNorth, "nord"},
    {Str::DirNorthEast, "nordøst"},
    {Str::DirEast, "øst"},
    {Str::DirSouthEast, "sørøst"},
    {Str::DirSouth, "sør"},
    {Str::DirSouthWest, "sørvest"},
    {Str::DirWest, "vest"},
    {Str::DirNorthWest, "nordvest"},
    {Str::OtherFloor, "i en annen etasje"},
    {Str::SelectedSimSuffix, "valgt"},
    {Str::NoInteractionsAtAll, "ingen handlinger: man kan ikke gjøre noe med denne gjenstanden"},
    {Str::WalkingThere, "går dit"},
    {Str::CannotWalk, "kan ikke gå dit"},
    {Str::NoSimSelected, "ingen sim valgt"},
    // Needs
    {Str::MotiveValue, "%s, %d prosent"},
    // Lot: status (Ctrl+E) and action queue
    {Str::GroupActionQueue, "handlingskø"},
    {Str::StatusIdle, "gjør ingenting"},
    {Str::StatusNeedsLow, "lave behov: %s"},
    {Str::StatusNeedsFine, "behovene er i orden"},
    // Narrator: the family watch
    {Str::ActionStarted, "%1$s: %2$s"},
    {Str::ActionFinished, "%1$s er ferdig med: %2$s"},
    {Str::NeedLow, "%1$s, lavt behov: %2$s"},
    {Str::NeedUrgent, "%1$s, akutt behov: %2$s"},
    // Lot: selecting Sims
    {Str::SimSelectedNow, "%s, valgt"},
    {Str::NoSuchSim, "ingen sim %d"},
    // Narrator: visitors
    {Str::VisitorArrived, "%s har kommet"},
    {Str::VisitorLeft, "%s har gått"},
    // Lot: rooms
    {Str::RoomObjects, "%s, %d objekter"},
    {Str::RoomObjectsOne, "%s, 1 objekt"},
    {Str::FundsChanged, "%1$s, igjen %2$s"},
    // Buy mode
    {Str::PlaceBegin,
     "%s: Page Down finner plasser der den får plass, piltastene flytter den, Enter setter den ned, Escape "
     "avbryter"},
    {Str::PlaceTile, "rute %1$d, %2$d"},
    {Str::PlaceOffLot, "utenfor tomten"},
    {Str::PlaceFits, "får plass"},
    {Str::PlaceBlocked, "får ikke plass her"},
    {Str::PlacePlaced, "%s plassert"},
    {Str::PlaceFacing, "vendt mot %s"},
    {Str::PlaceNoSpot, "vendt slik får den ikke plass noe sted på tomten"},
    {Str::PlaceRoomSpots, "%1$s, %2$d plasser"},
    {Str::PlaceMoving,
     "flytt %s: Page Down finner plasser, Enter setter den ned, Delete selger den, Escape setter den tilbake"},
    {Str::PlaceCannotPick, "%s kan ikke plukkes opp"},
    {Str::PlaceSold, "%s solgt"},
    // Build mode
    {Str::BuildBegin,
     "%s: piltastene flytter hjørnet, mellomrom markerer starten, Enter bygger, Ctrl+Enter fjerner, Escape "
     "avslutter"},
    {Str::BuildCorner, "hjørne %1$d, %2$d"},
    {Str::BuildMarked, "start markert ved %1$d, %2$d"},
    {Str::BuildNoMark, "marker først starten med mellomrom"},
    {Str::BuildDone, "bygget for %1$s, %2$s igjen"},
    {Str::BuildRemoved, "fjernet, %1$s tilbake, %2$s"},
    {Str::BuildNothing, "ingenting endret"},
};

}  // namespace

extern const Translations kNorwegian = {kEntries, std::size(kEntries)};

}  // namespace lang
