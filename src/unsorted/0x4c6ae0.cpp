// Decompiled by Haiku. Names are provisional.

class Class_004c6ae0 {
public:
    char unknown_0[0x1c];
    int field_1c;
    int field_20;
    int field_24;
    int field_28;

    void FUN_004c6ae0(int*);
};

// FUNCTION: 0x4c6ae0
void Class_004c6ae0::FUN_004c6ae0(int* param_1)
{
    int* p = (int*)((char*)this + 0x1c);
    int temp;
    temp = p[0];
    param_1[0] = temp;
    temp = p[1];
    param_1[1] = temp;
    temp = p[2];
    param_1[2] = temp;
    temp = p[3];
    param_1[3] = temp;
}
