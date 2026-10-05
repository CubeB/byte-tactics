// Decompiled by Sonnet, class family consolidated by Opus. Names are provisional.
// The compiler-generated scalar deleting destructor of Class_00407350
// (vtable 0x4fc980), the base of the family listed in 0x407350.cpp. Its
// destructor is empty and inline, so only the vtable store is left.
//
// The constructor (0x407350, matched in 0x407350.cpp) is defined again below,
// unannotated, to emit the vtable and with it this COMDAT.

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

// FUNCTION: 0x407390 ??_GClass_00407350@@UAEPAXI@Z
Class_00407350::Class_00407350(Class_00408cb0* p, void* q)
    : owner(p), field_8(q), field_c(0), field_10(p->field_4)
{
}
