// Decompiled by space-bunny-free. Names are provisional.
// Creates a Class_0044cf60 from an order position when the unit's definition
// does not have flag 0x800 set, detaches the current attachment and attaches
// the new one. Same class as 0x4388d0; the flag test comes first here.

class Handler_004388d0 {
public:
    virtual void Slot0();
    virtual void Attach(void* obj);
};

struct Owner_004388d0 {
    Handler_004388d0* handler;          // +0
};

class Attached_004388d0 {
public:
    virtual ~Attached_004388d0();
};

#pragma pack(push, 1)
struct Flag_00438930 {
    char unknown_0[0x241];
    unsigned int value;                 // +0x241
};

struct Unit_00438930 {
    Owner_004388d0* owner;              // +0
    char unknown_4[0x8e];
    Flag_00438930* def;                 // +0x92
};
#pragma pack(pop)

struct Source_0044cf60;

class Class_0044cf60 : public Attached_004388d0 {
public:
    char unknown_4[0x10];
    Class_0044cf60(Source_0044cf60* source, int x, int y, int r);
};

#pragma pack(push, 1)
class Class_00438930 {
public:
    char unknown_0[0xe];
    Unit_00438930* unit;                // +0xe
    char unknown_12[0x4e - 0x12];
    unsigned int flags;                 // +0x4e
    Attached_004388d0* attached;        // +0x52

    void FUN_00438930(int* p, int n);
};
#pragma pack(pop)

// FUNCTION: 0x438930
void Class_00438930::FUN_00438930(int* p, int n)
{
    if ((unit->def->value & 0x800) == 0) {
        Class_0044cf60* obj = new Class_0044cf60((Source_0044cf60*)this, p[0], p[2], n);
        if (unit->owner) {
            if (attached) {
                unit->owner->handler->Attach(0);
                delete attached;
                attached = 0;
            }
            if (obj) {
                flags &= ~0x3e0;
                unit->owner->handler->Attach(obj);
                attached = obj;
            }
        }
    } else {
        if (unit->owner && attached) {
            unit->owner->handler->Attach(0);
            delete attached;
            attached = 0;
        }
    }
}
