// Decompiled by DeepSeek V4.1 Flash, finished by Claude Opus 5.5. Names are provisional.
// CollectSelectedUnits clears the vector and fills it with the local player's
// units that have bit 4 of +0x110 set (probably the selected units); each one
// whose low two flag bits are not 2, with +0x86 and +0x8a clear and whose type
// is not in the "Commander" set, is handed to the given player with
// GiveUnitToPlayer.
// The list is a real std::vector<Unit*> (0x48ca20 calls the vector's _Ucopy,
// _Ufill and _Destroy).
#include <vector>

#pragma pack(push, 1)
struct Unit {
    char unknown_0[0x86];
    int field_86;                      // +0x86
    int field_8a;                      // +0x8a
    char unknown_8e[0xa6 - 0x8e];
    unsigned short type;               // +0xa6
    char unknown_a8[0x110 - 0xa8];
    unsigned int flags;                // +0x110
};
#pragma pack(pop)

struct Player_004933e0 {
    char unknown_0[0x14b];
};

struct Game {
    char unknown_0[0x1b63];
    Player_004933e0 players[10];       // +0x1b63
};

extern Game* g_game;
void __stdcall CollectSelectedUnits(std::vector<Unit*>* list);
unsigned int* __stdcall GetCategoryMask(char* name);
void __stdcall GiveUnitToPlayer(Unit* unit, void* player, int arg);

// The same inlined bit-set test as 0x41c310.
static inline int TestBit(unsigned int* set, unsigned short n)
{
    return set[n >> 5] & (1 << (n & 0x1f));
}

// FUNCTION: 0x4933e0
void __stdcall GiveSelectedUnitsToPlayer(unsigned char player)
{
    // Must be a real std::vector: its constructor leaves the player index in place.
    std::vector<Unit*> list;
    CollectSelectedUnits(&list);
    Player_004933e0* p = &g_game->players[player];
    unsigned int* set = GetCategoryMask("Commander");
    for (std::vector<Unit*>::iterator it = list.begin(); it != list.end(); it++) {
        Unit* unit = *it;
        if ((unit->flags & 3) != 2 && unit->field_8a == 0 && unit->field_86 == 0
            && !TestBit(set, unit->type)) {
            GiveUnitToPlayer(unit, p, 0);
        }
    }
}
