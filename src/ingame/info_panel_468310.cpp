// Decompiled by Opus. Names are provisional.
// Draws a percentage bar: DrawRectangle on the whole rectangle, then, when the
// percentage (capped at 100) is positive, fills that fraction of its width.
// Both calls take the colour byte at g_game+0xdda through one reference.

struct Rect_004b0510 {
    int x1;                          // +0x0
    int y1;                          // +0x4
    int x2;                          // +0x8
    int y2;                          // +0xc
};

struct Game {
    char unknown_0[0xdda];
    unsigned char color;             // +0xdda
};

extern Game* g_game;

void __stdcall DrawRectangle(void* param_1, void* param_2, int param_3);
void __stdcall FillRectangle(void* surface, Rect_004b0510* rect, int color);

// FUNCTION: 0x468310
void __stdcall DrawPercentBar(void* surface, Rect_004b0510* rect, int percent)
{
    unsigned char& color = g_game->color;
    DrawRectangle(surface, rect, color);
    if (percent >= 100)
        percent = 100;
    if (percent > 0) {
        rect->x2 = (rect->x2 - rect->x1) * percent / 100 + rect->x1;
        FillRectangle(surface, rect, color);
    }
}
