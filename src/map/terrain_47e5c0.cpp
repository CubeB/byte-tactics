// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, reworked by Claude Sonnet 5.5, finished by space-bunny-free, finished by GPT-6.1-sol, finished by space-bunny-free. Names are provisional.
// MATCH (space-bunny-free). The blocker the earlier notes describe is real, and it is
// a register-allocation symptom of ONE thing: the original computes all four cell
// bounds plus both sums in the function's straight-line preheader, before the outer
// loop guard, while the 67.5% shape sinks the two y bounds past that guard.
// build/scratch/0x47e5c0/differ.py aligns 83 of the original's 124 instructions with
// ours and shows the 18 differing blocks all cascading from that one scheduling
// choice, so the winning shape hoists xstart/ystart/sumx/xend out of the for-inits.
// All 720 orders of the six hoisted declarations and 192 structural loop shapes built
// on them were swept and none beat 54%, because the hoisted shape then gives edi to ye
// and esi to the grid, where the original has ye in the volatile eax and the grid in
// edi. What matched came from running the permuter FROM the hoisted shape, which it had
// never been given: `uv run tools/permute.py 0x47e5c0 --file
// build/scratch/0x47e5c0/hoisted2.cpp --minutes 11 --seed 13 --jobs 10` went 54.0% ->
// 67.5% -> 75.8% -> 80.6% -> 100% in 14 seconds. Its score is a finer measure than
// check.py's ratio, and that is what found this.
//
// Three things in the body look redundant and are NOT. Each was checked, and the score
// in brackets is what removing it gives.
//  - `#include <math.h>` is load-bearing [41.3%]; <stdio.h> is not, so only <math.h>
//    is included.
//  - `int sumy;` is declared and only assigned four lines below [initialising it at the
//    declaration: 43.4%]. The split stops MSVC fusing the two uses of the sum, which is
//    what keeps sumy in the volatile edx.
//  - `ye` recomputes `pos.y + size.y` rather than shifting `sumy` [75.5%]. Writing the
//    sum twice, once through the local and once spelled out, leaves the shift unfusable
//    and keeps ye in eax, as the original has it.
// The declaration order (xstart, ystart, sumx, xend, sumy, ye) is load bearing too: the
// natural order (sumx, sumy, xstart, xend, ystart, ye) is 54%.
//
// The record of what was tried and did not work follows.
//
// Tried and worth not repeating (all verified with check.py):
//  - The byte differ's first finding: the frame CONTENT already matched in the 67.5%
//    shape (same eight dword homes, sumx at +0x00, the two Pt copies at +0x0c/+0x10,
//    sumy in the dead `size` argument slot, the inner counter reloaded from the dead
//    argument slot). Only four slots were permuted, all of it downstream of the
//    callee-saved assignment, so the frame layout was never the thing to chase.
//  - The original's [esp+0x10] compare in the overlap test IS sumx (frame+0x00 with
//    push ebp in effect), not a stray grid pointer: the four conditions read pos.x,
//    sumx, pos.y (arg1.y) and sumy (the arg2 slot) exactly as written.
//  - Declaration-order sweep of the hoisted shape: 720 orders x 2 operand orders of
//    sumx and sumy, best 54.0%. Structural sweep (bounds hoisted or inline x 2 y-init
//    forms x 3 guard shapes x guard order x grid via helper x sum operand order x 2
//    x-bounds placements, 192 shapes): best 67.5%, i.e. the old local optimum.
//  - Compiler-state sweeps on the 67.5% shape: 0 to 16 uncalled `static inline`
//    functions in four body shapes. Good only for N = 0..4 (67.5%), then 386 bytes
//    at 58.1% for N = 5..6 and 390 at 51.2% for N = 7..9. Nothing better.
//  - By-value and scalar helpers: CellLo/CellHi on the bounds, SumXY/SumYX taking the
//    two points by value, the overlap test as one helper taking the rect, HeadAt /
//    HeadOf on the cell, GridOf() with no argument: all 51-68%, none better. On the
//    hoisted shape the same helpers made it worse (43-54%).
//  - The 0x47eee0 mechanism (a pointer re-assigned as the first statement of a loop
//    body to block a [reg+disp] fold and let LICM hoist the lea): `Grid* g = grid;` at
//    the top of the row or column loop, a row pointer `grid->cells + grid->width * y`,
//    a cell pointer, a cells pointer. All four compile to exactly the same 382 bytes as
//    the base, so MSVC 5 folds every one of them away here.
//  - The code-free dead store and the self-assignment: `int t = 0; if (t) sumy =
//    sumy | t;` before and after the sums, on sumx, on xend, on the y counter, and
//    `size = size;` / `pos = pos;` at the head: all byte-identical to the base, except
//    a store to a still-live variable, which costs 8 points (52.6% at 384 bytes).
//  - `register` on the counters, the grid declared after the sums, `Grid* const`,
//    no grid local at all (writing g_game->grid.width out four times, as the matched
//    siblings 0x47e750 and 0x47e890 do), the outer loop as a `while`, the inner one
//    too: 42-67.5%, nothing better.
//  - Five permuter runs on the 67.5% file (seeds 11, 12, 7, 13, 21, 3768/3748/1380
//    candidates) found no improvement at all. The permuter only paid once it was
//    pointed at the hoisted shape (see above), which is the single useful lesson here.
#include <math.h>

struct Obj_0047e5c0;

class Class_0047db20 {
public:
    virtual void FUN_0047ed30(Obj_0047e5c0* obj);
};

struct Pt_0047db20 {
    short x;
    short y;
};

#pragma pack(push, 1)
struct Obj_0047e5c0 {
    char unknown_0[0x76];
    Pt_0047db20 pos;                        // +0x76
    char unknown_7a[4];
    Pt_0047db20 size;                       // +0x7e
    char unknown_82[8];
    Obj_0047e5c0* child;                    // +0x8a
    Obj_0047e5c0* next;                     // +0x8e
};

struct Cell_0047e5c0 {                      // 10 bytes
    char unknown_0[6];
    Obj_0047e5c0* head;                     // +0x6
};

struct Grid_0047e5c0 {
    Cell_0047e5c0* cells;                   // +0x0
    unsigned int width;                     // +0x4
    unsigned int height;                    // +0x8
};

struct Game {
    char unknown_0[0x1429f];
    Grid_0047e5c0 grid;                     // +0x1429f
};
#pragma pack(pop)

extern Game* g_game;

// Scans every grid cell overlapping the rectangle [pos, pos+size), calling
// visitor->FUN_0047ed30 for each object (and child) whose own rectangle overlaps.
// FUNCTION: 0x47e5c0
void __stdcall VisitObjectsInArea(Pt_0047db20 pos, Pt_0047db20 size, Class_0047db20* visitor)
{
    int sumy;
    Grid_0047e5c0* grid = &g_game->grid;
    int xstart = -1 + (pos.x >> 3);
    int ystart = -1 + (pos.y >> 3);
    int sumx = pos.x + size.x;
    sumy = pos.y + size.y;
    int xend = (sumx >> 3) + 1;
    int ye = ((pos.y + size.y) >> 3) + 1;
    for (int x = xstart; x <= xend; x++) {
        for (int y = ystart; y <= ye; y++) {
            if (x >= grid->width) {
                continue;
            }
            if (y >= grid->height) {
                continue;
            }
            for (Obj_0047e5c0* o = grid->cells[grid->width * y + x].head; o != 0; o = o->next) {
                Pt_0047db20 op = o->pos;
                Pt_0047db20 os = o->size;
                if (pos.x < os.x + op.x && sumx > op.x && pos.y < os.y + op.y && sumy > op.y) {
                    visitor->FUN_0047ed30(o);
                }
                for (Obj_0047e5c0* c = o->child; c != 0; c = c->next) {
                    Pt_0047db20 cs = c->size;
                    Pt_0047db20 cp = c->pos;
                    if (pos.x < cs.x + cp.x && sumx > cp.x && pos.y < cp.y + cs.y && sumy > cp.y) {
                        visitor->FUN_0047ed30(c);
                    }
                }
            }
        }
    }
}
