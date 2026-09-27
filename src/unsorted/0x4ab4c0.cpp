// Decompiled by Haiku. Names are provisional.

extern void FUN_004c2b20(int);

class Class_004ab4c0 {
public:
    char unknown_0[0x1c];
    int field_1c;
    char unknown_20[0x3c];
    int field_5c;

    void FUN_004ab4c0();
};

// FUNCTION: 0x4ab4c0
void Class_004ab4c0::FUN_004ab4c0() {
    FUN_004c2b20(field_1c);
    field_5c = field_5c & 0xfffffffe;
}
