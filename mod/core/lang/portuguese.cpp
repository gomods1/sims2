// Portuguese (Brazil): the game's language 10 and 23. Written by tools/mod_strings.py from a translation of
// util/strings_table.h; keep the order and groups of the English table.
#include "translations.h"

#include <iterator>

namespace lang {
namespace {

const Translation kEntries[] = {
    // Startup
    {Str::ModLoaded, "Acessibilidade para The Sims 2 carregada"},
    {Str::UnsupportedBuild, "Acessibilidade para The Sims 2: versão do jogo não suportada, mod desativado"},
    // Key help (Ctrl+F1)
    {Str::KeysScreen,
     "As setas se movem dentro de um grupo, Tab e Shift+Tab mudam de grupo, Home e End vão para os extremos, "
     "Enter aciona, R repete. Control+Seta para cima e Control+Seta para baixo leem os detalhes do que foi dito, "
     "Control+Seta para a esquerda e Control+Seta para a direita alternam entre detalhes e eventos."},
    // Control state
    {Str::Checked, "marcado"},
    {Str::NotChecked, "não marcado"},
    {Str::Selected, "selecionado"},
    {Str::TabSelected, "selecionada"},
    {Str::Unavailable, "indisponível"},
    {Str::UnlabeledButton, "botão sem nome"},
    {Str::Unnamed, "sem nome"},
    // Control roles: what a control is, said after its name
    {Str::RoleButton, "botão"},
    {Str::RoleTab, "guia"},
    {Str::RoleCheckBox, "caixa de seleção"},
    {Str::RoleSlider, "controle deslizante"},
    {Str::RolePicker, "seletor"},
    {Str::NothingHere, "nada para percorrer aqui"},
    // Review buffers
    {Str::BufferDetails, "detalhes"},
    {Str::BufferEvents, "eventos"},
    {Str::BufferEmpty, "%s, vazio"},
    {Str::PositionInGroup, "%d de %d, %s"},
    // Groups
    {Str::GroupSummary, "%s, %d itens"},
    {Str::GroupSummaryOne, "%s, 1 item"},
    {Str::GroupButtons, "botões"},
    {Str::GroupText, "texto"},
    {Str::GroupControlPanel, "painel de controle"},
    {Str::GroupLivePanel, "painel do Sim"},
    {Str::GroupFamily, "família"},
    {Str::GroupWantsFears, "desejos e medos"},
    {Str::GroupNeeds, "necessidades"},
    {Str::GroupControls, "controles"},
    // Text fields
    {Str::TextField, "%s %s, campo de texto"},
    {Str::Blank, "vazio"},
    {Str::Space, "espaço"},
    // Values
    {Str::ValueOfMax, "%d de %d"},
    {Str::LabelValue, "%s: %s"},
    {Str::NextPage, "próxima página"},
    {Str::PreviousPage, "página anterior"},
    // Interaction menu
    {Str::PieMenu, "interações"},
    {Str::NoInteractions, "sem interações"},
    {Str::Cancelled, "cancelado"},
    // Cheat console
    {Str::CheatConsole, "console de códigos"},
    {Str::CheatRan, "código: %s"},
    {Str::CheatEmpty, "digite ou escolha um código primeiro"},
    // Neighborhood: lots and families
    {Str::GroupLots, "lotes"},
    {Str::LotFamily, "família %s"},
    {Str::LotForSale, "à venda"},
    {Str::LotCommunity, "lote comunitário"},
    {Str::LotApartmentsEmpty, "prédio de apartamentos, sem inquilinos"},
    {Str::LotApartmentsWith, "prédio de apartamentos: %s"},
    {Str::FamilySummary, "%1$s: %2$d membros, fundos %3$s"},
    // Money
    {Str::Simoleons, "simoleons"},
    {Str::FundsNow, "fundos %s, %s"},
    // Lot: object browser (PageUp/PageDown)
    {Str::CategorySims, "Sims"},
    {Str::CategoryOther, "outros objetos"},
    {Str::NoObjects, "nenhum objeto aqui"},
    {Str::FloorNumber, "andar %d"},
    {Str::NotOnLot, "você não está em um lote"},
    {Str::HereNow, "bem aqui"},
    {Str::TilesAway, "a %d quadrados ao %s"},
    {Str::OneTileAway, "a 1 quadrado ao %s"},
    {Str::DirNorth, "norte"},
    {Str::DirNorthEast, "nordeste"},
    {Str::DirEast, "leste"},
    {Str::DirSouthEast, "sudeste"},
    {Str::DirSouth, "sul"},
    {Str::DirSouthWest, "sudoeste"},
    {Str::DirWest, "oeste"},
    {Str::DirNorthWest, "noroeste"},
    {Str::OtherFloor, "em outro andar"},
    {Str::SelectedSimSuffix, "selecionado"},
    {Str::NoInteractionsAtAll, "nenhuma interação: não dá para fazer nada com este objeto"},
    {Str::WalkingThere, "indo até lá"},
    {Str::CannotWalk, "não consegue ir até lá"},
    {Str::NoSimSelected, "nenhum Sim selecionado"},
    // Needs
    {Str::MotiveValue, "%s, %d por cento"},
    // Lot: status (Ctrl+E) and action queue
    {Str::GroupActionQueue, "fila de ações"},
    {Str::StatusIdle, "sem fazer nada"},
    {Str::StatusNeedsLow, "necessidades baixas: %s"},
    {Str::StatusNeedsFine, "necessidades em dia"},
    // Narrator: the family watch
    {Str::ActionStarted, "%1$s: %2$s"},
    {Str::ActionFinished, "%1$s terminou: %2$s"},
    {Str::NeedLow, "%1$s, necessidade baixa: %2$s"},
    {Str::NeedUrgent, "%1$s, necessidade urgente: %2$s"},
    // Lot: selecting Sims
    {Str::SimSelectedNow, "%s, selecionado"},
    {Str::NoSuchSim, "não há Sim %d"},
    // Narrator: visitors
    {Str::VisitorArrived, "%s chegou"},
    {Str::VisitorLeft, "%s foi embora"},
    {Str::FundsChanged, "%1$s, restam %2$s"},
};

}  // namespace

extern const Translations kPortuguese = {kEntries, std::size(kEntries)};

}  // namespace lang
