// Decompiled by Opus, finished by deepseek-v4.1-flash, finished by Claude Opus 5.5. Names are provisional.
// Draws the frame of one cell of the 16x16 "COLS" colour grid gadget
// (cell index = row * 16 + column, each cell 8 pixels).

#pragma pack(push, 1)
struct Gadget_004ac970 {               // 0x15b bytes
    char unknown_0[0x13];
    short x;                           // +0x13
    short y;                           // +0x15
    short width;                       // +0x17
    short height;                      // +0x19
    int attribs;                       // +0x1b
    char unknown_1f[0xb6 - 0x1f];
    short count;                       // +0xb6
    char unknown_b8[0xba - 0xb8];
    short selected;                    // +0xba
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

int __stdcall FindGadgetIndex(void* gadgets, const char* name, int flag);
void __stdcall DrawRectangle(void* param_1, void* param_2, int param_3);

// The x of one cell of the grid: the grid gadget's x plus the cell gadget's.
// Must stay a defined helper: its presence affects the caller's codegen.
static inline int CellX(Gadget_004ac970* gadgets, int index)
{
    return gadgets[index].x + gadgets->x;
}

// Stays out of dialogs_4aa8f0.cpp: it matches only while the symbol ids of
// its file stay small, and no header set or declaration count in the joined
// file reaches them (docs/c2-regalloc.md).
// FUNCTION: 0x4ac970
void __stdcall DrawColorCellOutline(Object_004ac970* obj, int cell, int color)
{
    Gadget_004ac970* gadgets = obj->holder->gadgets;
    // Lookup result goes through its own local before index.
    int index, found = FindGadgetIndex(gadgets, "COLS", 6);
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
