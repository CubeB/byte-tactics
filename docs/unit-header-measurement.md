# What a Unit header would cost

Measurement for #6334, part of phase 3 of `docs/cleanup-roadmap.md`. The
roadmap leaves open how much of phase 3 to attempt and whether `Unit` (and
`Game`) get one header each; the pilots so far give a fallout rate per type.
This page measures `Unit` the same way, without changing any source: a
candidate header in a scratch directory, the symbol fallout with
`tools/stateprobe.py`, and a swap test in a throwaway worktree that replaced
each file's own view of `Unit` with an include of the candidate. The numbers
are a snapshot of `main` at cbc464a70; later merges move individual verdicts.

The result in one line: the symbol fallout is in line with the other headers
(5.1%, no file fails to compile because of the extra declarations), but the
swap shows the real cost: with one name per offset only 3 of the 83 files
that define `Unit` take the header as they are, 2 keep their view because a
function drops, and 78 do not compile, mostly because they spell a member
differently.

## The candidate

The candidate is `struct Unit` (Thaldren's `UnitInstance`, 0x118 bytes, the
size every view agrees on), with the field names #6244 settled and the eight
member functions the exe has. It pulls in nothing: the pointer members are
forward declarations, and the by-value blocks are laid out in the header.
It is scratch only (`build/scratch/6334/unit.h`), not committed.

| Offset | Name | Type | Note |
| --- | --- | --- | --- |
| 0x0 | `motion` | `UnitMotion*` | Thaldren's pMotion; five views already name it |
| 0x4 | `weapons[3]` | `UnitWeaponSlot` | stride 0x1c, Thaldren's aWeapons |
| 0x58 | `extraction` | `float` | |
| 0x5c | `list` | `Order*` | mission queue head |
| 0x60 | `list2` | `Order*` | the flag 0x40000 queue |
| 0x64 | `bank` | `short` | |
| 0x66 | `heading` | `unsigned short` | |
| 0x68 | `pitch` | `short` | |
| 0x6a | `pos` | 3 ints, inline | some views give the y word a fractional half |
| 0x76 | `cell` | 2 shorts, inline | |
| 0x7a | `losCacheCellX` | `short` | |
| 0x7c | `losCacheCellZ` | `short` | |
| 0x7e | `footprint` | 2 shorts, inline | |
| 0x82 | `spatialBucket` | `SpatialBucket*` | |
| 0x86 | `carrier` | `Unit*` | |
| 0x8a | `cargo` | `Unit*` | |
| 0x8e | `cargoNext` | `Unit*` | |
| 0x92 | `def` | `UnitDef*` | |
| 0x96 | `player` | `Player*` | |
| 0x9a | `script` | `CobScript*` | |
| 0x9e | `state` | `ObjectState*` | |
| 0xa2 | `head` | `PathOrderAttach*` | |
| 0xa6 | `unitDefIndex` | `unsigned short` | |
| 0xa8 | `id` | `unsigned short` | |
| 0xaa | `hoverBobPhase` | `unsigned short` | |
| 0xac | `group` | `int` | |
| 0xb0 | `workTime` | `int` | |
| 0xb4 | `unknown_b4[4]` | `char` | no view names it |
| 0xb8 | `killCount` | `unsigned short` | |
| 0xba | `netDirtyFlags` | `unsigned short` | |
| 0xbc | `resourceSlot` | `UnitResources` | 0x34 bytes, see below |
| 0xf0 | `attacker` | `Unit*` | |
| 0xf4 | `lastAttackerSlot` | `unsigned char` | |
| 0xf5 | `lastDamageType` | `unsigned char` | |
| 0xf6 | `healthPercent` | `unsigned char` | |
| 0xf7 | `prevHealthPercent` | `unsigned char` | |
| 0xf8 | `losSightFrameIdx` | `unsigned char` | |
| 0xf9 | `transportPiece` | `unsigned char` | |
| 0xfa | `recentlyDamagedTimer` | `unsigned char` | |
| 0xfb | `postTransferHoldoff` | `int` | |
| 0xff | `playerIndex` | `unsigned char` | |
| 0x100 | `unfinishedInitZero` | `int` | |
| 0x104 | `buildLeft` | `float` | |
| 0x108 | `health` | `short` | |
| 0x10a | `unknown_10a[4]` | `char` | the views here spell it `state` or `team` |
| 0x10e | `activateFlags` | `unsigned char` | |
| 0x10f | `cobStateFlags` | `unsigned char` | |
| 0x110 | `flags` | `unsigned int` | |
| 0x114 | `zBufferFlag` | `unsigned int` | |

The methods are the real signatures from `data/progress.csv`: `ChooseWeapon`
(`unsigned char`), `ReleaseWeapons(unsigned char)`,
`ClaimWeapons(unsigned char)`, `CanReclaim(void*)`, `CanRepair(Unit*)`,
`CountCargo()`, `CanLoad(Unit*)`, `SetStateBits(int, int)`, plus the inline
`GetState()` and `Ready()` the callers define themselves. (`PlayerIndex()` is
not declared: its callers define it inline as `player->index`, and a header
that pulls in nothing cannot compile that body.) Three choices worth
recording:

- The by-value weapon slot (`UnitWeaponSlot`, 0x1c bytes) carries Thaldren's
  fields (aim target pair, aimCob, weapon, reloadTimer, fireHeading,
  firePitch, stockpile, flags). The resource block is `UnitResources`,
  0x34 bytes, with `economy.cpp`'s method signatures.
- `pos`, `cell` and `footprint` are inline anonymous structs, not `Vec3` and
  `Point16`: 43 of the 83 files define their own `struct Vec3` and 16 their
  own `Point16`, so naming those types in the header would clash in those
  files before any member name is reached. The by-value `UnitResources` and
  `UnitWeaponSlot` cannot be dodged that way, and their redefinitions show up
  in the swap below.
- It is `struct Unit`, not `class Unit`: `data/progress.csv` has 285 symbols
  with `PAUUnit` against 26 with `PAVUnit`, and the class key of an earlier
  declaration decides the mangling in a file (`unit_position.cpp` has a
  `class Unit;` forward declaration above its `struct Unit` and mangulates
  `PAV`). A `class Unit` candidate changed those mangled names, which the
  checker matches on, and turned three files that take the header below into
  name errors in the first run.

The by-value helpers in full: `UnitWeaponSlot` is Thaldren's slot, 0x1c
bytes (`aimTargetXOrUnitId` short +0x0, `aimTargetZOrUnitSentinel` short +0x2,
`aimCob` `WeaponAimCobCb` +0x4, `weapon` `WeaponDef*` +0xc,
`muzzleAimFromDeltaZ` int +0x10, `reloadTimer` unsigned short +0x14,
`fireHeading` short +0x16, `firePitch` short +0x18, `stockpile` byte +0x1a,
`flags` byte +0x1b); `WeaponAimCobCb` is 8 bytes with `OnAimCobReturn(int)`;
`UnitResources` is 0x34 bytes (12 unnamed ints and a `Player*` at +0x30) with
`economy.cpp`'s methods (`Reset`, `SaveUnitAccounts`, `LoadUnitAccounts`,
`RequestEnergy`, `RequestEnergyAndMetal`, `SpendEnergy`, `SpendMetal`,
`SpendEnergyAndMetal`); `UnitInfo` is unit_save.cpp's 0xaa-byte save record
with `id` at +0xa8.

## The probe

`tools/stateprobe.py` force-includes the header in front of every file with a
matched function and counts the functions whose bytes change. The probe copy
has every class name prefixed (`probe_Unit`) so it cannot collide with the
files' own views:

```
uv run tools/stateprobe.py build/scratch/6334/unit_probe.h
build/probe/unit_probe-8ba6e8ec.h in front of 279 files: 165 of 3267 matched
functions no longer match (5.1%)
```

No file failed to compile. The shared-type headers so far:

| Header | Probe | Functions changed | Files that kept their view |
| --- | --- | --- | --- |
| `HapiBank` (#6024) | not probed | | 5 of 43 (12%) |
| `Sound` (#6275) | 5.2% | 169 of 3267 | 7 of 15 (47%) |
| `Mission` (#6289) | 4.8% | 158 of 3267 | 25 of 43 (60%) |
| `UnitDef` (#6328) | 4.3% | 142 of 3267 | 9 of 26 (35%) |
| `Unit` (#6334) | 5.1% | 165 of 3267 | 80 of 83 (96%, see below) |

The probe adds the header in front of files that still have their own view,
so it overstates the swap for the files that could adopt it; on the other
hand `Unit` is the first measured type whose by-value members bring helper
types with them (the slot and the resource block), which the others did not
have.

## The swap

In a throwaway worktree (never committed), each file's own `Unit` view was
replaced by `#include "unit.h"`, the file was compiled with the usual flags,
and every matched function of the file in `data/progress.csv` was compared
(1,194 functions in the 83 files). The verdicts:

- 3 files take it and every function still matches: `ai_player_408f30.cpp`,
  `unit_orders_406c10.cpp`, `unit_orders_406c40.cpp`. Their views declare
  `Unit` only as an opaque pointer with one field, so the header replaces the
  view without any use changing.
- 2 files compile and a function drops:
  `ai/ai_player_409730.cpp` (`0x409730` 98.0%) and
  `game/console_commands.cpp` (`0x417030` 28.6%, `0x417600` 41.7%,
  `0x4181d0` 98.6%, `0x418310` 82.7%). Both are files with a documented
  symbol-id sensitivity (0x409730 matched only with its ids made small,
  `docs/c2-regalloc.md`), and the four console_commands functions are the
  same four the `Sound` header drops, with the same scores.
- 78 files do not compile. The full per-file table is at the end of this
  page.

The failures fall into four groups:

1. **A different spelling of a member** (53 files). The biggest groups:
   `owner` against `player` (12 files), `type` against `def`,
   `active` against `motion`, `progress` and `speed` against `buildLeft`,
   `flags_110` against `flags`, `busy` against `spatialBucket`, `angle`
   against `heading`, `timeout` against `workTime`, `rot` against the three
   angle shorts, `s108` against `health`, `transporter`/`carried`/
   `nextCarried` against `carrier`/`cargo`/`cargoNext`, `pieces` against
   `weapons`, `b1e`/`b1f`/`b3a`/`b56` against the weapon slot bytes, and the
   state flags (`on`/`on2`/`bit0` to `bit3`) against `activateFlags` and
   `cobStateFlags`. Most are one rename per use, the kind of edit #6328 did
   while adopting `UnitDef`.
2. **The file defines one of the helper types** (8 files): `UnitInfo`
   (`economy.cpp`), `UnitResources` (`players_464290.cpp`,
   `control_panel.cpp`, `unit_orders.cpp`, `unit_save.cpp`),
   `UnitWeaponSlot` (`markers.cpp`, `weapons.cpp`) and `WeaponAimCobCb`
   (`units_485010.cpp`). One header cannot carry those names while the files
   keep their own definitions; they would need shared headers (or to drop
   their views) first.
3. **A pointee the header only forward-declares** (6 files): `UnitDef` in
   `ai_player_407e70.cpp`, `ai_player_40b530.cpp`, `unit_orders_401e00.cpp`,
   `unit_orders_406aa0.cpp` and `unit_targets.cpp`, and `Player` in
   `info_panel_467440.cpp`. The real header would include `unit_def.h` (and a
   player header when one exists); a header that pulls in nothing cannot let
   the caller dereference `def`.
4. **A type or union the single member cannot carry** (11 files): the state
   pointer as `SpotState*` (`info_panel_46a610.cpp`), `def` as a
   `UnitType*`/`char*` (`selection.cpp`, `sound.cpp`), whole-`Vec3` or
   whole-`Point` assignments (`order_list.cpp`, `unit_orders_405980.cpp`,
   `line_of_sight_482ac0.cpp`, `net_game.cpp`), a union at +0x10 read as
   `mover`/`field_10` (`vtol_orders.cpp`, `vtol_orders_4118e0.cpp`,
   `vtol_orders_412710.cpp`, `vtol_orders_413470.cpp`), `active` as an
   `int` at +0x0 (`unit_orders_403a20.cpp` and the CreateUnit call), a
   bitfield view of `flags` (`terrain.cpp`), and
   `CanRepair(Unit_0043e490*)` (`order_dispatch.cpp`).

The genuinely hard cases are in group 4: the `side` byte some campaign views
read at +0x95 sits inside the `def` pointer, the `mover` pointer some VTOL
views read at +0x10 sits inside weapon slot 0, and the `name` some sound code
reads at +0x92 is a `char*` where the rest of the game has `UnitDef*`. Those
need a decision about which view is the real one, not a rename.

## What it means for phase 3

- The symbol cost of one `Unit` header is the highest measured so far but in
  the same range as `Sound`, `Mission` and `UnitDef` (5.1% against 5.2%,
  4.8%, 4.3%), and it does not break any file by itself. The extra
  declarations come from the class, its methods and the two helper types, not
  from the fields (data members cost no symbol ids).
- The adoption cost is dominated by the views' spellings: 53 of the 83 files
  need a member renamed, 8 keep a helper type the header also defines, 6 need
  a pointee complete, and 11 carry a type or union a single member cannot
  express. In its current shape the header replaces no view without edits, so
  `Unit` stays a last type, as the roadmap says.
- The prerequisites the swap exposed, in the order the failures suggest:
  1. a shared `Vec3`/`Point16` header (43 and 16 files define their own; the
     inline structs in the candidate dodge the clash but not the whole-value
     assignments),
  2. shared headers for `UnitResources`, `UnitWeaponSlot` and
     `WeaponAimCobCb`, or dropping those local views,
  3. `unit_def.h` in front of the files that dereference `def` (the header
     already exists, adopted by #6328),
  4. per-module member renames for the 53 files (the #6328 recipe),
  5. a decision on the conflicting views (`side` at +0x95, `mover` at +0x10,
     `name` at +0x92, the angle triple, `state`'s type).

## Game

`Game` was not swapped: 143 files define a view of it, and it has no member
functions in the exe (data only). The views use 811 distinct `g_game->` names
in 7,083 places, and `include/ta_types.h` already carries the merge from 904
views (0x3924d bytes, 2026-10-04), but that merge holds 19 by-value members
(`Settings` at +0x471, `Entry_00463ca0[30]` at +0x12ef, `Player[10]` at
+0x1b63, `Grid`, `Rect`, `Vec3`, `Object3do`, `FrameTimers`, and so on) whose
types have no headers of their own yet. A self-contained `game.h` cannot
exist before those do, so the same swap would fail on missing types everywhere
rather than measure anything. The symbol side is cheap by comparison: a type
is 7 ids and data members cost none, so `Game`'s probe number would be small;
the cost is the compile side. `Game` deserves its own measurement issue after
its by-value types have headers.

## Limits and reproduction

- The probe overstates the swap for files that keep their view, and the swap
  is compile plus bytes only: no link, no `place.py` build.
- The candidate is a measurement artifact, not a reviewed header, and no
  source, data or docs file changed for it.
- The scratch files (`build/scratch/6334/unit.h`, `unit_probe.h`, `swap.py`,
  the probe logs) live outside the tree's tracked files, as the issue asks;
  this page is the record.

```
uv run tools/stateprobe.py build/scratch/6334/unit_probe.h   # the 5.1%
# throwaway worktree: copy unit.h to include/, replace each view with
# #include "unit.h", then:
uv run python3 build/scratch/6334/swap.py                    # the 83 verdicts
```

## The 83 files, one by one

| File | Verdict | Detail |
| --- | --- | --- |
| `src/ai/ai_player.cpp` | does not compile | C2084 function 'int Unit::Ready(void)const ' already has a body |
| `src/ai/ai_player_407e70.cpp` | does not compile | C2027 use of undefined type 'UnitDef' |
| `src/ai/ai_player_408f30.cpp` | takes it | |
| `src/ai/ai_player_409730.cpp` | drops | `0x409730` 98.0% |
| `src/ai/ai_player_40b1c0.cpp` | does not compile | C2039 is not a member of 'Unit' |
| `src/ai/ai_player_40b530.cpp` | does not compile | C2027 use of undefined type 'UnitDef' |
| `src/ai/pathfind.cpp` | does not compile | C2039 is not a member of 'Unit' |
| `src/ai/player_ai.cpp` | does not compile | C2039 is not a member of 'Unit' |
| `src/frontend/campaign_menu.cpp` | does not compile | C2039 is not a member of 'Unit' |
| `src/frontend/skirmish_menu.cpp` | does not compile | C2039 is not a member of 'Unit' |
| `src/game/console_commands.cpp` | drops | `0x417030` 28.6%, `0x417600` 41.7%, `0x4181d0` 98.6%, `0x418310` 82.7% |
| `src/game/economy.cpp` | does not compile | C2011 'struct' type redefinition |
| `src/game/economy_401360.cpp` | does not compile | C2039 is not a member of 'Unit' |
| `src/game/players_464290.cpp` | does not compile | C2011 'class' type redefinition |
| `src/game/players_465ac0.cpp` | does not compile | C2039 is not a member of 'Unit' |
| `src/game/settings_campaign.cpp` | does not compile | C2039 is not a member of 'Unit' |
| `src/game/victory.cpp` | does not compile | C2039 is not a member of 'Unit' |
| `src/ingame/control_panel.cpp` | does not compile | C2011 'class' type redefinition |
| `src/ingame/control_panel_41bde0.cpp` | does not compile | C2039 is not a member of 'Unit' |
| `src/ingame/dialogs_493340.cpp` | does not compile | C2039 is not a member of 'Unit' |
| `src/ingame/info_panel.cpp` | does not compile | C2039 is not a member of 'Unit' |
| `src/ingame/info_panel_467440.cpp` | does not compile | C2027 use of undefined type 'Player' |
| `src/ingame/info_panel_46a610.cpp` | does not compile | C2440 '=' : cannot convert from 'struct SpotState *' to 'struct ObjectState *' |
| `src/ingame/markers.cpp` | does not compile | C2011 'struct' type redefinition |
| `src/ingame/selection.cpp` | does not compile | C2440 cannot convert from 'struct UnitDef *' to 'struct UnitType *' |
| `src/ingame/selection_48cf30.cpp` | does not compile | C2039 is not a member of 'Unit' |
| `src/map/features.cpp` | does not compile | C2039 is not a member of 'Unit' |
| `src/map/line_of_sight.cpp` | does not compile | C2039 is not a member of 'Unit' |
| `src/map/line_of_sight_4816a0.cpp` | does not compile | C2039 is not a member of 'Unit' |
| `src/map/line_of_sight_482ac0.cpp` | does not compile | C2679 =: no operator for the whole-value assignment |
| `src/map/radar.cpp` | does not compile | C2039 is not a member of 'Unit' |
| `src/map/terrain.cpp` | does not compile | C2228 left of '.all' must have class/struct/union type |
| `src/map/terrain_47e2d0.cpp` | does not compile | C2440 cannot convert from 'struct ' to 'struct Point' |
| `src/network/net_chat.cpp` | does not compile | C2039 is not a member of 'Unit' |
| `src/network/net_game.cpp` | does not compile | C2679 =: no operator for the whole-value assignment |
| `src/network/net_game_453d40.cpp` | does not compile | C2039 is not a member of 'Unit' |
| `src/orders/order_dispatch.cpp` | does not compile | C2664 cannot convert parameter 1 from 'struct Unit_0043e490 *' to 'struct Unit *' |
| `src/orders/order_list.cpp` | does not compile | C2440 cannot convert from 'struct ' to 'struct Vec3' |
| `src/orders/order_queue_4384a0.cpp` | does not compile | C2039 is not a member of 'Unit' |
| `src/orders/order_targets.cpp` | does not compile | C2039 is not a member of 'Unit' |
| `src/orders/unit_orders.cpp` | does not compile | C2011 'class' type redefinition |
| `src/orders/unit_orders_401e00.cpp` | does not compile | C2027 use of undefined type 'UnitDef' |
| `src/orders/unit_orders_403a20.cpp` | does not compile | C2664 cannot convert parameter 1 from 'struct Player *' to 'unsigned char' |
| `src/orders/unit_orders_403f70.cpp` | does not compile | C2039 is not a member of 'Unit' |
| `src/orders/unit_orders_404ad0.cpp` | does not compile | C2039 is not a member of 'Unit' |
| `src/orders/unit_orders_404db0.cpp` | does not compile | C2039 is not a member of 'Unit' |
| `src/orders/unit_orders_405980.cpp` | does not compile | C2679 =: no operator for the whole-value assignment |
| `src/orders/unit_orders_406300.cpp` | does not compile | C2039 is not a member of 'Unit' |
| `src/orders/unit_orders_406aa0.cpp` | does not compile | C2027 use of undefined type 'UnitDef' |
| `src/orders/unit_orders_406c10.cpp` | takes it | |
| `src/orders/unit_orders_406c40.cpp` | takes it | |
| `src/orders/vtol_orders.cpp` | does not compile | C2039 is not a member of 'Unit' |
| `src/orders/vtol_orders_40fbe0.cpp` | does not compile | C2039 is not a member of 'Unit' |
| `src/orders/vtol_orders_4103e0.cpp` | does not compile | C2039 is not a member of 'Unit' |
| `src/orders/vtol_orders_410850.cpp` | does not compile | C2039 is not a member of 'Unit' |
| `src/orders/vtol_orders_410c70.cpp` | does not compile | C2039 is not a member of 'Unit' |
| `src/orders/vtol_orders_410e70.cpp` | does not compile | C2039 is not a member of 'Unit' |
| `src/orders/vtol_orders_4111b0.cpp` | does not compile | C2039 is not a member of 'Unit' |
| `src/orders/vtol_orders_4118e0.cpp` | does not compile | C2039 is not a member of 'Unit' |
| `src/orders/vtol_orders_411f50.cpp` | does not compile | C2039 is not a member of 'Unit' |
| `src/orders/vtol_orders_412710.cpp` | does not compile | C2039 is not a member of 'Unit' |
| `src/orders/vtol_orders_412d40.cpp` | does not compile | C2039 is not a member of 'Unit' |
| `src/orders/vtol_orders_413470.cpp` | does not compile | C2039 is not a member of 'Unit' |
| `src/orders/vtol_orders_414380.cpp` | does not compile | C2039 is not a member of 'Unit' |
| `src/orders/vtol_orders_414770.cpp` | does not compile | C2039 is not a member of 'Unit' |
| `src/orders/vtol_orders_414a80.cpp` | does not compile | C2039 is not a member of 'Unit' |
| `src/orders/vtol_orders_4152f0.cpp` | does not compile | C2039 is not a member of 'Unit' |
| `src/sound/sound.cpp` | does not compile | C2039 is not a member of 'Unit' |
| `src/units/squads.cpp` | does not compile | C2039 is not a member of 'Unit' |
| `src/units/unit.cpp` | does not compile | C2065 undeclared identifier |
| `src/units/unit_commands.cpp` | does not compile | C2039 is not a member of 'Unit' |
| `src/units/unit_motion.cpp` | does not compile | C2039 is not a member of 'Unit' |
| `src/units/unit_position.cpp` | does not compile | C2039 is not a member of 'Unit' |
| `src/units/unit_save.cpp` | does not compile | C2011 'class' type redefinition |
| `src/units/unit_script.cpp` | does not compile | C2039 is not a member of 'Unit' |
| `src/units/unit_scripts.cpp` | does not compile | C2039 is not a member of 'Unit' |
| `src/units/unit_targets.cpp` | does not compile | C2027 use of undefined type 'UnitDef' |
| `src/units/units_485010.cpp` | does not compile | C2011 'struct' type redefinition |
| `src/units/units_4851c0.cpp` | does not compile | C2039 is not a member of 'Unit' |
| `src/weapons/explosions.cpp` | does not compile | C2039 is not a member of 'Unit' |
| `src/weapons/weapons.cpp` | does not compile | C2011 'struct' type redefinition |
| `src/weapons/weapons_49b090.cpp` | does not compile | C2039 is not a member of 'Unit' |
| `src/weapons/weapons_49b720.cpp` | does not compile | C2039 is not a member of 'UnitWeaponSlot' |
