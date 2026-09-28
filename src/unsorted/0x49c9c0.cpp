// Decompiled by Space Bunny Free. Names are provisional.
// Partial (74.6%). Every instruction, offset, call sequence, branch and stack
// slot of the original is reproduced; what still differs is one register
// allocation choice, which then permutes a dozen otherwise identical
// instructions (see "still differs" below).
//
// Firing a projectile: take the next free slot of the 300-entry projectile
// array, initialise it through FUN_0049c740, aim it (the two 16.16 fixed point
// angles and the horizontal distance), take the shot's launch angle, derive
// two offsets with the fixed point sine/cosine helpers, schedule the impact
// time with the 0x49c920 helper, then play the firing and "RockUnit" sounds
// and animations. Returns 0 when the array is full.
//
// A second pass added these and none of them moved the allocation: caching
// p3's three fields in int locals so the pointer is dereferenced once (74.6%),
// caching `(fire->f_1b >> 2) & 3` in a local so the unit is dereferenced fewer
// times in the tail block (65.6%), and both together (65.6%). The first is
// neutral and the second actively hurts, which is consistent with the wanted
// allocation being one where `unit` holds the third callee-saved register and
// `p3` is rematerialised from its argument slot, i.e. the opposite of what
// caching the unit's fields pushes towards.
//
// Two details are needed to get the numbers right:
//   - `dy` has to be a 4-byte union: the original reads only its high half,
//     as `movsx ecx, word ptr [esp+0x22]`, so the local needs a real frame
//     home and the read has to be a 16-bit load at +2. A plain int gives
//     `sar edi, 0x10` in a register instead (same trick as 0x49b520).
//   - the second FUN_004b715a argument is `(short)(dist >> 16)` with a
//     *logical* shift (`shr eax, 0x10`), so `dist` is unsigned.
//
// Still differs: MSVC 5 gives the third callee-saved register to p3 and leaves
// the unit pointer in a scratch register, while the original gives it to the
// unit pointer and keeps p3 in its incoming argument slot (re-read three
// times, at 0x49ca39, 0x49caff and 0x49cc00). The register pool only has
// room for three of the four pointer parameters, so exactly one of them is
// left in memory, and this build picks the other one. That flips the register
// in the argument loads at the top of the function, the two slots dx/dy take
// from the dead argument slots (dx+0x18/dy+0x20 in the original, dx+0x20/
// dy+0x1c here) and every later use of those registers. Tried and rejected:
// all six declaration orders of dx/dy/dz, references and void* for every
// parameter, const, a local copy of p3, static inline helpers for the
// difference block, for the allocation block and for the two inlined helpers,
// three-element structs and arrays for the differences, moving the f_3e store,
// and unsigned/int variants of dist: all compile to the same allocation.
#include <math.h>

struct Vec3_0049c9c0 {
    int x;
    int y;
    int z;
};

class Class_004b0940 {
public:
    int FUN_004b0940(char* name, int param_2, int param_3);
};

class Class_004b0a70 {
public:
    int FUN_004b0a70(char* name, void* param_2, int param_3, int param_4, int param_5,
                    int param_6, int param_7, int param_8);
};

#pragma pack(push, 1)
struct Shot_0049c9c0 {
    char unknown_0[0x68];
    unsigned int f_68;                // +0x68
    int f_6c;                          // +0x6c
    int f_70;                          // +0x70
    char unknown_74[0xea - 0x74];
    unsigned short f_ea;               // +0xea
    char unknown_ec[0x111 - 0xec];
    unsigned int f_lower : 9;
    unsigned int f_bit9 : 1;           // bit 9 of +0x111
};

struct Unit_0049c9c0;

struct Proj_0049c9c0 {
    Unit_0049c9c0* unit;               // +0x00
    char unknown_4[0x1c - 4];
    int f_1c;                          // +0x1c
    int f_20;                          // +0x20
    int f_24;                          // +0x24
    char unknown_28[0x36 - 0x28];
    short f_36;                        // +0x36
    short f_38;                        // +0x38
    int f_3a;                          // +0x3a
    int f_3e;                          // +0x3e
    int f_42;                          // +0x42
    int f_46;                          // +0x46
    char unknown_4a[0x4e - 0x4a];
    int f_4e;                          // +0x4e
    char unknown_52[0x60 - 0x52];
    short f_60;                        // +0x60
    char unknown_62[0x69 - 0x62];
    unsigned short flags;              // +0x69
};
#pragma pack(pop)

struct Gun_0049c9c0 {
    short angle;
    char unknown_2[2];
};

#pragma pack(push, 1)
struct Unit_0049c9c0 {
    char unknown_0[0x1a];
    Gun_0049c9c0 f_1a[19];             // +0x1a, indexed as f_1a[i * 7]
    short heading;                     // +0x66
    unsigned int f_68;                // +0x68
    char unknown_6c[0x9a - 0x6c];
    Class_004b0940* anims;             // +0x9a
    char unknown_9e[0xdc - 0x9e];
    int f_dc;                          // +0xdc
    char unknown_e0[0xe6 - 0xe0];
    unsigned short f_e6;               // +0xe6
    char unknown_e8[0x111 - 0xe8];
    unsigned int flags;                // +0x111
};
#pragma pack(pop)

struct Fire_0049c9c0 {
    char unknown_0[0xc];
    Shot_0049c9c0* shot;               // +0xc
    char unknown_10[0x1b - 0x10];
    unsigned char f_1b;                // +0x1b
};

#pragma pack(push, 1)
struct Game_0049c9c0 {
    char unknown_0[0x141f3];
    int projectileCount;               // +0x141f3
    Proj_0049c9c0* projectiles;        // +0x141f7
    char unknown_141fb[0x38a47 - 0x141fb];
    int frame;                         // +0x38a47
};
#pragma pack(pop)

extern Game_0049c9c0* g_game;
extern char* DAT_00509678[];

int __cdecl FUN_004b715a(int x, int z);
int __cdecl FUN_004b70ef(short angle, int scale);
int __cdecl FUN_004b7123(unsigned short angle, int scale);

void __stdcall FUN_0049c740(Proj_0049c9c0* proj, Shot_0049c9c0* shot,
                           Vec3_0049c9c0* pos, Vec3_0049c9c0* aim, int frame,
                           Unit_0049c9c0* unit);
void __stdcall FUN_004729d0(Vec3_0049c9c0* p, short index);

// FUNCTION: 0x49c9c0
int __stdcall FUN_0049c9c0(Fire_0049c9c0* fire, Unit_0049c9c0* unit,
                           Vec3_0049c9c0* p3, Vec3_0049c9c0* p4, int param_5)
{
    Proj_0049c9c0* proj = 0;
    int i = g_game->projectileCount;
    if (i < 0x12c) {
        proj = g_game->projectiles + i;
        g_game->projectileCount = i + 1;
        proj->flags &= ~2;
        proj->f_4e = 0;
    }
    if (!proj)
        return 0;

    FUN_0049c740(proj, fire->shot, p3, p4, g_game->frame, unit);

    int dx = p3->x - p4->x;
    union { int value; short halves[2]; } dy;
    dy.value = p3->y - p4->y;
    int dz = p3->z - p4->z;
    short a1 = FUN_004b715a(dx, dz);
    unsigned int dist = (int)_hypot(dx, dz);
    proj->f_3e = (int)dist;
    short a2 = FUN_004b715a(-(int)dy.halves[1], (short)(dist >> 16));
    proj->f_36 = a1;
    proj->f_38 = a2;

    // The same helper as 0x49c980, inlined here.
    if (fire->shot->f_6c) {
        proj->f_3a = fire->shot->f_6c;
    } else if (!fire->shot->f_70) {
        proj->f_3a = fire->shot->f_68;
    } else {
        proj->f_3a = 0;
    }

    proj->f_20 = FUN_004b70ef(a2, proj->f_3a);
    int t = FUN_004b7123(a2, proj->f_3a);
    proj->f_1c = -FUN_004b70ef(a1, t);
    // Suspected original bug: the scale argument of FUN_004b7123 (which
    // multiplies [ebp+0xc], see 0x4b7123) is the position pointer itself
    // rather than a length; the two neighbours of this call take the launch
    // angle field and the value just computed. Kept as the original has it.
    proj->f_24 = -FUN_004b7123(a1, (int)p3);

    // The same helper as 0x49c920, inlined here.
    Unit_0049c9c0* u = proj->unit;
    if (u->f_68 != 0 && !(u->flags & 0x8000000))
        proj->f_46 = (u->f_dc << 16) / u->f_68 + g_game->frame;
    else
        proj->f_46 = g_game->frame + u->f_e6;
    proj->f_4e = param_5;

    proj->f_60 = fire->shot->f_ea;
    unit->anims->FUN_004b0940(DAT_00509678[(fire->f_1b >> 2) & 3], 0, 0);
    short angle = unit->f_1a[((fire->f_1b >> 2) & 3) * 7].angle - unit->heading;
    int a = -FUN_004b70ef(angle, 800);
    int b = -FUN_004b7123(angle, 800);
    ((Class_004b0a70*)unit->anims)->FUN_004b0a70("RockUnit", 0, 0, 2, b, a, 0, 0);

    if (fire->shot->f_bit9)
        FUN_004729d0(p3, 9);
    return 1;
}
