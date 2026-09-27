# Agent calibration

Which model can decompile which functions, measured on real batches. Every
result here was re-verified with `tools/check.py` by the orchestrator; agents'
own claims are not counted. Raw per-function records are in `data/attempts.csv`.

## Method

- Functions are grouped by size band and whether they call other game
  functions ("leaf" functions call none).
- Batches for different models are drawn evenly from the same band, so each
  model sees a similar spread of difficulty.
- Each agent reads `docs/agent-guide.md`, then works through its list with a
  fixed budget of `check.py` runs per function.

## Results so far

<!-- calibration:start -->
### First-attempt match rate by function size

| Size (bytes) | Haiku | Opus | Sonnet |
| --- | ---: | ---: | ---: |
| 1-16 | 283/312 (91%) | 3/3 (100%) |  |
| 17-40 | 158/217 (73%) |  | 9/10 (90%) |
| 41-64 | 3/11 (27%) |  | 64/74 (86%) |
| 65-160 | 1/6 (17%) | 246/249 (99%) | 2/6 (33%) |
| 161-400 |  | 8/12 (67%) |  |

### Cost per batch

Cost units: thousands of tokens weighted by price relative to Haiku (Sonnet 5 costs 2x per token, Opus 5.5 4x). Token counts are the harness's totals per agent.

| Batch | Model | Functions | Matched | Tokens | Tokens per match | Cost units per match | Minutes |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: |
| H1 | haiku | 8 | 5 | 74,514 | 14,902 | 15 | 6 |
| H0 | haiku | 30 | 29 | 75,811 | 2,614 | 3 | 7 |
| H2 | haiku | 6 | 1 | 113,234 | 113,234 | 113 | 10 |
| S1 | sonnet | 8 | 7 | 109,303 | 15,614 | 31 | 11 |
| H3 | haiku | 10 | 7 | 93,884 | 13,412 | 13 | 9 |
| H4 | haiku | 40 | 40 | 55,163 | 1,379 | 1 | 3 |
| H5 | haiku | 40 | 40 | 62,315 | 1,557 | 2 | 5 |
| S2 | sonnet | 6 | 2 | 207,870 | 103,935 | 208 | 25 |
| H6 | haiku | 12 | 12 | 67,840 | 5,653 | 6 | 6 |
| H7 | haiku | 40 | 40 | 57,819 | 1,445 | 1 | 4 |
| S3 | sonnet | 10 | 8 | 133,720 | 16,715 | 33 | 15 |
| H8 | haiku | 15 | 7 | 98,163 | 14,023 | 14 | 10 |
| H9 | haiku | 8 | 3 | 95,670 | 31,890 | 32 | 8 |
| O2 | opus | 4 | 4 | 122,629 | 30,657 | 123 | 12 |
| H10 | haiku | 40 | 40 | 70,066 | 1,751 | 2 | 5 |
| O3 | opus | 6 | 6 | 89,281 | 14,880 | 60 | 6 |
| S5 | sonnet | 8 | 8 | 94,116 | 11,764 | 24 | 9 |
| H11 | haiku | 40 | 35 | 96,376 | 2,753 | 3 | 11 |
| O5 | opus | 8 | 8 | 76,715 | 9,589 | 38 | 4 |
| S7 | sonnet | 8 | 6 | 128,367 | 21,394 | 43 | 14 |
| S8 | sonnet | 5 | 5 | 90,150 | 18,030 | 36 | 7 |
| S4 | sonnet | 11 | 7 | 246,350 | 35,192 | 70 | 30 |
| H12 | haiku | 48 | 35 | 113,802 | 3,251 | 3 | 11 |
| O9 | opus | 4 | 4 | 77,674 | 19,418 | 78 | 4 |
| O4 | opus | 5 | 3 | 227,688 | 75,896 | 304 | 25 |
| S6 | sonnet | 10 | 5 | 258,908 | 51,781 | 104 | 36 |
| O8 | opus | 8 | 7 | 135,415 | 19,345 | 77 | 10 |
| O12 | opus | 5 | 5 | 59,980 | 11,996 | 48 | 2 |
| H13 | haiku | 34 | 24 | 110,397 | 4,599 | 5 | 12 |
| O7 | opus | 5 | 5 | 217,143 | 43,428 | 174 | 19 |
| H14 | haiku | 15 | 13 | 106,141 | 8,164 | 8 | 11 |
| O13 | opus | 8 | 8 | 73,619 | 9,202 | 37 | 3 |
| S9 | sonnet | 10 | 10 | 139,663 | 13,966 | 28 | 17 |
| O10 | opus | 8 | 8 | 177,828 | 22,228 | 89 | 14 |
| O15 | opus | 10 | 10 | 122,592 | 12,259 | 49 | 8 |
| S10 | sonnet | 10 | 7 | 209,102 | 29,871 | 60 | 23 |
| H15 | haiku | 20 | 8 | 114,543 | 14,317 | 14 | 12 |
| O16 | opus | 10 | 10 | 181,978 | 18,197 | 73 | 13 |
| O17 | opus | 10 | 10 | 146,685 | 14,668 | 59 | 11 |
| O19 | opus | 10 | 10 | 125,371 | 12,537 | 50 | 8 |
| H16 | haiku | 20 | 17 | 112,841 | 6,637 | 7 | 11 |
| S11 | sonnet | 11 | 11 | 155,898 | 14,172 | 28 | 17 |
| O14 | opus | 6 | 4 | 300,817 | 75,204 | 301 | 42 |
| O20 | opus | 10 | 10 | 174,015 | 17,401 | 70 | 16 |
| O21 | opus | 10 | 10 | 95,324 | 9,532 | 38 | 5 |
| H17 | haiku | 20 | 16 | 115,116 | 7,194 | 7 | 12 |
| O23 | opus | 10 | 10 | 116,571 | 11,657 | 47 | 6 |
| O22 | opus | 10 | 10 | 112,464 | 11,246 | 45 | 6 |
| S12 | sonnet | 12 | 12 | 165,246 | 13,770 | 28 | 19 |
| O24 | opus | 10 | 10 | 100,952 | 10,095 | 40 | 5 |
| O25 | opus | 10 | 10 | 116,767 | 11,676 | 47 | 6 |
| H18 | haiku | 20 | 12 | 105,654 | 8,804 | 9 | 9 |
| O18 | opus | 6 | 5 | 388,863 | 77,772 | 311 | 45 |
| S14 | sonnet | 8 | 8 | 95,103 | 11,887 | 24 | 7 |
| O27 | opus | 10 | 10 | 139,262 | 13,926 | 56 | 9 |
| O26 | opus | 10 | 10 | 136,052 | 13,605 | 54 | 10 |
| O28 | opus | 10 | 10 | 132,391 | 13,239 | 53 | 7 |
| S13 | sonnet | 10 | 10 | 179,018 | 17,901 | 36 | 19 |
| H19 | haiku | 20 | 17 | 117,010 | 6,882 | 7 | 12 |
| O31 | opus | 10 | 10 | 180,994 | 18,099 | 72 | 13 |
| O30 | opus | 11 | 10 | 236,166 | 23,616 | 94 | 20 |
| H20 | haiku | 20 | 14 | 121,616 | 8,686 | 9 | 11 |
| O32 | opus | 10 | 10 | 112,033 | 11,203 | 45 | 6 |
| O29 | opus | 10 | 9 | 247,478 | 27,497 | 110 | 27 |
| S15 | sonnet | 10 | 10 | 202,339 | 20,233 | 40 | 21 |
| S16 | sonnet | 10 | 10 | 144,922 | 14,492 | 29 | 14 |
| H21 | haiku | 20 | 18 | 120,661 | 6,703 | 7 | 11 |
| O34 | opus | 10 | 10 | 127,012 | 12,701 | 51 | 8 |
| O33 | opus | 10 | 10 | 175,050 | 17,505 | 70 | 12 |
| S18 | sonnet | 10 | 10 | 126,236 | 12,623 | 25 | 10 |
| O35 | opus | 10 | 10 | 116,874 | 11,687 | 47 | 6 |
| H22 | haiku | 20 | 12 | 107,474 | 8,956 | 9 | 10 |
| O36 | opus | 10 | 10 | 126,901 | 12,690 | 51 | 10 |

### Escalations

- Opus matched 32 of 33 functions a cheaper model had failed.
- Sonnet matched 61 of 67 functions a cheaper model had failed.
<!-- calibration:end -->

## Findings about the target

- Only 3 of 3,342 game functions set up a C++ exception frame (`fs:[0]`), all at
  0x4e3a90 to 0x4e4010, just before the runtime library. Cavedog's code barely
  uses C++ exceptions, so `/GX` rarely matters. The FPO "has SEH" bit is never
  set in this exe, so `data/functions.csv`'s `seh` column carries no information.
- Cavedog compiled with automatic inlining, `/O2 /Ob2`, not the Visual C++ 5.0
  Release default of `/O2`. Found through global `std::vector` initialisers,
  whose construct and `atexit` steps are only merged into one function under
  `/Ob2`; every earlier match still matches with it.
- The game uses the compiler's own STL (`std::vector`, including out-of-line
  `erase`, and `std::map`) and also a vector-shaped container of its own: the
  global at 0x438450 has an atexit destructor with no destroy loop, which
  `std::vector` never produces. The global at 0x434a30 is a file-scope `static`
  `std::vector` of 8-byte elements with a destructor; both destructors match. Every case where our compiler seemed to "optimise more" than
  Cavedog's turned out to be a difference in the source (an inlined helper, a
  no-op cast, an extra return value, an off-by-one), not in the compiler.
- Cavedog compiled without `/GX` (no C++ exception handling). The real STL
  version of `std::map`'s iterator increment (0x46ea10) matches byte-for-byte
  only without it; with it MSVC adds an exception frame the original lacks.
  The only exception frames in the exe belong to Microsoft's C++ library.
- The C++ runtime library (LIBCPMT) accounts for 15 functions inside the game
  region: `std::string` internals instantiated in Cavedog's objects, two copies
  of `std::_Lockit`, and the std exception classes. They are now `library`.
- Known checker limit: a vtable defined in a decompiled file is verified by
  name, not by its entries, so a file can declare fewer virtual methods than
  the original vtable has (0x4b0610 declares 4 of 21). Resolving each vtable
  entry against the name map would close this.
- Two copies of the C++ library's lock code (`std::_Lockit` and its cleanup)
  are linked in, and a block of code at 0x4d8000-0x4e3000 calls the copy that
  sits inside it. That block is probably a separately built library of
  Cavedog's (or a third party's) linked after the game's own objects.
- Some near-misses depend on compiler state left by earlier functions in the
  same source file: in 0x4581e0 the load order of one `a + b` flips with
  unrelated code placed before it. These should resolve once functions are
  regrouped into their original translation units in address order, which is
  a later phase of the project.
- The game statically links **zlib 1.0.4** (its `zlibVersion()` returns "1.0.4";
  TA's archives are compressed), built with `/Gz /Zp1`: every function
  `__stdcall` and structs packed to 1 byte. Compiling the real zlib 1.0.4 source
  that way reproduces 53 functions (about 25 KB, 0x4d1c80-0x4d7d70) byte for byte,
  so they are marked `library`; `tools/setup_toolchain.sh` builds it.
- The game itself was **not** built with `/Zp1`: adding it to every matched file
  loses 11 matches (all STL containers, which need natural alignment) and gains
  none. About a fifth of the files use `#pragma pack` for game structs with
  fields at odd offsets, so Cavedog packed particular structs (game state, file
  formats) in their headers rather than the whole build.
