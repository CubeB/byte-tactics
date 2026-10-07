// Decompiled by Claude Opus 5.5. Names are provisional.
// <stdio.h> (or another header, see tools/headers.py) is needed only for
// compiler state: without it the energy + metal sum loads metal first.
#include <stdio.h>

struct Vec3 {
    int x, y, z;
};

struct Box {
    Vec3 lo;
    Vec3 hi;
};

struct Point16 {
    short x;
    short z;
};

struct Unit;

class UnitMotion {
public:
    char unknown_0[0x2e];
    unsigned char field_2e;            // +0x2e
    void SetFlightMode(Unit* unit, int state);
};
class Class_004388d0 { public: void FUN_004388d0(int); };
class Class_00438880 { public: void FUN_00438880(const char*); };
class Class_0044e6c0 { public: void FUN_0044e6c0(int); };
class Class_00439e80 { public: void FUN_00439e80(int ticks); };

#pragma pack(push, 1)
struct Feature {
    char unknown_0[0x94];
    Point16 footprint;                 // +0x94
    char unknown_98[0xec - 0x98];
    float metal;                       // +0xec
    float energy;                      // +0xf0
    char unknown_f4[0xfa - 0xf4];
    unsigned char height;              // +0xfa
    char unknown_fb[0xfe - 0xfb];
    unsigned char flags;               // +0xfe
    char unknown_ff[0x100 - 0xff];
};

struct UnitDef {
    char unknown_0[0x21c];
    short field_21c;                   // +0x21c
    char unknown_21e[0x241 - 0x21e];
    unsigned int flags;                // +0x241
    unsigned int flags2;               // +0x245
};

struct Unit {
    UnitMotion* type;                  // +0x0
    char unknown_4[0x6a - 0x4];
    Vec3 pos;                          // +0x6a
    char unknown_76[0x86 - 0x76];
    int field_86;                      // +0x86
    char unknown_8a[0x92 - 0x8a];
    UnitDef* def;                      // +0x92
    char unknown_96[0xb0 - 0x96];
    int workTime;                      // +0xb0
    void ClaimWeapons(int);
    void SetStateBits(int, int);
};
// Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
int __stdcall FindLandingPad(Unit*, int);
unsigned short __stdcall ChooseBuildOption(unsigned int, Unit*);
void __stdcall ClearWeaponTarget(Unit*, int);
void __stdcall DetonateUnitWeapon(Unit*, int);
void __stdcall DrawUnit(void*, Unit*);


struct Order {
    char unknown_0[5];
    unsigned char state;               // +0x5
    unsigned int flags;                // +0x6
    char unknown_a[0x22 - 0xa];
    Vec3 pos;                          // +0x22
    char unknown_2e[0x36 - 0x2e];
    int time;                          // +0x36
};

class Class_0044e2d0 {
public:
    char unknown_0[0x36];
    Class_0044e2d0(Order* order, const Vec3& pos);
};

struct Game {
    char unknown_0[0x1426f];
    Feature* features;                 // +0x1426f
    char unknown_14273[0x38a47 - 0x14273];
    int ticks;                         // +0x38a47
};
#pragma pack(pop)

extern Game* g_game;

unsigned short __stdcall FindFeatureAtPos(Vec3* pos, Point16* cell, Point16* size);
void __stdcall QueueUnitSpeech(Unit* unit, int kind, char* text);
int __stdcall GetGroundHeight(Vec3* pos);
void __stdcall GetNanoPiecePosition(Unit* unit, Vec3* out);
void __stdcall EmitReverseNanoParticles(Box* from, Vec3* to, int count);
void __stdcall ReclaimFeature(Unit* unit, Vec3* pos);
void __stdcall AttachUnitToPiece(Unit* unit, Unit* target, char p3, char p4);

// 0x40f200, matched in 0x40f200.cpp; inlined into the state 0 case below.
void __stdcall FUN_0040f200(Unit* unit, Order* order, unsigned int flags)
{
    unit->ClaimWeapons(3);
    if (unit->field_86)
        AttachUnitToPiece(unit, 0, -1, 2);
    unit->SetStateBits(1, 1);
    if ((unit->type->field_2e & 3) == 1) {
        unit->type->SetFlightMode(unit, 2);
        Class_0044e2d0* obj = new Class_0044e2d0(order, unit->pos);
        ((Class_0044e6c0*)obj)->FUN_0044e6c0(unit->def->field_21c / 2);
        ((Class_004388d0*)order)->FUN_004388d0((int)obj);
        order->flags |= flags | 0xe0;
    }
}

// Order handler "Reclaiming" (second variant, driven by a Class_0044e2d0
// move object rather than the turn/approach states of 0x404ad0).
// FUNCTION: 0x414770
int __stdcall VtolReclaimOrder(Unit* unit, Order* order, int flags)
{
    Point16 cell;
    Point16 size;
    unsigned short index = FindFeatureAtPos(&order->pos, &cell, &size);
    if (index == 0xffff) {
        QueueUnitSpeech(unit, 7, "Reclamation failed");
        return 8;
    }
    Feature* f = &g_game->features[index];
    if (!(f->flags & 0x80))
        return 8;
    switch (order->state) {
    case 0:
        if (unit->type && (unit->def->flags & 0x800) && (unit->def->flags2 & 0x400)) {
            ((Class_00438880*)order)->FUN_00438880("Reclaiming");
            FUN_0040f200(unit, order, 0);
            return 1;
        }
        break;
    case 1: {
        Class_0044e2d0* obj = new Class_0044e2d0(order, order->pos);
        ((Class_004388d0*)order)->FUN_004388d0((int)obj);
        order->flags = 0xe0;
        return 1;
    }
    case 2:
        if (flags & 0x40)
            return 8;
        order->time = (int)(30.0f - (f->energy + f->metal) * -0.5f);
        QueueUnitSpeech(unit, 11, 0);
        return 1;
    case 3:
        ((Class_00439e80*)order)->FUN_00439e80(2);
        order->time -= 2;
        if (order->time <= 0)
            return 1;
        unit->workTime = g_game->ticks + 300;
        if (order->time > 30) {
            Vec3 nano;
            GetNanoPiecePosition(unit, &nano);
            Box box;
            box.lo.x = cell.x << 20;
            box.lo.z = cell.z << 20;
            box.lo.y = GetGroundHeight(&box.lo) << 16;
            box.hi = box.lo;
            box.hi.x += f->footprint.x << 20;
            box.hi.z += f->footprint.z << 20;
            box.hi.y += f->height << 16;
            EmitReverseNanoParticles(&box, &nano, 6);
            EmitReverseNanoParticles(&box, &nano, 6);
        }
        return 2;
    case 4:
        ReclaimFeature(unit, &order->pos);
        return 5;
    }
    return 7;
}
