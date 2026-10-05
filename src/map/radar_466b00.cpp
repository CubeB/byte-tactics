// Decompiled by Opus. Names are provisional.
#pragma pack(push, 1)
struct Game {
    char unknown_0[0xdd9];
    unsigned char unknown_dd9;         // +0xdd9
    char unknown_dda[0x142cb - 0xdda];
    char unknown_142cb[0x142db - 0x142cb]; // +0x142cb
    void* unknown_142db;               // +0x142db
    char unknown_142df[0x142e7 - 0x142df];
    short unknown_142e7;               // +0x142e7
    short unknown_142e9;               // +0x142e9
    char unknown_142eb[0x142f1 - 0x142eb];
    unsigned short flag0 : 1;          // +0x142f1
    unsigned short pending : 1;
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall FUN_004c6b70(void* param_1, void* param_2, int param_3, int param_4);
void __stdcall FUN_004bf8c0(void* param_1, void* param_2, int param_3);

// FUNCTION: 0x466b00
void __stdcall FUN_00466b00(void* param_1)
{
    if (g_game->pending) {
        g_game->pending = 0;
        FUN_004c6b70(param_1, g_game->unknown_142db, g_game->unknown_142e7, g_game->unknown_142e9);
        FUN_004bf8c0(param_1, g_game->unknown_142cb, g_game->unknown_dd9);
    }
}
