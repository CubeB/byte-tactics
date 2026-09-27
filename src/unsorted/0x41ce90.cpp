// Decompiled by Claude Opus 5.5. Names are provisional.
// Edge and arrow-key scrolling: moves the view by the scroll speed (capped at
// 0x80) when an arrow key is held (unless the TALK.GUI chat box is open) or
// the mouse sits on the edge of the screen, then clamps and saves it.
//
// Nothing from <string> is used. It is here for compiler state only (see
// "Why headers matter at all" in docs/agent-guide.md): without it MSVC 5
// computes the scroll speed as `xor ecx, ecx; mov cl, [speed]; mov ebx,
// [scale]; imul ebx, ecx` instead of loading the byte straight into ebx and
// multiplying by memory, and no rewrite of the multiply changed that. A
// scratch copy with 2000 or more unused prototypes (0 to 1750 do not) matches
// too, as do <vector> + <map> and <iostream>.
#include <windows.h>
#include <string>

struct Mouse_0041ce90 {
    int x;                             // +0x0
    int y;                             // +0x4
    int unknown_8[4];
};

#pragma pack(push, 1)
struct Display_0041ce90 {
    char unknown_0[0x40];
    HWND hwnd;                         // +0x40
    char unknown_44[0xf0 - 0x44];
    unsigned char flags_f0;            // +0xf0
};

struct Game_0041ce90 {
    char unknown_0[0x519];
    char menu[0x14281 - 0x519];        // +0x519
    unsigned short flags_14281;        // +0x14281
    char unknown_14283[0x142f1 - 0x14283];
    unsigned char flags_142f1;         // +0x142f1
    char unknown_142f2;
    int value_142f3;                   // +0x142f3
    int value_142f7;                   // +0x142f7
    char unknown_142fb[0x1431f - 0x142fb];
    int x;                             // +0x1431f
    int y;                             // +0x14323
    int x2;                            // +0x14327
    int y2;                            // +0x1432b
    char unknown_1432f[0x1434b - 0x1432f];
    short value_1434b;                 // +0x1434b
    unsigned char scrollSpeed;         // +0x1434d
    char unknown_1434e[0x37e1f - 0x1434e];
    int width;                         // +0x37e1f
    int height;                        // +0x37e23
    char unknown_37e27[0x38a3f - 0x37e27];
    int scrollScale;                   // +0x38a3f
};
#pragma pack(pop)

extern Game_0041ce90* g_game;

void __stdcall FUN_004c2340(Mouse_0041ce90* mouse);
Display_0041ce90* FUN_004b6220();
int __stdcall FUN_004ab060(void* obj, const char* name);
int __stdcall FUN_004c1b80(int key);
void FUN_0041c3c0();

// FUNCTION: 0x41ce90
void FUN_0041ce90()
{
    Mouse_0041ce90 mouse;
    POINT pt;
    int speed = g_game->scrollSpeed * g_game->scrollScale;
    if (speed > 0x80)
        speed = 0x80;
    if (speed == 0)
        return;
    FUN_004c2340(&mouse);
    Display_0041ce90* d = FUN_004b6220();
    if (!(d->flags_f0 & 2)) {
        GetCursorPos(&pt);
        int w = g_game->width;
        int h = g_game->height;
        if ((pt.x >= w || pt.y >= h) && pt.x < w + 100 && pt.y < h + 100
            && GetFocus() == d->hwnd) {
            mouse.x = pt.x;
            if (pt.x >= w)
                mouse.x = w - 1;
            mouse.y = pt.y;
            if (pt.y >= h)
                mouse.y = h - 1;
        }
    } else {
        int w = g_game->width;
        int h = g_game->height;
        if (mouse.x >= w)
            mouse.x = w - 1;
        if (mouse.y >= h)
            mouse.y = h - 1;
    }
    int talk = FUN_004ab060(g_game->menu, "TALK.GUI");
    int x = g_game->x;
    int y = g_game->y;
    if ((FUN_004c1b80(0xf4) && !talk) || (mouse.x == 0 && mouse.y < g_game->height))
        x -= speed;
    else if ((FUN_004c1b80(0xf6) && !talk) || mouse.x == g_game->width - 1)
        x += speed;
    if ((FUN_004c1b80(0xf5) && !talk) || (mouse.y == 0 && mouse.x < g_game->width))
        y -= speed;
    else if ((FUN_004c1b80(0xf7) && !talk) || mouse.y == g_game->height - 1)
        y += speed;
    if (g_game->x != x || g_game->y != y) {
        g_game->x = x;
        g_game->y = y;
        g_game->flags_142f1 |= 2;
        FUN_0041c3c0();
        g_game->x2 = g_game->x;
        g_game->y2 = g_game->y;
        g_game->flags_14281 &= 0xfff7;
        g_game->value_1434b = 0;
        g_game->value_142f3 = 0;
        g_game->value_142f7 = 0;
    }
}
