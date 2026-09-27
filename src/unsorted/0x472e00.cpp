// Decompiled by Haiku. Names are provisional.

extern int DAT_00511de8;

class Class_00472e00 {
public:
    char unknown_0[4];
    int field_4;
    int field_8;

    int FUN_00472e00();
};

// FUNCTION: 0x472e00
int Class_00472e00::FUN_00472e00() {
    unsigned int field8 = field_8;
    unsigned int field4 = field_4;
    if (!(field8 > field4)) {
        void* game = &DAT_00511de8;
        unsigned int game_field = *(unsigned int*)((char*)game + 0x38a47);
        if (!(field8 > game_field)) {
            return 1;
        }
    }
    return 0;
}
