// Decompiled by Opus. Names are provisional.
// Stops a unit (FUN_004898b0(3), FUN_0048aac0 when +0x86 is set, then
// FUN_0048b090(1, 1)); for units whose type has mode 1 in the low bits of
// +0x2e, also attaches a new Class_0044e2d0 at the unit's position to the
// order and sets flags on it.

class Class_004898b0 {
public:
    void FUN_004898b0(int param);
};

class Class_0048b090 {
public:
    void FUN_0048b090(int param_1, int param_2);
};

class Class_004388d0 {
public:
    void FUN_004388d0(int param);
};

class Class_0044e6c0 {
public:
    void FUN_0044e6c0(int param);
};

struct Unit;

class Class_0043d210 {
public:
    char unknown_0[0x2e];
    unsigned char field_2e;            // +0x2e
    void FUN_0043d210(Unit* unit, int state);
};

struct Info_0040f200 {
    char unknown_0[0x21c];
    short field_21c;                   // +0x21c
};

struct Vec3_0044e2d0 {
    int x;
    int y;
    int z;
};

#pragma pack(push, 2)
struct Source_0044e2d0 {
    char unknown_0[6];
    unsigned int flags;                // +0x6
};

struct Unit {
    Class_0043d210* type;              // +0x0
    char unknown_4[0x6a - 0x4];
    Vec3_0044e2d0 pos;                 // +0x6a
    char unknown_76[0x86 - 0x76];
    int field_86;                      // +0x86
    char unknown_8a[0x92 - 0x8a];
    Info_0040f200* info;               // +0x92
};

class Class_0044e2d0 {
public:
    char unknown_0[0x36];

    Class_0044e2d0(Source_0044e2d0* source, const Vec3_0044e2d0& p);
};
#pragma pack(pop)

void __stdcall FUN_0048aac0(Unit* unit, Unit* target, char p3, char p4);

// FUNCTION: 0x40f200
void __stdcall FUN_0040f200(Unit* unit, Source_0044e2d0* order, unsigned int flags)
{
    ((Class_004898b0*)unit)->FUN_004898b0(3);
    if (unit->field_86)
        FUN_0048aac0(unit, 0, -1, 2);
    ((Class_0048b090*)unit)->FUN_0048b090(1, 1);
    if ((unit->type->field_2e & 3) == 1) {
        unit->type->FUN_0043d210(unit, 2);
        Class_0044e2d0* obj = new Class_0044e2d0(order, unit->pos);
        ((Class_0044e6c0*)obj)->FUN_0044e6c0(unit->info->field_21c / 2);
        ((Class_004388d0*)order)->FUN_004388d0((int)obj);
        order->flags |= flags | 0xe0;
    }
}
