// Decompiled by Space Bunny Free, finished by deepseek-v4.1-flash. Names are provisional.
#include <math.h>

class Class_004b07c0 {
public:
    int FUN_004b07c0(char* name);
};

class Class_004b0a70 {
public:
    int FUN_004b0a70(char* name, void* param_2, int param_3, int param_4,
                     int param_5, int param_6, int param_7, int param_8);
};

struct Vec3_0048a1e0 {
    int x;
    int y;
    int z;
};

struct Muzzle_0048a1e0 {
    char unknown_0[8];
    Vec3_0048a1e0 offset;              // +0x8
};

#pragma pack(push, 1)
struct UnitDef_0048a1e0;

struct Entry_0048a1e0 {
    short x;                           // +0x0, a unit id when z is 0x8000
    short z;                           // +0x2, the marker 0x8000
    char unknown_4[0xc - 4];
    UnitDef_0048a1e0* target;          // +0xc
    char unknown_10[0x1b - 0x10];
    unsigned char flags;               // +0x1b
};

// A unit record as g_game->units holds it (stride 0x118).
struct UnitDef_0048a1e0 {
    Muzzle_0048a1e0* muzzle;           // +0x0
    char unknown_4[0x68 - 0x4];
    int radius;                        // +0x68
    char unknown_6c[0xa6 - 0x6c];
    unsigned short field_a6;           // +0xa6
    char unknown_a8[0x111 - 0xa8];
    unsigned int field_111;            // +0x111
    char unknown_115[0x118 - 0x115];
};

struct Unit {
    char unknown_0[4];
    Entry_0048a1e0 entries[3];         // +0x4
    char unknown_58[0x6a - 0x58];
    Vec3_0048a1e0 pos;                 // +0x6a
    char unknown_76[0x9a - 0x76];
    Class_004b07c0* script;            // +0x9a
    char unknown_9e[0xb8 - 0x9e];
    unsigned short field_b8;           // +0xb8
};

struct Game_0048a1e0 {
    char unknown_0[0x1427f];
    unsigned char seaLevel;            // +0x1427f
    char unknown_14280[0x14357 - 0x14280];
    UnitDef_0048a1e0* units;           // +0x14357
};
#pragma pack(pop)

extern Game_0048a1e0* g_game;

int __stdcall FUN_00485070(Vec3_0048a1e0* pos);
void __stdcall FUN_0043e3c0(UnitDef_0048a1e0* def, int pos);

// The intact tail block was the last diff. Writing the three adds in their
// natural x,y,z order made MSVC sink the first product to its use (products
// came out y,z,x, 91.5%); writing them z,y,x kept the products but ordered the
// adds z,y,x (96.2%). Passing the three products through a static helper that
// returns the vector by value makes all three writes land in memory before the
// adds are read, and the original's x,y,z schedule falls out.
static Vec3_0048a1e0 offset_0048a1e0(Muzzle_0048a1e0* m, __int64 s)
{
    Vec3_0048a1e0 d;
    d.x = (int)(((__int64)m->offset.x * s) >> 16);
    d.y = (int)(((__int64)m->offset.y * s) >> 16);
    d.z = (int)(((__int64)m->offset.z * s) >> 16);
    return d;
}

#define max(a, b) (((a) > (b)) ? (a) : (b))

// FUNCTION: 0x48a1e0
int __stdcall FUN_0048a1e0(Unit* unit, Vec3_0048a1e0* pos, int index)
{
    Entry_0048a1e0* e = &unit->entries[index];
    if (e->z != (short)0x8000) {
        pos->x = e->x << 16;
        pos->z = e->z << 16;
        pos->y = max(FUN_00485070(pos), g_game->seaLevel) << 16;
        return 1;
    }
    if (e->x == 0) {
        return 0;
    }
    UnitDef_0048a1e0* def = &g_game->units[e->x];
    if (def->field_a6 == 0) {
        Entry_0048a1e0* f = &unit->entries[index];
        if (f->x != 0 || f->z != (short)0x8000) {
            e->x = 0;
            e->z = (short)0x8000;
            unit->script->FUN_004b07c0("StartBuilding");
            ((Class_004b0a70*)unit->script)->FUN_004b0a70("TargetCleared", 0, 0, 1, index, 0, 0, 0);
        }
        return 0;
    }
    FUN_0043e3c0(def, (int)pos);
    if ((e->flags & 2) && !(e->target->field_111 & 0x2000000) && def->muzzle != 0
        && unit->field_b8 > 5 && e->target->radius != 0) {
        Vec3_0048a1e0 d;
        d.x = unit->pos.x - pos->x;
        d.y = unit->pos.y - pos->y;
        d.z = unit->pos.z - pos->z;
        int dist = (int)sqrt((double)d.x * d.x + (double)d.y * d.y + (double)d.z * d.z);
        int scale = (int)(((__int64)dist << 16) / e->target->radius);
        scale = (int)((scale * (__int64)0xcccc) >> 16);
        __int64 s = scale;
        d = offset_0048a1e0(def->muzzle, s);
        pos->x = pos->x + d.x;
        pos->y = pos->y + d.y;
        pos->z = pos->z + d.z;
    }
    return 1;
}
