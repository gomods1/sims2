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
    {Str::KeysLot,
     "Lote: Page Down e Page Up, os objetos do lote, os mais próximos primeiro; com Shift as categorias, com "
     "Ctrl os cômodos. Enter abre as interações de um objeto, ou o pega no modo Compra; Ctrl+Enter leva o Sim "
     "selecionado até lá. Ctrl+E o estado do Sim, Ctrl+1 a 9 escolhem um Sim, Tab os painéis, Ctrl+seta para "
     "cima e para baixo os detalhes."},
    {Str::KeysPlacement,
     "Colocando: Page Down e Page Up os lugares onde cabe, com Ctrl os cômodos, setas um quadrado, Home volta ao "
     "Sim, vírgula e ponto o giram, Enter o coloca, Delete o vende, Esc termina."},
    {Str::KeysBuild,
     "Construindo: as setas movem o canto, com Shift cinco, Page Up e Page Down o nível, Espaço marca o início, "
     "Enter constrói a partir da marca, Ctrl+Enter remove, Home volta ao Sim, R repete, Esc sai."},
    {Str::KeysNeighborhood,
     "Vizinhança: primeiro a lista de lotes, Enter abre o painel de um lote. Para mudar uma família, escolha-a "
     "na caixa de famílias e pressione Enter num lote. Tab os painéis."},
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
    {Str::GroupBuyPanel, "modo Compra"},
    {Str::GroupBuildPanel, "modo Construção"},
    {Str::GroupCatalog, "catálogo"},
    {Str::GroupStarSign, "signo"},
    {Str::GroupAspiration, "aspiração"},
    {Str::GroupSteps, "etapas"},
    {Str::GroupAge, "idade"},
    {Str::GroupSex, "sexo"},
    {Str::GroupTraits, "traços"},
    // Text fields
    {Str::TextField, "%s %s, campo de texto"},
    {Str::Blank, "vazio"},
    {Str::Space, "espaço"},
    // Values
    {Str::ValueOfMax, "%d de %d"},
    {Str::SliderRange, "de %1$s a %2$s"},
    {Str::RadioBetween, "entre %1$s e %2$s"},
    {Str::PointsLeft, "restam %d pontos"},
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
    // Lot: rooms
    {Str::RoomObjects, "%s, %d objetos"},
    {Str::RoomObjectsOne, "%s, 1 objeto"},
    {Str::FundsChanged, "%1$s, restam %2$s"},
    // Camera
    {Str::CameraFree, "câmera livre"},
    {Str::CameraFollowing, "seguindo %s"},
    // Narrator
    {Str::NarratorAll, "narrador: tudo"},
    {Str::NarratorWarnings, "narrador: só avisos"},
    {Str::NarratorOff, "narrador desligado"},
    // Relationships
    {Str::RelationshipValues, "%1$s, diária %2$s, permanente %3$s"},
    // Room layout (Ctrl+L)
    {Str::LayoutNoRoom, "não há cômodo aqui"},
    {Str::LayoutSummary, "%1$s, andar %2$d, %3$d quadrados, de %4$d, %5$d a %6$d, %7$d"},
    {Str::LayoutOpenings, "%1$d portas, %2$d janelas"},
    {Str::LayoutDoor, "porta %1$s, parede %2$s, dá para %3$s, quadrado %4$d, %5$d"},
    {Str::LayoutWindow, "janela %1$s, parede %2$s, dá para %3$s, quadrado %4$d, %5$d"},
    // Buy mode
    {Str::PlaceBegin, "%s: Page Down procura lugares onde cabe, as setas o movem, Enter o coloca, Esc cancela"},
    {Str::PlaceTile, "quadrado %1$d, %2$d"},
    {Str::PlaceOffLot, "fora do lote"},
    {Str::PlaceFits, "cabe"},
    {Str::PlaceBlocked, "não cabe aqui"},
    {Str::PlacePlaced, "%s colocado"},
    {Str::PlaceFacing, "virado para o %s"},
    {Str::PlaceNoSpot, "não cabe em lugar nenhum do lote, em nenhuma direção"},
    {Str::PlaceRoomSpots, "%1$s, %2$d lugares"},
    {Str::PlaceWallSide, "parede %s"},
    {Str::PlaceLeadsTo, "dá para %s"},
    {Str::PlaceLeadsToAnother, "dá para outro cômodo: %s"},
    {Str::PlaceMoving,
     "mover %s: Page Down procura lugares, Enter o coloca, Delete o vende, Esc o devolve ao lugar"},
    {Str::PlaceCannotPick, "%s não pode ser pego"},
    {Str::PlaceSold, "%s vendido"},
    // Build mode
    {Str::BuildBegin,
     "%s: as setas movem o canto, Espaço marca o início, Enter constrói, Ctrl+Enter remove, Esc sai"},
    {Str::BuildCorner, "canto %1$d, %2$d"},
    {Str::BuildMarked, "início marcado em %1$d, %2$d"},
    {Str::BuildNoMark, "marque primeiro o início com Espaço"},
    {Str::BuildDone, "construído por %1$s, restam %2$s"},
    {Str::BuildRemoved, "removido, %1$s de volta, %2$s"},
    {Str::BuildNothing, "nada mudou"},
    {Str::BalloonThinks, "%1$s pensa: %2$s"},
    {Str::BalloonDreams, "%1$s sonha: %2$s"},
    {Str::BalloonSays, "%1$s diz: %2$s"},
    {Str::BalloonScreams, "%1$s grita: %2$s"},
    {Str::BalloonScreamsNothing, "%s grita"},
    {Str::BalloonCrossed, "sem %s"},
    {Str::BalloonRelationUp, "%s: o relacionamento melhora"},
    {Str::BalloonRelationUpMuch, "%s: o relacionamento melhora muito"},
    {Str::BalloonRelationDown, "%s: o relacionamento piora"},
    {Str::BalloonRelationDownMuch, "%s: o relacionamento piora muito"},
    {Str::BalloonRomance, "%s: romance"},
    {Str::BalloonRomanceBroken, "%s: romance acabado"},
    {Str::BalloonCrush, "%s: paixonite"},
    {Str::BalloonCrushBroken, "%s: fim da paixonite"},
    {Str::BalloonMomentBad, "%s: um momento ruim"},
    {Str::BalloonMomentGood, "%s: um grande momento"},
    {Str::BalloonFirstKiss, "%s: primeiro beijo"},
    {Str::BalloonFirstLove, "%s: primeiro amor"},
    {Str::BalloonInLove, "%s: apaixonado"},
    {Str::BalloonLoveLost, "%s: amor perdido"},
    {Str::BalloonFriend, "%s: novo amigo"},
    {Str::BalloonFriendLost, "%s: amigo perdido"},
    {Str::BalloonBestFriend, "%s: novo melhor amigo"},
    {Str::BalloonBestFriendLost, "%s: melhor amigo perdido"},
    {Str::BalloonEnemy, "%s: novo inimigo"},
    {Str::BalloonEnemyLost, "%s: não são mais inimigos"},
    {Str::BalloonMemoryGood, "%s: boa lembrança"},
    {Str::BalloonMemoryBad, "%s: má lembrança"},
    {Str::BalloonTopicLiked, "%s: gosta do assunto"},
    {Str::BalloonTopicDisliked, "%s: não gosta do assunto"},
    {Str::BalloonPersonalityGood, "%s: personalidades combinam"},
    {Str::BalloonPersonalityBad, "%s: personalidades se chocam"},
    {Str::BalloonRouteFailed, "%s não consegue chegar lá"},
    {Str::IconAmbulance, "ambulância"},
    {Str::IconPhone, "telefone"},
    {Str::IconPoolLadder, "escada da piscina"},
    {Str::IconQuestion, "ponto de interrogação"},
    {Str::IconFingerprint, "impressão digital"},
    {Str::IconBaby, "bebê"},
    {Str::IconDoor, "porta"},
    {Str::IconBabyQuestion, "um bebê?"},
    {Str::FamilyTreePicked, "%s selecionado: Enter em outro Sim para ligá-los, Enter de novo para soltar"},
    {Str::FurnitureGone, "%1$s não está mais no lote, valor %2$s"},
    {Str::BuildShutIn, "cuidado: %2$s não tem porta e %1$s está lá dentro; Ctrl+Z desfaz a parede"},
    {Str::KeysLotMore, "Ctrl+L a planta do cômodo, Ctrl+N o que o narrador diz, Ctrl+F se a câmera segue o Sim."},
    {Str::GroupBusiness, "negócio"},
    {Str::GroupSeasons, "estações"},
    {Str::GroupDate, "encontro"},
};

}  // namespace

extern const Translations kPortuguese = {kEntries, std::size(kEntries)};

}  // namespace lang
