// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, verified by GPT-6.1-sol, edited by deepseek-v4.1, finished by GPT-6.1-sol, finished by mimo-v2.6-pro, finished by space-bunny-free, finished by DeepSeek V4.1 Flash, verified by GPT-6, finished by claude-opus-5-5. Names are provisional.
// Per player slot init: stamps the current tick into three fields, clears 22
// dwords and six shorts, allocates the 0x34-byte UnitResources and the squads table,
// sizes and clears the map-cell buffer at (width/2) * (height/2) rounded up to
// eight, and gives an inactive or non-network (type 3) player a SquadManager.

#include <string.h>

class UnitResources {
public:
    int unknown[12];
    void* player;
    void Reset(unsigned char playerIndex);
};

#pragma pack(push, 1)
class SquadManager {                   // 0x3d bytes
public:
    void* player;                      // +0x0
    unsigned char field_4;             // +0x4
    int countdown;                     // +0x5
    int field_9;                       // +0x9
    int field_d;                       // +0xd
    void* timers[10];                  // +0x11
    void* cursor;                      // +0x39
    SquadManager(void* p);
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Player_00464700 {
    int active;                        // +0x00
    char unknown_4[0x73 - 0x4];
    unsigned char type;                // +0x73
    SquadManager* unit;                // +0x74
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
    UnitResources* ref;                // +0xec
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
void __stdcall CreatePlayerAI(int player);

// FUNCTION: 0x464700
void __stdcall InitPlayerSlot(Player_00464700* p)
{
    p->ff0 = g_game->ticks;
    p->ff4 = g_game->ticks;
    // The third stamp runs here but is written after the clears: C2 has to number its location after theirs.
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
        p->ref = new UnitResources;
    }
    p->ref->Reset(p->team);
    p->flags &= 0xfffe;
    p->ffc = 0;
    p->ffe = 0;
    p->f104 = 0;
    p->f106 = 0;
    p->f102 = p->f100 = -1;
    // w declared first: puts height/2 in edi and width/2 in ebx.
    int w, h;
    h = g_game->height / 2;
    w = g_game->width / 2;
    p->f80 = w;
    p->f84 = h;
    operator delete(p->buffer);
    int& sz = p->f88;
    sz = (h * w + 7) & ~7;
    p->buffer = sz ? operator new(sz) : 0;
    // Both references are load-bearing (`sz` also for the first block). With `memset(p->buffer, 0, p->f88)`
    // MSVC propagates the phi and the size into the inlined memset, which puts
    // the phi in edi and stores it from edi; the original reloads [esi+0x7c]
    // into edi and keeps the phi in eax. Reading the fields through references
    // stops that propagation, and it also stops the scheduler from hoisting
    // the size load `mov ecx, [esi+0x88]` above the phi store (with the size
    // read directly the load comes first, the original has it second).
    void*& bref = p->buffer;
    memset(bref, 0, sz);
    CreateSquads(p);
    // Keep `||` with both tests as written: the branch layout depends on it.
    if (!p->active || p->type != 3) {
        p->unit = new SquadManager(p);
        CreatePlayerAI(p->team);
    }
}
