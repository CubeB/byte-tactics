// Decompiled by Opus. Names are provisional.
// Destructor of the 32-byte entry whose out-of-line constructor is 0x4402e0
// (0x440290.cpp inlines both for its array of entries, as Class_00440320).
// Its one caller (0x42bf40) constructs a local entry with 0x4402e0 and calls
// this on it (ecx) when the local goes out of scope.

void FUN_004d85a0(int*);

struct Class_004402e0 {
    int* field_0;
    short field_4;
    short field_6;
    short field_8;
    short field_a;
    unsigned char field_c;
    unsigned char field_d;
    unsigned char field_e;
    unsigned char field_f;
    int field_10;
    int field_14;
    void* field_18;
    int field_1c;

    Class_004402e0();
    ~Class_004402e0();
};

// FUNCTION: 0x440320
Class_004402e0::~Class_004402e0()
{
    FUN_004d85a0(field_0);
    operator delete(field_18);
}
