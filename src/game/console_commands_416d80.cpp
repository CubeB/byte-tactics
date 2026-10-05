// Decompiled by Opus. Names are provisional.
// Toggles bit 0 of the game flags at +0x14281 (0x416d50 toggles bit 1).

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x14281];
    unsigned short flag0 : 1;          // +0x14281, bit 0 (mask 1)
    unsigned short flag1 : 1;
    unsigned short rest : 14;
};
#pragma pack(pop)

extern Game* g_game;

extern void SaveSettings();
extern void __stdcall FUN_004816a0(int flag);

// FUNCTION: 0x416d80
void __stdcall CmdMapping(int unused)
{
    g_game->flag0 = !g_game->flag0;
    SaveSettings();
    FUN_004816a0(1);
}
