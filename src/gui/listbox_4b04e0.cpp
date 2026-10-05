// Decompiled by Opus. Names are provisional.
// Fills a rectangle, then draws its border through DrawBevelBorderDarkFirst (same shape
// as 0x4b04b0, which uses DrawBevelBorder).

struct Rect_004b0510 {
    int x1;                          // +0x0
    int y1;                          // +0x4
    int x2;                          // +0x8
    int y2;                          // +0xc
};

void __stdcall FillRectangle(void* surface, Rect_004b0510* rect, int color);
void __stdcall DrawBevelBorderDarkFirst(void* surface, Rect_004b0510* rect, int light, int dark, int fill);

// FUNCTION: 0x4b04e0
void __stdcall FillBevelBoxDarkFirst(void* surface, Rect_004b0510* rect, int light, int dark, int fill)
{
    FillRectangle(surface, rect, fill);
    DrawBevelBorderDarkFirst(surface, rect, light, dark, fill);
}
