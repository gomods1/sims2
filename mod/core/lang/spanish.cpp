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
    {Str::GroupControls, "controles"},
    // Text fields
    {Str::TextField, "%s %s, campo de texto"},
    {Str::Blank, "vacío"},
    {Str::Space, "espacio"},
    // Values
    {Str::ValueOfMax, "%d de %d"},
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
    // Money
    {Str::FundsNow, "fondos %s, %s"},
    {Str::FundsChanged, "%1$s, quedan %2$s"},
};

}  // namespace

extern const Translations kSpanish = {kEntries, std::size(kEntries)};

}  // namespace lang
