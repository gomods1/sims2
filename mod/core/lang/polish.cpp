// Polish: the game's language 20. Written by tools/mod_strings.py from a translation of
// util/strings_table.h; keep the order and groups of the English table.
#include "translations.h"

#include <iterator>

namespace lang {
namespace {

const Translation kEntries[] = {
    // Startup
    {Str::ModLoaded, "Moduł dostępności The Sims 2 wczytany"},
    {Str::UnsupportedBuild, "Moduł dostępności The Sims 2: nieobsługiwana wersja gry, mod wyłączony"},
    // Key help (Ctrl+F1)
    {Str::KeysScreen,
     "Strzałki poruszają się w obrębie grupy, Tab i Shift+Tab zmieniają grupę, Home i End przechodzą na początek "
     "i na koniec, Enter aktywuje, R powtarza. Control+strzałka w górę i Control+strzałka w dół czytają "
     "szczegóły tego, co zostało powiedziane, Control+strzałka w lewo i Control+strzałka w prawo przełączają "
     "między szczegółami a zdarzeniami."},
    {Str::KeysLot,
     "Parcela: Page Down i Page Up, obiekty parceli, najbliższe najpierw; z Shift kategorie, z Ctrl pokoje. "
     "Enter otwiera interakcje obiektu albo podnosi go w trybie kupowania; Ctrl+Enter wysyła tam wybranego Sima. "
     "Ctrl+E stan Sima, Ctrl+1 do 9 wybierają Sima, Tab panele, Ctrl+strzałka w górę i w dół szczegóły."},
    {Str::KeysPlacement,
     "Ustawianie: Page Down i Page Up miejsca, gdzie się mieści, z Ctrl pokoje, strzałki o jedno pole, Home "
     "powrót do Sima, przecinek i kropka obracają, Enter stawia, Delete sprzedaje, Escape kończy."},
    {Str::KeysBuild,
     "Budowanie: strzałki przesuwają narożnik, z Shift o pięć, Page Up i Page Down poziom, spacja zaznacza "
     "początek, Enter buduje od znacznika, Ctrl+Enter usuwa, Home powrót do Sima, R powtarza, Escape kończy."},
    {Str::KeysNeighborhood,
     "Okolica: najpierw lista parcel, Enter otwiera panel parceli. Aby wprowadzić rodzinę, wybierz ją w "
     "pojemniku rodzin i naciśnij Enter na parceli. Tab panele."},
    // Control state
    {Str::Checked, "zaznaczone"},
    {Str::NotChecked, "niezaznaczone"},
    {Str::Selected, "wybrany"},
    {Str::TabSelected, "wybrana"},
    {Str::Unavailable, "niedostępny"},
    {Str::UnlabeledButton, "przycisk bez nazwy"},
    {Str::Unnamed, "bez nazwy"},
    // Control roles: what a control is, said after its name
    {Str::RoleButton, "przycisk"},
    {Str::RoleTab, "karta"},
    {Str::RoleCheckBox, "pole wyboru"},
    {Str::RoleSlider, "suwak"},
    {Str::RolePicker, "selektor"},
    {Str::NothingHere, "nie ma tu nic do przeglądania"},
    // Review buffers
    {Str::BufferDetails, "szczegóły"},
    {Str::BufferEvents, "zdarzenia"},
    {Str::BufferEmpty, "%s, puste"},
    {Str::PositionInGroup, "%d z %d, %s"},
    // Groups
    {Str::GroupSummary, "%s, elementów: %d"},
    {Str::GroupSummaryOne, "%s, 1 element"},
    {Str::GroupButtons, "przyciski"},
    {Str::GroupText, "tekst"},
    {Str::GroupControlPanel, "panel sterowania"},
    {Str::GroupLivePanel, "panel Sima"},
    {Str::GroupFamily, "rodzina"},
    {Str::GroupWantsFears, "pragnienia i lęki"},
    {Str::GroupNeeds, "potrzeby"},
    {Str::GroupControls, "sterowanie"},
    {Str::GroupBuyPanel, "tryb kupowania"},
    {Str::GroupBuildPanel, "tryb budowania"},
    {Str::GroupCatalog, "katalog"},
    {Str::GroupStarSign, "znak zodiaku"},
    {Str::GroupAspiration, "aspiracja"},
    {Str::GroupSteps, "kroki"},
    {Str::GroupAge, "wiek"},
    {Str::GroupSex, "płeć"},
    // Text fields
    {Str::TextField, "%s %s, pole tekstowe"},
    {Str::Blank, "puste"},
    {Str::Space, "spacja"},
    // Values
    {Str::ValueOfMax, "%d z %d"},
    {Str::SliderRange, "od %1$s do %2$s"},
    {Str::PointsLeft, "pozostało punktów: %d"},
    {Str::LabelValue, "%s: %s"},
    {Str::NextPage, "następna strona"},
    {Str::PreviousPage, "poprzednia strona"},
    // Interaction menu
    {Str::PieMenu, "działania"},
    {Str::NoInteractions, "brak działań"},
    {Str::Cancelled, "anulowano"},
    // Cheat console
    {Str::CheatConsole, "konsola kodów"},
    {Str::CheatRan, "kod: %s"},
    {Str::CheatEmpty, "najpierw wpisz lub wybierz kod"},
    // Neighborhood: lots and families
    {Str::GroupLots, "parcele"},
    {Str::LotFamily, "rodzina %s"},
    {Str::LotForSale, "na sprzedaż"},
    {Str::LotCommunity, "parcela publiczna"},
    {Str::LotApartmentsEmpty, "blok mieszkalny, bez lokatorów"},
    {Str::LotApartmentsWith, "blok mieszkalny: %s"},
    {Str::FamilySummary, "%1$s: członkowie: %2$d, fundusze %3$s"},
    // Money
    {Str::Simoleons, "simoleonów"},
    {Str::FundsNow, "fundusze %s, %s"},
    // Lot: object browser (PageUp/PageDown)
    {Str::CategorySims, "Simowie"},
    {Str::CategoryOther, "inne obiekty"},
    {Str::NoObjects, "brak obiektów"},
    {Str::FloorNumber, "kondygnacja %d"},
    {Str::NotOnLot, "nie jesteś na parceli"},
    {Str::HereNow, "tuż obok"},
    {Str::TilesAway, "w kierunku %2$s, pól: %1$d"},
    {Str::OneTileAway, "w kierunku %s, 1 pole"},
    {Str::DirNorth, "północy"},
    {Str::DirNorthEast, "północnego wschodu"},
    {Str::DirEast, "wschodu"},
    {Str::DirSouthEast, "południowego wschodu"},
    {Str::DirSouth, "południa"},
    {Str::DirSouthWest, "południowego zachodu"},
    {Str::DirWest, "zachodu"},
    {Str::DirNorthWest, "północnego zachodu"},
    {Str::OtherFloor, "na innej kondygnacji"},
    {Str::SelectedSimSuffix, "aktywna postać"},
    {Str::NoInteractionsAtAll, "brak działań: z tym przedmiotem nic nie można zrobić"},
    {Str::WalkingThere, "w drodze"},
    {Str::CannotWalk, "nie może tam dojść"},
    {Str::NoSimSelected, "nie wybrano Sima"},
    // Needs
    {Str::MotiveValue, "%s, %d procent"},
    // Lot: status (Ctrl+E) and action queue
    {Str::GroupActionQueue, "kolejka działań"},
    {Str::StatusIdle, "nic nie robi"},
    {Str::StatusNeedsLow, "niskie potrzeby: %s"},
    {Str::StatusNeedsFine, "potrzeby w porządku"},
    // Narrator: the family watch
    {Str::ActionStarted, "%1$s: %2$s"},
    {Str::ActionFinished, "%1$s: koniec, %2$s"},
    {Str::NeedLow, "%1$s, niska potrzeba: %2$s"},
    {Str::NeedUrgent, "%1$s, pilna potrzeba: %2$s"},
    // Lot: selecting Sims
    {Str::SimSelectedNow, "aktywna postać: %s"},
    {Str::NoSuchSim, "brak Sima numer %d"},
    // Narrator: visitors
    {Str::VisitorArrived, "przybywa: %s"},
    {Str::VisitorLeft, "odchodzi: %s"},
    // Lot: rooms
    {Str::RoomObjects, "%s, obiektów: %d"},
    {Str::RoomObjectsOne, "%s, 1 obiekt"},
    {Str::FundsChanged, "%1$s, zostało %2$s"},
    // Buy mode
    {Str::PlaceBegin,
     "%s: Page Down szuka miejsc, gdzie się mieści, strzałki go przesuwają, Enter go stawia, Escape anuluje"},
    {Str::PlaceTile, "pole %1$d, %2$d"},
    {Str::PlaceOffLot, "poza parcelą"},
    {Str::PlaceFits, "mieści się"},
    {Str::PlaceBlocked, "tu się nie mieści"},
    {Str::PlacePlaced, "%s postawiony"},
    {Str::PlaceFacing, "skierowany w stronę %s"},
    {Str::PlaceNoSpot, "tak obrócony nie mieści się nigdzie na tej parceli"},
    {Str::PlaceRoomSpots, "%1$s, miejsc: %2$d"},
    {Str::PlaceMoving,
     "przenoszenie: %s. Page Down szuka miejsc, Enter stawia, Delete sprzedaje, Escape odkłada na miejsce"},
    {Str::PlaceCannotPick, "nie można podnieść: %s"},
    {Str::PlaceSold, "sprzedano: %s"},
    // Build mode
    {Str::BuildBegin,
     "%s: strzałki przesuwają narożnik, spacja zaznacza początek, Enter buduje, Ctrl+Enter usuwa, Escape kończy"},
    {Str::BuildCorner, "narożnik %1$d, %2$d"},
    {Str::BuildMarked, "początek zaznaczony w %1$d, %2$d"},
    {Str::BuildNoMark, "najpierw zaznacz początek spacją"},
    {Str::BuildDone, "zbudowano za %1$s, zostało %2$s"},
    {Str::BuildRemoved, "usunięto, zwrot %1$s, %2$s"},
    {Str::BuildNothing, "nic się nie zmieniło"},
};

}  // namespace

extern const Translations kPolish = {kEntries, std::size(kEntries)};

}  // namespace lang
