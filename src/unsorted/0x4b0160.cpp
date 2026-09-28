// Decompiled by deepseek-v4.1-flash. Names are provisional.

struct Rect_004b0160 {
    int x1;                          // +0x0
    int y1;                          // +0x4
    int x2;                          // +0x8
    int y2;                          // +0xc
};

void __stdcall FUN_004be950(void* surface, int x0, int y0, int x1, int y1, int color);

// Draws a two-pixel bevelled frame around a rectangle: the top and left edges
// use `light`, the right and bottom edges use `dark`. The fifth argument is
// not read by the original.
// FUNCTION: 0x4b0160
void __stdcall FUN_004b0160(void* surface, Rect_004b0160* rect, int dark, int light, int unused)
{
    FUN_004be950(surface, rect->x1, rect->y1, rect->x2, rect->y1, light);
    FUN_004be950(surface, rect->x1, rect->y1 + 1, rect->x2 - 1, rect->y1 + 1, light);
    FUN_004be950(surface, rect->x1, rect->y1, rect->x1, rect->y2, light);
    FUN_004be950(surface, rect->x1 + 1, rect->y1, rect->x1 + 1, rect->y2 - 1, light);
    FUN_004be950(surface, rect->x2, rect->y1 + 1, rect->x2, rect->y2, dark);
    FUN_004be950(surface, rect->x2 - 1, rect->y1 + 2, rect->x2 - 1, rect->y2, dark);
    FUN_004be950(surface, rect->x1 + 1, rect->y2, rect->x2, rect->y2, dark);
    FUN_004be950(surface, rect->x1 + 2, rect->y2 - 1, rect->x2, rect->y2 - 1, dark);
}
