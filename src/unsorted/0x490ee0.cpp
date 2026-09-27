// Decompiled by Sonnet. Names are provisional.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x38a4b];
    unsigned short field_38a4b;         // +0x38a4b
};
#pragma pack(pop)

extern Game* g_game;
extern void __stdcall FUN_00490df0(unsigned int param1, int param2);

// FUNCTION: 0x490ee0
void FUN_00490ee0()
{
    unsigned short value = g_game->field_38a4b;
    if (value < 0x14) {
        FUN_00490df0(value + 1, 1);
    }
}
