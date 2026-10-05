// Decompiled by Sonnet. Names are provisional.

class Obj {
public:
    virtual void FUN_Virtual0(int);
};

class UnitMotion {
public:
    Obj* field_0;

    void FUN_0043dd10();
};

// FUNCTION: 0x43dd10
void UnitMotion::FUN_0043dd10()
{
    if (field_0 != 0) {
        field_0->FUN_Virtual0(1);
    }
}
