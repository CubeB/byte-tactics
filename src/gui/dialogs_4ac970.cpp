// Decompiled by Opus, finished by deepseek-v4.1-flash, finished by Claude Opus 5.5. Names are provisional.
// GPT-6.1-sol issue 3121 retest: 96.6% after four checks. Reversing y operand
// order, splitting y into short locals, and moving x before y did not change
// codegen. Remaining difference: the two y loads use ecx/edx in swapped order.
// Verified by GPT-6.1-sol for #1705: best retained score 83.1%; not a MATCH.
// Codex / GPT-6 retest in #13:
// a rectangle constructor, a drawing helper, coordinate updates and
// reordered rectangle stores changed register allocation without a match.
// GPT-6.1-sol retry in #1705: the retained baseline scores 83.1%. Pair
// aggregate and separated coordinate updates scored 56.2% and 71.2%; all 128
// header combinations topped out at 83.1%. Remaining differences include
// surface and coordinate register assignment and final rectangle stores.
// Refinement: moving the surface and rectangle declarations among the
// coordinate locals, and using a selected-gadget pointer, did not exceed 83.1%.
// GPT-6.1-sol issue 2309 refinement: reordered the x/y initializers, then
// tried reading grid x through an explicit grid pointer before computing y.
// Both retained 83.1%. A pointer rewrite compile probe failed on a duplicate
// x local. Six checker invocations total. Remaining diffs are surface and
// coordinate register assignment, followed by rect stack slots/stores.
// deepseek-v4.1-flash issue 2930 retry: 96.6%. Two changes to the baseline:
// x is `unsigned int` (not int), and rect.right/rect.bottom are written as
// rect.left + 7 / rect.top + 7 instead of repeating the full coordinate
// expression. That puts surface in edi and x in ebx and matches the original
// store scheduling. The only remaining diff is the pair of y loads: the
// original puts gadgets->y in ecx and gadgets[index].y in edx, ours swaps
// those two scratch registers (the sum still lands in ecx either way).
// Claude Opus 5.5 (found with tools/permute.py): MATCH. Three changes, each
// needed: the cell's x comes from a small inline helper, CellX; the lookup's
// result goes through its own local before `index` (96.6% without it); and the
// grid's y is read straight into the y sum, not through a `baseY` local (93.2%).
// The helper counts as a definition the compiler has seen: writing its sum out
// inline still matches while CellX stays defined, and drops to 93.2% once it
// is gone.
// Draws the frame of one cell of the 16x16 "COLS" colour grid gadget
// (cell index = row * 16 + column, each cell 8 pixels).

#pragma pack(push, 1)
struct Gadget_004ac970 {               // 0x15b bytes
    char unknown_0[0x13];
    short x;                           // +0x13
    short y;                           // +0x15
    char unknown_17[0xbc - 0x17];
    void* surface;                     // +0xbc
    char unknown_c0[0x15b - 0xc0];
};
#pragma pack(pop)

struct Holder_004ac970 {
    int unknown_0;
    Gadget_004ac970* gadgets;          // +0x4
};

struct Object_004ac970 {
    char unknown_0[0x18];
    Holder_004ac970* holder;           // +0x18
};

struct Rect_004ac970 {
    int left;
    int top;
    int right;
    int bottom;
};

int __stdcall FUN_0049fdf0(void* gadgets, const char* name, int flag);
void __stdcall DrawRectangle(void* param_1, void* param_2, int param_3);

// The x of one cell of the grid: the grid gadget's x plus the cell gadget's.
static inline int CellX(Gadget_004ac970* gadgets, int index)
{
    return gadgets[index].x + gadgets->x;
}

// FUNCTION: 0x4ac970
void __stdcall FUN_004ac970(Object_004ac970* obj, int cell, int color)
{
    Gadget_004ac970* gadgets = obj->holder->gadgets;
    int index, found = FUN_0049fdf0(gadgets, "COLS", 6);
    void* surface = gadgets->surface;
    index = found;
    short cellY = gadgets[index].y;
    int y = gadgets->y + cellY;
    Rect_004ac970 rect;
    rect.left = (unsigned int)CellX(gadgets, index) + (cell % 16) * 8;
    rect.top = y + (cell / 16) * 8;
    rect.right = rect.left + 7;
    rect.bottom = rect.top + 7;
    DrawRectangle(surface, &rect, color);
}
