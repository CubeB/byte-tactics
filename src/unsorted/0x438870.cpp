// Decompiled by Haiku. Names are provisional.
// Class_0043a1f0's override of its base's only virtual (vtable 0x4fd2c8, slot
// 0; see 0x43a420.cpp): it sets flag bits in the dword at +0x4e.

class Class_0043a1e0 {
public:
    virtual void FUN_0043a1e0(unsigned int);
};

class Class_0043a1f0 : public Class_0043a1e0 {
public:
    virtual void FUN_0043a1e0(unsigned int param_1);
};

// FUNCTION: 0x438870
void Class_0043a1f0::FUN_0043a1e0(unsigned int param_1)
{
    int* ptr = (int*)((char*)this + 0x4e);
    *ptr |= param_1;
}
