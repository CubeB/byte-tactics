// Decompiled by Sonnet. Names are provisional.

int __stdcall FUN_004b6ba0(unsigned int a, unsigned int b);

#pragma pack(push, 1)
class Class_004c43f0 {
public:
    char unknown_0[0x25];
    int field_25;                       // +0x25
    void FUN_004c43f0(unsigned int param_1, unsigned int param_2);
};
#pragma pack(pop)

// FUNCTION: 0x4c43f0
void Class_004c43f0::FUN_004c43f0(unsigned int param_1, unsigned int param_2)
{
    if (param_1 <= param_2) {
        field_25 = FUN_004b6ba0(param_1, param_2 - param_1 - 1);
    } else {
        field_25 = 0;
    }
}
