// Decompiled by Sonnet. Names are provisional.

int __cdecl FUN_0044fe00();
int __cdecl FUN_00450030();
int __stdcall FUN_00451bc0(int a, int b, void* c, int d);

#pragma pack(push, 1)
struct ParamStruct_0046d4c0
{
    char unknown_0[2];
    int field_2; // +0x2
};
#pragma pack(pop)

class Class_0046d4c0
{
public:
    char unknown_0[0x58];
    int field_58; // +0x58

    void FUN_0046d4c0(unsigned int* param_1, ParamStruct_0046d4c0* param_2, int unused);
};

// FUNCTION: 0x46d4c0
void Class_0046d4c0::FUN_0046d4c0(unsigned int* param_1, ParamStruct_0046d4c0* param_2, int unused)
{
    unsigned int val;
    if (field_58 != 0)
        val = *param_1;
    else
        val = FUN_00450030();

    param_2->field_2 = 0;
    FUN_00451bc0(FUN_0044fe00(), val, param_2, 14);
}
