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
