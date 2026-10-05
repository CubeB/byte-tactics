// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x1438f];
    int field_1438f;                   // +0x1438f
    char unknown_14393[8];
    int* field_1439b;                  // +0x1439b
};
#pragma pack(pop)

extern Game* g_game;

void __cdecl ProtectBlockReadWrite(void* param_1);
void __cdecl FUN_004d85a0(int* param_1);

// FUNCTION: 0x42bcc0
void FreeUnitInfo()
{
    if (g_game->field_1439b != 0) {
        ProtectBlockReadWrite(g_game->field_1439b);
        FUN_004d85a0(g_game->field_1439b);
        g_game->field_1439b = 0;
        g_game->field_1438f = 0;
    }
}
