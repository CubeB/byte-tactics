// Decompiled by Opus, class hierarchy fixed by Claude Opus 5.5. Names are provisional.
// Destructor (callers do `if (p) { p->~X(); operator delete(p); }`): notifies
// the owner through a callback table, stops the unit's build animation (the
// same code as StopBuildingScript), releases the attached object at +0x52 and
// unlinks the list node at +0x12 (0x489650 is the link's destructor).

class CobScript {
public:
    int FindScript(char* name);
    int StartScriptWithArgsByIndex(int index, void* param_2, int param_3, int param_4, int param_5, int param_6, int param_7, int param_8);
};

class UnitRef {
public:
    void FUN_00489650();
};

// Object at +0x52, deleted through its virtual destructor.
class Attached_0043a1f0 {
public:
    virtual ~Attached_0043a1f0();
};

class Slot_0043a1f0 {
public:
    virtual void Slot0();
    virtual void Slot1(int param_1);
    Attached_0043a1f0* current;        // +0x4
};

struct Owner_0043a1f0 {
    Slot_0043a1f0* slot;               // +0x0
};

#pragma pack(push, 1)
struct Unit {
    Owner_0043a1f0* owner;             // +0x0
    char unknown_4[0x9a - 0x4];
    CobScript* names;                  // +0x9a
    void ReleaseWeapons(int param_1);
};

class Class_0043a1f0;

struct Callback_0043a1f0 {
    char unknown_0[4];
    void (__stdcall* notify)(Unit* unit, Class_0043a1f0* obj, int code);           // +0x4
    char unknown_8[0x19 - 0x8];
};

// The base class (vtable 0x4fd2cc, one slot, the empty 0x43a1e0). It has no
// destructor of its own, so this destructor stores only the derived vtable.
class Class_0043a1e0 {
public:
    virtual void FUN_0043a1e0(unsigned int);
};

class Class_0043a1f0 : public Class_0043a1e0 {
public:
    // Slot 0 of vtable 0x4fd2c8, overriding the base's: 0x438870 (defined
    // in 0x438870.cpp). See 0x43a0c0.cpp, the constructor.
    virtual void FUN_0043a1e0(unsigned int);
    unsigned char kind;                // +0x4
    char unknown_5;
    unsigned char flags_6;             // +0x6
    char unknown_7[0xe - 0x7];
    Unit* unit;                        // +0xe
    char unknown_12[0x42 - 0x12];
    unsigned int flags;                // +0x42
    char unknown_46[0x52 - 0x46];
    Attached_0043a1f0* attached;       // +0x52

    ~Class_0043a1f0();
};
#pragma pack(pop)

extern Callback_0043a1f0* DAT_00512344;

int __stdcall SendScriptCallNoArgs(Unit* obj, short index);

// FUNCTION: 0x43a1f0
Class_0043a1f0::~Class_0043a1f0()
{
    if (flags_6 & 2) {
        DAT_00512344[kind].notify(unit, this, 2);
    }
    if (flags & 0x400000) {
        Unit* obj = unit;
        int index = obj->names->FindScript("StopBuilding");
        ((CobScript*)obj->names)->StartScriptWithArgsByIndex(index, 0, 0, 0, 0, 0, 0, 0);
        SendScriptCallNoArgs(obj, index);
        flags &= ~0x400000;
    }
    if (unit->owner != 0) {
        Slot_0043a1f0* slot = unit->owner->slot;
        if (slot->current != 0 && slot->current == attached) {
            if (attached != 0) {
                slot->Slot1(0);
                delete attached;
                attached = 0;
            }
        } else {
            delete attached;
            attached = 0;
        }
    }
    if (!(flags & 0x10000)) {
        ((Unit*)unit)->ReleaseWeapons(3);
    }
    ((UnitRef*)((char*)this + 0x12))->FUN_00489650();
}
