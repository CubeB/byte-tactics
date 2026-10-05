// Decompiled by Haiku. Names are provisional.

class Class_004c93b0 {
public:
    char unknown_0[4];
    int field_at_4;

    void* FUN_00432d20(int* param);
    void Assign(int* param);
};

// FUNCTION: 0x432d20
void* Class_004c93b0::FUN_00432d20(int* param)
{
    Assign(param);
    field_at_4 = param[1];
    return this;
}
