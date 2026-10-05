// Decompiled by Opus. Names are provisional.

struct Surface_0041df20;

struct Rect_0041df20 {
    int left;                          // +0x0
    int top;                           // +0x4
    int right;                         // +0x8
    int bottom;                        // +0xc
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x37e1f];
    int width;                         // +0x37e1f
    int height;                        // +0x37e23
    char unknown_37e27[0x3905f - 0x37e27];
    unsigned int nextTime;             // +0x3905f
    int done;                          // +0x39063
    int steps;                         // +0x39067
};
#pragma pack(pop)

extern Game* g_game;

unsigned int __cdecl GetTicks();
void __stdcall FadeRectangle(Surface_0041df20* dst, Rect_0041df20* rect, int level);

// FUNCTION: 0x41df20
void StepScreenFade(void)
{
    Rect_0041df20 rect;
    rect.left = rect.top = 0;
    rect.right = g_game->width;
    rect.bottom = g_game->height;
    if (g_game->nextTime < GetTicks()) {
        FadeRectangle(0, &rect, g_game->steps - 0x1d);
        g_game->nextTime = GetTicks() + 1;
        g_game->steps--;
        if (g_game->steps == 0)
            g_game->done = 1;
    }
}
