// Decompiled by deepseek-v4.1-flash, finished by LongCat 2.5 Preview Free,
// claude-opus-5-5 (#4373), GPT-6.1-sol, mimo-v2.6-pro, space-bunny-free (#4489)
// and space-bunny-free (#4539), space-bunny-free (#4652), Space Bunny Free (#4688),
// finished by DeepSeek V4.1 Flash (issue 4875), finished by GPT-6,
// matched by Claude Opus 5.5.
// Names are provisional.
// Draws the 16 x 16 palette grid: one 8 x 8 cell per colour, at the "COLS"
// gadget's position.
//
// Claude Opus 5.5 (#5155), from 89.8%: two changes.
// 1. The loop is written the way the matched sibling 0x4ac970 writes one
//    cell: a separate `void* surface` and `Rect rect` (no surface-plus-rect
//    aggregate), `rect.left = x0 + col * 8`, `rect.top = y + row * 8`,
//    `rect.right = rect.left + 7`, `rect.bottom = rect.top + 7`. That loop is
//    byte-identical to the original (93.2%).
// 2. `grid` is declared before `gadgets`. The operand order MSVC 5 picks for
//    the two prologue sums (which load goes straight into esi or ebx) follows
//    the two pointers' symbol order, not the order of the operands in the
//    source: with `gadgets` declared first every spelling gave
//    gadgets->x + grid->x and grid->y + gadgets->y. With `grid` first both
//    sums come out as in the original.
// The match depends on the declaration count in front of the function: one
// extra `extern int` above this function gives 91.5% (two give MATCH again).

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

int __stdcall FindGadgetIndex(void* gadgets, const char* name, int flag);
void __stdcall FillRectangle(void* surface, Rect_004ac8c0* rect, int color);

// FUNCTION: 0x4ac8c0
void __stdcall FUN_004ac8c0(Object_004ac8c0* obj)
{
    Gadget_004ac8c0 *grid, *gadgets;
    gadgets = obj->holder->gadgets;
    int index = FindGadgetIndex(gadgets, "COLS", 6);
    grid = &gadgets[index];
    void* surface = gadgets->surface;
    int x0 = grid->x + gadgets->x;
    int y = gadgets->y + grid->y;
    Rect_004ac8c0 rect;
    for (int row = 0; row < 16; row++) {
        for (int col = 0; col < 16; col++) {
            rect.left = x0 + col * 8;
            rect.top = y + row * 8;
            rect.right = rect.left + 7;
            rect.bottom = rect.top + 7;
            FillRectangle(surface, &rect, row * 16 + col);
        }
    }
}
