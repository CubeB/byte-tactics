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

| Size (bytes) |  | Deepseek-v4.1-flash | Gpt-6 | Gpt-6-astra | Haiku | Opus | Sonnet | Space-bunny-free |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| 1-16 |  |  |  |  | 283/312 (91%) | 3/3 (100%) |  |  |
| 17-40 |  |  |  |  | 235/337 (70%) | 124/124 (100%) | 9/10 (90%) |  |
| 41-64 |  |  |  |  | 3/11 (27%) | 255/255 (100%) | 96/109 (88%) |  |
| 65-160 |  | 64/67 (96%) |  |  | 1/6 (17%) | 739/747 (99%) | 2/6 (33%) |  |
| 161-400 | 0/10 (0%) | 153/194 (79%) | 17/18 (94%) |  |  | 30/36 (83%) |  | 180/256 (70%) |
| 401+ |  |  | 5/6 (83%) | 17/27 (63%) |  | 56/80 (70%) |  |  |

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
| #275 | opus | 3 | 2 | 463,278 | 231,639 | 927 | 48 |
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
| #286 | space-bunny-free | 6 | 4 | 0 | n/a | n/a | n/a |
| #438 | space-bunny-free | 2 | 1 | 0 | n/a | n/a | n/a |
| #436 | opus | 2 | 2 | 0 | n/a | n/a | n/a |
| #431 | opus | 3 | 3 | 0 | n/a | n/a | n/a |

### Escalations

- Opus matched 94 of 113 functions a cheaper model had failed.
- Sonnet matched 109 of 116 functions a cheaper model had failed.
- Gpt-6 matched 0 of 9 functions a cheaper model had failed.
- Space-bunny-free matched 22 of 41 functions a cheaper model had failed.
- Deepseek-v4.1-flash matched 2 of 3 functions a cheaper model had failed.
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
