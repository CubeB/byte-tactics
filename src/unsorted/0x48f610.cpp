// Decompiled by Sonnet. Names are provisional.

extern void* g_game;

class Class_0048f610 {
public:
    char unknown_0[0xc];
    unsigned int field_c;

    int FUN_0048f610();
};

// FUNCTION: 0x48f610
int Class_0048f610::FUN_0048f610()
{
    unsigned int game_val = *(unsigned int*)((char*)g_game + 0x38a47);
    return game_val >= field_c;
}
