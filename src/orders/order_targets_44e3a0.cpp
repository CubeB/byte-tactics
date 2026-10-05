// Decompiled by Haiku. Names are provisional.

#pragma pack(push, 1)
class Class_0044e3a0 {
public:
    char unknown_0[8];
    unsigned char field_8;
    char unknown_9[17];
    int field_1a;

    int FUN_0044e3a0();
};
#pragma pack(pop)

// FUNCTION: 0x44e3a0
int Class_0044e3a0::FUN_0044e3a0() {
    if ((field_8 & 1) == 0 || field_1a == 0) {
        return 0;
    }
    return 1;
}
