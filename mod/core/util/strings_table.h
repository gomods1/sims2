// Every string the mod speaks, in English: the source the translations in lang/ follow.
// X(Id, "English"), grouped by feature. A translation keeps the printf placeholders (it may
// reorder them as %1$s, %2$d); tools/mod_strings.py checks that and lists missing translations.
#pragma once

#define SA_STRINGS(X)                                                                                                  \
    /* Startup */                                                                                                      \
    X(ModLoaded, "Sims 2 accessibility loaded")                                                                        \
    X(UnsupportedBuild, "Sims 2 accessibility: unsupported game version, mod disabled")                                \
    /* Key help (Ctrl+F1) */                                                                                           \
    X(KeysScreen,                                                                                                      \
      "Arrows move within a group, Tab and Shift+Tab change group, Home and End go to the ends, Enter presses, R "     \
      "repeats. Control+Up and Control+Down read the details of what was said, Control+Left and Control+Right "        \
      "switch between details and events.")                                                                            \
    /* Control state */                                                                                                \
    X(Checked, "checked")                                                                                              \
    X(NotChecked, "not checked")                                                                                       \
    X(Selected, "selected")                                                                                            \
    X(TabSelected, "selected")                                                                                         \
    X(Unavailable, "unavailable")                                                                                      \
    X(UnlabeledButton, "unlabeled button")                                                                             \
    X(Unnamed, "unnamed")                                                                                              \
    /* Control roles: what a control is, said after its name */                                                        \
    X(RoleButton, "button")                                                                                            \
    X(RoleTab, "tab")                                                                                                  \
    X(RoleCheckBox, "check box")                                                                                       \
    X(RoleSlider, "slider")                                                                                            \
    X(RolePicker, "value picker")                                                                                      \
    X(NothingHere, "nothing to navigate here")                                                                         \
    /* Review buffers */                                                                                               \
    X(BufferDetails, "details")                                                                                        \
    X(BufferEvents, "events")                                                                                          \
    X(BufferEmpty, "%s, empty")                                                                                        \
    X(PositionInGroup, "%d of %d, %s")                                                                                 \
    /* Groups */                                                                                                       \
    X(GroupSummary, "%s, %d items")                                                                                    \
    X(GroupSummaryOne, "%s, 1 item")                                                                                   \
    X(GroupButtons, "buttons")                                                                                         \
    X(GroupText, "text")                                                                                               \
    X(GroupControlPanel, "control panel")                                                                              \
    X(GroupLivePanel, "Sim panel")                                                                                     \
    X(GroupFamily, "family")                                                                                           \
    X(GroupWantsFears, "wants and fears")                                                                              \
    X(GroupControls, "controls")                                                                                       \
    /* Text fields */                                                                                                  \
    X(TextField, "%s %s, text field")                                                                                  \
    X(Blank, "blank")                                                                                                  \
    X(Space, "space")                                                                                                  \
    /* Values */                                                                                                       \
    X(ValueOfMax, "%d of %d")                                                                                          \
    X(LabelValue, "%s: %s")                                                                                            \
    X(NextPage, "next page")                                                                                           \
    X(PreviousPage, "previous page")                                                                                   \
    /* Interaction menu */                                                                                             \
    X(PieMenu, "interactions")                                                                                         \
    X(NoInteractions, "no interactions")                                                                               \
    X(Cancelled, "cancelled")                                                                                          \
    /* Cheat console */                                                                                                \
    X(CheatConsole, "cheat console")                                                                                   \
    X(CheatRan, "cheat: %s")                                                                                           \
    X(CheatEmpty, "type or pick a cheat first")                                                                        \
    /* Neighborhood: lots and families */                                                                              \
    X(GroupLots, "lots")                                                                                               \
    X(LotFamily, "%s family")                                                                                          \
    X(LotForSale, "for sale")                                                                                          \
    X(LotCommunity, "community lot")                                                                                   \
    X(LotApartmentsEmpty, "apartment building, no tenants")                                                            \
    X(LotApartmentsWith, "apartment building: %s")                                                                     \
    X(FamilySummary, "%1$s: %2$d members, funds %3$s")                                                                 \
    /* Money */                                                                                                        \
    X(Simoleons, "simoleons")                                                                                          \
    X(FundsNow, "funds %s, %s")                                                                                        \
    /* Lot: object browser (PageUp/PageDown) */                                                                        \
    X(CategorySims, "Sims")                                                                                            \
    X(CategoryOther, "other objects")                                                                                  \
    X(NoObjects, "no objects here")                                                                                    \
    X(FloorNumber, "floor %d")                                                                                         \
    X(NotOnLot, "not on a lot")                                                                                        \
    X(HereNow, "right here")                                                                                           \
    X(TilesAway, "%d tiles %s")                                                                                        \
    X(OneTileAway, "1 tile %s")                                                                                        \
    X(DirNorth, "north")                                                                                               \
    X(DirNorthEast, "north-east")                                                                                      \
    X(DirEast, "east")                                                                                                 \
    X(DirSouthEast, "south-east")                                                                                      \
    X(DirSouth, "south")                                                                                               \
    X(DirSouthWest, "south-west")                                                                                      \
    X(DirWest, "west")                                                                                                 \
    X(DirNorthWest, "north-west")                                                                                      \
    X(OtherFloor, "on another floor")                                                                                  \
    X(SelectedSimSuffix, "selected")                                                                                   \
    X(NoInteractionsAtAll, "no interactions: nothing can be done with this object")                                    \
    X(WalkingThere, "walking there")                                                                                   \
    X(CannotWalk, "cannot walk there")                                                                                 \
    X(NoSimSelected, "no Sim selected")                                                                                \
    X(FundsChanged, "%1$s, %2$s left")
