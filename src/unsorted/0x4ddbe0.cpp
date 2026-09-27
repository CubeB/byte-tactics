// Decompiled by Haiku. Names are provisional.

class Class_004ddbe0 {
public:
    int field_0;                              // +0x0
    unsigned char field_4;                    // +0x4

    Class_004ddbe0* FUN_004ddbe0(int* param_1, unsigned char* param_2);
};

// FUNCTION: 0x4ddbe0
Class_004ddbe0* Class_004ddbe0::FUN_004ddbe0(int* param_1, unsigned char* param_2)
{
    Class_004ddbe0* eax = this;
    int* ecx = param_1;
    int edx = *ecx;
    unsigned char* ecx2 = (unsigned char*)param_2;
    eax->field_0 = edx;
    unsigned char dl = *ecx2;
    eax->field_4 = dl;
    return eax;
}
