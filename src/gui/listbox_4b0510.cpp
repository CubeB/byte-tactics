// Decompiled by Opus. Names are provisional.

struct Rect_004b0510 {
    int x1;                          // +0x0
    int y1;                          // +0x4
    int x2;                          // +0x8
    int y2;                          // +0xc
};

void __stdcall FillRectangle(void* surface, Rect_004b0510* rect, int color);
void __stdcall DrawLine(void* surface, int x1, int y1, int x2, int y2, unsigned char color);

// Fills a rectangle and draws a bevelled border: top and left edges in one
// colour, right and bottom edges in another.
// FUNCTION: 0x4b0510
void __stdcall FUN_004b0510(void* surface, Rect_004b0510* rect, int light, unsigned char dark, int fill)
{
    FillRectangle(surface, rect, fill);
    DrawLine(surface, rect->x1, rect->y1, rect->x2, rect->y1, light);
    DrawLine(surface, rect->x1, rect->y1, rect->x1, rect->y2, light);
    DrawLine(surface, rect->x2, rect->y1 + 1, rect->x2, rect->y2, dark);
    DrawLine(surface, rect->x1 + 1, rect->y2, rect->x2, rect->y2, dark);
}
