// Decompiled by Claude Opus 5.5. Names are provisional.
// Kills the feature on map cell (x, z) (a footprint cell is first moved to
// the feature's origin cell): flag 0 when it was destroyed, 1 when it was
// reclaimed. A feature with an animation for that case (flags bit 0) takes a
// spot from the pool (the inlined FUN_004232a0) that plays it; any other is
// handed to FUN_00423710, which replaces it straight away.
// Needs <windows.h>: 78.1% without it. The if/else around the spot code
// (not an early return for a null animation) puts the FUN_00423710 call last.
#include <windows.h>

#pragma pack(push, 1)
struct AnimSrc_00423550;

struct AnimRef_00423550 {
    unsigned short index;              // +0x0
    unsigned short value;              // +0x2
    unsigned char kind;                // +0x4
    char unknown_5[3];
    AnimSrc_00423550* src;             // +0x8
};

struct AnimPair_00423550 {
    AnimSrc_00423550* anim;
    AnimSrc_00423550* shadow;
};

struct Feature_00423550 {
    char unknown_0[0xb4];
    AnimPair_00423550 burn;            // +0xb4
    AnimPair_00423550 death[2];        // +0xbc: destroyed, reclaimed
    char unknown_cc[0xfe - 0xcc];
    unsigned char flags;               // +0xfe
    char unknown_ff[0x100 - 0xff];
};

struct Spot_00423550 {
    short prev;                        // +0x0
    short next;                        // +0x2
    AnimRef_00423550 anim;             // +0x4
    AnimRef_00423550 shadow;           // +0x10
    char unknown_1c[0x28 - 0x1c];
    short x;                           // +0x28
    short z;                           // +0x2a
    unsigned short feature;            // +0x2c
    unsigned char burnTime;            // +0x2e
    unsigned char used : 1;            // +0x2f bit 0
    unsigned char reclaimed : 1;
    unsigned char hasShadow : 1;
    unsigned char noSend : 1;
    unsigned char bit4 : 1;
    unsigned char bits5 : 3;
};

struct Cell_00423550 {
    char unknown_0[8];
    unsigned short feature;            // +0x8
    union {
        struct {
            unsigned char offsetY;     // +0xa
            unsigned char offsetX;     // +0xb
        };
        unsigned short spot;           // +0xa
    };
    unsigned char flags;               // +0xc
};
#pragma pack(pop)

struct Pool_00423550 {
    Spot_00423550* entries;            // +0x0
    char unknown_4[4];
    int usedHead;                      // +0x8
    char unknown_c[4];
    int freeHead;                      // +0x10
};

#pragma pack(push, 1)
struct Game_00423550 {
    char unknown_0[0x1420b];
    Pool_00423550 pool;                // +0x1420b
    char unknown_1421f[0x1426f - 0x1421f];
    Feature_00423550* features;        // +0x1426f
};
#pragma pack(pop)

extern Game_00423550* g_game;

Cell_00423550* __stdcall FUN_00481550(int x, int y);
void __stdcall FUN_004232f0(int index, int* head);
void __stdcall FUN_004b8b30(AnimRef_00423550* ref, AnimSrc_00423550* src, int index);
void __stdcall FUN_00423710(int x, int y, int flag);

static inline int AllocSpot()
{
    Pool_00423550* p = &g_game->pool;
    int i = p->freeHead;
    if (i == -1) {
        return 0x800;
    }
    FUN_004232f0(i, &p->usedHead);
    p->entries[i].used = 0;
    return i;
}

// FUNCTION: 0x423550
void __stdcall FUN_00423550(int x, int z, int flag)
{
    Cell_00423550* cell = FUN_00481550(x, z);
    if (cell->feature == 0xfffe) {
        x -= cell->offsetX;
        z -= cell->offsetY;
        cell = FUN_00481550(x, z);
    }
    if (cell->feature >= 0xfffb)
        return;
    Feature_00423550* f = &g_game->features[cell->feature];
    AnimPair_00423550 pair;
    pair.anim = 0;
    pair.shadow = 0;
    if (f->flags & 1) {
        if (flag != 0)
            pair = f->death[1];
        else
            pair = f->death[0];
    }
    if (pair.anim != 0) {
        if (cell->flags & 1)
            return;
        int i = AllocSpot();
        if (i >= 0x800)
            return;
        Spot_00423550* s = &g_game->pool.entries[i];
        s->feature = cell->feature;
        s->bit4 = flag != 0;
        cell->spot = i;
        cell->flags |= 1;
        FUN_004b8b30(&s->anim, pair.anim, 0);
        if (pair.shadow != 0) {
            FUN_004b8b30(&s->shadow, pair.shadow, 0);
            s->hasShadow = 1;
        } else {
            s->hasShadow = 0;
        }
        s->used = 0;
        s->reclaimed = flag;
        s->x = x;
        s->z = z;
    } else {
        FUN_00423710(x, z, flag);
    }
}
