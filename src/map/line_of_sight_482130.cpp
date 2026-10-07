// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, edited by deepseek-v4.1, finished by Sonnet 5.5. Names are provisional.
// The original is std::remove_if (find_if, then remove_copy_if after the first
// expired element) plus vector::erase's count update. Eye::operator= re-points
// the two self pointers (screen at +4, flagPtr at +0xc).
#include <stddef.h>
#include <algorithm>

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

// One of the "eyeball" records of the array at g_game + 0x1427b. The two
// pointer fields point into the record itself: +4 at its screen position, +0xc
// at the byte flag, so the copy has to re-point them at the destination.
struct Eye_00482130 {
    void* player;                          // +0x00
    Pos_00482130* screen;                  // +0x04, &screenPos
    short x;                               // +0x08
    unsigned char flagA;                   // +0x0a
    char flagB;                            // +0x0b
    char* flagPtr;                         // +0x0c, &flagB
    Vec3_00482130 v;                       // +0x10
    unsigned int expires;                  // +0x1c
    Pos_00482130 screenPos;                // +0x20
    Eye_00482130& operator=(const Eye_00482130& s) {
        player = s.player;
        screen = &screenPos;
        x = s.x;
        flagPtr = &flagB;
        v = s.v;
        flagA = s.flagA;
        expires = s.expires;
        screenPos = s.screenPos;
        flagB = s.flagB;
        return *this;
    }
};

struct Game {
    char unknown_0[0x14277];
    int count;                             // +0x14277
    Eye_00482130* eyes;                    // +0x1427b
    char unknown_1427f[0x38a47 - 0x1427f];
    unsigned int ticks;                    // +0x38a47
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall RemoveLineOfSight(Eye_00482130* eye);


struct Expired {
    // Returns int, not bool: a bool return breaks the fused compare.
    int operator()(const Eye_00482130& e) const { return e.expires < g_game->ticks; }
};

// FUNCTION: 0x482130
void ExpireEyeballs()
{
    // Left uninitialised, as in the original.
    int changed;
    Eye_00482130* p = g_game->eyes;
    for (int i = 0; i < g_game->count; i++, p++) {
        if (p->expires < g_game->ticks) {
            RemoveLineOfSight(p);
            changed = 1;
        }
    }
    if (!changed)
        return;
    Eye_00482130* end = g_game->eyes + g_game->count;
    Eye_00482130* n = std::remove_if(g_game->eyes, end, Expired());
    g_game->count = n - g_game->eyes;
}