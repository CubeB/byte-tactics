// Decompiled by Haiku. Names are provisional.
// Class_0044f010's override of slot 5 (vtable 0x4fd458, see 0x44f450.cpp):
// the active flag (bit 0 of +0x64).

class Class_0044f010 {
public:
    virtual int FUN_0044ef80();        // slot 5
};

// FUNCTION: 0x44f290
int Class_0044f010::FUN_0044ef80()
{
    return *(unsigned char*)((char*)this + 0x64) & 1;
}
