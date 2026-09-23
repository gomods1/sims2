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
    // Text fields
    {Str::TextField, "%s %s, champ de texte"},
    {Str::Blank, "vide"},
    {Str::Space, "espace"},
    // Values
    {Str::ValueOfMax, "%d sur %d"},
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
    {Str::FundsChanged, "%1$s, il reste %2$s"},
};

}  // namespace

extern const Translations kFrench = {kEntries, std::size(kEntries)};

}  // namespace lang
