// Decompiled by deepseek-v4.1-flash. Names are provisional.

#pragma pack(push, 1)
struct Unit {
    char unknown_0[0xa6];
    short field_a6;                    // +0xa6
    char unknown_a8[0xac - 0xa8];
    int field_ac;                      // +0xac
    char unknown_b0[0x110 - 0xb0];
    unsigned int unknown_110_0 : 4;    // +0x110
    unsigned int field_110_4 : 1;      // +0x110 bit 4
    unsigned int unknown_110_5 : 27;
    char unknown_114[0x118 - 0x114];
};

struct UnitRange_0048d920 {
    Unit* begin;                       // +0x67 in Player
    Unit* end;                         // +0x6b in Player (last unit, inclusive)
};

struct Player_0048d920 {               // 0x14b bytes
    char unknown_0[0x67];
    UnitRange_0048d920 units;          // +0x67
    char unknown_6f[0x14b - 0x6f];
};

struct Game {
    char unknown_0[0x1b63];
    Player_0048d920 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char player;              // +0x2a42
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall SetUnitSquad(Unit* unit, int arg);

// FUNCTION: 0x48d920
void __stdcall FUN_0048d920(int param_1)
{
    Player_0048d920* player = &g_game->players[g_game->player];
    for (Unit* u = player->units.begin; u <= player->units.end; u++) {
        if (u->field_a6 != 0) {
            if (u->field_110_4) {
                SetUnitSquad(u, param_1);
            } else if (u->field_ac == param_1) {
                SetUnitSquad(u, 0);
            }
        }
    }
}
