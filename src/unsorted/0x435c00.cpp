// Decompiled by Haiku. Names are provisional.

extern void __stdcall FUN_00435da0(char*);

struct Class_00435c00 {
    char unknown_0[0xc18];
    int field_c18;
    int field_c1c;

    void FUN_00435c00(int param_1);
};

// FUNCTION: 0x435c00
void Class_00435c00::FUN_00435c00(int param_1)
{
    field_c1c = 0;
    field_c18 = param_1;
    FUN_00435da0(0);
}
