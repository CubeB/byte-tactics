// Decompiled by Haiku. Names are provisional.

class Class_004c93b0 {
public:
    void Assign(int* param_1);
};

class Class_004c5470 {
public:
    void* FUN_004c5470(int* param_1);
};

// FUNCTION: 0x4c5470
void* Class_004c5470::FUN_004c5470(int* param_1)
{
    ((Class_004c93b0*)this)->Assign(param_1);
    ((Class_004c93b0*)((char*)this + 4))->Assign(param_1 + 1);
    return this;
}
