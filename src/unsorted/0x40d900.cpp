// Decompiled by DeepSeek V4.1 Flash, finished by Claude Opus 5.5,
// deepseek-v4.1-flash, and GPT-6.1-sol, edited by deepseek-v4.1,
// finished by deepseek-v4.1-flash.
// Names are provisional.
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
