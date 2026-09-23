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
    // Money
    {Str::FundsNow, "fundos %s, %s"},
    {Str::FundsChanged, "%1$s, restam %2$s"},
};

}  // namespace

extern const Translations kPortuguese = {kEntries, std::size(kEntries)};

}  // namespace lang
