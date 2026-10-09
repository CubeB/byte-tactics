# Placeholders left

Phase 5 of `docs/cleanup-roadmap.md` ends when every `FUN_` name that can earn
a name has one, and the rest are listed here. These are the `FUN_` names that
were still left after their modules' rename pull requests; issue #6220 named
the ones it could and recorded the rest on this page. The Globals section
records the data names left the same way.

Only placeholders spelled in `src/` appear here, and only those that stay.
A placeholder that an open `cleanup` issue lists by name is not listed until
that issue closes. The generated headers `include/ta_types.h` and
`include/ta_protos.h` also spell placeholders that no source file uses; they
go with those headers (#6407, #6427). Placeholders spelled only in comments
are #6526's.

## Gap entry labels

`tools/rename.py` refuses these by design: a `// ENTRY: 0x...` label's symbol
is made from the address, so there is no identifier to rename. They stay
`FUN_<address>` until a later change gives entry labels their names in the
annotation itself (`docs/linking.md`), as the blit issue did for its 19 other
entry points. The label is a symbol (`_FUN_<address>` in `data/layout.csv`),
so most of these are not spelled `FUN_` in the sources; callers that declare
one spell it as an ordinary function (`FUN_004b70ef` and `FUN_004b7123` in
over twenty files).

| label | file | Thaldren's name |
| --- | --- | --- |
| `FUN_004b70c0` | `src/util/math_4b70a0.cpp` | `Trig_LookupCosHalf` |
| `FUN_004b70e0` | `src/util/math_4b70a0.cpp` | `Math_MulHigh32` |
| `FUN_004b70ef` | `src/util/math_4b70a0.cpp` | `Math_MulSinFixed` |
| `FUN_004b7123` | `src/util/math_4b70a0.cpp` | `Math_MulCosFixed` |
| `FUN_004b715a` | `src/util/math_4b70a0.cpp` | `Math_Atan2Scaled` |
| `FUN_004b7173` | `src/util/math_4b70a0.cpp` | `Math_Rotate2DPoint` |
| `FUN_004b71a7` | `src/util/math_4b70a0.cpp` | `Trig_BuildRotationMatrix` |
| `FUN_004b72e2` | `src/util/math_4b70a0.cpp` | `Math_MulVec3ByMatrix3x3` |
| `FUN_004b7381` | `src/util/math_4b70a0.cpp` | `Math_MulDiv` |
| `FUN_004cca33` | `src/graphics/blit.cpp` | `Gfx_XorLineBresenham` |
| `FUN_004ccb65` | `src/graphics/blit.cpp` | `Rect_ClipLineToWidthHeight` |
| `FUN_004ccd1c` | `src/graphics/blit.cpp` | `Gfx_DrawRectOutlineRaw` |
| `FUN_004ccd85` | `src/graphics/blit.cpp` | `Gfx_XorRectOutlineBresenham` |

## Functions and methods

| placeholder | file | Thaldren's name | why it stays |
| --- | --- | --- | --- |
| `FUN_0041d8a0` | `src/game/cd_check.cpp`, `src/units/unit_types_42a8d0.cpp`, `src/weapons/weapon_types.cpp` | `CdCheck_IsEnabled` | no evidence for a correct name: the flag is set in the exe (1, never written) and makes `FindGameCdDrive` use the current directory instead of scanning the CD, the opposite of the name's polarity (#6037) |
| `FUN_004161f0` | `src/network/net_stats.cpp`, `src/game/console_commands.cpp`, `src/map/line_of_sight.cpp` | `Code_PaddingLoopUnreachable` | no evidence: Thaldren marks it padding, the loop's sums are dead, and its only callee is a no-op (#6058) |
| `FUN_00406f40` | `src/ai/ai_player.cpp`, `src/map/line_of_sight.cpp` | none | no evidence: a one-byte `ret` with no callers (the forward declaration in `line_of_sight.cpp` only keeps a symbol id) (#6504) |
| `FUN_0042f900` | `src/sound/sound_types.cpp` | none | no evidence: an empty 3-byte stub with no callers (#6067) |
| `Class_00432c00::FUN_00432c00` | `src/map/los_tables.cpp` | `RefCounted_ScalarDtor` | waits on #6019: a compiler-generated scalar deleting destructor whose decorated name (`??_GElem_00432be0@@QAEPAXI@Z`) is in `data/aliases.csv`; the decorated name can be emitted only once the class views are one class (#6150) |
| `Class_00432c20::FUN_00432c20` | `src/map/los_tables.cpp` | `RefCounted_ScalarDtorB` | waits on #6019: the same, `??_GEntry_00432cf0@@QAEPAXI@Z` (#6150) |
| `FUN_0046c8c0` | `src/network/unit_sync.cpp` | none | no evidence: an empty stub with no callers (#6170) |
| `FUN_0046c8d0` | `src/network/unit_sync.cpp` | none | no evidence: an empty stub with no callers (#6170) |
| `FUN_0047caf0` | `src/map/terrain.cpp` | none | no evidence: an empty body with no callers (#6163) |
| `FUN_00490200` | `src/game/victory_48dfb0.cpp`, `src/ai/ai_player.cpp`, `src/game/players_464290.cpp`, `src/map/los_tables.cpp` | `VictoryCond_IsLocalPlayerEliminated` | a dead alias: byte-identical to `IsLocalPlayerEliminated` at 0x490050, which already carries that name, and Thaldren marks this address a DEAD alias of it (#6195) |
| `FUN_00407e70` | `src/data/vtables.cpp` | `Ai_DtorSpatialBehaviorSlot` | already named: `SpatialTimer`'s compiler-generated scalar deleting destructor, `??_GSpatialTimer@@UAEPAXI@Z` in `data/symbols.csv`; the spelling that stays is the vtable's stand-in declaration, and a decorated deleting destructor cannot be written in C++ (#6313) |
| `FUN_0044f590` | `src/data/vtables.cpp` | `PathGoal_Patrol_DtorDeleting` | already named: `PatrolGoal`'s compiler-generated scalar deleting destructor, `??_GPatrolGoal@@UAEPAXI@Z` in `data/symbols.csv`; the spelling that stays is the vtable's stand-in declaration (#6313) |
| `Class_00475840` | `src/graphics/particles.cpp`, `src/graphics/particles_472630.cpp`, `src/graphics/particles_473d50.cpp` | `Fx_ParticleVec_CountNanoBeam` | a view of `std::vector<NanoParticle>` to call its out-of-line `size()` (0x475840); a join through a `std::` template is not trusted (#5750) |
| `Class_00475bd0` | `src/graphics/particles.cpp` | `Fx_ParticleVec_PushExtra` | the caller spelling of `std::vector<ThrustParticle>::insert` (0x475bd0), `Class_00475bd0::insert` in `data/aliases.csv`; the function is already named, and a join through a `std::` template is not trusted (#6313) |
| `Class_00476210` | `src/graphics/particles.cpp`, `src/graphics/particles_472630.cpp`, `src/graphics/particles_475470.cpp` | `Fx_ParticleSlotVec_InsertN` | the caller spelling of `std::vector<SmokeParticle>::insert` (0x476210), `Class_00476210::insert` in `data/aliases.csv`; the function is already named, and a join through a `std::` template is not trusted (#6313) |
| `FUN_0049fd20` | `src/gui/gui.cpp` | `Gui_CopyGadgetRectMinusOrigin` | no evidence: copies the 24 unnamed bytes at +0x3c of one object and subtracts two unnamed shorts of another; dead, and no view names that block (#6313) |
| `FUN_004a14c0` | `src/gui/gui.cpp` | `Gui_SetGadgetStatusByName` | a dead duplicate: byte-identical to `SetGadgetGrayedOutByName` at 0x4a1450, which sets the entry's grayed-out flag, not its status (#6313) |
| `FUN_004a1950` | `src/gui/gui.cpp` | `Gui_IsIndexBelowCount` | no evidence: `value < obj->field_4` for an anonymous 8-byte object; dead, so whether the field is a count is unknown (#6313) |
| `FUN_004a1970` | `src/gui/gui.cpp` | `Gui_IsCountLessThan` | no evidence: the same comparison as 0x4a1950 on the anonymous field at +0xc (#6313) |
| `FUN_004a3eb0` | `src/gui/gui.cpp` | `Gui_ClearGadgetAnimState` | no evidence: zeroes the three unnamed fields +0x140, +0x144 and +0x14a of entry i; in the slider view (0x4a4170) the same offsets are the scroll offset and its change callback, so no name is supported (#6313) |
| `Class_004c5ba0` | `src/util/tdf_4c54f0.cpp`, `src/util/tdf_4c59d0.cpp` | `Translate_VectorGetPairCount` | a derived-vector stand-in to call `std::vector<TdfField>`'s protected members out of line (its `Insert` is 0x4c59d0, its size 0x4c5ba0); a join through a `std::` template is not trusted (#5750) |
| `Class_004c90b0` | `src/graphics/surface.cpp`, `src/util/tdf_4c2ea0.cpp` | `RefStr_AppendCString` | the reference-counted string handle's append (0x4c90b0); the handle's name (`StringRef`) was reverted in #6371 and the remaining handle views wait for #6384 |
| `BlockHistory::FUN_004d8d40` | `src/debug/debug_lib.cpp` | none | no evidence: formats the block history into a 2000-byte local buffer through `FormatBlockHistory` and discards it; dead, and nothing says what the stripped output call was (#6504) |
| `FUN_004e16b0` | `src/debug/debug_lib_4e16b0.cpp` | `cpuid_Version_info` | no evidence: sets a flag from CPUID EDX bit 23 (SEP) after `IsPentiumOrBetter`; nothing reads the flag, so what it would name is unknown (#6313) |
| `Class_004e18c0::Clear` | `src/debug/debug_lib.cpp` | `PerfDlg_StrMap_ClearAllNodes` | joining it into `NameTable` or `NameMapTree` needs its own `erase` (the `Iter_004e18c0` variant, which keeps `Erase` and `EraseSubtree` out of line through declaration-only overloads) and a raw tree view of the embedded map in `NameTable`; the bytes match only as a separate view (#6376) |
| `Class_004e2240::Begin` | `src/debug/debug_lib.cpp` | `PerfDlg_StrMap_CopyBeginNode` | the out-of-line `begin()` that `Clear` returns through; it shares `Clear`'s `Node_004e18c0` view, so it stays with it (#6376) |
| `Class_0044cf00::ContainsUnit` | `src/orders/order_targets.cpp`, `src/data/vtables.cpp` | `OrderFx::OrderFx_Shared_ContainsUnitViaCell` | a view of `OrderFx` (the base default of slot 4, also `PointMarker`'s) that declares six virtual slots so its body can call slot 5, `ContainsCell`; `OrderFx` stores its vtable by hand in a plain `vtable` member and cannot declare virtual functions without a second vptr, so the view cannot join it (#6374) |
| `Class_0044e5b0::IsComplete` | `src/orders/order_targets.cpp`, `src/data/vtables.cpp` | `PathOrder::PathOrder_IsComplete` | a view of `PathOrder` that declares nine virtual slots so its body can call slot 8, `FillWorldPos`, on itself; `PathOrder` derives from the hand-vtable `OrderFx`, so it cannot declare them and the view stays (#6374) |

## Classes

| placeholder | file | Thaldren's name | why it stays |
| --- | --- | --- | --- |
| `Class_00451fd0` | `src/network/net_game.cpp` | `Net_InitPacketTypeTable` | no evidence for a name: the view of `g_game` at +0x12ef that `InitPacketTables` fills (the tick at +0x870, the receive buffer size at +0x1745 and its pointer at +0x1749 are `g_game` +0x1b5f, +0x2a34 and +0x2a38); Thaldren names only the function and the members belong to its `GameState`, so there is no type to join (#6399) |
| `Class_0046e5c0` | `src/network/unit_sync.cpp`, `src/network/unit_sync_46dad0.cpp` | `Sync_InitTaggedVector` | the constructor body of a `std::vector` (the allocator byte copied, then `_First`, `_Last` and `_End` zeroed); a join through a `std::` template is not trusted, and Thaldren gives the function only (#6399) |
| `Class_0046e5e0` | `src/network/unit_sync.cpp`, `src/network/unit_sync_46dad0.cpp` | `Stl_VectorClearFree_SyncTempTagged` | the out-of-line destructor of a `std::vector` of 4-byte elements, modelled as a class that holds the vector; no exe name gives it a type (#6399) |
| `Class_0046e610` | `src/network/unit_sync.cpp`, `src/network/unit_sync_46dad0.cpp`, `src/network/unit_sync_player.cpp` | `Stl_VectorClearFree_SyncChecksumVec` | the out-of-line destructor of a `std::vector` of 14-byte elements, the type of the `list_c` and `list_d` members of `PacketSequencer` (and of the member at +0x38 of `UnitSync` in the other files); a stand-in for the vector, which no exe name gives a type (#6399) |
| `Class_0046eaa0` | `src/network/unit_sync.cpp`, `src/network/unit_sync_46dad0.cpp`, `src/network/unit_sync_46f7a0.cpp`, `src/network/unit_sync_470040.cpp` | `SyncPlayerRecord` | the 0x5c-byte player record in the vector of `UnitSync`, the type `UnitSyncPlayer` names in `unit_sync_player.cpp`; the two cannot be joined: `unit_sync.cpp` needs this view's implicit member destruction (the `_Destroy` at 0x46eaa0) and `UnitSyncPlayer`'s out-of-line destructor (0x46c920) in one file, and `unit_sync_46f7a0.cpp` derives `UnitSyncPlayer` from this view (#6399) |
| `Class_0046eba0` | `src/network/unit_sync.cpp` | `Sync_InsertStruct0EAt` | a call-site view of the `std::vector<UnitSyncPacket>` queues of `PacketSequencer` that calls the vector's out-of-line `insert` (0x46eba0, already named in `data/symbols.csv`); a join through a `std::` template is not trusted (#6399) |
| `Class_00470250` | `src/network/unit_sync.cpp`, `src/network/unit_sync_470040.cpp` | `Stl_VectorCapacity` | the out-of-line `capacity()` of a `std::vector` of 4-byte elements (0x470250), seen through a cut-down vector; a join through a `std::` template is not trusted (#6399) |
| `Class_00470270` | `src/network/unit_sync.cpp`, `src/network/unit_sync_470040.cpp` | `Stl_VectorSize` | the same for `size()` (0x470270) (#6399) |
| `Class_00452370` | `src/game/game_state_490ac0.cpp` | `Net_ShutdownMultiplaySession` | no evidence for a name: one call-site view of the packet-data area of `g_game` at +0x12ef, the same type as `Class_00451fd0` in `net_game.cpp`, which no exe name marks; Thaldren names only the function at 0x452370 (#6372) |
| `Class_00435920` | `src/frontend/multi_449bb0.cpp` | `MapInfo_MapMemoryReqToTier` | waits on #6407: a call-site view of `Mission` for `GetTerrainSizeTier` (0x435920, `Mission::GetTerrainSizeTier` in `data/symbols.csv`) on `g_game->map`; `tools/rename.py` refuses to join it while the file includes `include/ta_types.h`, which spells `Mission` and its own views of it (#6403) |
| `Class_00435a20` | `src/frontend/multi_449bb0.cpp` | `MapInfo_SelectByName` | waits on #6407: the same view of `Mission` for `LoadMissionByName` (0x435a20) |
| `Class_00435c30` | `src/frontend/multi_449bb0.cpp` | `MapInfo_GetMissionName` | waits on #6407: the same view of `Mission` for `GetMissionName` (0x435c30) |
| `Class_00435c40` | `src/frontend/multi_449bb0.cpp` | `MapInfo_HasSelectedMap` | waits on #6407: the same view of `Mission` for `HasMissionName` (0x435c40) |
| `Class_00435d30` | `src/frontend/multi_449bb0.cpp` | `MapInfo_RefreshOtaListAndSelect` | waits on #6407: the same view of `Mission` for `RefreshMapList` (0x435d30) |
| `Class_004373a0` | `src/frontend/multi_449bb0.cpp` | `MapInfo_ComputeTntContentChecksum` | waits on #6407: the same view of `Mission` for `ComputeMapChecksum` (0x4373a0) |
| `Class_004b4bf0` | `src/map/features_424c00.cpp` | `SaveStore_GetBlobSize` | waits on #6407: a call-site view of `HapiBank` for `GetBoxSize` (0x4b4bf0, `HapiBank::GetBoxSize` in `data/symbols.csv`); `tools/rename.py` refuses to join it while the file includes `include/ta_types.h`, which spells `HapiBank` and its own views of it (#6403) |
| `Class_004b4c10` | `src/map/features_424c00.cpp` | `SaveStore_SetBlobCursor` | waits on #6407: the same view of `HapiBank` for `SeekBox` (0x4b4c10) |
| `Class_0045ba20` | `src/gui/gui.cpp` | `Gui_SliderGetValue` | not a type of its own: an unused forward declaration kept for the symbol id it counts (the "Unused here" block in `gui.cpp`); in `include/ta_protos.h` it is the first parameter of `ReadSliderValue` (0x45ba20), which Thaldren types `GuiRoot*`, but `gui.cpp` already spells `Gui`, so `tools/rename.py` refuses to make the two one name |
| `Class_0049fa90` | `src/gui/gui.cpp` | `Gui_Show` | the same: the first parameter of `MarkChanged` (0x49fa90) |
| `Class_004a04f0` | `src/gui/gui.cpp` | `Gui_GetGadgetStatusByteByName` | the same: the first parameter of `GetGadgetActiveByName` (0x4a04f0) |
| `Class_004a1030` | `src/gui/gui.cpp` | `Gui_SetGadgetToggleStateIfActive` | the same: the first parameter of `SetButtonStage` (0x4a1030) |
| `Class_004a1200` | `src/gui/gui.cpp` | `Gui_SetGadgetDisabledByIndex` | the same: the first parameter of `SetGrayedOut` (0x4a1200) |
| `Class_004a1530` | `src/gui/gui.cpp` | `Gui_SetGadgetStatusByteByName` | the same: the first parameter of `SetQuickKeyByName` (0x4a1530) |
| `Class_004a1b40` | `src/gui/gui.cpp` | `Gui_DrawListboxGadget` | the same: the first parameter of `DrawListBox` (0x4a1b40) |
| `Class_004a32a0` | `src/gui/gui.cpp` | `Gui_ConfigureListboxByName` | the same: the first parameter of `ConfigureListBoxByName` (0x4a32a0) |
| `Class_004a4620` | `src/gui/gui.cpp` | `Gui_ScheduleGadgetAnimEndTick` | the same: the first parameter of `ScheduleGadgetEndTick` (0x4a4620) |

## Globals

| placeholder | file | Thaldren's name | why it stays |
| --- | --- | --- | --- |
| `DAT_00000000` | `src/network/unit_sync_46dad0.cpp` | none | not a global: `ProcessSync` (0x46dad0) reads the dword at address 0 (`mov ecx, ds:0x0` at 0x46ddb5 and `mov eax, ds:0x0` at 0x46de59) where its two sends take a player id; `docs/linking.md` lists the null reference among those no symbol names, and the checker accepts it as `DAT_00000000` at 0x0 |
| `DAT_004fc6e8` | `src/orders/unit_orders.cpp` | `MissionOrder_Standby` | not a global of its own: an unused declaration kept for the symbol ids it counts; the table at 0x4fc6e8 is already named `g_groundOrders` |
| `DAT_004fc980` | `src/ai/ai_player_407d40.cpp` | `AiBehaviorSlot_vftable` | already named by the compiler that emits it: `??_7SquadTimer@@6B@`; renaming the hand spelling makes `tools/check.py` say 0x4fc980 is already named that and no source defines the new name (#6288) |
| `DAT_0050289c` | `src/game/cd_check.cpp` | `g_dwCdCheck_Enabled` | no evidence for a correct name: the flag is 1 in the shipped exe and makes `FindGameCdDrive` return the current directory (and `RegisterDataArchives` load the local archives), the opposite of the name's polarity (#6037) |
| `DAT_0051234c` | `src/orders/order_list.cpp` | `g_MissionOrderTableCapacityEnd` | not a global of its own: an unused declaration kept for the symbol ids it counts, the `_End` at +0xc of the vector at 0x512340 |
| `DAT_0051e598` | `src/network/unit_sync.cpp`, `src/network/unit_sync_46d2e0.cpp` | `g_pSyncMapC_NilNode` | already named by the compiler that emits it: `IUUnitSyncEntry::IU?$pair::?$_Tree::_Nil`; renaming the hand spelling makes `tools/check.py` demand that decorated name |
| `DAT_0051e59c` | `src/network/unit_sync.cpp` | `g_nSyncMapC_NilNodeRefs` | the same for the map's `_Nilrefs`: `IUUnitSyncEntry::IU?$pair::?$_Tree::_Nilrefs` |
| `DAT_00512c8c` | `src/network/net_game.cpp` | none | a view of the online configuration block (`DAT_00512c80`) at +0xc: read only in `JoinLobbyGame`, clamped to 10 and passed as the lobby's max player count to `HAPINET_createorjoinlobbygame`, while the block itself has no name yet (Thaldren names the fields at +0, +4, +0x14 and +0x28 but not this one) |
