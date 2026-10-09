// Decompiled by Opus, space-bunny-free, Sonnet, Haiku, DeepSeek V4.1 Flash, Claude Opus 5.5, Space Bunny Free, GPT-6, deepseek-v4.1-flash, deepseek-v4.1, GPT-6 Astra, GPT-6.1-sol, LongCat 2.5 Preview Free and mimo-v2.6-pro. Names are provisional.
// Kept its own file: in particles.cpp the ParticleSystem constructor,
// destructor and operator new are defined in the same file, and /Ob2 then
// inlines the base constructor into the two smoke constructors and the
// zeroing loop into the emission entry points, where the original calls
// them out of line.
#include <windows.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>
#include <ctype.h>     // only for its symbol ids, with the block above

// The object pool at g_particlePool (see particles.cpp): AllocSlot takes an
// object from the free list and FreeSlot returns one to it; the inlined
// operator new and delete need them.
#include "object_pool.h"

extern ObjectPool g_particlePool;
extern char g_fxEventPoolBlocked;

// Vtable 0x4fd5a8, constructor 0x471cc0, destructor 0x471d00, ??_G 0x471cd0.
class ParticleSystem {
public:
    int deadline;                                       // +0x4

    ParticleSystem();
    virtual ~ParticleSystem();                          // slot 0
    virtual void Update() = 0;                          // slot 1
    virtual void Render(int) = 0;                       // slot 2
    virtual int IsFinished() = 0;                       // slot 3

    // Not memset: the dword loop plus byte tail is what operator new at
    // 0x471d10 shows, and the loop's size is what /Ob2 charges this function's
    // inline budget for, which is what leaves std::vector::insert (0x4732e0)
    // out of line here. A memset costs one builtin node and the insert gets
    // inlined instead.
    static void* __stdcall operator new(size_t size)
    {
        if (g_fxEventPoolBlocked)
            return 0;
        void* p = (void*)g_particlePool.AllocSlot(size);
        if (p)
        {
            int* q = (int*)p;
            int n = size >> 2;
            while (n-- > 0)
                *q++ = 0;
            if (size & 3)
            {
                char* c = (char*)p + (size & ~3);
                int m = size & 3;
                while (m-- > 0)
                    *c++ = 0;
            }
        }
        return p;
    }

    static void __stdcall operator delete(void* p);

    void SetLifetime(int ticks);
};

// The owner of the per-index lists (see 0x471d90).
#include "particle_lists.h"

struct Vec3_00472630 {
    int x;
    int y;
    int z;
};

struct Vec3_00472720 {
    int x;
    int y;
    int z;
};

struct Vec3_00472810 {
    int x;
    int y;
    int z;
};

struct Vec3_00474cd0 {
    int x;
    int y;
    int z;
};

// The smoke puff's position views and the record.
#include "smoke_particle.h"

struct Vec3_00475150 {
    int x;
    int y;
    int z;
};

struct Vec3_00473560 {
    int x;
    int y;
    int z;
    Vec3_00473560& operator+=(const Vec3_00473560& o)
    {
        x += o.x;
        y += o.y;
        z += o.z;
        return *this;
    }
};

struct Vec3i_00474130 {
    int x;
    int y;
    int z;
    Vec3i_00474130& operator+=(const Vec3i_00474130& o)
    {
        x += o.x;
        y += o.y;
        z += o.z;
        return *this;
    }
};

struct Vec3_004742c0 {
    int x;
    int y;
    int z;

    Vec3_004742c0 operator-(const Vec3_004742c0& o) const
    {
        Vec3_004742c0 r;
        r.x = x - o.x;
        r.y = y - o.y;
        r.z = z - o.z;
        return r;
    }

    // s is 16.16 fixed point. MSVC 5 only orders the two __allmul arguments
    // the way the original does (and keeps the cdq that sign extends the
    // quotient) when the three components go through one inlined method.
    void Scale(int s)
    {
        // One named 64-bit product: MSVC 5 then orders the two __allmul
        // operands as the original does.
        __int64 p;
        p = (__int64)x * s;
        x = (int)(p >> 16);
        p = (__int64)y * s;
        y = (int)(p >> 16);
        p = (__int64)z * s;
        z = (int)(p >> 16);
    }
};

struct Vec3_004739b0 {
    int x, y, z;
    Vec3_004739b0& operator+=(const Vec3_004739b0& o)
    {
        x += o.x;
        y += o.y;
        z += o.z;
        return *this;
    }
};

struct Vec3_00474580 {
    int x, y, z;
    void operator+=(const Vec3_00474580& o) { x += o.x; y += o.y; z += o.z; }
};

#pragma pack(push, 1)
// The integer halves of the 16.16 position.
struct Pos_00473590 {
    short x;                       // +0x00 (record +0x06)
    char unknown_2[0x4 - 0x2];
    short h;                       // +0x04 (record +0x0a)
    char unknown_6[0x8 - 0x6];
    short y;                       // +0x08 (record +0x0e)
};

struct MapSize_00473590 {
    unsigned int width;            // +0x80
    unsigned int height;           // +0x84

    int Contains(int col, int row)
    {
        return (unsigned int)col < width && (unsigned int)row < height;
    }
};

struct Player_00473590 {
    char unknown_0[0x7c];
    unsigned char* seen;           // +0x7c
    MapSize_00473590 size;         // +0x80
    char unknown_88[0x14b - 0x88];
};

// The integer halves of the 16.16 position.
struct Pos_004739b0 {
    char unknown_0[0x2];
    short x;                           // +0x2
    char unknown_4[0x2];
    short height;                      // +0x6
    char unknown_8[0x2];
    short y;                           // +0xa
};

struct Rect_004b0510 {
    int x1;                          // +0x0
    int y1;                          // +0x4
    int x2;                          // +0x8
    int y2;                          // +0xc
};

struct MapSize_00473a00 {
    unsigned int width;              // +0x0
    unsigned int height;             // +0x4

    int Contains(unsigned int tx, unsigned int ty)
    {
        return tx < width && ty < height;
    }
};

struct ByteMap_00473a00 {
    unsigned char* data;             // +0x0
    MapSize_00473a00 size;           // +0x4

    unsigned char Get(int x, int y) { return data[size.width * y + x]; }
};

struct Player_00473a00 {
    char unknown_0[0x7c];
    ByteMap_00473a00 explored;       // +0x7c
    char unknown_88[0x14b - 0x88];   // stride 331
};

struct MapSize_004745e0 {
    unsigned int width;             // +0x80
    unsigned int height;            // +0x84

    int Contains(int col, int row)
    {
        return (unsigned int)col < width && (unsigned int)row < height;
    }
};

struct ByteMap_004745e0 {
    unsigned char* data;            // +0x7c
    MapSize_004745e0 size;          // +0x80

    unsigned char Get(int tx, int ty) { return data[size.width * ty + tx]; }
};

struct Map_004745e0 {               // one entry of g_game->players
    char unknown_0[0x7c];
    ByteMap_004745e0 explored;      // +0x7c
    char unknown_88[0x14b - 0x88];
};

struct MapSize_00474b80 {
    unsigned int width;            // +0x80
    unsigned int height;           // +0x84

    int Contains(int col, int row)
    {
        return (unsigned int)col < width && (unsigned int)row < height;
    }
};

struct ByteMap_00474b80 {
    unsigned char* data;           // +0x7c
    MapSize_00474b80 size;         // +0x80

    unsigned char Get(int x, int y) { return data[size.width * y + x]; }
};

struct Player_00474b80 {
    char unknown_0[0x7c];
    ByteMap_00474b80 explored;     // +0x7c
    char unknown_88[0x14b - 0x88];
};

struct MapSize_00475470 {
    unsigned int width;                // +0x0
    unsigned int height;               // +0x4
    int Contains(unsigned int tx, unsigned int ty)
    {
        return tx < width && ty < height;
    }
};

struct ByteMap_00475470 {
    unsigned char* data;               // +0x0
    MapSize_00475470 size;             // +0x4
    int Index(int x, int y) { return size.width * y + x; }
    // Get goes through Index: the extra register shapes both arms.
    unsigned char Get(int x, int y) { return data[Index(x, y)]; }
};

struct Player_00475470 {
    char unknown_0[0x7c];
    ByteMap_00475470 explored;         // +0x7c
    char unknown_88[0x14b - 0x88];
};

struct Position_00475150 {             // 16.16 fixed point; only high words read
    short xFrac;
    short x;                           // +0x2
    short yFrac;
    short y;                           // +0x6, the height
    short zFrac;
    short z;                           // +0xa
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x1b63];
    // The five partial views of one player slot the inlined fog tests below
    // were matched with; one file cannot hold five of the header's type.
    union {
        Player_00473590 players_00473590[10];   // +0x1b63, stride 0x14b
        Player_00473a00 players_00473a00[10];
        Map_004745e0 players_004745e0[10];
        Player_00474b80 players_00474b80[10];
        Player_00475470 players_00475470[10];
    };
    char unknown_2851[0x2a43 - 0x2851];
    unsigned char playerIndex;         // +0x2a43
    char unknown_2a44[0x14263 - 0x2a44];
    int rise;                          // +0x14263
    char unknown_14267[0x14273 - 0x14267];
    unsigned short* visibilityMask;    // +0x14273
    char unknown_14277[0x1427f - 0x14277];
    unsigned char seaLevel;            // +0x1427f
    char debugMode;
    unsigned short mapFlags;           // +0x14281, bit 1 (mask 2)
    char unknown_14283[0x1431f - 0x14283];
    int scrollX;                       // +0x1431f
    int scrollY;                       // +0x14323
    char unknown_14327[0x147cf - 0x14327];
    void* unknown_147cf;               // +0x147cf, the smoke animation
    void* unknown_147d3;               // +0x147d3, the other smoke animation
    char unknown_147d7[0x37ecc - 0x147d7];
    int windX;                         // +0x37ecc
    char unknown_37ed0[4];
    int windZ;                         // +0x37ed4
    char unknown_37ed8[0x38a47 - 0x37ed8];
    int ticks;                         // +0x38a47
    char unknown_38a4b[0x38d77 - 0x38a4b];
    ParticleLists* lists;              // +0x38d77, the ten per-index lists
};
#pragma pack(pop)

extern Game* g_game;

void* __stdcall GetGafFrame(void* a, int b);
void __stdcall DrawFrameBlended(void* dest, void* src, int x, int y);
int __stdcall GetGafFrameCount(void* ptr);
void __stdcall FillRectangle(void* surface, Rect_004b0510* rect, int color);
int __stdcall GetGroundHeight(void* param_1);

// The second arm of the spark draws, inlined. The two player parameters are
// not a typo: the Contains test reads the map width through `p` and the index
// reads it through `q`, two address nodes, which is what stops the width load
// folding into the imul and keeps the original's `mov edx,[edx+0x80]; imul
// edx,ecx` pair.
static inline int IsSeen_00473590(Player_00473590* p, Player_00473590* q, int col, int row)
{
    if (!p->size.Contains(col, row))
        return 0;
    return (g_game->visibilityMask[q->size.width * row + col] &
            (1 << g_game->playerIndex)) != 0;
}

// The allocation lever, as at 0x473a00: the wrap pins the prologue, the
// pre-branch block and the mask arm's register choice without emitting a
// single extra instruction.
static inline int Identity_00473590(int v) { return v; }

static inline int Identity_00473a00(int v) { return v; }

static inline int IsSeen_00473a00(Player_00473a00* p, Player_00473a00* q, int col, int row)
{
    if (!p->explored.size.Contains((unsigned int)col, (unsigned int)row))
        return 0;
    return (g_game->visibilityMask[q->explored.size.width * row + col] &
            (1 << g_game->playerIndex)) != 0;
}

static inline int Identity_00474170(int v) { return v; }

static inline int IsSeen_00474170(Player_00473590* p, Player_00473590* q, int col, int row)
{
    if (!p->size.Contains(col, row))
        return 0;
    return (g_game->visibilityMask[q->size.width * row + col] &
            (1 << g_game->playerIndex)) != 0;
}

// The pin the earlier passes were looking for: a helper that returns its
// argument. It emits no instruction, but MSVC 5 allocates what it returns as a
// fresh live range, which is what holds the frame in place.
static inline int Identity_004745e0(int v) { return v; }

static inline int IsSeen_004745e0(Map_004745e0* p, Map_004745e0* q, int col, int row)
{
    if (!p->explored.size.Contains(col, row))
        return 0;
    return (g_game->visibilityMask[q->explored.size.width * row + col] &
            (1 << g_game->playerIndex)) != 0;
}

static inline int Identity_00474b80(int v) { return v; }

static inline int IsSeen_00474b80(Player_00474b80* p, Player_00474b80* q, int col, int row)
{
    if (!p->explored.size.Contains(col, row))
        return 0;
    return (g_game->visibilityMask[q->explored.size.width * row + col] &
            (1 << g_game->playerIndex)) != 0;
}

#pragma pack(push, 1)
struct Pos_004745e0 {
    short x;                        // +0
    char unknown_2[2];
    short height;                   // +4
    char unknown_6[2];
    short y;                        // +8

    // Is this record's position inside the explored byte map of the local
    // player (map flags bit 1 set), or inside their bit of the shared visibility
    // mask (bit clear)? The two arms keep their own fail block, as the original
    // does.
    int Visible()
    {
        Map_004745e0* p = &g_game->players_004745e0[g_game->playerIndex];
        Map_004745e0* p2 = &g_game->players_004745e0[g_game->playerIndex];
        int visible;
        if ((g_game->mapFlags & 2) == 2) {
            int col = x >> 5;
            int row = (y - (height >> 1)) >> 5;
            if (p->explored.size.Contains(col, row) &&
                p->explored.Get(col, row) != 0)
                visible = 1;
            else
                visible = 0;
        } else {
            int col = x >> 5;
            int row = (y - (height >> 1)) >> 5;
            visible = Identity_004745e0(IsSeen_004745e0(p, p2, col, row));
        }
        return visible;
    }
};
#pragma pack(pop)

// One particle of TimedSubParticles, 0x20 bytes.
struct TimedSubParticle {
    void* data;                        // +0x00, the animation
    union {
        Vec3_00475150 pos;             // +0x04
        Position_00475150 posw;
    };
    int limit;                         // +0x10, the rounds it lives
    int count;                         // +0x14, the rounds so far (the frame)
    int period;                        // +0x18
    int timer;                         // +0x1c

    void Step();
    void DrawParticle(void* dest, short px, short py);
    int IsExpired(int unused);
};

// Vtable 0x4fd618, constructor 0x474cd0, ??_G 0x474d10; 0x38 bytes.
class SmokeParticles : public ParticleSystem {
public:
    int time;                                           // +0x8, the next emit tick
    std::vector<SmokeParticle> records;                // +0xc (_First +0x10)
    int emitPeriod;                                     // +0x1c, the emit period
    int holdPeriod;                                     // +0x20
    int maxFrame;                                       // +0x24, the frame count - 1
    int altAnimation;                                   // +0x28, the other animation
    Vec3_00474d50 pos;                                  // +0x2c

    SmokeParticles();
    virtual void Update();                              // slot 1, 0x475340
    virtual void Render(int);                           // slot 2, 0x475470
    virtual int IsFinished();                           // slot 3, 0x474f80
    virtual void Emit();                                // slot 4, 0x474df0
    virtual int IsEmitDue();                            // slot 5, 0x475440
    virtual void Init(Vec3_00474d50* pos, int limit, int a, int b, int c,
                              int alt);                 // slot 6, 0x474d50
};

// Vtable 0x4fd638, constructor 0x4750b0, ??_G 0x475110; 0x34 bytes.
class TimedSubParticles : public ParticleSystem {
public:
    int time;                                           // +0x8, the next emit tick
    std::vector<TimedSubParticle> records;              // +0xc (_First +0x10)
    int emitPeriod;                                     // +0x1c, the emit period
    int holdPeriod;                                     // +0x20
    int maxFrame;                                       // +0x24, the frame count - 1
    Vec3_00475150 pos;                                  // +0x28

    TimedSubParticles();
    virtual void Update();                              // slot 1, 0x475600
    virtual void Render(int);                           // slot 2, 0x475700
    virtual int IsFinished();                           // slot 3, 0x475330
    virtual void Emit();                                // slot 4, 0x4751c0
    // In particles_4750f0.cpp: it is defined returning bool, and Update tests
    // its result as an int.
    virtual int IsEmitDue();                            // slot 5, 0x4750f0
    virtual void Init(Vec3_00475150* pos, int a, int b, int c); // slot 6, 0x475150
};

// One teleport spark (the element of TeleportParticles' vector), 0x34 bytes.
#pragma pack(push, 1)
class TeleportParticle {
public:
    void* data;                    // +0x00, the animation
    union {
        struct {
            Vec3_00473560 pos;     // +0x04
            Vec3_00473560 pos2;    // +0x10
            Vec3_00473560 vel;     // +0x1c
        };
        struct {
            char unknown_4[0x6 - 0x4];
            Pos_00473590 posw;     // +0x06
        };
    };
    int count;                     // +0x28, the frame count
    int frame;                     // +0x2c, the frame
    int endTime;                   // +0x30, the tick it expires

    void Step();
    void DrawParticle(void* dest, short px, short py);
    int IsExpired(int param_1);
};
#pragma pack(pop)

// One nano spark (the element of NanoParticles' vector), 0x30 bytes.
#pragma pack(push, 1)
struct NanoParticle {
public:
    union {
        Vec3_004739b0 pos;             // +0x0
        Pos_004739b0 posw;
    };
    char unknown_c[0xc];
    Vec3_004739b0 vel;                 // +0x18
    char unknown_24[4];
    int flags;                         // +0x28, low 4 bits: frame; the colour
    int endTime;                       // +0x2c, the tick it expires

    void Step();
    void DrawParticle(int param_1, short x, short y);
    int IsExpired(int value);
};
#pragma pack(pop)

// One exhaust puff (the element of ThrustParticles' vector), 0x3c bytes.
#pragma pack(push, 1)
class ThrustParticle {
public:
    void* data;                    // +0x00, the animation
    union {
        struct {
            Vec3i_00474130 pos;    // +0x04
            Vec3i_00474130 pos1;   // +0x10
            Vec3i_00474130 vel;    // +0x1c
        };
        struct {
            char unknown_4[0x6 - 0x4];
            Pos_00473590 posw;     // +0x06
        };
    };
    int mod2_28;                   // +0x28, the frame count
    int frame;                     // +0x2c, the frame
    int val_30;                    // +0x30
    int mod_34;                    // +0x34
    int endTime;                   // +0x38, the tick it expires

    void Step();
    void DrawParticle(void* dest, short px, short py);
    int IsExpired(int value);
};
#pragma pack(pop)

// One wake spark (the element of WakeParticles' vector), 0x44 bytes.
#pragma pack(push, 1)
class WakeParticle {
public:
    int data;                          // +0x00
    union {
        struct {
            Vec3_00474580 pos;         // +0x04
            Vec3_00474580 pos2;        // +0x10
            Vec3_00474580 vel;         // +0x1c
        };
        struct {
            char unknown_4[0x6 - 0x4];
            Pos_004745e0 posw;         // +0x06
        };
    };
    int min;                           // +0x28
    int max;                           // +0x2c
    int value;                         // +0x30, the colour
    int step;                          // +0x34
    int tick;                          // +0x38
    int period;                        // +0x3c
    int endTime;                       // +0x40, the tick it expires

    void Step();
    void DrawParticle(void* surface, short px, short py);
    int IsExpired(int param_1);
};
#pragma pack(pop)

struct Shape_00472ab0;

// Vtable 0x4fd5d8, ??_G 0x4716a0; 0x44 bytes.
class ThrustParticles : public ParticleSystem {
public:
    int time;                                           // +0x8
    std::vector<ThrustParticle> items;                  // +0xc (_First +0x10)
    int period;                                         // +0x1c
    Vec3_004742c0 pos0;                                 // +0x20
    Vec3_004742c0 pos1;                                 // +0x2c
    Vec3_004742c0 pos2;                                 // +0x38

    ThrustParticles() {}
    virtual void Update();                              // slot 1, 0x473010
    virtual void Render(int);                           // slot 2, 0x4730f0
    virtual int IsFinished();                           // slot 3, 0x473130
    virtual void Emit();                                // slot 4, 0x4743a0
    virtual int IsEmitDue();                            // slot 5, 0x4730c0
    virtual void Init(Shape_00472ab0* p, Shape_00472ab0* q, int a, int b);
    void Init(Vec3_004742c0* p, Vec3_004742c0* q, int a, int b);
};

// The pool allocator of the vector members below, to call their out-of-line
// insert under this name.
class Class_00476210 {
public:
    void FUN_00476210(std::vector<SmokeParticle>::iterator p, unsigned int m,
                     const SmokeParticle& x);
};

class Vec_00476490 {
public:
    void FUN_00476490(TimedSubParticle* pos, int count, const TimedSubParticle* src);
};

// The vector's own out-of-line size() (0x475840) under the name
// data/symbols.csv gives that address.
class Class_00475840 {
public:
    int unknown_0;
    int field_4;
    int field_8;

    int GetCount();
};

// std::vector<Elem_00473500> and its out-of-line members: size() (0x472d30),
// _Destroy (0x4732d0), _Ucopy (0x473500) and _Ufill (0x473530). 0x471820 and
// 0x471a50 call them.
struct Elem_00473500 {
    int unknown_0;
};

typedef std::vector<Elem_00473500> Vec_00473500;
typedef Vec_00473500::size_type (Vec_00473500::*SizeFn_00473500)() const;
typedef Vec_00473500::iterator (Vec_00473500::*UcopyFn_00473500)(
    Vec_00473500::const_iterator, Vec_00473500::const_iterator, Vec_00473500::iterator);
typedef void (Vec_00473500::*UfillFn_00473500)(
    Vec_00473500::iterator, Vec_00473500::size_type, const Elem_00473500&);
typedef void (Vec_00473500::*DestroyFn_00473500)(Vec_00473500::iterator, Vec_00473500::iterator);

// _Ucopy, _Ufill and _Destroy are protected: a derived class takes their
// addresses to emit them out of line.
struct Access_00473500 : Vec_00473500 {
    static UcopyFn_00473500 ucopy;
    static UfillFn_00473500 ufill;
    static DestroyFn_00473500 destroy;
};

// Taking the member's address emits the out-of-line copy.
SizeFn_00473500 g_size_00473500 = &Vec_00473500::size;

// std::vector<NanoParticle> (0x30-byte element type) and its out-of-line
// members: reserve (0x475770), _Destroy (0x475870), _Ucopy (0x475880) and
// _Ufill (0x476710).
typedef std::vector<NanoParticle> Vec_004739b0;
typedef void (Vec_004739b0::*ReserveFn_004739b0)(Vec_004739b0::size_type);
typedef Vec_004739b0::iterator (Vec_004739b0::*UcopyFn_004739b0)(
    Vec_004739b0::const_iterator, Vec_004739b0::const_iterator, Vec_004739b0::iterator);
typedef void (Vec_004739b0::*UfillFn_004739b0)(
    Vec_004739b0::iterator, Vec_004739b0::size_type, const NanoParticle&);
typedef void (Vec_004739b0::*DestroyFn_004739b0)(Vec_004739b0::iterator, Vec_004739b0::iterator);

struct Access_00475770 : Vec_004739b0 {
    static ReserveFn_004739b0 fn;
};

struct Access_00475870 : Vec_004739b0 {
    static DestroyFn_004739b0 fn;
};

struct Access_00475880 : Vec_004739b0 {
    static UcopyFn_004739b0 fn;
};

struct Access_00476710 : Vec_004739b0 {
    static UfillFn_004739b0 fn;
};

// FUNCTION: 0x472630
void __stdcall EmitSmoke(Vec3_00472630* pos, int param_2, int param_3, short index)
{
    ParticleLists* owner = g_game->lists;
    SmokeParticles* e = new SmokeParticles;
    if (e) {
        e->Init((Vec3_00474d50*)pos, 0, param_2, 0, param_3, 0);
        owner->Add(index, e);
    }
}

// FUNCTION: 0x472720
void __stdcall EmitTimedBlackSmoke(Vec3_00472720* pos, int param_2, int param_3, short index)
{
    ParticleLists* owner = g_game->lists;
    SmokeParticles* e = new SmokeParticles;
    if (e) {
        e->Init((Vec3_00474d50*)pos, 0, param_2, 0, param_3, 1);
        owner->Add(index, e);
    }
}

// FUNCTION: 0x472810
void __stdcall EmitWhiteSmoke(Vec3_00472810* pos, short index)
{
    ParticleLists* owner = g_game->lists;
    SmokeParticles* e = new SmokeParticles;
    if (e) {
        e->Init((Vec3_00474d50*)pos, 0, 1, 0, 0, 0);
        owner->Add(index, e);
    }
}

// FUNCTION: 0x4728f0
void __stdcall EmitBlackSmoke(Vec3_00474cd0* p, short index)
{
    // The list owner is read through a reference before the allocation, so
    // g_game and its +0x38d77 field are loaded ahead of the pool call and stay
    // live across it.
    ParticleLists& l = *g_game->lists;
    SmokeParticles* e = new SmokeParticles;
    if (e) {
        e->Init((Vec3_00474d50*)p, 0, 1, 0, 0, 1);
        l.Add(index, e);
    }
}

// FUNCTION: 0x4729d0
void __stdcall EmitWeaponSmoke(Vec3_00474cd0* p, short index)
{
    ParticleLists& l = *g_game->lists;
    SmokeParticles* e = new SmokeParticles;
    if (e) {
        e->Init((Vec3_00474d50*)p, 3, 1, 0x1e, 0, 0);
        l.Add(index, e);
    }
}

// FUNCTION: 0x472c50
void __stdcall EmitTimedSubParticles(Vec3_00475150* p, short index)
{
    ParticleLists& l = *g_game->lists;
    TimedSubParticles* e = new TimedSubParticles;
    if (e) {
        e->Init(p, 5, 0, 0x96);
        l.Add(index, e);
    }
}

#pragma auto_inline(off)
// FUNCTION: 0x474cd0
// FUNCTION: 0x474d10 ??_GSmokeParticles@@UAEPAXI@Z
SmokeParticles::SmokeParticles()
{
    time = g_game->ticks;
}
#pragma auto_inline(on)

#pragma auto_inline(off)
// FUNCTION: 0x4750b0
// FUNCTION: 0x475110 ??_GTimedSubParticles@@UAEPAXI@Z
TimedSubParticles::TimedSubParticles()
{
    time = g_game->ticks;
}
#pragma auto_inline(on)
