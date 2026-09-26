// Decompiled by Haiku. Names are provisional.

extern int (__stdcall* DAT_004fc0f0)(char*, const char*, int);

struct Class_004d8b30 {
    char unknown_0[0x98];
    char field_98[0x20];

    void FUN_004d8b30(const char* param_1);
};

// FUNCTION: 0x4d8b30
void Class_004d8b30::FUN_004d8b30(const char* param_1)
{
    if (param_1 != 0) {
        DAT_004fc0f0(field_98, param_1, 0x20);
    } else {
        field_98[0] = 0;
    }
}
