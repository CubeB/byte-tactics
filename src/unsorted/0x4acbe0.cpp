// Decompiled by Opus. Names are provisional.
// Converts a screen position to the index of a cell in the 16x16 "COLS"
// colour grid gadget (row * 16 + column, each cell 8 pixels).
// (stdio.h only for its effect on register allocation; see tools/headers.py)
#include <stdio.h>

#pragma pack(push, 1)
struct Gadget_004acbe0 {               // 0x15b bytes
    char unknown_0[0x13];
    short x;                           // +0x13
    short y;                           // +0x15
    char unknown_17[0x15b - 0x17];
};
#pragma pack(pop)

struct Holder_004acbe0 {
    int unknown_0;
    Gadget_004acbe0* gadgets;          // +0x4
};

struct Object_004acbe0 {
    char unknown_0[0x18];
    Holder_004acbe0* holder;           // +0x18
};

int __stdcall FUN_0049fdf0(void* gadgets, const char* name, int flag);

// FUNCTION: 0x4acbe0
int __stdcall FUN_004acbe0(Object_004acbe0* obj, int x, int y)
{
    Gadget_004acbe0* gadgets = obj->holder->gadgets;
    int index = FUN_0049fdf0(gadgets, "COLS", 6);
    Gadget_004acbe0* grid = &gadgets[index];
    int col = (x - gadgets->x - grid->x) / 8;
    int row = (y - gadgets->y - grid->y) / 8;
    if (col < 0) {
        col = 0;
    }
    if (col > 15) {
        col = 15;
    }
    if (row < 0) {
        row = 0;
    }
    if (row > 15) {
        row = 15;
    }
    return row * 16 + col;
}
