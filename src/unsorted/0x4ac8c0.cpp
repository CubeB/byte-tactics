// Decompiled by deepseek-v4.1-flash, finished by LongCat 2.5 Preview Free and
// space-bunny-free. Names are provisional.
// Draws the 16x16 "COLS" colour grid of a gadget, each cell 8x8 pixels; the
// cell index (0..255) is the fill colour. Inverse of 0x4acbe0, sibling of
// 0x4ac970 (which draws one cell's frame).
// check.py: 88.1%, still not a match. Two differences, both the scheduler
// placing p.surface's memory access one slot too early: the store of
// p.surface in the prologue (the original has it between the load of
// gadgets->x and the add that uses it) and the reload of p.surface in the
// loop (the original has it after the store of p.r.right, which then lets it
// reuse eax for the surface and ecx for ebx + 7).
#pragma pack(push, 1)
struct Gadget_004ac8c0 {               // 0x15b bytes
    char unknown_0[0x13];
    short x;                           // +0x13
    short y;                           // +0x15
    char unknown_17[0xbc - 0x17];
    void* surface;                     // +0xbc
    char unknown_c0[0x15b - 0xc0];
};

struct Holder_004ac8c0 {
    int unknown_0;
    Gadget_004ac8c0* gadgets;          // +0x4
};

struct Object_004ac8c0 {
    char unknown_0[0x18];
    Holder_004ac8c0* holder;           // +0x18
};
#pragma pack(pop)

struct Rect_004ac8c0 {
    int left;                          // +0x0
    int top;                           // +0x4
    int right;                         // +0x8
    int bottom;                        // +0xc
};

// The surface and the rectangle share one local: the surface is spilled before
// the loop and reloaded in it, and keeping both in a single aggregate is what
// reproduces the original's load order and its two stack slots.
struct Pair_004ac8c0 {
    void* surface;                     // +0x0
    Rect_004ac8c0 r;                   // +0x4
};

int __stdcall FUN_0049fdf0(void* gadgets, const char* name, int flag);
void __stdcall FUN_004bf6f0(void* surface, Rect_004ac8c0* rect, int color);

// FUNCTION: 0x4ac8c0
void __stdcall FUN_004ac8c0(Object_004ac8c0* obj)
{
    Gadget_004ac8c0* gadgets = obj->holder->gadgets;
    int index = FUN_0049fdf0(gadgets, "COLS", 6);
    Gadget_004ac8c0* grid = &gadgets[index];
    Pair_004ac8c0 p;
    int y = gadgets->y;
    int x0 = grid->x;
    p.surface = gadgets->surface;
    x0 += gadgets->x;
    y += grid->y;
    for (int row = 0; row < 16; row++) {
        int x = x0;
        for (int col = 0; col < 16; col++) {
            p.r.right = x + 7;
            p.r.left = x;
            p.r.top = y;
            p.r.bottom = y + 7;
            FUN_004bf6f0(p.surface, &p.r, row * 16 + col);
            x += 8;
        }
        y += 8;
    }
}
