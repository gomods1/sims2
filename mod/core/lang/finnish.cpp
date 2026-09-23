// Finnish: the game's language 7. Written by tools/mod_strings.py from a translation of
// util/strings_table.h; keep the order and groups of the English table.
#include "translations.h"

#include <iterator>

namespace lang {
namespace {

const Translation kEntries[] = {
    // Startup
    {Str::ModLoaded, "Sims 2 -saavutettavuus ladattu"},
    {Str::UnsupportedBuild, "Sims 2 -saavutettavuus: pelin versiota ei tueta, modi on poistettu käytöstä"},
    // Key help (Ctrl+F1)
    {Str::KeysScreen,
     "Nuolinäppäimet liikkuvat ryhmän sisällä, Sarkain ja Vaihto+Sarkain vaihtavat ryhmää, Home ja End siirtyvät "
     "alkuun ja loppuun, Enter aktivoi, R toistaa. Control+nuoli ylös ja Control+nuoli alas lukevat juuri "
     "sanotun tiedot, Control+nuoli vasemmalle ja Control+nuoli oikealle vaihtavat tietojen ja tapahtumien "
     "välillä."},
    // Control state
    {Str::Checked, "valittu"},
    {Str::NotChecked, "ei valittu"},
    {Str::Selected, "valittu"},
    {Str::TabSelected, "valittu"},
    {Str::Unavailable, "ei käytettävissä"},
    {Str::UnlabeledButton, "nimetön painike"},
    {Str::Unnamed, "nimetön"},
    // Control roles: what a control is, said after its name
    {Str::RoleButton, "painike"},
    {Str::RoleTab, "välilehti"},
    {Str::RoleCheckBox, "valintaruutu"},
    {Str::RoleSlider, "liukusäädin"},
    {Str::RolePicker, "valitsin"},
    {Str::NothingHere, "täällä ei ole mitään selattavaa"},
    // Review buffers
    {Str::BufferDetails, "tiedot"},
    {Str::BufferEvents, "tapahtumat"},
    {Str::BufferEmpty, "%s, tyhjä"},
    {Str::PositionInGroup, "%d, yhteensä %d, ryhmä %s"},
    // Groups
    {Str::GroupSummary, "%s, %d kohdetta"},
    {Str::GroupSummaryOne, "%s, 1 kohde"},
    {Str::GroupButtons, "painikkeet"},
    {Str::GroupText, "teksti"},
    {Str::GroupControlPanel, "ohjauspaneeli"},
    {Str::GroupLivePanel, "Sim-paneeli"},
    {Str::GroupFamily, "perhe"},
    {Str::GroupWantsFears, "toiveet ja pelot"},
    {Str::GroupNeeds, "tarpeet"},
    {Str::GroupControls, "ohjaimet"},
    // Text fields
    {Str::TextField, "%s %s, tekstikenttä"},
    {Str::Blank, "tyhjä"},
    {Str::Space, "välilyönti"},
    // Values
    {Str::ValueOfMax, "%d, enintään %d"},
    {Str::LabelValue, "%s: %s"},
    {Str::NextPage, "seuraava sivu"},
    {Str::PreviousPage, "edellinen sivu"},
    // Interaction menu
    {Str::PieMenu, "toiminnot"},
    {Str::NoInteractions, "ei toimintoja"},
    {Str::Cancelled, "peruttu"},
    // Cheat console
    {Str::CheatConsole, "huijauskonsoli"},
    {Str::CheatRan, "huijaus: %s"},
    {Str::CheatEmpty, "kirjoita tai valitse ensin huijaus"},
    // Neighborhood: lots and families
    {Str::GroupLots, "tontit"},
    {Str::LotFamily, "perhe %s"},
    {Str::LotForSale, "myytävänä"},
    {Str::LotCommunity, "yleinen tontti"},
    {Str::LotApartmentsEmpty, "kerrostalo, ei asukkaita"},
    {Str::LotApartmentsWith, "kerrostalo: %s"},
    {Str::FamilySummary, "%1$s: %2$d jäsentä, rahaa %3$s"},
    // Money
    {Str::Simoleons, "simoleonia"},
    {Str::FundsNow, "rahat %s, %s"},
    // Lot: object browser (PageUp/PageDown)
    {Str::CategorySims, "simit"},
    {Str::CategoryOther, "muut esineet"},
    {Str::NoObjects, "täällä ei ole esineitä"},
    {Str::FloorNumber, "kerros %d"},
    {Str::NotOnLot, "et ole tontilla"},
    {Str::HereNow, "juuri tässä"},
    {Str::TilesAway, "%d ruutua %ssuuntaan"},
    {Str::OneTileAway, "1 ruutu %ssuuntaan"},
    {Str::DirNorth, "pohjois"},
    {Str::DirNorthEast, "koillis"},
    {Str::DirEast, "itä"},
    {Str::DirSouthEast, "kaakkois"},
    {Str::DirSouth, "etelä"},
    {Str::DirSouthWest, "lounais"},
    {Str::DirWest, "länsi"},
    {Str::DirNorthWest, "luoteis"},
    {Str::OtherFloor, "eri kerroksessa"},
    {Str::SelectedSimSuffix, "valittu"},
    {Str::NoInteractionsAtAll, "ei toimintoja: tällä esineellä ei voi tehdä mitään"},
    {Str::WalkingThere, "menee sinne"},
    {Str::CannotWalk, "ei pääse sinne"},
    {Str::NoSimSelected, "simiä ei ole valittu"},
    // Needs
    {Str::MotiveValue, "%s, %d prosenttia"},
    // Lot: status (Ctrl+E) and action queue
    {Str::GroupActionQueue, "toimintojono"},
    {Str::StatusIdle, "ei tee mitään"},
    {Str::StatusNeedsLow, "matalat tarpeet: %s"},
    {Str::StatusNeedsFine, "tarpeet kunnossa"},
    // Narrator: the family watch
    {Str::ActionStarted, "%1$s: %2$s"},
    {Str::ActionFinished, "%1$s lopetti: %2$s"},
    {Str::NeedLow, "%1$s, matala tarve: %2$s"},
    {Str::NeedUrgent, "%1$s, kiireellinen tarve: %2$s"},
    // Lot: selecting Sims
    {Str::SimSelectedNow, "%s, valittu"},
    {Str::NoSuchSim, "simiä numero %d ei ole"},
    // Narrator: visitors
    {Str::VisitorArrived, "%s saapui"},
    {Str::VisitorLeft, "%s lähti"},
    // Lot: rooms
    {Str::RoomObjects, "%s, %d esinettä"},
    {Str::RoomObjectsOne, "%s, 1 esine"},
    {Str::FundsChanged, "%1$s, jäljellä %2$s"},
};

}  // namespace

extern const Translations kFinnish = {kEntries, std::size(kEntries)};

}  // namespace lang
