// Decompiled by space-bunny-free. Names are provisional.

struct Point_00438ad0 {
    short x;
    short y;
};

class Handler_00438ad0;

struct Owner_00438ad0 {
    Handler_00438ad0* handler;         // +0x0
};

#pragma pack(push, 1)
struct Def_00438ad0 {
    char unknown_0[0x241];
    unsigned int flags;                // +0x241
};

struct Unit_00438ad0 {
    Owner_00438ad0* owner;             // +0x0
    char unknown_4[0x92 - 4];
    Def_00438ad0* def;                 // +0x92
};
#pragma pack(pop)

class Handler_00438ad0 {
public:
    virtual void Slot0();
    virtual void Attach(void* obj);
};

class Class_0044d8a0 {
public:
    virtual ~Class_0044d8a0();

    void* field_4;                     // +0x4
    int a;                             // +0x8
    int b;                             // +0xc
    int c;                             // +0x10
    int d;                             // +0x14

    Class_0044d8a0(void* owner, Point_00438ad0 pos, Point_00438ad0 size);
};

#pragma pack(push, 1)
class Class_00438ad0 {
public:
    char unknown_0[0xe];
    Unit_00438ad0* unit;               // +0xe
    char unknown_12[0x4e - 0x12];
    unsigned int flags;                // +0x4e
    Class_0044d8a0* attached;          // +0x52

    void FUN_00438ad0(Point_00438ad0 cell, Point_00438ad0 size);
};
#pragma pack(pop)

// FUNCTION: 0x438ad0
void Class_00438ad0::FUN_00438ad0(Point_00438ad0 cell, Point_00438ad0 size)
{
    if (!(unit->def->flags & 0x800)) {
        Class_0044d8a0* obj = new Class_0044d8a0(this, cell, size);
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
        if (unit->owner) {
            if (attached) {
                unit->owner->handler->Attach(0);
                delete attached;
                attached = 0;
            }
        }
    }
}
