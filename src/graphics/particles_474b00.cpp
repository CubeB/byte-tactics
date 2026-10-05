// Decompiled by Opus. Names are provisional.
// Steps one 32-byte record of the kind Class_00474cd0 keeps in its vector
// (0x471cc0.cpp lists that family; 0x475340 inlines this same step): advances
// three cursors by the game's per-tick counts and, when the countdown runs
// out, counts one more round and restarts the countdown at half the period
// plus a random part of the other half.
#include <stdlib.h>

#pragma pack(push, 1)
struct Game_00474b00 {
    char unknown_0[0x14263];
    int count2;                        // +0x14263
    char unknown_14267[0x37ecc - 0x14267];
    int count1;                        // +0x37ecc
    char unknown_37ed0[0x37ed4 - 0x37ed0];
    int count3;                        // +0x37ed4
};
#pragma pack(pop)

extern Game_00474b00* g_game;

struct Pair_00474b00 {
    int a;
    int b;
};

struct Class_00474b00 {
    int field_0;                       // +0x00
    Pair_00474b00* cursor1;            // +0x04
    int* cursor2;                      // +0x08
    Pair_00474b00* cursor3;            // +0x0c
    int limit;                         // +0x10
    int rounds;                        // +0x14
    int period;                        // +0x18
    int countdown;                     // +0x1c

    void FUN_00474b00();
};

// FUNCTION: 0x474b00
void Class_00474b00::FUN_00474b00()
{
    cursor1 += g_game->count1;
    cursor2 += g_game->count2;
    cursor3 += g_game->count3;
    if (--countdown == 0) {
        rounds++;
        int half = period / 2;
        countdown = (int)((__int64)rand() * half / 0x8000) + half;
    }
}
