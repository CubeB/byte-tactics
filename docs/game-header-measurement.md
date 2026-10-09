# What a Game header would cost

Measurement for #6629, part of phase 3 of `docs/cleanup-roadmap.md`.
`docs/unit-header-measurement.md` left `Game` unmeasured: 143 files defined a
view of it, and the by-value sub-blocks it holds had no types of their own.
Since then the four field ranges (#6239 to #6242), the disagreements (#6332,
#6432) and the sub-blocks (#6443, #6486) have landed, `Game` is defined in 145
files, and this page repeats the `Unit` measurement for it without changing any
source: a candidate header in a scratch directory, the symbol fallout with
`tools/stateprobe.py`, and a swap test in a throwaway worktree that replaced
each file's own view of `Game` with an include of the candidate. The numbers
are a snapshot of `main` at 7d23bc863; later merges move individual verdicts.

The result in one line: the symbol fallout is the highest measured so far
(5.7%) but of the same order as the other headers, and the swap is worse than
for `Unit`: of the 145 files that define `Game`, 6 take the header as it is, 10
compile but lose every function in the file to the symbol counter, and 129 do
not compile, because they spell members differently, keep their own copy of a
type the header also defines (`Player` above all), or dereference a pointee the
header only forward-declares.

## The candidate

The candidate is `struct Game`, 0x3924d bytes (two views, the ones with
functions at 0x48bae0 and 0x48dfb0 in `src/ingame/selection.cpp` and
`src/game/victory.cpp`, declare 0x39553 bytes, 0x306 more than the merge keeps,
so those two cannot be covered). The names are the ones `uv run tools/gametypes.py
--explain Game` settled on, which is the `struct Game` in `include/ta_types.h`
(it reported 144 views joined, with five disputed byte ranges: 0x519 to 0x535,
0x14907 to 0x14927, 0x1491f to 0x1ab8f, 0x37ebd to 0x389b7 and 0x380eb to
0x38be5). It is scratch only (`build/scratch/6629/game.h`, 849 lines), not
committed, and pulls in nothing:

- Game itself is 463 lines: 55 `unknown_` gaps and 47 `field_` placeholders
  where the views disagree or none names the bytes, named members for the
  rest, the `#pragma pack(1)` the rest of `ta_types.h` uses (the offsets are
  unaligned), and the three member functions that header carries (`Game(void)`,
  `Current()`, `Current2()`).
- The 22 types held by value are laid out in front of it, each with its own
  by-value members first: `Settings`, `Entry_00463ca0`, `MapSize_00408090`,
  `ByteMap_00408090`, `Player` (0x14b bytes, with its five methods),
  `Arg_00419670`, `Point_0041cd50`, `CursorState_0041cc60`, `Def_0049d270`,
  `Grid`, `Handle`, `Debris_00420f30`, `Object3do`, `FaceFlags_459830`,
  `PieceFrame`, `Sub_004679a0_a`, `Sub_004679a0_b`, `Flags_00495e90_37f06`,
  `Bits_0045c570`, `Flags_0045c570`, `FrameTimers` and `Slot_0041e420`.
- `Vec3` and `Rect` are inline anonymous structs where they are held by value,
  as the `Unit` candidate did, because 37 and 12 files define their own and a
  named type would clash before any member is reached.
- The 56 pointer targets are forward declarations only. The header compiles
  alone with `sizeof(Game) == 0x3924d` and `sizeof(Player) == 0x14b`.

## The probe

`tools/stateprobe.py` force-includes the header in front of every file with a
matched function and counts the functions whose bytes change. The probe copy
has every name prefixed (`--rename`) so it cannot collide with the files' own
views:

```
build/probe/game_probe-98f2cb03.h in front of 271 files: 185 of 3267 matched
functions no longer match (5.7%)
```

No file failed to compile. The shared-type headers so far:

| Header | Probe | Functions changed | Files that kept their view |
| --- | --- | --- | --- |
| `HapiBank` (#6024) | not probed | | 5 of 43 (12%) |
| `Sound` (#6275) | 5.2% | 169 of 3267 | 7 of 15 (47%) |
| `Mission` (#6289) | 4.8% | 158 of 3267 | 25 of 43 (60%) |
| `UnitDef` (#6328) | 4.3% | 142 of 3267 | 9 of 26 (35%) |
| `Unit` (#6334) | 5.1% | 165 of 3267 | 80 of 83 (96%) |
| `Game` (#6629) | 5.7% | 185 of 3267 | 139 of 145 (96%, see below) |

The extra cost over `Unit` is the 22 helper types: a type is 7 symbol ids, so
the header adds about 150 ids from the types alone before any pointer target
or method, and the files that sit on a symbol-id edge (`gui.cpp` 14 functions,
`unit_scripts.cpp` 6, `plot_map.cpp` 6, `unit_types.cpp`, `samples.cpp`,
`radar.cpp`, `line_of_sight.cpp`, `unit_orders.cpp` and `console_commands.cpp`
4 each) are the same ones the earlier headers hit. As before the probe adds
the header in front of files that still have their own view, so it overstates
the swap for files that could adopt it.

## The swap

In a throwaway worktree (never committed), each of the 145 files' own `struct
Game { ... };` was replaced by `#include "game.h"` at the same place (141
files have the full view, three a one-line `char pad[...]` view with two
fields, one a pad with `first` and `unitsEnd`), each file was compiled with the
usual flags, and every matched function of the file in `data/progress.csv` was
compared (1,966 functions). The verdicts:

| Verdict | Files | Matched functions |
| --- | ---: | ---: |
| takes it, every function still matches | 6 | 20 |
| compiles, a function drops | 10 | 15 |
| does not compile | 129 | 1,931 |

- The 6 that take it: `ai_player_407d40.cpp`, `net_chat_464000.cpp`,
  `unit_orders_404ad0.cpp`, `vtol_orders_4111b0.cpp`, `vtol_orders_414770.cpp`
  and `unit_motion.cpp` (15 of the 20 functions). Having a small view is not
  enough: three of the four files whose view is only a pad and one or two
  fields drop a function (below), and the fourth needs a name (`first`).
- The 10 that compile and drop (all 15 functions of those files; every one
  matches without the swap, and nine of the ten are in the probe's list, so
  this is the symbol counter and not the swap itself): `multi_441220.cpp`
  (`0x441220` 90.3%), `players_4658e0.cpp` (`0x4658e0` 84.1%),
  `particles_475700.cpp` (`0x475700` 64.1%), `plot_map.cpp` (`0x484ce0`
  95.0%, `0x484df0` 96.2%, `0x484d60` 96.2%, `0x484e80` 97.1%, `0x484f50`
  91.3%, `0x484fa0` 93.5%), `terrain_47d820.cpp` (`0x47d820` 67.9%),
  `unit_orders_401e00.cpp` (`0x401e00` 99.4%), `vtol_orders_40fbe0.cpp`
  (`0x40fbe0` 94.5%), `vtol_orders_4103e0.cpp` (`0x4103e0` 99.5%),
  `vtol_orders_410850.cpp` (`0x410850` 86.3%) and `vtol_orders_412d40.cpp`
  (`0x412d40` 94.1%). Three of these (`vtol_orders_40fbe0.cpp`,
  `vtol_orders_4103e0.cpp`, `vtol_orders_410850.cpp`) are the minimal pad views:
  a view that declares nothing the header adds still pays the header's ids.
- The 129 that do not compile. The full per-file table is at the end of this
  page. `src/map/features.cpp` (whose `0x424050`, `0x424840` and `0x424890`
  fail on this machine on `main`) is among them, so that quirk did not
  influence any count.

The failures overlap, so the groups below count a file under every cause it
has (the compiler reports up to 100 errors, and 12 files hit that limit and
16 more stop on an unrecoverable error, so the member lists are a lower
bound):

| Cause | Files | Only this cause |
| --- | ---: | ---: |
| a member the header does not have under that name | 102 | 56 |
| the file defines a type the header also defines | 39 | 7 |
| a pointee the header only forward-declares | 35 | 11 |
| neither of those: a pointer type the file's view names differently | 9 | 9 |

1. **A different spelling of a member** (102 files, 358 distinct names, 539
   file and name pairs). Looking each name up in the header at the offset its
   view gives:
   - 253 pairs: the header names those bytes differently. The common ones are
     `mapInfo` and `campaign` for `net` (0x391e9), `victoryConditions` for
     `list_391ed`, `unitTypes`, `unitDefs` and `items`
     for `defs`, `frame`, `tick` and `field_38a47` for `ticks`, `colors` for
     `textColor`, `player` for `localPlayer`, `displayWidth`/`displayHeight` for
     `width_37f1b`/`height_37f1f`, `flags_37f2f` for `uiOptionFlags`,
     `flags_3923b` for `endGameFlags`, `unitTypeCount` for `count_1438f`.
   - 74 pairs: the header only has a placeholder there (`numPlayers` over
     `field_2a3c`, `sound` over `field_10`, `displayContext` over `unknown_c`,
     `frontendSubstateRequest` over `field_2bc0`, `color1` over `field_dcf`).
   - 56 pairs: the bytes are inside a gap or array the header could not
     split, mostly the unmerged block at 0x519 (`menu` 13 files, `gui`, `sub`,
     `message`, `field_531`: the `Menu` view that the merge refused), the
     331 bytes around `players[10]` (`playerInfo`, `p1b8a`, `p1cd5`) and
     `unknown_18`.
   - 156 pairs: bitfield and union alternatives (`bit0_37ebe`, `bits_3923b`,
     `flags_2bee`, `flag2`, `flag3`, `tick`) that a single member cannot
     carry.
   Most are one rename per use, but the table is long: `menu` (13 files),
   `mapInfo` (11), `numPlayers` (10), `sound` (10), `flags` (8),
   `displayContext` (8), `frame`, `tick` and `player` (6 each).
2. **The file defines one of the helper types** (39 files). `Player` in 31,
   `Grid` in 5, `FrameTimers` in 3, `Settings`, `Handle` and `MapSize_00408090`
   in 2 each, and `ByteMap_00408090`, `Object3do`, `Arg_00419670`,
   `Sub_004679a0_a`, `Sub_004679a0_b` and `Flags_00495e90_37f06` in one. One
   header cannot carry those names while the files keep their own definitions.
   Seven files fail for this alone (`ai_player_407e70.cpp`,
   `ai_player_408090.cpp`, `player_ai.cpp`, `economy.cpp`,
   `info_panel_46a610.cpp`, `vtol_orders_412710.cpp`, `vtol_orders_413470.cpp`).
   `Player` is the first error in 28 of the 129 files.
3. **A pointee the header only forward-declares** (35 files, 17 types):
   `Feature` and `Cell` (10 files each), `Unit` (7), `Mission` and
   `Cell_004816a0` (4), `UnitDef`, `Entry_00479620`, `Data_0046d970` and
   `Class_00437c80` (3), `PathMap`, `Obj_00406f50` and `Eye_00482130` (2), and
   `V4i`, `Src_004ab400`, `IconSet_00466780`, `FeatureSpot` and
   `BuildList_0041ace0` (1). A header that pulls in nothing cannot let the
   caller dereference `g_game->features`; the real header would include a
   header per pointee (`unit.h`, `unit_def.h` and so on), and every include
   costs symbol ids.
4. **The member is there but its type is the file's own numbered view** (9
   files only, 16 for `Player`): `g_game->players` is `Player*` in the header
   and `Player_00473a00*`, `Player_00474b80*`, `Player_00452960*` and so on in
   the files; the same for `Cell_N`, `Def_N` (`Def_0049d270`) against
   `UnitDef`, `Grid_N` and `Timers_N`. The 9 are `statistics.cpp`,
   `particles_473a00.cpp`, `particles_474b80.cpp`, `particles_475470.cpp`,
   `selection_48cf30.cpp`, `net_game_452960.cpp`, `net_game_453360.cpp`,
   `unit_commands_487bf0.cpp` and `weapon_types.cpp`. The numbered views of
   `Player`, `Cell` and `UnitDef` would have to be replaced by the shared type
   first.

## What it means for phase 3

- The symbol cost (5.7%) is in the same range as `Unit` and the others, and no
  file fails to compile because of the extra declarations. It is the 22
  by-value helper types, not the 460 data members, that cost ids.
- The adoption cost is larger than for `Unit`, and larger than anything else
  measured: 6 of 145 files take the header, 10 more compile but cannot keep
  their functions with the header's ids, and 129 need edits that are
  prerequisites in their own right. The `Unit` conclusion stands for `Game`
  with a longer list: `Game` is the last type, after the ones it holds.
- The prerequisites the swap exposed, in the order the failures suggest:
  1. a shared `Player` header (31 files redefine it and 16 reach it through a
     numbered view); then `Grid`, `FrameTimers`, `Settings`, `Handle` and the
     rest of the 22 helper types, or dropping those local views,
  2. retiring the numbered views of types the header names (`Player_N`,
     `Cell_N`, `Def_N`, `Grid_N`, `Timers_N`),
  3. headers for the pointees callers dereference (`Feature`, `Cell`, `Unit`,
     `Mission`, `UnitDef` and the rest of the 17), after which the real header
     can include them (the sizes of those includes in symbol ids are not
     measured here),
  4. a settled name for each disputed range, above all the 0x519 `Menu`
     block, the `players[10]` surroundings, the bitfield words at 0x37ebe,
     0x3923b and 0x2cc6, and the two views of 0x39553 bytes that disagree with
     everyone else about the size,
  5. per-module member renames for the 102 files that spell a member
     differently (the #6328 recipe), the cheapest step once the others are
     done.
- `Player` is the single biggest blocker: it is the first error in 28 of the
  129 files and one of the errors in 31.

## Limits and reproduction

- The probe overstates the swap for files that keep their view, and the swap
  is compile plus bytes only: no link, no `place.py` build.
- The candidate is a measurement artifact, not a reviewed header. It was built
  by `build/scratch/6629/build_header.py` from the `struct Game` of
  `include/ta_types.h` (which `gametypes.py --explain Game` reports as its
  merge), taking the closure of the by-value types and forward-declaring every
  other name; nothing was hand-edited. `Vec3` and `Rect` were inlined. A
  version with named `Vec3` and `Rect` would fail more files, since 37 and 12
  files define them.
- The swap replaces a file's view of `Game` only; other views of the same
  object (`Game_00449bb0`, `Game_0041b2e0`, ...) keep their names and were not
  tried. The 145 files are those with `struct Game {` (or `class Game {`) at
  the start of a line.
- Every one of the 145 files was tried; none is unmeasured. A compile error
  list is truncated at 100 errors, so for the 28 files that stop early the
  member names and helper types listed are the first ones, not all of them,
  and a file that fails only on `Player` may hide further failures behind it.
  Baselines for the 10 files that drop were run on the unswapped tree
  (15 of 15 MATCH).
- The first probe run failed on every file because the renaming pass prefixed
  the bitfield widths (`: 1;`); the probe header had the widths put in
  parentheses and the number above is the second run. This is a quirk of
  `stateprobe.py --rename`, not of the header.
- The compile jobs ran three at a time; the probe and the swap overlapped for
  part of the run, which only affects wall time.

## Per file

Verdicts for the swap, one row per file. `redefines` lists helper types the
file also defines, `member` the members the header does not have under that
name (first four, then a count), `incomplete` the pointees that are only
forward-declared, and `type clash` the numbered view against the header's type.

| File | Verdict | Detail |
| --- | --- | --- |
| `src/ai/ai_player.cpp` | does not compile | redefines `Player`; member `matchFlags`, `playerInfo` |
| `src/ai/ai_player_407d40.cpp` | takes it |  |
| `src/ai/ai_player_407e70.cpp` | does not compile | redefines `ByteMap_00408090`, `MapSize_00408090` |
| `src/ai/ai_player_408090.cpp` | does not compile | redefines `MapSize_00408090` |
| `src/ai/ai_player_408620.cpp` | does not compile | member `items` |
| `src/ai/ai_player_409730.cpp` | does not compile | incomplete `UnitDef` |
| `src/ai/ai_profile.cpp` | does not compile | member `mapInfo`, `unitDefs` |
| `src/ai/pathfind.cpp` | does not compile | redefines `Grid`, `Player`; member `numPlayers` |
| `src/ai/player_ai.cpp` | does not compile | redefines `Player` |
| `src/frontend/campaign_menu.cpp` | does not compile | member `frontendSubstateRequest`, `menu`, `side`; incomplete `Data_0046d970` |
| `src/frontend/endgame.cpp` | does not compile | redefines `Grid`, `Player`; member `bar`, `bit4_3923b`, `campaign`, `color1` +9 |
| `src/frontend/frontend.cpp` | does not compile | redefines `Player`; member `chatMode`, `cursorKeyFlags`, `dplayAddressDialogFlags`, `flags` +7 |
| `src/frontend/multi.cpp` | does not compile | redefines `Player`; member `dplayAddressDialogFlags`, `frontendSubstate`, `frontendSubstateRequest`, `info` +1 |
| `src/frontend/multi_441220.cpp` | compiles, function drops | `0x441220` 90.3% |
| `src/frontend/multi_441460.cpp` | does not compile | member `provider`, `sub`, `unknown_14`, `version` |
| `src/frontend/multi_443ff0.cpp` | does not compile | member `info`; incomplete `V4i` |
| `src/frontend/multi_44a680.cpp` | does not compile | redefines `Player`; member `dirty`, `frame`, `frontendSubstateRequest`, `gui` +2 |
| `src/frontend/multi_44c420.cpp` | does not compile | member `items`, `queue` |
| `src/frontend/options.cpp` | does not compile | member `bit2`, `bit3`, `bit4`, `bit5` +7 |
| `src/frontend/options_45c820.cpp` | does not compile | member `sound` |
| `src/frontend/skirmish_menu_479660.cpp` | does not compile | redefines `Player`; member `colorCount`, `menu`; incomplete `Src_004ab400` |
| `src/game/console_commands.cpp` | does not compile | redefines `Player`; member `b2`, `b3`, `b4`, `b6` +14; incomplete `Class_00437c80` |
| `src/game/data_files_429000.cpp` | does not compile | member `animCount`, `anims`, `messageBox`, `sounds` |
| `src/game/economy.cpp` | does not compile | redefines `Player` |
| `src/game/economy_401360.cpp` | does not compile | member `wind` |
| `src/game/game_load.cpp` | does not compile | member `color1`, `displayContext`, `displayHeight`, `displayWidth` +28 |
| `src/game/game_state_490ac0.cpp` | does not compile | member `assem`, `bit0_37ebe`, `bit11_37ebe`, `bit2_3923b` +24 |
| `src/game/main.cpp` | does not compile | member `displayContext`, `musicMode`, `sound`, `version` +2 |
| `src/game/main_loop.cpp` | does not compile | member `bit0_14281`, `bit0_37ebe`, `bit1_14281`, `bit2` +19 |
| `src/game/players_464290.cpp` | does not compile | redefines `Player`; member `blinkOn`, `flags_3923b`, `map`, `menu` +4; incomplete `PathMap` |
| `src/game/players_4658e0.cpp` | compiles, function drops | `0x4658e0` 84.1% |
| `src/game/players_465ac0.cpp` | does not compile | member `limitY` |
| `src/game/savegame.cpp` | does not compile | member `buf391cf`, `flag_38a51`, `flags_3923b`, `mapInfo` +11 |
| `src/game/settings.cpp` | does not compile | member `cdmode`, `displayContext`, `displaymodeHeight`, `displaymodeWidth` +15; incomplete `Entry_00479620` |
| `src/game/settings_campaign.cpp` | does not compile | member `buildDate`, `buildTime`, `campaign`, `numPlayers` +4; incomplete `Data_0046d970`, `Entry_00479620` |
| `src/game/share_commands.cpp` | does not compile | redefines `Player`; member `compressionOff`, `flags`, `showBps` |
| `src/game/side_data.cpp` | does not compile | member `field_148d7`, `sideCount` |
| `src/game/statistics.cpp` | does not compile | C2440: '=' : cannot convert from 'class Mission *' to 'char *' |
| `src/game/victory.cpp` | does not compile | redefines `Player`; member `field_1df2`, `player`, `units2` |
| `src/game/victory_490230.cpp` | does not compile | member `player` |
| `src/graphics/font.cpp` | does not compile | member `colour1`, `colour2`, `colour3`, `font` |
| `src/graphics/model_render.cpp` | does not compile | redefines `Object3do`; member `visualFlags`; incomplete `Class_00437c80` |
| `src/graphics/model_render_4589c0.cpp` | does not compile | member `visualFlags` |
| `src/graphics/movies.cpp` | does not compile | member `display` |
| `src/graphics/particles.cpp` | does not compile | member `bits_147f3`, `field_38a47`, `lists_00471d90`, `lists_00471eb0` +13 |
| `src/graphics/particles_472630.cpp` | does not compile | member `players_004745e0` |
| `src/graphics/particles_473a00.cpp` | does not compile | type clash `Player`/`Player_00473a00` |
| `src/graphics/particles_474b80.cpp` | does not compile | type clash `Player`/`Player_00474b80` |
| `src/graphics/particles_475470.cpp` | does not compile | type clash `Player`/`Player_00475470` |
| `src/graphics/particles_475700.cpp` | compiles, function drops | `0x475700` 64.1% |
| `src/ingame/build_placement.cpp` | does not compile | member `field_2cac`, `field_2cb0`, `field_2cb4`, `field_531` +5; incomplete `Entry_00479620`, `Unit` |
| `src/ingame/camera.cpp` | does not compile | member `origin_x`, `origin_y`, `screen_w`, `value_142f7` +2; incomplete `Mission` |
| `src/ingame/control_panel.cpp` | does not compile | redefines `Arg_00419670`, `Player`; member `buildTypes`, `flags`, `menu`, `orderState` +2 |
| `src/ingame/control_panel_41bde0.cpp` | does not compile | member `flags` |
| `src/ingame/dialogs_493340.cpp` | does not compile | redefines `FrameTimers`, `Player`; member `bit11_37ebe`, `bit6_37ebe`, `bits_2bee`, `menu` +2 |
| `src/ingame/info_panel.cpp` | does not compile | redefines `FrameTimers`, `Handle`, `Sub_004679a0_a`, `Sub_004679a0_b`; member `color`, `color_00467a50`, `colors`, `f_37e23` +9 |
| `src/ingame/info_panel_467440.cpp` | does not compile | member `numPlayers` |
| `src/ingame/info_panel_468cf0.cpp` | does not compile | redefines `FrameTimers`; member `bits_37f2f`, `bits_3923b`, `colors`, `cursorScreenX` +21; incomplete `Data_0046d970`, `Feature`, `Unit` |
| `src/ingame/info_panel_46a610.cpp` | does not compile | redefines `Handle` |
| `src/ingame/info_panel_46a860.cpp` | does not compile | member `bits_3923b`, `colors`, `displayContext`, `field_14353` +10; incomplete `Cell`, `Feature`, `Unit` |
| `src/ingame/keys.cpp` | does not compile | redefines `Flags_00495e90_37f06`; member `field_38a53`, `field_38b53`, `field_531`, `flags_37f2f` +9 |
| `src/ingame/markers.cpp` | does not compile | member `anims`, `color1`, `field_dd9`, `frame` +1 |
| `src/ingame/screenshots_499890.cpp` | does not compile | member `callback`, `displayContext`, `manager`, `menu` +4 |
| `src/ingame/selection.cpp` | does not compile | redefines `Player`; member `player` |
| `src/ingame/selection_48cf30.cpp` | does not compile | type clash `Player`/`Player_0048cf30` |
| `src/map/features.cpp` | does not compile | redefines `Player`; member `flags_37f2f`, `pool`; incomplete `FeatureSpot` |
| `src/map/features_421f20.cpp` | does not compile | member `anim`, `unitDefs` |
| `src/map/features_4224b0.cpp` | does not compile | incomplete `Feature` |
| `src/map/features_422ea0.cpp` | does not compile | incomplete `Feature` |
| `src/map/line_of_sight.cpp` | does not compile | redefines `Grid`, `Player`; member `colors`, `grid`, `grid2`, `sortIndices` +4; incomplete `Cell_004816a0`, `Eye_00482130`, `IconSet_00466780` |
| `src/map/line_of_sight_4816a0.cpp` | does not compile | member `viewDirtyFlags` |
| `src/map/line_of_sight_481930.cpp` | does not compile | member `flag2`, `flag3`; incomplete `Cell_004816a0` |
| `src/map/line_of_sight_481d50.cpp` | does not compile | member `flag2`, `flag3`, `flags_142f1_mapChanged` |
| `src/map/line_of_sight_482270.cpp` | does not compile | member `flag2`, `flag3`, `flagA` |
| `src/map/line_of_sight_482910.cpp` | does not compile | incomplete `Cell_004816a0`, `Eye_00482130` |
| `src/map/line_of_sight_482ac0.cpp` | does not compile | incomplete `Cell_004816a0` |
| `src/map/line_of_sight_4843c0.cpp` | does not compile | redefines `Grid`; member `info` |
| `src/map/map_list.cpp` | does not compile | member `lineOfSight`, `mapInfo`, `mapping`, `messages` +4 |
| `src/map/meteors.cpp` | does not compile | member `field_38a47` |
| `src/map/plot_map.cpp` | compiles, function drops | `0x484ce0` 95.0%, `0x484df0` 96.2%, `0x484d60` 96.2%, `0x484e80` 97.1%, `0x484f50` 91.3%, `0x484fa0` 93.5% |
| `src/map/radar.cpp` | does not compile | member `currentPlayer`, `dim`, `field_142cb`, `field_dd9` +13 |
| `src/map/terrain.cpp` | does not compile | redefines `Grid`, `Player`; member `featureBytes`, `grid`, `player` |
| `src/map/terrain_47cc30.cpp` | does not compile | member `ownerCols`, `owners` |
| `src/map/terrain_47d0e0.cpp` | does not compile | incomplete `Cell` |
| `src/map/terrain_47d820.cpp` | compiles, function drops | `0x47d820` 67.9% |
| `src/map/terrain_47e2d0.cpp` | does not compile | member `maxFeature` |
| `src/network/net_chat.cpp` | does not compile | member `colors`, `font`, `mapInfo`, `max_lines` +2 |
| `src/network/net_chat_464000.cpp` | takes it |  |
| `src/network/net_game.cpp` | does not compile | redefines `Player`, `Settings`; member `campaign`, `connection`, `displayHeight`, `displayWidth` +13 |
| `src/network/net_game_450530.cpp` | does not compile | member `local_player` |
| `src/network/net_game_452960.cpp` | does not compile | type clash `Player`/`Player_00452960` |
| `src/network/net_game_452cc0.cpp` | does not compile | redefines `Player`; member `flags`, `numPlayers` |
| `src/network/net_game_453360.cpp` | does not compile | type clash `Player`/`Player_00453360` |
| `src/network/net_game_453d40.cpp` | does not compile | redefines `Player`, `Settings`; member `local` |
| `src/network/net_stats.cpp` | does not compile | member `items` |
| `src/network/packets_460f40.cpp` | does not compile | redefines `Player`; member `tick` |
| `src/network/unit_sync.cpp` | does not compile | member `mapInfo`, `player` |
| `src/network/unit_sync_46ca60.cpp` | does not compile | member `sync` |
| `src/network/unit_sync_46d2e0.cpp` | does not compile | incomplete `UnitDef` |
| `src/network/unit_sync_46dad0.cpp` | does not compile | incomplete `UnitDef` |
| `src/network/unit_sync_player.cpp` | does not compile | member `sync` |
| `src/orders/order_dispatch_43f0e0.cpp` | does not compile | member `flag37efa`, `localPlayerBit`, `multiplayer`, `threshold` +1; incomplete `Feature` |
| `src/orders/order_list.cpp` | does not compile | member `count2`, `field_38a47`, `frame`, `unitTypeCount` +1 |
| `src/orders/order_queue_4384a0.cpp` | does not compile | member `unitTypeCount`, `unitTypes` |
| `src/orders/order_targets.cpp` | does not compile | incomplete `PathMap` |
| `src/orders/unit_orders.cpp` | does not compile | redefines `Player`; member `unitTypes` |
| `src/orders/unit_orders_401e00.cpp` | compiles, function drops | `0x401e00` 99.4% |
| `src/orders/unit_orders_403a20.cpp` | does not compile | member `tick` |
| `src/orders/unit_orders_403f70.cpp` | does not compile | member `tick` |
| `src/orders/unit_orders_404ad0.cpp` | takes it |  |
| `src/orders/unit_orders_404db0.cpp` | does not compile | member `unitTypes` |
| `src/orders/unit_orders_406aa0.cpp` | does not compile | member `first` |
| `src/orders/vtol_orders.cpp` | does not compile | member `tick` |
| `src/orders/vtol_orders_40fbe0.cpp` | compiles, function drops | `0x40fbe0` 94.5% |
| `src/orders/vtol_orders_4103e0.cpp` | compiles, function drops | `0x4103e0` 99.5% |
| `src/orders/vtol_orders_410850.cpp` | compiles, function drops | `0x410850` 86.3% |
| `src/orders/vtol_orders_4111b0.cpp` | takes it |  |
| `src/orders/vtol_orders_411f50.cpp` | does not compile | member `mapInfo` |
| `src/orders/vtol_orders_412710.cpp` | does not compile | redefines `Player` |
| `src/orders/vtol_orders_412d40.cpp` | compiles, function drops | `0x412d40` 94.1% |
| `src/orders/vtol_orders_413470.cpp` | does not compile | redefines `Player` |
| `src/orders/vtol_orders_414380.cpp` | does not compile | member `tick` |
| `src/orders/vtol_orders_414770.cpp` | takes it |  |
| `src/sound/sound_47ed40.cpp` | does not compile | member `categories`, `displayContext`, `flags_37f19`, `frame` +1 |
| `src/sound/sound_types.cpp` | does not compile | member `sounds` |
| `src/units/movement_class.cpp` | does not compile | member `mapInfo`, `players2`; incomplete `Cell`, `Unit` |
| `src/units/unit.cpp` | does not compile | member `f1427f` |
| `src/units/unit_commands_487bf0.cpp` | does not compile | type clash `Player`/`Player_00488310` |
| `src/units/unit_motion.cpp` | takes it |  |
| `src/units/unit_position.cpp` | does not compile | redefines `Player`; member `autoFollowFlags`, `f14371`, `f2a44` |
| `src/units/unit_save.cpp` | does not compile | member `mapInfo` |
| `src/units/unit_script.cpp` | does not compile | redefines `Player`; member `limitY`, `sources` |
| `src/units/unit_scripts.cpp` | does not compile | member `localPlayerBit`, `multiplayer`; incomplete `Feature` |
| `src/units/unit_targets_489280.cpp` | does not compile | member `frame`, `gridH`, `gridW`, `hmaps` +2 |
| `src/units/unit_types.cpp` | does not compile | member `categories`, `categoryCount`, `displayContext`, `models` +2; incomplete `BuildList_0041ace0`, `Class_00437c80` |
| `src/units/unit_types_42a8d0.cpp` | does not compile | member `unit_count`, `unitinfo`, `version_major`, `version_minor` |
| `src/units/units_485010.cpp` | does not compile | redefines `Player`; member `autoFollowFlags`, `b7`, `conditions`, `definitions` +5; incomplete `Cell`, `Feature` |
| `src/units/units_485070.cpp` | does not compile | incomplete `Cell` |
| `src/units/units_4851c0.cpp` | does not compile | incomplete `Cell`, `Feature` |
| `src/weapons/explosions.cpp` | does not compile | redefines `Player`; member `explosions`, `image`, `pieces`, `viewport`; incomplete `Mission` |
| `src/weapons/weapon_types.cpp` | does not compile | type clash `Def_0049d270`/`Weapon_0042e440` |
| `src/weapons/weapons.cpp` | does not compile | redefines `Player`; member `frame`, `now`, `projCount`, `selectedProjectile` +3; incomplete `Mission`, `Obj_00406f50` |
| `src/weapons/weapons_49b090.cpp` | does not compile | member `lastPos`, `lastSound`, `limit`; incomplete `Feature`, `Mission` |
| `src/weapons/weapons_49b720.cpp` | does not compile | member `projCount`, `projs` |
| `src/weapons/weapons_49be60.cpp` | does not compile | member `field_37e27`, `gaf_147f3`, `palette`, `time`; incomplete `Obj_00406f50` |
