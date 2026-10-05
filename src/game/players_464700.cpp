// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, verified by GPT-6.1-sol, edited by deepseek-v4.1, finished by GPT-6.1-sol, finished by mimo-v2.6-pro, finished by space-bunny-free, finished by DeepSeek V4.1 Flash, verified by GPT-6, finished by claude-opus-5-5. Names are provisional.
// Per player slot init: stamps the current tick into three fields, clears 22
// dwords and six shorts, allocates the 0x34-byte PlayerRef and the squads table,
// sizes and clears the map-cell buffer at (width/2) * (height/2) rounded up to
// eight, and gives an inactive or non-network (type 3) player a Class_00408cb0.
//
// How the first block matched (#5560), read out of C2 with the #5288
// scheduler tracer (build/scratch/0x464700/c2order.py, hook 0x4315f1):
// - The original keeps only p and the constant 0 as register candidates in
//   the first block. Any value carried across the six clears (a tick local,
//   an inline parameter) raises that block's K from 2 to 3 and the zero's
//   priority from 67 to 89, above w's 70, which moves the zero from ebp to
//   ebx. So the third tick's load and store stay one statement, and C2's
//   post-allocation scheduler has to sink the store below the six clears.
// - The scheduler orders two accesses through p when C2's alias table
//   (FUN_00422afb) says they may alias. It numbers p's memory locations in
//   tuple order, gives each the bit min(31, number of locations found after
//   it), and makes all locations at bit 31 alias each other. The original's
//   order (three loads, six clears, cmp, ff8 store, f8c store, ...) needs the
//   six clears and f8c, and nothing stored after them, at bit 31, and the ff8
//   store below bit 31.
// - So the ff8 stamp is written after the clears and reached by `goto`:
//   it still runs before them (C2 lays the blocks out in execution order
//   before scheduling), but its location is numbered after f90. This is
//   probably not the original's spelling; it is the one found that gives C2
//   this tuple order without adding code. In the natural order (stamp third)
//   the ff8 store chains in front of the clears (98.3%).
// - The size field is written through the reference `sz`, which takes one
//   location out of p's count (41 to 40) and so drops f90 out of bit 31;
//   with `p->f88 = ...` f90 stays in the chain and the f8c store lands
//   before the cmp (99.2%). Writing p->flags or p->unit through a reference
//   does the same.
//
// The 22 zero stores come out in the order the source writes them (0xac, 0xb4,
// 0xbc, 0xc4, 0xcc, 0xd4, then 0x8c..0xc8, then 0xe8, 0xe4, 0xd0, 0xd8).
//
// MSVC 5 gives the first *declared* of two uninitialised locals ebx and the
// second edi, so the width/2 temporary is declared first even though the
// height/2 temporary is assigned first. That is what puts height/2 in edi and
// width/2 in ebx across the operator delete call, as the original has; with
// initialised locals (int h = ...; int w = ...) the allocation comes out the
// other way round.
//
// The last test is `!p->active || p->type != 3`, not `p->active && p->type != 3`:
// the original's first branch is `je` into the allocation and the second `je`
// over it, which is the `||` with both tests left as they are.
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

struct Game {
    char unknown_0[0x14233];
    int width;                         // +0x14233
    int height;                        // +0x14237
    char unknown_1423b[0x38a47 - 0x1423b];
    int ticks;                         // +0x38a47
};
#pragma pack(pop)

extern Game* g_game;
void* __cdecl operator new(unsigned int size);
void __cdecl operator delete(void* p);
void __stdcall CreateSquads(Player_00464700* p);
void __stdcall FUN_0040b320(int player);

// FUNCTION: 0x464700
void __stdcall InitPlayerSlot(Player_00464700* p)
{
    p->ff0 = g_game->ticks;
    p->ff4 = g_game->ticks;
    // The third stamp runs here but is written after the clears (see the
    // notes at the top): C2 has to number its location after theirs.
    goto stamp;
back:
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
    goto guard;
stamp:
    p->ff8 = g_game->ticks;
    goto back;
guard:
    if (!p->ref) {
        p->ref = new PlayerRef;
    }
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
    int& sz = p->f88;
    sz = (h * w + 7) & ~7;
    p->buffer = sz ? operator new(sz) : 0;
    // Both references are load-bearing (`sz` also for the first block, see
    // the notes at the top). With `memset(p->buffer, 0, p->f88)`
    // MSVC propagates the phi and the size into the inlined memset, which puts
    // the phi in edi and stores it from edi; the original reloads [esi+0x7c]
    // into edi and keeps the phi in eax. Reading the fields through references
    // stops that propagation, and it also stops the scheduler from hoisting
    // the size load `mov ecx, [esi+0x88]` above the phi store (with the size
    // read directly the load comes first, the original has it second).
    void*& bref = p->buffer;
    memset(bref, 0, sz);
    CreateSquads(p);
    if (!p->active || p->type != 3) {
        p->unit = new Class_00408cb0(p);
        FUN_0040b320(p->team);
    }
}
