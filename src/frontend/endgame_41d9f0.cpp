// Decompiled by Haiku. Names are provisional.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x39057];
    int field_39057;
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x41d9f0
void __stdcall FUN_0041d9f0(unsigned int param_1)
{
    unsigned int v = param_1 & 0xff;
    g_game->field_39057 = v;
}
