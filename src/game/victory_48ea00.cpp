// Decompiled by Haiku. Names are provisional.
// The victory/defeat condition base's IsSatisfied (slot 0 of the condition
// vtables whose class does not override it): returns `satisfied` (+0x4).

class MissionCondition {
public:
    virtual int IsSatisfied();           // IsSatisfied
};

// FUNCTION: 0x48ea00
int MissionCondition::IsSatisfied()
{
    return *(int*)((char*)this + 4);
}
