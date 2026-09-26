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

| Size (bytes) | Haiku | Sonnet |
| --- | ---: | ---: |
| 1-16 | 149/150 (99%) |  |
| 17-40 | 31/42 (74%) | 5/5 (100%) |
| 41-64 | 3/11 (27%) | 6/7 (86%) |
| 65-160 | 1/6 (17%) | 2/6 (33%) |

### Cost per batch

| Batch | Model | Functions | Matched | Tokens | Tokens per match | Minutes |
| --- | --- | ---: | ---: | ---: | ---: | ---: |
| H1 | haiku | 8 | 5 | 74,514 | 14,902 | 6 |
| H0 | haiku | 30 | 29 | 75,811 | 2,614 | 7 |
| H2 | haiku | 6 | 1 | 113,234 | 113,234 | 10 |
| S1 | sonnet | 8 | 7 | 109,303 | 15,614 | 11 |
| H3 | haiku | 10 | 7 | 93,884 | 13,412 | 9 |
| H4 | haiku | 40 | 40 | 55,163 | 1,379 | 3 |
| H5 | haiku | 40 | 40 | 62,315 | 1,557 | 5 |
| S2 | sonnet | 6 | 2 | 207,870 | 103,935 | 25 |
| H6 | haiku | 12 | 12 | 67,840 | 5,653 | 6 |
| H7 | haiku | 40 | 40 | 57,819 | 1,445 | 4 |
| S3 | sonnet | 10 | 8 | 133,720 | 16,715 | 15 |
| H8 | haiku | 15 | 7 | 98,163 | 14,023 | 10 |
| H9 | haiku | 8 | 3 | 95,670 | 31,890 | 8 |
| O2 | opus | 4 | 4 | 122,629 | 30,657 | 12 |

### Escalations

- Opus matched 6 of 6 functions a cheaper model had failed.
- Sonnet matched 4 of 6 functions a cheaper model had failed.
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
- The game uses the compiler's own STL (`std::vector` at least). The vector
  destructor stubs registered with `atexit` (e.g. 0x438480) do not match yet:
  VC5's `~vector` leaves a dead stack store that the original lacks, whatever
  the element type tried so far.
