// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Draws the 16x16 "COLS" colour grid of a gadget, each cell 8x8 pixels; the
// cell index (0..255) is the fill colour. Inverse of 0x4acbe0, sibling of
// 0x4ac970 (which draws one cell's frame).
//
// Best 84.7% (167 vs 167 bytes). The frame, prologue, call, loop structure,
// loop tails and the rectangle stores are byte-identical. What still differs
// is only MSVC's scheduling/register choice, which no source spelling tried
// moved (hundreds of variants: operand order, declaration and assignment
// order, grid pointer vs gadgets[index], inline helpers, Rect constructor,
// aggregate init, temp locals, stdio.h and other headers, const, int/unsigned
// types, function return types):
//   - head: the original loads gadgets->y into ebx while the index scaling is
//     still running and keeps the x accumulator in esi starting from
//     grid->x; we always load gadgets->x first with esi and take grid->y into
//     ebx. Writing y before x instead moves `gadgets` out of edi into esi
//     (72.9%).
//   - body: the original reloads the spilled surface with `mov eax,[esp+0x10]`
//     after storing rect.right and uses edx for &rect, ecx for bottom; we
//     hoist the same load into edx at the top of the loop and use ecx for
//     &rect, eax for bottom.
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

int __stdcall FUN_0049fdf0(void* gadgets, const char* name, int flag);
void __stdcall FUN_004bf6f0(void* surface, Rect_004ac8c0* rect, int color);

// FUNCTION: 0x4ac8c0
void __stdcall FUN_004ac8c0(Object_004ac8c0* obj)
{
    Gadget_004ac8c0* gadgets = obj->holder->gadgets;
    int index = FUN_0049fdf0(gadgets, "COLS", 6);
    Gadget_004ac8c0* grid = &gadgets[index];
    void* surface = gadgets->surface;
    int x0 = gadgets->x + grid->x;
    int y = gadgets->y + grid->y;
    for (int row = 0; row < 16; row++) {
        int x = x0;
        for (int col = 0; col < 16; col++) {
            Rect_004ac8c0 r;
            r.left = x;
            r.top = y;
            r.right = x + 7;
            r.bottom = y + 7;
            FUN_004bf6f0(surface, &r, row * 16 + col);
            x += 8;
        }
        y += 8;
    }
}
