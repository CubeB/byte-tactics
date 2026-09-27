// Decompiled by Haiku. Names are provisional.

struct Class_004c93b0 {
    void FUN_004c93b0(int);
};

class Class_004b7e00 {
public:
    Class_004c93b0 obj0;        // +0x0
    int field1;                 // +0x4
    int field2;                 // +0x8

    void* FUN_004b7e00(int* param_1);
};

// FUNCTION: 0x4b7e00
void* Class_004b7e00::FUN_004b7e00(int* param_1)
{
    obj0.FUN_004c93b0((int)param_1);
    field1 = param_1[1];
    field2 = param_1[2];
    return this;
}
