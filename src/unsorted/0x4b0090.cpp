// Decompiled by deepseek-v4.1-flash. Names are provisional.

struct Rect_004b0510 {
    int x1;                          // +0x0
    int y1;                          // +0x4
    int x2;                          // +0x8
    int y2;                          // +0xc
};

void __stdcall FUN_004be950(void* surface, int x1, int y1, int x2, int y2,
                            unsigned char color);

// Draws a double (two-line) bevelled border: the top and left pairs of edges
// use the third argument's colour, the right and bottom pairs the fourth's.
// FUNCTION: 0x4b0090
void __stdcall FUN_004b0090(void* surface, Rect_004b0510* rect, int light,
                            unsigned char dark, int unused)
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
