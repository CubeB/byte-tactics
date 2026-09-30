// Decompiled by Haiku. Names are provisional.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x148d7];
    int field_148d7;
};
#pragma pack(pop)

extern void __cdecl FUN_004d85a0(int);
extern Game* g_game;

// FUNCTION: 0x431920
void FUN_00431920(void)
{
    int val = g_game->field_148d7;
    FUN_004d85a0(val);
    g_game->field_148d7 = 0;
}
