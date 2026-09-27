// Decompiled by Haiku. Names are provisional.

extern char* g_game;

class Class_004730c0 {
public:
    char unknown_0[4];
    int field_4;
    int field_8;

    int FUN_004730c0();
};

// FUNCTION: 0x4730c0
int Class_004730c0::FUN_004730c0() {
    if (field_8 <= field_4) {
        int* game_ptr = (int*)g_game;
        unsigned int val = *(unsigned int*)((char*)game_ptr + 0x38a47);
        if ((unsigned int)field_8 <= val) {
            return 1;
        }
    }
    return 0;
}
