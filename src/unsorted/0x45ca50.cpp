// Decompiled by Opus. Names are provisional.
// Copies saved option values (globals around 0x512f2c) into the game.

#pragma pack(push, 1)
struct Game_0045ca50 {
    char unknown_0[0x1434d];
    char field_1434d;                  // +0x1434d
    char unknown_1434e[0x37efa - 0x1434e];
    int field_37efa;                   // +0x37efa
    char unknown_37efe[0x37f17 - 0x37efe];
    char field_37f17;                  // +0x37f17
    char field_37f18;                  // +0x37f18
    char unknown_37f19[0x37f23 - 0x37f19];
    int field_37f23;                   // +0x37f23
    int field_37f27;                   // +0x37f27
    char unknown_37f2b[0x38a4b - 0x37f2b];
    short field_38a4b;                 // +0x38a4b
    short field_38a4d;                 // +0x38a4d
};
#pragma pack(pop)

extern Game_0045ca50* g_game;
extern int DAT_00512f2c;
extern char DAT_00512f49;
extern char DAT_00512f4a;
extern int DAT_00512f55;
extern int DAT_00512f59;
extern int DAT_00512f6d;
extern char DAT_00512f71;

// FUNCTION: 0x45ca50
void FUN_0045ca50()
{
    g_game->field_37f23 = DAT_00512f55;
    g_game->field_38a4b = DAT_00512f6d;
    g_game->field_38a4d = DAT_00512f6d;
    g_game->field_1434d = DAT_00512f71;
    g_game->field_37efa = DAT_00512f2c;
    g_game->field_37f17 = DAT_00512f49;
    g_game->field_37f18 = DAT_00512f4a;
    g_game->field_37f27 = DAT_00512f59;
}
