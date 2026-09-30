// Decompiled by Opus. Names are provisional.

void __cdecl FUN_004d85a0(int* param_1);

class Class_00437a00 {
public:
    char unknown_0[4];
    int* field_4;                      // +0x4

    void FUN_00437a00();
};

// FUNCTION: 0x437a00
void Class_00437a00::FUN_00437a00()
{
    if (field_4 != 0) {
        FUN_004d85a0(field_4);
        field_4 = 0;
    }
}
