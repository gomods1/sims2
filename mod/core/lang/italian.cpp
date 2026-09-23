// Italian: the game's language 4. Written by tools/mod_strings.py from a translation of
// util/strings_table.h; keep the order and groups of the English table.
#include "translations.h"

#include <iterator>

namespace lang {
namespace {

const Translation kEntries[] = {
    // Startup
    {Str::ModLoaded, "Accessibilità per The Sims 2 caricata"},
    {Str::UnsupportedBuild, "Accessibilità per The Sims 2: versione del gioco non supportata, mod disattivata"},
    // Key help (Ctrl+F1)
    {Str::KeysScreen,
     "Le frecce si spostano in un gruppo, Tab e Maiuscole+Tab cambiano gruppo, Inizio e Fine vanno agli estremi, "
     "Invio attiva, R ripete. Control+freccia su e Control+freccia giù leggono i dettagli di quanto detto, "
     "Control+freccia sinistra e Control+freccia destra passano tra dettagli ed eventi."},
    // Control state
    {Str::Checked, "selezionata"},
    {Str::NotChecked, "non selezionata"},
    {Str::Selected, "selezionato"},
    {Str::TabSelected, "selezionata"},
    {Str::Unavailable, "non disponibile"},
    {Str::UnlabeledButton, "pulsante senza nome"},
    {Str::Unnamed, "senza nome"},
    // Control roles: what a control is, said after its name
    {Str::RoleButton, "pulsante"},
    {Str::RoleTab, "scheda"},
    {Str::RoleCheckBox, "casella di controllo"},
    {Str::RoleSlider, "cursore"},
    {Str::RolePicker, "selettore"},
    {Str::NothingHere, "qui non c'è niente da esplorare"},
    // Review buffers
    {Str::BufferDetails, "dettagli"},
    {Str::BufferEvents, "eventi"},
    {Str::BufferEmpty, "%s, vuoto"},
    {Str::PositionInGroup, "%d di %d, %s"},
    // Groups
    {Str::GroupSummary, "%s, %d elementi"},
    {Str::GroupSummaryOne, "%s, 1 elemento"},
    {Str::GroupButtons, "pulsanti"},
    {Str::GroupText, "testo"},
    {Str::GroupControlPanel, "pannello di controllo"},
    {Str::GroupLivePanel, "pannello del Sim"},
    {Str::GroupFamily, "famiglia"},
    {Str::GroupWantsFears, "desideri e paure"},
    {Str::GroupNeeds, "bisogni"},
    {Str::GroupControls, "controlli"},
    {Str::GroupBuyPanel, "modalità Compra"},
    {Str::GroupCatalog, "catalogo"},
    // Text fields
    {Str::TextField, "%s %s, campo di testo"},
    {Str::Blank, "vuoto"},
    {Str::Space, "spazio"},
    // Values
    {Str::ValueOfMax, "%d su %d"},
    {Str::SliderRange, "da %1$s a %2$s"},
    {Str::PointsLeft, "%d punti rimasti"},
    {Str::LabelValue, "%s: %s"},
    {Str::NextPage, "pagina successiva"},
    {Str::PreviousPage, "pagina precedente"},
    // Interaction menu
    {Str::PieMenu, "interazioni"},
    {Str::NoInteractions, "nessuna interazione"},
    {Str::Cancelled, "annullato"},
    // Cheat console
    {Str::CheatConsole, "console dei trucchi"},
    {Str::CheatRan, "trucco: %s"},
    {Str::CheatEmpty, "prima scrivi o scegli un trucco"},
    // Neighborhood: lots and families
    {Str::GroupLots, "lotti"},
    {Str::LotFamily, "famiglia %s"},
    {Str::LotForSale, "in vendita"},
    {Str::LotCommunity, "lotto comunitario"},
    {Str::LotApartmentsEmpty, "condominio, senza inquilini"},
    {Str::LotApartmentsWith, "condominio: %s"},
    {Str::FamilySummary, "%1$s: %2$d membri, fondi %3$s"},
    // Money
    {Str::Simoleons, "simoleon"},
    {Str::FundsNow, "denaro %s, %s"},
    // Lot: object browser (PageUp/PageDown)
    {Str::CategorySims, "Sims"},
    {Str::CategoryOther, "altri oggetti"},
    {Str::NoObjects, "nessun oggetto qui"},
    {Str::FloorNumber, "livello %d"},
    {Str::NotOnLot, "non sei in un lotto"},
    {Str::HereNow, "proprio qui"},
    {Str::TilesAway, "a %d caselle a %s"},
    {Str::OneTileAway, "a 1 casella a %s"},
    {Str::DirNorth, "nord"},
    {Str::DirNorthEast, "nord-est"},
    {Str::DirEast, "est"},
    {Str::DirSouthEast, "sud-est"},
    {Str::DirSouth, "sud"},
    {Str::DirSouthWest, "sud-ovest"},
    {Str::DirWest, "ovest"},
    {Str::DirNorthWest, "nord-ovest"},
    {Str::OtherFloor, "su un altro livello"},
    {Str::SelectedSimSuffix, "Sim selezionato"},
    {Str::NoInteractionsAtAll, "nessuna interazione: con questo oggetto non si può fare niente"},
    {Str::WalkingThere, "sta andando lì"},
    {Str::CannotWalk, "non può andare lì"},
    {Str::NoSimSelected, "nessun Sim selezionato"},
    // Needs
    {Str::MotiveValue, "%s, %d per cento"},
    // Lot: status (Ctrl+E) and action queue
    {Str::GroupActionQueue, "coda delle azioni"},
    {Str::StatusIdle, "non fa niente"},
    {Str::StatusNeedsLow, "bisogni bassi: %s"},
    {Str::StatusNeedsFine, "bisogni a posto"},
    // Narrator: the family watch
    {Str::ActionStarted, "%1$s: %2$s"},
    {Str::ActionFinished, "%1$s ha finito: %2$s"},
    {Str::NeedLow, "%1$s, bisogno basso: %2$s"},
    {Str::NeedUrgent, "%1$s, bisogno urgente: %2$s"},
    // Lot: selecting Sims
    {Str::SimSelectedNow, "%s, Sim selezionato"},
    {Str::NoSuchSim, "non c'è il Sim %d"},
    // Narrator: visitors
    {Str::VisitorArrived, "%s arriva"},
    {Str::VisitorLeft, "%s se ne va"},
    // Lot: rooms
    {Str::RoomObjects, "%s, %d oggetti"},
    {Str::RoomObjectsOne, "%s, 1 oggetto"},
    {Str::FundsChanged, "%1$s, restano %2$s"},
    // Buy mode
    {Str::PlaceBegin, "%s: Pagina giù trova i posti dove entra, le frecce lo spostano, Invio lo posa, Esc annulla"},
    {Str::PlaceTile, "casella %1$d, %2$d"},
    {Str::PlaceOffLot, "fuori dal lotto"},
    {Str::PlaceFits, "ci sta"},
    {Str::PlaceBlocked, "qui non ci sta"},
    {Str::PlacePlaced, "%s posato"},
    {Str::PlaceFacing, "rivolto a %s"},
    {Str::PlaceNoSpot, "così girato non ci sta da nessuna parte in questo lotto"},
    {Str::PlaceRoomSpots, "%1$s, %2$d posti"},
    {Str::PlaceMoving, "spostare %s: Pagina giù trova posti, Invio lo posa, Canc lo vende, Esc lo rimette a posto"},
    {Str::PlaceCannotPick, "%s non si può prendere"},
    {Str::PlaceSold, "%s venduto"},
};

}  // namespace

extern const Translations kItalian = {kEntries, std::size(kEntries)};

}  // namespace lang
