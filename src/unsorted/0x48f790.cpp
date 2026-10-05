// Decompiled by Opus. Names are provisional.
// Slot 0 of the unit visitor at +0xc of the "all units killed" defeat
// condition (vtable 0x4fd7f8, stored by the constructors inlined at 0x48e6e5
// and friends). Clears the condition when it meets a live, finished unit.

#pragma pack(push, 1)
struct Unit {
    char unknown_0[0x86];
    Unit* field_86;                    // +0x86
    char unknown_8a[0xfb - 0x8a];
    int field_fb;                      // +0xfb
    char unknown_ff[0x104 - 0xff];
    float field_104;                   // +0x104
    char unknown_108[0x110 - 0x108];
    unsigned int flags;                // +0x110
};
#pragma pack(pop)

class Base_0048f7e0 {
public:
    virtual int FUN_0048ea00();        // IsSatisfied
    int satisfied;                     // +0x4
    int celebrated;                    // +0x8
};

// Interface at +0xc of the object: slot 0 is called for each unit.
class UnitCallback_0048f7e0 {
public:
    virtual int FUN_0048f790(Unit* unit) = 0;
};

// `this` is the visitor subobject (+0xc), so satisfied sits at -8.
class Class_0048f840 : public Base_0048f7e0, public UnitCallback_0048f7e0 {
public:
    virtual int FUN_0048f790(Unit* unit);
};

// FUNCTION: 0x48f790
int Class_0048f840::FUN_0048f790(Unit* unit)
{
    if ((unit->flags & 0x20) && unit->field_104 == 0.0f && unit->field_fb == 0
        && (unit->field_86 == 0 || (unit->field_86->flags & 0x40000000))) {
        satisfied = 0;
    }
    return satisfied;
}
