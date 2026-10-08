# Linking

The matching workflow checks one function at a time, and nothing yet checks
that the tree as a whole could become one executable. This page tracks that
second question (#2662): what stands between `src/` and a linked
`TotalA.exe`, how to measure it, and what to do next. It covers phases 0
(measure) and 1 (the global data manifest) of that issue; nothing here changes
how any function compiles.

```sh
uv run tools/linkcheck.py              # the counts and the linkability line
uv run tools/linkcheck.py --verbose    # also every symbol behind each count
uv run tools/linkcheck.py --json build/linkcheck.json   # the counts as JSON
uv run tools/globals.py                # rebuild data/globals.csv, link/globals.h, link/data.cpp
uv run tools/globals.py --check        # also compile link/data.cpp and compare it with the exe
uv run tools/stateprobe.py HEADER --rename   # how many matches a shared header would break
uv run tools/place.py                  # link at the original's addresses: build/place/TotalA.exe
uv run tools/place.py --no-orig        # the same from data/layout.csv, without reading orig/TotalA.exe
uv run tools/place.py --write-layout   # rewrite data/layout.csv after a change moves a piece
uv run tools/resources.py              # compile src/res/ and compare it with the original's .rsrc
uv run tools/link.py --carve           # an ordinary LINK.EXE link that runs: build/link/TotalA.exe
uv run tools/linkcmp.py                # does every reference in it reach what the original's does?
uv run tools/imagecmp.py               # the whole image's placement and data against the original
uv run tools/gapcheck.py [0x...]       # the gap regions' source against the original
uv run tools/vtablecheck.py 0x48e010   # does each vtable slot name the original's function?
uv run tools/playtest.py --exe orig,carve --scenario full   # play both under Wine, PASS or FAIL
```

`linkcheck.py` and `globals.py` compile through `tools/progress.py`'s cache in
`build/progress`, so only files changed since the last `progress.py` run are
compiled, and each runs in a few seconds once the cache is warm. `globals.py`
writes only `data/globals.csv` and the two files under `link/`.
`stateprobe.py` recompiles every file with a matched function into
`build/probe/` (about a minute) and touches nothing else.

## What tools/linkcheck.py measures

It reads every object's COFF symbol table the way LINK would.

**Undefined symbols** are names some object refers to and no object defines.
Each is sorted by what it refers to:

- *game functions*: a `FUN_<address>` name, or a name `data/symbols.csv` maps
  to a game function. Every game function has a definition somewhere (all
  3,267 have been attempted), so these are almost all spelling problems: the
  caller declared the callee with other parameter types or another class name
  than the file that defines it, and the mangled names differ. The report
  splits them into *same name, signature differs only in struct names* (the
  two spellings agree once every struct name is blanked: unifying the types
  fixes these), *same name, other signature* (a real difference: `int` against
  `short`, `char*` against `const char*`, `void` against `int` returns, a
  virtual against a plain method), *defined under another name* (another class,
  or a real name against a placeholder), and *in code with no FPO record*:
  entry points into the 29 `gap` regions of `data/functions.csv`, which have no
  source at all yet. They look hand-written: the 7.6 KB of surface drawing
  code at 0x4cbbe0 and the angle-table lookups at 0x4b70a0 each have several
  entry points and no FPO records.
- *game globals*: a `DAT_<address>` name, or a name `data/symbols.csv` maps to
  `.rdata` or `.data`. No file defines any game data, so every one of these is
  unresolved.
- *Win32 imports*: names the import libraries of the DLLs the original
  imports supply (KERNEL32, USER32, GDI32, ADVAPI32, DDRAW, DSOUND, DPLAY,
  SHELL32, and WINMM, which the exe imports under the patched name
  `WIN32.dll`), plus those with no import library in the toolchain
  (`smackw32.dll`, and DPLAYX's functions, which the game imports by ordinal).
- *CRT/library*: names LIBCMT, LIBCPMT or OLDNAMES supply (the libraries the
  objects' `-defaultlib` directives ask for), C functions declared without
  `extern "C"` (their C++-mangled names exist in no library), and library code
  in the game region that the toolchain's libraries do not have.
- *other*: what is left, mostly compiler helpers (`??_U`, `??_V`) and
  constructors or operators no file defines.

**Duplicate definitions** are names defined in more than one object. A COMDAT
symbol (an inline function, a template instance, a string literal, a vtable,
an exception table) is folded by the linker, so it is no error; the report
still separates folds whose contents differ between objects, because the
linker silently keeps one of them. Everything else (a non-inline function, a
global, or a mix of both kinds) is a real conflict, LNK2005. It also lists
names an object defines that a CRT library defines too, which fail only if
that library member is linked.

**Global types**: for every global the source refers to by address, the types
the files declare for it. The type is read from the mangled name of each data
symbol (`?g_missionOrderTableBegin@@3PAUEntry@@A` is an `Entry*`). An array mangles exactly
like a pointer (`char x[]` and `char* x` are both `PAD`, so they link but mean
different things), so arrays are read from the declarations themselves. The
verdicts are *one type*; *struct names only* (the views agree once struct names
are blanked); *signedness or const*; *array or pointer*; and *shape* (anything
else, such as `int` against `char*`).

**The linkability line** at the end is the number to track over time:

```
linkability: 20.4% of 6,592 referenced names resolve; 5,279 link errors (5,247 unresolved, 32 duplicate); 132 of 788 globals have conflicting types
```

*Referenced names* are the distinct external names any object refers to; one
*resolves* when some object or one of the toolchain's libraries defines it.
*Link errors* counts what LINK would report, once per name: unresolved
externals (LNK2001) and real duplicates (LNK2005).

## Current figures

Measured on 2026-10-01 at bd7d7a18 (3,237 objects, 69.90% matched):

| Undefined symbols | Names |
| --- | ---: |
| Game functions, same name, other signature | 2,027 |
| Game functions, same name, signature differs only in struct names | 1,044 |
| Game functions, defined under another name | 149 |
| Game functions, in code with no FPO record (no source yet) | 75 |
| Game globals (784 addresses) | 1,894 |
| Win32 imports (163 in the toolchain's import libraries) | 183 |
| CRT/library (93 in LIBCMT and LIBCPMT) | 113 |
| Other | 18 |
| **Total** (256 supplied by the libraries, 5,247 unresolved) | **5,503** |

| Duplicate definitions | Names |
| --- | ---: |
| COMDAT folds, identical (652 string literals, 266 inline and template functions, 28 vtables, ...) | 960 |
| COMDAT folds whose contents differ (47 inline functions, 4 vtables, 2 exception tables) | 53 |
| Real conflicts: a game function defined in more than one file | 29 |
| Real conflicts: a global defined in more than one file | 3 |

| Global types (788 globals) | Globals |
| --- | ---: |
| One type | 654 |
| One type, plus `extern "C"` references | 2 |
| Conflicting: struct names only | 21 |
| Conflicting: signedness or const | 26 |
| Conflicting: shape | 85 |

The issue's first estimate (about 1,870 undefined globals, 3,280 undefined
C++ symbols, and about 326 of 1,043 duplicates that were not obviously string
literals or inline code) agrees on the totals: 1,894 is the number of global
*names*, which cover only 784 addresses, and once each COMDAT section's
selection is read from its auxiliary symbol, only 32 duplicates are real
conflicts.

What stands out:

- **Spelling, not missing code.** Of 3,295 undefined game function names, 3,220
  have a definition at the same address under another mangled name. Only 75
  point at code with no source, and those are hand-written regions that need
  either a C rewrite or an assembler.
- **`g_game` is spelt 800 ways.** 1,031 files refer to it, with 800 different
  declared types: `Game*` (97 files), `char*` (95), `void*` (30) and hundreds
  of per-file `Game_<address>*` structs. Its 800 spellings (one of them
  `DAT_00511de8`) are 800 of the 1,894 undefined global names.
- **The duplicates are few and known.** The 29 functions defined in several
  files are copies kept in callers' files so that they inline
  (`PrepVtolClimb` is defined in 13 files; see "Context-dependent functions" in
  `docs/consolidation.md`) and constructors, destructors and class
  `operator delete`s repeated in each file of a class family
  (`SquadTimer`'s constructor in 8 files, `ParticleSystem`'s destructor and
  `operator delete` in 6). The 3 globals defined twice are `g_missionOrderTableVec`,
  `g_movementClasses` and `g_packetManager`.
- **53 folds are silent hazards.** Inline functions and vtables that differ
  between objects link without complaint and the linker keeps any one. Most
  come from files standing in for a helper differently: `vector<Unit*>::_Ucopy`
  exists in two versions, one calling the `/Gz` stand-in `CopyDwordIfNonNull`
  (0x405d90 and four others) and one inlining it (0x406c10 and five others);
  `vector<Elem_00434360>::operator=` has four. The `std` exception classes
  differ in 0x4c3cc0 only, and the vtables of `NanoParticles`,
  `ThrustParticles`, `VictoryDestroyAllUnits` and `DefeatAllUnitsKilled` differ between the
  files that declare them. A linked build needs one version of each, and for a
  byte-identical build it must be the original's.
- **`.bss` starts before the raw end of `.data`.** The section's raw bytes run
  to 0x511a00 (file alignment), but the last non-zero byte is at 0x5119b3 and
  `g_playerBudgetCap` (an `int[10]`) runs past 0x511a00, so uninitialised data
  begins at about 0x5119b8.

## The global data manifest

`tools/globals.py` builds three files from the same objects.

**`data/globals.csv`**: one row per global the source refers to by address,
and one per stretch of `.bss` after a global that nothing names (916 rows:
606 in `.bss`, 224 in `.data`, 86 in `.rdata`).

| Column | Meaning |
| --- | --- |
| `address`, `name` | `name` is `data/symbols.csv`'s when it has a real one, else `DAT_<address>` |
| `section` | `.rdata`, `.data`, or `.bss` (the zero-filled tail of `.data`, estimated as above) |
| `size`, `size_from` | `type`: the size of the largest type any file declares; `gap`: the distance to the next address the original's code or data, `data/symbols.csv` or the source refers to (past the largest offset the source uses; a dword of text, "BAR" and its terminator, is no address); `gap>type`: an uninitialised array that runs on to the next known address, or a global the source reaches past its declared type (`DAT_00528ae8` is declared `char[0x1e8]` in a 0x3e8-byte slot); `parts`: grown to hold the globals inside it; `definition`: the size of the largest definition a tree file has (0x460e20's `Class_00460f60` is 0xb53c bytes, 0x460f60's view 0xb528); `type>gap`: a declared type that runs over the next known address (12 rows; for example `g_messageCountByType`, an `int[45][2]`, overlaps `DAT_00511a60`, a `Pair_00419560[44]` that other files declare 8 bytes further on) |
| `kind` | `data`, `string` (its bytes are a C string), `vtable` (a run of function pointers in `.rdata`), `float`, `template` (a static member of an STL tree), `library` (CRT data), `unreferenced` (`.bss` after a global up to the next known address, that nothing names) |
| `defined` | where it is defined if not in `link/`: a data file (the global itself, or one that holds it), a tree file (a class object with a static initialiser, a template's static), or `in DAT_x+0x10` for a global that is a part of another (a field of a struct or an entry of an array that the code reaches by its address) |
| `type`, `type_files`, `other_files`, `types` | the most common declared type, how many files declare exactly it, how many declare something else, and how many distinct types there are; `T[]` counts as agreeing with `T[N]` |
| `verdict` | tools/linkcheck.py's verdict on the declarations |
| `files` | how many files refer to it |
| `max_offset` | the largest offset from it the source uses (`DAT_x + 8` is 0x8) |
| `ghidra` | the label Ghidra's export uses for the address (`s_` a string, `PTR_` a pointer) |
| `pointers` | aligned dwords in its initial bytes that point into the exe: initial values that need symbols, not numbers, before relinking |
| `init` | its first 64 bytes in the exe, in hex (empty for `.bss`) |

**`link/globals.h`** declares 677 of the 916 globals once, with the type most
files give them, when that type is settled: every view agrees up to struct
names or signedness, three quarters of the files agree on it, or three
quarters agree on its shape and it is the commonest type of that shape (so
`g_game` is declared `Game*`). A pointer to a struct needs only a forward
declaration; a struct held by value, or a global the source reaches past its
type, is declared as a byte array of its size with the type in a comment; a
global only ever declared `extern "C"` is declared `extern "C"`. The globals
left out are listed at the end: those a data file or a tree file defines, the
parts of other globals, vtables, STL tree statics, and globals whose files
disagree (`g_guiContext`, `g_playerAI`, ...). `tools/link.py` defines
those last ones as byte arrays with their whole initial value.

**`link/data.cpp`** defines every global `globals.h` declares, with the
original's initial values: numbers, floats (exact), strings as literals,
pointers to strings as string literals, pointers to other globals in the file
as their address, and zero-initialised `.bss`. `--check` compiles it with the
game's flags and compares each definition with the exe: it compiles with no
warnings and every definition holds the original's bytes (pointer fields are
compared as "some address"). No initial value is left as `TODO`, and none
holds an address as a number: tables of pointers to functions or to other
data are defined in the data files with their types (see below).

Of the 2,456 references in the tree to the globals `data.cpp` defines, 1,292
spell them exactly as it does and would resolve against it today; the rest
declare another type and so have another mangled name.

Neither file is included by anything under `src/`; both builds link
`data.cpp` (see below).

`tools/ctx.py` prints, under `-- globals --`, the type and size
`data/globals.csv` gives each global a function refers to, so a decompiling
agent sees the type most of the tree already uses. Without `globals.csv` it
prints what it did before.

## The data as source: the data files

`link/data.cpp` is generated from what the tree's declarations say, which is
enough for numbers, strings and pointers to strings. Tables of records, of
function pointers and of pointers into other data need their real types, so
they are written by hand in data files, one file per subject, kept in the
folder of their subject (`orders/unit_orders.cpp`; `docs/tidy-up.md`), and
every global there is annotated with its address on the line before. A data
file is any file with `// GLOBAL:` annotations and no functions
(`tools/sources.py`):

```cpp
// GLOBAL: 0x4fc490
extern const UnitOrderType g_unitOrders[23] = {
    {"Stopping", StopOrder, 0, 0, 0x13, 0, "Stop"},
    ...
```

- **Names.** A global gets a real name where the code gives it a meaning
  (`g_unitOrders`, `g_consoleCommands`, `IID_IDirectDraw2`) and its
  placeholder `DAT_<address>` otherwise. `tools/check.py`'s `load_symbols`
  adds the annotated names to `data/symbols.csv`'s, so every tool finds the
  address of `g_unitOrders`, and the tree's `DAT_004fc490` binds to it by
  address.
- **Types** come from how the matched code uses the data: 0x43bad0 calls the
  order record's `+0x4` with a unit, an order and flags; 0x4b7760 walks the
  command records until a null name; 0x4da480 copies a dialog template of
  0x3e0 bytes.
- **Pointers are symbols.** A string is a literal, a function is its name
  (`FUN_<address>`, declared with the signature the record gives it; the
  link binds the name to the definition at that address), and other data is
  its global (`&DAT_004fcfc8`). Nothing holds an address as a number, so the
  ordinary link relocates all of it.
- **Checks.** `tools/place.py` places each annotated global at its address and
  every literal it points at where the original's pointer points, compares
  the bytes, and resolves every pointer: one that leads elsewhere than the
  original's is an error, as it is in compiled code and data.
  `tools/globals.py` and `tools/link.py` leave the addresses
  the data files define, and every global inside one of them, to them.

| File | What it defines |
| --- | --- |
| `unit_orders.cpp` | the unit order types (`Move_Ground`, `VTOL_Patrol`, ...) with their functions and status texts, the four tables 0x43bc90 registers |
| `console_commands.cpp` | the console commands, cheats and debug commands, with their handlers |
| `vtables.cpp` | the vtables the code stores by hand (`this->vtable = &DAT_004fd2f8;`) for classes not yet written as classes |
| `unit_messages.cpp` | what a unit reports (`select`, `underattack`), whose entry 0 is empty: the tree's `g_speechTypes` to `g_speechCategories` are fields of its first two entries |
| `perf_counters.cpp` | the Pentium and Pentium Pro events the profiler can count |
| `debug_dialogs.cpp` | the dialog templates of Cavedog's debug library, as `DLGTEMPLATE` structures |
| `guids.cpp` | the DirectDraw, DirectPlay, lobby and DirectSound ids (the DirectX 5 SDK's, which the toolchain's headers predate), the game's session id and the four providers 0x4ca100 skips |
| `ballistics.cpp` | how far a shot carries at each elevation |
| `unused.cpp` | constants, variables, menu option lists and a few bytes among the constants that nothing reads |

## The placement link

`tools/place.py` builds a `TotalA.exe` that runs: under Wine it plays the
intro and reaches the main menu, exactly as the original does. It links the
same objects as `tools/link.py`, but puts every piece at the address the
original has it, the layout LEGO Island's decomp checks its rebuilt binaries
against (reccmp's placement report, ReproBit's byte-for-byte verify). The
original exe has no relocation table, so code and data that are not rebuilt
yet (the gap regions, data no object defines) only work at their own
addresses; with everything else at its own address too, they
need no relocation at all. And every call reaches the one function at its
address, whatever its caller calls it, so the spellings `tools/link.py`
bridges with aliases do not matter here.

How it places things:

- **Game functions** are placed at their `data/progress.csv` addresses, from
  the objects in `build/progress` that `tools/check.py` compares. Each
  object's COMDAT padding fills the space up to the next function, as LINK's
  would.
- **The import tables** are the `.idata$2` to `.idata$6` sections of the
  import libraries' members (VC5's import libraries are the long format:
  every member an ordinary object). A function's member holds its address
  table entry (`.idata$5`), its lookup table entry (`.idata$4`) and its hint
  and name (`.idata$6`); a DLL's descriptor member holds the import
  descriptor (`.idata$2`, whose section symbols `.idata$4` and `.idata$5` mean
  where the DLL's tables start) and the DLL's name; its null thunk member ends
  the DLL's two tables, and `__NULL_IMPORT_DESCRIPTOR` (`.idata$3`) ends the
  directory. LINK put the address table at the start of `.rdata` and the rest
  at its end, after the exception tables; the descriptors in the order it
  pulled the DLLs in (DDRAW first: the order of the libraries on Cavedog's
  command line, SHELL32 pulled in a later pass), each DLL's two tables in the
  order of the DLLs' names, and the hints and names in the order it pulled the
  members in. Within a DLL's tables the order is LINK's own (neither the pull
  order nor the names'; a test link with LINK 5.10 and the same pulls orders
  them differently again), and the pull order follows Cavedog's objects, so
  each piece goes where the original's import directory has it, as every
  other piece goes where the original has it. The libraries are the
  toolchain's, `WIN32.LIB` for GOG's winmm (see the ordinary link below), and
  LIB.EXE's from `link/*.def`: `smackw32.def` and `dplayx.def`, and
  `ddraw.def` and `dsound.def`, which stand for the DirectX 5 SDK's
  `ddraw.lib` and `dsound.lib`. Those give `DirectDrawCreate` and
  `DirectSoundCreate` the hints 5 and 3 the original has, where the
  toolchain's DirectX 3 libraries have 6 and 0 (a hint is the import's place
  among the DLL's sorted export names, and LIB numbers them so). A `.def`
  spells each export as its `__stdcall` symbol does (`DirectDrawCreate@12`);
  `place.py` gives LIB the plain names and an object defining the decorated
  symbols, so the library imports `DirectDrawCreate` by name and defines
  `__imp__DirectDrawCreate@12` for the callers.
- **The linker's import thunks** (`jmp [slot]`, for calls to imports declared
  without `__declspec(dllimport)`) are the `.text` of the same members, each
  placed where the original has its stub: 70 in the gap row 0x49f710 and 184
  in the unnamed library row 0x4faff0, after the runtime library. Each jumps
  through its own member's `.idata$5`.
- **Gap regions with matching source** (see below) are placed
  function by function from the objects `tools/gapcheck.py` checks, and
  counted as gap code. The padding between their functions is the
  original's.
- **Relocations** are resolved by name: a placed function, a placeholder
  (`FUN_`/`DAT_<address>`), a `data/symbols.csv` name or one of its
  `data/aliases.csv` copies, a runtime library function `data/functions.csv`
  names, or an import: its slot in the original's import address table, or
  the linker's `jmp [slot]` stub for a direct call. The DLLs imported by
  ordinal are named through `link/smackw32.def` and `link/dplayx.def`, read
  from the DLLs' own export tables.
- **Data the compiler emits with the code** (string literals, floating-point
  constants, jump tables, exception tables, vtables, file and function
  statics) is placed one symbol at a time where the original's code refers to
  it, the way a map file would say. Its contents come from the object, and
  a vtable slot that leads elsewhere than the original's is an error like any
  other relocation (`tools/vtablecheck.py` names the function the slot's own
  file should define).
- **The globals**: those a tree file defines (a class object, a template's
  static) from its largest definition, then the data files', then
  `link/data.cpp`'s and the byte arrays `tools/link.py` adds for the globals
  `tools/globals.py` leaves out, each at its address.
- **The initialiser tables** at the start of `.data` (`__xc_a` to `__xc_z`
  and the others) are built by LINK from every object's `.CRT$X*` sections,
  so each section goes where the original's table holds its function, and
  the original's initialisers the tree spells as ordinary functions get the
  entry `tools/link.py` writes for them.
- **Padding**: zeros after one piece of data up to the next, at that piece's
  alignment; fewer than 8 (or 16) zeros before a piece of the game's data at
  an 8-byte (or 16-byte) boundary, where one of the original's objects began
  (they held many functions each, and most of the DirectX setup code's
  strings start on 8-byte boundaries, which the tree's one-function objects
  cannot show); fewer than 16 zeros between two pieces of the runtime
  library's data where the second begins on a 16-byte boundary (assembler
  members such as `strchr.obj` and `memmove.obj` have empty `.data` sections
  aligned to 16 bytes, and LINK aligned for each one it laid out: 48 bytes in
  five places); the zeros before a communal variable up to its alignment
  (its size, at most 32 bytes: `___pioinfo` follows `__crtheap` after 24
  zeros); and the even padding of the import name table and the DLL names.
  Past `.data`'s raw data, in its `.bss`, the bytes no object defines are not
  in the file at all: the loader zeroes them. They are counted as
  uninitialised: 3 bytes of alignment after 0x4df160's static, and 5 that
  no alignment explains, so evidently variables of Cavedog's objects that the
  tree's objects leave out (0x51fc98, the start of 0x4b7ad0's 8-byte aligned
  `.bss` before its vector at 0x51fc99, and 0x5292c0 to 0x5292c3).
- **Thread-local data**: LINK builds `.tls` from the linked objects' `.tls`
  sections in the order of their names: the runtime's `__tls_start`
  (`tlssup.obj`'s `.tls`), the game's thread-local variables (`.tls$`), and
  `__tls_end` (`.tls$ZZZ`), each at its alignment. The only `.tls$` the
  original has is Cavedog's object at 0x4d8d70 (gap region 0x4d8d70),
  whose three variables 0x4d8df0 and 0x4d8e20 read too; they declare them
  rather than defining struct views of their own, so the ordinary link has
  one copy as well. A thread-local variable's offset (a `SECREL` relocation)
  is resolved by name like any other reference and checked against the
  original's. `tlssup.obj`'s `.rdata`, `__tls_used`, is the TLS directory,
  and the empty table of TLS callbacks it points at (`.CRT$XLA`, then
  `.CRT$XLZ`, which nothing refers to and so goes right after the last entry
  placed in its table) sits among the initialiser tables.
- **The runtime library** comes from the members of the libraries the
  original links statically: the VC5 SP3 `LIBCMT.LIB` and `LIBCPMT.LIB`, and
  zlib 1.0.4 as `tools/setup_toolchain.sh` builds it with Cavedog's options. A
  member goes where `data/functions.csv` has its function, provided the
  original holds its bytes there and refers to the same things: its calls
  reach functions of the same names, its imports the same slots, and its own
  data (strings, tables, exception and type information, initial values) the
  same contents. Bytes alone cannot tell many of them apart: the locking
  wrappers `_read`, `_write` and `_lseek`, `_Xlen` and `_Xran`, or zlib's
  `get_crc_table` and `zlibVersion` differ only in what they refer to.
  `tools/functions.py` makes the same check when it names the library
  functions, following pointers into the member's own data (`length_error`'s
  throw information differs from `failure`'s only in the type names its
  tables lead to), and where references cannot tell a pair apart, by what the
  chosen callers call the address (`_strdup` and `_mbsdup` are the same code;
  `copy_environ` calls `_strdup`). So `data/functions.csv` gives each the half
  of such a pair that is really there, and the report lists any member placed
  under another name. A call from a library member may also reach a game
  function of that name: the `exception` constructors call Cavedog's
  `operator new`. Functions with no FPO record of their own that sit inside
  another's row (`__allshr` and `__allshl` after `__ftol`, `_acos` after
  `_strcspn`, 87tran.obj's routines) are found by their first bytes (up to
  the first relocation) at each free 16-byte boundary. A static function a
  row names (string.obj's initialiser `_$E50`) comes from the placed member
  nearest it; other static functions and the members' data follow where the
  placed code refers to them, and communal (`.bss`) data where the original
  has it. A pointer before a section's first symbol goes with it: each of
  the library's vtables (compiled with RTTI) is preceded by its complete
  object locator's address, which leads to the RTTI descriptors. The rest of
  a data section placed in part goes beside it (zlib's copyright strings),
  and a placed member's other initialised data where the original holds its
  bytes with its pointers leading the same way (throw.obj's and frame.obj's
  pointers to the unhandled exception filter). The three-byte `return 0` functions (`_matherr`, `__init_collate`,
  setlocale's `__init_dummy`) have no name in `data/functions.csv`, so they go
  where the placed code and tables refer to them, not by size. Alignment
  padding (nops, or int3s after assembler code) is counted as padding.
- **`basic_string` members compiled from source** (a `library` file,
  `tools/sources.py`): six members of
  `std::basic_string<char>` that no `LIBCPMT.LIB` member matches, because the
  original's copies were compiled with the game's options. Five sit among
  Cavedog's functions (the copies one of Cavedog's objects instantiated, at
  0x4c4ac0 to 0x4c50a0, with `_Copy` between them as the gap region 0x4c4fa0)
  and `assign` (0x4e3c00) among the original's `string.obj`. The file
  explicitly instantiates them from the compiler's own `<xstring>`, each
  annotated and checked by `tools/check.py`; `tools/place.py` places them as
  library code, before the library members, and `tools/link.py` links the
  object like any other (`tools/progress.py` leaves `library` files out:
  these are not `game` rows).
- **The resources** are `src/res/TotalA.rc` (the icon, the cursor and the
  version resource), which `tools/resources.py --extract` wrote out of the
  original once, and the icon and cursor it names. Those two are Cavedog's
  art, which the repository does not hold: `tools/resources.py` takes them
  from a directory given with `--art` or `BT_ART_DIR` (CI supplies them so),
  or else extracts them from the player's own `orig/TotalA.exe` into
  `build/res/art/`, and stops with a message saying so when it has neither.
  It compiles the script with the toolchain's `RC.EXE` (5.00.1472, which
  `tools/setup_toolchain.sh` takes from the CD's `SHAREDIDE/BIN`), finding
  the art through RC's include path, and converts the `.res` with SP3's
  `CVTRES.EXE` (5.00.1668, the build the exe's Rich header names), with
  `/MACHINE:IX86 /READONLY` as LINK runs it on a `.res` file. CVTRES lays out
  the whole resource directory itself (`.rsrc$01`), with the data after it in
  the script's order (`.rsrc$02`), so `place.py` puts the two sections at the
  start of `.rsrc` and resolves their relocations like any other object's.
  The script is plain ASCII: the copyright sign is an octal escape (`\251`)
  read through `#pragma code_page(1252)`. Each version string ends in an
  explicit `\0`, as Developer Studio wrote them, which is what makes the
  lengths the original's. `tools/link.py` links the same object.
- **The headers and the debug data** are generated, as LINK wrote them,
  from the placed sections and the link's settings (`tools/pe.py`, below).
- **What has no source** would be copied from the original and counted as
  copied: the gap regions without matching source and data no object
  defines. Nothing is copied any more: every byte of the file is compiled,
  built by the toolchain's tools from source in the repository, or generated
  from the settings in `link/link.toml`.

Every relocation is checked against the address the original's bytes give at
that spot, and the finished image is compared with the original byte for
byte, which is the placement and data compare #4869 asks for. The report
counts where each section's bytes came from. On 2026-10-05:

| Section | Bytes | Built | Copied |
| --- | ---: | --- | --- |
| `.text` | 1,026,560 | 850,853 game code, 24,762 gap code, 120,848 runtime library and import thunks, 30,097 padding | none |
| `.rdata` | 18,432 | 6,564 import tables, 3,234 compiled data, 4,172 library data (with the TLS directory), 2,965 the data files, 324 `link/` globals, 1,089 padding, 84 debug directory | none |
| `.data` | 173,660 | 83,515 compiled data (with the tree's own globals), 29,549 library data, 7,694 the data files, 48,458 `link/` globals, 4,436 padding, 8 uninitialised | none |
| `.tls` | 512 | 8 `__tls_start` and `__tls_end`, 9 thread-local variables, 495 padding | none |
| `.rsrc` | 3,072 | 2,640 resources, 432 padding | none |
| headers and debug data | 61,862 | 1,024 headers, 84 debug directory (in `.rdata`), 60,838 debug records after the sections | none |

Before the data was defined in the data files (#2662), 34,662 bytes of `.data`
and 1,546 of `.rdata` were copied: tables whose pointers `link/data.cpp` held
as numbers left their strings undefined, `.bss` buffers were cut short at
false boundaries, and the padding between pieces counted as data. The last 56
bytes copied were zeros nothing refers to: 48 among the runtime library's
data, which are LINK's alignment for the assembler members' empty `.data`
sections, and 8 between the game's statics in `.bss`, which the file does not
hold. The 8 bytes of the initialiser tables that were copied too are the
empty table of TLS callbacks (`.CRT$XLA` and `.CRT$XLZ`), built with `.tls`.

Before the runtime library's last functions were built (#2662), 8,267 bytes
of `.text` were copied as library code: 11 functions (3,163 bytes, among
them the second run of import thunks), two runs of code with no FPO record
(`_acos` and 87tran.obj, 722 bytes) and the alignment padding between
library functions. The last gap region, 0x49a120, has matched its source
since (1,840 bytes with its padding), so no code is copied.

Of the 39,414 relocations in placed pieces, every one agrees with the
original (136 of them reach the second copy of a function `data/aliases.csv`
lists, such as the two `std::_Lockit`), the vtables' too. 94 vtable entries
used to disagree: the slot named a base class's method where the original
has the derived class's override, which its own file matched under a
placeholder class. Each override is now named after the virtual it
overrides, with the base's parameter types and declared virtual, which binds
the slot with no function's bytes changed (`Class_0043a1f0`, the
`ParticleSystem`, `Class_0044ef20` and condition families,
`UnitScript`); `tools/vtablecheck.py` checks a file's slots so. The compiled image differs from the
original only under the two rows of `data/exe_patches.csv`, where it has the
compiler's bytes rather than GOG's no-CD music patch, and after the link
`tools/exepatch.py` writes the patch over them, so `build/place/TotalA.exe` is
`orig/TotalA.exe` byte for byte (`--no-exe-patches` leaves the compiler's
bytes).
`build/place/TotalA.map` lists every placed piece and the object it came
from.

### The headers

`tools/pe.py` writes what LINK adds around the sections, from the placed
image and the settings in `link/link.toml`: the options Cavedog's build gave
LINK 5.10 and what LINK recorded of its run.

| Setting | Value | Where it shows |
| --- | --- | --- |
| `timestamp` | 1998-07-30 19:22:29 BST (0x35c0b9e5) | the file header, the debug directory, the CodeView record's signature |
| `subsystem`, `entry` | `windows`, `_WinMainCRTStartup` | the optional header (the entry point is where the runtime's function is placed) |
| `out` | `.\Release\TotalA.exe` | the MISC debug record |
| `pdb`, `pdb_age` | `C:\cavedog\wargame\Release\TotalA.pdb`, 0 | the CodeView (NB10) debug record |
| `unmarked_objects` | 622 | the Rich header: the objects without a `@comp.id` (Cavedog's, the runtime library's, zlib's and every import library member) |
| LINK 5.10's defaults | base 0x400000, sections at 0x1000, file at 0x200, OS and subsystem 4.0, stack and heap 1 MB reserved, 4 KB committed | the optional header |

Everything else follows from the image:

- **The MS-DOS stub** is LINK's default, which `LINK.EXE` holds as a
  template (an `MZ` header with `e_lfanew` 0); `pe.py` takes it from the
  toolchain's `LINK.EXE`.
- **The Rich header** has one entry per `@comp.id` among the linked objects
  with how many carry it: the unmarked objects (the setting), then the
  resources' object (CVTRES 5.00.1668, whose `@comp.id` `pe.py` reads from
  it), in the order LINK met them. The key LINK 5.10 XORs it with is 0x80
  (the stub's size) plus each entry's `@comp.id` rotated left by its count;
  unlike later linkers' it leaves out the stub's own bytes. `e_lfanew`
  follows the key at the next 16-byte boundary after 8 more bytes. Test
  links with the toolchain's LINK 5.10 confirm the key, the order (the
  entries come as LINK met their first objects) and, for objects marked like
  the original's, `e_lfanew`; links with import libraries that LIB 5.10
  built leave more room. LIB 5.10 marks the import libraries it builds from
  `link/*.def`, which the SDKs' own libraries were not, so `pe.py` takes the
  count from the setting rather than from the objects.
- **The sections** are where the placed pieces are: each runs from a
  section boundary (0x1000) to its last byte before a gap that reaches the
  next boundary, is named for its first piece (`.idata$5` opens `.rdata`,
  `.CRT$XIA` opens `.data`), and has the characteristics of its input
  sections of its own name. Its raw data runs to its last piece of
  initialised data (`.data`'s `.bss` is not in the file), at the file
  alignment. The headers' sizes and the image's follow from them.
- **The data directories** are where the pieces are: the import descriptors
  (with the null one), the resources, the debug directory, `__tls_used` and
  the import address table.
- **The debug directory** goes into `.rdata` right after the import address
  table, as LINK put it, and lists three records that follow the last
  section in the file: MISC (the exe's name as `/OUT` gave it, in a
  `MAX_PATH` buffer), FPO and CodeView (NB10).
- **The FPO records** (3,782 of them, 60,512 bytes) are the placed
  functions' own: each object's `.debug$F` holds one per function the
  compiler made, relocated to the function's address, and LINK sorts them by
  address. A scalar deleting destructor's record names the function by an
  undefined symbol of its own name, which means the definition in the same
  object. Four of the tree's functions declared no parameter where the
  original's record counts one dword; they now take the argument their
  callers pass (#5712). The gap regions are the code between FPO records,
  and their objects add none.

To run it, copy it into a copy of the game's directory (the Steam or GOG
install, with `smackw32.dll` and `win32.dll`) and start it under Wine, for
example `wine explorer /desktop=TA,800x600 TotalA.exe`. Like the original, it
shows a DirectX version warning over the main menu in a fresh Wine prefix,
and it starts and plays a skirmish game.

### Building without the original

`uv run tools/place.py --no-orig` builds the same exe without opening
`orig/TotalA.exe` at all, and so does `tools/place.py` when that file is
missing (as in CI). Everything comes from the repository and the toolchain:

- **The layout** comes from `data/layout.csv`, which the build with the
  original writes: `place.py` writes `build/place/layout.csv` on every run
  and says when it differs from `data/layout.csv`, and `--write-layout`
  updates the latter. Rerun it after any change that moves a piece (a
  function that grows, a file that moves, a new global in a data file).
- **The art** (the icon and cursor) comes from `--art` or `BT_ART_DIR`, the
  one input that is not in the repository (see the resources, above).
- **The check**: the exe's SHA-256 must be `orig/TotalA.exe.sha256`'s and its
  MD5 `link/link.toml`'s (8e74a1dffa1f5988624c52048f5b20cd). With the
  original present, `place.py` also compares the file byte for byte.

GitHub Actions does exactly this on every push to main and every pull request
that touches the code, the data, the link inputs or the tools
(`.github/workflows/build.yml`): it installs Wine, 7-Zip and cabextract, runs
`BT_NO_ORIG=1 BT_NO_GHIDRA=1 tools/setup_toolchain.sh` (the compiler is
cached between runs), unpacks the icon and cursor from the `BT_ART_TGZ`
repository secret (a base64 tar.gz of `TotalA.ico` and `TotalA.cur`, as
`tools/resources.py` extracts them into `build/res/art/`), checks
`tools/modules.py --check`, runs `uv run tools/place.py --no-orig --strict`
and fails unless the exe's MD5 is the shipped one. The run's summary shows
the MD5 and SHA-256; the exe itself is never uploaded.

`data/layout.csv` records what the build with the original decides from the
original's bytes, as rows of these kinds:

| Kind | What it says | Rows |
| --- | --- | ---: |
| `piece` | a slice of an object's section (`object`, `section`, `offset`, `size`) goes at `address`; every piece but the game functions, which `data/progress.csv` places, in the order the pieces were placed | 5,962 |
| `fill` | `size` bytes of `offset` (0x00, 0x90 or 0xcc) that no piece wrote: padding, communal variables, `.bss` | 5,908 |
| `alias` | a gap region's entry label: the public symbol `gapcheck.py` adds to its object | 32 |
| `common` | where a communal variable is | 19 |
| `same` | a slice of an object that is not placed itself but stands for the piece at `address`: the original had one copy of a literal, a constant or a file static that the tree's objects each keep | 572 |
| `reloc` | the target of a reference that no name leads to: the 136 calls to the other copy of `std::_Lockit`, the 30 names whose address only the original's bytes give (the vector deleting destructors, `SmackSoundEnable`), and three more | 169 |

An object is named by its source file (`src/map/features.cpp`,
`src/weapons/weapons_49a120.cpp`), by a library and the member's place in it
(`LIBCMT.LIB#123`, `KERNEL32.LIB#40`, the import libraries too), as
`zlib/deflate.obj`, `link/data.obj`, `init/<symbol>` (the initialiser table
entries `place.py` writes) and `res`, with `@<n>` for a second copy of a
member. Everything else follows from the placed pieces as LINK's rules
have it: the game functions and their padding, the import address table's
slots and the stubs that jump through them, where each import descriptor's
tables start, `.tls` and its directory, the sections, the headers and the
debug data. Every other reference is resolved by name, as in the build with
the original; a reference the layout cannot account for stops the build.

### Why not LINK's own layout

LINK 5.10 can put functions in a given order: with `/ORDER` listing every
game function in address order, the first 164 land at their original
addresses, with the same 16-byte alignment and `nop` padding. Three things
stop that from reaching the whole image:

- LINK keeps the first copy of a COMDAT it sees. The tree keeps copies of
  callees in callers' files so they inline (`docs/consolidation.md`), and
  their bodies can differ: at 0x40d020 LINK kept `vector<short>::insert` from
  `0x409160.obj`, 16 bytes longer than the annotated one, and everything after
  moved.
- Data cannot be ordered the same way. The original's `.data` holds each of
  Cavedog's objects' literals, constants and globals together; the tree's
  objects hold one function's worth each, so LINK would interleave them in a
  different order, and the code and data that are copied from the original
  would point at the wrong things.
- The gap regions have no objects to order, and the runtime library's
  members would land wherever LINK pulls them in.

Each of these could be worked around (dropping unwanted COMDAT copies from
the objects before linking, blob objects for the gaps, a single data object
with every global at its offset), but each workaround is a placement decision
made outside LINK, so `place.py` makes all of them itself and checks each one.
The ordinary link below is the way to a relocatable build, and `place.py`'s
map of what each address holds is what makes it run.

## The ordinary link: tools/link.py --carve

`uv run tools/link.py --carve` links the tree with LINK.EXE the ordinary way,
every function where LINK puts it, and the image runs: under Wine it plays
the intro, reaches the main menu, opens the single-player and skirmish
screens, and starts and plays a skirmish game. Since a function may now grow
or move, this is the build to change the game in.

What has no source comes from `tools/carve.py`, which takes it out of the
original as relocatable objects, the way LEGO Island's decomp carves its
Smacker library out of the retail DLL (its `tools/gen_smacker_lib.py`). That
DLL has base relocations to say where the addresses are; `TotalA.exe` has
none, so `carve.py` finds them itself:

- **`gaps.obj`** holds the gap regions that have no matching source yet and
  the exception handler code of their functions, one section each (a region
  with matching source is linked from its own object instead, named by its
  symbols). Every region matches now, so it is empty. Its relocations come from disassembly:
  every `rel32` branch that leaves its region, and every 32-bit immediate or
  displacement that holds an address in the image. Each entry point the tree
  calls gets its `FUN_<address>` name, and a carved WinMain its
  `_WinMain@16` (WinMain's region, 0x49eda0, now has source, which defines
  it).
- **`origdata.obj`** holds the runs of the original's `.rdata` and `.data`
  that no object defines, the bytes `place.py` copies, but for runs of zeros
  nothing refers to, and whatever those runs point at that no symbol names
  (a closure). It is empty now, down from the whole 192 KB of both sections:
  `place.py` copies no data, and LINK lays the library members' data out
  itself. Each run is a section of its own named for its
  address (see the data's order, below). Its relocations come from
  `place.py`'s layout: every pointer field of a placed piece of data; and
  where no object defines the data, every dword that holds the exact address
  of a function, a global, a placed piece or a string (unaligned ones too),
  every element of a global `data/globals.csv` types as a pointer, and every
  address in the compiler's exception tables.

The game's data itself comes from source: `link/data.cpp`, the globals
`tools/link.py` adds as byte arrays, the data files, and the globals and statics
of the tree's own objects. What makes that link run:

- **One definition per global.** The layout names every address of data by
  what it placed there: a global by its symbol, a literal or vtable by its
  external name, a static by a public name its object gets
  (`__static_<address>`), library data by its name. A name no object defines
  binds to that symbol; a name that points inside a global (a field the code
  reaches by its address, `DAT_0051e2f4` in `g_packetManager`; an entry,
  `DAT_005086e0` in `g_unitMessages`) has its references pointed at the
  global with the offset added (299 references), and the name itself then
  means the global, so it needs no stub. Every other definition of a placed
  global or vtable becomes a static nothing uses, its file's references
  going to the placed one: another file's smaller view of the same class
  (whose vtable LINK might otherwise keep), or another
  spelling of the same template static, like the `_Nil` node of a
  `std::map<int, int>` two files instantiate with different views of the
  pair. Linked as they were, that map's tree was set up through one `_Nil`
  and walked through the other, and starting a skirmish crashed.
- **The original's order.** Code treats neighbouring globals as blocks:
  0x451fd0 clears 44 separately named ints with one `memset` of 176 bytes,
  0x4da480 copies a dialog template from a `char`. The compiler lays a
  file's `.bss` out in its own hash order, so linked as they come the
  `memset` cleared the sound driver's pointer. `tools/coffsplit.py` puts each
  placed piece of data in a section of its own named `.data$<address>`
  (`.rdata$`, `.bss$`), as `origdata.obj` names its runs, and LINK sorts a
  grouped section's parts by name, so the game's data keeps the original's
  layout: 0x51e68c is 0xbbb0 bytes after 0x512adc in both. Only the library's
  data, from `LIBCMT.LIB` and the rest, comes in LINK's order.

The same layout says what every reference in the tree's own objects means,
and `link.py` applies that to patched copies of the objects under
`build/link/objs/`:

- A name no object defines is aliased to the symbol at the address the
  original's code holds wherever the name is used, in a placed function or in
  a byte-identical copy of one (4,952 names), rather than guessed from its
  spelling.
- A placed function's references to file statics, and to other functions of
  its own file, point where the original's code points (1,176 references).
  Many files keep a global as a file-scope `static` because that makes their
  function match (0x4223e0.cpp's `static FeatureList* DAT_00511fb4`), and
  copies of callees kept so that they inline (`docs/consolidation.md`) would
  otherwise be called instead of the real function.
- The 22 placed functions the compiler made static, the `_$E<n>` initialisers
  of global objects and the destructors they register with `atexit`, get a
  public name each, `__static_<address>`, so that those references can reach
  them. Six initialisers register a destructor that another file defines
  (0x4b2290 registers 0x4b2340); linked as they are, each would register its
  own object's copy, and 0x4b2290's calls a stub, so the original's map at
  0x51fbc0 was never freed at exit.
- Every other file's definition of an annotated game function is made
  static, so the name binds to the annotated one (590 copies).
- Of the tree's own objects, only the original's 17 C++ static initialisers
  run, in its order (see `fix_initialisers` in `link.py`): files that define
  global objects only so that a function matches would otherwise construct
  them with the wrong constructors at start-up. The gap regions' objects and
  the library files add none either (those that include `<string>` carry an
  initialiser for a `locale::id` guard).
- zlib comes from the objects `tools/setup_toolchain.sh` builds, and the
  runtime library from `LIBCMT.LIB` and `LIBCPMT.LIB`, as LINK picks them.
- The resources (the icon, the cursor and the version) come from `src/res/`,
  compiled as for the placement link.
- GOG's no-CD music fix has two parts, and the link makes both. The exe
  imports WINMM's functions from `WIN32.dll`, GOG's winmm that plays the CD
  tracks from `music/*.mp3`, so the link takes them from `WIN32.LIB`, a copy
  of `WINMM.LIB` with the DLL renamed: against `WINMM.LIB` the game opens
  Wine's own cdaudio device, finds no CD and disables the music options. And
  the code patch (`data/exe_patches.csv`) is applied after the link, as in
  the placement build, but moved: the `jmp` goes where 0x4cda00 now is, the
  code it jumps to goes past the end of the linked `.text` (whose virtual
  size grows to cover it), and their branches are retargeted
  (`tools/exepatch.py`; `--no-exe-patches` leaves it out). The map is always
  written in this mode, since the patch needs it.

Ten names are still stubbed (constructors, destructors and operators that
only unplaced copies or the dropped initialisers call), and three references
reach addresses no symbol names: the absolute offsets `__except_list` and
`__tls_array`, and the null `DAT_00000000`. A name whose address the layout
gives to communal data (0x52a4e4, the guard of `ctype<unsigned short>::id`,
which string.obj's `_$E50` and Cavedog's 0x463ba0 both test) has its
references pointed at the communal symbol rather than aliased to it: LINK
5.10 stops with an internal error on a weak external whose default is
communal.

`uv run tools/linkcmp.py` checks the result, the placement compare #4869 asks
for in the form an ordinary link needs: comparing addresses says little once
every function has moved, so it compares where each reference leads. For
every placed game function it reads each relocated field of
`build/link/TotalA.exe`, maps the address back to the original through the
map file (`link.py --carve --map`), and compares it with what the original's
code holds in that field. A reference past the end of a global (an array's
end, `&g_playerAI[10]`) agrees when its global does, and a function's own
statics where the layout put them. A constructor's store of the vtable its
own object defines is compared too, since another file's copy may be the
one LINK keeps. It then compares the data: every named
piece of data in the link (a global, a literal, a vtable, a run of
`origdata.obj`) against the original's bytes at its address, pointer fields
by where they lead, and it flags an address of the original's code held as
a number. On 2026-10-05 all 26,627 references it can place, in all 3,308
placed functions (the gap regions' among them), agree, apart from 134 calls
that reach the other copy of `std::_Lockit`; of 5,080 pieces of data, 20
differ, all of them a pointer to a string the link folded into an identical
literal of another object, or the library's data (its vtables and throw
information lead to `LIBCPMT.LIB`'s functions where the original's lead to
the copies Cavedog's objects instantiated). It exits non-zero on any
difference, so a change that rebinds a name shows up before the game is run.

## The whole image: tools/imagecmp.py

`linkcmp.py` checks references one field at a time. `tools/imagecmp.py` asks
the two questions that only make sense for the whole image, the placement and
data compares of #4869 (ideas from LEGO Island's reccmp `roadmap` and
`datacmp`):

```sh
uv run tools/imagecmp.py              # the placement and data reports
uv run tools/imagecmp.py --verbose    # every boundary, unit and difference
```

It reads `build/link/TotalA.exe` and its map; `--exe` names another build, and
`build/place/TotalA.exe` works too (its map says every piece already sits at
the original's address). In an ordinary link every function sits where LINK
puts it, so the placement report compares the order, not the addresses:

- **Runs.** The game and gap functions are sorted by their linked address; a
  run is a stretch whose displacement (linked - original) is constant, an
  island of the original's layout that LINK kept together. A change of
  displacement is an inferred object-file boundary, where a whole object sits
  somewhere else in the link order; a boundary whose displacement jumps
  backwards is an object placed out of the original's order.
- **Units.** Each of `tools/unitmap.py`'s units is checked: one whose members
  are not in the original's address order in the link is listed, with the
  members around its first inversion.
- **Areas.** Each inferred boundary is grouped by the 64 KB window of
  `data/areas.csv` it falls in, so the numbers say which of those area guesses
  the link order disagrees with.

The data report compares every global `data/globals.csv` lists in `.rdata` or
`.data` with the original's initialised bytes. A pointer field is compared by
where it leads, not by value, so a pointer to a global or a literal is checked
as the symbol it names, and a pointer the link folded into an identical string
elsewhere agrees. A string or float that differs is printed in readable form.
This is `tools/globals.py --check` extended to the linked image: `globals.py`
only covers `link/data.cpp`, while this reaches the globals game files define
and the data static initialisers build.

The placement report is a guide for the link order and does not fail the
command. The data report does: the tool exits non-zero when any global differs
(`--strict` also fails on a placement difference), so the phase 2+ steps can
gate on it.

On 2026-10-05 the ordinary link takes its data from source, so its data report
is clean: 312 of the 312 globals in `.rdata`/`.data` hold the original's bytes
(451 pointer fields agree, 3 reach an identical string elsewhere), and it
exits 0. The placement report of the ordinary link, whose order LINK chooses,
shows 3,296 game and gap functions in 383 runs (382 inferred boundaries, 92 of
them backwards) and 6 of 116 units with a member out of order
(`IURect_0046e160`, `PAUUnit`, `SquadTimer`, `SquadScoutTimer`,
`SpatialTimer`, `EscortTimer`); those are the first candidates for a
`/ORDER` or a link-order file. Against `build/place/TotalA.exe` the same tool
reports one run, no boundary and no out-of-order unit, as it must: that build
is `orig/TotalA.exe` byte for byte.

## Play tests: tools/playtest.py

`linkcmp.py` checks references; `tools/playtest.py` checks that a build plays
the way the original does. It drives the game under Wine through a scripted
scenario and gives each run PASS or FAIL:

```sh
uv run tools/playtest.py --list                                  # the scenarios
uv run tools/playtest.py --exe orig,place,carve --scenario full  # three runs at once
uv run tools/playtest.py --exe carve --scenario arm,core --repeat 2 --parallel 4
```

`--exe` takes `orig`, `place` (`build/place/TotalA.exe`), `carve`
(`build/link/TotalA.exe`) or the path of an exe, and `orig+carve` puts orig
in a scenario's first instance and carve in the others (a multiplayer game
between the two builds). Every pair of scenario and exe is a run, `--parallel` says how many run at once (all of them, at most
three, by default), and the game data comes from the Steam install
(`--game-dir` for another, such as GOG's). Each run is fenced off:

- its own X server, an invisible Xvfb display, so nothing opens on the
  desktop and its pointer and keyboard are nobody else's (`--window` shows it
  in a nested Xephyr window instead, to watch the game);
- its own Wine prefix, a copy of a template that `wineboot` makes once
  (`build/playtest/prefix`, made again when the Wine version changes), with
  Wine's crash dialog off, so a crash ends the game and leaves winedbg's
  message in `wine.log`;
- its own game directory: the install's small files copied, its large
  archives linked;
- its own PulseAudio null sink, so the game is silent on the desktop and the
  run can measure what it plays.

The output goes to `build/playtest/<session>/` (`build/playtest/latest` links
the newest): `summary.md`, one line per run with its result, the time, the
exit code, the end screens it met, the music tracks it opened and how many
of its named screenshots are identical to the first original run's of the
same scenario (`screens.md` lists the pixels that differ in the others: in a
game, the units and the shots fired are never quite where they were); and
one directory per run with `steps.log` (every step, timed), `wine.log`
(`WINEDEBUG=-all,err+seh` unless `--debug` says otherwise), `audio.log` (the
sink's level each second, with the music tracks open at the time),
`files.log` (when each music track was opened and closed), `result.json`,
the named screenshots as PNG and one JPEG every few seconds. A run fails when
a step fails (a screen not seen in time, the game gone when it should run),
when the game exits with a non-zero code or does not exit when told to, or
when `wine.log` reports an unhandled exception. Everything a run starts
(the X server, Wine and its server, `parec`, the sink) is stopped when it ends,
also on Ctrl-C (the run is then INTERRUPTED and the summary still written),
and the prefix and game directory are deleted unless `--keep` is given.

`--trap-stubs` plays a copy of each ordinary link instead (any exe whose map
names `stubs.obj`): every function stub there starts with `int3` and every
reference to a data stub holds 0 again, as the original's `mov eax, [0]`
does. A stub the game reaches then ends the run with a crash at the stub's
address (the map names it), where the link itself would carry on without
what the original does there. That is how the destructor 0x4b2290 registered
with `atexit` was found: the link called a stub at exit (#5701).

The scenarios (`tools/playtest/*.txt`):

| Scenario | What it plays | Time |
| --- | --- | ---: |
| `smoke` | the intro skipped, the main menu, quit | 10 s |
| `full` | #2662's first test: the whole intro, the options screens, a skirmish with save and load, Arm mission 1, quit | 12 min |
| `arm` | Arm missions 1 to 3 through the campaign, then 10, 18 and the last from the mission list, whose victory plays the ending and the credits | 11 min |
| `core` | Core mission 1 played out, 2 and 3 through the campaign, then 10, 18 and the last | 15 min |
| `long` | a 2 against 2 skirmish with Hard AIs and three factories queueing 100 units each, for about 45 minutes | 47 min |
| `mp` | two instances: one hosts a TCP/IP game, the other joins it at 127.0.0.1, both build and fight until a commander dies or the guest surrenders | 7 to 11 min |
| `music` | a skirmish with sound effects off: the game opens a `music/*.mp3` track and the sink carries it | 1.5 min |
| `idle`, `idle2` | one or two instances with the debug commands on, left for an hour to drive by hand | |

Three things in the game make these possible:

- **Any mission.** The campaign screen ("Play any game") lists every
  mission of every campaign, so a scenario picks one with the keyboard.
- **Debug commands.** With `DisplaymodeDepth` 256 and `Games` 1 in the
  game's registry key (0x42f9a0 reads them into a flag that the chat handler
  0x493bf0 turns into the command mask 7), the chat line accepts the debug
  commands of the table at 0x501fd0 as well as the cheats: `+iwin` and
  `+ilose` end a game or mission, `+kill`, `+control` and `+ai` change
  players. Without them `+iwin` is just a chat message. `PlayMovie` 0 skips
  the intro (the Cavedog logo still plays).
- **Multiplayer.** Wine's own DirectPlay TCP/IP provider (`dpwsockx`) is a
  stub up to Wine 9 (`DPWSCB_EnumSessions` and the rest only print fixmes),
  so TA finds no games. A scenario with `directplay` runs on a second
  template prefix with Microsoft's DirectPlay from the DirectX June 2010
  redistributable (`winetricks directplay`, which uses its cache in
  `~/.cache/winetricks` or downloads the redistributable). The two instances
  then see each other on 127.0.0.1. With Microsoft's DLLs the connection list
  is in the other order: TCP/IP is fourth. A host holds DirectPlay's port and
  a guest joins the first game it finds, so `directplay` runs take turns (a
  lock file in the temporary directory), also across sessions.

A scenario is a list of steps, one per line (`#` starts a comment). Lines
before `start` set up the run; `@2` before a step sends it to the second
instance.

| Step | Meaning |
| --- | --- |
| `instances N` | run N copies of the game (default 1) |
| `directplay` | use the prefix with Microsoft's DirectPlay |
| `reg [SUBKEY/]NAME dword\|sz VALUE` | set a value under the game's registry key before it starts |
| `file NAME TEXT` | write a file into the game directory (`\n` for line breaks) |
| `timeout MINUTES` | fail the run when it takes longer (default 60) |
| `start [WINEDEBUG]` | start the game (every instance) |
| `every S` | a JPEG screenshot every S seconds (0 stops them) |
| `click X Y [BUTTON]`, `sclick X Y`, `dclick X Y`, `hold X Y [S]`, `drag X1 Y1 X2 Y2`, `move X Y` | the mouse, in game coordinates; `sclick` holds shift |
| `key KEYS...`, `keyhold KEY S`, `type TEXT`, `chat TEXT` | the keyboard (xdotool key names); `chat` types a line between two Returns |
| `sleep S` | wait (the watched screens are checked meanwhile) |
| `shot NAME` | a PNG screenshot, compared with the original's in the summary |
| `waitfor SCREEN S` | fail unless the screen shows within S seconds |
| `trywait SCREEN S` | the same, carrying on either way |
| `until SCREEN S STEP [; STEP...]` | repeat the steps until the screen shows |
| `ifmatch SCREEN STEP`, `ifnot SCREEN STEP` | a step on condition |
| `expect SCREEN` | fail unless the screen shows now |
| `repeat N STEP [; STEP...]` | repeat steps |
| `label NAME`, `goto NAME` | jump |
| `on SCREEN goto NAME`, `on SCREEN off` | jump whenever the screen shows, such as a victory screen that may come at any time |
| `music S` | fail unless within S seconds the game holds a `music/*.mp3` track open while the sink carries sound |
| `alive` | fail if the game has exited |
| `waitexit S` | fail unless the game exits with code 0 within S seconds |
| `note TEXT`, `fail TEXT`, `stop` | a note in the summary, a failure, stop the game now |

Screens are recognised by signatures in `tools/playtest/screens.txt`, taken
from screenshots of the original exe rather than stored images: the mean
grey of every 4 x 4 square of an area (a button, a title), or for text over
terrain (PAUSED, "Click to continue.") the pixels of the text and its
outline. `--signature NAME WxH+X+Y SCREENSHOT...` prints a new line (several
screenshots of text over different terrain make a better mask) and
`--match SCREENSHOT...` lists the screens a screenshot shows.

The harness needs Xvfb (Xephyr for `--window`), xdotool, ImageMagick (`import`, `convert`,
`compare`), Wine, PulseAudio's `pactl` and `parec` (PipeWire's work), and
winetricks for `directplay`.

## The gap regions as source

The 29 `gap` rows of `data/functions.csv` (25,223 bytes, 25,456 with their
alignment padding) are the code no FPO record covers. They are not
assembly-language modules for the most part: they are compiled functions the
function finder could not see, because the compiler emits no FPO record for a
function with a frame it cannot describe. Every region but the import thunks
opens with `push ebp / mov ebp, esp`, and they fall into a few kinds:

- **Inline assembly** in otherwise compiled code: `cpuid` (0x4e16b0,
  0x4e35b0), `rdpmc` (0x4e1e50), `int 3` as an assertion (0x4d8310,
  0x4d9ab0), reading `ebp`, `esp` and `eip` for a stack trace (0x4d8870),
  reading `esp` for the stack's bounds (0x4d8d70), a `div` by zero inside
  `__try` (0x49e680).
- **No optimisation** (`/Od`): Cavedog's debug helpers at 0x4da120 and
  0x4da2c0 (DebugHelper.dll, the debug thread and its message pump), every
  value stored to its local and loaded again, and int3 padding (from the
  linker) after each function. Without optimisation the compiler lays the
  locals out in its symbol table's order, which follows their names, not
  their declarations: the names in those files are ones that give the
  original's frame.
- **Structured exception handling**, `__try`/`__except` with
  `__except_handler3` (0x497c70, 0x49e680, WinMain at 0x49eda0, 0x4d9ab0).
- **C++ exception handling**, `try`/`catch` frames (0x4441a0, 0x444580, the
  three at 0x45b250, 0x49ee30, and 0x4c4fa0, which is `basic_string::_Copy`
  from the compiler's own `<xstring>`). MSVC 5 builds these frames without
  `/GX` too (with a warning), and none of them matches with it. In a function
  with a `try` or `__try`, every local gets a frame slot of its own (unused
  ones, those kept in registers and those of inlined callees too), allocated
  one scope after another and, within a scope, in 16 buckets by a hash of
  the name (the later declaration first within a bucket): the names in those
  files are ones that give the original's frame.
- **Aligned frames**, `and esp, -8` before the locals (0x41dc20, 0x420d20,
  0x42a8d0, 0x466050, 0x46c2a0, 0x49a120): MSVC 5 builds this frame under the
  game's usual flags once a function keeps enough 8-byte values on the stack
  (`double` locals, an unsigned-to-float conversion through a qword
  temporary, doubles passed or returned). `/Op` builds it too, but stores
  every intermediate `double` and calls `_CIsqrt` for `sqrt`, so none of
  these was compiled with it.
- **`_alloca`** (0x4bb4e0, 0x4bc800, and the command-line parser at 0x49ee30).
- **Hand-written assembly**: the fixed-point trigonometry at 0x4b70a0 and the
  surface drawing at 0x4cbbe0 (MASM frames, `leave`, routines that run into
  one another with no alignment, int3 padding from the linker after them).
- **The linker's import thunks** at 0x49f710: 70 `jmp [__imp_X]` stubs, the
  `.text` of the import libraries' members for the APIs the game calls
  directly. These are library code rather than source, so `tools/place.py`
  places each one from its import library member (the toolchain's, and
  LIB.EXE's from `link/*.def` for the DLLs imported by ordinal), and
  `tools/carve.py` no longer carves the row.

A region's source is one file, in the folder of its subsystem like any
other (`game/main_49eda0.cpp` is WinMain), and `uv run tools/gapcheck.py
<address>` checks it. A file is gap code when it annotates a function inside a
gap region (`tools/sources.py`), wherever it lives:

- Each function is annotated `// FUNCTION: 0x...` as everywhere else, and
  `tools/check.py` checks one function on its own. `tools/progress.py` leaves
  gap files out: these are not `game` rows.
- Inline assembly is allowed in these files only (`__asm`, and `_emit` for
  instructions MSVC 5's inline assembler does not know: `cpuid` is
  `_emit 0x0f` `_emit 0xa2`). `// FLAGS:` may add `/Op`, `/GX`, `/Od` and
  `/Gy` here, besides `/Gi` (`/Od` turns off the `/Gy` that `/O2` implies, and
  the debug helpers need it back: each function is a COMDAT of its own).
- Hand-written assembly is a `__declspec(naked)` function holding the whole
  run of routines. MSVC 5 starts every function on a 16-byte boundary, each in
  a COMDAT of its own under `/Gy` and padded with nops in one `.text` section
  without it, so routines that follow one another unaligned cannot be
  separate functions. A routine that
  starts inside it is an `__asm` label with `// ENTRY: 0x... [symbol]` on the
  line before. Inline assembly keeps its labels private, so `gapcheck.py`
  adds a public symbol (`_FUN_<address>` unless one is named) at the label's
  offset, once the function holding it matches and the offset falls on one of
  its instructions; the object with those symbols is `build/gap/<address>.obj`.
  The inline assembler takes MASM's `ALIGN 4` and pads with MASM's bytes;
  `mov [ebp - 0xc], offset label` stores a label's address with a relocation;
  and the few encodings it cannot produce (`cmp` of two byte registers in the
  38 /r form, one backward jump it sizes short) are `_emit`ted with a comment.
- A region MATCHES when every annotated function matches as `check.py` defines
  it, each compared over the original's extent (to the next annotated function
  or the region's end, less padding), and together they cover every byte of
  the region apart from the padding between them. Data of a function's own
  whose fields point back into it (an SEH scope table's filter and handler)
  is compared by where those fields point.

Both builds use a region's object once it matches, and only then:
`tools/place.py` places its functions at their addresses (counted as gap
code), `tools/link.py` links it like any other object (in every mode), and
`tools/carve.py` carves only the regions still without matching source,
naming addresses in the others by their objects' symbols. `tools/linkcmp.py`
compares the gap functions' references too.

On 2026-10-05, 28 of the 29 regions match their source (24,804 of 25,223
bytes) and the import thunks are built from their library members, so
neither build takes any code from the original. The last, 0x49a120,
matched once `<windows.h>` (lean) and `<vector>` were included after the
game's own declarations rather than before them: two of its adds follow
the ids of its locals and of g_game, and the locals need ids past 16384
while g_game's stays small (its notes have the measurements).

| Region | Bytes | What it holds | Source |
| --- | ---: | --- | --- |
| 0x41dc20 | 697 | aligned frame: the end-of-game statistics table | matches |
| 0x420d20 | 291 | aligned frame: an explosion frame bitmap | matches |
| 0x42a8d0 | 2,719 | aligned frame: the unit type loader | matches |
| 0x4441a0 | 801 | `try`/`catch`: the SELPROV.GUI menu handler | matches |
| 0x444580 | 898 | `try`/`catch`: SELPROV.GUI, a button per online service | matches |
| 0x45b250 | 560 | `try`/`catch`: online.dll button commands | matches |
| 0x45b490 | 417 | `try`/`catch`: online.dll link names | matches |
| 0x45b670 | 395 | `try`/`catch`: online.dll configuration | matches |
| 0x466050 | 1,326 | aligned frames: a saved game's player section, loaded and saved | matches |
| 0x46c2a0 | 882 | aligned frame: the score tables for the statistics DLL | matches |
| 0x497c70 | 101 | `__try`/`__except`: the loading thread | matches |
| 0x49a120 | 1,829 | aligned frame: a weapon's area damage | matches |
| 0x49e680 | 106 | `__try`/`__except`, inline `div`: a deliberate fault to report a message | matches |
| 0x49eda0 | 1,942 | WinMain (`__try`/`__except`); command line (`try`/`catch`, `_alloca`) | matches |
| 0x49f710 | 419 | the linker's import thunks | placed from the import libraries |
| 0x4b70a0 | 772 | hand-written: fixed-point trigonometry, 10 entry points | matches |
| 0x4bb4e0 | 198 | `_alloca`: the archive entry a path names | matches |
| 0x4bc800 | 197 | `_alloca`: the archive directory a path ends in | matches |
| 0x4c4fa0 | 255 | `basic_string::_Copy`, `try`/`catch` | matches |
| 0x4cbbe0 | 7,622 | hand-written: surface drawing, five modules with 23 more entry points | matches |
| 0x4d8310 | 67 | inline `int 3`: a fill-pattern check | matches |
| 0x4d8870 | 318 | inline asm: two constructors that record a stack trace | matches |
| 0x4d8d70 | 125 | inline asm: stack bounds, in three thread-local variables | matches |
| 0x4d9ab0 | 420 | `__try`/`__except`, inline `int 3`: the fatal error handler | matches |
| 0x4da120 | 379 | `/Od`: DebugHelper.dll and the debug set-up | matches |
| 0x4da2c0 | 303 | `/Od`: the debug thread and its message pump | matches |
| 0x4e16b0 | 74 | inline `cpuid` | matches |
| 0x4e1e50 | 761 | inline `rdpmc`: a profiling timer's report and restart | matches |
| 0x4e35b0 | 349 | inline `cpuid`: opens the GDPERF counter driver | matches |

## Next steps

### The risk in consolidating: compiler state

MSVC 5's register allocation and operand order depend on the compiler's
state, not only on the function's source: `docs/agent-guide.md` records
functions that match only after a particular header set, or only after a
particular number of unrelated declarations (0x4b6c30, 0x417f60). Any shared
header that phases 2 to 4 add changes that state for every file that includes
it. `tools/stateprobe.py` measures this: it compiles every file holding a
matched function with a header force-included in front (`/FI`) and compares
each of the 3,038 matched functions again.

| Header in front (`tools/stateprobe.py ...`) | Matched functions that no longer match |
| --- | ---: |
| an empty file (`--empty`) | 33 (1.1%) |
| 700 unused `extern int` declarations (`--externs 700`) | 141 (4.6%) |
| `link/globals.h` itself, its names prefixed so nothing clashes (`link/globals.h --rename`) | 126 (4.1%) |

An empty header already changes 33 functions: merely opening one more file
moves them (0x401320 swaps the operands of two `fld`s). The three sets overlap
only in part (172 functions fail under at least one), so there is no fixed
list of fragile functions to route around: which ones move depends on the
exact header. A real consolidation also changes struct names, member order
and the inline budget, so expect more than these numbers.

### Phase 2: consolidate types and units

- **Re-check everything after every step.** Run `uv run tools/progress.py`
  after each merge and compare `data/progress.csv` with the version before:
  any function that drops from `matched` to `partial` is a regression to fix
  or revert. Keep each step small enough that a regression has one cause.
- **One address area at a time.** Work through one 64 KB window (the area
  table in the README) or one unit from `tools/unitmap.py` at a time, so a
  regression is local and revertible. Start with the 59 units whose
  declarations agree (`uv run tools/unitmap.py --list`), and generate each one
  with `tools/unitgen.py`.
- **Do not include `link/globals.h` wholesale.** Bring each global's
  canonical declaration into the files of the area being merged, re-check, and
  keep or revert per file. Before a new shared header goes into an area, run
  `uv run tools/stateprobe.py <header> --rename --area 0x<window>` to see which
  of its matches it would move. Where a function only matches in a particular
  compiler state (its file needs a header set from `tools/headers.py`, or the
  probe flags it), note it in `docs/consolidation.md` and keep it in a file of
  its own until its real neighbours are known.
- **Type names first, layouts second.** 1,044 undefined function names and
  21 globals differ only in struct names: renaming placeholder structs to one
  name per real type (starting with `g_game`'s `Game`, which 1,031 files
  touch) removes those without changing any layout. The 2,027 real signature
  differences and the 85 shape conflicts need a decision each, recorded where
  `docs/consolidation.md` already lists the known ones.
- **Settle the duplicates while merging.** The 29 functions defined in
  several files become one definition (or `inline` in a shared header) when
  their unit is merged; `tools/linkcheck.py --verbose` lists them.

### Phase 3: one name and one prototype per function

Make `data/symbols.csv` the only source of names and generate one prototype
per function from it, so every caller spells a callee as its definition does.
`tools/linkcheck.py`'s *defined under another name* and *same name, other
signature* lists are the work queue.

A spelling the source cannot yet express can be bridged at link time without
touching `src/`. LINK 5.10 has `/FORCE` (link despite unresolved or multiply
defined symbols) and, judging by the strings in LINK.EXE, no
`/ALTERNATENAME`, but it does resolve COFF weak externals with an alias:
OLDNAMES.LIB consists of nothing else (its member for `_fcloseall` is a weak
external `__imp__fcloseall` that falls back to `__imp___fcloseall`). A
generated object of such records, one per caller spelling, pointing at the
spelling that is defined, resolves the 3,220 function spellings and every
spelling of a global (all 800 of `g_game`'s) to one definition. It makes the
link succeed, not the types agree, so it is a bridge for phase 4 while phases
2 and 3 proceed, not a substitute for them.

### Phase 4: link and iterate

- Add `tools/link.py` (`tools/wlink` already runs LINK.EXE under Wine): link
  every object with `link/data.cpp`, LIBCMT, LIBCPMT and the import libraries
  above, and parse LINK's errors back into the categories here.
- Build import libraries for `smackw32.dll` and DPLAYX's ordinals from `.def`
  files with LIB.EXE.
- Give the 75 FPO-less entry points source: decompile the 29 gap regions
  (under way, see "The gap regions as source").
- Replace the initial values of `data.cpp` that hold addresses as plain
  bytes (`pointers` in `data/globals.csv`) with symbolic initialisers; as
  plain bytes they would point into the old layout.
- Link with the weak-external aliases described under phase 3 until the
  spellings agree, and settle the 32 real duplicates (keep the annotated
  definition, make the copies `static` or `inline`) and the 3 names that
  clash with LIBCMT and LIBCPMT (`operator new` and `operator delete` in
  0x4b4f10, a `ctype` id in 0x41ce90).
- Pick one version of each of the 53 differing COMDAT folds explicitly
  rather than leaving it to the linker.
