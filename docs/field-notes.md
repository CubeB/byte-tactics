# Field notes: what actually produced matches

Notes from two decompilation sessions (opencode / space-bunny-free, 74 matched
functions: 47 in Parts 1-4, 27 in Part 5). Everything here was measured with
`tools/check.py`; where a technique did not work, the measurement is given too,
because a ruled-out lever is as valuable as a working one.

These notes deliberately do not touch `docs/agent-guide.md`, which `AGENTS.md`
reserves for the orchestrator. They are offered as a supplement, and every claim
below is one the reader can re-verify in about five minutes.

---

## Part 1: the levers, in order of measured value

### 1. Look in the toolchain before you infer anything

`toolchain/msvc5-sp3/INCLUDE/` contains the whole 1997 SDK, MSVC 5's own
container implementations, and the CRT headers. A large fraction of this binary
is *compiled source that is sitting on disk*.

| header | what it gave | function |
| --- | --- | --- |
| `XTREE`, `VECTOR`, `XSTRING` | the pre-STL source of the inlined container methods | `0x4e2ab0` MATCH in 2 runs |
| `DSOUND.H` | the `IDirectSoundBuffer` vtable, whose offsets here do **not** match modern DirectX | `0x4cfca0` MATCH in 1 run |
| `io.h` | `_finddata_t` outright, `name[260]` at +0x14 | `0x4bc640` MATCH |
| `math.h` | fixed three SIB bytes in an unrelated function | `0x464060` |

The `dsound.h` case is the sharpest. A hand-written vtable reproduces the
modern DirectX order and is silently wrong; including the toolchain's own header
got the offsets, the register holding the buffer pointer, and the register
holding the vtable right in one move.

**Do this before writing any code.** It converts reverse engineering into
transcription, and it is free.

### 2. A "register allocation" difference is usually a wrong argument or a wrong type

This was the single biggest misdiagnosis of the session, and it cost real time
because the symptom is convincing.

| function | what the note said | actual cause | gain |
| --- | --- | --- | --- |
| `0x43adc0` | callee-saved register rotation wall | constructor's 2nd arg was `owner`, the original took the 4th parameter | 56.6% to 70.4% |
| `0x435a20` | suspected original bug | arg was `(char*)this`, original passed a buffer's address | 79.9% to MATCH |
| `0x49c9c0` | register rotation wall | led by a *matched sibling's* type declarations | new direction |

The `0x435a20` mechanism is the one to understand. Passing `this` meant the
buffer died at the `_strlwr`, so **nothing was live across the call**. That one
liveness fact then decided five things at once: which register held `this`,
whether it had a stack home, where a neighbour spilled, whether the loop was
rotated, and which addressing mode an address computation used.

**The tell:** a long list of downstream symptoms, all following from one
allocation decision. Four observations of one fact, not four faults. Count the
symptoms before deciding how many bugs you have.

**The check, before touching a single declaration:** verify the argument list
against the original's argument pushes, and each callee's `ret N` from `ctx.py`.
One read, and it was worth 14 to 20 points every time it paid.

### 3. A matched sibling's odd-looking declarations are evidence about yours

`0x49c9c0` was 25% short and pointed at register allocation. The lead came from
`0x49cc20`, MATCHED, same tail, whose notes record two things its own function
needed: a `+0x1a` array of **four byte elements** addressed as `base + index*4`
for a 16-bit load, and a `+0x111` flag word used both as a plain int and as a
bitfield, which **requires a union holding both views**. `0x49c9c0` declared
both structures differently.

Same move fixed `0x4bc640`: a 9-byte packed entry meant the element address
needed `lea` plus `add`, and copying the index into a local flipped the order.

**When a matched sibling shares your tail, read its header notes before its
body.**

### 4. Grep the exe for raw instruction bytes, and count the hits

When a construct matches no library function, scan for its bytes.

`8a 10 88 11 8a 10 41 40 84 d2 75` (double load, two walking indices) has
**exactly three** hits in `TotalA.exe`. One is inside a MATCHED file that already
documents the shape. That one scan settled two things at once: the construct is
hand-written source, `do { a[k] = a[j]; k++; } while (a[j++] != 0);` over two
indices into one array, and the answer was already written down.

Ten bytes of python beats forty source shapes. See
`build/scratch/scan-bytecopy.py` in my fork for a 12-line version.

### 5. Declaration order decides commutative-add operand order *and* SIB base

The precise rule, replacing the looser version I first published:

1. MSVC 5 emits the **later-declared** operand of a commutative add first.
2. The **first** operand becomes the **base register** of a two-register SIB
   address.

Both effects matter, and **several such adds in one function are coupled**.
`0x4bb2e0` had three, each wrong differently, and one declaration order fixed
all three: declaring the loading-block locals top-down as `size, off, k, p`
satisfies `off + size`, `k + off` and `p[k]`. Any other order fixes at most two
and breaks the third. Per-site operand swapping thrashes.

A sibling can appear to follow the opposite convention purely because its
declarations are in the other order. That is one rule, not two.

**With a non-power-of-two stride the lever is a local, not a declaration
reorder**: `0x4bc640`'s 9-byte entry gives `(base + 9i)` inline and `(9i + base)`
through `int i = f->index`, and the second put the address in `edi` and fixed an
unrelated block's loads.

### 6. A modified `__stdcall` pointer parameter is a register with no stack home

To get that you must **stop modifying the parameter**. The original keeps such a
pointer in a register, does `mov reg, eax` after each call, and pushes it at
every call site. Writing `dest = f(dest, n)` against the parameter makes MSVC
treat it as memory, delete the result stores, and re-read the incoming argument.

Declaring `char* out = dest;` and updating `out` took `0x4ba000` from **42.0% to
98.7%**, opcode similarity 35.7% to 79.7%.

This is the mirror image of "force a field to be re-read", and the instinct runs
the wrong way. Both rules are live; check which applies.

### 7. Spelled-out shapes that kept coming up

| construct | what it must be | evidence |
| --- | --- | --- |
| zeroing a struct | `memset(p, 0, sizeof(T))`, not `field = 0` | 60.4% to ~76% |
| a repeated `x != -1` | the test must name a **different variable** or it is dropped | `0x4bcb50` |
| a byte from packed data | `unsigned char`, not `int` or `char` | `0x4caa40` |
| a 16-bit field read | `*(int*)(p+off) & 0xffff`; a `(unsigned short)` cast narrows the mask | `0x4caa40` |
| a `switch` | sparse gives `cmp/jg` pairs over **signed**-sorted values; dense gives `sub eax,0/je/dec/jne` | `0x4c9530` MATCH in 1 run |
| string literals | placement is **not** work; `check.py` compares contents at the target address | 32 diffs resolved free |
| a counted loop | `do`-while inside `if (i <= n)` beat a plain `for` by **13 points** once | `0x438ea0` |
| a `for` latch | moving latch statements into the increment clause changes rotation; **but not always** | `0x4bc640` yes, `0x4bcf80` no |
| repeated failure tests | one `if`/`else if`/`else` chain, **failure arms first** | `0x4bbe50` MATCH |
| chained struct access | one-line `static inline` reference-returning accessors, nested | `0x4e2ab0` MATCH |
| a buffer size | `sub esp, N` with the array at `[esp+0x10]` means the array is `N` | `0x4bbe50` |

### 8. Frame arithmetic reads backwards

For a function with `char` buffers, **the array offsets pin the buffer sizes and
the frame total pins the number of int locals**. `char to[20]`, `char status[64]`,
`char cmd[200]` gave 0x10, 0x24, 0x64, with four 4-byte homes below them making
`sub esp, 0x11c`. So a 16-byte deficit with correct array offsets means **four
int locals, not a padding field**.

Two related facts: a frame one dword too small is usually **one variable MSVC
gave no stack home**, and that shifts *every* displacement rather than being a
separate fault; and a wrong frame size is often a **declaration-scope** problem,
since promoting one block-scope variable to function scope once changed a frame
size, two slot offsets and an import call together.

### 9. Confirm the baseline before you sweep anything

The N-declaration calibration for header state is only meaningful if the
header-free prelude you calibrate from **reproduces the current output exactly**.
And confirm the current output from the differ before you start. Assuming your
starting state is what you think it is, then reporting a sweep as flat, is how a
broken baseline gets mistaken for a compiler property.

### 10. A flat sweep is a result

The header/compiler-state lever is real (`0x4399f0`: two attempts stuck at
94.8% with *identical source*) and it is not universal:

| function | result |
| --- | --- |
| `0x4399f0` | worked: `<stdlib.h>+<math.h>+<memory.h>`, or `<windows.h>` alone |
| `0x438ea0` | dead: flat 54.0% for every N from 0 to 312 |
| `0x4bcb50` | dead: 21 real sets byte-identical, N flat 0-316 step 4 and 0-63 step 1 |

**Sweep it; do not assume it in either direction.** Record the numbers either
way. `0x4399f0` documents the calibration: N unused `extern int` declarations in
front of the source with no headers matches for N = 43 to 298, period 512, half
of each period matching.

Likewise, **thirty rejected source shapes is a diagnosis, not a failure**: it
says compiler state, and tells the next model not to keep reshaping source.

### 11. Free iteration is nearly all of the budget

```
tools/wcl /c /O2 /Ob2 /MT -I<repo>/include /Fa<lst> /Fo<obj> <src>
objdump -d -M intel <obj>
```

Ten functions matched on 1 to 3 `check.py` runs because variants were screened
here instead. Side-by-side differs:

- `build/scratch/0x4df280/diff.py` (in my fork): offset-aligned against the original.
- `build/scratch/0x4bcb50-hdr/` and `0x4bcf80/`: batch drivers, summaries.
- An **opcode-sequence** differ beats an offset-aligned one when the byte counts
  already match and only a register differs.

Scroring scratch variants with `check.py --sym` or against a scratch source does
not count against the run budget, so use it freely.

### 12. When matching the bytes would require a spelling you believe is buggy

`0x4caa40`'s height guard is `dec esi / js / inc esi` in the original. A worker
first read it as `if (--h < 0) { h++; ... }` storing `h-1`, a decoder that drops
the last row of every image, kept a different guard (`cmp esi, 1 / jl`) at a cost
of 0.7%, and reported a possible original bug. That reading was wrong: the `inc`
restores `h` before the row counter is saved (0x4cab3a), and each row then counts
it down to 0, so `h` rows are decoded (#5772). The original is a
`for (rows = h - 1; rows >= 0; rows--)` loop and matches exactly.

The lesson stands with a better example. If matching the bytes seems to require a
spelling you believe is buggy, **trace the stored value through the loop before
calling it a bug** (count the iterations for h = 0, 1, 2), and say which spelling
you kept and why, so the residual reads as a decision rather than an oversight.

### 13. Re-derive inherited "suspected original bug" notes

`0x435a20` inherited a note saying the original passed `(char*)this`, recorded as
a *suspected original bug*. It does not: at 0x435ac7 it computes
`lea eax, [esp+0x18]` and pushes the address of a lowercased buffer.

A confident-sounding note is the most dangerous thing in a partial, because it
closes off the investigation that would fix the function. Also true of a note
reading "the original does this odd thing" with no operand shown.

Related: **count the pushes outstanding before trusting any `esp` displacement**
in a hand-read disassembly. My own note claimed two re-reads both returned `dz`;
with two pushes outstanding one returned `dz` and the other returned `p3`.

---

## Part 2: managing subagents

### Never guess a session id

I sent a redirect to the wrong worker's session. The worker **refused to touch a
file that was not its own**, which was correct, and its refusal came with a free
read of the worktree that caught a stale premise I had sent. That is the system
working, but the whole turn was wasted.

Keep a table of address to session id. Do not infer it from the file, because:

**All workers share the model name, so their `// Decompiled by` lines are
identical, and the first line of a file does not tell you who owns it.**

### Brief workers on what the previous attempt rejected

The rejected-variants list is the most expensive thing to produce and the
cheapest to reuse. A retry that re-runs thirty known-failed shapes burns its
whole budget. Pass the previous file's header notes forward explicitly and say
"do not repeat these".

### Send the highest-value rule first, with the evidence

The instruction that paid off most was: *before touching a declaration to fix a
register difference, verify the argument list against the original's pushes and
each callee's `ret N`*, followed by the three cases and their point gains. It is
one read and it was worth 14 to 20 points each time.

### Watch for the wrong lever, not just slow progress

A worker grinding a completed 36-variant sweep of declaration orders is not slow,
it is misdirected. A sweep of *operand order* has the worst record of any lever
for a *register allocation* symptom, because that symptom keeps turning out to
be an argument or type error. When you see a systematic sweep, check which lever
it is before assuming the worker is stuck.

### Say when a hypothesis is a hypothesis

I told a worker the header lever was the untried option on the strength of one
precedent. It was dead. The brief said explicitly: *"this is a hypothesis, not a
prediction; a clean negative is a real result I will publish, and do not invent a
reason to keep going."* That is why the negative came back clean and measured
rather than rationalised.

### Redirect by continuing the session

Redirect a running worker by passing its `sessionID`, with an explicit list of
what to stop and what to do instead. State which premises are wrong; a worker
that trusted a stale item will otherwise keep working on it.

### Shared worktrees need stated prohibitions

When several workers share one worktree, every brief needs, verbatim:

- Only ever edit the one named file. Never touch another worker's.
- **Never run `git checkout`, `reset`, `rebase`, `pull`, `merge`, `clean` or
  `stash`** - each destroys another worker's uncommitted work.
- **Do not commit.** Leave the file on disk; the orchestrator handles commits.
- Scratch only under `build/scratch/<addr>/`.
- `toolchain/` and `orig/` are gitignored, so a fresh worktree has neither.
  Symlink or copy them in before anything will run. This cost two workers an
  hour each, invisibly, until every tool failed.

### One issue at a time, all workers on it

Spreading workers across issues leaves many half-finished with nothing mergeable
on any of them. But note the honest tension: concentrating workers does **not**
help an issue with one function left, so there is a point where slots idle. That
is the right trade, and it is not a reason to re-claim ahead - an empty claim
costs other agents time because the claim's purpose is to stop duplication.

### Verify every result yourself

Never report MATCH on a worker's word. Every PR in my session was re-checked
with `check.py` before publishing.

### Verify the push, then the pull request

Branch names are **not** unique on the shared fork. `pr-43adc0` and `pr-435a20`
both already existed from my own earlier, merged PRs; the push was rejected as
non-fast-forward and `gh pr create` still produced a PR pointing at the *old*
content. Both had to be closed and republished.

Procedure: check the push exit status **before** creating the PR; confirm the
remote hash matches local HEAD; then check the PR's file list and commit message
before reporting it done. Append `-rN` when reusing a name.

Note that a successful `git push` piped through `Select-String` can still yield
exit code 1 while having succeeded, so compare hashes rather than trust the
shell's status.

---

## Part 3: environment notes that cost time

These are Windows-host specifics from this session. They are probably not
general, but they cost hours.

- **`uv` and the toolchain only work inside WSL.** `uv` is not on the Windows
  PATH.
- **PowerShell eats `$var`, `|`, `;` and `$(...)` inside `wsl ... bash -lc "..."`,
  even with single quotes.** Write a shell script into `build/scratch/` and run
  it. Do not fight the quoting.
- **`gh --jq` expressions with spaces get split by PowerShell.** Use
  `| Out-String | ConvertFrom-Json`.
- **`gh issue edit --add-assignee @me` fails on a pull-only account**
  (`ReplaceActorsForAssignable` denied). The "Claimed by" comment alone is a
  valid claim.
- **WSL git has no credential helper.** Push with
  `git -c "http.extraHeader=Authorization: Basic $b64" push fork <branch>` and
  set identity repo-locally.
- **A worktree created with a Windows path in its `gitdir` is unusable and
  unremovable under WSL git.** Fix: `rm -rf .git/worktrees/<name>` and the
  directory, then `git worktree prune` and re-add. Check for real work first.
- **A fresh worktree is stale and the main checkout may be too.** Read reference
  files with `git show origin/main:<path>` (`uv run tools/sources.py <addr>` gives the path).
- Several agents share the same account, so branch names do not identify
  authorship and another session's pull requests appear in your author list.

---

## Part 4: findings still needing a decision

*Orchestrator status (2026-09-28): item 1 is done (0x472d30 and 0x470770 are
now `std::vector<T>::size`, and `data/aliases.csv` covers the second
`std::copy`). Item 3 is not a bug: `fcomp; fnstsw; test ah, 0x40` is MSVC's
ordinary `!= 0.0f` / `== 0.0f` test (see the guide). Item 6 is recorded in the
guide's "known wall" note on `vector::insert`.*

1. **Four functions are byte-exact and refused by the checker** because
   `data/symbols.csv` folds two COMDAT `std::vector<T>::size` instantiations
   after their own placeholder file addresses. `0x472d30` and `0x470770` are
   really `std::vector<T>::size`, and `std::copy` needs an aliases row. Verified
   fix: re-match `src/graphics/particles_472630.cpp` as
   `&std::vector<Elem_00473500>::size` and update the rows, which releases all
   four at once.
2. **The callee-saved register rotation wall, now 8 functions**: `0x4861d0`,
   `0x48a870`, `0x46a610`, `0x48d790`, `0x48c190`, `0x4732e0`, `0x438ea0`,
   `0x425480`. `0x43adc0` came off this list once its argument list was
   re-checked, so **re-examine the others' argument lists before treating the
   label as real**. A `check.py` diagnostic reporting *why* a register was
   chosen would pay for itself across all of them.
3. **An x87 zero-compare inversion** affecting six functions, where `0x405300`
   and `0x403a20` contradict each other on the same construct.
4. **`0x48a870` is not an overflow bug**: its on-water branch computes
   `(draft*0xffff + seaLevel) << 16`, which wraps to exactly
   `(seaLevel - draft) << 16` (the `draft * 2^32` term vanishes in 32 bits; checked
   over all 65,536 byte pairs), the same value the neighbouring branch computes
   directly (#5772).
5. **Similarity percentage can rise while code is deleted.** Always read the byte
   counts, not just the percentage.
6. The exe holds **both** register variants of `vector<T>::insert` (`0x46e640`
   and `0x408f30` common, `0x4732e0` and `0x425480` rarer), so those two are not
   fixable from source.

---

## Part 5: the 2026-10-02 batch

*Same model, a later batch: 27 matched functions, of which seven came out of the
rules below. Everything here was measured on this binary with `tools/check.py`, and
the ruled-out levers are reported with the measurement that ruled them out.*

### The address-temporary pool

MSVC 5 keeps **address temporaries in a three-slot pool that rotates
`edx -> eax -> ecx`, and when there is more than one use it assigns the LATER call
site first.** Measured on a minimal model that reproduces a 504-byte function's
hunk byte for byte:

| live address values | registers assigned, in source order |
| --- | --- |
| 1 | `edx` |
| 2 | `ecx, edx` |
| 3 | `ecx, edx, eax` |
| 4 | `ecx, edx, eax, ecx` |
| 5 | `..., ecx, edx` |

It is simply the lowest-numbered free register, with `eax` consumed by a nested
call's result. **So "which register does this `lea` get" is a question about how
many address values are live, not about the statement** — and the intuitive model,
that the first use gets the best register, is the *opposite* of what happens. A
pool-filling lab confirms it (0x45f8c0).

### A local pointer is the highest-yield single lever on the project

One lever, and it decides **two** independent questions:

- **A store through a local pointer alias blocks MSVC's disjointness proof and
  keeps a load it would otherwise eliminate.** `float* slot = &unit->field_bc;
  *slot = f;` made it re-emit a sibling field's load, where a direct
  `unit->field_bc = f;` let it drop the load. The pointer **folds back to a
  constant offset**, so the stored-to addressing is unchanged: a reload with no
  disturbance to the access. It also beat a union, which is the shape usually
  tried first (0x464f80).
- **A local pointer is the only way to force a reload, and it costs no
  instructions.** MSVC 5 CSEs two plain reads of a field and reloads only when an
  aliasing store or a call sits between them — **a deleted store does not defeat
  the fold** — but `unsigned int *flags = &unit->flags;` forces the reload while
  emitting nothing extra, and it flipped a bonus register choice as well
  (`and eax,0xffc3ffff`, 5 bytes, replacing `and edx,...`, 6). That one change
  took a 1811-byte function from 76.9% to byte-identical (0x487bf0).

### SIB and `lea` operand order

- **The deepest cause, and it is not about spelling: a temporary used BOTH as a
  memref base and as a plain int comes out of the memref with its children in the
  other order.** The swap only happens when two reads of the same address are
  **CSE-mergeable**. On 0x4a4d70 the colour read and a style argument both read
  `me->colours` at `+0x1f`; the front end shared one temporary between them and
  that sharing flipped the slot. Making the second read a **non-duplicate**
  (`entries[param_2].colours`) restored the original's order. Thirty
  re-spellings of the style read at the same address were byte-identical and a
  block-scope alias did nothing — **the answer came from deleting one region of
  the function at a time.** So if every spelling comes back identical, you are
  probably in the CSE-sharing case, and the lever is to make one read
  non-duplicate, not to re-spell either.
- **A block-scope named local alias does flip the SIB slot** where thirty
  in-expression spellings did not — but it moves the pointer register too, because
  a materialised global prefers EAX/EDX/ECX while a local prefers EAX/ECX/EDX.
  Write the two requirements down as a *pair*; on 0x493bf0 they were mutually
  exclusive under every spelling tried.
- **Only byte-scaled deltas can take the base slot; an element-scaled one cannot**,
  because x86 forces a scaled index into the index slot (same rule as
  `0x4bc370`'s `[int + ptr + 0]`). And the operand is *separable* from the slot:
  one variant on 0x471de0 reaches the delta-base slot **and** keeps the library's
  `sub ecx,ebx`. When an experiment moves one property and not the other, you
  have found the axis, not a dead end.
- **If the original puts the older value in the base and you put the newer one
  there, check whether your `lea` is an outlier inside its own function.** On
  0x40cca0 *every other* `lea`, matching or not, already puts the older value in
  the SIB base. Also: the wanted form is **one byte longer** (`disp8=0`), so total
  size is useless as an oracle on that family — use the lea's base register.
- **The `void **` MSVC builds for `(void **)&x` always lands at x's own slot + 4**
  — checked against three different locals in three shapes, always exactly 4
  (0x4b5510).
- **A commutative `lea`/`add` whose base/index assignment will not move is a
  property of the small translation unit, not of the function, and it is
  independent of the element type.** `vector<T>::insert` carries the identical
  single byte with `T` = `int`, `Unit*`, `Class_004c2ea0*` and `Elem_0040cfb0`
  (0x46e640, 0x408f30, 0x425210, 0x40cca0). The exe holds both register variants,
  so treat it as one problem with several members and do not spend boxes on the
  siblings individually.

### Order and placement

- **MSVC 5 never hoists a load out of an if-block** (proved with a minimal file).
  So a load that appears before a test but seems to come from inside a branch
  means the original's source genuinely reads it *before* the branch — a
  source-shape question, not a scheduler one.
- **Where a load lands depends on whether the address is taken.** MSVC 5
  materialises the load of a plain **non-address-taken** local at its
  *definition* point, and at its *use* point if the address is taken. Making a
  local non-address-taken produced an original's exact five-instruction sequence
  including the register (0x4b5510).
- **The tree order is strict source order, with an argument push spliced
  immediately before a through-pointer store.** So a residual of the form "these
  three instructions are rotated" can *never* be fixed by reordering statements
  within the group — the load and the store have to come from different source
  statements. Recognise that shape before testing all six orders.
- **To stop MSVC hoisting a load above a branch, duplicate the block rather than
  `goto`.** MSVC 5 tail-merges arms that share a `goto` target, and the hoist was
  a consequence of the merge; giving one arm **its own copy** made the merge
  impossible and produced the original's order (0x464f80). So "every if/else
  versus early-goto respelling is byte-identical" means the *other* arm wants its
  own copy.
- **Give each of two structurally identical statements its own scope.** A function
  stuck at 99.6% on one stack-slot ordering was fixed by putting each of two calls
  in its own nested block with its own local: the first block is dead when the
  second opens, so both locals land in the same stack slot and both statements
  emit the wanted form with the frame unchanged. **Nesting only one of the two
  grew the frame and scored 98.1%**, and nesting only the other got the order
  right but moved the struct — the two effects are separable and both had to be
  satisfied (0x491200, MATCH).
- **RGEN promotes a memory object only while nothing conflicts across the
  branch.** A union written by two stores of different widths is exactly such an
  object: promotion fails, the value spills (15 bytes here), and the pointer is
  forced into the other register. Register choice depends on a *promotion*
  decision, which is a different question from allocation — check it separately
  whenever a union is involved. A corollary that explains minimal-repro failures:
  **a range can be value-numbered with another arm's loop counter** that has to
  survive several calls (0x4a3ef0).
- **A constant zero CSEs away; a zero-fill does not.** To get a fresh
  `xor ecx,ecx` plus `push ecx` you need *runtime* zero — the aggregate's own
  zero-fill, or `20u & ~(unsigned)(size_t)z`. A literal `0` folds into whatever
  other zero the block already has, deleting the register being chased. Several
  passes on 0x4bd160 had this rule exactly backwards.
- **Copy-into-a-second-register is decided by whether the operation can be done in
  place.** MSVC breaks a value's live range into another register only for an
  operation it cannot perform in place: `~`, `^`, `&`, `|` need the copy and each
  leaves its own op behind, while `+`, `-`, `*` can be done in place and drop it.
  So a residual of the form `op eax; op2 eax` against a bare `mov eax,imm` means
  **the original's operand was independent, not computed**.
- **`x ? x : x` works by failing codegen's "is this a load" test, not by creating
  a phi.** MSVC 5 does not fold it away; it declines to treat the value as a bare
  load, so an operand that must stay in a register cannot be folded into the next
  instruction's memory operand. `& 0x7fffffff` does the same at the cost of a
  5-byte `and`. This explains why the construct is worth 15.2 points on one
  function and a byte-for-byte no-op on another: it only bites when the consumer
  has a memory-operand form to fold into (0x45f8c0, 0x47d2e0).
- **Local declaration order decides load order, independently of the order the
  values are used in.** This extends §5 of Part 1 (which covers commutative-add
  operand order): on 0x45f8c0 the multiply's operands are read through locals
  declared in the **opposite** order to the multiply, because the original loads
  the first one first. All six other orderings gave the same registers with the
  two loads swapped. When a load order is wrong and the expressions are right,
  invert the *declarations*.
- **A missing initialiser can be the fix.** `int x2; int y2;` beat every spelling
  that assigned them, because the original never initialises them at all.
- **One instruction of source movement can move a jump past a reload.**
  `processed = 1;` after a call, rather than before, pushed an arm's loop-back
  `jmp` past a reload that only *another* arm needed, because that arm clobbers
  `ebp` (0x487bf0).
- **Check whether a workaround is compensating for a bug elsewhere.** A `Found()`
  shim existed only because a different arm was wrong; once that arm was fixed,
  three others fell into place and the shim had to be **deleted** (0x487bf0).
- **The `sizeof`/argument-count is not the frame.** A `void **` argument does not
  scale the way a real pointer array does, so passing one does not grow the frame
  the way passing an array would; a template parameter instantiated at different
  granularities changes the amount of code emitted without changing the frame
  (0x4bd160, 0x5f9c00).
- **`register` is a no-op on VC5 and useless as a lever.** 52 unused `register`
  variables changed nothing, because MSVC 5 drops them in the front end *before*
  register allocation, so they reserve nothing.
- **Comma expressions drop their left operand** in argument position (measured on
  all 20 argument sites), so they cannot smuggle a node past the front end.

### Headers, and what they decide

**Run `tools/headers.py <addr> --cpp` before concluding that a register choice is
a compiler-internal coin.** On 0x47d2e0 five-plus notes attributed `mov di`
against `mov si` to "a colour tie-break inside MSVC 5's LCL, not a source-order
effect", and it was the **header set**: `<string.h>` **plus** `<math.h>` — neither
used by the file — deciding the allocation, worth 4.9 points. Either header alone
gives the same state; `<windows.h>` gives a 1392-byte frame.

`<minmax.h>` was added to that sweep recently and any file using min/max macros
was invisible to it before. It is **not** universal: the sweep came back flat on
the `vector::insert` family and on 0x4bd160 and 0x425210. Run it anyway — it is
cheap and it is silent.

### Method

- **Screen on binary features, never on the percentage.** `check.py`'s ratio only
  moves on a near-match, so on a register-allocation or instruction-order residual
  it is nearly useless. Pick two or three discrete facts about the original's
  bytes and print those per variant: 6560 variants scored on four features while
  the percentage sat *completely still*, and that is what surfaced a 90.6%
  two-exit `if`+`while` shape earlier passes had never seen. A register pair, a
  one-letter-per-call-site oracle, or the frame-slot order all work; §11 of Part 1
  and this paragraph are the same point from two directions.
- **Fix size before hunks, and read the delta as a *difference* of two shapes.**
  When ours is longer, count epilogues — but the prediction is refutable, and on
  0x487bf0 the tool showed 19 epilogue starts in both streams and no cloned tail.
  The real story was a shim's 7 instructions **minus** the 3 a neighbouring arm
  should have had. Build the align tool anyway; it cost one compile and refuted
  the hypothesis in one pass. Compare epilogue *counts*, not just bytes, when ours
  is longer, because a tail just under MSVC's sharing threshold gets cloned into
  every predecessor (112 bytes over and 13 epilogues against 7, on 0x447b10).
- **Build a minimal model of the differing region, then a pool-filling lab.**
  `min.cpp` reproduced a 504-byte function's register hunk byte for byte at 0.3 s a
  compile, and sweeping the number of live address values produced the whole
  assignment table in one run. **3,400 real-function compiles and 14,559 permuter
  candidates on that function produced nothing the model did not already
  explain.**
- **Run the permuter from several seeds, including deliberately worse shapes.** One
  MATCH came from a 54%-scoring shape in 14 seconds, where six workers permuting
  the best file gained nothing — because the ratio cannot see register-only
  differences. Conversely, 9,768 and 7,712 candidates from the *best* file on two
  functions changed nothing but cosmetics, which upgrades "that is where I
  stopped" to "**that is a local optimum of the mutation space**" and saves the
  next attempt a box.
- **A one-instruction-short shape that scores correctly means a missing node.**
  Adding one register-consuming node put the target register at both call sites on
  0x45f8c0, and the only reason it was not the answer is that the node emitted one
  instruction. Record it as "the original had one more node than any spelling I
  can produce, and it emitted no instruction" — far more useful than "all
  spellings flat".
- **Settle an allocator question in a simpler sibling, not the hard function.**
  0x47d820 and 0x47d0e0 both hinge on one coin — which register the sign-extended
  short gets, `ecx` in one and `eax` in another — and 0x47d0e0 settles it in a
  small harness because it contains that instruction in isolation. That one coin
  then explains five other differences across the family. When one instruction's
  register blocks a whole function, find the sibling that has it alone.
- **Read the exe's stack traffic to work out which variable a load means.** The
  biggest single gain in one session was a misidentified variable: a `y1` that
  actually read the loop counter's dead argument slot, worth 54.0% → 64.3% *and* a
  frame move as a side effect. Build the frame map from **your own object file**
  (track the pushes and each callee's `ret N`; then `[esp+N]` is frame offset
  `N-0x10`) — the `/Fa` name-to-offset mapping has been misread repeatedly. And
  check whether the slot is a variable at all: one function's `[esp+0x24]` is an
  unnamed CSE temp, so no spelling of any variable can move it.
- **"Tried, flat" can mean "tried the wrong arity".** The clearest instance yet:
  five passes recorded that every hoist shape was flat, when the correct shape was
  **one** hoisted pointer and **one** nested expression at 98.0% shape-matched,
  while the two-temporary forms everyone actually tried scored 34–36%. When a
  negative spans a family of shapes, check that the family's members differ in
  the dimension that matters.
- **A recorded conclusion is conditional on the frame state it was measured in,
  and it can invert.** "Declare `bit` before the Contains test, worth several
  points" was true at 82% and became **exactly backwards** at 86.9%. When a big
  change lands — a header set, a frame layout, a scope move — re-run the earlier
  conclusions rather than carrying them forward. (This refines Part 1 §13: the
  negative was true, just not now.)
- **A register tie-break blamed on the LCL is often the header set** — see the
  section above. Put it with the size-delta rule: both are cases where a plausible
  compiler-internal explanation was really something in the build.
- **Transplant the real neighbouring function when it is MATCHed.** This is much
  stronger than the knife-edge note about the number of functions in a file: on
  0x408100 one small `static inline` helper was **neutral**, but adding
  **0x408090** — the function that actually preceded it in the original
  translation unit, and whose two extra `g_game` field reads are needed —
  **flipped MapRange's two loads to the original's order**. Sweep it, do not
  apply it blindly: on that same function it also moved the score down, 98.5% to
  98.0%.
- **Verify the compiler is deterministic before treating a flat sweep as a fact.**
  12 parallel compiles of the same file produced the same SIB byte, which is what
  licenses reading a negative as evidence about the compiler rather than noise.
- **A suspected original bug must be checked against the original's bytes, not
  your source.** 0x487bf0's notes carried an uninitialised read at frame 0x28;
  that word is written and read by the arm that owns it, and **our own wrong
  spelling was what read a stale word.** Reporting a bug that rests on your
  decompilation being right costs the reviewer a box to disprove.

### Tooling and environment

- **MSVC 5 gives a `float` *literal* an 8-byte `.rdata` slot.** The checker then
  compares its 4 padding bytes against the next function's constant, so a file can
  report **100.0% and still fail with a data-reference error** — the seen state was
  `100.0%` with `100.0f`'s padding against the following function's `12700.0f`.
  Naming the constant makes it a 4-byte object, and declaring doubles before
  floats keeps it last in `.rdata`. If you see 100% with a bad reference, this is
  why.
- **Import `check.py`'s `compare` directly to score variants in-process.** About
  0.2 s instead of 7 s, which is what makes a 3000-variant sweep affordable
  inside a timebox.
- **`check.py`'s object tag is a hash of the file's *parent directory***, so a
  parallel batch needs one directory per variant, not one filename per variant.
- **CL.EXE compiles a `.c` file as C**, so a behaviour harness written with C++
  syntax must be named `.cpp` or it fails with a misleading
  `missing ')' before '*'`.
- **`pkill -f <script>.py` will kill your own shell** if the pattern matches the
  parent command line. Kill by PID.
- **In-shell `cp`/`sed` writes lose stray `)` and `:` characters.** Write files
  with a heredoc or an editor call instead of shell string surgery.
- **`sed -i` prints `preserving permissions ... Invalid argument` in these
  worktrees and still applies the edit.** Do not treat it as a failure.
- **`build/obj/scratch/<tag>/` is case-insensitive under Wine**, so one case per
  tag, unique per batch. A reused tag silently overwrote a variant on another
  address.
- **Never name a scratch module after a stdlib module.** `copy.py`, `dis.py` and
  `bisect.py` each broke `pefile`, capstone and `random` in turn.
- **A wrapper that imports `tools/permute.py` must carry the same PEP 723
  dependency block and guard `permute.main()` with `if __name__ == "__main__"`**,
  because the forkserver re-imports the script as `__mp_main__`. And
  `multiprocessing` needs `get_context("fork")` here.
- **`/tmp/opencode` is not writable on this box**; keep scratch in
  `build/scratch` inside the worktree.

### Two process rules that cost boxes

- **Verify an inherited harness before believing the notes it produced.** This is
  the **fourth** tool defect on this project to corrupt the record, after a reused
  `build/obj/scratch` tag, the wrong `/Fa` name-to-offset mapping, and a generator
  that renamed identifiers *before* substituting so every variant it produced had a
  doubled frame. The fourth: a sweep script mis-merged its variant dicts and
  printed a bogus "delta-base" line for a variant that recompiles walker-base.
  **A harness bug produces confident, specific, wrong output, which is worse than
  no output.** Re-derive any prior result that came from a harness you have not
  re-verified, and rebuild generators rather than extending them. Re-check any
  surprising positive against the `/Fa` listing before it goes in your notes.
- **On a rebase conflict, diff the stages before choosing.** `git show :2:<file>`
  (upstream) against `git show :3:<file>` (yours) — during a rebase **theirs is
  your commit**. Taking it blindly discarded 21 lines of an independent earlier
  account of the same shape, including two facts the newer notes lacked. Keep
  both: a superseded note that contradicts the new one costs nothing as a comment,
  and losing one costs the next attempt a measurement. A conflicted `git pull
  --rebase` also leaves `HEAD` detached, so `git-safe push` will refuse the push
  until the rebase is resolved.

---

## Part 6: corrections, and one family closed

*Added after Part 5. Two claims in Parts 4 and 5 were too weak and are corrected
here, and one six-function family is now proven unreachable. Please fold these into
Parts 4 and 5 rather than leaving them to contradict.*

### `std::vector<T>::insert`: closed, and it is the build, not the source

**Superseded by Part 7: the family is reachable with `/Gi` per file, and six of
these inserts now match. The measurements below still hold for the default
flags.**

Part 4 item 6 said "the exe holds both register variants of `vector<T>::insert`,
so those two are not fixable from source". Part 5 said the byte is "a property of
the small translation unit". **Both were too weak. The correct statement is
stronger: the mov/lea split is a property of the compiler build, the six stuck
inserts cannot be matched with this toolchain, and no source or translation unit
we can compile reaches it.**

**The evidence is a matched sibling, and it is one comparison.** In the exe,
`0x408f30` (`vector<Unit*>::insert`, wants `lea eax,[ebx+ecx] / sub / sub`,
546 bytes) and `0x4c4d70` (`vector<Class_004c3e40*>::insert`, has
`mov eax,ecx / sub / add / sub`, 547 bytes, **already MATCHED** by
`src/util/tdf.cpp`) are **the same template on the same 4-byte
dword-copied element**. Diff their instruction lists with jump targets masked and
you get 225 against 226 instructions and **exactly one replace plus one insert**,
both inside that group — and the same register assignment too (`_P` in ebx, `_Q`
in edx, dest in ecx, `_M*4` in edi, `_Last` in esi). So the exe's own compiler
emitted both shapes from one STL source.

- **All ten MATCHED 3-argument vector inserts produce the mov form; the six stuck
  ones (`0x408f30`, `0x40cca0`, `0x425210`, `0x44ec30`, `0x46e640`, `0x476210`)
  all want the lea form.**
- **The element type is not the variable.** Taking the MATCHED `0x4c4d70.cpp`
  verbatim and changing *only* the element type to `Unit*` — declared exactly as
  `src/ai/ai_player_408f30.cpp` declares it — still gives the mov form at 547
  bytes. One file style, both pointer-element instantiations, one shape; the exe
  has two.
- **~9,700 in-process compiles produced only those two shapes**: about 70 source
  spellings (destination expression, `_Ucopy` loop form and argument order with
  the call sites updated, swapped comparison operands, post-increment in the body,
  dead `(_P - _F)`, fill shape) crossed with 3 to 5 pad counts; 15 filler kinds at
  counts 0 to 400 plus prototypes to 6000; a genuinely large TU of real game code
  (832 KB, 32,306 lines, about 700 of our own matched files, each kept only after
  the whole file still compiled) with `<list>`/`<map>`/`<set>`/`<deque>`/`<string>`
  instantiation blocks before, after and around it; `BT_TOOLCHAIN=msvc5-rtm`; and
  24 flag variations. **For 15 different spellings the mov/lea flip happens at
  exactly the same filler count (17 unused prototypes), so the count decides and
  the text does not.**
- In the sibling `0x408f30` both forms are three-byte `lea`s, so the whole question
  is **one SIB byte** (wanted `0x0b`, ours `0x19`): 300 prototype counts give 126
  builds at ours and 174 at the mov form, **never `0x0b`**. Twelve parallel
  compiles of a file produce byte-identical code, so the flat sweeps are facts.

**Conclusion: the wanted byte is a one-byte difference in this compiler's handling
of the loop optimiser's synthesised add** — which of its two registers goes in the
SIB base slot when the emitter has a free register for the result. **The fix would
be a different C1XX/C2 build, not a rewrite. Do not spend boxes on this family.**

### The habit behind that: a matched sibling is a control, not a hint

**When one template instantiation is MATCHED and a sibling of the same template is
not, diff the two in the exe before spending a box on the sibling.** Here it took
one comparison and closed six functions. Two supporting habits:

- **A size difference between two instantiations of the same template is evidence,
  not noise.** 546 against 547 bytes was the entire signal.
- **Importing `check.py`'s `compile_source()`/`compare()`** scored variants at
  0.03 s each, which is what made 9,700 compiles affordable inside a timebox —
  and is the only reason the "exhaustive" claim above is credible rather than
  aspirational.

### A branch-hygiene incident worth reading

This one nearly destroyed other agents' work, and it is the failure mode the
parallel loop warned about, so the mechanism is recorded exactly.

I rebased a worktree branch onto moved `main`. `git-safe push` **correctly**
refused as non-fast-forward. But `gh pr create` still **succeeded** against the
stale fork branch, and that pull request's diff against `main` was **86 files,
+4,077/−7,272** — including reverts of `tools/check.py`, `tools/headers.py`,
`tools/record.py`, several `data/*.csv`, and other agents' `src/unsorted/` files.
Merging it would have reverted a lot of work.

The fix: push the rebased commit to a **fresh branch name**, open a new pull
request from that, and comment on the bad one asking for closure (this token
cannot close pull requests itself).

**So: a successful `gh pr create` is not evidence that the diff is right. Always
check `git diff --stat origin/main <the branch you actually pushed>`, and do it
after a fetch, immediately before the push.** Part 5's rebase-conflict advice is
the companion piece: diff `:2:` against `:3:` before choosing, because during a
rebase *theirs* is your commit.

### For the orchestrator: two `data/symbols.csv` items

1. **`0x40cca0` looks wrong.** `data/symbols.csv` names it
   `UElem_0040cfb0::?$vector::insert`, but the file defines the decorated
   `=?insert@?$vector@UElem_0040cfb0@@V?$allocator@UElem_0040cfb0@@@std@@@std@@QAEXPAUElem_0040cfb0@@IABU3@@Z`,
   and the other `vector::insert` files use the decorated form. `check.py` warns
   about exactly this mismatch. Suggested: record it decorated, like its siblings.
2. **Part 4 item 1 is still open and releases four functions**: four are
   byte-exact and refused by the checker because `data/symbols.csv` folds two
   COMBAT `std::vector<T>::size` instantiations after their own placeholder file
   addresses. `0x472d30` and `0x470770` are really `std::vector<T>::size`, and
   `std::copy` needs an aliases row. Worth doing before more boxes go into this
   family.

---

*Model: opencode / space-bunny-free. Every function named as MATCH here was
verified with `tools/check.py` at 100%. Ruled-out levers are reported with the
measurement that ruled them out, not as "did not work".*

---

## Part 7: the vector insert family is `/Gi`, not a different build

*Added 2026-10-03. Corrects Part 6's verdict on `std::vector<T>::insert`.*

Part 6 closed the six stuck inserts as needing a different compiler build. They
do not. Some of the original's translation units were built with `/Gi`
(incremental compilation, found in #5035), and a file can ask for it with a
`// FLAGS: /Gi` line of its own: `check.py` and the other tools then add `/Gi`
for that file only, and `/Gi` is the only flag a file may add. Under it, each
insert below matches with the stock `#include <vector>`, the member pointer to
`insert`, and **one ordinary use of one other member of the same vector type**.
Which member it is decides the bytes, so try each:

| Function | Element | MATCH with | Other uses (under `/Gi`) |
|---|---|---|---|
| 0x475ef0 | 0x44-byte record | `operator=` or `resize` | `reserve`, copy ctor: 83.0% |
| 0x476490 | 32-byte record | `operator=` or `resize` | `reserve`, copy ctor: 60.8% |
| 0x4758c0 | 0x34-byte record | `operator=` or `resize` | `reserve`, copy ctor: 84.3% |
| 0x475bd0 | 0x3c-byte record | `reserve` or copy ctor | `operator=`, `resize`: 82.6% |
| 0x476210 | 32-byte record | `reserve` or copy ctor | `operator=`, `resize`: 61.2% |
| 0x46e640 | `int` | `reserve`, `resize` or copy ctor | `operator=`: 58.0% |

Those six are landed. The other four of Part 6's six were measured on scratch
copies only and are not landed yet: 0x408f30 (`Unit*`), 0x40cca0 (3-byte
`Elem_0040cfb0`), 0x425210 (`unsigned short`) and 0x44ec30 (two shorts) each
MATCH with `// FLAGS: /Gi`, the real `<vector>` and a `reserve` or copy-ctor
use (0x408f30 and 0x425210 also with `resize`). With no other use they are
99.1% to 99.3%.

- **Mechanism** (#5035, from `/Fa` listings): `/Gi` numbers internal IL symbols
  per function instead of with one TU-wide counter. That reverses the ties that
  pick `_P`'s register, the `_N`/`_S` frame slots and the SIB base of the
  synthesised `lea`. Part 6's finding that a filler count, not the text, flips
  the `mov`/`lea` shape was the same tie seen from the default-flags side.
- **`/Gi` is per TU, not global.** In #5035, 15 of 48 sampled MATCHED functions
  (and 11 of the 12 inserts matched before this) stop matching under `/Gi`. Add
  it only to a file it moves to MATCH.
- **The member use stands in for the TU's own.** The callers of 0x475bd0 and
  0x476210 really call `reserve`. The callers of the `operator=` group also call
  `reserve`, so their TU did something else as well that we have not found.
- **Parallel `/Gi` compiles collide.** `/Gi` writes `vc50.idb` into the
  compiler's working directory (the repo root), so two at once fail with
  `C1033: cannot open program database`. `checkall.py`, `progress.py` and
  `permute.py` all compile in parallel. Until `compile_source()` passes a
  per-object `/Fd` (measured: it moves the `.idb` beside the object and leaves
  the bytes unchanged), run `checkall.py` on `/Gi` files one address at a time.
  `vc50.idb` is not gitignored; never commit it.

*Model: Claude Code / opus. Every MATCH above was verified with `tools/check.py`
without `--flags`; the percentages were measured with the `// FLAGS: /Gi` line in
place.*
