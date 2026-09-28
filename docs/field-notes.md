# Field notes: what actually produced matches

Notes from a decompilation session (opencode / space-bunny-free, 47 matched
functions). Everything here was measured with `tools/check.py`; where a
technique did not work, the measurement is given too, because a ruled-out lever
is as valuable as a working one.

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

`0x4caa40`'s height guard is `dec esi / js / inc esi` in the original, reachable
only from `if (--h < 0) { h++; ... }`, which **stores `h-1` and decodes one row
too few**. The worker kept the correct `cmp esi, 1 / jl`, cost 0.7%, and reported
it as a possible original bug with the evidence.

Matching the instruction stream there would have satisfied `check.py` and left a
decoder that drops the last row of every image. **Say which you did and why**, so
the residual reads as a decision rather than an oversight.

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
  files with `git show origin/main:src/unsorted/<addr>.cpp`.
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
   fix: re-match `src/unsorted/0x472d30.cpp` as
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
4. **`0x48a870` looks like a genuine original bug**: `draft*0xffff + seaLevel`
   overflows. Still unresolved.
5. **Similarity percentage can rise while code is deleted.** Always read the byte
   counts, not just the percentage.
6. The exe holds **both** register variants of `vector<T>::insert` (`0x46e640`
   and `0x408f30` common, `0x4732e0` and `0x425480` rarer), so those two are not
   fixable from source.

---

*Model: opencode / space-bunny-free. Every function named as MATCH here was
verified with `tools/check.py` at 100%. Ruled-out levers are reported with the
measurement that ruled them out, not as "did not work".*
