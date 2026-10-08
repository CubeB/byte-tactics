# Placeholders left

Phase 5 of `docs/cleanup-roadmap.md` ends when every `FUN_` name that can earn
a name has one, and the rest are listed here. These are the `FUN_` names that
were still left after their modules' rename pull requests; issue #6220 named
the ones it could and recorded the rest on this page.

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
| `FUN_0045b670` | `src/network/online_45b670.cpp` | `Onl_LoadConfigFile` | waits on a gap name mapping: only gap code calls it, and `tools/place.py` resolved the call through the address in the placeholder, so a rename adds a reloc row to `data/layout.csv`, which `tools/rename.py` refuses (the layout holds no names); the same limit left the online global unnamed in #6059 |
| `FUN_00467960` | `src/ingame/info_panel.cpp` | `Sensor_ApplyRadarJamFlag` | waits on #6019: the vtable slot names `RadarJamVisitor::ApplyRadarJamFlag` and the free function is the slot's body, so renaming it breaks the vtable check and `linkcmp.py` (#6158) |
| `FUN_00467980` | `src/ingame/info_panel.cpp` | `Sensor_ApplySonarJamFlag` | waits on #6019: the same, `SonarJamVisitor::ApplySonarJamFlag` (#6158) |
| `FUN_0046c8c0` | `src/network/unit_sync.cpp` | none | no evidence: an empty stub with no callers (#6170) |
| `FUN_0046c8d0` | `src/network/unit_sync.cpp` | none | no evidence: an empty stub with no callers (#6170) |
| `FUN_0047caf0` | `src/map/terrain.cpp` | none | no evidence: an empty body with no callers (#6163) |
| `FUN_00490200` | `src/game/victory_48dfb0.cpp` | `VictoryCond_IsLocalPlayerEliminated` | a dead alias: byte-identical to `IsLocalPlayerEliminated` at 0x490050, which already carries that name, and Thaldren marks this address a DEAD alias of it (#6195) |
