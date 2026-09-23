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
    X(KeysLot, "Lot: PageDown and PageUp the lot's objects, nearest first; Shift with them the categories, Control with them the rooms. Enter opens an object's interactions, or picks it up in buy mode; Control+Enter walks the selected Sim there. Control+E the Sim's state, Control+1 to 9 choose a Sim, Tab the panels, Control+Up and Down the details.")\
    X(KeysPlacement, "Placing: PageDown and PageUp the spots where it fits, Control with them the rooms, arrows one tile, Home back to the Sim, comma and period turn it, Enter puts it down, Delete sells it, Escape stops.")\
    X(KeysBuild, "Building: arrows move the corner, Shift five, PageUp and PageDown the level, Space marks the start, Enter builds from the mark, Control+Enter removes, Home back to the Sim, R repeats, Escape leaves.")\
    X(KeysNeighborhood, "Neighborhood: the lots list comes first, Enter opens a lot's panel. To move a family in, choose it in the family bin, then press Enter on a lot. Tab the panels.")\
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
    X(GroupNeeds, "needs")                                                                                             \
    X(GroupControls, "controls")                                                                                       \
    X(GroupBuyPanel, "buy mode")                                                                                       \
    X(GroupBuildPanel, "build mode")                                                                                   \
    X(GroupCatalog, "catalog")                                                                                         \
    X(GroupStarSign, "star sign")                                                                                      \
    X(GroupAspiration, "aspiration")                                                                                   \
    X(GroupSteps, "steps")                                                                                             \
    X(GroupAge, "age")                                                                                                 \
    X(GroupSex, "sex")                                                                                                 \
    X(GroupTraits, "traits")                                                                                           \
    /* Text fields */                                                                                                  \
    X(TextField, "%s %s, text field")                                                                                  \
    X(Blank, "blank")                                                                                                  \
    X(Space, "space")                                                                                                  \
    /* Values */                                                                                                       \
    X(ValueOfMax, "%d of %d")                                                                                          \
    X(SliderRange, "%1$s to %2$s")                                                                                     \
    X(RadioBetween, "between %1$s and %2$s")                                                                           \
    X(PointsLeft, "%d points left")                                                                                    \
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
    /* Needs */                                                                                                        \
    X(MotiveValue, "%s, %d percent")                                                                                   \
    /* Lot: status (Ctrl+E) and action queue */                                                                        \
    X(GroupActionQueue, "action queue")                                                                                \
    X(StatusIdle, "doing nothing")                                                                                     \
    X(StatusNeedsLow, "low needs: %s")                                                                                 \
    X(StatusNeedsFine, "needs fine")                                                                                   \
    /* Narrator: the family watch */                                                                                   \
    X(ActionStarted, "%1$s: %2$s")                                                                                     \
    X(ActionFinished, "%1$s finished %2$s")                                                                            \
    X(NeedLow, "%1$s, low need: %2$s")                                                                                 \
    X(NeedUrgent, "%1$s, urgent need: %2$s")                                                                           \
    /* Lot: selecting Sims */                                                                                          \
    X(SimSelectedNow, "%s, selected")                                                                                  \
    X(NoSuchSim, "no Sim %d")                                                                                          \
    /* Narrator: visitors */                                                                                           \
    X(VisitorArrived, "%s arrived")                                                                                    \
    X(VisitorLeft, "%s left")                                                                                          \
    /* Lot: rooms */                                                                                                   \
    X(RoomObjects, "%s, %d objects")                                                                                   \
    X(RoomObjectsOne, "%s, 1 object")                                                                                  \
    X(FundsChanged, "%1$s, %2$s left")                                                                                 \
    /* Camera */                                                                                                       \
    X(CameraFree, "camera free")                                                                                       \
    X(CameraFollowing, "following %s")                                                                                 \
    /* Narrator */                                                                                                     \
    X(NarratorAll, "narrator: everything")                                                                             \
    X(NarratorWarnings, "narrator: warnings only")                                                                     \
    X(NarratorOff, "narrator off")                                                                                     \
    /* Relationships */                                                                                                \
    X(RelationshipValues, "%1$s, daily %2$s, lifetime %3$s")                                                           \
    /* Room layout (Ctrl+L) */                                                                                         \
    X(LayoutNoRoom, "no room here")                                                                                    \
    X(LayoutSummary, "%1$s, floor %2$d, %3$d tiles, from %4$d, %5$d to %6$d, %7$d")                                    \
    X(LayoutOpenings, "%1$d doors, %2$d windows")                                                                      \
    X(LayoutDoor, "door %1$s, %2$s wall, leads to %3$s, tile %4$d, %5$d")                                              \
    X(LayoutWindow, "window %1$s, %2$s wall, looks onto %3$s, tile %4$d, %5$d")                                        \
    /* Buy mode */                                                                                                     \
    X(PlaceBegin, "%s: PageDown finds spots where it fits, arrows move it, Enter puts it down, Escape cancels")        \
    X(PlaceTile, "tile %1$d, %2$d")                                                                                    \
    X(PlaceOffLot, "off the lot")                                                                                      \
    X(PlaceFits, "fits")                                                                                               \
    X(PlaceBlocked, "does not fit here")                                                                               \
    X(PlacePlaced, "%s placed")                                                                                        \
    X(PlaceFacing, "facing %s")                                                                                        \
    X(PlaceNoSpot, "it fits nowhere on this lot, turned any way")                                                      \
    X(PlaceRoomSpots, "%1$s, %2$d spots")                                                                              \
    X(PlaceWallSide, "%s wall")                                                                                        \
    X(PlaceLeadsTo, "leads to %s")                                                                                     \
    X(PlaceLeadsToAnother, "leads to another %s")                                                                      \
    X(PlaceMoving, "moving %s: PageDown finds spots, Enter puts it down, Delete sells it, Escape puts it back")        \
    X(PlaceCannotPick, "%s cannot be picked up")                                                                       \
    X(PlaceSold, "%s sold")                                                                                            \
    /* Build mode */                                                                                                   \
    X(BuildBegin, "%s: arrows move the corner, Space marks the start, Enter builds, Control+Enter removes, Escape leaves")\
    X(BuildCorner, "corner %1$d, %2$d")                                                                                \
    X(BuildMarked, "start marked at %1$d, %2$d")                                                                       \
    X(BuildNoMark, "mark the start with Space first")                                                                  \
    X(BuildDone, "built for %1$s, %2$s left")                                                                                          \
    X(BuildRemoved, "removed, %1$s back, %2$s")                                                                                     \
    X(BuildNothing, "nothing changed")                                                                                 \
    X(BalloonThinks, "%1$s thinks: %2$s")                                                                              \
    X(BalloonDreams, "%1$s dreams: %2$s")                                                                              \
    X(BalloonSays, "%1$s says: %2$s")                                                                                  \
    X(BalloonScreams, "%1$s screams: %2$s")                                                                            \
    X(BalloonScreamsNothing, "%s screams")                                                                             \
    X(BalloonCrossed, "no %s")                                                                                         \
    X(BalloonRelationUp, "%s: relationship improves")                                                                  \
    X(BalloonRelationUpMuch, "%s: relationship improves a lot")                                                        \
    X(BalloonRelationDown, "%s: relationship worsens")                                                                 \
    X(BalloonRelationDownMuch, "%s: relationship worsens a lot")                                                       \
    X(BalloonRomance, "%s: romance")                                                                                   \
    X(BalloonRomanceBroken, "%s: romance broken")                                                                      \
    X(BalloonCrush, "%s: crush")                                                                                       \
    X(BalloonCrushBroken, "%s: crush over")                                                                            \
    X(BalloonMomentBad, "%s: a bad moment")                                                                            \
    X(BalloonMomentGood, "%s: a great moment")                                                                         \
    X(BalloonFirstKiss, "%s: first kiss")                                                                              \
    X(BalloonFirstLove, "%s: first love")                                                                              \
    X(BalloonInLove, "%s: in love")                                                                                    \
    X(BalloonLoveLost, "%s: love lost")                                                                                \
    X(BalloonFriend, "%s: new friend")                                                                                 \
    X(BalloonFriendLost, "%s: friend lost")                                                                            \
    X(BalloonBestFriend, "%s: new best friend")                                                                        \
    X(BalloonBestFriendLost, "%s: best friend lost")                                                                   \
    X(BalloonEnemy, "%s: new enemy")                                                                                   \
    X(BalloonEnemyLost, "%s: no longer enemies")                                                                       \
    X(BalloonMemoryGood, "%s: good memory")                                                                            \
    X(BalloonMemoryBad, "%s: bad memory")                                                                              \
    X(BalloonTopicLiked, "%s: likes the topic")                                                                        \
    X(BalloonTopicDisliked, "%s: dislikes the topic")                                                                  \
    X(BalloonPersonalityGood, "%s: personalities click")                                                               \
    X(BalloonPersonalityBad, "%s: personalities clash")                                                                \
    X(BalloonRouteFailed, "%s cannot get there")                                                                       \
    X(IconAmbulance, "ambulance")                                                                                      \
    X(IconPhone, "phone")                                                                                              \
    X(IconPoolLadder, "pool ladder")                                                                                   \
    X(IconQuestion, "question mark")                                                                                   \
    X(IconFingerprint, "fingerprint")                                                                                  \
    X(IconBaby, "baby")                                                                                                \
    X(IconDoor, "door")                                                                                                \
    X(IconBabyQuestion, "a baby?")                                                                                     \
    X(FamilyTreePicked, "%s picked up: Enter on another Sim to join them, Enter again to put down")                    \
    X(FurnitureGone, "%1$s has left the lot, worth %2$s")                                                              \
    X(BuildShutIn, "careful: %2$s has no door and %1$s is inside; Ctrl+Z undoes the wall")                             \
    X(KeysLotMore, "Control+L the room's layout, Control+N what the narrator says, Control+F whether the camera follows the Sim.")\
    X(GroupBusiness, "business")                                                                                       \
    X(GroupSeasons, "seasons")                                                                                         \
    X(GroupDate, "date")                                                                                               \
