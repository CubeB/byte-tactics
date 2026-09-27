// Decompiled by Haiku. Names are provisional.

extern int DAT_00511de8;

class Class_0048f610 {
public:
    char unknown_0[0xc];
    int field_c;

    int FUN_0048f610();
};

// FUNCTION: 0x48f610
int Class_0048f610::FUN_0048f610() {
    unsigned int game_field = *(unsigned int*)((unsigned char*)&DAT_00511de8 + 0x38a47);
    if ((unsigned int)field_c > game_field) {
        return 0;
    }
    return 1;
}
