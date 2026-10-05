// Decompiled by Sonnet. Names are provisional.

extern void* g_game;

class Class_00472e00 {
public:
    char unknown_0[4];
    int field_4;
    int field_8;

    int FUN_00472e00();
};

// FUNCTION: 0x472e00
int Class_00472e00::FUN_00472e00()
{
    if (field_8 <= field_4) {
        unsigned int game_val = *(unsigned int*)((char*)g_game + 0x38a47);
        if ((unsigned int)field_8 <= game_val) {
            return 1;
        }
    }
    return 0;
}
