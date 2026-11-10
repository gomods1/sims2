// Every string the mod speaks, in English: the source the translations in lang/ follow.
// X(Id, "English"), grouped by feature. A translation keeps the printf placeholders (it may
// reorder them as %1$s, %2$d); tools/mod_strings.py checks that and lists missing translations.
#pragma once

#define SA_STRINGS(X)                                                                                                  \
    /* Startup */                                                                                                      \
    X(ModLoaded, "Sims 2 accessibility loaded")                                                                        \
    X(UnsupportedBuild, "Sims 2 accessibility: unsupported game version, mod disabled")                                \
    /* Key help (Ctrl+F1) */                                                                                           \
    X(KeysScreen, "Tab and Shift+Tab go from group to group; Up and Down go through a group, Left and Right too, or change a slider; Home and End go to the ends; Enter presses, and on a kind or a room opens its things; Backspace goes back; R repeats. Control+Up and Control+Down read the details of what was said, Control+Left and Control+Right switch between details and events.") \
    X(KeysLot, "Lot: Tab goes through the needs, the action queue, things by kind, things by room and the game's panels. Page Down and Page Up go through the lot's things, nearest first; Shift with them changes the kind of thing, Control with them the room. Enter on a thing opens its interactions, or picks it up in buy mode; Control+Enter walks the selected Sim there. Control+E the Sim's state, Control+1 to 9 choose a Sim, Control+Up and Down the details.") \
    X(KeysPlacement, "Placing: arrows one tile, Shift five, Page Down and Page Up the spots where it fits, Control with them the rooms, Shift with them or comma and period turn it, Home back to the Sim, Enter puts it down, Delete sells it, Escape stops.") \
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
    X(GroupSongs, "songs")                                                                                             \
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
    X(VisitorAtDoor, "%s is at the door waiting to be greeted")                                                        \
    X(WantFulfilled, "%1$s fulfilled a want: %2$s, %3$d aspiration points")                                            \
    X(AspirationPointsGained, "%1$s gains %2$d aspiration points")                                                     \
    X(FearRealized, "%1$s: a fear came true: %2$s")                                                                    \
    X(AspirationMeter, "Aspiration: %d percent")                                                                        \
    X(RoleBartender, "bartender")                                                                                      \
    X(RoleBurglar, "burglar")                                                                                          \
    X(RoleDriver, "driver")                                                                                            \
    X(RoleStreaker, "streaker")                                                                                        \
    X(RolePolice, "police officer")                                                                                    \
    X(RoleDelivery, "delivery person")                                                                                 \
    X(RoleExterminator, "exterminator")                                                                                \
    X(RoleFirefighter, "firefighter")                                                                                  \
    X(RoleGardener, "gardener")                                                                                        \
    X(RoleBarista, "barista")                                                                                          \
    X(RoleGrimReaper, "Grim Reaper")                                                                                   \
    X(RoleHandyman, "handyman")                                                                                        \
    X(RoleMatchmaker, "matchmaker")                                                                                    \
    X(RoleMaid, "maid")                                                                                                \
    X(RoleMailCarrier, "mail carrier")                                                                                 \
    X(RoleNanny, "nanny")                                                                                              \
    X(RolePaperDelivery, "newspaper delivery")                                                                         \
    X(RolePizzaDelivery, "pizza delivery")                                                                             \
    X(RoleProfessor, "professor")                                                                                      \
    X(RoleRepoMan, "repo man")                                                                                         \
    X(RoleClerk, "shop clerk")                                                                                         \
    X(RoleSocialWorker, "social worker")                                                                               \
    X(RoleParamedic, "paramedic")                                                                                      \
    X(RoleFoodDelivery, "takeout delivery")                                                                            \
    X(RoleHost, "restaurant host")                                                                                     \
    X(RoleWaiter, "waiter")                                                                                            \
    X(RoleChef, "chef")                                                                                                \
    X(RoleDJ, "DJ")                                                                                                    \
    X(RoleReporter, "reporter")                                                                                        \
    X(RoleStylist, "stylist")                                                                                          \
    X(RoleAnimalControl, "animal control officer")                                                                     \
    X(RoleTrainer, "obedience trainer")                                                                                \
    X(RoleMasseuse, "massage therapist")                                                                               \
    X(RoleBellhop, "bellhop")                                                                                          \
    X(RolePickpocket, "pickpocket")                                                                                    \
    X(RoleTourGuide, "tour guide")                                                                                     \
    X(RoleHousekeeper, "housekeeper")                                                                                  \
    X(RoleFoodStandChef, "food stand chef")                                                                            \
    X(RoleFireDancer, "fire dancer")                                                                                   \
    X(RoleWitchDoctor, "witch doctor")                                                                                 \
    X(RoleFoodJudge, "food judge")                                                                                     \
    X(RoleWitch, "witch")                                                                                              \
    X(RoleBreakdancer, "breakdancer")                                                                                  \
    X(RoleLandlord, "landlord")                                                                                        \
    X(RoleButler, "butler")                                                                                            \
    X(VisitorLeft, "%s left")                                                                                          \
    /* Lot: rooms */                                                                                                   \
    X(RoomObjects, "%s, %d objects")                                                                                   \
    X(RoomObjectsOne, "%s, 1 object")                                                                                  \
    X(ThingsByKind, "things by kind")                                                                                   \
    X(ThingsByRoom, "things by room")                                                                                   \
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
    X(BalloonSays, "%1$s talks about %2$s")                                                                            \
    X(BalloonTalksAgainst, "%1$s complains about %2$s")                                                                \
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
    X(IconBrain, "brains")                                                                                             \
    X(IconNeedOven, "an oven is needed")                                                                               \
    X(IconNeedStove, "a stove is needed")                                                                              \
    X(IconNeedGrill, "a grill is needed")                                                                              \
    X(IconNeedCounter, "a counter is needed")                                                                          \
    X(IconNeedToasterOven, "a toaster oven is needed")                                                                 \
    X(IconNeedMicrowave, "a microwave is needed")                                                                      \
    X(BalloonSomeone, "someone")                                                                                       \
    X(FamilyTreePicked, "%s picked up: Enter on another Sim to join them, Enter again to put down")                    \
    X(FurnitureGone, "%1$s has left the lot, worth %2$s")                                                              \
    X(BuildShutIn, "careful: %2$s has no door and %1$s is inside; Ctrl+Z undoes the wall")                             \
    X(KeysLotMore, "Control+L the room's layout, Control+N what the narrator says, Control+Shift+F whether the camera follows the Sim.")\
    X(GroupBusiness, "business")                                                                                       \
    X(GroupSeasons, "seasons")                                                                                         \
    X(GroupDate, "date")                                                                                               \
    /* Groups by letter, the focus on screen (ported from the Sims 1 mod) */                                        \
    X(NoGroupLetter, "no group starting with %s")                                                                   \
    X(KeysGroupLetter, "Alt and a letter: the next group whose name starts with that letter.")                      \
    X(KeysShowFocus, "Control Shift V: light the focused control on screen for someone helping.")                   \
    X(ShowFocusOn, "focus shown on screen")                                                                         \
    X(ShowFocusOff, "focus no longer shown on screen")                                                              \
    /* Placing: size, what it runs into and faces, beside the lot's things */                                       \
    X(ObjectSize, "takes %1$d tiles: %2$d west to east, %3$d north to south")                                       \
    X(CollidesWith, "runs into %s")                                                                                 \
    X(FacingThing, "toward %s")                                                                                     \
    X(FacingThingAway, "toward %1$s, %2$d tiles away")                                                                 \
    X(RoomChange, "leaving %1$s, entering %2$s")                                                                    \
    X(BesideThing, "beside %s")                                                                                     \
    X(KeysPlacingThings, "Control Shift Page Down and Page Up: put it beside the next thing of the lot, nearest first.")\
    /* Sound cues and the lot map */                                                                                \
    X(CuesOn, "cursor sounds on")                                                                                   \
    X(CuesOff, "cursor sounds off")                                                                                 \
    X(SurfaceNone, "no floor")                                                                                      \
    X(SurfaceGround, "ground")                                                                                      \
    X(SurfaceGrass, "grass")                                                                                        \
    X(SurfaceWood, "wood floor")                                                                                    \
    X(SurfaceTile, "tile floor")                                                                                    \
    X(SurfaceCarpet, "carpet")                                                                                      \
    X(SurfacePaving, "paving")                                                                                      \
    X(SurfaceGravel, "gravel")                                                                                      \
    X(SurfacePoolRim, "pool edge")                                                                                  \
    X(SurfaceWater, "water")                                                                                        \
    X(EdgeFence, "fence to the %s")                                                                                 \
    X(EdgeRailing, "railing to the %s")                                                                             \
    X(MapStairs, "stairs")                                                                                          \
    X(TerrainSloped, "sloped")                                                                                      \
    X(EdgeWall, "wall to the %s")                                                                                   \
    X(EdgeDoor, "door to the %s")                                                                                   \
    X(EdgeWindow, "window to the %s")                                                                               \
    X(MapOn, "Lot map. Control+F1 says the keys.")                                                                  \
    X(MapOff, "map off")                                                                                            \
    X(MapNoFloor, "nothing is built on floor %d")                                                                   \
    X(MapNothingHere, "nothing to use here")                                                                        \
    X(MapLotEdge, "edge of the lot")                                                                                \
    X(KeysMap, "Lot map: arrows move one tile, with Shift five; each tile says what stands there and which way it faces, the room and the floor when they change, and the walls, fences, railings, doors and windows around it, stairs and slopes, with a sound for each. Home goes back to the selected Sim. Page Down and Page Up go through the lot's things as always, and the map goes to each one. Control+Shift+Page Up and Page Down change floor. Enter opens the menu of what stands there; Control+Enter walks the selected Sim there. R says everything, Control+L the room's layout. Control+Shift+S turns the sounds off and on. Escape or Control+M leaves the map.")\
    X(KeysMapHint, "Control+M: the lot map, a cursor that walks the lot tile by tile.")                             \
    X(KeysCuesHint, "Control+Shift+S turns the cursor sounds off and on.")                                          \
    /* The manual (Control+Shift+F1, the mod help group) */                                                         \
    X(ManualHint, "Control+Shift+F1 opens the manual; Control+F1 says the keys of each screen.")                    \
    X(KeysManual, "Control+Shift+F1 opens the mod's full manual in your browser.")                                  \
    X(ManualOpening, "opening the manual in your browser")                                                          \
    X(ManualMissing, "the manual is missing: reinstall the mod")                                                    \
    X(ManualButton, "Sims 2 Accessibility manual")                                                                  \
    X(GroupModHelp, "mod help")                                                                                     \
    /* Find, problems, doors and windows, the camera's view (ported from the Sims 1 mod, 2026-11-02) */                \
    X(CategoryProblems, "problems")                                                                                    \
    X(CategoryPortals, "doors, windows and stairs")                                                                    \
    X(ConditionBroken, "broken")                                                                                       \
    X(ConditionDirty, "needs cleaning")                                                                                \
    X(ObjPuddle, "puddle")                                                                                             \
    X(ObjAshes, "ashes")                                                                                               \
    X(ObjFire, "fire")                                                                                                 \
    X(FindPrompt, "Find a thing of the lot: type part of its name. Up and Down go through what matches, Enter opens it, Escape cancels.")\
    X(FindCount, "%1$s: %2$d found")                                                                                   \
    X(FindCountOne, "%s: 1 found")                                                                                     \
    X(FindNoMatch, "%s: nothing matches")                                                                              \
    X(FindEmpty, "search empty")                                                                                       \
    X(FindTypeFirst, "Type part of a name first.")                                                                     \
    X(FindResults, "found: %s")                                                                                        \
    X(FindCancelled, "search cancelled")                                                                               \
    X(FindNotNow, "Finish placing first: Escape, then Control+F.")                                                     \
    X(KeysFind, "Control+F finds a thing of the lot by typing part of its name.")                                      \
    X(KeysPortalDelete, "Delete on a door or window of the lot's things removes it.")                                  \
    X(ZoomFar, "far view")                                                                                             \
    X(ZoomMedium, "medium view")                                                                                       \
    X(ZoomNear, "close view")                                                                                          \
    X(CameraFacing, "camera facing %s")                                                                                \
    X(CameraStatus, "camera: %1$s, facing %2$s")                                                                       \
    X(PortalRemoved, "%s removed")                                                                                     \
    X(PortalNotRemoved, "%s could not be removed")                                                                     \
    X(PortalNeedsHand, "To remove a door or window: build mode, hand tool.")                                           \
    X(ActionWithSim, "%1$s, with %2$s")                                                                                \
    X(DoorUsable, "Sims can walk through it")                                                                          \
    X(DoorBlocked, "Warning: Sims can't use this door, %s is in the way")                                              \
    X(DoorNoWay, "Warning: Sims can't get to this door from the lot's entrance")                                       \
    /* Mess names (ported from the Sims 1 mod, 2026-11-02) */                                                          \
    X(ObjRoaches, "cockroaches")                                                                                       \
    X(ObjTrashPile, "pile of trash")                                                                                   \
    X(ObjDigPile, "pile of dirt a dog dug up")                                                                         \
    X(ObjTaxi, "taxi")                                                                                                 \
    X(ObjCar, "car")                                                                                                   \
    X(BowlHasFood, "with food")                                                                                        \
    X(BowlEmpty, "empty")                                                                                              \
    X(LotSize, "%1$d by %2$d tiles")                                                                                   \
    /* Ported from the Sims 1 mod's tester feedback (2026-11-06) */                                                    \
    X(HourChanged, "it is %s")                                                                                         \
    X(HoursOn, "each new hour is said")                                                                                \
    X(HoursOff, "hours are no longer said")                                                                            \
    X(SocialEveryone, "conversations: everyone's")                                                                     \
    X(SocialWithFamily, "conversations: anyone with your family")                                                      \
    X(SocialFamilyOnly, "conversations: only your family's")                                                           \
    X(SocialNobody, "conversations: none, kept in the events")                                                         \
    X(ActionInterruptedWaits, "%1$s stopped %2$s for %3$s; it waits in the queue")                                     \
    X(NobodyReaches, "no Sim can reach it")                                                                            \
    X(SimsCanUse, "Sims can reach it")                                                                                 \
    X(MapNoWalls, "no walls")                                                                                          \
    X(MapLayerAll, "everything")                                                                                       \
    X(MapLayerObjects, "objects only")                                                                                 \
    X(MapLayerFloors, "floors only")                                                                                   \
    X(MapLayerWalls, "walls only")                                                                                     \
    X(MapLayerRooms, "rooms only")                                                                                     \
    X(KeysMapMore, "Walls, fences and windows stop the arrows; Alt with the arrows goes through. Comma and period choose what each step says: everything, objects, floors, walls or rooms.")\
    X(KeysPlacingWalls, "Walls, fences and windows stop the arrows; Alt with the arrows goes through. When moving an object, the first arrow starts where it stood.")\
    X(KeysLotNarration, "Control+T says each new hour of the clock, or stops. Control+Shift+N chooses whose conversations are said: anyone with your family, everyone, only your family, or nobody.")\
    X(SkillNeededForPromotion, "the next promotion needs %d")                                                          \
    X(SimLookUnknown, "%s: the mod has no description of this look")                                                   \
    X(LookHairDyed, "dyed hair")                                                                                       \
    X(LookSkinCustom, "custom")                                                                                        \
    X(LookSkinLight, "light")                                                                                          \
    X(LookSkinMedium, "medium")                                                                                        \
    X(LookSkinMediumDark, "medium dark")                                                                               \
    X(LookSkinDark, "dark")                                                                                            \
    X(LookSkinAlien, "alien")                                                                                          \
    X(LookSkinZombie, "zombie")                                                                                        \
    X(LookEyesCustom, "custom")                                                                                        \
    X(LookEyesAlien, "alien")                                                                                          \
    X(LookBodyHeavy, "heavy")                                                                                          \
    X(LookBodyFit, "fit")                                                                                              \
    X(LookOutfitNaked, "none")                                                                                         \
    X(LookOutfitMaternity, "maternity")                                                                                \
    X(KeysLook, "Control+Shift+E says what a Sim looks like: the Sim focused among the lot's things, else the selected one.")\
    X(SeatFor, "to use %s sitting down")                                                                               \
    X(UsedFromSeat, "used from %s")                                                                                    \
    X(KeysShowFocusThings, "With it on, the mouse pointer also goes to the lot's thing in focus.")                     \
    /* Placing: where it fits, its parts; catalog sizes and pages */                                                   \
    X(FitsTurned, "fits when turned, %s")                                                                              \
    X(NearestFit, "nearest fit %1$s, %2$s")                                                                            \
    X(NoFitNearby, "fits nowhere within %d tiles")                                                                     \
    X(CursorCorner, "cursor at its %s corner")                                                                         \
    X(CursorEdge, "cursor on its %s edge")                                                                             \
    X(CursorMiddle, "cursor inside it")                                                                                \
    X(ObjectExtends, "it extends %s")                                                                                  \
    X(ProductSize, "%1$d by %2$d tiles")                                                                               \
    X(NoNextPage, "last page")                                                                                         \
    X(NoPreviousPage, "first page")                                                                                    \
    X(KeysCatalogPages, "In the catalogs of buy and build mode, Up and Down go through the products, Left and Right turn the pages.")\
    X(KeysCatalogBack, "In buy mode, Backspace goes back from a subcategory to the categories.")\
    X(LightLevel, "light %d%%")                                                                                        \
    /* Rewards */                                                                                                      \
    X(CannotAfford, "can't afford")                                                                                    \
    X(RewardPrice, "%s aspiration points")                                                                             \
    X(RewardCareer, "career: %s")                                                                                      \
    X(RewardLocked, "locked")                                                                                          \
    X(LtaOwned, "owned")                                                                                               \
    X(LtaChosen, "chosen, bought on Accept")                                                                           \
    X(LtaAvailable, "not chosen")                                                                                      \
    X(LtaLevel, "level %d")                                                                                            \
    X(KeysButton, "Keys for this screen")                                                                           \
