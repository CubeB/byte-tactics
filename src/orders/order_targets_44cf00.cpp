// Decompiled by Sonnet. Names are provisional.

class Class_0044cf00
{
public:
    virtual void FUN_00000000();
    virtual void FUN_00000001();
    virtual void FUN_00000002();
    virtual void FUN_00000003();
    virtual void FUN_00000004();
    virtual void FUN_00000005(int param_1, int param_2);

    void FUN_0044cf00(int param_1);
};

// FUNCTION: 0x44cf00
void Class_0044cf00::FUN_0044cf00(int param_1)
{
    short esi = *(short*)(param_1 + 0x78);
    short eax = *(short*)(param_1 + 0x76);

    FUN_00000005(eax, esi);
}
