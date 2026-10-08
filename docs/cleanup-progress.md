# Cleanup progress

Written by `tools/cleanup_progress.py`.

**Readability cleanup: about 44% of the way** (mean of the rows below)

`[##################----------------------]`

Counts in `src/` and `include/` at `origin/main`, against `e9367f13` (the roadmap's starting point).

| | Start | Now | Target | Done | |
| --- | ---: | ---: | --- | ---: | --- |
| Source files | 2,727 | 705 | 112, one per module (`data/modules.csv`) | 77% | `[###############-----]` |
| Placeholder functions `FUN_<addr>` | 1,076 | 638 | 0, named | 41% | `[########------------]` |
| Placeholder globals `DAT_<addr>` | 737 | 505 | 0, named | 31% | `[######--------------]` |
| Placeholder classes `Class_<addr>` | 769 | 729 | 0, named | 5% | `[#-------------------]` |
| Placeholder fields `field_<offset>` | 589 | 569 | 0, named | 3% | `[#-------------------]` |
| Files that define `Unit` | 285 | 104 | 1, one shared definition | 64% | `[#############-------]` |
| Files opening with a matching note | 566 | 84 | 0, none | 85% | `[#################---]` |

| Cleanup issues | | | |
| --- | --- | --- | ---: |
| Gather issues | 112 of 125 closed | `[##################--]` | 89% |
| Join issues | 13 of 19 closed | `[##############------]` | 68% |
| Name issues | 72 of 94 closed | `[###############-----]` | 76% |
