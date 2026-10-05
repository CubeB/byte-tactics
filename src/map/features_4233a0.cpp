// Decompiled by Claude Opus 5.5. Names are provisional.
// Sets the feature on map cell (x, z) burning: takes a spot from the pool
// (the inlined FUN_004232a0), starts the feature's burn animations on it,
// plays "treeburn" and, unless flag is set, sends the burn to the others.
// Needs <windows.h> (tools/headers.py): 72.5% without it.
#include <windows.h>

#pragma pack(push, 1)
struct AnimSrc_004233a0;

struct AnimRef_004233a0 {
    unsigned short index;              // +0x0
    unsigned short value;              // +0x2
    unsigned char kind;                // +0x4
    char unknown_5[3];
    AnimSrc_004233a0* src;             // +0x8
};

struct Feature_004233a0 {
    char unknown_0[0xb4];
    AnimSrc_004233a0* burn;            // +0xb4
    AnimSrc_004233a0* burnShadow;      // +0xb8
    char unknown_bc[0xe8 - 0xbc];
    unsigned short burnTime;           // +0xe8
    char unknown_ea[0x100 - 0xea];
};

struct Spot_004233a0 {
    short prev;                        // +0x0
    short next;                        // +0x2
    AnimRef_004233a0 anim;             // +0x4
    AnimRef_004233a0 shadow;           // +0x10
    char unknown_1c[0x28 - 0x1c];
    short x;                           // +0x28
    short z;                           // +0x2a
    unsigned short feature;            // +0x2c
    unsigned char burnTime;            // +0x2e
    unsigned char used : 1;            // +0x2f bit 0
    unsigned char bit1 : 1;
    unsigned char hasShadow : 1;
    unsigned char noSend : 1;
    unsigned char bits4 : 4;
};

struct Cell_004233a0 {
    char unknown_0[8];
    unsigned short feature;            // +0x8
    unsigned short spot;               // +0xa
    unsigned char flags;               // +0xc
};
#pragma pack(pop)

struct Pool_004233a0 {
    Spot_004233a0* entries;            // +0x0
    char unknown_4[4];
    int usedHead;                      // +0x8
    char unknown_c[4];
    int freeHead;                      // +0x10
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x1420b];
    Pool_004233a0 pool;                // +0x1420b
    char unknown_1421f[0x1426f - 0x1421f];
    Feature_004233a0* features;        // +0x1426f
};
#pragma pack(pop)

struct Vec3_004233a0 {
    int x, y, z;
};

struct Packet_004233a0 {
    unsigned char type;
    unsigned char sub;
    short x;
    short z;
};

extern Game* g_game;

Cell_004233a0* __stdcall FUN_00481550(int x, int y);
void __stdcall FUN_004232f0(int index, int* head);
void __stdcall FUN_004b8b30(AnimRef_004233a0* ref, AnimSrc_004233a0* src, int index);
int __stdcall FUN_004b6c30(int range);
void __stdcall FUN_0047f610(char* name, Vec3_004233a0* pos, int param_3);
int GetLocalDpid();
int __stdcall BroadcastPacket(int player, void* data, int size);

static inline int AllocSpot()
{
    Pool_004233a0* p = &g_game->pool;
    int i = p->freeHead;
    if (i == -1) {
        return 0x800;
    }
    FUN_004232f0(i, &p->usedHead);
    p->entries[i].used = 0;
    return i;
}

// FUNCTION: 0x4233a0
void __stdcall FUN_004233a0(int x, int z, int flag)
{
    Cell_004233a0* cell = FUN_00481550(x, z);
    if (cell == 0)
        return;
    if (cell->feature >= 0xfffb)
        return;
    Feature_004233a0* f = &g_game->features[cell->feature];
    if (f->burn == 0)
        return;
    if (cell->flags & 1)
        return;
    int i = AllocSpot();
    if (i >= 0x800)
        return;
    Spot_004233a0* s = &g_game->pool.entries[i];
    s->feature = cell->feature;
    cell->spot = i;
    cell->flags |= 1;
    FUN_004b8b30(&s->anim, f->burn, 0);
    if (f->burnShadow != 0) {
        FUN_004b8b30(&s->shadow, f->burnShadow, 0);
        s->hasShadow = 1;
    } else {
        s->hasShadow = 0;
    }
    s->used = 1;
    s->x = x;
    s->z = z;
    s->burnTime = FUN_004b6c30(f->burnTime >> 1) + (f->burnTime >> 1);
    s->noSend = flag;
    Vec3_004233a0 pos;
    pos.x = x << 20;
    pos.z = z << 20;
    FUN_0047f610("treeburn", &pos, 0);
    if (flag == 0) {
        Packet_004233a0 packet;
        packet.type = 0xf;
        packet.sub = 0xfe;
        packet.x = x;
        packet.z = z;
        BroadcastPacket(GetLocalDpid(), &packet, 6);
    }
}
