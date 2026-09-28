// Decompiled by space-bunny-free. Names are provisional.
// Per player slot init: stamps the current tick into three fields, clears 22
// dwords and six shorts, allocates the 0x34-byte PlayerRef and the squads table,
// sizes and clears the map-cell buffer at (width/2) * (height/2) rounded up to
// eight, and gives an active non-network player (type 3) a Class_00408cb0.
//
// The 22 zero stores come out in the order the source writes them (0xac, 0xb4,
// 0xbc, 0xc4, 0xcc, 0xd4, then 0x8c..0xc8, then 0xe8, 0xe4, 0xd0, 0xd8), so the
// statements are written in exactly that order rather than in address order.
// MSVC 5 never reorders stores, so that order is the original source's.
//
// MSVC 5 gives the first *declared* of two uninitialised locals ebx and the
// second edi, so the width/2 temporary is declared first even though the
// height/2 temporary is assigned first. That is what puts height/2 in edi and
// width/2 in ebx across the operator delete call, as the original has; with
// initialised locals (int h = ...; int w = ...) the allocation comes out the
// other way round.
//
// Two things still differ from the original:
//  1. The +0xf8 tick store. Here it follows the +0xf0/+0xf4 stores; the original
//     hoists its load (the g_game reload and the tick read) above the first six
//     zero stores but keeps the store itself below them. Writing the assignment
//     after those six zeros puts the store in the right place but then MSVC
//     leaves the load down there too, so neither order matches.
//  2. The map buffer allocation. The original merges both arms of the
//     conditional into one store with the pointer phi in eax, and then reloads
//     [p + 0x7c] into edi for the inlined memset; this version keeps the phi in
//     edi and forwards it, sinking the store past the memset's own loads, and
//     pushes esi after rep stosd rather than before the shift. Ternary, if/else,
//     a named local, a store through a cast pointer and an inlined helper all
//     give the same edi form.

#include <string.h>

class PlayerRef {
public:
    int unknown[12];
    void* player;
    void Reset(unsigned char playerIndex);
};

#pragma pack(push, 1)
class Class_00408cb0 {                 // 0x3d bytes
public:
    void* player;                      // +0x0
    unsigned char field_4;             // +0x4
    int countdown;                     // +0x5
    int field_9;                       // +0x9
    int field_d;                       // +0xd
    void* timers[10];                  // +0x11
    void* cursor;                      // +0x39
    Class_00408cb0(void* p);
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Player_00464700 {
    int active;                        // +0x00
    char unknown_4[0x73 - 0x4];
    unsigned char type;                // +0x73
    Class_00408cb0* unit;              // +0x74
    char unknown_78[0x7c - 0x78];
    void* buffer;                      // +0x7c
    int f80;                           // +0x80
    int f84;                           // +0x84
    int f88;                           // +0x88
    int f8c;                           // +0x8c
    int f90;
    int f94;
    int f98;
    int f9c;
    int fa0;
    int fa4;
    int fa8;
    int fac;
    int fb0;
    int fb4;
    int fb8;
    int fbc;
    int fc0;
    int fc4;
    int fc8;
    int fcc;
    int fd0;
    int fd4;
    int fd8;
    char unknown_dc[0xe4 - 0xdc];
    int fe4;                           // +0xe4
    int fe8;                           // +0xe8
    PlayerRef* ref;                    // +0xec
    int ff0;                           // +0xf0
    int ff4;                           // +0xf4
    int ff8;                           // +0xf8
    short ffc;                         // +0xfc
    short ffe;                         // +0xfe
    short f100;                        // +0x100
    short f102;                        // +0x102
    short f104;                        // +0x104
    short f106;                        // +0x106
    char unknown_108[0x146 - 0x108];
    unsigned char team;                // +0x146
    char unknown_147[0x149 - 0x147];
    unsigned short flags;              // +0x149
};

struct Game_00464700 {
    char unknown_0[0x14233];
    int width;                         // +0x14233
    int height;                        // +0x14237
    char unknown_1423b[0x38a47 - 0x1423b];
    int ticks;                         // +0x38a47
};
#pragma pack(pop)

extern Game_00464700* g_game;
void* operator new(unsigned int size);
void operator delete(void* p);
void __stdcall FUN_00480190(Player_00464700* p);
void __stdcall FUN_0040b320(int player);

// FUNCTION: 0x464700
void __stdcall FUN_00464700(Player_00464700* p)
{
    p->ff0 = g_game->ticks;
    p->ff4 = g_game->ticks;
    p->ff8 = g_game->ticks;
    p->fac = 0;
    p->fb4 = 0;
    p->fbc = 0;
    p->fc4 = 0;
    p->fcc = 0;
    p->fd4 = 0;
    p->f8c = 0;
    p->f90 = 0;
    p->f94 = 0;
    p->f98 = 0;
    p->f9c = 0;
    p->fa0 = 0;
    p->fa4 = 0;
    p->fa8 = 0;
    p->fb0 = 0;
    p->fb8 = 0;
    p->fc0 = 0;
    p->fc8 = 0;
    p->fe8 = 0;
    p->fe4 = 0;
    p->fd0 = 0;
    p->fd8 = 0;
    if (!p->ref)
        p->ref = new PlayerRef;
    p->ref->Reset(p->team);
    p->flags &= 0xfffe;
    p->ffc = 0;
    p->ffe = 0;
    p->f104 = 0;
    p->f106 = 0;
    p->f102 = p->f100 = -1;
    int w, h;
    h = g_game->height / 2;
    w = g_game->width / 2;
    p->f80 = w;
    p->f84 = h;
    operator delete(p->buffer);
    p->f88 = (h * w + 7) & ~7;
    {
        void* b;
        if (p->f88)
            b = operator new(p->f88);
        else
            b = 0;
        p->buffer = b;
    }
    memset(p->buffer, 0, p->f88);
    FUN_00480190(p);
    if (p->active && p->type != 3) {
        p->unit = new Class_00408cb0(p);
        FUN_0040b320(p->team);
    }
}
