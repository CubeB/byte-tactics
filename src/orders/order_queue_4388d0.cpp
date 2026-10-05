// Decompiled by Opus. Names are provisional.

class Handler_004388d0 {
public:
    virtual void Slot0();
    virtual void Attach(void* obj);
};

struct Owner_004388d0 {
    Handler_004388d0* handler;          // +0
};

struct Unit {
    Owner_004388d0* owner;              // +0
};

class Attached_004388d0 {
public:
    virtual ~Attached_004388d0();
};

#pragma pack(push, 1)
class Class_004388d0 {
public:
    char unknown_0[0xe];
    Unit* unit;                         // +0xe
    char unknown_12[0x4e - 0x12];
    unsigned int flags;                 // +0x4e
    Attached_004388d0* attached;        // +0x52

    void FUN_004388d0(Attached_004388d0* obj);
};
#pragma pack(pop)

// FUNCTION: 0x4388d0
void Class_004388d0::FUN_004388d0(Attached_004388d0* obj)
{
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
}
