// Decompiled by Haiku. Names are provisional.
// Class_004907e0's override of slot 7 (vtable 0x4fd9b0, see 0x44ef60.cpp for
// the family): the dirty bit.

class Class_004907e0 {
public:
    char unknown_4[0x27 - 0x4];
    unsigned char field_0x27;

    virtual int FUN_0044efe0();        // slot 7
};

// FUNCTION: 0x4908b0
int Class_004907e0::FUN_0044efe0()
{
    return this->field_0x27 & 1;
}
