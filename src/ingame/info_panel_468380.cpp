// Decompiled by space-bunny-free. Names are provisional.
// Network statistics: two rows, each a "Send"/"Receive" rate label with a
// 64x8 bar under it. The bar is full at 56 K/s (the rate times 100 over 5600,
// capped at 100 inside the bar helper, which is FUN_00468310 inlined twice).
// Both rows sit at x 0x81..0xc1; the first row's y comes from the game,
// the second one starts just under the first bar.
#include <stdio.h>

struct Rect_004b0510 {
    int x1;                          // +0x0
    int y1;                          // +0x4
    int x2;                          // +0x8
    int y2;                          // +0xc
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0xdda];
    unsigned char color;             // +0xdda
    char unknown_ddb[0x37e23 - 0xddb];
    int f_37e23;                     // +0x37e23
};
#pragma pack(pop)

extern Game* g_game;

int FUN_004c1450();
void __stdcall FUN_00416150(unsigned int* sent, unsigned int* received);
void __stdcall FUN_004c14f0(void* surface, const char* text, int x, int y, int maxWidth);
void __stdcall FUN_004bf8c0(void* surface, Rect_004b0510* rect, int color);
void __stdcall FUN_004bf6f0(void* surface, Rect_004b0510* rect, int color);

// FUN_00468310, the out-of-line bar, written here so both calls inline.
static void Bar(void* surface, Rect_004b0510* rect, int percent)
{
    unsigned char& color = g_game->color;
    FUN_004bf8c0(surface, rect, color);
    if (percent >= 100)
        percent = 100;
    if (percent > 0) {
        rect->x2 = (rect->x2 - rect->x1) * percent / 100 + rect->x1;
        FUN_004bf6f0(surface, rect, color);
    }
}

// FUNCTION: 0x468380
void __stdcall FUN_00468380(void* surface)
{
    unsigned int sent;
    unsigned int received;
    char buf[0x20];
    Rect_004b0510 r;
    r.x1 = 0x81;
    r.x2 = 0xc1;
    r.y1 = g_game->f_37e23 - 0x5f;
    r.y2 = r.y1 + 8;
    int h = FUN_004c1450();
    FUN_00416150(&sent, &received);
    sprintf(buf, "Send - %1.1f K/s", sent * 0.001);
    FUN_004c14f0(surface, buf, r.x1, r.y1, -1);
    r.y1 += h;
    r.y2 = r.y1 + 8;
    Bar(surface, &r, sent * 100 / 5600);
    r.x1 = 0x81;
    r.x2 = 0xc1;
    r.y1 = r.y2 + 1;
    sprintf(buf, "Receive - %1.1f K/s", received * 0.001);
    FUN_004c14f0(surface, buf, r.x1, r.y1, -1);
    r.y1 += h;
    r.y2 = r.y1 + 8;
    Bar(surface, &r, received * 100 / 5600);
}
