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
uv run tools/link.py --carve           # an ordinary LINK.EXE link that runs: build/link/TotalA.exe
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
symbol (`?DAT_00512344@@3PAUEntry@@A` is an `Entry*`). An array mangles exactly
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
  (`FUN_0040f200` is defined in 13 files; see "Context-dependent functions" in
  `docs/consolidation.md`) and constructors, destructors and class
  `operator delete`s repeated in each file of a class family
  (`Class_00407350`'s constructor in 8 files, `Class_00471cc0`'s destructor and
  `operator delete` in 6). The 3 globals defined twice are `DAT_00512340`,
  `DAT_00512358` and `DAT_00513000`.
- **53 folds are silent hazards.** Inline functions and vtables that differ
  between objects link without complaint and the linker keeps any one. Most
  come from files standing in for a helper differently: `vector<Unit*>::_Ucopy`
  exists in two versions, one calling the `/Gz` stand-in `FUN_00406c70`
  (0x405d90 and four others) and one inlining it (0x406c10 and five others);
  `vector<Elem_00434360>::operator=` has four. The `std` exception classes
  differ in 0x4c3cc0 only, and the vtables of `Class_00471560`,
  `Class_004716a0`, `Class_0048eb40` and `Class_0048f840` differ between the
  files that declare them. A linked build needs one version of each, and for a
  byte-identical build it must be the original's.
- **`.bss` starts before the raw end of `.data`.** The section's raw bytes run
  to 0x511a00 (file alignment), but the last non-zero byte is at 0x5119b3 and
  `DAT_005119e8` (an `int[10]`) runs past 0x511a00, so uninitialised data
  begins at about 0x5119b8.

## The global data manifest

`tools/globals.py` builds three files from the same objects.

**`data/globals.csv`**: one row per global the source refers to by address
(788 rows: 541 in `.bss`, 204 in `.data`, 43 in `.rdata`).

| Column | Meaning |
| --- | --- |
| `address`, `name` | `name` is `data/symbols.csv`'s when it has a real one, else `DAT_<address>` |
| `section` | `.rdata`, `.data`, or `.bss` (the zero-filled tail of `.data`, estimated as above) |
| `size`, `size_from` | `type`: the size of the declared type; `gap`: the distance to the next address the original's code or data, `data/symbols.csv` or the source refers to (past the largest offset the source uses); `type>gap`: a declared type that runs over the next known address (11 rows; for example `DAT_00511a58`, an `int[45][2]`, overlaps `DAT_00511a60`, a `Pair_00419560[44]` that other files declare 8 bytes further on) |
| `kind` | `data`, `string` (its bytes are a C string), `vtable` (a run of function pointers in `.rdata`), `float`, `template` (a static member of an STL tree), `library` (CRT data) |
| `type`, `type_files`, `other_files`, `types` | the most common declared type, how many files declare exactly it, how many declare something else, and how many distinct types there are; `T[]` counts as agreeing with `T[N]` |
| `verdict` | tools/linkcheck.py's verdict on the declarations |
| `files` | how many files refer to it |
| `max_offset` | the largest offset from it the source uses (`DAT_x + 8` is 0x8) |
| `ghidra` | the label Ghidra's export uses for the address (`s_` a string, `PTR_` a pointer) |
| `pointers` | aligned dwords in its initial bytes that point into the exe: initial values that need symbols, not numbers, before relinking |
| `init` | its first 64 bytes in the exe, in hex (empty for `.bss`) |

**`link/globals.h`** declares 706 of the 788 globals once, with the type most
files give them, when that type is settled: every view agrees up to struct
names or signedness, three quarters of the files agree on it, or three
quarters agree on its shape and it is the commonest type of that shape (so
`g_game` is declared `Game*`). A pointer to a struct needs only a forward
declaration; a struct held by value is declared as a byte array of its size
with the struct's name in a comment; a global only ever declared
`extern "C"` is declared `extern "C"`. The 82 globals left out are listed at
the end with their competing types: vtables, STL tree statics, and globals
whose files disagree (`DAT_0051fba4`, `DAT_00513000`, `DAT_005119c0`, ...).

**`link/data.cpp`** defines every global `globals.h` declares, with the
original's initial values: numbers, floats (exact), strings as literals,
pointers to strings as string literals, pointers to other globals in the file
as their address, and zero-initialised `.bss`. `--check` compiles it with the
game's flags and compares each definition with the exe: it compiles with no
warnings, and 704 of the 706 definitions hold the original's bytes (pointer
fields are compared as "some address"). The two that differ hold the 5
initial values left as `TODO`: `DAT_0050a788`'s four pointers to GUIDs no row
names, and the fourth element of `DAT_00509688`, which the source declares
`char*[4]` where the original has three pointers and then string bytes.

Of the 2,459 references in the tree to the globals `data.cpp` defines, 1,288
spell them exactly as it does and would resolve against it today; the rest
declare another type and so have another mangled name.

Neither file is included by anything under `src/`. They are reference
material and the starting point for phase 4.

`tools/ctx.py` prints, under `-- globals --`, the type and size
`data/globals.csv` gives each global a function refers to, so a decompiling
agent sees the type most of the tree already uses. Without `globals.csv` it
prints what it did before.

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
  where a vtable or table the source declares disagrees with the original's
  entry (the source's partial view of a class), the original's entry is kept
  and listed.
- **`link/data.cpp`** and the globals `tools/globals.py` leaves out are placed
  at their addresses, one global at a time.
- **The runtime library** comes from the members of the libraries the
  original links statically: the VC5 SP3 `LIBCMT.LIB` and `LIBCPMT.LIB`, and
  zlib 1.0.4 as `tools/setup_toolchain.sh` builds it with Cavedog's options. A
  member goes where `data/functions.csv` has its function, provided the
  original holds its bytes there and refers to the same things: its calls
  reach functions of the same names, its imports the same slots, and its own
  data (strings, tables, exception and type information, initial values) the
  same contents. Bytes alone cannot tell many of them apart: the locking
  wrappers `_read`, `_write` and `_lseek`, `_Xlen` and `_Xran`, or zlib's
  `get_crc_table` and `zlibVersion` differ only in what they refer to. The
  report lists the five functions whose `data/functions.csv` name was one of
  such a pair. Functions with no FPO record of their own that sit inside
  another's row (`__allshr` and `__allshl` after `__ftol`) are found by their
  first bytes at each free 16-byte boundary. Static functions and the
  members' data follow where the placed code refers to them, and communal
  (`.bss`) data where the original has it.
- **What has no source** is copied from the original and counted as copied:
  the 29 gap regions, 11 runtime library functions no member matches (four
  `basic_string` members Cavedog's objects instantiated, the `exception`
  constructors, three without a name), data no object defines, the linker's
  import tables, the headers, `.tls` and the resources.

Every relocation is checked against the address the original's bytes give at
that spot, and the finished image is compared with the original byte for
byte, which is the placement and data compare #4869 asks for. The report
counts where each section's bytes came from. On 2026-10-05 at cb45af91:

| Section | Bytes | Built | Copied |
| --- | ---: | --- | --- |
| `.text` | 1,026,560 | 850,853 game code, 116,543 runtime library, 25,371 padding | 25,456 gap regions, 8,337 runtime library |
| `.rdata` | 18,432 | 2,444 compiled data, 3,771 library data, 2,948 `link/` globals, 372 padding | 6,529 import tables, 2,368 other data |
| `.data` | 173,660 | 34,172 compiled data, 24,845 library data, 78,479 `link/` globals | 36,164 |

Of the 37,394 relocations in placed pieces, every one in code agrees with the
original (136 of them reach the second copy of a function `data/aliases.csv`
lists, such as the two `std::_Lockit`). 94 vtable entries in compiled data
disagree and keep the original's value. The image differs from the original
in 16 bytes, the `TODO` initial values of `link/data.cpp` (`DAT_0050a788`'s
four GUID pointers and the fourth element of `DAT_00509688`), and under the
two rows of `data/exe_patches.csv`, where it has the compiler's bytes rather
than GOG's no-CD music patch. `build/place/TotalA.map` lists every placed
piece and the object it came from.

To run it, copy it into a copy of the game's directory (the Steam or GOG
install, with `smackw32.dll` and `win32.dll`) and start it under Wine, for
example `wine explorer /desktop=TA,800x600 TotalA.exe`. Like the original, it
shows a DirectX version warning over the main menu in a fresh Wine prefix,
and it starts and plays a skirmish game.

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

- **`gaps.obj`** holds the 29 gap regions and the exception handler code of
  their functions, one section each. Its relocations come from disassembly:
  every `rel32` branch that leaves its region, and every 32-bit immediate or
  displacement that holds an address in the image. Each entry point the tree
  calls gets its `FUN_<address>` name, and WinMain (0x49eda0, a gap region)
  its `_WinMain@16`.
- **`origdata.obj`** holds the original's `.rdata` and `.data` byte for byte,
  apart from the tables LINK builds itself (imports, the TLS and debug
  directories, the `.CRT$X*` tables). Its relocations come from `place.py`'s
  layout: every pointer field of a placed piece of data; and where no object
  defines the data, every dword that holds the exact address of a function, a
  global, a placed piece or a string (unaligned ones too: the 25-byte packed
  order records 0x43bc90 registers hold their callbacks at +4), every element
  of a global `data/globals.csv` types as a pointer, and every address in the
  compiler's exception tables.

The same layout says what every reference in the tree's own objects means,
and `link.py` applies that to patched copies of the objects under
`build/link/objs/`:

- A name no object defines is aliased to the symbol at the address the
  original's code holds wherever the name is used, in a placed function or in
  a byte-identical copy of one (4,952 names), rather than guessed from its
  spelling.
- `origdata.obj` defines, at each global's address and each placed vtable's,
  every name a compiled object defines there, and is linked first, so LINK
  keeps one copy of each: the original's, at its full size and with every
  slot (the tree's vtables are partial views of their classes).
- A placed function's references to file statics, and to other functions of
  its own file, point where the original's code points (1,159 references).
  Many files keep a global as a file-scope `static` because that makes their
  function match (0x4223e0.cpp's `static FeatureList* DAT_00511fb4`), and
  copies of callees kept so that they inline (`docs/consolidation.md`) would
  otherwise be called instead of the real function.
- Every other file's definition of an annotated game function is made
  static, so the name binds to the annotated one (586 copies).
- Only the original's 17 C++ static initialisers run, in its order (see
  `fix_initialisers` in `link.py`): files that define global objects only so
  that a function matches would otherwise construct them with the wrong
  constructors at start-up.
- zlib comes from the objects `tools/setup_toolchain.sh` builds, and the
  runtime library from `LIBCMT.LIB` and `LIBCPMT.LIB`, as LINK picks them.

Eleven names are still stubbed (constructors, destructors and operators that
only unplaced copies or the dropped initialisers call), and 37 addresses in
dead data no symbol names.

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
- Give the 75 FPO-less entry points source: decompile the 29 gap regions or
  stub them for a first link.
- Replace `data.cpp`'s 5 `TODO` initial values and the 59 rows whose initial
  bytes hold addresses (`pointers` in `data/globals.csv`) with symbolic
  initialisers; as plain bytes they would point into the old layout.
- Link with the weak-external aliases described under phase 3 until the
  spellings agree, and settle the 32 real duplicates (keep the annotated
  definition, make the copies `static` or `inline`) and the 3 names that
  clash with LIBCMT and LIBCPMT (`operator new` and `operator delete` in
  0x4b4f10, a `ctype` id in 0x41ce90).
- Pick one version of each of the 53 differing COMDAT folds explicitly
  rather than leaving it to the linker.
