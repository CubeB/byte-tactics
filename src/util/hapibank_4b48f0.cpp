// Decompiled by Haiku. Names are provisional.

class Class_004b48f0 {
public:
    int FUN_004b48f0(const char* param_1);
    int FUN_004b4910(const char* param_1, int param_2);
};

// FUNCTION: 0x4b48f0
int Class_004b48f0::FUN_004b48f0(const char* param_1)
{
    int iVar1 = FUN_004b4910(param_1, 0);
    return iVar1 >= 0 ? 1 : 0;
}
