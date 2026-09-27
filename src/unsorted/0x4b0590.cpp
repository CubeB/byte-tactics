// Decompiled by Opus. Names are provisional.
// Sunken counterpart of FUN_004b0510: fills a rectangle and draws its top and
// left edges in the fourth argument's colour, the right and bottom edges in
// the third's.

struct Rect_004b0510 {
    int x1;                          // +0x0
    int y1;                          // +0x4
    int x2;                          // +0x8
    int y2;                          // +0xc
};

void __stdcall FUN_004bf6f0(void* surface, Rect_004b0510* rect, int color);
void __stdcall FUN_004be950(void* surface, int x1, int y1, int x2, int y2, unsigned char color);

// FUNCTION: 0x4b0590
void __stdcall FUN_004b0590(void* surface, Rect_004b0510* rect, int light, int dark, int fill)
{
    FUN_004bf6f0(surface, rect, fill);
    FUN_004be950(surface, rect->x1, rect->y1, rect->x2, rect->y1, dark);
    FUN_004be950(surface, rect->x1, rect->y1, rect->x1, rect->y2, dark);
    FUN_004be950(surface, rect->x2, rect->y1 + 1, rect->x2, rect->y2, light);
    FUN_004be950(surface, rect->x1 + 1, rect->y2, rect->x2, rect->y2, light);
}
