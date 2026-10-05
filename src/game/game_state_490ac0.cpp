// Decompiled by Sonnet. Names are provisional.

int __stdcall AllocSurface(const char* name, int a, int b);
void __stdcall SetRestoreSurface(int param_1);

#pragma pack(push, 1)
struct Game
{
    char unknown_0[0x37e1b];
    int field_37e1b; // +0x37e1b
    int field_37e1f; // +0x37e1f
    int field_37e23; // +0x37e23
};
#pragma pack(pop)

extern Game* g_game;
extern const char DAT_005091d4[]; // "OFFSCREEN"

// FUNCTION: 0x490ac0
void FUN_00490ac0()
{
    g_game->field_37e1b = AllocSurface(DAT_005091d4, g_game->field_37e1f, g_game->field_37e23);
    SetRestoreSurface(g_game->field_37e1b);
}
