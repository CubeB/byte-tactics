// Decompiled by Space Bunny Free, finished by Claude Sonnet 5.5. Names are provisional.
//
// Firing a projectile: take the next free slot of the 300-entry projectile
// array, initialise it through FUN_0049c740, aim it (the two 16.16 fixed point
// angles and the horizontal distance), take the shot's launch angle, derive
// two offsets with the fixed point sine/cosine helpers, schedule the impact
// time with the 0x49c920 helper, then play the firing and "RockUnit" sounds
// and animations. Returns 0 when the array is full.
//
// MATCH (600 of 600 bytes). The whole gap of the earlier 74.6% version was one
// argument: the last FUN_004b7123 call takes `t` (the result of the FUN_004b7123
// call before it, which has a stack home in the dead `fire` argument slot), not
// the position pointer p3. The earlier notes read the `mov ecx, [esp+0x1c]` at
// 0x49caff as dz, and then wrote the call as `FUN_004b7123(a1, (int)p3)` with a
// "suspected original bug" comment. Counting the stores: 0x49ca59 puts dz into
// E0+0x14, 0x49caad overwrites it with a2, and 0x49caf4 overwrites it again with
// t (the result of the FUN_004b7123 call), so the load at 0x49caff, with two
// pushes outstanding, reads t. The extra use of p3 in that call is what gave p3
// the fourth callee-saved register instead of `unit`; without it the allocation is
// the original's (esi = proj, edi = fire, ebx = p4, ebp = unit) and p3 is
// rematerialised from its argument slot. Compiler state is not involved: the
// declaration-count sweep (0 to 400 in steps of 8) is flat at 74.6% for the old
// source.
//
// Two details are needed to get the numbers right:
//   - `dy` has to be a 4-byte union: the original reads only its high half,
//     as `movsx ecx, word ptr [esp+0x22]`, so the local needs a real frame
//     home and the read has to be a 16-bit load at +2. A plain int gives
//     `sar edi, 0x10` in a register instead (same trick as 0x49b520).
//   - the second FUN_004b715a argument is `(short)(dist >> 16)` with a
//     *logical* shift (`shr eax, 0x10`), so `dist` is unsigned.
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

struct Unit;

struct Proj_0049c9c0 {
    Unit* unit;                        // +0x00
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
struct Unit {
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
struct Game {
    char unknown_0[0x141f3];
    int projectileCount;               // +0x141f3
    Proj_0049c9c0* projectiles;        // +0x141f7
    char unknown_141fb[0x38a47 - 0x141fb];
    int frame;                         // +0x38a47
};
#pragma pack(pop)

extern Game* g_game;
extern char* DAT_00509678[];

int __cdecl FUN_004b715a(int x, int z);
int __cdecl FUN_004b70ef(short angle, int scale);
int __cdecl FUN_004b7123(unsigned short angle, int scale);

void __stdcall FUN_0049c740(Proj_0049c9c0* proj, Shot_0049c9c0* shot,
                           Vec3_0049c9c0* pos, Vec3_0049c9c0* aim, int frame,
                           Unit* unit);
void __stdcall FUN_004729d0(Vec3_0049c9c0* p, short index);

// FUNCTION: 0x49c9c0
int __stdcall FUN_0049c9c0(Fire_0049c9c0* fire, Unit* unit,
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
    proj->f_24 = -FUN_004b7123(a1, t);

    // The same helper as 0x49c920, inlined here.
    Unit* u = proj->unit;
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
