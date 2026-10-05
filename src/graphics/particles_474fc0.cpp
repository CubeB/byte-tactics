// Decompiled by Opus. Names are provisional.
// Per-tick update of one 32-byte record in the std::vector at +0xc of
// Class_004750b0 (the family listed in 0x471cc0.cpp; that file calls the
// record Record_004750b0): drifts the 16.16 position by the wind (x, z) and a
// vertical rate (y), and when the timer runs out counts one more step and
// restarts the timer at a random value between period/2 and period. Slot 1
// of Class_004750b0 (0x475600) inlines this; this out-of-line copy is never
// called. 0x474b00 is the same update for Class_00474cd0's records (y * 4).
#include <stdlib.h>

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x14263];
    int rise;                          // +0x14263
    char unknown_14267[0x37ecc - 0x14267];
    int windX;                         // +0x37ecc
    char unknown_37ed0[4];
    int windZ;                         // +0x37ed4
};
#pragma pack(pop)

extern Game* g_game;

class Class_00474fc0 {
public:
    int field_0;                       // +0x0
    int x;                             // +0x4
    int y;                             // +0x8
    int z;                             // +0xc
    int limit;                         // +0x10
    int count;                         // +0x14
    int period;                        // +0x18
    int timer;                         // +0x1c

    void FUN_00474fc0();
};

// FUNCTION: 0x474fc0
void Class_00474fc0::FUN_00474fc0()
{
    x += g_game->windX * 8;
    y += g_game->rise * 16;
    z += g_game->windZ * 8;
    if (--timer == 0) {
        count++;
        int half = period / 2;
        timer = (int)((__int64)rand() * half / 0x8000) + half;
    }
}
