// Decompiled by space-bunny-free. Names are provisional.

struct Vec3_00438a00 {
    int x, y, z;
};

#pragma pack(push, 1)
struct Def_00438a00 {
    char unknown_0[0x241];
    unsigned int flags;                // +0x241
};

struct Handler_00438a00 {
    virtual void Slot0();
    virtual void Attach(void* obj);
};

struct Owner_00438a00 {
    Handler_00438a00* handler;         // +0
};

struct Unit {
    Owner_00438a00* owner;             // +0
    char unknown_4[0x92 - 4];
    Def_00438a00* def;                 // +0x92
};

struct Attached_00438a00 {
    virtual ~Attached_00438a00();
};
#pragma pack(pop)

class Class_0044d3b0 : public Attached_00438a00 {
public:
    void* field_4;                     // +4
    int field_8;                       // +8
    int field_c;                       // +0xc
    int field_10;                      // +0x10
    int field_14;                      // +0x14
    int field_18;                      // +0x18

    Class_0044d3b0(void* source, int x, int y, int r1, int r2);
};

#pragma pack(push, 1)
class Class_00438a00 {
public:
    char unknown_0[0xe];
    Unit* unit;                        // +0xe
    char unknown_12[0x4e - 0x12];
    unsigned int flags;                // +0x4e
    Attached_00438a00* attached;       // +0x52

    void FUN_00438a00(Vec3_00438a00* pos, int radius1, int radius2);
};
#pragma pack(pop)

// FUNCTION: 0x438a00
void Class_00438a00::FUN_00438a00(Vec3_00438a00* pos, int radius1, int radius2)
{
    if (!(unit->def->flags & 0x800)) {
        Class_0044d3b0* obj = new Class_0044d3b0(this, pos->x, pos->z, radius1, radius2);
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
