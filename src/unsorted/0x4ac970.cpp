// Decompiled by Opus, finished by space-bunny-free. Names are provisional.
// space-bunny-free retry in #1901: still 83.1%, and the residual is a single
// mirror-image register pair plus the store order it causes. The original puts
// the surface pointer in edi and the x accumulator in ebx, and keeps the left
// value in ebx across the whole `cell / 16` chain so that right = left + 7 is
// computed before top but stored after the call arguments are pushed. Every
// spelling that reproduces that store order (left, top, right, bottom) raises
// the register pressure by one and MSVC spills the cell argument to ebp and
// moves the rect one slot up, which costs far more than the pair swap:
//   left/top/right/bottom, four aggregate expressions  58.3%
//   left/top/right/bottom with y/x locals, right from the struct  65.0%
//   left/top/right/bottom, both recomputed  68.3%
//   left/right/top/bottom (this file)  83.1%
// also all 83.1%: swapping the x operands, a col/row pair of temps, and a
// `grid = &gadgets[index]` pointer; 71.2% for swapping the y operands or
// declaring x before y; 74.6% for a second syntactic read of gadgets->surface
// (one load, two uses, to weight the pointer the other way). tools/headers.py
// tried all 128 header sets: every one is 83.1%. The frame is 16 bytes for the
// address-taken rect only, so the surface really is register-resident.
// Verified by GPT-6.1-sol for #1705: best retained score 83.1%; not a MATCH.
// Codex / GPT-6 retest in #13:
// a rectangle constructor, a drawing helper, coordinate updates and
// reordered rectangle stores changed register allocation without a match.
// GPT-6.1-sol retry in #1705: the retained baseline scores 83.1%. Pair
// aggregate and separated coordinate updates scored 56.2% and 71.2%; all 128
// header combinations topped out at 83.1%. Remaining differences include
// surface and coordinate register assignment and final rectangle stores.
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
void __stdcall FUN_004bf8c0(void* param_1, void* param_2, int param_3);

// FUNCTION: 0x4ac970
void __stdcall FUN_004ac970(Object_004ac970* obj, int cell, int color)
{
    Gadget_004ac970* gadgets = obj->holder->gadgets;
    int index = FUN_0049fdf0(gadgets, "COLS", 6);
    void* surface = gadgets->surface;
    int y = gadgets->y + gadgets[index].y;
    int x = gadgets[index].x + gadgets->x;
    Rect_004ac970 rect;
    rect.left = x + (cell % 16) * 8;
    rect.right = x + (cell % 16) * 8 + 7;
    rect.top = y + (cell / 16) * 8;
    rect.bottom = rect.top + 7;
    FUN_004bf8c0(surface, &rect, color);
}
