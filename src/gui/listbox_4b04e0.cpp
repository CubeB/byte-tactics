// Decompiled by Opus. Names are provisional.
// Fills a rectangle, then draws its border through FUN_004b0160 (same shape
// as 0x4b04b0, which uses FUN_004b0090).

struct Rect_004b0510 {
    int x1;                          // +0x0
    int y1;                          // +0x4
    int x2;                          // +0x8
    int y2;                          // +0xc
};

void __stdcall FUN_004bf6f0(void* surface, Rect_004b0510* rect, int color);
void __stdcall FUN_004b0160(void* surface, Rect_004b0510* rect, int light, int dark, int fill);

// FUNCTION: 0x4b04e0
void __stdcall FUN_004b04e0(void* surface, Rect_004b0510* rect, int light, int dark, int fill)
{
    FUN_004bf6f0(surface, rect, fill);
    FUN_004b0160(surface, rect, light, dark, fill);
}
