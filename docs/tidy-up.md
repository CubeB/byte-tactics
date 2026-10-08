# Tidying the source tree

All 3,267 game functions match, and both builds come from source
(`docs/linking.md`). The tree still shows how it was made: one file per
function, named by its address, in one folder; placeholder names everywhere;
and in each file a partial view of every type it touches. This page is the
plan for tidying it before the CI build (#2662), in three phases:

1. **Folders and file names** (this phase): files sorted into folders by
   subsystem and named after what they hold, with tools that no longer care
   where a file lives. No symbol changes.
2. **Real names**: classes, functions, globals and fields renamed where the
   evidence gives a name, one batch at a time, with every check after each.
3. **Shared types**: one definition of each type, then one file per original
   translation unit (#2662's phases 2 to 4).

Every change keeps everything matching: all 3,267 functions
(`uv run tools/progress.py`), the placement build's MD5
(`8e74a1dffa1f5988624c52048f5b20cd`, `uv run tools/place.py`, and from
`data/layout.csv` alone with `--no-orig`), the ordinary link
(`uv run tools/link.py --carve`), `uv run tools/linkcmp.py` clean, and the 28
gap regions (`uv run tools/gapcheck.py`).

## Where things stand

| | |
| --- | --- |
| Files | 3,237 in `src/unsorted/` named `0x<address>.cpp` (24 hold two to four functions), 28 in `src/gap/`, 1 in `src/lib/`, 9 data files in `src/data/` |
| Type names | 2,797 placeholders of the form `X_<address>` (829 of them `Class_<address>`); 9,930 type definitions of 6,232 names, `Unit` defined in 106 files and `Game` in 95 |
| Function and global names | 2,958 `FUN_<address>` and 826 `DAT_<address>` names; `data/symbols.csv` has a real name for a handful of game functions and ten globals |
| Fields and parameters | 11,366 uses of 604 `field_<offset>` names, 10,205 `unknown_<offset>` padding members, `param_<n>` in 739 files |

## Phase 1: folders and file names

### The folders

Each folder is a subsystem; each file in it belongs to a **module**, a guess
at one of Cavedog's translation units. `data/modules.csv` records the guess:
one row per address range, with the folder, the module and the evidence.
`uv run tools/modules.py 0x401070` shows the row for an address.

| Folder | What it holds | Modules (files) |
| --- | --- | --- |
| `ai/` | the computer players and pathfinding | ai_player 115, pathfind 27, ai_profile 6 |
| `orders/` | unit orders, ground and VTOL, the order lists and their targets | order_targets 110, unit_orders 50, order_list 49, vtol_orders 30, order_queue 18, order_dispatch 1, and the order tables (unit_orders.cpp) |
| `units/` | units, unit types, their scripts (COB), movement classes, damage, squads | unit_targets 29, unit_script_calls 28, units 24, unit_scripts 23, movement_class 20, unit_commands 19, cob 19, unit_types 13, unit_position 13, unit_motion 12, squads 4, unit_save 4, and unit_messages.cpp |
| `weapons/` | weapons, projectiles, explosions | weapons 51, explosions 18, weapon_types 3, and ballistics.cpp |
| `map/` | maps, features, terrain, line of sight, radar | los_tables 50, features 36, line_of_sight 29, terrain 26, meteors 12, map_load 11, radar 8, map_list 6, plot_map 6 |
| `game/` | the game state, main loop, WinMain, settings, campaigns, players, victory, saved games, console commands, statistics | victory 98, console_commands 83, players 25, campaign 23, data_files 22, game_state 22, economy 12, share_commands 12, main 10, main_loop 10, settings_campaign 9, savegame 8, game_load 7, statistics 6, cd_check 6, settings 6, side_data 5, and console_commands.cpp |
| `ingame/` | the in-game interface: the control panel and build menus, selection, dialogs, keys | control_panel 49, info_panel 29, selection 29, dialogs 23, camera 8, markers 6, keys 5, build_placement 4, screenshots 1 |
| `frontend/` | the menus outside a game: main menu, campaign and skirmish setup, multiplayer setup, options, end of game | multi 95, options 70, frontend 38, campaign_menu 34, endgame 28, skirmish_menu 19 |
| `gui/` | the GUI library: layouts, gadgets, dialogs, GUI files, list boxes, mouse and keyboard | gui 130, dialogs 49, gui_file 33, mouse 17, file_list 14, keyboard 14, listbox 7 |
| `network/` | DirectPlay (HAPINET), packets, the network game, unit synchronisation, online.dll | net_game 91, unit_sync 74, packets 68, hapinet 44, net_stats 15, net_condenser 7, online 6, net_chat 6 |
| `graphics/` | DirectDraw surfaces, drawing, palettes, GAF, PCX, 3D objects, particles, movies | particles 127, surface 51, gaf 38, draw 37, model_render 35, palette 32, display 30, pcx 21, movies 12, palette_tables 5, font 4, blit 1 |
| `sound/` | sound effects, WAV samples, CD audio | samples 51, cd_audio 34, sound 31, sound_types 6, play_sound 6 |
| `util/` | TDF files, HPI archives, HapiBank, compression, commands, maths, the registry | hpi 58, tdf 56, hapibank 25, sqsh 22, int_map 20, commands 20, math 16, registry 16, operator_new 3 |
| `debug/` | Cavedog's debug library (memory checks, crash reports, profiler) | debug_lib 258, memory_stats 2, debug_helper 1, and debug_dialogs.cpp, perf_counters.cpp |
| `runtime/` | runtime library code compiled with the game's options | basic_string.cpp, basic_string_4c4fa0.cpp |
| `data/` | data that serves the whole game or no code yet | guids.cpp, unused.cpp, vtables.cpp |
| `res/` | the resources (`TotalA.rc`, `tools/resources.py`) | |

The gap regions (`src/gap/` before) go to the folder of their code like any
other file: WinMain is `game/main_49eda0.cpp`, the surface drawing
`graphics/blit_4cbbe0.cpp`. Their kind follows from their addresses
(`tools/sources.py`), not from a folder. zlib (0x4d1c80 to 0x4d7d70) is
runtime library code built from its own source, so it has no files here.
`link/` (the generated `globals.h` and `data.cpp`, and the import
definitions) and `include/` (the generated `ta_types.h` and `ta_protos.h`)
stay where they are: they are build inputs, not game source.

### How the modules were found

The linker kept each translation unit's functions together and in source
order, so a module is a range of addresses. Its edges come from:

- **The strings each function uses.** 820 game functions refer to a string,
  and four name their own source file in an assertion (`frontend.cpp`,
  `multi.cpp`, `endgame.cpp`, `wargame.cpp`), so those modules carry
  Cavedog's names. Most others are named after their strings: `QMove`,
  `VTOL_LANDING`, `HAPINET_sendpacket`, `[HapiBank::OpenBank]`,
  `VictoryCondition_KillAllMobile`.
- **`data/areas.csv`**, the 64 KB windows: each module refines one of them.
- **Class families and units**: `docs/consolidation.md` and
  `uv run tools/unitmap.py` (the pathfinder, the player AI object, the
  victory conditions, the 0x4fd5a8 family).
- **The call graph at each edge**: a function whose strings say nothing goes
  with the module its callers and callees are in. Every edge was checked by
  listing the functions near it whose calls go mostly to the module on the
  other side, and moved where they did. Static initialisers (`_$E`) also
  mark the start of some units.

A module is a hypothesis. Where the evidence is thin the module is broad
(`game/game_state`, `orders/order_targets`, `graphics/particles`). Phase 3,
which merges each module into one file, will find the real edges: a
function that only matches in its neighbour's file, or a type that two
modules share by value, moves the edge. Fix a row of `data/modules.csv`
and run `uv run tools/modules.py --move` to follow it.

### File names

A file holding game or gap code is `src/<folder>/<module>_<address>.cpp`,
where the address (six hex digits, no `0x`) is the function the file is about,
the one its old name gave: `orders/unit_orders_401c20.cpp`,
`ai/pathfind_40e9e0.cpp`. A module's files sort together and in address
order. A data file keeps its subject name (`orders/unit_orders.cpp` holds the
order tables), and a runtime library file its own (`runtime/basic_string.cpp`).
In phase 3, when a module becomes one file, it takes the module's name
(`frontend/frontend.cpp`, as Cavedog had it).

`uv run tools/sources.py 0x401c20` prints the file of any function or global;
`uv run tools/check.py 0x401c20` and the other tools find it the same way.

### One function per file, for now

Grouping a module's functions into one file changes each function's
compilation context: what is declared before it (symbol ids), and what is
defined in the same file (inlining). Measured on every pair of neighbouring
files in one module (3,129 pairs, each pair's two files concatenated as they
are):

| | pairs | functions |
| --- | ---: | ---: |
| do not compile as one file | 1,425 (46%) | |
| compile | 1,704 | 3,424 |
| of those, still match | | 3,318 (96.9%) |
| match their bytes but name something else | | 6 |
| change bytes | | 100 (2.9%) |

The pairs that do not compile fail because each file has its own view of the
types they share: C2371 (a redefinition with another type, 704 pairs), C2011
(a `struct` defined twice, 616), C2556 (overloads that differ only in their
return type, 65). So a module cannot become one file until its types are one
type each, which is phase 3; and even then about 3% of functions change and
need work, as `docs/linking.md` ("The risk in consolidating") and the
files that match only in a narrow symbol id window
(`docs/c2-regalloc.md`, "Files that depend on its exact size") lead one to
expect. Phase 1 therefore keeps one function per file and moves files only.

### Tools that find files by their annotations

`tools/sources.py` finds every source by its annotations, wherever it lives
under `src/`: `// FUNCTION: 0x...` for a function, `// GLOBAL: 0x...` for a
global. A file's kind follows from the addresses it annotates
(`data/functions.csv`'s `kind`), not from its folder:

| Kind | A file with | Was | What it means |
| --- | --- | --- | --- |
| `game` | game functions | `src/unsorted/` | checked by `tools/progress.py` |
| `gap` | a function inside a gap region | `src/gap/` | inline assembly and more `// FLAGS:` allowed; checked by `tools/gapcheck.py` |
| `library` | only runtime library functions | `src/lib/` | placed as library code by `tools/place.py` |
| `data` | `// GLOBAL:` annotations and no functions | `src/data/` | its names join `data/symbols.csv`'s; placed by address |

The builds link the objects in an order that does not depend on folders
(`link_order`): data, gap, library and game files, each by the address in its
name (or its lowest annotated address when the name holds none). It is the
order `sorted(src.rglob("*.cpp"))` gave before, so the builds did not change.
Anything that counts or picks among files (`tools/linkcheck.py`'s types,
`tools/globals.py`, `tools/gametypes.py`, `tools/unitmap.py`) uses the same
order. `tools/check.py`, `progress.py`, `gapcheck.py`, `link.py`, `place.py`,
`carve.py`, `globals.py`, `linkcheck.py`, `gametypes.py`, `ghidratypes.py`,
`unitmap.py`, `vtablecheck.py`, `review.sh` and `regionrun.sh` were changed;
no tool names `src/unsorted`, `src/gap`, `src/lib` or `src/data` any more.

### How the moves are done and checked

`uv run tools/modules.py --move --folder <folder>` runs `git mv` for each file
whose place `data/modules.csv` gives another folder or name, and rewrites the
old paths where the docs and the sources' comments spell them out (comments
change no code). File names must stay unique under `src/`, since
`tools/link.py` and `carve.py` keep objects by file name; `--check` and
`--move` refuse a tree where they are not. The moves go in a few pull requests
by folder. After each:

- `uv run tools/progress.py`: 3,267 of 3,267, and `data/progress.csv` the same
  but for its `file` column;
- `uv run tools/place.py --write-layout`: MD5 `8e74a1dffa1f5988624c52048f5b20cd`,
  and `data/layout.csv` rewritten, changed only in the paths that name the
  objects (`docs/linking.md`, "Building without the original");
- `uv run tools/place.py --no-orig`: the same MD5, from `data/layout.csv`;
- `uv run tools/link.py --carve --map`: the same exe as before the move, but for
  the PE timestamp and checksum (the object order is the same, so the layout
  is too);
- `uv run tools/linkcmp.py`: unchanged;
- `uv run tools/gapcheck.py`: 28 of 29 regions match;
- `uv run tools/globals.py`: `link/data.cpp` unchanged, and `data/globals.csv`
  and `link/globals.h` changed only in the paths they print;
- `uv run tools/gametypes.py --out ...`: the same header as before the move.

What could have moved and does not:

- **Symbol ids** depend on the declarations in a file, not on its path.
- **The frame layout of functions with a C++ try block** follows their
  locals' names, which the moves do not touch.
- **Mangled names** do not contain paths.
- **Assertion strings** that name a source file (`c:\cavedog\wargame\multi.cpp`)
  are string literals in the source, not `__FILE__`; no file uses `__FILE__`.
- **`// FLAGS:` files** (`/Gi`, and `/Od` in two gap files), whose partial
  scores could depend on the length of their path (`docs/agent-guide.md`): all
  20 match before and after.

## Phase 2: real names

### Conventions

Cavedog's own names, where the exe keeps them, win: `PlayerFrameInfo::Initialize`,
`HapiBank::OpenBank`, `HapiBank::LoadAccount`, `CMemoryCache`,
`m_defaultSendPacing`, the `HAPINET_*` and `HAPI_*` functions and the `ONL*`
exports of online.dll. Not every name in the exe is Cavedog's:
`SJE_CdPlayerClass` is the window class of the Windows 95 CD Player, which
the game looks for (FindCdPlayerWindow, 0x4ce1e0) and closes so that it can
open the CD itself. Elsewhere:

| What | Convention | Example |
| --- | --- | --- |
| Classes, structs, enums | PascalCase noun, no prefix | `Pathfinder`, `UnitType`, `VictoryCondition` |
| Member functions | PascalCase verb, as Cavedog's (`Initialize`, `OpenBank`, `RemovePacket`) | `Unit::TakeDamage` |
| Free functions | PascalCase; a C-style API keeps Cavedog's prefix | `LoadFeatures`, `HAPINET_sendpacket` |
| Globals | `g_` and camelCase, as `g_game` and `g_unitOrders` already are | `g_unitTypes` |
| File statics | `s_` and camelCase | `s_mouseQueue` |
| Fields | camelCase; `m_` only where Cavedog's own strings show it | `health`, `m_defaultSendPacing` |
| Unknown fields and padding | `field_<offset>` and `unknown_<offset>` stay until their use is known | `field_9b` |
| Locals and parameters | camelCase, short (`param_<n>` replaced) | `unit`, `count` |
| Enumerators and constants | UPPER_SNAKE, as Cavedog's order names (`VTOL_LANDING`) | `ORDER_STOP` |
| Placeholders still unknown | as now: `Class_<address>`, `FUN_<address>`, `DAT_<address>` | |

A name needs evidence (a string, the data it reads, what calls it, the
original's own text); a guess stays a placeholder. Files keep their names in
phase 2 (`src/map/features_4222e0.cpp` may come to define `LoadFeatures`), and
take their module's name in phase 3.

### How renames are done and checked

A rename changes a function's or a class's mangled name, so every file that
spells it must change at once, with its rows in `data/symbols.csv` (one name per
address) and `data/aliases.csv`. `uv run tools/rename.py OLD NEW [OLD NEW ...]`
does it:

1. It rewrites the identifier in every file under `src/` (whole words, in code
   and comments, never in string literals; inside the decorated symbol of an
   annotation too), in `include/ta_types.h` and `include/ta_protos.h`, in
   `data/symbols.csv` and `data/aliases.csv` (inside decorated names), and in
   `data/modules.csv` and the docs.
   The generated headers take only the renames that merge no two of their
   names: the files that include them match only at their exact symbol counts
   (`docs/c2-regalloc.md`), and a name declared twice counts once.
2. It refuses, pair by pair (`--keep-going` renames the pairs that pass and
   lists the others):
   - a name that a file spelling OLD already uses, or that another OLD in that
     file is renamed to: two things would share the name there, and merging two
     types in one file is phase 3;
   - a name `data/symbols.csv` gives another address;
   - a local of a function whose frame follows its locals' names (below);
   - a gap entry label;
   - a name that is already a type elsewhere, unless `--join` says OLD's views
     are views of that type (`uv run tools/gametypes.py --explain NEW` gives
     the evidence).
3. It runs the checks: `tools/progress.py` (any function that stops matching
   fails the rename), `tools/place.py --write-layout` and `--no-orig` (the
   shipped MD5 both ways), `tools/globals.py`, and with `--full`
   `tools/link.py --carve` and `tools/linkcmp.py`. A rename of one class in
   four files takes about 40 seconds.

Many placeholder classes cannot simply take one name yet: the pathfinder's
methods, say, are matched under several placeholder classes, and a file that
calls two of them defines both, so they can only become one class where the
files' views are merged (phase 3). Phase 2 names what it can without a merge;
`tools/rename.py` says which renames need one.

What a rename can and cannot move:

- **Symbol ids** count declarations in order, so renaming one changes no id.
  Adding or removing a declaration (a shared header, a merged type) does: that is
  phase 3's risk, not phase 2's.
- **Frame layout in functions with a C++ try block, or built with `/Od`**,
  follows a 16-bucket hash of each local's name (`docs/agent-guide.md`,
  `docs/linking.md`). The 9 files with `try` or `__try` (all gap code:
  0x4441a0, 0x444580, 0x45b250, 0x45b490, 0x45b670, 0x497c70, 0x49e680,
  0x49eda0, 0x4d9ab0) and the `/Od` debug helpers (0x4da120, 0x4da2c0) keep
  their locals' names, or take new names from the same buckets.
- **Overloads**: `tools/check.py` compares names without parameter types, and
  several placeholders are overloads of one name (`docs/consolidation.md`);
  a rename must keep each address's signature.
- **Vtables**: a slot names the overridden function; `tools/vtablecheck.py`
  checks every class whose members are renamed.
- **`data/symbols.csv`** is rebuilt by `tools/progress.py` from the matched
  files, so the files are the source of truth and the CSV follows them.

Renames go module by module, in pull requests small enough that a regression
has one cause. The order: the classes with the most files (`Unit`, `Game`,
`Order`, the player AI object, the pathfinder), then each module's own
functions, then globals and fields.

The first classes are done:

- `Unit`: 191 views (#5723);
- `Game`: 796 views, so 907 of the 1,042 files that use `g_game` now agree on its type (#5725);
- `Order`, `PlayerAI` with `g_playerAI`, and `Pathfinder` (#5726).

In each, the names a file shares with another of the type's names wait for
phase 3.

### Renaming a module

One worker takes one module of `data/modules.csv`
(`uv run tools/modules.py --summary` lists them) and names what the module
defines:

- the functions whose files are in it (`FUN_<address>`, and the method part
  of `Class_<address>::FUN_<address>`);
- the placeholder classes whose constructor or most of whose methods are in it;
- the globals only its functions use.

1. **Find the evidence for each name.**
   - `uv run tools/ctx.py <address>` shows a function's strings, callers and
     callees.
   - The module's row in `data/modules.csv`, the file's own comments and
     `docs/consolidation.md` say what a class is.
   - `uv run tools/gametypes.py --explain <Type>` lists the views of a type
     and what joins them.
   - Trust a join through a declaration the files share. Do not trust one
     through a `std::` template: MSVC 5 names every instantiation of a
     function template alike, so those joins reach unrelated element types.
   - Size is no evidence on its own: the `_finddata_t` views are as big as a
     unit.
   - A name with no evidence stays a placeholder.
2. **Write the pairs** in a CSV of `old,new,evidence` rows. Follow the
   conventions above. A method keeps its class: renaming `FUN_00401234`
   renames `Class_X::FUN_00401234`, and a constructor takes its class's
   name. Views of a type that already has a name take that name with
   `--join`.
3. **Dry-run, then rename**: `uv run tools/rename.py --from pairs.csv
   [--join] --keep-going --dry-run`, then again with `--full` instead of
   `--dry-run`. A pair the tool refuses waits for phase 3; say which in the
   pull request. The tool compares words, not scopes. So it also refuses a
   method name that a file already uses for another class's member (two
   `Reset`s, say): a more specific name usually passes.
4. **Open the pull request against main**, with the pairs and their evidence in
   its body. A rename rewrites every file that spells the name, in other
   modules too, so rebase onto main first. If the rebase conflicts, reset to
   main and run `tools/rename.py` again with the same pairs rather than merging
   by hand.

## Phase 3: shared types, then one file per module

This is #2662's phases 2 to 4, now in the tidied tree:

1. **One definition of each type**, module by module: `tools/unitmap.py` and
   `tools/unitgen.py` give a class's views and a merged candidate;
   `tools/gametypes.py` writes `include/ta_types.h` from all the views. Each
   module's own types go in a header beside its files (`orders/orders.h`, say),
   the shared ones in `include/`. Measure each new header with
   `uv run tools/stateprobe.py HEADER --rename` before it goes in, and recheck
   the files that match only at one header size (`docs/c2-regalloc.md`).
2. **One file per module**, once its types are shared: concatenate the
   module's files in address order, recheck every function (about 3% will need
   work, from the measurement above), and keep a function that only matches
   alone in a file of its own with a note why.
3. **One prototype per function** (`tools/protos.py`), and the link's aliases
   retired as the spellings agree.

### One file per class

A named class's methods are being gathered into one file named after the
class, in the folder of its first method (`src/network/frame_queue.cpp`,
`src/util/tdf_record.cpp`); a placeholder class's file takes the usual
`<module>_<address>.cpp` name of its first method. `tools/modules.py --check`
accepts a file whose name holds no address in that folder. Each merge
unifies the files' views of every type into one, in address order, and is
checked like any other change (every function, the MD5 both ways).

What a merge can move, and what to do about it:

- **Inlining.** A method defined in the same file can now be inlined into its
  callers. Where the original inlined it (BitWriter::WriteBits and
  GrowBuffer, MissionConditions::CheckDefeat), the copies kept for that
  disappear. Where it did not (seen in the debug library: RunningStats,
  BlockHistory), the callee's definition sits between
  `#pragma auto_inline(off)` and `(on)` with a comment.
- **Symbol ids.** A function that matches only at its old file's symbol count
  may need its declarations reordered (PerformanceDialog), a standard header
  included only for its symbols, with a comment saying so (`<stdlib.h>` in
  `util/hapi_bank.cpp`, `<io.h>` in `sound/samples.cpp`; `tools/check.py` on
  each function after adding one), or a file of its own with a note at its
  declaration in the class (MissionConditions::CheckVictory,
  `game/victory_490230.cpp`).
- **Views that cannot agree.** TdfFile's methods that delete the root section
  each need a different view of the section's entries, so four of them stay
  in their own files (the note is in `util/tdf_file.cpp`).
- **Compiler-generated functions.** A deleting destructor with a
  `// FUNCTION: <address> ??_G...` annotation is emitted by whatever deletes
  the class in the file; one emitted in another module stays there.
- **Overloads** in one file need the decorated name on their annotation
  (`// FUNCTION: 0x4e1d20 ??0Timer@@QAE@H@Z`).


The CI build (an MD5 check of `tools/place.py` on GitHub Actions) can start
after phase 1, since no tool depends on paths any more; it can also run
`uv run tools/modules.py --check`, which fails when a file is not where
`data/modules.csv` puts it.
