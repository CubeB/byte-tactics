// Decompiled by space-bunny-free. Names are provisional.
// Per player slot: frees the two heap buffers, zeroes four dwords, calls
// FreeSquads on the entry, then retires the unit at +0x74 (DestroyPlayerAI on
// the team byte, then the unit's own method and operator delete).
//
// The loop bound is `p <= g_game->players + 10`, one past the last slot, so
// the body also runs once on the fields that follow the array (the `ja` guard
// only skips the loop when the array is empty). The original really does
// that.

#pragma pack(push, 1)
class SquadManager {
public:
    char unknown_0[0x11];
    void* ptrs[10];

    void DeleteTimers();
};

struct Player_00464a00 {
    char unknown_0[0x74];
    SquadManager* unit;                 // +0x74
    char unknown_78[0x7c - 0x78];
    void* buffer;                       // +0x7c
    int field_80;                       // +0x80
    int field_84;                       // +0x84
    int field_88;                       // +0x88
    char unknown_8c[0xec - 0x8c];
    void* buffer2;                      // +0xec
    char unknown_f0[0x146 - 0xf0];
    unsigned char team;                 // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Game {
    char unknown_0[0x1b63];
    Player_00464a00 players[10];        // +0x1b63
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall FreeSquads(Player_00464a00* param_1);
// Takes an int: the byte-to-int promotion is part of the original code.
void __stdcall DestroyPlayerAI(int param_1);

// FUNCTION: 0x464a00
void FreePlayers()
{
    for (Player_00464a00* p = g_game->players; p <= g_game->players + 10; p++) {
        // Fields reached as indices off q: keeps two induction variables.
        int* q = (int*)((char*)p + 0x84);
        q[-1] = 0;
        q[0] = 0;
        delete[] ((void**)q)[-2];
        q[1] = 0;
        ((void**)q)[-2] = 0;
        FreeSquads(p);
        if (((SquadManager**)q)[-4]) {
            DestroyPlayerAI(((unsigned char*)q)[0xc2]);
            if (SquadManager* u = ((SquadManager**)q)[-4]) {
                u->DeleteTimers();
                delete u;
            }
            ((SquadManager**)q)[-4] = 0;
        }
        if (((void**)q)[0x1a]) {
            delete[] ((void**)q)[0x1a];
            ((void**)q)[0x1a] = 0;
        }
    }
}
