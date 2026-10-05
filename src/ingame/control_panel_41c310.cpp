// Decompiled by Opus. Names are provisional.
// Finds the local player's commander: the last of the player's units whose
// type is in the "Commander" unit-type set.

struct Unit {
    char unknown_0[0xa6];
    unsigned short type;               // +0xa6
    char unknown_a8[0x118 - 0xa8];
};

#pragma pack(push, 1)
struct Player_0041c310 {
    char unknown_0[0x67];
    Unit* unitsBegin;                  // +0x67
    Unit* unitsEnd;                    // +0x6b (last unit, inclusive)
    char unknown_6f[0x14b - 0x6f];
};

struct Game {
    char unknown_0[0x1b63];
    Player_0041c310 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;         // +0x2a42
    char unknown_2a43[0x142f3 - 0x2a43];
    Unit* commander;                   // +0x142f3
};
#pragma pack(pop)

extern Game* g_game;

// Returns the bit set of the unit types in the named category.
unsigned int* __stdcall FUN_00488c50(char* name);

// The bit test was an inlined helper taking the type as unsigned short.
static inline int TestBit(unsigned int* set, unsigned short n)
{
    return set[n >> 5] & (1 << (n & 0x1f));
}

// FUNCTION: 0x41c310
void FUN_0041c310()
{
    Player_0041c310* p = &g_game->players[g_game->localPlayer];
    unsigned int* set = FUN_00488c50("Commander");
    for (Unit* u = p->unitsBegin; u <= p->unitsEnd; u++) {
        if (TestBit(set, u->type)) {
            g_game->commander = u;
        }
    }
}
