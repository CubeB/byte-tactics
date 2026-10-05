// Decompiled by Opus. Names are provisional.
// The counterpart of 0x490ee0: steps a 16-bit game setting down by one.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x38a4b];
    unsigned short field_38a4b;        // +0x38a4b
};
#pragma pack(pop)

extern Game* g_game;
void __stdcall SetGameSpeed(unsigned int param1, int param2);

// FUNCTION: 0x490f10
void FUN_00490f10()
{
    unsigned short value = g_game->field_38a4b;
    if (value > 1) {
        SetGameSpeed(value - 1, 1);
    }
}
