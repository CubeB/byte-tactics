// Decompiled by Space Bunny Free. Names are provisional.
// Finds the entry (of the 11 inside this) whose field_14 is param_1 and returns
// it. When there is none, param_2 decides whether a new entry may be made:
// first an unused slot (field_14 == -1), otherwise the first slot whose
// field_14 is not the color of any of the ten player slots in g_game. The
// chosen entry is re-initialised with FUN_00461db0 and returned.
//
// The one instruction that decides the whole register allocation is in the
// player-colour scan: walking a pointer (`Player* p = &g_game->players[0]; for
// (j = 0; j < 10; j++, p++)`) instead of indexing `g_game->players[j]` adds
// just enough register pressure that MSVC 5 stops assuming ecx survives the two
// FUN_00461db0 calls, and gives the object pointer the callee-saved ebx
// (`push ebx; mov ebx, ecx`, plus a spill to [esp+0x10] for the block that
// later borrows ebx as a cursor). With plain array indexing every instruction
// is identical but for that missing `mov ebx, ecx` and the rotation it causes,
// which is 74.9%. The outer loops stay plain array indexing, which is what
// gives the `add esi, 0x1044` after the loop guard.

#pragma pack(push, 1)
// One entry, 0x1044 bytes, as laid out by the neighbours 0x461820 and 0x462470.
class Class_00462470 {
public:
    int field_0;                       // +0x00
    char unknown_4[0x14 - 0x4];
    int field_14;                      // +0x14
    char unknown_18[0x1c - 0x18];
    int field_1c;                      // +0x1c
    char unknown_20[4];
    int field_24;                      // +0x24
    char unknown_28[0x1044 - 0x28];

    void FUN_00461db0(int a1, int a2, int a3, int a4);
};

class Class_00461630 {
public:
    char unknown_0[4];
    int field_4;                       // +0x04
    Class_00462470 entries[11];        // +0x08

    Class_00462470* FUN_00461630(int param_1, int param_2);
};

// A player slot: the 0x14b byte record the game keeps in g_game->players.
struct Player_00461630 {
    int field_0;                       // +0x00
    int color;                         // +0x04
    char unknown_8[0x14b - 0x8];
};

struct Game_00461630 {
    char unknown_0[0x1b63];
    Player_00461630 players[10];       // +0x1b63
};
#pragma pack(pop)

extern Game_00461630* g_game;

// FUNCTION: 0x461630
Class_00462470* Class_00461630::FUN_00461630(int param_1, int param_2)
{
    unsigned i;
    for (i = 0; i <= 10; i++) {
        if (entries[i].field_14 == param_1)
            return &entries[i];
    }
    if (param_2 == 0)
        return 0;
    for (i = 0; i <= 10; i++) {
        if (entries[i].field_14 == -1) {
            entries[i].FUN_00461db0(param_1, field_4, 2, 0x64);
            return &entries[i];
        }
    }
    for (i = 1; i <= 10; i++) {
        int used = 0;
        Player_00461630* p = &g_game->players[0];
        for (int j = 0; j < 10; j++, p++) {
            if (p->color == entries[i].field_14) {
                used = 1;
                break;
            }
        }
        if (used == 0) {
            entries[i].FUN_00461db0(param_1, field_4, 2, 0x64);
            return &entries[i];
        }
    }
    return 0;
}
