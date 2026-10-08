# Placeholders left

Phase 5 of `docs/cleanup-roadmap.md` ends when every `FUN_` name that can earn
a name has one, and the rest are listed here. These are the `FUN_` names that
were still left after their modules' rename pull requests; issue #6220 named
the ones it could and recorded the rest on this page. The Globals section
records the data names left the same way.

## Gap entry labels

`tools/rename.py` refuses these by design: a `// ENTRY: 0x...` label's symbol
is made from the address, so there is no identifier to rename. They stay
`FUN_<address>` until a later change gives entry labels their names in the
annotation itself (`docs/linking.md`), as the blit issue did for its 19 other
entry points.

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
| `FUN_004ccd1c` | `src/graphics/blit_4cbbe0.cpp` | `Gfx_DrawRectOutlineRaw` |

## Functions and methods

| placeholder | file | Thaldren's name | why it stays |
| --- | --- | --- | --- |
| `FUN_0041d8a0` | `src/game/cd_check.cpp` | `CdCheck_IsEnabled` | no evidence for a correct name: the flag is set in the exe (1, never written) and makes `FindGameCdDrive` use the current directory instead of scanning the CD, the opposite of the name's polarity (#6037) |
| `FUN_004161f0` | `src/network/net_stats.cpp` | `Code_PaddingLoopUnreachable` | no evidence: Thaldren marks it padding, the loop's sums are dead, and its only callee is a no-op (#6058) |
| `FUN_0042f900` | `src/sound/sound_types.cpp` | none | no evidence: an empty 3-byte stub with no callers (#6067) |
| `Class_00432c00::FUN_00432c00` | `src/map/los_tables.cpp` | `RefCounted_ScalarDtor` | waits on #6019: a compiler-generated scalar deleting destructor whose decorated name (`??_GElem_00432be0@@QAEPAXI@Z`) is in `data/aliases.csv`; the decorated name can be emitted only once the class views are one class (#6150) |
| `Class_00432c20::FUN_00432c20` | `src/map/los_tables.cpp` | `RefCounted_ScalarDtorB` | waits on #6019: the same, `??_GEntry_00432cf0@@QAEPAXI@Z` (#6150) |
| `FUN_0046c8c0` | `src/network/unit_sync.cpp` | none | no evidence: an empty stub with no callers (#6170) |
| `FUN_0046c8d0` | `src/network/unit_sync.cpp` | none | no evidence: an empty stub with no callers (#6170) |
| `FUN_0047caf0` | `src/map/terrain.cpp` | none | no evidence: an empty body with no callers (#6163) |
| `FUN_00490200` | `src/game/victory_48dfb0.cpp` | `VictoryCond_IsLocalPlayerEliminated` | a dead alias: byte-identical to `IsLocalPlayerEliminated` at 0x490050, which already carries that name, and Thaldren marks this address a DEAD alias of it (#6195) |
| `FUN_00407e70` | `src/data/vtables.cpp` | `Ai_DtorSpatialBehaviorSlot` | already named: `SpatialTimer`'s compiler-generated scalar deleting destructor, `??_GSpatialTimer@@UAEPAXI@Z` in `data/symbols.csv`; the spelling that stays is the vtable's stand-in declaration, and a decorated deleting destructor cannot be written in C++ (#6313) |
| `FUN_0044f590` | `src/data/vtables.cpp` | `PathGoal_Patrol_DtorDeleting` | already named: `PatrolGoal`'s compiler-generated scalar deleting destructor, `??_GPatrolGoal@@UAEPAXI@Z` in `data/symbols.csv`; the spelling that stays is the vtable's stand-in declaration (#6313) |
| `FUN_004758c0` | `src/graphics/particles.cpp` | `Fx_ParticleVec_PushTimedDot` | already named: `std::vector<TeleportParticle>::insert`, matched under its decorated name (#5040); the caller view spelling stays because `insert` is already a member name in this file and `rename.py` refuses all five insert views at once (#6313) |
| `FUN_00475bd0` | `src/graphics/particles.cpp` | `Fx_ParticleVec_PushExtra` | already named: `std::vector<Record_00475bd0>::insert` (#5040); refused with the other four insert views in #6313 |
| `FUN_00475ef0` | `src/graphics/particles.cpp` | `Fx_ParticleVec_PushQueued` | already named: `std::vector<Element_00475ef0>::insert` (#5040); refused with the other four insert views in #6313 |
| `FUN_00476210` | `src/graphics/particles.cpp` | `Fx_ParticleSlotVec_InsertN` | already named: `std::vector<Elem_00476210>::insert` (#5040); refused with the other four insert views in #6313 |
| `FUN_00476490` | `src/graphics/particles.cpp` | `Fx_ParticleVec_PushTimedSub` | already named: `std::vector<Elem_00476490>::insert` (#5040); refused with the other four insert views in #6313 |
| `FUN_0049fd20` | `src/gui/gui.cpp` | `Gui_CopyGadgetRectMinusOrigin` | no evidence: copies the 24 unnamed bytes at +0x3c of one object and subtracts two unnamed shorts of another; dead, and no view names that block (#6313) |
| `FUN_004a14c0` | `src/gui/gui.cpp` | `Gui_SetGadgetStatusByName` | a dead duplicate: byte-identical to `SetGadgetGrayedOutByName` at 0x4a1450, which sets the entry's grayed-out flag, not its status (#6313) |
| `FUN_004a1950` | `src/gui/gui.cpp` | `Gui_IsIndexBelowCount` | no evidence: `value < obj->field_4` for an anonymous 8-byte object; dead, so whether the field is a count is unknown (#6313) |
| `FUN_004a1970` | `src/gui/gui.cpp` | `Gui_IsCountLessThan` | no evidence: the same comparison as 0x4a1950 on the anonymous field at +0xc (#6313) |
| `FUN_004a3eb0` | `src/gui/gui.cpp` | `Gui_ClearGadgetAnimState` | no evidence: zeroes the three unnamed fields +0x140, +0x144 and +0x14a of entry i; in the slider view (0x4a4170) the same offsets are the scroll offset and its change callback, so no name is supported (#6313) |
| `FUN_004c3e40` | `src/util/tdf_4c2ea0.cpp` | `Tdf_Parse` | already named: the definition is `TdfRecord::TdfRecord`, the section constructor that parses `[name] { ... }`; the spellings that stay are caller views constructing on `operator new` storage (`node->FUN_004c3e40(...)`), which a constructor call cannot spell (#6313) |
| `FUN_004e16b0` | `src/debug/debug_lib_4e16b0.cpp` | `cpuid_Version_info` | no evidence: sets a flag from CPUID EDX bit 23 (SEP) after `IsPentiumOrBetter`; nothing reads the flag, so what it would name is unknown (#6313) |
| `FUN_004e6110` | `src/orders/order_targets.cpp` | `CRT_PurecallAbort` | already named: the address is the CRT `_purecall` in `data/symbols.csv`; the spelling that stays is the base class's pure slot 8 declaration, and `rename.py` refuses `FillWorldPos` there because the file already uses that name for five class views (#6313) |

## Classes

| placeholder | file | Thaldren's name | why it stays |
| --- | --- | --- | --- |
| `Class_00452370` | `src/game/game_state_490ac0.cpp` | `Net_ShutdownMultiplaySession` | no evidence for a name: one call-site view of the packet-data area of `g_game` at +0x12ef, the same type as `Class_00451fd0` in `net_game.cpp`, which no exe name marks; Thaldren names only the function at 0x452370 (#6372) |

## Globals

| placeholder | file | Thaldren's name | why it stays |
| --- | --- | --- | --- |
| `DAT_004fc6e8` | `src/orders/unit_orders.cpp` | `MissionOrder_Standby` | not a global of its own: an unused declaration kept for the symbol ids it counts; the table at 0x4fc6e8 is already named `g_groundOrders` |
| `DAT_004fc980` | `src/ai/ai_player_407d40.cpp` | `AiBehaviorSlot_vftable` | already named by the compiler that emits it: `??_7SquadTimer@@6B@`; renaming the hand spelling makes `tools/check.py` say 0x4fc980 is already named that and no source defines the new name (#6288) |
| `DAT_0050289c` | `src/game/cd_check.cpp` | `g_dwCdCheck_Enabled` | no evidence for a correct name: the flag is 1 in the shipped exe and makes `FindGameCdDrive` return the current directory (and `RegisterDataArchives` load the local archives), the opposite of the name's polarity (#6037) |
| `DAT_0051234c` | `src/orders/order_list.cpp` | `g_MissionOrderTableCapacityEnd` | not a global of its own: an unused declaration kept for the symbol ids it counts, the `_End` at +0xc of the vector at 0x512340 |
| `DAT_0051e598` | `src/network/unit_sync.cpp` | `g_pSyncMapC_NilNode` | already named by the compiler that emits it: `IUUnitSyncEntry::IU?$pair::?$_Tree::_Nil`; renaming the hand spelling makes `tools/check.py` demand that decorated name |
| `DAT_0051e59c` | `src/network/unit_sync.cpp` | `g_nSyncMapC_NilNodeRefs` | the same for the map's `_Nilrefs`: `IUUnitSyncEntry::IU?$pair::?$_Tree::_Nilrefs` |
