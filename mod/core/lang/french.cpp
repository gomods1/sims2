// French: the game's language 2. Written by tools/mod_strings.py from a translation of
// util/strings_table.h; keep the order and groups of the English table.
#include "translations.h"

#include <iterator>

namespace lang {
namespace {

const Translation kEntries[] = {
    // Startup
    {Str::ModLoaded, "Accessibilité pour Les Sims 2 chargée"},
    {Str::UnsupportedBuild, "Accessibilité pour Les Sims 2 : version du jeu non prise en charge, mod désactivé"},
    // Key help (Ctrl+F1)
    {Str::KeysScreen,
     "Les flèches se déplacent dans un groupe, Tab et Maj+Tab changent de groupe, Origine et Fin vont aux "
     "extrémités, Entrée active, R répète. Contrôle+flèche haut et Contrôle+flèche bas lisent les détails de ce "
     "qui a été dit, Contrôle+flèche gauche et Contrôle+flèche droite passent des détails aux événements."},
    {Str::KeysLot,
     "Terrain : Page suivante et précédente, les objets du terrain, les plus proches d'abord ; avec Maj, les "
     "catégories ; avec Contrôle, les pièces. Entrée ouvre les interactions d'un objet, ou le prend en mode "
     "Achat ; Contrôle+Entrée y envoie le Sim sélectionné. Contrôle+E, l'état du Sim ; Contrôle+1 à 9 "
     "choisissent un Sim ; Tab, les panneaux ; Contrôle+haut et bas, les détails."},
    {Str::KeysPlacement,
     "Placement : Page suivante et précédente, les places où il tient ; avec Contrôle, les pièces ; flèches, une "
     "case ; Origine, retour au Sim ; virgule et point le tournent ; Entrée le pose ; Suppr le vend ; Échap "
     "arrête."},
    {Str::KeysBuild,
     "Construction : les flèches déplacent le coin, avec Maj cinq ; Page précédente et suivante, le niveau ; "
     "Espace marque le début ; Entrée construit depuis la marque ; Contrôle+Entrée supprime ; Origine retour au "
     "Sim ; R répète ; Échap quitte."},
    {Str::KeysNeighborhood,
     "Quartier : d'abord la liste des terrains ; Entrée ouvre le panneau d'un terrain. Pour installer une "
     "famille, choisissez-la dans la réserve de familles puis Entrée sur un terrain. Tab, les panneaux."},
    // Control state
    {Str::Checked, "coché"},
    {Str::NotChecked, "non coché"},
    {Str::Selected, "sélectionné"},
    {Str::TabSelected, "sélectionné"},
    {Str::Unavailable, "indisponible"},
    {Str::UnlabeledButton, "bouton sans nom"},
    {Str::Unnamed, "sans nom"},
    // Control roles: what a control is, said after its name
    {Str::RoleButton, "bouton"},
    {Str::RoleTab, "onglet"},
    {Str::RoleCheckBox, "case à cocher"},
    {Str::RoleSlider, "glissière"},
    {Str::RolePicker, "sélecteur"},
    {Str::NothingHere, "rien à parcourir ici"},
    // Review buffers
    {Str::BufferDetails, "détails"},
    {Str::BufferEvents, "événements"},
    {Str::BufferEmpty, "%s, vide"},
    {Str::PositionInGroup, "%d sur %d, %s"},
    // Groups
    {Str::GroupSummary, "%s, %d éléments"},
    {Str::GroupSummaryOne, "%s, 1 élément"},
    {Str::GroupButtons, "boutons"},
    {Str::GroupText, "texte"},
    {Str::GroupControlPanel, "panneau de contrôle"},
    {Str::GroupLivePanel, "panneau du Sim"},
    {Str::GroupFamily, "famille"},
    {Str::GroupWantsFears, "souhaits et peurs"},
    {Str::GroupNeeds, "besoins"},
    {Str::GroupControls, "contrôles"},
    {Str::GroupBuyPanel, "mode Achat"},
    {Str::GroupBuildPanel, "mode Construction"},
    {Str::GroupCatalog, "catalogue"},
    {Str::GroupStarSign, "signe astrologique"},
    {Str::GroupAspiration, "aspiration"},
    {Str::GroupSteps, "étapes"},
    {Str::GroupAge, "âge"},
    {Str::GroupSex, "sexe"},
    // Text fields
    {Str::TextField, "%s %s, champ de texte"},
    {Str::Blank, "vide"},
    {Str::Space, "espace"},
    // Values
    {Str::ValueOfMax, "%d sur %d"},
    {Str::SliderRange, "de %1$s à %2$s"},
    {Str::PointsLeft, "%d points restants"},
    {Str::LabelValue, "%s : %s"},
    {Str::NextPage, "page suivante"},
    {Str::PreviousPage, "page précédente"},
    // Interaction menu
    {Str::PieMenu, "interactions"},
    {Str::NoInteractions, "aucune interaction"},
    {Str::Cancelled, "annulé"},
    // Cheat console
    {Str::CheatConsole, "console de triche"},
    {Str::CheatRan, "code : %s"},
    {Str::CheatEmpty, "tapez ou choisissez d'abord un code"},
    // Neighborhood: lots and families
    {Str::GroupLots, "terrains"},
    {Str::LotFamily, "famille %s"},
    {Str::LotForSale, "à vendre"},
    {Str::LotCommunity, "terrain communautaire"},
    {Str::LotApartmentsEmpty, "immeuble d'appartements, sans locataires"},
    {Str::LotApartmentsWith, "immeuble d'appartements : %s"},
    {Str::FamilySummary, "%1$s : %2$d membres, fonds %3$s"},
    // Money
    {Str::Simoleons, "simflouz"},
    {Str::FundsNow, "fonds %s, %s"},
    // Lot: object browser (PageUp/PageDown)
    {Str::CategorySims, "Sims"},
    {Str::CategoryOther, "autres objets"},
    {Str::NoObjects, "aucun objet ici"},
    {Str::FloorNumber, "niveau %d"},
    {Str::NotOnLot, "vous n'êtes pas sur un terrain"},
    {Str::HereNow, "ici même"},
    {Str::TilesAway, "à %d cases direction %s"},
    {Str::OneTileAway, "à 1 case direction %s"},
    {Str::DirNorth, "nord"},
    {Str::DirNorthEast, "nord-est"},
    {Str::DirEast, "est"},
    {Str::DirSouthEast, "sud-est"},
    {Str::DirSouth, "sud"},
    {Str::DirSouthWest, "sud-ouest"},
    {Str::DirWest, "ouest"},
    {Str::DirNorthWest, "nord-ouest"},
    {Str::OtherFloor, "à un autre niveau"},
    {Str::SelectedSimSuffix, "Sim sélectionné"},
    {Str::NoInteractionsAtAll, "aucune interaction : on ne peut rien faire avec cet objet"},
    {Str::WalkingThere, "en route"},
    {Str::CannotWalk, "impossible d'y aller"},
    {Str::NoSimSelected, "aucun Sim sélectionné"},
    // Needs
    {Str::MotiveValue, "%s, %d pour cent"},
    // Lot: status (Ctrl+E) and action queue
    {Str::GroupActionQueue, "file d'actions"},
    {Str::StatusIdle, "ne fait rien"},
    {Str::StatusNeedsLow, "besoins bas : %s"},
    {Str::StatusNeedsFine, "besoins satisfaits"},
    // Narrator: the family watch
    {Str::ActionStarted, "%1$s : %2$s"},
    {Str::ActionFinished, "%1$s a fini : %2$s"},
    {Str::NeedLow, "%1$s, besoin bas : %2$s"},
    {Str::NeedUrgent, "%1$s, besoin urgent : %2$s"},
    // Lot: selecting Sims
    {Str::SimSelectedNow, "%s, Sim sélectionné"},
    {Str::NoSuchSim, "pas de Sim %d"},
    // Narrator: visitors
    {Str::VisitorArrived, "%s arrive"},
    {Str::VisitorLeft, "%s s'en va"},
    // Lot: rooms
    {Str::RoomObjects, "%s, %d objets"},
    {Str::RoomObjectsOne, "%s, 1 objet"},
    {Str::FundsChanged, "%1$s, il reste %2$s"},
    // Buy mode
    {Str::PlaceBegin,
     "%s : Page suivante trouve les places où il tient, les flèches le déplacent, Entrée le pose, Échap annule"},
    {Str::PlaceTile, "case %1$d, %2$d"},
    {Str::PlaceOffLot, "hors du terrain"},
    {Str::PlaceFits, "il tient"},
    {Str::PlaceBlocked, "ne tient pas ici"},
    {Str::PlacePlaced, "%s posé"},
    {Str::PlaceFacing, "orientation : %s"},
    {Str::PlaceNoSpot, "il ne tient nulle part sur ce terrain dans cette orientation"},
    {Str::PlaceRoomSpots, "%1$s, %2$d places"},
    {Str::PlaceMoving,
     "déplacer %s : Page suivante trouve des places, Entrée le pose, Suppr le vend, Échap le remet"},
    {Str::PlaceCannotPick, "%s ne peut pas être pris"},
    {Str::PlaceSold, "%s vendu"},
    // Build mode
    {Str::BuildBegin,
     "%s : les flèches déplacent le coin, Espace marque le début, Entrée construit, Contrôle+Entrée supprime, "
     "Échap quitte"},
    {Str::BuildCorner, "coin %1$d, %2$d"},
    {Str::BuildMarked, "début marqué en %1$d, %2$d"},
    {Str::BuildNoMark, "marque d'abord le début avec Espace"},
    {Str::BuildDone, "construit pour %1$s, il reste %2$s"},
    {Str::BuildRemoved, "supprimé, %1$s rendus, %2$s"},
    {Str::BuildNothing, "rien n'a changé"},
};

}  // namespace

extern const Translations kFrench = {kEntries, std::size(kEntries)};

}  // namespace lang
