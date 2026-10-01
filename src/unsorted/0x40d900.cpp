// Decompiled by DeepSeek V4.1 Flash, finished by Claude Opus 5.5, edited by deepseek-v4.1-flash,
// deepseek-v4.1-flash (#3770 round): still 98.7% (191 bytes, exact), the one hunk
// is unchanged. Four more store-first spellings confirmed dead at 93.7%
// (`&cells[i << 10]`, `(Cell*)((char*)cells + (i << 10))`, a scalar
// `unsigned int base = (unsigned int)cells;` temp and a named `int off = i << 10;`
// all emit `mov edi,ebp` / `shl edi,0xa` / `add edi,ecx`), and reading the bits
// through `unsigned int* dp = &dirty[i]` with the store after it collapses the
// whole function to 78.5% (`mov ecx,ebp` for the loop index); reading through the
// pointer while keeping `p = cells` before the store is byte-identical at 98.7%.
// Only the `mov edi,[esi+0x1c]` base-load slot differs (ours right after the `je`,
// original after `mov [eax],0`).

// deepseek-v4.1-flash (#4170 round, final): still 98.7 (191 bytes, exact), the
// same single hunk (ClearLast's `mov edi,[esi+0x1c]` load emitted right after
// the `je` instead of after the `mov [eax],0` store). ~320 scored variants this
// round (all logged in build/scratch/0x40d900/), none above 98.7:
// * The outcome is a strict binary switch. The load-first two-statement form
//   (`Cell* p = cells;` before the store, `p += i*256;` after) loads the base
//   straight into edi (p's home) and the offset into edx, and emits the load
//   first (98.7). Every store-first form (any spelling) makes the code
//   generator defer the base load to the add, so the offset is allocated first
//   into edi, the base lands in ecx or edx, and the `mov ebx,edx` bits copy is
//   emitted after the base load (93.7).
// * New shapes tried this round: named base/offset locals in every order
//   (`base = cells; off = i*256; store; p = base + off;` and permutations)
//   reach 96.2 with the base in edx and the offset in edi, still the wrong
//   roles; `for`-init pointers, comma expressions (`(cells, &cells[i*256])`,
//   optimized away), byte-pointer (`unsigned char*`) grids, direct
//   AISearch-style fields instead of the Grid sub-object, inline methods
//   defined out of class, static free helpers, reversed method definition
//   order, `int`/`unsigned int` count and dirty types, shared function-scope
//   bits, a second `mask = bits` variable (88.6, different prologue), extra
//   typedef/function state declarations (flat), and seven standard headers
//   (flat) all reproduce one of the two schedules.
// * The two schedules are the same allocator/live-range tie the earlier
//   rounds describe: the bits copy can only free edx for the offset if it is
//   placed at the start of the then-block, and this compiler only places it
//   there when the base load comes first in the source.
// deepseek-v4.1-flash (#4170 round, 30-min checkpoint): still 98.7 (191 bytes,
// exact), same single hunk (ClearLast's `mov edi,[esi+0x1c]` load emitted right
// after the `je` instead of after the `mov [eax],0` store). This round ran ~190
// scored variants (all logged in build/scratch/0x40d900/): all pointer
// spellings, offset types, statement orders, split pointer forms, value- and
// pointer-parameter helpers, inline-vs-method bodies, caller loop spellings,
// extra `mask = bits` variables and guard spellings either keep 98.7 with the
// load first or drop to 93.7 with the offset in edi and the base in ecx. The
// allocator only reuses edx for the offset when the `mov ebx,edx` bits copy
// has already freed it; every source that stores before it loads cells emits
// that copy too late. Same wall as the earlier rounds.
// deepseek-v4.1-flash (#3265 round): re-confirmed 98.7; no new angle on the ClearLast base-load slot after the store-first/load-first sweep above.
// deepseek-v4.1-flash, and GPT-6.1-sol, edited by deepseek-v4.1,
// finished by deepseek-v4.1-flash.
// Names are provisional.
// deepseek-v4.1-flash (#3023 retry): still 98.7% (191 bytes, exact). Only diff:
// ClearLast emits `mov edi,[esi+0x1c]` 3 slots early. Load-first spellings give the
// original registers (edi base / edx offset) at 98.7 but hoist the load; every
// store-first spelling gives the right slot order but swaps to ecx base / edi offset
// (93.7%). headers.py swept all 128 sets, all flat. Scheduler/live-range tie.
//
// Partial (98.7%): clears the kind byte of every cell in each dirty group of
// eight cells, then clears the dirty masks. One dirty word covers 256 cells
// (0x400 bytes). Every full block is cleared unconditionally; the last block
// is bounds-checked against the cell count.
//
// The first loop's inlined body matches exactly. The last block's body also
// has the original's registers (ebx = dirty word, edx = i << 10, edi = base
// pointer) and all of the original's instruction order except one: the
// original loads cells into edi after the `dirty[i] = 0` store, ours loads
// it three slots earlier, right after the guard's `je` (see the note below).
// What puts the base in edi and the shift in edx is declaring the pointer
// inside the guard, after the `bits` load and before the store: any shape
// with the store before the `cells` load coalesces the shift into edi and
// takes a scratch for the base (93.7%).
//
// still-differs note (deepseek-v4.1, ~45 check runs): the only hunk left is
// the ClearLast tail: the original emits `mov ebx,edx` / `mov edx,ebp` /
// `mov [eax],0` / `mov edi,[esi+0x1c]` / `shl edx,0xa` / `add edi,edx`,
// while this version emits the `mov edi,[esi+0x1c]` base load immediately
// after the `je` (three slots early) and everything else in the original's
// order. Register allocation is right (edi = base, edx = i << 10, ebx =
// dirty word); only that load's slot differs.
// Load placement rule found by bisection: as long as `p = cells;` stands
// before the `dirty[i] = 0;` store in the source, MSVC gives p edi and the
// offset edx (98.7%) but places the load first; with the store first MSVC
// coalesces the offset into edi and loads the base into a scratch (93.7%:
// `mov edi,ebp` / `shl edi,0xa` / `add edi,ecx`), whether the pointer is
// written as `&cells[i*256]`, `cells + i*256`, `(char*)cells + (i<<10)`,
// `p = cells; p += ...`, `p = p + ...`, `p = &p[i*256]`, a named `int off`,
// a `T&` store (`unsigned int& word = dirty[i]; word = 0;`), a pointer store
// (`unsigned int* dp = &dirty[i]; *dp = 0;`) or a comma expression that
// forces the store first (`p = (dirty[i] = 0, ...)`). Two pointer locals
// (`base`/`p`) do not change it. The same helper inlined twice with a
// constant `last` (the shape the original evidently came from) also gives
// 93.7%.
// Re-confirmed by deepseek-v4.1 (11 more check.py runs): store-first written
// as `&cells[i*256]`, `cells + off`, a named `base`, a byte-cast
// `(char*)cells + (i << 10)`, or with the offset in its own
// `unsigned int bytes = i << 10;` temp all give the same 93.7%
// (`mov edi,ebp` / `mov [eax],0` / `mov ecx,[esi+0x1c]` / `shl edi,0xa` /
// `add edi,ecx`); a pointer local `unsigned int* pd = &dirty[i]` changes the
// whole function's allocation (78.5%, the loop index moves to ecx). Only the
// base-load-first order reaches 98.7%.
//
// round 2 (deepseek-v4.1): new data points on that one hunk. With the store
// first in every spelling tried (`dirty[i] = 0;` textually before any mention
// of `cells`, split or single-expression) the load does land after the store
// but the allocator then puts the base in ecx and the offset in edi
// (`mov edi,ebp` / `shl edi,0xa` / `add edi,ecx`, 93.7%), so the slot is
// governed by the same live-range decision that picks the registers, not by
// statement order: while `dirty` is still live in ecx the base can only go to
// edi, and by then the load is already scheduled. Load-first spellings
// (`Cell* p = cells;` first, even before the `bits` load: 98.7%) all give the
// original's registers with the load three slots early.
//
// round 3 (deepseek-v4.1-flash): the outcome is a binary switch. Load-first
// (`p = cells;` before the store) keeps base in edi and offset in edx (98.7%)
// but schedules the base load first; every store-first spelling gives 93.7%
// with base in ecx and offset in edi. Still 93.7/unchanged after: a separate
// `unsigned int off = i << 10;` before or after the store, byte arithmetic
// `(char*)cells + i*0x400` or `+ (i << 10)`, `i << 8` element indexing,
// `cells + 0` then `p = (char*)p + (i<<10)`, `p = &p[i * 256]`, the comma
// form `p = (dirty[i] = 0, cells)`, shared function-scope locals for both
// blocks, an `unsigned` index and parameter, and a `bits ? cells : cells`
// ternary (worse: +2 bytes). Adding an inline member helper (a `BlockAt` or
// `PutZero` method, or a sub-object `TakeDirty`) is inlined but shifts the
// whole loop's allocation (ebp/ecx swap, +2 bytes), so it is worse.
// Only the base-load slot in the last block still differs.
//
// Possible original bug: in the last block the bounds check starts `c` at
// `i << 8` again for every group of eight cells instead of at the group's own
// offset (group g holds cells i*256 + g*8 .. +7). So only the first group is
// really compared against the count; later groups clear cells past the end of
// the grid.
//
// Round 4 (deepseek-v4.1-flash retry): the single remaining hunk is still the
// ClearLast base load slot. New negatives, all confirmed from the /Fa listing:
// * Inlined accessor and member helpers (Grid::Block(i), Grid::Cells()),
//   static inline At(base, idx) and Add(base, off), and argument-order swaps
//   all give the same two schedules: load-first (98.7%, edi base / edx offset,
//   load emitted early) or any store-first spelling (93.7%, edi offset /
//   ecx base). No third schedule was found.
// * Exhaustive search of the 24 statement orderings of bits / store / pointer
//   start / pointer offset, plus pointer spellings (comma form, char cast,
//   i<<8, idx locals, reference local, double pointer), found nothing new.
// * The N-unused-declaration compiler-state sweep (extern int dNNNNN) is flat:
//   load-first 0..200 step 1, store-first 0..560 step 2.
// * Defining the real preceding function 0x40d8b0 (matched, own file) above
//   this one in the same file does not move the load.
// The load can only land after the store when the source stores before it
// loads cells, and that spelling always reallocates the offset to edi and the
// base to ecx. The two requirements are in conflict for this compiler state.

struct Cell_0040d900 {
    unsigned char kind;
    char unknown_1[3];
};

struct Grid_0040d900 {
    Cell_0040d900* cells;              // +0x0
    unsigned int width;                // +0x4
    unsigned int height;               // +0x8
    int count;                         // +0xc
    unsigned int* dirty;               // +0x10, one bit per 8 cells

    void ClearBlock(int i, int last)
    {
        if (dirty[i]) {
            unsigned int bits = dirty[i];
            dirty[i] = 0;
            Cell_0040d900* p = &cells[i * 256];
            while (bits) {
                if (bits & 1) {
                    int c = i << 8;
                    Cell_0040d900* q = p;
                    for (int k = 8; k; k--) {
                        if (!last || c < count)
                            q->kind = 0;
                        c++;
                        q++;
                    }
                }
                bits >>= 1;
                p += 8;
            }
        }
    }

    void ClearLast(int i)
    {
        if (dirty[i]) {
            unsigned int bits = dirty[i];
            Cell_0040d900* p = cells;
            dirty[i] = 0;
            p += i * 256;
            while (bits) {
                if (bits & 1) {
                    int c = i << 8;
                    Cell_0040d900* q = p;
                    for (int k = 8; k; k--) {
                        if (c < count)
                            q->kind = 0;
                        c++;
                        q++;
                    }
                }
                bits >>= 1;
                p += 8;
            }
        }
    }
};

class Class_0040d900 {
public:
    char unknown_0[0x1c];
    Grid_0040d900 grid;                // +0x1c

    void FUN_0040d900();
};

// FUNCTION: 0x40d900
void Class_0040d900::FUN_0040d900()
{
    int n = ((grid.count + 0xff) >> 8) - 1;
    int i;
    for (i = 0; i < n; i++)
        grid.ClearBlock(i, 0);
    grid.ClearLast(i);
}
