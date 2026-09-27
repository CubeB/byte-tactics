// Decompiled by Sonnet. Names are provisional.

struct Class_0044f480_target {
    char unknown_0[0x2e];
    unsigned char field_2e;
};

struct Class_0044f480_link {
    Class_0044f480_target* ptr;
};

struct Class_0044f480 {
    char unknown_0[8];
    Class_0044f480_link* field_8;
    char unknown_c[0x64 - 0xc];
    unsigned char field_64;

    int FUN_0044f480();
};

// FUNCTION: 0x44f480
int Class_0044f480::FUN_0044f480()
{
    return (field_64 & 8) || ((field_8->ptr->field_2e ^ field_64) & 4);
}
