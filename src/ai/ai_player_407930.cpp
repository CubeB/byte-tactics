// Decompiled by Sonnet, class family consolidated by Opus. Names are provisional.
// Constructor of Class_00407930 (vtable 0x4fc988), derived from
// Class_00407350 (the family is listed in 0x407350.cpp). The owner creates
// two of them, with (3, 20000) and (7, 50000). MSVC stores the vtable after
// the member initialisers, so +0x1c and +0x20 are initialised in the list
// and the rest in the body.

struct Class_00408cb0 {                // the owner (constructor 0x408cb0)
    char unknown_0[4];
    unsigned char field_4;             // +0x4
};

// Vtable 0x4fc980, constructor 0x407350, ??_G 0x407390.
class Class_00407350 {
public:
    Class_00408cb0* owner;             // +0x4
    void* field_8;                     // +0x8
    int field_c;                       // +0xc
    unsigned int field_10;             // +0x10

    Class_00407350(Class_00408cb0* p, void* q);
    virtual void FUN_00407380();                    // slot 0
    virtual ~Class_00407350() {}                    // slot 1
};

// Vtable 0x4fc988, constructor 0x407930, ??_G 0x407980.
class Class_00407930 : public Class_00407350 {
public:
    int field_14;                      // +0x14
    int field_18;                      // +0x18
    int field_1c;                      // +0x1c
    int field_20;                      // +0x20
    int field_24;                      // +0x24

    Class_00407930(Class_00408cb0* p, void* q, int a, int b);
    virtual void FUN_00407380();                    // slot 0, 0x4077e0
};

// The base constructor (0x407350, matched in 0x407350.cpp) was defined in the
// same file, and /Ob2 inlines it below.
Class_00407350::Class_00407350(Class_00408cb0* p, void* q)
    : owner(p), field_8(q), field_c(0), field_10(p->field_4)
{
}

// FUNCTION: 0x407930
Class_00407930::Class_00407930(Class_00408cb0* p, void* q, int a, int b)
    : Class_00407350(p, q), field_1c(b), field_20(a)
{
    field_24 = 0;
    field_18 = 6;
    field_14 = 3;
}
