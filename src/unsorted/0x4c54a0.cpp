// Decompiled by Haiku. Names are provisional.

class Class_004c91a0 {
public:
    void FUN_004c91a0(int* param_1);
};

class Class_004c54a0 {
public:
    char unknown_0[4];
    int field_4;                              // +0x4

    void* FUN_004c54a0(int* param_1);
};

// FUNCTION: 0x4c54a0
void* Class_004c54a0::FUN_004c54a0(int* param_1)
{
    ((Class_004c91a0*)this)->FUN_004c91a0(param_1);
    ((Class_004c91a0*)((unsigned char*)this + 4))->FUN_004c91a0(param_1 + 1);
    return this;
}
