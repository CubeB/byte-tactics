# Agent calibration

Which model can decompile which functions, measured on real batches. Every
result here was re-verified with `tools/check.py` by the orchestrator; agents'
own claims are not counted. Raw per-function records are in `data/attempts.csv`.

## Overnight run, 26 to 27 September 2026: summary

The repository was created at 23:05. By 06:45 it held 1,938 matched
functions, 120,222 bytes, **14.13%** of Cavedog's code, in 134 commits. Every match was
re-verified by the orchestrator with `tools/check.py` before it was committed;
agents' own claims were never counted.

| Time | Matched |
| --- | ---: |
| 23:29 | 0.09% |
| 00:56 | 1.47% |
| 02:58 | 5.03% |
| 04:57 | 8.82% |
| 06:45 | 14.13% |

The rate rose from under 2% of the code per hour before 03:30 to about 2.5%
an hour after it, and about 3% in the last two hours, once the calibration
showed Opus was the cheapest model per match above 40 bytes and the work moved
to it.

### Which model for which functions

| Model | Batches | Functions matched | Bytes matched | Tokens | Cost units per matched byte |
| --- | ---: | ---: | ---: | ---: | ---: |
| Haiku | 29 | 522 of 666 (78%) | 8,273 | 2.9M | 346 |
| Sonnet | 26 | 216 of 241 (90%) | 8,882 | 4.3M | 963 |
| Opus | 108 | 1,188 of 1,203 (99%) | 102,355 | 17.0M | 665 |

Cost units weight tokens by each model's input price (Haiku 1, Sonnet 2, Opus 4:
$1, $2 and $4 per million input tokens). The token counts are the subagents'
own totals, so treat the absolute numbers as rough; the ratios are what matter.

- **Haiku** is reliable only on the smallest functions: 91% of 1-16 byte
  functions first time, 70% of 17-40 bytes, and under 30% above 40 bytes. It
  tends to give up after one or two check runs; telling it to make at least
  six different attempts raised one 17-40 byte batch from 7 of 20 to 15 of 20.
  Its drafts often repeat the same mistakes (a `__fastcall` free function
  instead of a method, `&DAT_00511de8` instead of loading the `g_game` pointer,
  invented vtable slot names), so every Haiku match needs checking.
- **Sonnet** matched about 88-90% of 17-64 byte functions first time and 94% of
  the functions Haiku failed on, at about 2.5 times Haiku's cost per matched
  byte.
- **Opus** matched 99-100% of everything up to 160 bytes and about 80% of
  161-260 byte functions, usually on the first check run. Because it needs far
  fewer tokens per function, it was cheaper per match than Sonnet from 41 bytes
  up and within about 1.5 times the Haiku-then-Sonnet pipeline at 17-40 bytes,
  with no second pass and cleaner code. Batches of 12-20 functions took 5-15
  minutes each.
- **Recommendation:** use Opus for everything above 16 bytes, in batches of
  12-20 functions with five agents at a time. Haiku is only worth it for the
  1-16 byte band, which is finished. For 161-260 bytes Opus costs about twice
  as much per matched byte as for 65-160 bytes.

### Remaining work

- Unattempted game functions: 0 of 1-16 bytes, 1 of 17-40 bytes, 0 of 41-64 bytes, 67 of 65-160 bytes, 669 of 161-400 bytes, 582 of over 400 bytes.
- Near-misses left partial (each with notes in its file): 0x419400, 0x4223e0, 0x438650, 0x485140, 0x49c880, 0x4ac970, 0x4c0a90, 0x4c1ab0, 0x4d1820, 0x4ddf00.
- Worth a human look: the Smacker video library is imported by ordinal, and
  the names the agents gave those imports are inferred from call sites (listed
  in the guide); and 0x415bb0 looks like a real Cavedog bug (a one-dword
  `new` where an array was meant), reproduced as written.
- `docs/consolidation.md` lists the naming and class-structure clean-up found
  along the way: classes known under several placeholder names, callee
  signatures that disagree between files, and two suspicious constructs.

### Tooling added during the night

- `tools/headers.py`: tries a function's file with all 128 combinations of seven
  common headers in a few seconds. MSVC 5's operand order and register choice
  depend on which headers are included, and this settled a dozen functions
  that no source rewrite could.
- `tools/checkall.py`: checks many functions in one parallel run.
- `tools/check.py`: vtable slots must use the names their functions already
  have (so the source could link), deleting destructors in a slot must be the
  known `??_G`, constants are compared only up to the next symbol, and scratch
  objects from different folders no longer overwrite each other.
- `tools/ctx.py` stops a vtable at the next vtable start, and
  `tools/functions.py` no longer cuts the last byte off a hand-written routine
  ending in `ret N`.
- Three class families that had been matched as unrelated placeholder
  functions with hand-stored vtable pointers (vtables 0x4fd428, 0x4fc980 and
  0x4fd5a8) are now real base and derived classes whose vtables the compiler
  emits, plus `Class_004b0610` and its derived class.

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

| Size (bytes) |  | Deepseek-v4.1-flash | Gpt-5.6-terra | Gpt-6 | Gpt-6-astra | Gpt-6-luna | Haiku | Longcat-2.5-preview-free | Mimo-v2.6-flash | Mimo-v2.6-pro | Nemotron-3.5-lightning-free | Opus | Sonnet | Sonnet-5.5 | Space-bunny-free |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| 1-16 |  |  |  |  |  |  | 283/312 (91%) |  |  |  |  | 3/3 (100%) |  |  |  |
| 17-40 |  |  |  |  |  |  | 235/337 (70%) |  |  |  |  | 124/124 (100%) | 9/10 (90%) |  |  |
| 41-64 |  |  |  |  |  |  | 3/11 (27%) |  |  |  |  | 255/255 (100%) | 96/109 (88%) |  |  |
| 65-160 |  | 64/67 (96%) |  |  |  |  | 1/6 (17%) |  |  |  |  | 739/747 (99%) | 2/6 (33%) |  |  |
| 161-400 | 0/10 (0%) | 271/350 (77%) |  | 17/18 (94%) |  |  |  |  |  |  |  | 30/36 (83%) |  |  | 204/291 (70%) |
| 401+ |  | 63/189 (33%) | 1/8 (12%) | 8/15 (53%) | 17/27 (63%) | 3/8 (38%) |  | 2/18 (11%) | 4/7 (57%) | 10/11 (91%) | 0/3 (0%) | 59/83 (71%) |  | 9/31 (29%) | 72/180 (40%) |

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
| S19 | sonnet | 10 | 10 | 148,532 | 14,853 | 30 | 15 |
| O37 | opus | 10 | 10 | 143,542 | 14,354 | 57 | 9 |
| H23 | haiku | 20 | 18 | 117,068 | 6,503 | 7 | 10 |
| S17 | sonnet | 10 | 9 | 238,260 | 26,473 | 53 | 31 |
| O38 | opus | 10 | 10 | 155,498 | 15,549 | 62 | 11 |
| H24 | haiku | 20 | 13 | 119,853 | 9,219 | 9 | 11 |
| O41 | opus | 10 | 10 | 137,103 | 13,710 | 55 | 8 |
| O40 | opus | 10 | 10 | 178,382 | 17,838 | 71 | 12 |
| O39 | opus | 10 | 9 | 206,883 | 22,987 | 92 | 19 |
| H25 | haiku | 20 | 11 | 87,162 | 7,923 | 8 | 7 |
| S20 | sonnet | 10 | 8 | 209,038 | 26,129 | 52 | 24 |
| O43 | opus | 10 | 10 | 111,365 | 11,136 | 45 | 5 |
| O42 | opus | 10 | 10 | 123,137 | 12,313 | 49 | 6 |
| S21 | sonnet | 10 | 10 | 162,874 | 16,287 | 33 | 16 |
| O45 | opus | 10 | 10 | 138,163 | 13,816 | 55 | 8 |
| H26 | haiku | 20 | 7 | 108,963 | 15,566 | 16 | 10 |
| S22 | sonnet | 9 | 9 | 157,991 | 17,554 | 35 | 15 |
| O46 | opus | 10 | 10 | 118,409 | 11,840 | 47 | 5 |
| S24 | sonnet | 13 | 13 | 118,024 | 9,078 | 18 | 8 |
| H27 | haiku | 20 | 15 | 110,784 | 7,385 | 7 | 8 |
| O44 | opus | 6 | 6 | 216,699 | 36,116 | 144 | 27 |
| O48 | opus | 10 | 10 | 133,943 | 13,394 | 54 | 7 |
| S23 | sonnet | 10 | 10 | 243,104 | 24,310 | 49 | 28 |
| O50 | opus | 10 | 10 | 131,679 | 13,167 | 53 | 7 |
| O49 | opus | 1 | 1 | 129,653 | 129,653 | 519 | 8 |
| S25 | sonnet | 5 | 4 | 194,637 | 48,659 | 97 | 23 |
| O47 | opus | 6 | 6 | 286,349 | 47,724 | 191 | 29 |
| O51 | opus | 10 | 10 | 147,536 | 14,753 | 59 | 9 |
| H28 | haiku | 20 | 13 | 113,874 | 8,759 | 9 | 9 |
| O54 | opus | 12 | 12 | 86,268 | 7,189 | 29 | 3 |
| O52 | opus | 10 | 10 | 136,623 | 13,662 | 55 | 8 |
| O55 | opus | 20 | 20 | 125,665 | 6,283 | 25 | 6 |
| S26 | sonnet | 7 | 7 | 115,916 | 16,559 | 33 | 8 |
| O57 | opus | 15 | 15 | 141,413 | 9,427 | 38 | 9 |
| O60 | opus | 19 | 19 | 114,190 | 6,010 | 24 | 5 |
| O56 | opus | 12 | 12 | 194,308 | 16,192 | 65 | 17 |
| O53 | opus | 8 | 7 | 280,128 | 40,018 | 160 | 28 |
| O58 | opus | 1 | 1 | 209,103 | 209,103 | 836 | 16 |
| O59 | opus | 12 | 12 | 175,548 | 14,629 | 59 | 11 |
| O63 | opus | 12 | 12 | 126,603 | 10,550 | 42 | 7 |
| O62 | opus | 15 | 15 | 142,065 | 9,471 | 38 | 8 |
| O65 | opus | 12 | 12 | 134,601 | 11,216 | 45 | 5 |
| O64 | opus | 13 | 13 | 165,681 | 12,744 | 51 | 10 |
| O67 | opus | 12 | 12 | 127,386 | 10,615 | 42 | 5 |
| O61 | opus | 12 | 12 | 194,580 | 16,215 | 65 | 16 |
| O68 | opus | 15 | 15 | 119,277 | 7,951 | 32 | 6 |
| O66 | opus | 3 | 3 | 176,033 | 58,677 | 235 | 11 |
| O72 | opus | 15 | 15 | 118,103 | 7,873 | 31 | 7 |
| O70 | opus | 12 | 12 | 161,892 | 13,491 | 54 | 9 |
| O71 | opus | 20 | 20 | 147,296 | 7,364 | 29 | 9 |
| O74 | opus | 15 | 15 | 122,926 | 8,195 | 33 | 5 |
| O77 | opus | 15 | 15 | 119,313 | 7,954 | 32 | 5 |
| O75 | opus | 12 | 12 | 155,052 | 12,921 | 52 | 9 |
| O76 | opus | 21 | 21 | 178,309 | 8,490 | 34 | 12 |
| O78 | opus | 15 | 15 | 123,353 | 8,223 | 33 | 6 |
| O79 | opus | 12 | 12 | 138,059 | 11,504 | 46 | 8 |
| O80 | opus | 20 | 20 | 135,270 | 6,763 | 27 | 7 |
| O81 | opus | 15 | 15 | 120,682 | 8,045 | 32 | 5 |
| O73 | opus | 12 | 12 | 284,437 | 23,703 | 95 | 29 |
| O69 | opus | 8 | 7 | 329,049 | 47,007 | 188 | 38 |
| O83 | opus | 20 | 20 | 126,577 | 6,328 | 25 | 6 |
| O82 | opus | 12 | 12 | 130,659 | 10,888 | 44 | 7 |
| O84 | opus | 15 | 15 | 125,377 | 8,358 | 33 | 5 |
| O86 | opus | 12 | 12 | 141,921 | 11,826 | 47 | 6 |
| O89 | opus | 15 | 15 | 129,608 | 8,640 | 35 | 6 |
| O87 | opus | 16 | 16 | 141,906 | 8,869 | 35 | 6 |
| O88 | opus | 12 | 12 | 186,168 | 15,514 | 62 | 11 |
| O91 | opus | 15 | 15 | 123,639 | 8,242 | 33 | 5 |
| O90 | opus | 12 | 12 | 145,517 | 12,126 | 49 | 8 |
| O85 | opus | 12 | 12 | 209,910 | 17,492 | 70 | 16 |
| O96 | opus | 15 | 15 | 128,774 | 8,584 | 34 | 6 |
| O95 | opus | 12 | 12 | 148,560 | 12,380 | 50 | 7 |
| O94 | opus | 12 | 12 | 162,340 | 13,528 | 54 | 8 |
| O97 | opus | 15 | 15 | 127,388 | 8,492 | 34 | 5 |
| O93 | opus | 12 | 11 | 206,622 | 18,783 | 75 | 17 |
| O100 | opus | 15 | 15 | 148,264 | 9,884 | 40 | 8 |
| O99 | opus | 12 | 12 | 156,216 | 13,018 | 52 | 11 |
| O101 | opus | 15 | 15 | 152,028 | 10,135 | 41 | 8 |
| O102 | opus | 14 | 14 | 163,941 | 11,710 | 47 | 10 |
| O103 | opus | 12 | 12 | 157,671 | 13,139 | 53 | 10 |
| O98 | opus | 12 | 11 | 242,463 | 22,042 | 88 | 21 |
| O105 | opus | 12 | 12 | 169,609 | 14,134 | 57 | 10 |
| O109 | opus | 12 | 12 | 151,739 | 12,644 | 51 | 8 |
| O106 | opus | 12 | 12 | 151,447 | 12,620 | 50 | 8 |
| O104 | opus | 12 | 11 | 250,983 | 22,816 | 91 | 21 |
| O107 | opus | 12 | 12 | 179,593 | 14,966 | 60 | 11 |
| O110 | opus | 12 | 12 | 162,056 | 13,504 | 54 | 10 |
| O111 | opus | 10 | 10 | 156,977 | 15,697 | 63 | 9 |
| O108 | opus | 12 | 11 | 248,708 | 22,609 | 90 | 24 |
| O112 | opus | 10 | 10 | 170,925 | 17,092 | 68 | 12 |
| #8 | gpt-6 | 6 | 6 | 0 | n/a | n/a | n/a |
| #1 | deepseek-v4.1-flash | 12 | 12 | 0 | n/a | n/a | n/a |
| #9 | gpt-6 | 6 | 5 | 0 | n/a | n/a | n/a |
| #43 | opus | 1 | 0 | 191,629 | n/a | n/a | 17 |
| #2 | deepseek-v4.1-flash | 12 | 12 | 0 | n/a | n/a | n/a |
| #35 | opus | 3 | 2 | 248,955 | 124,477 | 498 | n/a |
| #36 | opus | 3 | 3 | 345,400 | 115,133 | 461 | n/a |
| #37 | gpt-6-astra | 3 | 1 | 0 | n/a | n/a | n/a |
| #38 | gpt-6-astra | 3 | 2 | 0 | n/a | n/a | n/a |
| #39 | opus | 3 | 3 | 241,723 | 80,574 | 322 | 18 |
| #40 | gpt-6-astra | 3 | 2 | 0 | n/a | n/a | n/a |
| #53 | gpt-6-astra | 3 | 2 | 0 | n/a | n/a | n/a |
| #52 | opus | 2 | 1 | 243,080 | 243,080 | 972 | 23 |
| #3 | deepseek-v4.1-flash | 12 | 12 | 0 | n/a | n/a | n/a |
| #60 | opus | 1 | 1 | 173,428 | 173,428 | 694 | 11 |
| #63 | opus | 1 | 0 | 0 | n/a | n/a | n/a |
| #65 | opus | 1 | 0 | 0 | n/a | n/a | n/a |
| #4 | deepseek-v4.1-flash | 12 | 11 | 0 | n/a | n/a | n/a |
| #6 | deepseek-v4.1-flash | 7 | 7 | 0 | n/a | n/a | n/a |
| #55 | opus | 3 | 2 | 280,719 | 140,359 | 561 | 21 |
| #10 | deepseek-v4.1-flash | 6 | 6 | 0 | n/a | n/a | n/a |
| #54 | opus | 3 | 1 | 0 | n/a | n/a | n/a |
| #5 | deepseek-v4.1-flash | 12 | 10 | 0 | n/a | n/a | n/a |
| #72 | opus | 1 | 0 | 0 | n/a | n/a | n/a |
| #84 | opus | 2 | 2 | 116,379 | 58,189 | 233 | 5 |
| #56 | opus | 3 | 1 | 409,292 | 409,292 | 1637 | 45 |
| #11 | deepseek-v4.1-flash | 6 | 3 | 0 | n/a | n/a | n/a |
| #57 | opus | 3 | 2 | 298,804 | 149,402 | 598 | 26 |
| #82 | opus | 3 | 3 | 259,597 | 86,532 | 346 | 19 |
| #12 | deepseek-v4.1-flash | 6 | 4 | 0 | n/a | n/a | n/a |
| #15 | deepseek-v4.1-flash | 6 | 6 | 0 | n/a | n/a | n/a |
| #90 | opus | 3 | 2 | 271,979 | 135,989 | 544 | 31 |
| #16 | deepseek-v4.1-flash | 6 | 5 | 0 | n/a | n/a | n/a |
| #17 | deepseek-v4.1-flash | 6 | 4 | 0 | n/a | n/a | n/a |
| #81 | opus | 3 | 2 | 346,841 | 173,420 | 694 | 32 |
| #99 | opus | 2 | 2 | 164,661 | 82,330 | 329 | 8 |
| #80 | opus | 3 | 0 | 376,412 | n/a | n/a | 38 |
| #18 | deepseek-v4.1-flash | 6 | 6 | 0 | n/a | n/a | n/a |
| #103 | opus | 1 | 0 | 174,429 | n/a | n/a | 15 |
| #19 | deepseek-v4.1-flash | 6 | 4 | 0 | n/a | n/a | n/a |
| #106 | opus | 2 | 2 | 219,248 | 109,624 | 438 | 20 |
| #113 | opus | 2 | 2 | 127,022 | 63,511 | 254 | 13 |
| #20 | deepseek-v4.1-flash | 6 | 5 | 0 | n/a | n/a | n/a |
| #111 | opus | 2 | 2 | 242,823 | 121,411 | 486 | 23 |
| #13 | gpt-6 | 9 | 0 | 0 | n/a | n/a | n/a |
| #21 | deepseek-v4.1-flash | 6 | 6 | 0 | n/a | n/a | n/a |
| #7 | gpt-6 | 6 | 6 | 0 | n/a | n/a | n/a |
| #98 | opus | 3 | 1 | 444,190 | 444,190 | 1777 | 40 |
| #58 | gpt-6 | 3 | 3 | 0 | n/a | n/a | n/a |
| #24 | deepseek-v4.1-flash | 6 | 6 | 0 | n/a | n/a | n/a |
| #118 | opus | 1 | 1 | 149,998 | 149,998 | 600 | 16 |
| #97 | opus | 3 | 2 | 309,094 | 154,547 | 618 | 23 |
| #22 | deepseek-v4.1-flash | 6 | 5 | 0 | n/a | n/a | n/a |
| #27 | space-bunny-free | 6 | 6 | 0 | n/a | n/a | n/a |
| #30 | deepseek-v4.1-flash | 6 | 6 | 0 | n/a | n/a | n/a |
| #25 | space-bunny-free | 6 | 6 | 0 | n/a | n/a | n/a |
| #34 | space-bunny-free | 6 | 4 | 0 | n/a | n/a | n/a |
| #31 | space-bunny-free | 6 | 5 | 0 | n/a | n/a | n/a |
| #96 | opus | 3 | 2 | 334,017 | 167,008 | 668 | 34 |
| #26 | deepseek-v4.1-flash | 6 | 5 | 0 | n/a | n/a | n/a |
| #23 | deepseek-v4.1-flash | 6 | 4 | 0 | n/a | n/a | n/a |
| #123 | space-bunny-free | 6 | 6 | 0 | n/a | n/a | n/a |
| #146 | opus | 1 | 1 | 182,660 | 182,660 | 731 | 19 |
| #124 | deepseek-v4.1-flash | 6 | 6 | 0 | n/a | n/a | n/a |
| #130 | opus | 3 | 3 | 164,262 | 54,754 | 219 | 8 |
| #33 | deepseek-v4.1-flash | 6 | 6 | 0 | n/a | n/a | n/a |
| #143 | space-bunny-free | 6 | 6 | 0 | n/a | n/a | n/a |
| #126 | deepseek-v4.1-flash | 6 | 6 | 0 | n/a | n/a | n/a |
| #29 | space-bunny-free | 6 | 5 | 0 | n/a | n/a | n/a |
| #127 | deepseek-v4.1-flash | 6 | 6 | 0 | n/a | n/a | n/a |
| #160 | opus | 1 | 1 | 0 | n/a | n/a | n/a |
| #161 | opus | 2 | 2 | 128,383 | 64,191 | 257 | 13 |
| #136 | deepseek-v4.1-flash | 6 | 6 | 0 | n/a | n/a | n/a |
| #131 | opus | 3 | 2 | 464,597 | 232,298 | 929 | 51 |
| #28 | space-bunny-free | 6 | 5 | 0 | n/a | n/a | n/a |
| #32 | space-bunny-free | 6 | 4 | 0 | n/a | n/a | n/a |
| #78 | gpt-6-astra | 3 | 3 | 0 | n/a | n/a | n/a |
| #125 | space-bunny-free | 6 | 5 | 0 | n/a | n/a | n/a |
| #138 | deepseek-v4.1-flash | 6 | 5 | 0 | n/a | n/a | n/a |
| #79 | gpt-6-astra | 3 | 2 | 0 | n/a | n/a | n/a |
| #128 | deepseek-v4.1-flash | 6 | 4 | 0 | n/a | n/a | n/a |
| #164 | space-bunny-free | 6 | 6 | 0 | n/a | n/a | n/a |
| #165 | deepseek-v4.1-flash | 6 | 6 | 0 | n/a | n/a | n/a |
| #137 | space-bunny-free | 6 | 6 | 0 | n/a | n/a | n/a |
| #142 | space-bunny-free | 6 | 4 | 0 | n/a | n/a | n/a |
| #216 | opus | 2 | 2 | 61,991 | 30,995 | 124 | 3 |
| #95 | gpt-6-astra | 3 | 1 | 0 | n/a | n/a | n/a |
| #154 | space-bunny-free | 2 | 1 | 0 | n/a | n/a | n/a |
| #175 | opus | 3 | 2 | 413,723 | 206,861 | 827 | 41 |
| #167 | deepseek-v4.1-flash | 6 | 5 | 0 | n/a | n/a | n/a |
| #176 | opus | 3 | 2 | 426,973 | 213,486 | 854 | 44 |
| #139 | deepseek-v4.1-flash | 6 | 2 | 0 | n/a | n/a | n/a |
| #180 | deepseek-v4.1-flash | 1 | 1 | 0 | n/a | n/a | n/a |
| #129 | gpt-6-astra | 3 | 2 | 0 | n/a | n/a | n/a |
| #207 | space-bunny-free | 6 | 4 | 0 | n/a | n/a | n/a |
| #174 | gpt-6-astra | 3 | 2 | 0 | n/a | n/a | n/a |
| #195 | opus | 1 | 0 | 0 | n/a | n/a | n/a |
| #197 | opus | 2 | 1 | 0 | n/a | n/a | n/a |
| #223 | opus | 1 | 0 | 0 | n/a | n/a | n/a |
| #171 | deepseek-v4.1-flash | 6 | 2 | 0 | n/a | n/a | n/a |
| #169 | space-bunny-free | 6 | 4 | 0 | n/a | n/a | n/a |
| #77 | gpt-6 | 3 | 2 | 0 | n/a | n/a | n/a |
| #194 | deepseek-v4.1-flash | 1 | 1 | 0 | n/a | n/a | n/a |
| #198 | deepseek-v4.1-flash | 1 | 1 | 0 | n/a | n/a | n/a |
| #199 | deepseek-v4.1-flash | 1 | 1 | 0 | n/a | n/a | n/a |
| #186 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #201 | deepseek-v4.1-flash | 6 | 4 | 0 | n/a | n/a | n/a |
| #166 | space-bunny-free | 6 | 5 | 0 | n/a | n/a | n/a |
| #225 | opus | 1 | 1 | 169,017 | 169,017 | 676 | 14 |
| #141 | space-bunny-free | 6 | 5 | 0 | n/a | n/a | n/a |
| #228 | opus | 4 | 3 | 279,531 | 93,177 | 373 | 33 |
| #239 | opus | 4 | 1 | 0 | n/a | n/a | n/a |
| #249 | opus | 2 | 2 | 226,445 | 113,222 | 453 | 16 |
| #173 | space-bunny-free | 6 | 4 | 0 | n/a | n/a | n/a |
| #158 | space-bunny-free | 1 | 0 | 0 | n/a | n/a | n/a |
| #250 | space-bunny-free | 1 | 1 | 0 | n/a | n/a | n/a |
| #248 | opus | 1 | 1 | 0 | n/a | n/a | n/a |
| #211 | opus | 3 | 3 | 253,234 | 84,411 | 338 | 21 |
| #210 | opus | 3 | 3 | 215,073 | 71,691 | 287 | 18 |
| #253 | space-bunny-free | 6 | 6 | 0 | n/a | n/a | n/a |
| #267 | opus | 1 | 1 | 88,916 | 88,916 | 356 | 12 |
| #209 | opus | 3 | 3 | 0 | n/a | n/a | n/a |
| #189 | space-bunny-free | 2 | 1 | 0 | n/a | n/a | n/a |
| #140 | deepseek-v4.1-flash | 6 | 1 | 0 | n/a | n/a | n/a |
| #240 | space-bunny-free | 2 | 2 | 0 | n/a | n/a | n/a |
| #252 | space-bunny-free | 1 | 1 | 0 | n/a | n/a | n/a |
| #259 | space-bunny-free | 6 | 5 | 0 | n/a | n/a | n/a |
| #206 | space-bunny-free | 6 | 4 | 0 | n/a | n/a | n/a |
| #256 | space-bunny-free | 6 | 3 | 0 | n/a | n/a | n/a |
| #294 | opus | 1 | 1 | 168,436 | 168,436 | 674 | 11 |
| #254 | space-bunny-free | 6 | 4 | 0 | n/a | n/a | n/a |
| #200 | space-bunny-free | 6 | 3 | 0 | n/a | n/a | n/a |
| #221 | space-bunny-free | 2 | 0 | 0 | n/a | n/a | n/a |
| #205 | space-bunny-free | 6 | 3 | 0 | n/a | n/a | n/a |
| #232 | space-bunny-free | 2 | 1 | 0 | n/a | n/a | n/a |
| #246 | space-bunny-free | 5 | 3 | 0 | n/a | n/a | n/a |
| #247 | space-bunny-free | 5 | 2 | 0 | n/a | n/a | n/a |
| #260 | space-bunny-free | 6 | 6 | 0 | n/a | n/a | n/a |
| #255 | space-bunny-free | 6 | 2 | 0 | n/a | n/a | n/a |
| #257 | space-bunny-free | 6 | 1 | 0 | n/a | n/a | n/a |
| #258 | space-bunny-free | 6 | 2 | 0 | n/a | n/a | n/a |
| #264 | space-bunny-free | 2 | 0 | 0 | n/a | n/a | n/a |
| #208 | opus | 3 | 2 | 0 | n/a | n/a | n/a |
| #307 | space-bunny-free | 1 | 0 | 0 | n/a | n/a | n/a |
| #276 | opus | 3 | 1 | 442,727 | 442,727 | 1771 | 47 |
| #202 | space-bunny-free | 6 | 5 | 0 | n/a | n/a | n/a |
| #203 | space-bunny-free | 6 | 3 | 0 | n/a | n/a | n/a |
| #204 | space-bunny-free | 6 | 3 | 0 | n/a | n/a | n/a |
| #330 | opus | 2 | 2 | 75,560 | 37,780 | 151 | 4 |
| #321 | opus | 2 | 2 | 219,335 | 109,667 | 439 | 21 |
| #343 | space-bunny-free | 3 | 3 | 0 | n/a | n/a | n/a |
| #342 | space-bunny-free | 3 | 3 | 0 | n/a | n/a | n/a |
| #333 | opus | 1 | 1 | 0 | n/a | n/a | n/a |
| #295 | opus | 5 | 3 | 337,044 | 112,348 | 449 | 35 |
| #340 | space-bunny-free | 1 | 0 | 0 | n/a | n/a | n/a |
| #279 | space-bunny-free | 6 | 1 | 0 | n/a | n/a | n/a |
| #278 | space-bunny-free | 6 | 2 | 0 | n/a | n/a | n/a |
| #280 | space-bunny-free | 6 | 1 | 0 | n/a | n/a | n/a |
| #324 | opus | 1 | 0 | 218,587 | n/a | n/a | 24 |
| #287 | space-bunny-free | 6 | 6 | 0 | n/a | n/a | n/a |
| #349 | opus | 1 | 0 | 0 | n/a | n/a | n/a |
| #277 | space-bunny-free | 6 | 6 | 0 | n/a | n/a | n/a |
| #282 | deepseek-v4.1-flash | 6 | 6 | 0 | n/a | n/a | n/a |
| #274 | opus | 3 | 3 | 225,742 | 75,247 | 301 | 20 |
| #275 | opus | 3 | 3 | 463,278 | 154,426 | 618 | 48 |
| #281 | space-bunny-free | 6 | 4 | 0 | n/a | n/a | n/a |
| #172 | space-bunny-free | 6 | 6 | 0 | n/a | n/a | n/a |
| #284 | deepseek-v4.1-flash | 6 | 3 | 0 | n/a | n/a | n/a |
| #288 | space-bunny-free | 6 | 5 | 0 | n/a | n/a | n/a |
| #283 | deepseek-v4.1-flash | 6 | 4 | 0 | n/a | n/a | n/a |
| #273 | opus | 3 | 2 | 0 | n/a | n/a | n/a |
| #353 | space-bunny-free | 5 | 3 | 0 | n/a | n/a | n/a |
| #358 | space-bunny-free | 5 | 4 | 0 | n/a | n/a | n/a |
| #428 | opus | 3 | 2 | 323,510 | 161,755 | 647 | 31 |
| #357 | space-bunny-free | 4 | 0 | 0 | n/a | n/a | n/a |
| #420 | space-bunny-free | 2 | 1 | 0 | n/a | n/a | n/a |
| #285 | deepseek-v4.1-flash | 6 | 4 | 0 | n/a | n/a | n/a |
| #438 | space-bunny-free | 2 | 1 | 0 | n/a | n/a | n/a |
| #436 | opus | 2 | 2 | 0 | n/a | n/a | n/a |
| #431 | opus | 3 | 3 | 0 | n/a | n/a | n/a |
| #389 | deepseek-v4.1-flash | 6 | 5 | 0 | n/a | n/a | n/a |
| #286 | space-bunny-free | 6 | 5 | 0 | n/a | n/a | n/a |
| #427 | opus | 3 | 2 | 435,916 | 217,958 | 872 | 48 |
| #433 | space-bunny-free | 1 | 0 | 0 | n/a | n/a | n/a |
| #311 | space-bunny-free | 2 | 0 | 0 | n/a | n/a | n/a |
| #312 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #323 | space-bunny-free | 3 | 0 | 0 | n/a | n/a | n/a |
| #467 | space-bunny-free | 2 | 2 | 0 | n/a | n/a | n/a |
| #317 | deepseek-v4.1-flash | 2 | 1 | 0 | n/a | n/a | n/a |
| #449 | space-bunny-free | 2 | 1 | 0 | n/a | n/a | n/a |
| #415 | space-bunny-free | 4 | 3 | 0 | n/a | n/a | n/a |
| #372 | space-bunny-free | 6 | 4 | 0 | n/a | n/a | n/a |
| #418 | space-bunny-free | 4 | 3 | 0 | n/a | n/a | n/a |
| #319 | deepseek-v4.1-flash | 3 | 3 | 0 | n/a | n/a | n/a |
| #419 | space-bunny-free | 4 | 1 | 0 | n/a | n/a | n/a |
| #414 | space-bunny-free | 4 | 0 | 0 | n/a | n/a | n/a |
| #417 | space-bunny-free | 4 | 4 | 0 | n/a | n/a | n/a |
| #386 | space-bunny-free | 6 | 4 | 0 | n/a | n/a | n/a |
| #325 | deepseek-v4.1-flash | 2 | 1 | 0 | n/a | n/a | n/a |
| #408 | space-bunny-free | 4 | 2 | 0 | n/a | n/a | n/a |
| #411 | space-bunny-free | 4 | 2 | 0 | n/a | n/a | n/a |
| #416 | space-bunny-free | 4 | 0 | 0 | n/a | n/a | n/a |
| #326 | deepseek-v4.1-flash | 3 | 1 | 0 | n/a | n/a | n/a |
| #271 | gpt-6 | 3 | 2 | 0 | n/a | n/a | n/a |
| #474 | space-bunny-free | 2 | 0 | 0 | n/a | n/a | n/a |
| #478 | space-bunny-free | 1 | 0 | 0 | n/a | n/a | n/a |
| #473 | space-bunny-free | 1 | 0 | 0 | n/a | n/a | n/a |
| #498 | space-bunny-free | 4 | 2 | 0 | n/a | n/a | n/a |
| #378 | space-bunny-free | 6 | 5 | 0 | n/a | n/a | n/a |
| #412 | space-bunny-free | 4 | 2 | 0 | n/a | n/a | n/a |
| #502 | space-bunny-free | 4 | 3 | 0 | n/a | n/a | n/a |
| #495 | space-bunny-free | 4 | 2 | 0 | n/a | n/a | n/a |
| #500 | mimo-v2.6-flash | 4 | 2 | 0 | n/a | n/a | n/a |
| #501 | mimo-v2.6-flash | 4 | 3 | 0 | n/a | n/a | n/a |
| #413 | space-bunny-free | 4 | 1 | 0 | n/a | n/a | n/a |
| #540 | mimo-v2.6-flash | 1 | 1 | 0 | n/a | n/a | n/a |
| #525 | mimo-v2.6-flash | 2 | 1 | 0 | n/a | n/a | n/a |
| #523 | space-bunny-free | 1 | 1 | 0 | n/a | n/a | n/a |
| #527 | space-bunny-free | 1 | 0 | 0 | n/a | n/a | n/a |
| #328 | space-bunny-free | 5 | 2 | 0 | n/a | n/a | n/a |
| #373 | deepseek-v4.1-flash | 6 | 1 | 0 | n/a | n/a | n/a |
| #519 | space-bunny-free | 2 | 0 | 0 | n/a | n/a | n/a |
| #327 | space-bunny-free | 4 | 1 | 0 | n/a | n/a | n/a |
| #375 | deepseek-v4.1-flash | 6 | 4 | 0 | n/a | n/a | n/a |
| #542 | space-bunny-free | 3 | 0 | 0 | n/a | n/a | n/a |
| #572 | space-bunny-free | 2 | 2 | 0 | n/a | n/a | n/a |
| #371 | deepseek-v4.1-flash | 6 | 5 | 0 | n/a | n/a | n/a |
| #559 | space-bunny-free | 4 | 3 | 0 | n/a | n/a | n/a |
| #363 | deepseek-v4.1-flash | 6 | 5 | 0 | n/a | n/a | n/a |
| #402 | space-bunny-free | 4 | 2 | 0 | n/a | n/a | n/a |
| #564 | space-bunny-free | 5 | 1 | 0 | n/a | n/a | n/a |
| #329 | space-bunny-free | 4 | 1 | 0 | n/a | n/a | n/a |
| #387 | deepseek-v4.1-flash | 6 | 4 | 0 | n/a | n/a | n/a |
| #374 | deepseek-v4.1-flash | 6 | 4 | 0 | n/a | n/a | n/a |
| #562 | space-bunny-free | 4 | 4 | 0 | n/a | n/a | n/a |
| #536 | mimo-v2.6-flash | 2 | 1 | 0 | n/a | n/a | n/a |
| #369 | deepseek-v4.1-flash | 6 | 6 | 0 | n/a | n/a | n/a |
| #511 | space-bunny-free | 4 | 1 | 0 | n/a | n/a | n/a |
| #388 | deepseek-v4.1-flash | 6 | 6 | 0 | n/a | n/a | n/a |
| #368 | deepseek-v4.1-flash | 6 | 6 | 0 | n/a | n/a | n/a |
| #364 | deepseek-v4.1-flash | 6 | 5 | 0 | n/a | n/a | n/a |
| #556 | space-bunny-free | 4 | 3 | 0 | n/a | n/a | n/a |
| #366 | deepseek-v4.1-flash | 6 | 6 | 0 | n/a | n/a | n/a |
| #585 | space-bunny-free | 1 | 0 | 0 | n/a | n/a | n/a |
| #390 | deepseek-v4.1-flash | 6 | 5 | 0 | n/a | n/a | n/a |
| #592 | space-bunny-free | 2 | 0 | 0 | n/a | n/a | n/a |
| #401 | space-bunny-free | 4 | 2 | 0 | n/a | n/a | n/a |
| #380 | deepseek-v4.1-flash | 6 | 2 | 0 | n/a | n/a | n/a |
| #611 | space-bunny-free | 4 | 1 | 0 | n/a | n/a | n/a |
| #404 | deepseek-v4.1-flash | 4 | 3 | 0 | n/a | n/a | n/a |
| #379 | deepseek-v4.1-flash | 6 | 3 | 0 | n/a | n/a | n/a |
| #399 | space-bunny-free | 4 | 1 | 0 | n/a | n/a | n/a |
| #365 | deepseek-v4.1-flash | 6 | 2 | 0 | n/a | n/a | n/a |
| #407 | deepseek-v4.1-flash | 4 | 3 | 0 | n/a | n/a | n/a |
| #381 | deepseek-v4.1-flash | 6 | 5 | 0 | n/a | n/a | n/a |
| #398 | space-bunny-free | 3 | 2 | 0 | n/a | n/a | n/a |
| #642 | space-bunny-free | 3 | 0 | 0 | n/a | n/a | n/a |
| #560 | longcat-2.5-preview-free | 4 | 2 | 0 | n/a | n/a | n/a |
| #376 | deepseek-v4.1-flash | 6 | 5 | 0 | n/a | n/a | n/a |
| #646 | space-bunny-free | 4 | 3 | 0 | n/a | n/a | n/a |
| #656 | space-bunny-free | 1 | 1 | 0 | n/a | n/a | n/a |
| #367 | deepseek-v4.1-flash | 6 | 3 | 0 | n/a | n/a | n/a |
| #377 | deepseek-v4.1-flash | 6 | 6 | 0 | n/a | n/a | n/a |
| #634 | space-bunny-free | 1 | 1 | 0 | n/a | n/a | n/a |
| #370 | deepseek-v4.1-flash | 6 | 6 | 0 | n/a | n/a | n/a |
| #384 | deepseek-v4.1-flash | 6 | 5 | 0 | n/a | n/a | n/a |
| #652 | space-bunny-free | 1 | 0 | 0 | n/a | n/a | n/a |
| #613 | space-bunny-free | 4 | 2 | 0 | n/a | n/a | n/a |
| #650 | deepseek-v4.1-flash | 1 | 1 | 0 | n/a | n/a | n/a |
| #616 | space-bunny-free | 1 | 0 | 0 | n/a | n/a | n/a |
| #382 | deepseek-v4.1-flash | 6 | 5 | 0 | n/a | n/a | n/a |
| #385 | deepseek-v4.1-flash | 6 | 4 | 0 | n/a | n/a | n/a |
| #629 | space-bunny-free | 4 | 0 | 0 | n/a | n/a | n/a |
| #391 | deepseek-v4.1-flash | 6 | 4 | 0 | n/a | n/a | n/a |
| #558 | longcat-2.5-preview-free | 4 | 0 | 0 | n/a | n/a | n/a |
| #561 | longcat-2.5-preview-free | 4 | 0 | 0 | n/a | n/a | n/a |
| #392 | space-bunny-free | 6 | 2 | 0 | n/a | n/a | n/a |
| #396 | space-bunny-free | 6 | 4 | 0 | n/a | n/a | n/a |
| #586 | space-bunny-free | 2 | 0 | 0 | n/a | n/a | n/a |
| #397 | deepseek-v4.1-flash | 6 | 6 | 0 | n/a | n/a | n/a |
| #627 | space-bunny-free | 2 | 1 | 0 | n/a | n/a | n/a |
| #400 | deepseek-v4.1-flash | 4 | 4 | 0 | n/a | n/a | n/a |
| #637 | space-bunny-free | 3 | 2 | 0 | n/a | n/a | n/a |
| #405 | deepseek-v4.1-flash | 4 | 2 | 0 | n/a | n/a | n/a |
| #403 | deepseek-v4.1-flash | 4 | 2 | 0 | n/a | n/a | n/a |
| #614 | space-bunny-free | 4 | 0 | 0 | n/a | n/a | n/a |
| #612 | space-bunny-free | 4 | 1 | 0 | n/a | n/a | n/a |
| #608 | space-bunny-free | 4 | 2 | 0 | n/a | n/a | n/a |
| #446 | deepseek-v4.1-flash | 1 | 1 | 0 | n/a | n/a | n/a |
| #406 | deepseek-v4.1-flash | 4 | 3 | 0 | n/a | n/a | n/a |
| #461 | deepseek-v4.1-flash | 1 | 1 | 0 | n/a | n/a | n/a |
| #472 | deepseek-v4.1-flash | 1 | 1 | 0 | n/a | n/a | n/a |
| #493 | mimo-v2.6-pro | 4 | 4 | 0 | n/a | n/a | n/a |
| #470 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #440 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #463 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #409 | gpt-6-luna | 4 | 1 | 0 | n/a | n/a | n/a |
| #453 | deepseek-v4.1-flash | 1 | 1 | 0 | n/a | n/a | n/a |
| #707 | opus | 1 | 0 | 0 | n/a | n/a | n/a |
| #445 | deepseek-v4.1-flash | 4 | 1 | 0 | n/a | n/a | n/a |
| #727 | deepseek-v4.1-flash | 2 | 1 | 0 | n/a | n/a | n/a |
| #728 | space-bunny-free | 2 | 0 | 0 | n/a | n/a | n/a |
| #736 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #447 | deepseek-v4.1-flash | 2 | 1 | 0 | n/a | n/a | n/a |
| #458 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #580 | longcat-2.5-preview-free | 1 | 1 | 0 | n/a | n/a | n/a |
| #730 | space-bunny-free | 3 | 3 | 0 | n/a | n/a | n/a |
| #717 | mimo-v2.6-pro | 3 | 2 | 0 | n/a | n/a | n/a |
| #718 | deepseek-v4.1-flash | 3 | 1 | 0 | n/a | n/a | n/a |
| #466 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #632 | space-bunny-free | 3 | 1 | 0 | n/a | n/a | n/a |
| #486 | deepseek-v4.1-flash | 3 | 2 | 0 | n/a | n/a | n/a |
| #462 | space-bunny-free | 2 | 0 | 0 | n/a | n/a | n/a |
| #729 | space-bunny-free | 4 | 3 | 0 | n/a | n/a | n/a |
| #489 | deepseek-v4.1-flash | 2 | 1 | 0 | n/a | n/a | n/a |
| #711 | deepseek-v4.1-flash | 3 | 1 | 0 | n/a | n/a | n/a |
| #491 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #719 | deepseek-v4.1-flash | 3 | 1 | 0 | n/a | n/a | n/a |
| #714 | space-bunny-free | 3 | 0 | 0 | n/a | n/a | n/a |
| #607 | mimo-v2.6-pro | 4 | 4 | 0 | n/a | n/a | n/a |
| #487 | deepseek-v4.1-flash | 4 | 2 | 0 | n/a | n/a | n/a |
| #734 | space-bunny-free | 2 | 0 | 0 | n/a | n/a | n/a |
| #716 | space-bunny-free | 3 | 2 | 0 | n/a | n/a | n/a |
| #686 | longcat-2.5-preview-free | 1 | 1 | 0 | n/a | n/a | n/a |
| #610 | space-bunny-free | 4 | 3 | 0 | n/a | n/a | n/a |
| #492 | deepseek-v4.1-flash | 2 | 2 | 0 | n/a | n/a | n/a |
| #494 | gpt-6-luna | 4 | 2 | 0 | n/a | n/a | n/a |
| #715 | space-bunny-free | 3 | 1 | 0 | n/a | n/a | n/a |
| #496 | deepseek-v4.1-flash | 4 | 3 | 0 | n/a | n/a | n/a |
| #713 | space-bunny-free | 3 | 1 | 0 | n/a | n/a | n/a |
| #800 | space-bunny-free | 2 | 1 | 0 | n/a | n/a | n/a |
| #497 | deepseek-v4.1-flash | 4 | 4 | 0 | n/a | n/a | n/a |
| #712 | deepseek-v4.1-flash | 3 | 1 | 0 | n/a | n/a | n/a |
| #797 | space-bunny-free | 2 | 0 | 0 | n/a | n/a | n/a |
| #697 | gpt-5.6-terra | 4 | 2 | 0 | n/a | n/a | n/a |
| #789 | space-bunny-free | 3 | 1 | 0 | n/a | n/a | n/a |
| #685 | longcat-2.5-preview-free | 3 | 1 | 0 | n/a | n/a | n/a |
| #720 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #721 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #513 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #499 | deepseek-v4.1-flash | 4 | 3 | 0 | n/a | n/a | n/a |
| #522 | gpt-5.6-terra | 1 | 0 | 0 | n/a | n/a | n/a |
| #825 | space-bunny-free | 1 | 1 | 0 | n/a | n/a | n/a |
| #815 | space-bunny-free | 1 | 1 | 0 | n/a | n/a | n/a |
| #819 | space-bunny-free | 2 | 1 | 0 | n/a | n/a | n/a |
| #535 | gpt-5.6-terra | 1 | 0 | 0 | n/a | n/a | n/a |
| #509 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #537 | gpt-5.6-terra | 2 | 0 | 0 | n/a | n/a | n/a |
| #609 | space-bunny-free | 4 | 1 | 0 | n/a | n/a | n/a |
| #555 | gpt-5.6-terra | 4 | 0 | 0 | n/a | n/a | n/a |
| #531 | deepseek-v4.1-flash | 2 | 1 | 0 | n/a | n/a | n/a |
| #557 | gpt-5.6-terra | 4 | 1 | 0 | n/a | n/a | n/a |
| #581 | deepseek-v4.1-flash | 1 | 1 | 0 | n/a | n/a | n/a |
| #724 | space-bunny-free | 3 | 1 | 0 | n/a | n/a | n/a |
| #780 | longcat-2.5-preview-free | 3 | 0 | 0 | n/a | n/a | n/a |
| #725 | nemotron-3.5-lightning-free | 3 | 0 | 0 | n/a | n/a | n/a |
| #605 | gpt-5.6-terra | 1 | 0 | 0 | n/a | n/a | n/a |
| #831 | space-bunny-free | 2 | 1 | 0 | n/a | n/a | n/a |
| #623 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #681 | gpt-5.6-terra | 1 | 0 | 0 | n/a | n/a | n/a |
| #692 | gpt-5.6-terra | 2 | 0 | 0 | n/a | n/a | n/a |
| #691 | deepseek-v4.1-flash | 1 | 1 | 0 | n/a | n/a | n/a |
| #851 | space-bunny-free | 3 | 0 | 0 | n/a | n/a | n/a |
| #852 | space-bunny-free | 3 | 0 | 0 | n/a | n/a | n/a |
| #698 | gpt-5.6-terra | 4 | 1 | 0 | n/a | n/a | n/a |
| #699 | longcat-2.5-preview-free | 4 | 0 | 0 | n/a | n/a | n/a |
| #700 | deepseek-v4.1-flash | 2 | 2 | 0 | n/a | n/a | n/a |
| #859 | space-bunny-free | 3 | 1 | 0 | n/a | n/a | n/a |
| #774 | deepseek-v4.1-flash | 2 | 1 | 0 | n/a | n/a | n/a |
| #773 | space-bunny-free | 1 | 0 | 0 | n/a | n/a | n/a |
| #595 | deepseek-v4.1-flash | 2 | 1 | 0 | n/a | n/a | n/a |
| #781 | deepseek-v4.1-flash | 3 | 2 | 0 | n/a | n/a | n/a |
| #696 | deepseek-v4.1-flash | 2 | 1 | 0 | n/a | n/a | n/a |
| #861 | space-bunny-free | 3 | 0 | 0 | n/a | n/a | n/a |
| #425 | space-bunny-free | 3 | 3 | 0 | n/a | n/a | n/a |
| #749 | space-bunny-free | 3 | 1 | 0 | n/a | n/a | n/a |
| #782 | deepseek-v4.1-flash | 3 | 1 | 0 | n/a | n/a | n/a |
| #901 | space-bunny-free | 3 | 0 | 0 | n/a | n/a | n/a |
| #899 | space-bunny-free | 3 | 1 | 0 | n/a | n/a | n/a |
| #882 | space-bunny-free | 2 | 0 | 0 | n/a | n/a | n/a |
| #855 | space-bunny-free | 3 | 0 | 0 | n/a | n/a | n/a |
| #848 | sonnet-5.5 | 1 | 1 | 0 | n/a | n/a | n/a |
| #783 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #870 | space-bunny-free | 3 | 0 | 0 | n/a | n/a | n/a |
| #788 | sonnet-5.5 | 3 | 2 | 0 | n/a | n/a | n/a |
| #873 | space-bunny-free | 3 | 0 | 0 | n/a | n/a | n/a |
| #860 | space-bunny-free | 3 | 1 | 0 | n/a | n/a | n/a |
| #853 | sonnet-5.5 | 3 | 2 | 0 | n/a | n/a | n/a |
| #934 | space-bunny-free | 3 | 0 | 0 | n/a | n/a | n/a |
| #785 | deepseek-v4.1-flash | 3 | 2 | 0 | n/a | n/a | n/a |
| #786 | deepseek-v4.1-flash | 3 | 1 | 0 | n/a | n/a | n/a |
| #787 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #926 | space-bunny-free | 3 | 0 | 0 | n/a | n/a | n/a |
| #854 | sonnet-5.5 | 3 | 0 | 0 | n/a | n/a | n/a |
| #784 | deepseek-v4.1-flash | 3 | 3 | 0 | n/a | n/a | n/a |
| #810 | deepseek-v4.1-flash | 1 | 1 | 0 | n/a | n/a | n/a |
| #857 | sonnet-5.5 | 3 | 0 | 0 | n/a | n/a | n/a |
| #963 | space-bunny-free | 1 | 1 | 0 | n/a | n/a | n/a |
| #803 | deepseek-v4.1-flash | 3 | 1 | 0 | n/a | n/a | n/a |
| #822 | deepseek-v4.1-flash | 2 | 1 | 0 | n/a | n/a | n/a |
| #837 | deepseek-v4.1-flash | 2 | 2 | 0 | n/a | n/a | n/a |
| #927 | sonnet-5.5 | 3 | 1 | 0 | n/a | n/a | n/a |
| #858 | space-bunny-free | 3 | 0 | 0 | n/a | n/a | n/a |
| #877 | deepseek-v4.1-flash | 3 | 2 | 0 | n/a | n/a | n/a |
| #844 | deepseek-v4.1-flash | 3 | 2 | 0 | n/a | n/a | n/a |
| #842 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #827 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #928 | sonnet-5.5 | 3 | 0 | 0 | n/a | n/a | n/a |
| #961 | space-bunny-free | 3 | 1 | 0 | n/a | n/a | n/a |
| #962 | space-bunny-free | 3 | 0 | 0 | n/a | n/a | n/a |
| #951 | space-bunny-free | 3 | 0 | 0 | n/a | n/a | n/a |
| #931 | sonnet-5.5 | 3 | 2 | 0 | n/a | n/a | n/a |
| #940 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #960 | space-bunny-free | 3 | 2 | 0 | n/a | n/a | n/a |
| #1018 | sonnet-5.5 | 3 | 0 | 0 | n/a | n/a | n/a |
| #907 | space-bunny-free | 2 | 0 | 0 | n/a | n/a | n/a |
| #885 | space-bunny-free | 3 | 3 | 0 | n/a | n/a | n/a |
| #957 | space-bunny-free | 1 | 1 | 0 | n/a | n/a | n/a |
| #924 | space-bunny-free | 3 | 0 | 0 | n/a | n/a | n/a |
| #930 | space-bunny-free | 3 | 0 | 0 | n/a | n/a | n/a |
| #933 | space-bunny-free | 3 | 0 | 0 | n/a | n/a | n/a |
| #932 | space-bunny-free | 3 | 1 | 0 | n/a | n/a | n/a |
| #953 | space-bunny-free | 3 | 0 | 0 | n/a | n/a | n/a |
| #917 | space-bunny-free | 1 | 1 | 0 | n/a | n/a | n/a |
| #874 | space-bunny-free | 4 | 1 | 0 | n/a | n/a | n/a |
| #954 | space-bunny-free | 3 | 1 | 0 | n/a | n/a | n/a |
| #923 | space-bunny-free | 3 | 0 | 0 | n/a | n/a | n/a |
| #925 | space-bunny-free | 3 | 2 | 0 | n/a | n/a | n/a |
| #883 | deepseek-v4.1-flash | 3 | 1 | 0 | n/a | n/a | n/a |
| #929 | sonnet-5.5 | 3 | 2 | 0 | n/a | n/a | n/a |
| #950 | muse-spark-1.3-free | 2 | 0 | 0 | n/a | n/a | n/a |
| #946 | muse-spark-1.3-free | 2 | 0 | 0 | n/a | n/a | n/a |
| #1073 | space-bunny-free | 1 | 1 | 0 | n/a | n/a | n/a |
| #1050 | space-bunny-free | 1 | 1 | 0 | n/a | n/a | n/a |
| #549 | deepseek-v4.1-flash | 1 | 1 | 0 | n/a | n/a | n/a |
| #1060 | space-bunny-free | 1 | 0 | 0 | n/a | n/a | n/a |
| #1057 | space-bunny-free | 3 | 2 | 0 | n/a | n/a | n/a |
| #1070 | space-bunny-free | 3 | 0 | 0 | n/a | n/a | n/a |
| #1061 | space-bunny-free | 2 | 0 | 0 | n/a | n/a | n/a |
| #1072 | space-bunny-free | 2 | 0 | 0 | n/a | n/a | n/a |
| #553 | space-bunny-free | 1 | 0 | 0 | n/a | n/a | n/a |
| #1082 | sonnet-5.5 | 3 | 1 | 0 | n/a | n/a | n/a |
| #1067 | space-bunny-free | 3 | 0 | 0 | n/a | n/a | n/a |
| #566 | space-bunny-free | 2 | 0 | 0 | n/a | n/a | n/a |
| #571 | space-bunny-free | 3 | 0 | 0 | n/a | n/a | n/a |
| #894 | sonnet-5.5 | 2 | 2 | 0 | n/a | n/a | n/a |
| #574 | sonnet-5.5 | 3 | 1 | 0 | n/a | n/a | n/a |
| #589 | space-bunny-free | 4 | 0 | 0 | n/a | n/a | n/a |
| #1098 | sonnet-5.5 | 2 | 2 | 0 | n/a | n/a | n/a |
| #939 | space-bunny-free | 2 | 0 | 0 | n/a | n/a | n/a |
| #911 | space-bunny-free | 1 | 0 | 0 | n/a | n/a | n/a |
| #591 | space-bunny-free | 3 | 0 | 0 | n/a | n/a | n/a |
| #846 | sonnet-5.5 | 2 | 2 | 0 | n/a | n/a | n/a |
| #599 | sonnet-5.5 | 1 | 1 | 0 | n/a | n/a | n/a |
| #679 | sonnet-5.5 | 3 | 1 | 0 | n/a | n/a | n/a |
| #1103 | space-bunny-free | 1 | 0 | 0 | n/a | n/a | n/a |
| #959 | sonnet-5.5 | 3 | 2 | 0 | n/a | n/a | n/a |
| #778 | longcat-2.5-preview-free | 1 | 0 | 0 | n/a | n/a | n/a |
| #601 | space-bunny-free | 3 | 0 | 0 | n/a | n/a | n/a |
| #1086 | space-bunny-free | 1 | 1 | 0 | n/a | n/a | n/a |
| #1105 | sonnet-5.5 | 3 | 1 | 0 | n/a | n/a | n/a |
| #1093 | deepseek-v4.1-flash | 1 | 1 | 0 | n/a | n/a | n/a |
| #759 | sonnet-5.5 | 1 | 0 | 0 | n/a | n/a | n/a |
| #554 | sonnet-5.5 | 3 | 0 | 0 | n/a | n/a | n/a |
| #687 | sonnet-5.5 | 1 | 1 | 0 | n/a | n/a | n/a |
| #1076 | sonnet-5.5 | 3 | 2 | 0 | n/a | n/a | n/a |
| #1071 | deepseek-v4.1-flash | 1 | 1 | 0 | n/a | n/a | n/a |
| #618 | sonnet-5.5 | 1 | 1 | 0 | n/a | n/a | n/a |
| #1123 | sonnet-5.5 | 1 | 1 | 0 | n/a | n/a | n/a |
| #779 | longcat-2.5-preview-free | 2 | 0 | 0 | n/a | n/a | n/a |
| #680 | space-bunny-free | 2 | 1 | 0 | n/a | n/a | n/a |
| #688 | space-bunny-free | 2 | 1 | 0 | n/a | n/a | n/a |
| #1135 | sonnet-5.5 | 1 | 0 | 0 | n/a | n/a | n/a |
| #1107 | sonnet-5.5 | 2 | 1 | 0 | n/a | n/a | n/a |
| #1124 | sonnet-5.5 | 3 | 2 | 0 | n/a | n/a | n/a |
| #903 | deepseek-v4.1-flash | 3 | 1 | 0 | n/a | n/a | n/a |
| #1083 | deepseek-v4.1-flash | 2 | 1 | 0 | n/a | n/a | n/a |
| #1053 | space-bunny-free | 1 | 0 | 0 | n/a | n/a | n/a |
| #1063 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #1064 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #1088 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #1112 | space-bunny-free | 3 | 0 | 0 | n/a | n/a | n/a |
| #945 | space-bunny-free | 3 | 1 | 0 | n/a | n/a | n/a |
| #764 | space-bunny-free | 1 | 1 | 0 | n/a | n/a | n/a |
| #1099 | sonnet-5.5 | 2 | 0 | 0 | n/a | n/a | n/a |
| #921 | space-bunny-free | 1 | 1 | 0 | n/a | n/a | n/a |
| #624 | space-bunny-free | 2 | 0 | 0 | n/a | n/a | n/a |
| #1081 | sonnet-5.5 | 3 | 0 | 0 | n/a | n/a | n/a |
| #683 | sonnet-5.5 | 1 | 1 | 0 | n/a | n/a | n/a |
| #762 | longcat-2.5-preview-free | 1 | 0 | 0 | n/a | n/a | n/a |
| #1097 | sonnet-5.5 | 3 | 0 | 0 | n/a | n/a | n/a |
| #757 | space-bunny-free | 2 | 1 | 0 | n/a | n/a | n/a |
| #1173 | deepseek-v4.1-flash | 2 | 1 | 0 | n/a | n/a | n/a |
| #1080 | sonnet-5.5 | 3 | 0 | 0 | n/a | n/a | n/a |
| #689 | sonnet-5.5 | 1 | 1 | 0 | n/a | n/a | n/a |
| #1160 | space-bunny-free | 1 | 1 | 0 | n/a | n/a | n/a |
| #1174 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #1091 | sonnet-5.5 | 2 | 0 | 0 | n/a | n/a | n/a |
| #1058 | longcat-2.5-preview-free | 3 | 0 | 0 | n/a | n/a | n/a |
| #1188 | space-bunny-free | 2 | 0 | 0 | n/a | n/a | n/a |
| #694 | sonnet-5.5 | 4 | 1 | 0 | n/a | n/a | n/a |
| #799 | deepseek-v4.1-flash | 1 | 1 | 0 | n/a | n/a | n/a |
| #702 | sonnet-5.5 | 2 | 2 | 0 | n/a | n/a | n/a |
| #1209 | space-bunny-free | 3 | 0 | 0 | n/a | n/a | n/a |
| #890 | space-bunny-free | 1 | 1 | 0 | n/a | n/a | n/a |
| #904 | deepseek-v4.1-flash | 4 | 1 | 0 | n/a | n/a | n/a |
| #891 | space-bunny-free | 1 | 0 | 0 | n/a | n/a | n/a |
| #705 | sonnet-5.5 | 1 | 0 | 0 | n/a | n/a | n/a |
| #1090 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #793 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #914 | space-bunny-free | 1 | 0 | 0 | n/a | n/a | n/a |
| #756 | longcat-2.5-preview-free | 1 | 0 | 0 | n/a | n/a | n/a |
| #743 | sonnet-5.5 | 1 | 1 | 0 | n/a | n/a | n/a |
| #838 | space-bunny-free | 2 | 0 | 0 | n/a | n/a | n/a |
| #792 | space-bunny-free | 1 | 0 | 0 | n/a | n/a | n/a |
| #808 | space-bunny-free | 2 | 0 | 0 | n/a | n/a | n/a |
| #850 | space-bunny-free | 1 | 0 | 0 | n/a | n/a | n/a |
| #1201 | space-bunny-free | 1 | 0 | 0 | n/a | n/a | n/a |
| #1199 | space-bunny-free | 1 | 0 | 0 | n/a | n/a | n/a |
| #744 | sonnet-5.5 | 1 | 0 | 0 | n/a | n/a | n/a |
| #723 | deepseek-v4.1-flash | 3 | 3 | 0 | n/a | n/a | n/a |
| #1133 | longcat-2.5-preview-free | 1 | 0 | 0 | n/a | n/a | n/a |
| #1247 | space-bunny-free | 1 | 0 | 0 | n/a | n/a | n/a |
| #722 | space-bunny-free | 3 | 3 | 0 | n/a | n/a | n/a |
| #1095 | space-bunny-free | 1 | 0 | 0 | n/a | n/a | n/a |
| #1116 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1131 | space-bunny-free | 2 | 0 | 0 | n/a | n/a | n/a |
| #1104 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1245 | space-bunny-free | 1 | 0 | 0 | n/a | n/a | n/a |
| #1242 | space-bunny-free | 1 | 0 | 0 | n/a | n/a | n/a |
| #746 | sonnet-5.5 | 3 | 0 | 0 | n/a | n/a | n/a |
| #1087 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1121 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1235 | space-bunny-free | 2 | 0 | 0 | n/a | n/a | n/a |
| #1146 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1117 | deepseek-v4.1-flash | 4 | 1 | 0 | n/a | n/a | n/a |
| #755 | sonnet-5.5 | 3 | 1 | 0 | n/a | n/a | n/a |
| #886 | longcat-2.5-preview-free | 1 | 0 | 0 | n/a | n/a | n/a |
| #863 | space-bunny-free | 1 | 0 | 0 | n/a | n/a | n/a |
| #1234 | space-bunny-free | 1 | 1 | 0 | n/a | n/a | n/a |
| #867 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #875 | space-bunny-free | 1 | 0 | 0 | n/a | n/a | n/a |
| #868 | space-bunny-free | 2 | 1 | 0 | n/a | n/a | n/a |
| #909 | space-bunny-free | 1 | 0 | 0 | n/a | n/a | n/a |
| #1049 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1051 | space-bunny-free | 3 | 0 | 0 | n/a | n/a | n/a |
| #1054 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1056 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1059 | space-bunny-free | 2 | 0 | 0 | n/a | n/a | n/a |
| #1178 | longcat-2.5-preview-free | 1 | 0 | 0 | n/a | n/a | n/a |
| #1055 | space-bunny-free | 3 | 1 | 0 | n/a | n/a | n/a |
| #1062 | space-bunny-free | 3 | 0 | 0 | n/a | n/a | n/a |
| #1065 | space-bunny-free | 3 | 0 | 0 | n/a | n/a | n/a |
| #1069 | deepseek-v4.1-flash | 3 | 1 | 0 | n/a | n/a | n/a |
| #1066 | space-bunny-free | 2 | 0 | 0 | n/a | n/a | n/a |
| #1068 | space-bunny-free | 2 | 0 | 0 | n/a | n/a | n/a |
| #1004 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1003 | sonnet-5.5 | 3 | 0 | 0 | n/a | n/a | n/a |
| #272 | deepseek-v4.1-flash | 3 | 1 | 0 | n/a | n/a | n/a |
| #422 | deepseek-v4.1-flash | 3 | 1 | 0 | n/a | n/a | n/a |
| #1074 | space-bunny-free | 2 | 0 | 0 | n/a | n/a | n/a |
| #421 | deepseek-v4.1-flash | 3 | 3 | 0 | n/a | n/a | n/a |
| #1079 | space-bunny-free | 2 | 0 | 0 | n/a | n/a | n/a |
| #1077 | space-bunny-free | 3 | 0 | 0 | n/a | n/a | n/a |
| #887 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #1140 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1078 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1152 | space-bunny-free | 1 | 0 | 0 | n/a | n/a | n/a |
| #426 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #423 | deepseek-v4.1-flash | 3 | 1 | 0 | n/a | n/a | n/a |
| #1183 | space-bunny-free | 2 | 0 | 0 | n/a | n/a | n/a |
| #424 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1163 | space-bunny-free | 1 | 0 | 0 | n/a | n/a | n/a |
| #987 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #988 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #989 | deepseek-v4.1-flash | 3 | 1 | 0 | n/a | n/a | n/a |
| #990 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #991 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #992 | deepseek-v4.1-flash | 3 | 1 | 0 | n/a | n/a | n/a |
| #835 | gpt-6 | 2 | 0 | 0 | n/a | n/a | n/a |
| #1262 | longcat-2.5-preview-free | 2 | 0 | 0 | n/a | n/a | n/a |
| #1197 | deepseek-v4.1-flash | 3 | 1 | 0 | n/a | n/a | n/a |
| #1190 | space-bunny-free | 3 | 0 | 0 | n/a | n/a | n/a |
| #1159 | space-bunny-free | 1 | 0 | 0 | n/a | n/a | n/a |
| #996 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #997 | deepseek-v4.1-flash | 3 | 1 | 0 | n/a | n/a | n/a |
| #998 | deepseek-v4.1-flash | 3 | 1 | 0 | n/a | n/a | n/a |
| #999 | deepseek-v4.1-flash | 3 | 1 | 0 | n/a | n/a | n/a |
| #993 | longcat-2.5-preview-free | 3 | 0 | 0 | n/a | n/a | n/a |
| #1000 | deepseek-v4.1-flash | 3 | 1 | 0 | n/a | n/a | n/a |
| #994 | space-bunny-free | 3 | 1 | 0 | n/a | n/a | n/a |
| #1001 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1261 | space-bunny-free | 2 | 0 | 0 | n/a | n/a | n/a |
| #995 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1217 | space-bunny-free | 3 | 0 | 0 | n/a | n/a | n/a |
| #1002 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1006 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1005 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1263 | space-bunny-free | 2 | 0 | 0 | n/a | n/a | n/a |
| #1243 | space-bunny-free | 2 | 0 | 0 | n/a | n/a | n/a |
| #1008 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1007 | deepseek-v4.1-flash | 3 | 1 | 0 | n/a | n/a | n/a |
| #1342 | space-bunny-free | 3 | 0 | 0 | n/a | n/a | n/a |
| #1009 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1010 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1014 | deepseek-v4.1-flash | 3 | 1 | 0 | n/a | n/a | n/a |
| #1343 | gpt-6 | 2 | 0 | 0 | n/a | n/a | n/a |
| #1011 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1267 | space-bunny-free | 3 | 0 | 0 | n/a | n/a | n/a |
| #1276 | space-bunny-free | 2 | 0 | 0 | n/a | n/a | n/a |
| #1017 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1020 | deepseek-v4.1-flash | 3 | 1 | 0 | n/a | n/a | n/a |
| #1021 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1023 | gpt-6 | 3 | 0 | 0 | n/a | n/a | n/a |
| #1015 | gpt-6 | 3 | 1 | 0 | n/a | n/a | n/a |
| #1025 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1464 | longcat-2.5-preview-free | 3 | 1 | 0 | n/a | n/a | n/a |
| #1027 | deepseek-v4.1-flash | 3 | 1 | 0 | n/a | n/a | n/a |
| #1026 | deepseek-v4.1-flash | 3 | 2 | 0 | n/a | n/a | n/a |
| #776 | gpt-6 | 3 | 0 | 0 | n/a | n/a | n/a |
| #1012 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1318 | gpt-6 | 3 | 0 | 0 | n/a | n/a | n/a |
| #1319 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1321 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1322 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1324 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1323 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1013 | deepseek-v4.1-flash | 3 | 1 | 0 | n/a | n/a | n/a |
| #1348 | gpt-6 | 3 | 0 | 0 | n/a | n/a | n/a |
| #1356 | deepseek-v4.1-flash | 2 | 1 | 0 | n/a | n/a | n/a |
| #1349 | deepseek-v4.1-flash | 3 | 1 | 0 | n/a | n/a | n/a |
| #805 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #828 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #777 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1460 | deepseek-v4.1-flash | 2 | 1 | 0 | n/a | n/a | n/a |
| #1377 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1461 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1428 | deepseek-v4.1-flash | 3 | 1 | 0 | n/a | n/a | n/a |
| #833 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1463 | deepseek-v4.1-flash | 3 | 1 | 0 | n/a | n/a | n/a |
| #1443 | space-bunny-free | 3 | 0 | 0 | n/a | n/a | n/a |
| #866 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #1177 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #1137 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1172 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #1210 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1390 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1379 | gpt-6 | 3 | 0 | 0 | n/a | n/a | n/a |
| #1229 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #1280 | space-bunny-free | 3 | 1 | 0 | n/a | n/a | n/a |
| #1384 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1389 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1157 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1406 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1230 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1320 | space-bunny-free | 3 | 0 | 0 | n/a | n/a | n/a |
| #1415 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1386 | deepseek-v4.1-flash | 2 | 1 | 0 | n/a | n/a | n/a |
| #1228 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #1420 | longcat-2.5-preview-free | 3 | 1 | 0 | n/a | n/a | n/a |
| #1393 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1408 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1419 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1418 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1385 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1417 | gpt-6 | 3 | 0 | 0 | n/a | n/a | n/a |
| #1244 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #1422 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1232 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #1233 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #1226 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1246 | deepseek-v4.1-flash | 1 | 1 | 0 | n/a | n/a | n/a |
| #1264 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #1429 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1407 | gpt-6 | 2 | 1 | 0 | n/a | n/a | n/a |
| #1440 | gpt-6 | 2 | 0 | 0 | n/a | n/a | n/a |
| #1182 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1279 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1252 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #1281 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1283 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1293 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #1259 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #1285 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #1294 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1426 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1250 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #1459 | gpt-6 | 3 | 0 | 0 | n/a | n/a | n/a |
| #1439 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1297 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #1303 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #1213 | deepseek-v4.1-flash | 3 | 1 | 0 | n/a | n/a | n/a |
| #1196 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #1266 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #1312 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #1306 | deepseek-v4.1-flash | 2 | 1 | 0 | n/a | n/a | n/a |
| #1305 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1466 | gpt-6 | 2 | 0 | 0 | n/a | n/a | n/a |
| #1296 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #1278 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1202 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1325 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1314 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1469 | gpt-6 | 3 | 0 | 0 | n/a | n/a | n/a |
| #1326 | deepseek-v4.1-flash | 3 | 1 | 0 | n/a | n/a | n/a |
| #1309 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1327 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1446 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1304 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1448 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1450 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1310 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1470 | gpt-6 | 2 | 0 | 0 | n/a | n/a | n/a |
| #1330 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1328 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1334 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1316 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1484 | deepseek-v4.1-flash | 3 | 1 | 0 | n/a | n/a | n/a |
| #1134 | gpt-6.1-sol | 1 | 0 | 0 | n/a | n/a | n/a |
| #1344 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1332 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1333 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1347 | deepseek-v4.1-flash | 2 | 1 | 0 | n/a | n/a | n/a |
| #1180 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #1331 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1338 | gpt-6.1-sol | 1 | 0 | 0 | n/a | n/a | n/a |
| #1366 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #1329 | deepseek-v4.1-flash | 3 | 1 | 0 | n/a | n/a | n/a |
| #1367 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1369 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #1355 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1486 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1335 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1380 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #1368 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1337 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1400 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1374 | deepseek-v4.1-flash | 3 | 1 | 0 | n/a | n/a | n/a |
| #1396 | deepseek-v4.1-flash | 2 | 1 | 0 | n/a | n/a | n/a |
| #1401 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1402 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #1487 | deepseek-v4.1-flash | 3 | 1 | 0 | n/a | n/a | n/a |
| #1421 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1432 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1378 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1431 | gpt-6.1-sol | 2 | 0 | 0 | n/a | n/a | n/a |
| #1365 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1425 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1451 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1455 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1647 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1644 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1441 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1489 | opus | 2 | 0 | 0 | n/a | n/a | n/a |
| #1491 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1498 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1494 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1516 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1526 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1504 | gpt-6 | 1 | 0 | 0 | n/a | n/a | n/a |
| #1510 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1506 | longcat-2.5-preview-free | 2 | 0 | 0 | n/a | n/a | n/a |
| #1542 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #1532 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1549 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1520 | longcat-2.5-preview-free | 2 | 0 | 0 | n/a | n/a | n/a |
| #1660 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1536 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #1547 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1544 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #1531 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1553 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #1557 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1554 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1561 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1517 | gpt-6 | 1 | 0 | 0 | n/a | n/a | n/a |
| #1566 | deepseek-v4.1-flash | 1 | 1 | 0 | n/a | n/a | n/a |
| #1513 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #1656 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1563 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1472 | space-bunny-free | 1 | 0 | 0 | n/a | n/a | n/a |
| #1555 | deepseek-v4.1-flash | 3 | 1 | 0 | n/a | n/a | n/a |
| #1449 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1579 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1564 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1574 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1474 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1585 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1582 | deepseek-v4.1-flash | 2 | 1 | 0 | n/a | n/a | n/a |
| #1648 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1569 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #1587 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1571 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1590 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1560 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1550 | gpt-6 | 3 | 0 | 0 | n/a | n/a | n/a |
| #1593 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1603 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #1609 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1615 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1599 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1591 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #1620 | deepseek-v4.1-flash | 1 | 1 | 0 | n/a | n/a | n/a |
| #1684 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1595 | gpt-6.1-sol | 1 | 0 | 0 | n/a | n/a | n/a |
| #1623 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #1622 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1597 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #1618 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #1619 | deepseek-v4.1-flash | 1 | 1 | 0 | n/a | n/a | n/a |
| #1543 | gpt-6.1-sol | 3 | 0 | 0 | n/a | n/a | n/a |
| #1556 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1636 | gpt-6 | 1 | 0 | 0 | n/a | n/a | n/a |
| #1610 | gpt-6.1-sol | 2 | 0 | 0 | n/a | n/a | n/a |
| #1529 | gpt-6.1-sol | 2 | 0 | 0 | n/a | n/a | n/a |
| #1695 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1612 | sonnet-5.5 | 1 | 0 | 0 | n/a | n/a | n/a |
| #1673 | gpt-6 | 3 | 0 | 0 | n/a | n/a | n/a |
| #1672 | opus | 3 | 0 | 0 | n/a | n/a | n/a |
| #1692 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #1700 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #1702 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1704 | gpt-6.1-sol | 3 | 0 | 0 | n/a | n/a | n/a |
| #1720 | gpt-6 | 2 | 0 | 0 | n/a | n/a | n/a |
| #1705 | opus | 3 | 0 | 0 | n/a | n/a | n/a |
| #1708 | longcat-2.5-preview-free | 1 | 0 | 0 | n/a | n/a | n/a |
| #1718 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1722 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #1723 | gpt-6 | 3 | 0 | 0 | n/a | n/a | n/a |
| #1721 | gpt-6 | 2 | 0 | 0 | n/a | n/a | n/a |
| #1727 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #1480 | gpt-6 | 3 | 0 | 0 | n/a | n/a | n/a |
| #1359 | space-bunny-free | 2 | 0 | 0 | n/a | n/a | n/a |
| #1738 | gpt-6 | 3 | 0 | 0 | n/a | n/a | n/a |
| #1478 | gpt-6.1-sol | 3 | 0 | 0 | n/a | n/a | n/a |
| #1751 | gpt-6 | 2 | 0 | 0 | n/a | n/a | n/a |
| #1739 | gpt-6 | 3 | 2 | 0 | n/a | n/a | n/a |
| #1361 | gpt-6.1-sol | 2 | 0 | 0 | n/a | n/a | n/a |
| #1399 | gpt-6.1-sol | 2 | 0 | 0 | n/a | n/a | n/a |
| #1757 | gpt-6 | 2 | 1 | 0 | n/a | n/a | n/a |
| #1646 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1759 | deepseek-v4.1-flash | 3 | 1 | 0 | n/a | n/a | n/a |
| #1650 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1765 | gpt-6 | 3 | 0 | 0 | n/a | n/a | n/a |
| #1652 | gpt-6.1-sol | 3 | 0 | 0 | n/a | n/a | n/a |
| #1728 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1729 | gpt-6.1-sol | 2 | 0 | 0 | n/a | n/a | n/a |
| #1768 | gpt-6 | 2 | 0 | 0 | n/a | n/a | n/a |
| #1778 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1733 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1736 | gpt-6.1-sol | 2 | 0 | 0 | n/a | n/a | n/a |
| #1782 | gpt-6 | 2 | 0 | 0 | n/a | n/a | n/a |
| #1748 | sonnet-5.5 | 3 | 0 | 0 | n/a | n/a | n/a |
| #1758 | gpt-6.1-sol | 3 | 0 | 0 | n/a | n/a | n/a |
| #1788 | gpt-6 | 3 | 0 | 0 | n/a | n/a | n/a |
| #1764 | gpt-6.1-sol | 2 | 0 | 0 | n/a | n/a | n/a |
| #1766 | gpt-6.1-sol | 3 | 0 | 0 | n/a | n/a | n/a |
| #1790 | gpt-6 | 2 | 0 | 0 | n/a | n/a | n/a |
| #1792 | gpt-6 | 3 | 0 | 0 | n/a | n/a | n/a |
| #1779 | gpt-6.1-sol | 1 | 0 | 0 | n/a | n/a | n/a |
| #1769 | gpt-6.1-sol | 2 | 0 | 0 | n/a | n/a | n/a |
| #1781 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1816 | gpt-6 | 3 | 0 | 0 | n/a | n/a | n/a |
| #1819 | gpt-6 | 3 | 0 | 0 | n/a | n/a | n/a |
| #1784 | gpt-6.1-sol | 1 | 0 | 0 | n/a | n/a | n/a |
| #1822 | space-bunny-free | 1 | 0 | 0 | n/a | n/a | n/a |
| #1785 | gpt-6.1-sol | 3 | 0 | 0 | n/a | n/a | n/a |
| #1823 | gpt-6 | 2 | 0 | 0 | n/a | n/a | n/a |
| #1524 | gpt-6.1-sol | 3 | 0 | 0 | n/a | n/a | n/a |
| #1825 | gpt-6 | 2 | 0 | 0 | n/a | n/a | n/a |
| #1655 | gpt-6.1-sol | 3 | 0 | 0 | n/a | n/a | n/a |
| #1690 | deepseek-v4.1-flash | 3 | 1 | 0 | n/a | n/a | n/a |
| #1605 | gpt-6.1-sol | 3 | 0 | 0 | n/a | n/a | n/a |
| #1606 | gpt-6.1-sol | 1 | 0 | 0 | n/a | n/a | n/a |
| #1827 | gpt-6 | 2 | 0 | 0 | n/a | n/a | n/a |
| #1829 | gpt-6 | 2 | 0 | 0 | n/a | n/a | n/a |
| #1608 | gpt-6.1-sol | 2 | 0 | 0 | n/a | n/a | n/a |
| #1616 | gpt-6.1-sol | 2 | 0 | 0 | n/a | n/a | n/a |
| #1834 | gpt-6 | 2 | 0 | 0 | n/a | n/a | n/a |
| #1621 | gpt-6.1-sol | 2 | 0 | 0 | n/a | n/a | n/a |
| #1835 | gpt-6 | 1 | 0 | 0 | n/a | n/a | n/a |
| #1624 | gpt-6.1-sol | 3 | 0 | 0 | n/a | n/a | n/a |
| #1843 | gpt-6 | 3 | 0 | 0 | n/a | n/a | n/a |
| #1625 | gpt-6.1-sol | 3 | 0 | 0 | n/a | n/a | n/a |
| #1626 | gpt-6.1-sol | 1 | 0 | 0 | n/a | n/a | n/a |
| #1847 | gpt-6 | 2 | 0 | 0 | n/a | n/a | n/a |
| #1850 | gpt-6 | 3 | 0 | 0 | n/a | n/a | n/a |
| #1627 | gpt-6.1-sol | 1 | 0 | 0 | n/a | n/a | n/a |
| #1679 | gpt-6 | 2 | 0 | 0 | n/a | n/a | n/a |
| #1714 | gpt-6 | 3 | 0 | 0 | n/a | n/a | n/a |
| #1735 | gpt-6 | 2 | 0 | 0 | n/a | n/a | n/a |
| #1853 | gpt-6 | 3 | 0 | 0 | n/a | n/a | n/a |
| #1628 | gpt-6.1-sol | 2 | 0 | 0 | n/a | n/a | n/a |
| #1870 | gpt-6 | 3 | 0 | 0 | n/a | n/a | n/a |
| #1631 | gpt-6.1-sol | 1 | 0 | 0 | n/a | n/a | n/a |
| #1924 | gpt-6 | 3 | 0 | 0 | n/a | n/a | n/a |
| #1635 | gpt-6.1-sol | 1 | 0 | 0 | n/a | n/a | n/a |
| #1987 | gpt-6 | 3 | 0 | 0 | n/a | n/a | n/a |
| #1642 | space-bunny-free | 1 | 0 | 0 | n/a | n/a | n/a |
| #1639 | space-bunny-free | 3 | 0 | 0 | n/a | n/a | n/a |
| #1643 | space-bunny-free | 2 | 0 | 0 | n/a | n/a | n/a |
| #1990 | gpt-6 | 3 | 1 | 0 | n/a | n/a | n/a |
| #1683 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1665 | deepseek-v4.1-flash | 3 | 0 | 0 | n/a | n/a | n/a |
| #1637 | gpt-6.1-sol | 1 | 0 | 0 | n/a | n/a | n/a |
| #1693 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #2062 | space-bunny-free | 2 | 1 | 0 | n/a | n/a | n/a |
| #1677 | space-bunny-free | 1 | 0 | 0 | n/a | n/a | n/a |
| #1661 | space-bunny-free | 2 | 0 | 0 | n/a | n/a | n/a |
| #1657 | space-bunny-free | 2 | 0 | 0 | n/a | n/a | n/a |
| #1641 | space-bunny-free | 2 | 0 | 0 | n/a | n/a | n/a |
| #1998 | deepseek-v4.1-flash | 3 | 1 | 0 | n/a | n/a | n/a |
| #1658 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1663 | space-bunny-free | 2 | 0 | 0 | n/a | n/a | n/a |
| #1687 | space-bunny-free | 1 | 1 | 0 | n/a | n/a | n/a |
| #1666 | gpt-6.1-sol | 3 | 0 | 0 | n/a | n/a | n/a |
| #1685 | space-bunny-free | 1 | 0 | 0 | n/a | n/a | n/a |
| #1793 | space-bunny-free | 1 | 0 | 0 | n/a | n/a | n/a |
| #1696 | space-bunny-free | 2 | 0 | 0 | n/a | n/a | n/a |
| #1675 | space-bunny-free | 2 | 0 | 0 | n/a | n/a | n/a |
| #1795 | space-bunny-free | 2 | 0 | 0 | n/a | n/a | n/a |
| #1805 | space-bunny-free | 2 | 0 | 0 | n/a | n/a | n/a |
| #1804 | space-bunny-free | 2 | 0 | 0 | n/a | n/a | n/a |
| #1814 | space-bunny-free | 1 | 0 | 0 | n/a | n/a | n/a |
| #1800 | gpt-6.1-sol | 2 | 0 | 0 | n/a | n/a | n/a |
| #1807 | space-bunny-free | 2 | 0 | 0 | n/a | n/a | n/a |
| #1957 | deepseek-v4.1-flash | 2 | 1 | 0 | n/a | n/a | n/a |
| #1964 | deepseek-v4.1-flash | 2 | 1 | 0 | n/a | n/a | n/a |
| #1849 | gpt-6.1-sol | 3 | 0 | 0 | n/a | n/a | n/a |
| #1961 | gpt-6.1-sol | 3 | 0 | 0 | n/a | n/a | n/a |
| #1928 | gpt-6.1-sol | 2 | 0 | 0 | n/a | n/a | n/a |
| #1910 | gpt-6.1-sol | 3 | 0 | 0 | n/a | n/a | n/a |
| #1914 | gpt-6.1-sol | 1 | 0 | 0 | n/a | n/a | n/a |
| #1921 | gpt-6.1-sol | 2 | 0 | 0 | n/a | n/a | n/a |
| #1901 | gpt-6.1-sol | 3 | 0 | 0 | n/a | n/a | n/a |
| #1206 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #1911 | gpt-6.1-sol | 2 | 0 | 0 | n/a | n/a | n/a |
| #1175 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #2009 | gpt-6.1-sol | 2 | 0 | 0 | n/a | n/a | n/a |
| #1903 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #2055 | gpt-6.1-sol | 2 | 0 | 0 | n/a | n/a | n/a |
| #1892 | deepseek-v4.1-flash | 1 | 0 | 0 | n/a | n/a | n/a |
| #1923 | gpt-6.1-sol | 3 | 0 | 0 | n/a | n/a | n/a |
| #2076 | deepseek-v4.1-flash | 2 | 0 | 0 | n/a | n/a | n/a |
| #1905 | gpt-6.1-sol | 3 | 0 | 0 | n/a | n/a | n/a |

### Escalations

- Opus matched 94 of 121 functions a cheaper model had failed.
- Sonnet matched 109 of 116 functions a cheaper model had failed.
- Gpt-6 matched 5 of 134 functions a cheaper model had failed.
- Space-bunny-free matched 80 of 359 functions a cheaper model had failed.
- Deepseek-v4.1-flash matched 78 of 560 functions a cheaper model had failed.
- Gpt-5.6-terra matched 3 of 16 functions a cheaper model had failed.
- Mimo-v2.6-flash matched 3 of 5 functions a cheaper model had failed.
- Sonnet-5.5 matched 29 of 73 functions a cheaper model had failed.
- Longcat-2.5-preview-free matched 5 of 34 functions a cheaper model had failed.
- Muse-spark-1.3-free matched 0 of 4 functions a cheaper model had failed.
- Gpt-6.1-sol matched 0 of 107 functions a cheaper model had failed.
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
- Fixed checker limit: a vtable defined in a decompiled file used to be verified by
  name only. check.py now checks each declared slot against the original
  vtable: a slot must hold a function, must not run into the next class's
  vtable, and must agree with established names. Declaring fewer slots than
  the original is still allowed (0x4b0610 declares 4 of 21).
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
