// Decompiled by Sonnet, class family consolidated by Opus. Names are provisional.
// Constructor of Class_00408810 (vtable 0x4fc9b0), derived from
// Class_00407350 (the family is listed in 0x407350.cpp) without new fields.
// The class keeps the name its scalar deleting destructor (0x408810) gave it.

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

// Vtable 0x4fc9b0, constructor 0x4087e0, ??_G 0x408810.
class Class_00408810 : public Class_00407350 {
public:
    Class_00408810(Class_00408cb0* p, void* q);
    virtual void FUN_00407380();                    // slot 0, 0x4086d0
};

// The base constructor (0x407350, matched in 0x407350.cpp) was defined in the
// same file, and /Ob2 inlines it below.
Class_00407350::Class_00407350(Class_00408cb0* p, void* q)
    : owner(p), field_8(q), field_c(0), field_10(p->field_4)
{
}

// FUNCTION: 0x4087e0
Class_00408810::Class_00408810(Class_00408cb0* p, void* q)
    : Class_00407350(p, q)
{
}
