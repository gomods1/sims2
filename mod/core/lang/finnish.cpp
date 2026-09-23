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
    {Str::KeysLot,
     "Tontti: Page Down ja Page Up, tontin esineet, lähimmät ensin; Shiftin kanssa luokat, Ctrl:n kanssa "
     "huoneet. Enter avaa esineen toiminnot, tai nostaa sen ostotilassa; Ctrl+Enter lähettää valitun simin "
     "sinne. Ctrl+E simin tila, Ctrl+1–9 valitsevat simin, Tab paneelit, Ctrl+nuoli ylös ja alas yksityiskohdat."},
    {Str::KeysPlacement,
     "Sijoittaminen: Page Down ja Page Up paikat, joihin se mahtuu, Ctrl:n kanssa huoneet, nuolet yksi ruutu, "
     "Home takaisin simin luo, pilkku ja piste kääntävät sitä, Enter asettaa sen, Delete myy sen, Escape "
     "lopettaa."},
    {Str::KeysBuild,
     "Rakentaminen: nuolet siirtävät kulmaa, Shiftin kanssa viisi, Page Up ja Page Down kerros, välilyönti "
     "merkitsee alun, Enter rakentaa merkistä, Ctrl+Enter poistaa, Home takaisin simin luo, R toistaa, Escape "
     "lopettaa."},
    {Str::KeysNeighborhood,
     "Naapurusto: ensin tonttien luettelo, Enter avaa tontin paneelin. Muuttaaksesi perheen, valitse se "
     "perhesäiliöstä ja paina Enter tontin kohdalla. Tab paneelit."},
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
    {Str::GroupBuyPanel, "ostotila"},
    {Str::GroupBuildPanel, "rakennustila"},
    {Str::GroupCatalog, "luettelo"},
    {Str::GroupStarSign, "tähtimerkki"},
    {Str::GroupAspiration, "tavoite"},
    {Str::GroupSteps, "vaiheet"},
    {Str::GroupAge, "ikä"},
    {Str::GroupSex, "sukupuoli"},
    {Str::GroupTraits, "piirteet"},
    // Text fields
    {Str::TextField, "%s %s, tekstikenttä"},
    {Str::Blank, "tyhjä"},
    {Str::Space, "välilyönti"},
    // Values
    {Str::ValueOfMax, "%d, enintään %d"},
    {Str::SliderRange, "%1$s–%2$s"},
    {Str::RadioBetween, "%1$s ja %2$s välillä"},
    {Str::PointsLeft, "%d pistettä jäljellä"},
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
    // Camera
    {Str::CameraFree, "kamera vapaa"},
    {Str::CameraFollowing, "seuraa: %s"},
    // Narrator
    {Str::NarratorAll, "kertoja: kaikki"},
    {Str::NarratorWarnings, "kertoja: vain varoitukset"},
    {Str::NarratorOff, "kertoja pois"},
    // Relationships
    {Str::RelationshipValues, "%1$s, päivittäinen %2$s, elinikäinen %3$s"},
    // Room layout (Ctrl+L)
    {Str::LayoutNoRoom, "tässä ei ole huonetta"},
    {Str::LayoutSummary, "%1$s, kerros %2$d, %3$d ruutua, %4$d, %5$d – %6$d, %7$d"},
    {Str::LayoutOpenings, "%1$d ovea, %2$d ikkunaa"},
    {Str::LayoutDoor, "ovi %1$s, seinä: %2$s, johtaa: %3$s, ruutu %4$d, %5$d"},
    {Str::LayoutWindow, "ikkuna %1$s, seinä: %2$s, näkymä: %3$s, ruutu %4$d, %5$d"},
    // Buy mode
    {Str::PlaceBegin,
     "%s: Page Down etsii paikat, joihin se mahtuu, nuolet siirtävät sitä, Enter asettaa sen, Escape peruu"},
    {Str::PlaceTile, "ruutu %1$d, %2$d"},
    {Str::PlaceOffLot, "tontin ulkopuolella"},
    {Str::PlaceFits, "mahtuu"},
    {Str::PlaceBlocked, "ei mahdu tähän"},
    {Str::PlacePlaced, "%s asetettu"},
    {Str::PlaceFacing, "suunta: %s"},
    {Str::PlaceNoSpot, "ei mahdu minnekään tontilla, ei mihinkään suuntaan"},
    {Str::PlaceRoomSpots, "%1$s, %2$d paikkaa"},
    {Str::PlaceWallSide, "seinä: %s"},
    {Str::PlaceLeadsTo, "johtaa: %s"},
    {Str::PlaceLeadsToAnother, "johtaa toiseen huoneeseen: %s"},
    {Str::PlaceMoving,
     "siirrä %s: Page Down etsii paikat, Enter asettaa sen, Delete myy sen, Escape palauttaa sen"},
    {Str::PlaceCannotPick, "%s ei voi nostaa"},
    {Str::PlaceSold, "%s myyty"},
    // Build mode
    {Str::BuildBegin,
     "%s: nuolet siirtävät kulmaa, välilyönti merkitsee alun, Enter rakentaa, Ctrl+Enter poistaa, Escape lopettaa"},
    {Str::BuildCorner, "kulma %1$d, %2$d"},
    {Str::BuildMarked, "alku merkitty kohtaan %1$d, %2$d"},
    {Str::BuildNoMark, "merkitse ensin alku välilyönnillä"},
    {Str::BuildDone, "rakennettu hintaan %1$s, jäljellä %2$s"},
    {Str::BuildRemoved, "poistettu, %1$s takaisin, %2$s"},
    {Str::BuildNothing, "mikään ei muuttunut"},
    {Str::BalloonThinks, "%1$s ajattelee: %2$s"},
    {Str::BalloonDreams, "%1$s näkee unta: %2$s"},
    {Str::BalloonSays, "%1$s sanoo: %2$s"},
    {Str::BalloonScreams, "%1$s huutaa: %2$s"},
    {Str::BalloonScreamsNothing, "%s huutaa"},
    {Str::BalloonCrossed, "ei %s"},
    {Str::BalloonRelationUp, "%s: suhde paranee"},
    {Str::BalloonRelationUpMuch, "%s: suhde paranee paljon"},
    {Str::BalloonRelationDown, "%s: suhde huononee"},
    {Str::BalloonRelationDownMuch, "%s: suhde huononee paljon"},
    {Str::BalloonRomance, "%s: romanssi"},
    {Str::BalloonRomanceBroken, "%s: romanssi päättyi"},
    {Str::BalloonCrush, "%s: ihastus"},
    {Str::BalloonCrushBroken, "%s: ihastus ohi"},
    {Str::BalloonMomentBad, "%s: huono hetki"},
    {Str::BalloonMomentGood, "%s: hieno hetki"},
    {Str::BalloonFirstKiss, "%s: ensisuudelma"},
    {Str::BalloonFirstLove, "%s: ensirakkaus"},
    {Str::BalloonInLove, "%s: rakastunut"},
    {Str::BalloonLoveLost, "%s: rakkaus menetetty"},
    {Str::BalloonFriend, "%s: uusi ystävä"},
    {Str::BalloonFriendLost, "%s: ystävä menetetty"},
    {Str::BalloonBestFriend, "%s: uusi paras ystävä"},
    {Str::BalloonBestFriendLost, "%s: paras ystävä menetetty"},
    {Str::BalloonEnemy, "%s: uusi vihollinen"},
    {Str::BalloonEnemyLost, "%s: ei enää vihollisia"},
    {Str::BalloonMemoryGood, "%s: hyvä muisto"},
    {Str::BalloonMemoryBad, "%s: huono muisto"},
    {Str::BalloonTopicLiked, "%s: pitää aiheesta"},
    {Str::BalloonTopicDisliked, "%s: ei pidä aiheesta"},
    {Str::BalloonPersonalityGood, "%s: persoonallisuudet sopivat"},
    {Str::BalloonPersonalityBad, "%s: persoonallisuudet törmäävät"},
    {Str::BalloonRouteFailed, "%s ei pääse sinne"},
    {Str::IconAmbulance, "ambulanssi"},
    {Str::IconPhone, "puhelin"},
    {Str::IconPoolLadder, "uima-altaan tikkaat"},
    {Str::IconQuestion, "kysymysmerkki"},
    {Str::IconFingerprint, "sormenjälki"},
    {Str::IconBaby, "vauva"},
    {Str::IconDoor, "ovi"},
    {Str::IconBabyQuestion, "vauva?"},
    {Str::FamilyTreePicked, "%s valittu: Enter toisen simin kohdalla yhdistää heidät, Enter uudelleen peruu"},
    {Str::FurnitureGone, "%1$s on poissa tontilta, arvo %2$s"},
    {Str::BuildShutIn, "varo: huoneessa %2$s ei ole ovea ja %1$s on sisällä; Ctrl+Z peruu seinän"},
    {Str::KeysLotMore, "Ctrl+L huoneen pohjapiirros, Ctrl+N mitä kertoja sanoo, Ctrl+F seuraako kamera simiä."},
    {Str::GroupBusiness, "yritys"},
    {Str::GroupSeasons, "vuodenajat"},
    {Str::GroupDate, "treffit"},
};

}  // namespace

extern const Translations kFinnish = {kEntries, std::size(kEntries)};

}  // namespace lang
