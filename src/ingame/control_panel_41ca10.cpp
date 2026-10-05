// Decompiled by Claude Opus 5.5. Names are provisional.
// Camera follow: picks a target position (a pending jump at +0x1433f while
// its counter runs, else the followed object at +0x142f7, else the followed
// unit at +0x142f3 while it is still alive), centres the scroll target on it
// and clamps it to the map, then moves the scroll position halfway towards
// the target (at most 320 pixels per axis) and re-clamps it.
// Needs <windows.h> (found with tools/headers.py): without it the view
// centre x is computed after viewWidth / 2 and lands in edx instead of esi.
// The halfway step is an inline helper with `cur + -320`: written in place,
// MSVC emits `sub ecx, 0x140` and a `neg`/`add` for `cur - d / 2`, and the
// SetTarget/ClampTarget helpers give the original's load order.

#include <windows.h>

#pragma pack(push, 1)
struct Pos_0041ca10 {
    int x;                             // +0x0 (16.16 fixed point)
    int z;                             // +0x4
    int y;                             // +0x8
};

struct Follow_0041ca10 {
    int unknown_0;
    Pos_0041ca10 pos;                  // +0x4
};

struct Unit {
    char unknown_0[0x6a];
    Pos_0041ca10 pos;                  // +0x6a
    char unknown_76[0x110 - 0x76];
    unsigned int flags;                // +0x110
};

struct Game {
    char unknown_0[0x1422b];
    int mapWidth;                      // +0x1422b
    int mapHeight;                     // +0x1422f
    char unknown_14233[0x14281 - 0x14233];
    unsigned short flags_14281;        // +0x14281
    char unknown_14283[0x142f1 - 0x14283];
    unsigned short flags_142f1;        // +0x142f1
    Unit* followUnit;                  // +0x142f3
    Follow_0041ca10* follow;           // +0x142f7
    char unknown_142fb[0x1431f - 0x142fb];
    int x;                             // +0x1431f
    int y;                             // +0x14323
    int x2;                            // +0x14327
    int y2;                            // +0x1432b
    char unknown_1432f[0x1433f - 0x1432f];
    Pos_0041ca10 jump;                 // +0x1433f
    unsigned short jumpCount;          // +0x1434b
    char unknown_1434d[0x37e37 - 0x1434d];
    int viewWidth;                     // +0x37e37
    int viewHeight;                    // +0x37e3b
};
#pragma pack(pop)

extern Game* g_game;

void UpdateScreenShake();
void ClampCameraPosition();

static inline void SetTarget(int x, int y)
{
    g_game->x2 = x;
    g_game->y2 = y;
}

static inline void ClampTarget()
{
    int maxX = g_game->mapWidth - g_game->viewWidth;
    int maxY = g_game->mapHeight - g_game->viewHeight;
    if (g_game->x2 < 0) {
        g_game->x2 = 0;
    } else if (g_game->x2 > maxX) {
        g_game->x2 = maxX;
    }
    if (g_game->y2 < 0) {
        g_game->y2 = 0;
    } else if (g_game->y2 > maxY) {
        g_game->y2 = maxY;
    }
}

static inline int Approach(int cur, int target)
{
    int d = cur - target;
    if (d > 0) {
        if (d > 320)
            return cur + -320;
    } else {
        if (d < -320)
            return cur + 320;
    }
    return cur - d / 2;
}

// FUNCTION: 0x41ca10
void UpdateCameraFollow()
{
    Pos_0041ca10* p = 0;
    if (g_game->jumpCount != 0) {
        g_game->jumpCount--;
        p = &g_game->jump;
    } else if (g_game->follow != 0) {
        p = &g_game->follow->pos;
    } else if (g_game->followUnit != 0) {
        if (g_game->followUnit->flags & 0x10000000) {
            p = &g_game->followUnit->pos;
        } else {
            g_game->jumpCount = 0;
            g_game->followUnit = 0;
            g_game->follow = 0;
        }
    }
    if (p != 0) {
        SetTarget((short)(p->x >> 16) - g_game->viewWidth / 2,
                  (short)((p->y - (p->z >> 1)) >> 16) - g_game->viewHeight / 2);
        ClampTarget();
        g_game->flags_14281 &= 0xfff7;
    }
    if (g_game->x != g_game->x2) {
        g_game->flags_142f1 |= 2;
        g_game->flags_14281 &= 0xfff7;
        g_game->x = Approach(g_game->x, g_game->x2);
    }
    if (g_game->y != g_game->y2) {
        g_game->flags_142f1 |= 2;
        g_game->flags_14281 &= 0xfff7;
        g_game->y = Approach(g_game->y, g_game->y2);
    }
    UpdateScreenShake();
    ClampCameraPosition();
}
