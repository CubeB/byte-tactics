// Decompiled by Haiku. Names are provisional.

struct Class_0044ced0 {
    char unknown_0[4];
    int field_4;

    void FUN_0044ced0(int param_1);
};

// FUNCTION: 0x44ced0
void Class_0044ced0::FUN_0044ced0(int param_1)
{
    int val = field_4;
    if (val != 0) {
        int* target = (int*)(val + 0x4e);
        *target |= param_1;
    }
}
