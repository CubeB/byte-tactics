// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Game_0042bcc0 {
    char unknown_0[0x1438f];
    int field_1438f;                   // +0x1438f
    char unknown_14393[8];
    int* field_1439b;                  // +0x1439b
};
#pragma pack(pop)

extern Game_0042bcc0* g_game;

void __cdecl FUN_004d8780(void* param_1);
void FUN_004d85a0(int* param_1);

// FUNCTION: 0x42bcc0
void FUN_0042bcc0()
{
    if (g_game->field_1439b != 0) {
        FUN_004d8780(g_game->field_1439b);
        FUN_004d85a0(g_game->field_1439b);
        g_game->field_1439b = 0;
        g_game->field_1438f = 0;
    }
}
