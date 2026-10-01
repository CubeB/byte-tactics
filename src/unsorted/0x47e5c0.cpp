// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, reworked by Claude Sonnet 5.5, finished by space-bunny-free, finished by GPT-6.1-sol. Names are provisional.
// GPT-6.1-sol retry (#2427): best remains 67.5% (382/386), no MATCH after 3 checks. Remaining mismatch is register allocation: target keeps grid in edi and inner y in esi, with size.y loaded into edx before saved-register pushes; current build assigns grid to esi and y to ecx. Swapping sum order and sumy operands did not improve it.
// Pass #1958 (space-bunny-free): still 67.5% (382/386), no MATCH, 1 check.py
// run. What is new and worth keeping:
// (1) The frame CONTENT already matches exactly: both have the same eight
// dword homes, sumx at +0x00, the two Pt copies at +0x0c/+0x10, xend at
// +0x1c, and the same 0x20 frame, with sumy in the dead arg2 slot and the
// inner counter reloaded from the arg3 slot. Only FOUR slots are permuted
// (original: grid +0x04, x +0x08, yend +0x14, y +0x18; ours: x +0x04,
// y +0x08, grid +0x14, yend +0x18). So do not chase the frame layout: it is
// a symptom. The single root cause is the callee-saved assignment, and it
// cascades into the slot order.
// (2) The original's [esp+0x10] compare in the overlap test is sumx, not the
// grid pointer: with push ebp in effect [esp+0x10] is frame+0x00, which holds
// the sumx spill. There is no stray-pointer bug in the test, the four
// conditions read pos.x, sumx, pos.y (arg1.y at [esp+0x36]) and sumy
// (arg2 slot at [esp+0x38]) exactly as written.
// (3) Hoisting is now measured against the original's own code ORDER (all
// four cell bounds computed before the first guard, in the order y, xend,
// yend, x) and it still loses: 41.5% (379 B) to 44.5% (381 B) for six
// declaration orders and both for-init styles, versus 67.5% for the
// combined-init inner for. Hoisting only yend is 54.3% (384 B), hoisting
// only x is 67.5% (382 B, same code), moving the grid declaration after the
// sums is 54.5% (383 B). The 67.5% allocation is a strong local optimum:
// swapping the guards, merging them into one if, and both operand orders of
// sumx and sumy all give byte-identical 382-byte output.
// (4) Still to find: how to make the allocator give esi to the inner
// counter y and edi to the grid pointer, with size.y in the volatile edx
// (the original loads size.y into edx BEFORE the pushes, which is why its
// lea of the grid can take edi; in the 67.5% shape size.y is loaded after
// the pushes and takes edi itself).
// Retry #1748: GPT-6.1-sol confirmed 67.5% (382/386) after eight checks; no MATCH. The best source still differs in register allocation and stack-slot placement.
// Claude Sonnet 5.5 pass (#746): no change beat 67.5% (382 bytes). Compiler state
// is not the lever: the declaration-count sweep (0 to 400) is 67.5% only for N = 0
// and 8 and worse (43.3 to 58.1%) everywhere else, and no header set beats 67.5%
// (best other set 51.2%). Source shapes scored on top of the older list: the two
// overlap tests as a `static inline Hits(pos, sumx, sumy, o)` helper and as one taking
// a four-int rect (both 390 bytes, 51.2%); declaring all four grid bounds
// (x1, y1, x2, y2) before the loops in eight orders and looping `for (x = x1; x <= x2;
// ...) for (y = y1; y <= y2; ...)` (all 386 bytes, the original size, but 50.8 to
// 54.0%). In those the frame slots differ from the original by one dword: the original
// keeps sumx at [esp+0xc], the grid pointer at [esp+0x10] (edi), xstart at [esp+0x14],
// sumy in the dead arg slot [esp+0x34], yend at [esp+0x20], ystart at [esp+0x24] and
// xend at [esp+0x28], while these put grid at [esp+0x1c] and xstart at [esp+0x10].
// 67.5%: the loop bodies and the value sequence match, but MSVC picks a
// different set of registers. The original loads size.y into edx before the
// pushes, keeps the grid pointer in edi and the y counter in esi; this source
// gets size.y into edi after the pushes, grid in esi and the y counter in ecx,
// then spills y. Hoisting ystart/yend/xend as separate variables, swapping the
// sum operand order or making Pt copies all compile to the same (wrong)
// allocation or worse; any change that moves size.y before the pushes flips
// the whole allocation.
//
// Retry notes (deepseek-v4.1-flash): computing ys right after y2 into its own
// local (instead of in the inner for-init) does put size.y back in edx and
// size.x in ecx, but the allocator still gives the grid pointer esi and the
// loop counters end up elsewhere, scoring 45-52 percent. Declaring x1/y1/x2/y2
// plus all four bounds collapses to the v1 allocation (grid esi, x ecx, y edx).
// What is still missing is making the y counter outrank the grid pointer for
// esi (so grid takes edi); no source-level ordering tried made that happen.
//
// More retries (deepseek-v4.1-flash): the 67.5% shape is the combined-init
// `for (int y = (pos.y>>3)-1, ye = (sumy>>3)+1; y <= ye; y++)`, still the best
// found. Neither operand order in sumx/sumy (`size.x + pos.x` etc.), nor
// old-style `int x, y;` declarations, nor `while` loops, nor hoisting size.y
// into its own local, nor computing xend before sumy beats it. They all land
// in the same allocation: MSVC loads size.y into edi just after the pushes
// (so the `lea` of the grid pointer takes esi), keeps the x counter in edx and
// the y counter in ecx. The original instead loads size.y into edx before the
// pushes, then `lea edi, [eax+0x1429f]` for the grid, and keeps y in esi.
// What is needed is to stop size.y from occupying edi at the grid lea point.

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

struct Game_0047e5c0 {
    char unknown_0[0x1429f];
    Grid_0047e5c0 grid;                     // +0x1429f
};
#pragma pack(pop)

extern Game_0047e5c0* g_game;

// Scans every grid cell overlapping the rectangle [pos, pos+size), calling
// visitor->FUN_0047ed30 for each object (and child) whose own rectangle overlaps.
// FUNCTION: 0x47e5c0
void __stdcall FUN_0047e5c0(Pt_0047db20 pos, Pt_0047db20 size, Class_0047db20* visitor)
{
    Grid_0047e5c0* grid = &g_game->grid;
    int sumx = pos.x + size.x;
    int sumy = pos.y + size.y;
    int xend = (sumx >> 3) + 1;
    for (int x = (pos.x >> 3) - 1; x <= xend; x++) {
        for (int y = (pos.y >> 3) - 1, ye = (sumy >> 3) + 1; y <= ye; y++) {
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
                    if (pos.x < cs.x + cp.x && sumx > cp.x && pos.y < cs.y + cp.y && sumy > cp.y) {
                        visitor->FUN_0047ed30(c);
                    }
                }
            }
        }
    }
}
