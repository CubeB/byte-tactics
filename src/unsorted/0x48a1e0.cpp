// Decompiled by Space Bunny Free. Names are provisional.
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

struct Unit_0048a1e0 {
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

#define max(a, b) (((a) > (b)) ? (a) : (b))

// A final pass attacked the add block structurally rather than by permutation,
// as the previous pass recommended, and did not beat 96.2%. The clue followed
// was that the original's `d.x` and `d.y` occupy frame slots while `d.z` stays
// in eax, a pattern MSVC produces for three independent component adds, so the
// idea was to stop the scheduler merging the three adds. Seven shapes, all with
// `check.py --sym` after `rm -rf build/obj`:
//
//   adds in x,y,z order                                91.5%
//   a fresh Vec3 built up and written back whole       90.1%
//   a static inline AddVec taking two vectors by value 90.1%
//   a static inline Add1 per component, x,y,z          91.5%
//   a static inline Add1 per component, z,y,x          96.2%  (same as the file)
//   pos->x += d.x three times, x,y,z                  91.5%
//   pos->x += d.x three times, z,y,x                  96.2%  (same as the file)
//
// The pattern across all seven is that anything which perturbs the *statement*
// shape of the three adds drops about five points, and the two shapes that hold
// 96.2% are exactly the two orderings already in the file. Together with the
// previous pass's 36 orderings of the three `_allmul` products against the 36
// orderings of the three adds, in two shapes each, the permutation space is
// measured and the add scheduling does not come out of it. A `volatile` on the
// product locals would block the reordering but is not permitted.
//
// FUNCTION: 0x48a1e0
int __stdcall FUN_0048a1e0(Unit_0048a1e0* unit, Vec3_0048a1e0* pos, int index)
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
        d.x = (int)(((__int64)def->muzzle->offset.x * s) >> 16);
        d.y = (int)(((__int64)def->muzzle->offset.y * s) >> 16);
        d.z = (int)(((__int64)def->muzzle->offset.z * s) >> 16);
        pos->z = pos->z + d.z;
        pos->y = pos->y + d.y;
        pos->x = pos->x + d.x;
    }
    return 1;
}
