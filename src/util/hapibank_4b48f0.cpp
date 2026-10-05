// Decompiled by Haiku. Names are provisional.

class Class_004b48f0 {
public:
    int HasItem(const char* param_1);
    int FindItem(const char* param_1, int param_2);
};

// FUNCTION: 0x4b48f0
int Class_004b48f0::HasItem(const char* param_1)
{
    int iVar1 = FindItem(param_1, 0);
    return iVar1 >= 0 ? 1 : 0;
}
