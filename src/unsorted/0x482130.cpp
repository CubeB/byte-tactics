// Decompiled by space-bunny-free. Names are provisional.
#include <stddef.h>

#pragma pack(push, 1)
struct Pos_00482130 {
    short x;
    short y;
};

struct Vec3_00482130 {
    int x;
    int y;
    int z;
};

// One of the 20 "eyeball" records of the array at g_game + 0x1427b. The two
// pointer fields point into the record itself: +4 at its screen position, +0xc
// at the byte flag.
struct Eye_00482130 {
    void* player;                          // +0x00
    Pos_00482130* screen;                  // +0x04, &screenPos
    short x;                               // +0x08
    char flagA;                            // +0x0a
    char flagB;                            // +0x0b
    char* flagPtr;                         // +0x0c, &flagB
    Vec3_00482130 v;                       // +0x10
    int expires;                           // +0x1c
    Pos_00482130 screenPos;                // +0x20
};

struct Game_00482130 {
    char unknown_0[0x14277];
    int count;                             // +0x14277
    Eye_00482130* eyes;                    // +0x1427b
    char unknown_1427f[0x38a47 - 0x1427f];
    int ticks;                             // +0x38a47
};
#pragma pack(pop)

extern Game_00482130* g_game;

void __stdcall FUN_00481d50(Eye_00482130* eye);

// FUNCTION: 0x482130
void FUN_00482130()
{
    int changed;                           // never initialised, as in the original
    Eye_00482130* p = g_game->eyes;

    for (int i = 0; i < g_game->count; i++, p++) {
        if (p->expires < g_game->ticks) {
            FUN_00481d50(p);
            changed = 1;
        }
    }
    if (!changed)
        return;

    Eye_00482130* end = g_game->eyes + g_game->count;
    p = g_game->eyes;
    while (p != end && p->expires >= g_game->ticks)
        p++;
    if (p != end) {
        Eye_00482130* src = p + 1;
        if (src != end) {
            for (; src != end; src++) {
                if (src->expires >= g_game->ticks) {
                    p->player = src->player;
                    p->screen = &p->screenPos;
                    p->x = src->x;
                    p->flagPtr = &p->flagB;
                    p->v = src->v;
                    p->flagA = src->flagA;
                    p->expires = src->expires;
                    p->screenPos = src->screenPos;
                    p->flagB = src->flagB;
                    p++;
                }
            }
        }
    }
    g_game->count = (int)((char*)p - (char*)g_game->eyes) / 0x24;
}
