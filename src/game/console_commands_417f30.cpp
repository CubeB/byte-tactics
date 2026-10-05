// Decompiled by Opus. Names are provisional.

class Iface_00417f30 {
public:
    virtual void Method_00();
    virtual void Method_04();
    virtual void Method_08();
    virtual void Method_0c();
    virtual void Method_10();
    virtual void Method_14();
    virtual void Method_18();
    virtual void Method_1c();
    virtual void Method_20();
    virtual void Method_24();
    virtual void Method_28(int param_1);
};

// FUNCTION: 0x417f30
void __stdcall FUN_00417f30(int param_1, Iface_00417f30*** param_2)
{
    if (param_2 && *param_2 && **param_2) {
        (**param_2)->Method_28(param_1);
    }
}
