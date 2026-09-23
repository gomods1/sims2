// Spanish (Spain): the game's language 5. Written by tools/mod_strings.py from a translation of
// util/strings_table.h; keep the order and groups of the English table.
#include "translations.h"

#include <iterator>

namespace lang {
namespace {

const Translation kEntries[] = {
    // Startup
    {Str::ModLoaded, "Accesibilidad de Los Sims 2 cargada"},
    {Str::UnsupportedBuild, "Accesibilidad de Los Sims 2: versión del juego no compatible, mod desactivado"},
    // Key help (Ctrl+F1)
    {Str::KeysScreen,
     "Las flechas se mueven dentro de un grupo, Tab y Mayúsculas+Tab cambian de grupo, Inicio y Fin van a los "
     "extremos, Intro pulsa, R repite. Control+Arriba y Control+Abajo leen los detalles de lo dicho, "
     "Control+Izquierda y Control+Derecha cambian entre detalles y eventos."},
    // Control state
    {Str::Checked, "marcado"},
    {Str::NotChecked, "no marcado"},
    {Str::Selected, "seleccionado"},
    {Str::TabSelected, "seleccionada"},
    {Str::Unavailable, "no disponible"},
    {Str::UnlabeledButton, "botón sin nombre"},
    {Str::Unnamed, "sin nombre"},
    // Control roles: what a control is, said after its name
    {Str::RoleButton, "botón"},
    {Str::RoleTab, "pestaña"},
    {Str::RoleCheckBox, "casilla de verificación"},
    {Str::RoleSlider, "control deslizante"},
    {Str::RolePicker, "selector"},
    {Str::NothingHere, "no hay nada que recorrer aquí"},
    // Review buffers
    {Str::BufferDetails, "detalles"},
    {Str::BufferEvents, "eventos"},
    {Str::BufferEmpty, "%s, vacío"},
    {Str::PositionInGroup, "%d de %d, %s"},
    // Groups
    {Str::GroupSummary, "%s, %d elementos"},
    {Str::GroupSummaryOne, "%s, 1 elemento"},
    {Str::GroupButtons, "botones"},
    {Str::GroupText, "texto"},
    {Str::GroupControlPanel, "panel de control"},
    {Str::GroupLivePanel, "panel del Sim"},
    {Str::GroupFamily, "familia"},
    {Str::GroupWantsFears, "deseos y miedos"},
    {Str::GroupNeeds, "necesidades"},
    {Str::GroupControls, "controles"},
    {Str::GroupBuyPanel, "modo Comprar"},
    {Str::GroupBuildPanel, "modo Construir"},
    {Str::GroupCatalog, "catálogo"},
    {Str::GroupStarSign, "signo del zodiaco"},
    {Str::GroupAspiration, "aspiración"},
    {Str::GroupSteps, "pasos"},
    {Str::GroupAge, "edad"},
    {Str::GroupSex, "sexo"},
    // Text fields
    {Str::TextField, "%s %s, campo de texto"},
    {Str::Blank, "vacío"},
    {Str::Space, "espacio"},
    // Values
    {Str::ValueOfMax, "%d de %d"},
    {Str::SliderRange, "de %1$s a %2$s"},
    {Str::PointsLeft, "quedan %d puntos"},
    {Str::LabelValue, "%s: %s"},
    {Str::NextPage, "página siguiente"},
    {Str::PreviousPage, "página anterior"},
    // Interaction menu
    {Str::PieMenu, "interacciones"},
    {Str::NoInteractions, "sin interacciones"},
    {Str::Cancelled, "cancelado"},
    // Cheat console
    {Str::CheatConsole, "consola de trucos"},
    {Str::CheatRan, "truco: %s"},
    {Str::CheatEmpty, "escribe o elige antes un truco"},
    // Neighborhood: lots and families
    {Str::GroupLots, "solares"},
    {Str::LotFamily, "familia %s"},
    {Str::LotForSale, "en venta"},
    {Str::LotCommunity, "solar comunitario"},
    {Str::LotApartmentsEmpty, "edificio de apartamentos, sin inquilinos"},
    {Str::LotApartmentsWith, "edificio de apartamentos: %s"},
    {Str::FamilySummary, "%1$s: %2$d miembros, fondos %3$s"},
    // Money
    {Str::Simoleons, "simoleones"},
    {Str::FundsNow, "fondos %s, %s"},
    // Lot: object browser (PageUp/PageDown)
    {Str::CategorySims, "Sims"},
    {Str::CategoryOther, "otros objetos"},
    {Str::NoObjects, "no hay objetos aquí"},
    {Str::FloorNumber, "piso %d"},
    {Str::NotOnLot, "no estás en un solar"},
    {Str::HereNow, "aquí mismo"},
    {Str::TilesAway, "a %d casillas al %s"},
    {Str::OneTileAway, "a 1 casilla al %s"},
    {Str::DirNorth, "norte"},
    {Str::DirNorthEast, "noreste"},
    {Str::DirEast, "este"},
    {Str::DirSouthEast, "sureste"},
    {Str::DirSouth, "sur"},
    {Str::DirSouthWest, "suroeste"},
    {Str::DirWest, "oeste"},
    {Str::DirNorthWest, "noroeste"},
    {Str::OtherFloor, "en otro piso"},
    {Str::SelectedSimSuffix, "seleccionado"},
    {Str::NoInteractionsAtAll, "sin interacciones: con este objeto no se puede hacer nada"},
    {Str::WalkingThere, "yendo hacia allí"},
    {Str::CannotWalk, "no puede ir allí"},
    {Str::NoSimSelected, "ningún Sim seleccionado"},
    // Needs
    {Str::MotiveValue, "%s, %d por ciento"},
    // Lot: status (Ctrl+E) and action queue
    {Str::GroupActionQueue, "cola de acciones"},
    {Str::StatusIdle, "sin hacer nada"},
    {Str::StatusNeedsLow, "necesidades bajas: %s"},
    {Str::StatusNeedsFine, "necesidades bien"},
    // Narrator: the family watch
    {Str::ActionStarted, "%1$s: %2$s"},
    {Str::ActionFinished, "%1$s terminó: %2$s"},
    {Str::NeedLow, "%1$s, necesidad baja: %2$s"},
    {Str::NeedUrgent, "%1$s, necesidad urgente: %2$s"},
    // Lot: selecting Sims
    {Str::SimSelectedNow, "%s, seleccionado"},
    {Str::NoSuchSim, "no hay Sim %d"},
    // Narrator: visitors
    {Str::VisitorArrived, "%s ha llegado"},
    {Str::VisitorLeft, "%s se ha ido"},
    // Lot: rooms
    {Str::RoomObjects, "%s, %d objetos"},
    {Str::RoomObjectsOne, "%s, 1 objeto"},
    {Str::FundsChanged, "%1$s, quedan %2$s"},
    // Buy mode
    {Str::PlaceBegin,
     "%s: Avance de página busca sitios donde cabe, las flechas lo mueven, Intro lo coloca, Escape cancela"},
    {Str::PlaceTile, "casilla %1$d, %2$d"},
    {Str::PlaceOffLot, "fuera del solar"},
    {Str::PlaceFits, "cabe"},
    {Str::PlaceBlocked, "aquí no cabe"},
    {Str::PlacePlaced, "%s colocado"},
    {Str::PlaceFacing, "mirando al %s"},
    {Str::PlaceNoSpot, "así girado no cabe en ningún sitio del solar"},
    {Str::PlaceRoomSpots, "%1$s, %2$d sitios"},
    {Str::PlaceMoving,
     "mover %s: Avance de página busca sitios, Intro lo coloca, Suprimir lo vende, Escape lo devuelve a su sitio"},
    {Str::PlaceCannotPick, "%s no se puede coger"},
    {Str::PlaceSold, "%s vendido"},
    // Build mode
    {Str::BuildBegin,
     "%s: las flechas mueven la esquina, Espacio marca el inicio, Intro construye, Control+Intro quita, Escape "
     "sale"},
    {Str::BuildCorner, "esquina %1$d, %2$d"},
    {Str::BuildMarked, "inicio marcado en %1$d, %2$d"},
    {Str::BuildNoMark, "marca primero el inicio con Espacio"},
    {Str::BuildDone, "construido por %1$s, quedan %2$s"},
    {Str::BuildRemoved, "quitado, se devuelven %1$s, %2$s"},
    {Str::BuildNothing, "no ha cambiado nada"},
};

}  // namespace

extern const Translations kSpanish = {kEntries, std::size(kEntries)};

}  // namespace lang
