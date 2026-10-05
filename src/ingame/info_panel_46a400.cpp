// Decompiled by Haiku. Names are provisional.

unsigned int __cdecl GetMilliseconds(void);

class Class_0046a400 {
public:
    void FUN_0046a400(int param_1);
};

// FUNCTION: 0x46a400
void Class_0046a400::FUN_0046a400(int param_1)
{
    unsigned int result = GetMilliseconds();
    int edi = *(int*)((char*)this);
    int edx = result - edi;
    int field_val = *(int*)((char*)this + param_1 * 4 + 0x2c);
    field_val = field_val + edx;
    *(int*)((char*)this + param_1 * 4 + 0x2c) = field_val;
    *(int*)((char*)this) = result;
}
