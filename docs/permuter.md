# The source permuter

`tools/permute.py` grinds on a function that is close to matching and stuck. It
rewrites the function's source in small ways that cannot change what it
computes (swap two independent statements, name a subexpression, flip a
comparison, turn a `for` into a `while`, ...), compiles every candidate with the
real compiler, scores it against the original, and climbs. It is this
project's version of the decomp community's
[decomp-permuter](https://github.com/simonlindholm/decomp-permuter).

It is for the last few percent that are about register allocation, operand
order, instruction scheduling and stack slots: the differences where an agent
knows what the function does but not which spelling MSVC 5 wants. It does not
fix wrong types, wrong struct layouts, missing statements or wrong callees;
get those right first.

## Running it

```sh
uv run tools/permute.py 0x4ac970                    # 15 minutes, 12 parallel compiles
uv run tools/permute.py 0x4ac970 --minutes 30 --seed 7
uv run tools/permute.py 0x4ac970 --file build/scratch/0x4ac970/try.cpp
uv run tools/permute.py --batch 0x4ac970 0x4c1ab0 --minutes 10
uv run tools/permute.py --batch --partials 99 --minutes 15 --stall 8
```

It needs nothing beyond the normal setup: the script declares its own
dependencies (tree-sitter and its C++ grammar), which `uv run` installs.

| Option | Meaning |
| --- | --- |
| `--minutes N` | time budget per function (default 15) |
| `--patience M` | after the budget, keep going while the last gain is less than M minutes old |
| `--max-minutes N` | hard limit when `--patience` extends a run |
| `--stall M` | stop early once M minutes pass without a gain |
| `--jobs N` | parallel compiles (default 12); the tool runs at `nice 10` |
| `--seed N` | make the search repeatable |
| `--file F` | start from F instead of the file under `src/` |
| `--resume` | start from the last `build/permute/<address>/best.cpp` |
| `--no-helpers` | only rewrite the annotated function, not the inline helpers it calls |
| `--no-focus` | pick mutation sites anywhere, not mostly on the lines behind differing instructions |
| `--only a,b` | use only these mutation kinds (names below) |
| `--keep-going` | do not stop at the first MATCH |
| `--cleanup M` | minutes for the final cleanup (default 2, 0 to skip) |
| `--minimize F` | only clean up an existing candidate F against the starting file |
| `--batch`, `--partials P` | several functions in a row; `--partials P` adds every partial at P% or more from `data/progress.csv`, best first, and prints a summary table |

It never writes to `src/`, `data/` or any tracked file. Everything goes to
`build/permute/<address>/`:

| File | What it is |
| --- | --- |
| `best.cpp` | the whole file with the best version found, after cleanup |
| `best.diff` | `best.cpp` against the starting file: read this |
| `best.json` | start and best scores, status, minutes, candidates tried, and the chain of mutations behind the best |
| `log.txt` | one line per improvement: time, score before and after, check.py percentage, mutations |
| `stats.json` | per mutation kind: tried, compiled, equal score, improved, new best |
| `matches/` | every candidate that MATCHed |
| `best_raw.cpp` | the best version before cleanup |
| `best_ratio.cpp` | written when the highest check.py percentage seen is not `best.cpp`'s |
| `best_search.cpp` | the lowest fine score seen, when its check.py percentage is below the start's (`best.cpp` never is): often the right instruction order with the wrong registers, worth a look |

Always confirm a result with the checker before using it:

```sh
uv run tools/check.py 0x4ac970 build/permute/0x4ac970/best.cpp
```

## How it scores

check.py's similarity (a ratio of matching instruction lines) is too coarse to
climb: most single rewrites leave it unchanged. The permuter disassembles the
candidate exactly as check.py does (same normalisation, same masking of the
addresses the linker fills in) and then scores the difference like
decomp-permuter does. Identical instructions are aligned first; inside each
block that differs, the rest are paired by mnemonic; then

| Difference | Cost |
| --- | ---: |
| same branch, different target (code moved around it) | 1 |
| same instruction, different registers | 5 |
| same instruction, different stack slot | 5 |
| same mnemonic, other operands | 10 |
| an instruction present on both sides but in a different place | 60 |
| the same, but its `[esp+N]` changed on the way (it moved across a push) | 65 |
| the same, but with other registers too | 70 |
| an instruction only one side has | 100 |
| each byte of size difference | 2 |

Lower is better and 0 is a MATCH: the bytes are identical and every reference
the linker fills in resolves (check.py's own `compare` decides that). A
candidate whose bytes match but whose references do not is reported as
"bytes match, reference wrong" (a naming problem, not a permutation one). If
the file has other annotated functions that match, every candidate must keep
them matching, or it is thrown away.

## How it searches

A hill climb with random restarts over a small population:

- It keeps the 12 best candidates. Each new candidate takes one of them (the
  best half the time, otherwise biased towards the better ones), or now and
  then the starting file (a restart), and applies 1 to 4 random mutations.
- A candidate joins the population when it scores at least as well as its
  parent. Equal scores are accepted on purpose, newest first: near misses sit
  on wide plateaus, and the useful rewrite is often two neutral steps away.
- Workers mutate, compile (`compile_source` from check.py, each worker in its
  own scratch folder under `build/permute/<address>/work/`) and score in
  parallel processes. Duplicates are skipped by a hash of the text.
- **Focus.** Candidates are compiled with `/Zd` added, which puts COFF line
  numbers in the object and leaves the code itself alone (the tool checks
  that for the file's scored functions before relying on it, and otherwise
  turns focus off). The scorer maps every differing instruction to its source
  line, and three times in four a mutation picks its site on or next to one of
  those lines. In a 2,000-byte function this is the difference between
  rewriting the one block that differs and rewriting anything. `--no-focus`
  turns it off.

### Cleanup

Accepting equal scores means the winner carries many neutral rewrites. Before
writing `best.cpp` the tool removes them:

- For a MATCH it first runs delta debugging over the text diff (by lines, then
  by tokens) to find a small subset of the change that still matches. That is
  safe only because the result must still MATCH, so it computes exactly what
  the original computes.
- Then, for every result, a short search applies only meaning-preserving
  mutations (the kinds that can undo the others: inline a temporary, merge a
  declaration back, move a statement back, strip parentheses, drop an unused
  local, ...) and keeps each one that brings the text closer to the starting
  file without losing score.

The cleanup is a heuristic. A partial result that is not a MATCH is never
recombined at the text level, so its meaning is only as safe as the mutations
themselves; still read `best.diff`, and simplify further by hand if a rewrite
looks odd (re-check after every edit).

## The mutations

Each mutation is a function in `tools/permute_mutate.py`. They work on a
tree-sitter parse of the file and only touch the target function and the small
inline helpers it calls (functions defined in the file, not annotated, under 60
lines). Each one checks what it needs before rewriting, with a small effect
analysis: which locals a statement reads and writes, which memory it reads and
writes (by access path, so `p->a` and `p->b` do not conflict but `p->a` and
`q->a` might), whether it calls anything, and whether it leaves the block
(`return`, `break`, `goto`, labels). Locals whose address is taken, arrays,
and arguments passed by reference count as memory.

| Kind | Rewrite | Condition |
| --- | --- | --- |
| `move_stmt` | move a statement up or down past one or more others | no dependency between them, no control flow |
| `move_decl` | move a declaration among the others (stack slots follow declaration order) | as above |
| `split_multi_decl` | `int a, b;` to `int a; int b;`, or swap two declarators | |
| `merge_decls` | `int a; int b;` to `int a, b;` | same type text |
| `split_init`, `merge_init` | `T x = e;` and `T x; x = e;` | scalar type, no `static` or `const` |
| `decl_scope` | move a declaration into the innermost block using it, or out to the top | name declared once; never into a loop |
| `swap_commutative` | `a + b` to `b + a` (also `*`, `&`, `|`, `^`, `==`, `!=`) | neither side writes; no call against a memory read |
| `flip_compare` | `a < b` to `b > a` | as above |
| `negate_if`, `empty_then` | `if (c) A else B` to `if (!c) B else A`; `if (!c) {} else A` and back | `<` is only inverted for integers and pointers |
| `loop_form` | `for` to `while` (increments at the end), `for` to `if (c) do ... while (inc, c)`, `while` to `for`, trailing increments into the `for` header, `do`/`while` to `for (;;) { ... if (!c) break; }`, `for` initialiser out of the header | no `continue` where it would skip the increment |
| `temp_intro` | name a subexpression: `T tmp = e;` before its statement | e has no side effects, is evaluated unconditionally, and nothing the statement does first changes its inputs |
| `temp_inline` | put a temporary's expression back at its single use, or re-express one use of a multi-use temporary (a fresh value number) | the temporary is never written again and the statements in between leave its inputs alone |
| `return_var` | `return e;` to `T ret = e; return ret;` | scalar type |
| `compound_assign` | `x += y` and `x = x + y` | x has no side effects |
| `incdec` | `i++`, `++i`, `i += 1`, `i = i + 1` | statement position only |
| `andor_swap` | `a && b` to `b && a` | both sides cannot fault (no dereference, call or division) |
| `nested_if` | `if (a && b) S` and `if (a) { if (b) S }` | no `else` |
| `ternary` | `x = c ? a : b;` and `if (c) x = a; else x = b;` (also for `return`) | integer types |
| `zero_compare` | `!e` and `e == 0`; `if (e)` and `if (e != 0)` | |
| `cast` | add a cast of a local to its own type, or remove one | |
| `sign` | `int` and `unsigned int` for a local | only when every use is blind to the sign |
| `do_while0` | wrap statements in `do { } while (0);`, or unwrap | no `break`/`continue` inside |
| `goto_polarity` | `if (c) { A }` to `if (!c) goto skip; A; skip:;` (which arm falls through) | no declarations in A |
| `extract_helper`, `inline_helper` | move a pure expression into `static inline T inlN(...) { return e; }` and back | the inlined boundary changes evaluation order |
| `extract_stmts` | move one to three adjacent statements into `static inline void inlN(...)` (undone by `inline_helper`) | they assign no local of the caller; locals whose address escapes stay out |
| `self_store`, `drop_self_store` | `T same0 = v; v = same0;` after a store to local v, and back | compiles to nothing; the guide's 0x461b10 lever |
| `include` | add or remove one of `windows.h`, `stdio.h`, `stdlib.h`, `string.h`, `math.h`, `memory.h` | compiler state; the compiler rejects a needed header's removal |
| `convention` | `__cdecl`, `__fastcall` or none on the target when it has no parameters | the guide's 0x46c920 lever; callees keep theirs |
| `dead_decl`, `strip_parens` | drop an unused local with a pure initialiser; `(x)` to `x` | mostly for cleanup |

## When to use it

- **When a function is close and stuck.** Typically 90% or more, the right
  size or nearly, and the remaining diff is registers, operand order, a moved
  load or a swapped stack slot, after the guide's levers have been tried by
  hand. Below that the differences are usually semantic (a wrong type, a
  missing statement) and a permuter cannot find them.
- **Run it on the file as it stands.** It starts from `src/unsorted/<addr>.cpp`
  (or `--file`). Get the types, struct layouts, callee names and calling
  conventions right first; the permuter keeps all of them.
- **Give it time.** A few thousand candidates a minute is normal. Small
  functions finish a 15 minute budget with tens of thousands tried; a
  1,500-byte function far fewer. Use `--stall` to stop early when nothing
  moves, and `--patience` to keep going while it improves.
- **Run headers.py too.** The permuter's `include` mutation only toggles one
  header at a time; `tools/headers.py` sweeps every header set in one go.

## Reading the output

1. `best.json`: `status` is `match`, `bytes` (bytes match, a reference is
   wrong) or `partial`; `start_ratio` and `best_ratio` are check.py's
   percentages; `best_score` is the fine score (lower is better).
2. `best.diff`: the change. The cleanup removes most neutral rewrites, but not
   all: if a line of the diff looks pointless, revert it by hand and re-check.
   A hunk can look dead and still be needed: what the file defines before the
   function is part of MSVC 5's state. At 0x4ac970 the MATCH keeps a `static
   inline` helper that nothing calls; deleting it drops the function to 93.2%.
   The cleanup only keeps such a hunk when removing it loses the score.
3. `log.txt` and `best.json`'s `lineage`: which mutations produced the gains.
   The mutation that made the last step is a hint about the lever (a
   `move_stmt` means statement order, `temp_intro` a missing local, `include`
   compiler state) worth noting in the file's comments.
4. A MATCH: confirm with `check.py`, then bring the change into the source
   file as a normal edit, keeping the file's credit line, and simplify the
   spelling where you can while it still matches.
5. A better partial: the same, if check.py's percentage went up. `best.cpp`
   is chosen by the fine score among the candidates whose check.py
   percentage is at least the start's; `best_ratio.cpp` holds the version with
   the highest percentage when that is another one.

## First run

About an hour on 25 of the 68 partial functions at 90% or more (6,000 to
13,000 candidates per function in 6 to 9 minutes, two runs side by side) gave
three MATCHes and three better partials:

| Function | Before | After | What it took |
| --- | ---: | ---: | --- |
| 0x4b3770 | 99.0% | MATCH | one declaration moved (stack slots), found within a second |
| 0x4be400 | 99.2% | MATCH | an `a && b` test split into two nested ifs |
| 0x4ac970 | 96.6% | MATCH | a lookup result through its own local, a value read straight into a sum, an inline helper |
| 0x4a4d70 | 98.7% | 99.1% | a value read through the array again instead of through a pointer local |
| 0x42d2e0 | 96.8% | 97.1% | a global pointer loaded into a local just before its use |
| 0x4b6c30 | 91.1% | 95.0% | a named correction term that divides again, and `<windows.h>` dropped |

The other functions in the 96% to 99.8% range, which agents had already
worked over many times, did not move: their differences are scheduler and
register ties that no rewrite in the catalogue changed. The
`std::vector::insert` instantiations, which earlier notes put down to
compiler state, were not run beyond a smoke test.

## Performance

A compile and score takes about 0.2 s. All Wine processes that share a prefix
also share one `wineserver`, which runs on one core and becomes the limit at
about 12 parallel compiles (60 to 70 candidates a second on a 20-core
machine). Two permuters, or a permuter and a full `tools/progress.py`, slow
each other down. A long run can use its own copy of the Wine prefix: make the
working copy's `toolchain` a folder holding a symlink to `msvc5-sp3` and a copy
of `wineprefix`, and start a server for it once with
`WINEPREFIX=<that prefix> wineserver -p`.
