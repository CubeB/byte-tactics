// Decompiled by Sonnet. Names are provisional.

extern char* g_game;

class Class_0048fd50 {
public:
    char unknown_0[0xc];
    int field_c;

    int FUN_0048fd50();
};

// FUNCTION: 0x48fd50
int Class_0048fd50::FUN_0048fd50()
{
    unsigned int game_val = *(unsigned int*)(g_game + 0x38a47);
    return game_val >= (unsigned int)field_c;
}
