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
| 1-16 | 283/312 (91%) | 2/2 (100%) |  |
| 17-40 | 69/97 (71%) |  | 9/10 (90%) |
| 41-64 | 3/11 (27%) |  | 21/31 (68%) |
| 65-160 | 1/6 (17%) | 77/78 (99%) | 2/6 (33%) |
| 161-400 |  | 7/11 (64%) |  |

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

### Escalations

- Opus matched 25 of 25 functions a cheaper model had failed.
- Sonnet matched 44 of 50 functions a cheaper model had failed.
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
  `erase`) and also a vector-shaped container of its own: the global at 0x438450
  has an atexit destructor with no destroy loop, which `std::vector` never
  produces. Every case where our compiler seemed to "optimise more" than
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
