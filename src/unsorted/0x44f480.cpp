// Decompiled by Sonnet. Names are provisional.
// Class_0044f010's override of slot 7 (vtable 0x4fd458, see 0x44f450.cpp):
// true when there is something to send: flag 3 of +0x64 (the path changed) is
// set, or flag 2 no longer matches bit 2 of the owner's target. Slot 8
// (0x44f4a0) writes the path out and brings both flags up to date.

struct Target_0044f480 {
    char unknown_0[0x2e];
    unsigned char field_2e;
};

struct Link_0044f480 {
    Target_0044f480* ptr;
};

class Class_0044f010 {
public:
    char unknown_4[8 - 4];
    Link_0044f480* field_8;
    char unknown_c[0x64 - 0xc];
    unsigned char field_64;

    virtual int FUN_0044efe0();        // slot 7
};

// FUNCTION: 0x44f480
int Class_0044f010::FUN_0044efe0()
{
    return (field_64 & 8) || ((field_8->ptr->field_2e ^ field_64) & 4);
}
