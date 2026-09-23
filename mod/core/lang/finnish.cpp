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
    // Money
    {Str::FundsNow, "rahat %s, %s"},
    {Str::FundsChanged, "%1$s, jäljellä %2$s"},
};

}  // namespace

extern const Translations kFinnish = {kEntries, std::size(kEntries)};

}  // namespace lang
