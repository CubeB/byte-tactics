// Decompiled by LongCat 2.5 Preview Free, finished by space-bunny-free, finished by GPT-6,
// finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by Claude Opus 5.5.
// Names are provisional.
//
// What made this match (87.4% before):
// - Both aim arms read DAT_00509688[(e->flags >> 2) & 3] afresh for each call,
//   as the original does; no cached name local. With the temporaries in the
//   original's rotation, MSVC merges the two SendScriptCallByName tails at 0x49e393.
// - The bit-4 test in the non-b19 arm goes through a `bool` local. Testing the
//   bitfield in place compiles to `test al, 0x10`, which skips one step of the
//   eax/ecx/edx rotation, so the later temporaries land one register off and
//   the tails stop merging. The `bool` (or an `unsigned char`) gives the
//   original's `mov edx, eax; shr edx, 4; test dl, 1`.
#include <string.h>

struct Vec3_0049e1a0 {
    int x;
    int y;
    int z;
};

class CobScript {
  public:
    int StartScriptWithArgs(char* name, void* param_2, int param_3, int param_4, int param_5, int param_6,
                     int param_7, int param_8);
};

#pragma pack(push, 1)
struct Store_0049e1a0 {
    char unknown_0[0x8c];
    float metal; // +0x8c
    char unknown_90[0x98 - 0x90];
    float energy; // +0x98
};
#pragma pack(pop)

#pragma pack(push, 1)
class UnitResources {
  public:
    char unknown_0[0x4];
    float x0;
    char unknown_8[0x1c - 0x8];
    float y0;
    char unknown_20[0x30 - 0x20];
    Store_0049e1a0* store; // +0x30
    int SpendEnergyAndMetal(float dx, float dy);
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Point_0049e1a0 {
    short x;
    short z;
};

struct Unit;

struct Flags_0049e1a0 {
    unsigned int b0 : 1, b1 : 1, b2_3 : 2, b4 : 1, b5_18 : 14, b19 : 1, b20_25 : 6, b26 : 1,
        b27 : 1, b28 : 1, b29_31 : 3;
};

// The unit a slot points at. Its class vtable sits at +0x60.
struct Target_0049e1a0 {
    char unknown_00[0x60];
    int(__stdcall* f60)(Unit*, Point_0049e1a0*, Unit*, Vec3_0049e1a0*);
    char unknown_64[0x68 - 0x64];
    int f_68;
    char unknown_6c[0xc0 - 0x6c];
    float f_c0;
    float f_c4;
    float f_c8;
    char unknown_cc[0xe4 - 0xcc];
    unsigned short f_e4;
    char unknown_e6[0x111 - 0xe6];
    Flags_0049e1a0 f_111;
};

struct Entry_0049e1a0 {        // 0x1c bytes
    Point_0049e1a0 point;      // +0x00
    char* name;                // +0x04
    int f_8;                   // +0x08
    Target_0049e1a0* attached; // +0x0c
    char unknown_10[0x14 - 0x10];
    unsigned short f_14; // +0x14, the slot's tick counter
    short f_16;          // +0x16
    short f_18;          // +0x18
    unsigned char f_1a;  // +0x1a
    unsigned char flags; // +0x1b
};

struct UnitType_0049e1a0 {
    char unknown_0[0x1fa];
    unsigned int f_1fa;
};

struct Unit {
    char unknown_00[0x4];
    Entry_0049e1a0 entries[3]; // +0x04
    char unknown_58[0x66 - 0x58];
    short heading; // +0x66
    char unknown_68[0x6a - 0x68];
    Vec3_0049e1a0 pos; // +0x6a
    char unknown_76[0x92 - 0x76];
    UnitType_0049e1a0* type; // +0x92
    char unknown_96[0x9a - 0x96];
    CobScript* script; // +0x9a
    char unknown_9e[0xb8 - 0x9e];
    unsigned short f_b8; // +0xb8
    union {              // +0xba
        unsigned short w;
        unsigned char b[2];
    } f_ba;
    UnitResources f_bc; // +0xbc
    char unknown_f0[0x108 - 0xf0];
    short f_108; // +0x108
};
#pragma pack(pop)

// The three aim script names. The original's array has three elements, with
// the string "AimTertiary" right after it, so a weapon index of 3 would read
// string bytes as a pointer.
extern char* DAT_00509688[3];

int __stdcall GetWeaponTargetPos(Unit* unit, Vec3_0049e1a0* pos, int index);
Unit* __stdcall GetWeaponTargetUnit(Unit* obj, int index);
void __stdcall GetAimFromPosition(Unit* unit, Vec3_0049e1a0* out, unsigned char weapon);
void __stdcall FUN_0049e570(Vec3_0049e1a0* a, Vec3_0049e1a0* b, int* dx, int* dy, int* dz);
short __cdecl FUN_004b715a(int x, int z);
unsigned short __stdcall SolveLaunchAngle(int a, int b, int c, int d, float e);
int __stdcall CalcAimAngles(Unit* unit, Target_0049e1a0* target,
                           unsigned short* out_heading, unsigned short* out_pitch,
                           unsigned char weapon, Vec3_0049e1a0* point);
int __stdcall WeaponCanReachPos(Unit* unit, Vec3_0049e1a0* a2, Vec3_0049e1a0* a3,
                           unsigned char a4);
int __stdcall SendScriptCallByName(Unit* obj, char* name, char field_5, int field_6, int field_a,
                           unsigned short field_e, unsigned short field_12);
void __stdcall FUN_0041c150(Unit* unit);

// FUNCTION: 0x49e1a0
void __stdcall UpdateUnitWeapons(Unit* unit) {
    unsigned short heading;
    unsigned char i;
    int dz;
    int dx;
    int dy;
    Vec3_0049e1a0 pos;
    Vec3_0049e1a0 aim;

    for (i = 0; i < 3; i++) {
        Entry_0049e1a0* e = &unit->entries[i];
        unsigned char fl = e->flags;
        Target_0049e1a0* attached = e->attached;
        if (!(fl & 2))
            continue;
        if (e->f_14 > 0)
            e->f_14--;
        if (!GetWeaponTargetPos(unit, &pos, i)) {
            e->flags &= 0xfe;
            continue;
        }
        if (attached->f60 == 0)
            continue;
        if (attached->f_111.b19) {
            if (!(e->flags & 1)) {
                Target_0049e1a0* t = e->attached;
                unsigned short angle;
                int ok;
                if (t->f_111.b1) {
                    GetAimFromPosition(unit, &aim, (unsigned char)((e->flags >> 2) & 3));
                    FUN_0049e570(&aim, &pos, &dx, &dy, &dz);
                    heading = (unsigned short)(FUN_004b715a(dx, dz) - unit->heading);
                    angle = SolveLaunchAngle(dx, dy, dz, t->f_68, t->f_c8);
                    ok = (angle != 0x8000);
                } else if (t->f_111.b0) {
                    ok = CalcAimAngles(unit, t, &heading, &angle, (unsigned char)(e->flags >> 2 & 3),
                                      &pos);
                } else {
                    ok = 0;
                }
                if (ok) {
                    e->f_18 = angle;
                    e->f_16 = heading;
                    e->f_8 = 0;
                    unit->script->StartScriptWithArgs(DAT_00509688[(e->flags >> 2) & 3], &e->name, 0, 2,
                                               heading, angle, 0, 0);
                    SendScriptCallByName(unit, DAT_00509688[(e->flags >> 2) & 3], 2, heading, angle, 0, 0);
                    e->flags |= 1;
                }
            }
        } else {
            bool armed = attached->f_111.b4;
            if (armed && (!attached->f_111.b28 || e->f_1a) && !(e->flags & 1)) {
                e->f_8 = 0;
                unit->script->StartScriptWithArgs(DAT_00509688[(e->flags >> 2) & 3], &e->name, 0, 2, 0, 0,
                                           0, 0);
                SendScriptCallByName(unit, DAT_00509688[(e->flags >> 2) & 3], 2, 0, 0, 0, 0);
                e->flags |= 1;
            }
        }
        if (e->f_14 != 0)
            continue;
        if (WeaponCanReachPos(unit, &unit->pos, &pos, i)) {
            int can = 0;
            if (attached->f_111.b28) {
                if (e->f_1a)
                    can = 1;
            } else {
                if (unit->f_bc.store->metal >= attached->f_c0 &&
                    unit->f_bc.store->energy >= attached->f_c4)
                    can = 1;
            }
            if (can == 0)
                continue;
            Unit* fired = GetWeaponTargetUnit(unit, i);
            if (attached->f60(unit, &e->point, fired, &pos) == 0)
                continue;
            if (attached->f_111.b28) {
                e->f_1a--;
                FUN_0041c150(unit);
            } else {
                int n = unit->f_b8 / 5;
                if (n > 5)
                    n = 5;
                int q = unit->f_108 * 20 / unit->type->f_1fa;
                int pct = 100 - n * 6;
                e->f_14 = (short)((120 - q) * (pct * attached->f_e4 / 100) / 100);
            }
            int m = (attached->f_111.b26) ? 0x800 : 0x400;
            unit->f_ba.w |= m;
            if (!attached->f_111.b28)
                unit->f_bc.SpendEnergyAndMetal(attached->f_c0, attached->f_c4);
        } else {
            unit->f_ba.b[1] |= 0x10;
        }
    }
}
