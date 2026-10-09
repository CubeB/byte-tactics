// Decompiled by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by GPT-6, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by claude-opus-5-5. Names are provisional.
//
// The projectile update loop. One if/else-if chain over the type's flag
// bits (b20 guided, b0 straight, b1 timed drift, b8 drift, b5 tumbling), with
// a burst-fire branch for projectiles whose counter is not 0.
// Remove() is the inlined "deselect and mark dead" block that 0x49b090 also
// writes out.

// Needed: without <stdio.h> the b0 arm loads g_game first and gets a 5-byte mov.
#include <stdio.h>

#pragma pack(push, 1)

struct Vec3_0049b720 {
    int x;
    union {
        int y;
        struct {
            unsigned short lo;
            short hi;
        } yw;
    };
    int z;
    Vec3_0049b720& operator+=(const Vec3_0049b720& o)
    {
        x += o.x;
        y += o.y;
        z += o.z;
        return *this;
    }
};

union TypeFlags_0049b720 {
    unsigned int raw;
    struct {
        unsigned int b0 : 1, b1 : 1, b2 : 1, b3 : 1, b4 : 1, b5 : 1, b6 : 1, b7 : 1;
        unsigned int b8 : 1, b9 : 1, b10 : 1, b11 : 1, b12 : 1, b13 : 1, b14 : 1, b15 : 1;
        unsigned int b16 : 1, b17 : 1, b18 : 1, b19 : 1, b20 : 1, b21 : 1, b22 : 1, b23 : 1;
        unsigned int b24 : 1, b25 : 1, b26 : 1, b27 : 1, b28 : 1, b29 : 1, b30 : 1, b31 : 1;
    } b;
};

struct WeaponDef {
    char unknown_0[0x68];
    unsigned int maxSpeed;             // +0x68
    char unknown_6c[4];
    unsigned int accel;                // +0x70
    char unknown_74[0x7c - 0x74];
    void* splash;                      // +0x7c
    char unknown_80[0xe6 - 0x80];
    unsigned short lifetime;           // +0xe6
    char unknown_e8[0xec - 0xe8];
    unsigned short burstRate;          // +0xec
    unsigned short spread;             // +0xee
    unsigned short f0;                 // +0xf0
    unsigned short lifeRand;           // +0xf2
    unsigned short sound;              // +0xf4
    char unknown_f6[0xfa - 0xf6];
    unsigned short smokeRate;          // +0xfa
    unsigned short fc;                 // +0xfc
    unsigned short deathSound;         // +0xfe
    char unknown_100[0x111 - 0x100];
    TypeFlags_0049b720 flags;          // +0x111
};

struct Weapon_0049b720 {
    char unknown_0[0x10];
    WeaponDef* weapon;                 // +0x10
    char unknown_14[0x1c - 0x14];
};

struct Unit {
    Weapon_0049b720 weapons[3];
};

union Word_0049b720 {
    int i;
    struct {
        unsigned short lo;
        short hi;
    } s;
};

struct ProjFlags_0049b720 {
    unsigned short b0 : 1;
    unsigned short dead : 1;
    unsigned short b2 : 2;
    unsigned short state : 2;
    unsigned short rest : 10;
};

struct Proj_0049b720 {
    WeaponDef* type;                   // +0x0
    Vec3_0049b720 pos;                 // +0x4
    Vec3_0049b720 start;               // +0x10
    Vec3_0049b720 vel;                 // +0x1c
    char unknown_28[0x34 - 0x28];
    short roll;                        // +0x34
    short heading;                     // +0x36
    short pitch;                       // +0x38
    unsigned int speed;                // +0x3a
    unsigned int range;                // +0x3e
    unsigned int f42;                  // +0x42
    unsigned int f46;                  // +0x46
    unsigned int f4a;                  // +0x4a
    int f4e;                           // +0x4e
    Unit* unit;                        // +0x52
    void* f56;                         // +0x56
    char unknown_5a[0x60 - 0x5a];
    unsigned short counter;            // +0x60
    unsigned short piece;              // +0x62
    short f64;                         // +0x64
    char unknown_66[0x69 - 0x66];
    ProjFlags_0049b720 flags;          // +0x69
};

struct Net_0049b720 {
    char unknown_0[0xd48];
    int noSeaLevelTrigger;
};

struct Game {
    char unknown_0[0x141f3];
    int projCount;                     // +0x141f3
    Proj_0049b720* projs;              // +0x141f7
    char unknown_141fb[0x14263 - 0x141fb];
    int gravity;                       // +0x14263
    char unknown_14267[0x1427f - 0x14267];
    unsigned char seaLevel;            // +0x1427f
    char unknown_14280[0x142f7 - 0x14280];
    Proj_0049b720* selected;           // +0x142f7
    char unknown_142fb[0x1433f - 0x142fb];
    Vec3_0049b720 lastPos;             // +0x1433f
    unsigned short lastSound;          // +0x1434b
    char unknown_1434d[0x37ecc - 0x1434d];
    Vec3_0049b720 wind;                // +0x37ecc
    char unknown_37ed8[0x38a47 - 0x37ed8];
    unsigned int time;                 // +0x38a47
    char unknown_38a4b[0x391e9 - 0x38a4b];
    Net_0049b720* net;                 // +0x391e9
};

struct Cell {
    char unknown_0[5];
    unsigned char height;              // +0x5
};

#pragma pack(pop)

extern Game* g_game;

void __stdcall CompactProjectiles();
void __stdcall CheckProjectileCollision(WeaponDef* type, Proj_0049b720* p);
Vec3_0049b720* __stdcall GetProjectileAimPoint(Proj_0049b720* p);
int __stdcall TurnUnitTowardsPoint(Proj_0049b720* p, Vec3_0049b720* target);
void __stdcall DetonateProjectile(Proj_0049b720* p, void* unit);
void __stdcall GetWeaponPiecePosition(Unit* unit, Vec3_0049b720* out, unsigned char weapon, int piece);
int __stdcall PlaySoundAt(int sound, Vec3_0049b720* pos, int flag);
void __stdcall EmitWhiteSmoke(Vec3_0049b720* pos, short kind);
Cell* __stdcall GetMapCellAtPosition(Vec3_0049b720* pos);
void __stdcall AddExplosionEffect(Vec3_0049b720* pos, void* src, int index, int flag);
int __stdcall RandomInt(int range);
int __cdecl FUN_004b70ef(short angle, int scale);
int __cdecl FUN_004b7123(short angle, int scale);

// A copy of AllocProjectile (matched in its own file), the function just before
// this one in the original source; defined here so that /Ob2 inlines it.
Proj_0049b720* AllocProjectile()
{
    Proj_0049b720* p = 0;
    if (g_game->projCount < 300) {
        p = &g_game->projs[g_game->projCount++];
        p->flags.dead = 0;
        p->f4e = 0;
    }
    return p;
}

// Gravity goes through this pointer store, not `p->vel.y -= ...`: it makes
// p->type get reloaded, so `type` is not propagated.
static inline void Fall(Vec3_0049b720* v)
{
    v->y -= g_game->gravity;
}

static inline void Remove(Proj_0049b720* p)
{
    if (p == g_game->selected) {
        g_game->lastPos = g_game->selected->pos;
        g_game->lastSound = p->type->deathSound;
        g_game->selected = 0;
    }
    p->flags.dead = 1;
}

// Stays in its own file: the b0 arm's register tie follows this file's symbol
// ids, which the joined weapons.cpp moves.
// FUNCTION: 0x49b720
void UpdateProjectiles()
{
    int n = g_game->projCount;
    for (int i = 0; i < n; i++) {
        Proj_0049b720* p = &g_game->projs[i];
        // type is read before oldY: orders the two spill stores.
        WeaponDef* type = p->type;
        int oldY = p->pos.yw.hi;

        if (p->counter != 0) {
            if (g_game->time >= p->f42 + type->burstRate) {
                if (type->burstRate >= 5 || (p->counter & 1)) {
                    unsigned char w;
                    for (w = 0; w < 3; w++) {
                        if (p->unit->weapons[w].weapon == type)
                            break;
                    }
                    GetWeaponPiecePosition(p->unit, &p->pos, w, p->piece);
                }
                p->counter--;
                p->f42 += type->burstRate;
                Proj_0049b720* q = AllocProjectile();
                if (q) {
                    *q = *p;
                    q->f42 = g_game->time;
                    if (type->flags.b.b11)
                        PlaySoundAt(type->sound, &p->pos, 0);
                    if (type->lifetime != 0)
                        q->f46 = g_game->time + type->lifetime;
                    else
                        q->f46 = (p->range + 0x100000) / p->speed + g_game->time;
                    if (type->lifeRand != 0)
                        q->f46 += RandomInt(type->lifeRand) - (type->lifeRand >> 1);
                    q->counter = 0;
                    if (type->spread != 0) {
                        short ang = RandomInt(type->spread) + (short)(p->heading - (type->spread >> 1));
                        int t = FUN_004b7123(p->pitch, type->maxSpeed);
                        p->vel.x = -FUN_004b70ef(ang, t);
                        p->vel.z = -FUN_004b7123(ang, t);
                    }
                }
                if (p->counter == 0)
                    Remove(p);
            }
            continue;
        }

        if (type->flags.b.b21)
            p->f64 += 0x400;

        if (type->flags.b.b20) {
            if (p->f46 > g_game->time) {
                if ((type->flags.raw & 0x10000) && p->pos.yw.hi >= g_game->seaLevel) {
                    Fall(&p->vel);
                    p->pitch = 0;
                } else {
                    int seek = 0;
                    if (p->speed < type->maxSpeed) {
                        p->speed += type->accel;
                        if (p->speed > type->maxSpeed)
                            p->speed = type->maxSpeed;
                    }
                    if (type->flags.b.b24) {
                        if (p->flags.state > 0)
                            seek = 1;
                    } else if (type->flags.b.b12) {
                        seek = 1;
                    }
                    if (seek) {
                        Vec3_0049b720* target = GetProjectileAimPoint(p);
                        if (TurnUnitTowardsPoint(p, target) == 0)
                            DetonateProjectile(p, 0);
                    }
                    p->vel.y = FUN_004b70ef(p->pitch, p->speed);
                    int t = FUN_004b7123(p->pitch, p->speed);
                    p->vel.x = -FUN_004b70ef(p->heading, t);
                    p->vel.z = -FUN_004b7123(p->heading, t);
                }
            } else if (type->flags.b.b23) {
                DetonateProjectile(p, 0);
            } else {
                Fall(&p->vel);
                if (type->flags.b.b24) {
                    if (p->flags.state == 0) {
                        p->f46 = g_game->time + p->type->fc;
                        p->flags.state++;
                        if (!(p->type->flags.raw & 0x2000)) {
                            p->f56 = 0;
                            p->f4e = 0;
                        }
                    }
                }
            }
            p->pos += p->vel;
            CheckProjectileCollision(type, p);
        } else if (type->flags.b.b0) {
            if (p->f46 > g_game->time) {
                p->pos += p->vel;
                if (type->flags.b.b3) {
                    if (p->flags.b0)
                        p->start += p->vel;
                    else if (g_game->time > p->f42 + type->f0)
                        p->flags.b0 = 1;
                }
                CheckProjectileCollision(type, p);
            } else {
                Remove(p);
            }
        } else if (type->flags.b.b1) {
            if (type->lifetime != 0) {
                if (p->f46 > g_game->time) {
                    p->pos += p->vel;
                    p->pos += g_game->wind;
                    p->vel.y -= g_game->gravity;
                    CheckProjectileCollision(type, p);
                } else if (type->flags.b.b23) {
                    DetonateProjectile(p, 0);
                } else {
                    EmitWhiteSmoke(&p->pos, 9);
                    Remove(p);
                }
            } else {
                p->pos += p->vel;
                p->pos += g_game->wind;
                p->vel.y -= g_game->gravity;
                CheckProjectileCollision(type, p);
            }
        } else if (type->flags.b.b8) {
            p->pos += p->vel;
            p->pos += g_game->wind;
            p->vel.y -= g_game->gravity;
            CheckProjectileCollision(type, p);
        } else if (type->flags.b.b5) {
            p->pos += p->vel;
            p->roll += ((short*)&p->vel.x)[1] << 8;
            p->pitch += ((short*)&p->vel.z)[1] << 8;
            CheckProjectileCollision(type, p);
        }

        if (!p->flags.dead) {
            if ((type->flags.raw & 0x40000) && p->f46 > g_game->time && p->f4a < g_game->time) {
                EmitWhiteSmoke(&p->pos, 9);
                p->f4a += type->smokeRate;
            }
            if (oldY > g_game->seaLevel && p->pos.yw.hi <= g_game->seaLevel) {
                Cell* cell = GetMapCellAtPosition(&p->pos);
                if (cell && cell->height < g_game->seaLevel && g_game->net->noSeaLevelTrigger == 0)
                    AddExplosionEffect(&p->pos, type->splash, 0, 1);
            }
        }
    }
    CompactProjectiles();
}
