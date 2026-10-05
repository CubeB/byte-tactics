// Decompiled by Haiku. Names are provisional.
// The victory/defeat condition base's IsSatisfied (slot 0 of the condition
// vtables whose class does not override it): returns `satisfied` (+0x4).

class Condition_0048ff40 {
public:
    virtual int FUN_0048ea00();          // IsSatisfied
};

// FUNCTION: 0x48ea00
int Condition_0048ff40::FUN_0048ea00()
{
    return *(int*)((char*)this + 4);
}
