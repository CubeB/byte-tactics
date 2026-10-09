// Decompiled by Opus, space-bunny-free, Sonnet, Haiku, DeepSeek V4.1 Flash, Claude Opus 5.5, Space Bunny Free, GPT-6, claude-opus-5-5, deepseek-v4.1-flash, deepseek-v4.1, GPT-6 Astra, GPT-6.1-sol, LongCat 2.5 Preview Free and mimo-v2.6-pro. Names are provisional.
// The particles module's translation unit: the object pool (vtable 0x4fd580),
// the ten per-index lists, the ParticleSystem base (vtable 0x4fd5a8) and its
// Teleport, Nano, Thrust, Wake, Smoke and TimedSubParticles subclasses, their
// particle records, and the out-of-line std::vector members they call. The
// lists and the pool are reached through g_game.
//
// Files the gather still leaves separate (docs/split-modules.md):
// 0x470c10 and 0x473d50 need a hand-written <vector> view that cannot share
// this file with the real <vector>. 0x471160, 0x471820 and 0x471a50 need the
// std::vector<Elem_00473500> view of the lists, whose inlined add cannot agree
// with this file's std::vector<ParticleSystem*>. 0x471de0's SIB byte follows
// the file's symbol total (docs/c2-regalloc.md). The smoke and
// TimedSubParticles cluster (0x472630 to 0x472c50 and the two classes'
// constructors and deleting destructors) stays in particles_472630.cpp: there
// ParticleSystem's constructor, destructor and operator new are not defined,
// which keeps the base calls and the vector insert out of line as the original
// has them. The six vector::insert files (0x4732e0, 0x4758c0, 0x475bd0,
// 0x475ef0, 0x476210, 0x476490) carry // FLAGS: /Gi, which this file cannot.
// 0x473a00, 0x474b80 and 0x475470's fog-culled draws and 0x475700's inlined
// draw pick other registers (SIB base, fog pointer, temporaries) in this
// file's symbol context. 0x4750f0 returns bool where 0x475600 tests the
// result as an int.
#include <windows.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <vector>
#include <cstdlib>
#include <ctype.h>     // only for its symbol ids, with the block above

void __cdecl GameFreeThunk(void* p);

#pragma pack(push, 2)
struct Elem_00470a40 {
    int a;
    int b;
    int c;
    short d;
};
#pragma pack(pop)

struct Item_00470ae0 {
    int unknown_0;
};

struct Elem_00470f00 {
    int unknown_0;
};

// The pool object at g_particlePool (vtable 0x4fd580). 0x470c10, its Grow,
// stays in particles_470c10.cpp.
class ObjectPool {
public:
    std::vector<Item_00470ae0*> items;  // +0x4
    void* field_14;                     // +0x14, the slot table
    int field_18;                       // +0x18
    int field_1c;                       // +0x1c, the slot count
    int field_20;                       // +0x20, slots handed out

    ObjectPool(int param_1, int param_2)
    {
        field_14 = 0;
        field_18 = 0;
        field_1c = 0;
        field_20 = 0;
        if (param_1 != 0 && param_2 != 0)
            Grow(param_1, param_2);
    }
    virtual ~ObjectPool()
    {
        if (field_14 != 0)
            GameFreeThunk(field_14);
        std::vector<Item_00470ae0*>::iterator it = items.begin();
        while (it != items.end()) {
            GameFreeThunk(*it);
            items.erase(it);
        }
    }
    void FreeBlocks();
    int Grow(int param_1, int param_2);
    int AllocSlot(int unused);
    void FreeSlot(int param_1);
    ObjectPool* Construct(int param_1, int param_2);
    void Destroy();
};

// The 12-byte fixed-point vector. The teleport sparks use the first three
// members and their Length; the teleport records' owner scales the vector
// between two positions with operator-= style members below.
#pragma pack(push, 1)
struct Vec3_004736e0 {
    int x;
    int y;
    int z;
    Vec3_004736e0 operator-(const Vec3_004736e0& o) const
    {
        Vec3_004736e0 r;
        r.x = x - o.x;
        r.y = y - o.y;
        r.z = z - o.z;
        return r;
    }
    int Length() const
    {
        float fx = x;
        float fy = y;
        float fz = z;
        return (int)sqrt(fx * fx + fy * fy + fz * fz);
    }
    Vec3_004736e0& operator+=(const Vec3_004736e0& o)
    {
        x += o.x;
        y += o.y;
        z += o.z;
        return *this;
    }
};

union Fix_004736e0 {
    int whole;
    short half[2];
};

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

// Is this record's position inside the explored byte map of the local player
// (flags bit 1 set), or inside their bit of the shared visibility mask (bit
// clear)? Visible's body is below, after g_game. The two arms keep their own
// fail block, as the original does.
struct Pos_004745e0 {
    short x;                        // +0
    char unknown_2[2];
    short height;                   // +4
    char unknown_6[2];
    short y;                        // +8

    int Visible();
};

// One teleport spark (the element of TeleportParticles' vector), 0x34 bytes.
class TeleportParticle {
public:
    void* data;                        // +0x00, the animation
    union {
        struct {
            Vec3_004736e0 pos1;        // +0x04
            Vec3_004736e0 pos2;        // +0x10
            Vec3_004736e0 dir;         // +0x1c
        };
        struct {
            char unknown_4[0x6 - 0x4];
            Pos_00473590 posw;         // +0x06
        };
    };
    int frameCount;                    // +0x28, the frame count - 1
    int frame;                         // +0x2c
    int endTime;                       // +0x30, the tick it expires

    void Step();
    void DrawParticle(void* dest, short px, short py);
    int IsExpired(int param_1);
};

// The particle vector seen as its four words, to call its out-of-line insert.
class List_004737c0 {
public:
    char* head;                        // +0x00
    char* first;                       // +0x04
    char* last;                        // +0x08
    char* end;                         // +0x0c
    void insert(char* where, int count, const TeleportParticle& val);
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

// The integer halves of the 16.16 position.
struct Pos_004739b0 {
    char unknown_0[0x2];
    short x;                           // +0x2
    char unknown_4[0x2];
    short height;                      // +0x6
    char unknown_8[0x2];
    short y;                           // +0xa
};

// One nano spark (the element of NanoParticles' vector), 0x30 bytes.
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

struct Vec3_00473b50 {
    int x;
    int y;
    int z;
};

struct Seg_00473b50 {
    Vec3_00473b50 start;
    Vec3_00473b50 end;
};

// Works on direct members, not references, and reads `d = e - s` after `v = s`:
// the store scheduling and the CSE depend on both.
#define SPLIT_SEG(g)                         \
    {                                        \
        int v, d;                            \
        v = (g).start.x;                     \
        d = (g).end.x - (g).start.x;         \
        (g).start.x = v + d * 4 / 11;        \
        (g).end.x = v + d * 7 / 11;          \
        v = (g).start.y;                     \
        d = (g).end.y - (g).start.y;         \
        (g).start.y = v + d * 4 / 11;        \
        (g).end.y = v + d * 7 / 11;          \
        v = (g).start.z;                     \
        d = (g).end.z - (g).start.z;         \
        (g).start.z = v + d * 4 / 11;        \
        (g).end.z = v + d * 7 / 11;          \
        (g).end.x -= (g).start.x;            \
        (g).end.y -= (g).start.y;            \
        (g).end.z -= (g).start.z;            \
    }

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

    Vec3_004742c0& operator+=(const Vec3_004742c0& o)
    {
        x += o.x;
        y += o.y;
        z += o.z;
        return *this;
    }
};

// One exhaust puff (the element of ThrustParticles' vector), 0x3c bytes.
class ThrustParticle {
public:
    unsigned short* bitmask;           // +0x00, the animation
    union {
        struct {
            Vec3_004742c0 pos0;        // +0x04
            Vec3_004742c0 pos1;        // +0x10
            Vec3_004742c0 pos2;        // +0x1c
        };
        struct {
            char unknown_4[0x6 - 0x4];
            Pos_00473590 posw;         // +0x06
        };
    };
    int frameCount;                    // +0x28, the frame count - 1
    int frame;                         // +0x2c
    int tick;                          // +0x30
    int period;                        // +0x34
    int endTime;                       // +0x38

    void Step();
    void DrawParticle(void* dest, short px, short py);
    int IsExpired(int param_1);
};

typedef std::vector<ThrustParticle> Vec_004743a0;

// The particle vector, to call its out-of-line insert under this name.
class Class_00475bd0 {
public:
    void* insert(ThrustParticle* at, unsigned int n, const ThrustParticle& x);
};

struct Pair_00474880 {
    short lo;
    short hi;
};

// A 16.16 point; the jitter in Emit writes the high halves.
struct Vec3_00474760 {
    union {
        int x;
        Pair_00474880 xp;
    };
    union {
        int y;
        Pair_00474880 yp;
    };
    union {
        int z;
        Pair_00474880 zp;
    };
    int Length() const
    {
        // Each component converts into its own double local.
        double fx = x;
        double fy = y;
        double fz = z;
        return (int)sqrt(fx * fx + fy * fy + fz * fz);
    }
    // Must be a member: the components are then reached through the vector's address.
    void Scale(int s)               // s is 16.16 fixed point
    {
        x = (int)(((__int64)x * s) >> 16);
        y = (int)(((__int64)y * s) >> 16);
        z = (int)(((__int64)z * s) >> 16);
    }
    Vec3_00474760& operator+=(const Vec3_00474760& o)
    {
        x += o.x;
        y += o.y;
        z += o.z;
        return *this;
    }
};

static inline Vec3_00474760 operator-(const Vec3_00474760& p, const Vec3_00474760& q)
{
    Vec3_00474760 r;
    r.x = p.x - q.x;
    r.y = p.y - q.y;
    r.z = p.z - q.z;
    return r;
}

// One wake spark (the element of WakeParticles' vector), 0x44 bytes.
class WakeParticle {
public:
    void* data;                        // +0x00
    union {
        struct {
            Vec3_00474760 pos;         // +0x04
            Vec3_00474760 pos2;        // +0x10
            Vec3_00474760 vel;         // +0x1c
        };
        struct {
            char unknown_4[0x6 - 0x4];
            Pos_004745e0 posw;         // +0x06
        };
    };
    int min;                           // +0x28
    int max;                           // +0x2c
    int value;                         // +0x30
    int step;                          // +0x34
    int tick;                          // +0x38
    int period;                        // +0x3c
    int endTime;                       // +0x40, the tick it expires

    void Step();
    void DrawParticle(void* surface, short px, short py);
    int IsExpired(int param_1);
};

// The particle vector seen as its four words, to call its out-of-line insert.
class List_00474880 {
public:
    char* head;                        // +0x00
    char* first;                       // +0x04
    char* last;                        // +0x08
    char* end;                         // +0x0c
    void insert(char* where, int count, const WakeParticle& val);
};

// Vtable 0x4fd5a8, constructor 0x471cc0, destructor 0x471d00, ??_G 0x471cd0.
// The base of the family: the pool's operator new/delete, the three virtual
// slots and the lifetime field at +0x4.
class ParticleSystem {
public:
    int deadline;                                       // +0x4

    ParticleSystem();
    virtual ~ParticleSystem();                          // slot 0
    virtual void Update() = 0;                          // slot 1
    virtual void Render(int) = 0;                       // slot 2
    virtual int IsFinished() = 0;                       // slot 3
    static void* __stdcall operator new(size_t size);   // 0x471d10
    static void __stdcall operator delete(void* p);     // 0x471d50
    void SetLifetime(int ticks);
};

// Vtable 0x4fd588, ??_G 0x471430; 0x44 bytes.
class TeleportParticles : public ParticleSystem {
public:
    int time;                                           // +0x8, the next emit tick
    std::vector<TeleportParticle> items;                // +0xc (_First +0x10)
    int sparkLifetime;                                  // +0x1c, the sparks' lifetime
    Vec3_004736e0 pos1;                                 // +0x20
    Vec3_004736e0 pos2;                                 // +0x2c
    Vec3_004736e0 dir;                                  // +0x38

    TeleportParticles() {}
    virtual void Update();                              // slot 1, 0x472d50
    virtual void Render(int);                           // slot 2, 0x472e30
    virtual int IsFinished();                           // slot 3, 0x472e70
    virtual void Emit();                                // slot 4, 0x4737c0
    virtual int IsEmitDue();                            // slot 5, 0x472e00
    virtual void Init(Vec3_004736e0* a, Vec3_004736e0* b, int c);          // slot 6
};

// Vtable 0x4fd5b8, ??_G 0x471560; 0x4c bytes.
class NanoParticles : public ParticleSystem {
public:
    int time;                                           // +0x8, the next emit tick
    std::vector<NanoParticle> items;                    // +0xc (_First +0x10)
    Seg_00473b50 seg_1c;                                // +0x1c, centre and box
    Seg_00473b50 seg_34;                                // +0x34, target and box

    NanoParticles() {}
    virtual void Update();                              // slot 1, 0x472eb0
    virtual void Render(int);                           // slot 2, 0x472f90
    virtual int IsFinished();                           // slot 3, 0x472fd0
    virtual void Emit();                                // slot 4, 0x473d50
    virtual int IsEmitDue();                            // slot 5, 0x472f60
    virtual void Init(Seg_00473b50* a, Seg_00473b50* b, int c);          // slot 6
};

struct Shape_00472ab0;

// Vtable 0x4fd5d8, ??_G 0x4716a0; 0x44 bytes.
class ThrustParticles : public ParticleSystem {
public:
    int time;                                           // +0x8, the next emit tick
    Vec_004743a0 items;                                 // +0xc (_First +0x10)
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

// Vtable 0x4fd5f8, ??_G 0x4717e0; 0x48 bytes.
class WakeParticles : public ParticleSystem {
public:
    int time;                                           // +0x8, the next emit tick
    std::vector<WakeParticle> items;                    // +0xc (_First +0x10)
    int period;                                         // +0x1c
    Vec3_00474760 pos_a;                                // +0x20
    Vec3_00474760 pos_b;                                // +0x2c
    Vec3_00474760 dir;                                  // +0x38
    int ascending;                                      // +0x44

    WakeParticles() {}
    virtual void Update();                              // slot 1, 0x473170
    virtual void Render(int);                           // slot 2, 0x473250
    virtual int IsFinished();                           // slot 3, 0x473290
    virtual void Emit();                                // slot 4, 0x474880
    virtual int IsEmitDue();                            // slot 5, 0x473220
    virtual void Init(Vec3_00474760* a, Vec3_00474760* b, int param_3,
                              int param_4, int param_5);  // slot 6
};

// The ten per-index lists owned by g_game (see 0x471d90). 0x471160, 0x471820
// and 0x471a50 stay in their files: they need the std::vector<Elem_00473500>
// view of the lists, whose inlined insert cannot agree with this one.
class ParticleLists {
public:
    std::vector<ParticleSystem*> lists[10];             // 0x10 bytes each

    void Add(short index, ParticleSystem* p)
    {
        if (lists[index].size() > 400) {
            delete lists[index][0];
            lists[index].erase(lists[index].begin());
        }
        lists[index].push_back(p);
    }

    ParticleLists();
    ~ParticleLists();
    void UpdateAll();
    void DrawAll(void* param);
    void DrawList(void* param, short index);
    void AddTeleportParticles(int param_1, int param_2, int param_3, short index);
    void AddNanoParticles(int param_1, int param_2, int param_3, short index);
    void AddThrustParticles(int param_1, int param_2, int param_3, int param_4, short index);
    void AddWakeParticles(int param_1, int param_2, int param_3, int param_4,
                          short index, int param_6);
};

class Listener_00471d90 {
public:
    virtual void Slot0();
    virtual void Slot1();
    virtual void Slot2(void* msg);
};

struct Lists_00471d90 {
    std::vector<Listener_00471d90*> lists[10];
};

class Listener_00471eb0 {
public:
    virtual ~Listener_00471eb0();
    virtual void Slot1();              // vtable +0x4
    virtual void Slot2(void* msg);     // vtable +0x8
    virtual int Slot3();               // vtable +0xc
};

struct Lists_00471eb0 {
    std::vector<Listener_00471eb0*> lists[10];
};

class Listener_471f40 {
public:
    virtual void Slot0();
    virtual void Slot1();
    virtual void Slot2(int arg);       // vtable +0x8
};

struct Lists_471f40 {
    std::vector<Listener_471f40*> lists[10];
};

class Listener_00471f90 {
public:
    virtual void Slot0();
    virtual void Slot1();
    virtual void Slot2(void* msg);     // vtable +0x8
};

struct Lists_00471f90 {
    std::vector<Listener_00471f90*> lists[10];
};

struct Lists_00471fd0 {
    std::vector<ParticleSystem*> lists[10];
};

struct Vec3_004720d0 {                 // the 12-byte argument struct
    int x, y, z;
};

struct Ctx_004720d0 {                  // both copies, one 24-byte local
    Vec3_004720d0 a;
    Vec3_004720d0 b;
};

struct Lists_004720d0 {
    std::vector<ParticleSystem*> lists[1];              // 0x10 each
};

#include "../util/vec3.h"

// The 24-byte struct slot 6 (0x473b50) copies both to the object, at +0x1c
// and +0x34; here both halves are initialised from the position argument.
struct Pos_00472200 {
    Vec3 a;
    Vec3 b;
};

class Lists_00472330 {
public:
    std::vector<ParticleSystem*> lists[10];             // 0x10 bytes each

    // Inlined member helper: leaves std::vector::insert out of line.
    void Add(short index, ParticleSystem* p)
    {
        if (lists[index].size() > 400) {
            delete lists[index][0];
            lists[index].erase(lists[index].begin());
        }
        lists[index].push_back(p);
    }
};

class Lists_00472430 {
public:
    std::vector<ParticleSystem*> lists[10];             // 0x10 bytes each

    // Inlined member helper: leaves std::vector::insert out of line.
    void Add(short index, ParticleSystem* p)
    {
        if (lists[index].size() > 400) {
            delete lists[index][0];
            lists[index].erase(lists[index].begin());
        }
        lists[index].push_back(p);
    }
};

struct Lists_00472530 {
    std::vector<ParticleSystem*> lists[10];             // 0x10 bytes each

    // Inlined member helper: leaves std::vector::insert out of line.
    void Add(short index, ParticleSystem* p)
    {
        if (lists[index].size() > 400) {
            delete lists[index][0];
            lists[index].erase(lists[index].begin());
        }
        lists[index].push_back(p);
    }
};

// The smoke and TimedSubParticles systems' point types.
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

struct Vec3_00474d50 {
    int x;
    int y;
    int z;
};

struct Vec3_00475150 {
    int x;
    int y;
    int z;
};

// The smoke puff's 16.16 position halves.
struct Position_00475470 {             // 16.16 fixed point; only high words read
    short xFrac;
    short x;                           // +0x2
    short yFrac;
    short y;                           // +0x6
    short zFrac;
    short z;                           // +0xa
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

struct Rect_004b0510 {
    int x1;                          // +0x0
    int y1;                          // +0x4
    int x2;                          // +0x8
    int y2;                          // +0xc
};

// One smoke puff (the element of SmokeParticles' vector), 0x20 bytes.
struct SmokeParticle {
    void* data;                        // +0x00, the animation
    union {
        Vec3_00474d50 pos;             // +0x04
        Position_00475470 posw;
    };
    int limit;                         // +0x10, the rounds it lives
    int count;                         // +0x14, the rounds so far (the frame)
    int period;                        // +0x18
    int timer;                         // +0x1c

    void Step();
    void DrawParticle(void* dest, short px, short py);
    int IsExpired(int unused);
};

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

// The explored-cell views of the smoke draws.
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
#pragma pack(pop)

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x1b63];
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
    int scrollX;                       // +0x1431f, the map scroll
    int scrollY;                       // +0x14323
    char unknown_14327[0x147cf - 0x14327];
    void* unknown_147cf;               // +0x147cf, the wake animation
    void* unknown_147d3;               // +0x147d3, the other smoke animation
    char unknown_147d7[0x147f3 - 0x147d7];
    union {
        void* unknown_147f3;           // +0x147f3, the spark animation
        unsigned short* bits_147f3;
    };
    char unknown_147f7[0x37ecc - 0x147f7];
    int windX;                         // +0x37ecc
    char unknown_37ed0[4];
    int windZ;                         // +0x37ed4
    char unknown_37ed8[0x38a47 - 0x37ed8];
    union {
        int field_38a47;               // +0x38a47, the tick
        int ticks;
        int time;
        unsigned int now;
    };
    char unknown_38a4b[0x38d77 - 0x38a4b];
    union {
        ParticleLists* lists;          // +0x38d77, the ten per-index lists
        ParticleLists* lists_00472200;
        Lists_00471d90* lists_00471d90;
        Lists_00471eb0* lists_00471eb0;
        Lists_471f40* lists_471f40;
        Lists_00471f90* lists_00471f90;
        Lists_00471fd0* lists_00471fd0;
        Lists_004720d0* lists_004720d0;
        Lists_00472330* lists_00472330;
        Lists_00472430* lists_00472430;
        Lists_00472530* lists_00472530;
    };
};
#pragma pack(pop)

extern Game* g_game;
extern ObjectPool g_particlePool;
extern char g_fxEventPoolBlocked;
extern unsigned char g_particlePoolDestroyed;
extern void __stdcall ExitParticlePool();

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

// Pos_004745e0's arms: the explored byte map of the local player with flags
// bit 1 set, or their bit of the shared visibility mask (bit clear). The two
// arms keep their own fail block, as the original does.
inline int Pos_004745e0::Visible()
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

// The pool allocator of the vector members below, to call their out-of-line
// insert under this name.
class Class_00476210 {
public:
    void insert(std::vector<SmokeParticle>::iterator p, unsigned int m,
                const SmokeParticle& x);
};

class Vec_00476490 {
public:
    void insert(TimedSubParticle* pos, int count, const TimedSubParticle* src);
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

// std::copy for 14-byte elements, compiled with __stdcall as the default
// convention (same shape as 0x4702d0). Its one caller copies one vector's
// elements into another. It is the second std::copy instantiation, so
// data/aliases.csv lists 0x470a40 for std::copy (0x4256a0 is the first).
// FUNCTION: 0x470a40
Elem_00470a40* __stdcall CopyStructRange(Elem_00470a40* first, Elem_00470a40* last, Elem_00470a40* dest)
{
    for (; first != last; ++dest, ++first)
        *dest = *first;
    return dest;
}

// The original calls Construct and Destroy out of line from 0x471c80 and
// 0x471ca0; in this file /Ob2 would inline them.

#pragma auto_inline(off)
// FUNCTION: 0x470a90
ObjectPool* ObjectPool::Construct(int param_1, int param_2)
{
    this->ObjectPool::ObjectPool(param_1, param_2);
    return (ObjectPool*)this;
}
#pragma auto_inline(on)

// The compiler-generated scalar deleting destructor of the class whose
// vtable (one slot) is at 0x4fd580. Its constructor is 0x470a90 and its
// out-of-line destructor 0x470b80. It frees field_14, then frees each item of
// a std::vector while erasing it from the front, then the vector's own
// destructor frees the storage.
//
// The game's instance is the function-local static g_particlePool constructed
// with (1000, 0x4c) at 0x471c80. The static object below must stay: it makes
// the compiler emit the vtable and with it this COMDAT.
// FUNCTION: 0x470ae0 ??_GObjectPool@@UAEPAXI@Z
static ObjectPool s_obj(1000, 0x4c);

// The original calls this out of line from 0x471ca0.

#pragma auto_inline(off)
// FUNCTION: 0x470b80
void ObjectPool::Destroy()
{
    this->ObjectPool::~ObjectPool();
}
#pragma auto_inline(on)

// FUNCTION: 0x470e50
void ObjectPool::FreeBlocks()
{
    if (field_14 != 0) {
        GameFreeThunk(field_14);
    }
    std::vector<Item_00470ae0*>::iterator it = items.begin();
    while (it != items.end()) {
        GameFreeThunk(*it);
        items.erase(it);
    }
}

// The pool's methods are called out of line from the inlined operator new and
// operator delete bodies; in this file /Ob2 would inline them too.

#pragma auto_inline(off)
// FUNCTION: 0x470eb0
int ObjectPool::AllocSlot(int unused)
{
    int edx = field_20;
    int esi = field_1c;
    int eax = 0;

    if (edx < esi) {
        void* ptr = field_14;
        edx++;
        eax = *(int*)((char*)ptr + edx * 4 - 4);
        field_20 = edx;
    }

    return eax;
}

// FUNCTION: 0x470ed0
void ObjectPool::FreeSlot(int param_1)
{
    int eax = field_20 - 1;
    field_20 = eax;
    ((int*)field_14)[eax] = param_1;
}
#pragma auto_inline(on)

// std::vector<Elem_00470f00>::_Destroy(first, last) from the <vector> header:
// empty, since the element type is trivial. Its caller 0x470c10 calls 0x470f30
// (_Ufill), 0x470f00 (_Ucopy) and 0x470ef0 (_Destroy).
typedef std::vector<Elem_00470f00> Vec_00470ef0;
typedef void (Vec_00470ef0::*DestroyFn_00470ef0)(Vec_00470ef0::iterator, Vec_00470ef0::iterator);

// _Destroy is protected: the derived struct takes its address to emit it.
struct Access_00470ef0 : Vec_00470ef0 {
    static DestroyFn_00470ef0 fn;
};

// FUNCTION: 0x470ef0 ?_Destroy@?$vector@UElem_00470f00@@V?$allocator@UElem_00470f00@@@std@@@std@@IAEXPAUElem_00470f00@@0@Z
DestroyFn_00470ef0 Access_00470ef0::fn = &Access_00470ef0::_Destroy;

// std::vector<Elem_00470f00>::_Ucopy(first, last, dest) from the <vector>
// header: copies [first, last) into raw storage at dest and returns the end of
// the copies.
typedef std::vector<Elem_00470f00> Vec_00470f00;
typedef Vec_00470f00::iterator (Vec_00470f00::*UcopyFn_00470f00)(
    Vec_00470f00::const_iterator, Vec_00470f00::const_iterator, Vec_00470f00::iterator);

// _Ucopy is protected: the derived struct takes its address to emit it.
struct Access_00470f00 : Vec_00470f00 {
    static UcopyFn_00470f00 fn;
};

// FUNCTION: 0x470f00 ?_Ucopy@?$vector@UElem_00470f00@@V?$allocator@UElem_00470f00@@@std@@@std@@IAEPAUElem_00470f00@@PBU3@0PAU3@@Z
UcopyFn_00470f00 Access_00470f00::fn = &Access_00470f00::_Ucopy;

// std::vector<Elem_00470f00>::_Ufill(first, n, value) from the <vector> header:
// copy-constructs n copies of value into raw storage at first.
typedef std::vector<Elem_00470f00> Vec_00470f30;
typedef void (Vec_00470f30::*UfillFn_00470f30)(
    Vec_00470f30::iterator, Vec_00470f30::size_type, const Elem_00470f00&);

// _Ufill is protected: the derived struct takes its address to emit it.
struct Access_00470f30 : Vec_00470f30 {
    static UfillFn_00470f30 fn;
};

// FUNCTION: 0x470f30 ?_Ufill@?$vector@UElem_00470f00@@V?$allocator@UElem_00470f00@@@std@@@std@@IAEXPAUElem_00470f00@@IABU3@@Z
UfillFn_00470f30 Access_00470f30::fn = &Access_00470f30::_Ufill;

// FUNCTION: 0x470f60
void __stdcall CopyPointer(int* param_1, int* param_2)
{
    if (param_1 != 0) {
        *param_1 = *param_2;
    }
}

// Out-of-line constructor of the ten lists.
// FUNCTION: 0x470f80
ParticleLists::ParticleLists()
{
}

// Destructor of the ten lists 0x471d90 allocates into the game
// object (used by 0x471f40 and 0x471f90): deletes every particle system, erasing it
// from the front of its list, then the vector members are destroyed.
// FUNCTION: 0x470fb0
ParticleLists::~ParticleLists()
{
    for (int i = 0; i < 10; i++) {
        std::vector<ParticleSystem*>::iterator it = lists[i].begin();
        while (it != lists[i].end()) {
            delete *it;
            lists[i].erase(it);
        }
    }
}

// Updates the ten lists: a particle system whose slot-3 check says it is
// finished is deleted and erased, every other one gets its slot-1
// call.
// FUNCTION: 0x471050
void ParticleLists::UpdateAll()
{
    for (int i = 0; i < 10; i++) {
        std::vector<ParticleSystem*>::iterator it = lists[i].begin();
        while (it != lists[i].end()) {
            ParticleSystem* l = *it;
            if (l->IsFinished()) {
                delete l;
                lists[i].erase(it);
            } else {
                l->Update();
                it++;
            }
        }
    }
}

// Passes param to slot 2 (the draw) of every particle system in the ten lists.
// FUNCTION: 0x4710e0
void ParticleLists::DrawAll(void* param)
{
    for (int i = 0; i < 10; i++) {
        for (std::vector<ParticleSystem*>::iterator it = lists[i].begin(); it != lists[i].end(); it++)
            (*it)->Render((int)param);
    }
}

// Passes param to slot 2 (the draw) of every particle system in one list.
// FUNCTION: 0x471120
void ParticleLists::DrawList(void* param, short index)
{
    std::vector<ParticleSystem*>* s = &lists[index];
    for (std::vector<ParticleSystem*>::iterator p = s->begin(); p != s->end(); p++)
        (*p)->Render((int)param);
}

// Creates a TeleportParticles (vtable 0x4fd588), initialises it through virtual
// slot 6, then appends it to the std::vector<ParticleSystem*> selected by the
// short index. When that list already holds more than 400 entries its oldest
// element is deleted and erased first.
// FUNCTION: 0x471340
void ParticleLists::AddTeleportParticles(int param_1, int param_2, int param_3, short index)
{
    TeleportParticles* p = new TeleportParticles;
    if (p) {
        p->Init((Vec3_004736e0*)param_1, (Vec3_004736e0*)param_2, param_3);
        Add(index, p);
    }
}

// The compiler-generated scalar deleting destructor of TeleportParticles.
// The global below exists only to make the compiler emit the vtable and this COMDAT.
// FUNCTION: 0x471430 ??_GTeleportParticles@@UAEPAXI@Z
static TeleportParticles* s_teleport = new TeleportParticles;

// Creates a NanoParticles (vtable 0x4fd5b8), initialises it through virtual
// slot 6, then appends it to the std::vector<ParticleSystem*> selected by the
// short index. When that list already holds more than 400 entries its oldest
// element is deleted and erased first.
// FUNCTION: 0x471470
void ParticleLists::AddNanoParticles(int param_1, int param_2, int param_3, short index)
{
    NanoParticles* p = new NanoParticles;
    if (p) {
        p->Init((Seg_00473b50*)param_1, (Seg_00473b50*)param_2, param_3);
        Add(index, p);
    }
}

// The compiler-generated scalar deleting destructor of NanoParticles.
// The global below exists only to make the compiler emit the vtable and this COMDAT.
// FUNCTION: 0x471560 ??_GNanoParticles@@UAEPAXI@Z
static NanoParticles* s_nano = new NanoParticles;

// Creates a ThrustParticles (vtable 0x4fd5d8), initialises it through virtual
// slot 6, then appends it to the std::vector<ParticleSystem*> selected by the
// short index. When that list already holds more than 400 entries its oldest
// element is deleted and erased first.
//
// Signature: four ints forwarded to virtual slot 6, then the short index.
// FUNCTION: 0x4715a0
void ParticleLists::AddThrustParticles(int param_1, int param_2, int param_3, int param_4, short index)
{
    ThrustParticles* p = new ThrustParticles;
    if (p) {
        p->Init((Shape_00472ab0*)param_1, (Shape_00472ab0*)param_2, param_3, param_4);
        Add(index, p);
    }
}

// The compiler-generated scalar deleting destructor of ThrustParticles.
// Must stay: it makes the compiler emit the vtable and with it this COMDAT.
// FUNCTION: 0x4716a0 ??_GThrustParticles@@UAEPAXI@Z
static ThrustParticles* s_thrust = new ThrustParticles;

// Creates a WakeParticles (vtable 0x4fd5f8), initialises it through virtual
// slot 6 with five arguments, then appends it to the
// std::vector<ParticleSystem*> selected by the short index. When that list
// already holds more than 400 entries its oldest element is deleted and erased
// first.
// FUNCTION: 0x4716e0
void ParticleLists::AddWakeParticles(int param_1, int param_2, int param_3,
                                  int param_4, short index, int param_6)
{
    WakeParticles* p = new WakeParticles;
    if (p) {
        p->Init((Vec3_00474760*)param_1, (Vec3_00474760*)param_2, param_3, param_4, param_6);
        Add(index, p);
    }
}

// The compiler-generated scalar deleting destructor of WakeParticles.
// The global below exists only to make the compiler emit the vtable and this COMDAT.
// FUNCTION: 0x4717e0 ??_GWakeParticles@@UAEPAXI@Z
static WakeParticles* s_wake = new WakeParticles;

// FUNCTION: 0x471c80
void InitParticlePool()
{
    g_particlePool.Construct(0x3e8, 0x4c);
    atexit((void (__cdecl*)(void))ExitParticlePool);
}

// FUNCTION: 0x471ca0
void ExitParticlePool()
{
    if ((g_particlePoolDestroyed & 1) == 0) {
        g_particlePoolDestroyed |= 1;
        g_particlePool.Destroy();
    }
}

// Constructor of ParticleSystem, the base of a family of objects allocated
// from the pool g_particlePool through the class's own operator new (0x471d10)
// and operator delete (0x471d50). The base vtable 0x4fd5a8 holds the virtual
// destructor and three pure virtuals; every derived class overrides those
// three and adds three more of its own (slot 6 initialises the object and
// calls 0x471d70, which sets deadline).
//
//   class           vtable    constructor  ??_G      slots 1-6
//   ParticleSystem  0x4fd5a8  0x471cc0     0x471cd0  _purecall x3
//   TeleportParticles  0x4fd588  inline       0x471430  0x472d50 0x472e30 0x472e70 0x4737c0 0x472e00 0x4736e0
//   NanoParticles  0x4fd5b8  inline       0x471560  0x472eb0 0x472f90 0x472fd0 0x473d50 0x472f60 0x473b50
//   ThrustParticles  0x4fd5d8  inline       0x4716a0  0x473010 0x4730f0 0x473130 0x4743a0 0x4730c0 0x4742c0
//   WakeParticles  0x4fd5f8  inline       0x4717e0  0x473170 0x473250 0x473290 0x474880 0x473220 0x474760
//   SmokeParticles  0x4fd618  0x474cd0     0x474d10  0x475340 0x475470 0x474f80 0x474df0 0x475440 0x474d50
//   TimedSubParticles  0x4fd638  0x4750b0     0x475110  0x475600 0x475700 0x475330 0x4751c0 0x4750f0 0x475150
//
// The base destructor is 0x471d00. An override keeps the name of the base
// slot it overrides, so slots 1-3 of every derived class carry the names of
// TeleportParticles's (0x472d50, 0x472e30, 0x472e70).
// FUNCTION: 0x471cc0
// FUNCTION: 0x471cd0 ??_GParticleSystem@@UAEPAXI@Z
ParticleSystem::ParticleSystem()
{
    deadline = 0;
}

// The out-of-line destructor of ParticleSystem (the family is listed at
// the constructor): an empty body, so only the vtable store is left.
// FUNCTION: 0x471d00
ParticleSystem::~ParticleSystem()
{
}

// The class-specific operator new of ParticleSystem (the family is listed
// at the constructor): takes a zeroed object from the pool g_particlePool, or
// returns null while g_fxEventPoolBlocked is set. Its operator delete is 0x471d50.
// FUNCTION: 0x471d10
void* __stdcall ParticleSystem::operator new(size_t size)
{
    if (g_fxEventPoolBlocked)
        return 0;
    void* p = (void*)g_particlePool.AllocSlot(size);
    if (p)
        memset(p, 0, size);
    return p;
}

// The class-specific operator delete of ParticleSystem (the family is listed
// at the constructor): returns the object to the pool g_particlePool.
// FUNCTION: 0x471d50
void __stdcall ParticleSystem::operator delete(void* p)
{
    g_particlePool.FreeSlot((int)p);
}

// SetLifetime, called out of line from slot 6 of the derived classes below.

#pragma auto_inline(off)
// FUNCTION: 0x471d70
void ParticleSystem::SetLifetime(int ticks)
{
    deadline = g_game->time + ticks;
}
#pragma auto_inline(on)

// Creates the ten listener lists used by 0x471f40 and 0x471f90.
// FUNCTION: 0x471d90
void CreateParticleLists()
{
    g_game->lists_00471d90 = new Lists_00471d90;
}

// Walks the ten listener lists (see 0x471d90, 0x471f40): a listener whose
// slot 3 returns nonzero is deleted and erased from its list; the others get
// slot 1 called.
// FUNCTION: 0x471eb0
void UpdateParticles()
{
    Lists_00471eb0* l = g_game->lists_00471eb0;
    for (int i = 0; i < 10; i++) {
        std::vector<Listener_00471eb0*>& v = l->lists[i];
        std::vector<Listener_00471eb0*>::iterator it = v.begin();
        while (it != v.end()) {
            Listener_00471eb0* p = *it;
            if (p->Slot3()) {
                delete p;
                v.erase(it);
            } else {
                p->Slot1();
                ++it;
            }
        }
    }
}

// FUNCTION: 0x471f40
void __stdcall DrawParticles(int arg)
{
    Lists_471f40* l = g_game->lists_471f40;
    for (int i = 0; i < 10; i++) {
        for (std::vector<Listener_471f40*>::iterator it = l->lists[i].begin(); it != l->lists[i].end(); ++it) {
            (*it)->Slot2(arg);
        }
    }
}

// Passes msg to every listener in one list (compare 0x471f40, which does
// all ten).
// FUNCTION: 0x471f90
void __stdcall DrawParticleList(void* msg, short kind)
{
    std::vector<Listener_00471f90*>& v = g_game->lists_00471f90->lists[kind];
    for (std::vector<Listener_00471f90*>::iterator it = v.begin(); it != v.end(); ++it) {
        (*it)->Slot2(msg);
    }
}

// Appends to one list, dropping its oldest entry once it holds 400 or more.
// Separate helper: leaves std::vector::insert out of line.
static void __stdcall Add(Lists_00471fd0* l, short index, ParticleSystem* p)
{
    if (l->lists[index].size() > 400) {
        delete l->lists[index][0];
        l->lists[index].erase(l->lists[index].begin());
    }
    l->lists[index].push_back(p);
}

// The same shape as 0x471340 (ParticleLists::AddTeleportParticles), but a free
// function: the owner of the ten lists comes from g_game->lists (+0x38d77)
// instead of `this`. The list is picked by the fourth argument, a short; the
// first three go to the virtual slot 6 initialiser (0x4736e0).
// FUNCTION: 0x471fd0
void __stdcall EmitTeleportParticles(int param_1, int param_2, int param_3, short index)
{
    Lists_00471fd0* l = g_game->lists_00471fd0;
    TeleportParticles* p = new TeleportParticles;
    if (p) {
        p->Init((Vec3_004736e0*)param_1, (Vec3_004736e0*)param_2, param_3);
        Add(l, index, p);
    }
}

// Separate helper: leaves std::vector::insert out of line.
static void Add_004720d0(Lists_004720d0* lists, short index, ParticleSystem* p)
{
    if (lists->lists[index].size() > 400) {
        delete lists->lists[index][0];
        lists->lists[index].erase(lists->lists[index].begin());
    }
    lists->lists[index].push_back(p);
}

// Creates a NanoParticles (vtable 0x4fd5b8) from a 12-byte argument struct,
// initialises it through virtual slot 6 (0x473b50), then appends it to the
// std::vector<ParticleSystem*> that g_game->lists[index] selects. When that
// list already holds more than 400 entries its oldest element is deleted and
// erased first. Same shape as the siblings 0x471340 and 0x4716e0, except that
// the per-index lists come from the game state at g_game + 0x38d77 instead of
// from a `this` pointer, so this is a __stdcall free function of three
// arguments: a pointer to the struct, a second pointer passed straight to slot
// 6, and the list index. Class family listed in 0x471cc0.cpp.
//
// The 12-byte argument is copied twice into one 24-byte local and only the
// first copy is read (its address is slot 6's first argument).
// FUNCTION: 0x4720d0
void __stdcall EmitNanoParticles(Vec3_004720d0* p, void* param_2, short index)
{
    // A struct whose address escapes: keeps the dead second copy's stores.
    Ctx_004720d0 ctx;
    ctx.a = *p;
    ctx.b = *p;
    Lists_004720d0* lists = g_game->lists_004720d0;
    NanoParticles* q = new NanoParticles;
    if (q) {
        q->Init((Seg_00473b50*)&ctx.a, (Seg_00473b50*)param_2, 1);
        Add_004720d0(lists, index, q);
    }
}

// Creates a NanoParticles (vtable 0x4fd5b8), initialises it through virtual
// slot 6 with a 24-byte struct built from the position argument, then appends
// it to the std::vector<ParticleSystem*> selected by the short index. When that
// list already holds more than 400 entries its oldest element is deleted and
// erased first. Same shape as 0x471470, but a free function that reads the list
// owner out of g_game.
// FUNCTION: 0x472200
void __stdcall EmitReverseNanoParticles(Pos_00472200* param_1, Vec3* param_2, short param_3)
{
    Pos_00472200 pos;
    pos.a = *param_2;
    pos.b = *param_2;
    ParticleLists* lists = g_game->lists_00472200;
    NanoParticles* p = new NanoParticles;
    if (p) {
        p->Init((Seg_00473b50*)param_1, (Seg_00473b50*)&pos, 1);
        lists->Add(param_3, p);
    }
}

// Creates a ThrustParticles (vtable 0x4fd5d8), initialises it through virtual
// slot 6 (0x4742c0) with the first four arguments, then appends it to the
// std::vector<ParticleSystem*> selected by the short index in the ten
// per-index lists at g_game->lists (created by 0x471d90, walked by 0x471eb0,
// 0x471f40 and 0x471f90). When that list already holds more than 400 entries
// its oldest element is deleted and erased first.
// Class family listed in 0x471cc0.cpp. Same shape as particles_470f80.cpp and
// 0x472430.cpp.
// FUNCTION: 0x472330
void __stdcall EmitThrustParticles(int param_1, int param_2, int param_3, int param_4,
                            short index)
{
    // Bound to a local at the top: g_game is loaded before the prologue pushes.
    Lists_00472330* lists = g_game->lists_00472330;
    ThrustParticles* p = new ThrustParticles;
    if (p) {
        p->Init((Shape_00472ab0*)param_1, (Shape_00472ab0*)param_2, param_3, param_4);
        lists->Add(index, p);
    }
}

// Creates a WakeParticles (vtable 0x4fd5f8), initialises it through virtual
// slot 6, then appends it to the std::vector<ParticleSystem*> selected by the
// short index in the ten per-index lists at g_game->lists (created by 0x471d90,
// walked by 0x471eb0, 0x471f40 and 0x471f90). When that list already holds
// more than 400 entries its oldest element is deleted and erased first.
// Class family listed in 0x471cc0.cpp.
// FUNCTION: 0x472430
void __stdcall EmitWakeParticles(int param_1, int param_2, int param_3, short index)
{
    Lists_00472430* l = g_game->lists_00472430;
    WakeParticles* p = new WakeParticles;
    if (p) {
        p->Init((Vec3_00474760*)param_1, (Vec3_00474760*)param_2, param_3, 1, 1);
        l->Add(index, p);
    }
}

// Creates a WakeParticles (vtable 0x4fd5f8, 0x48 bytes), initialises it
// through virtual slot 6, then appends it to the std::vector<ParticleSystem*>
// selected by the short index. When that list already holds more than 400
// entries its oldest element is deleted and erased first. Same shape as
// 0x471340, which builds a TeleportParticles instead; 0x471340 is a method on
// the lists owner, here the lists pointer is a local.
// Class family listed in wake_particles.cpp.
// FUNCTION: 0x472530
void __stdcall EmitBubbles(int param_1, int param_2, int param_3, short index)
{
    // Local read before the new: otherwise g_game is reloaded after the virtual call.
    Lists_00472530* l = g_game->lists_00472530;
    WakeParticles* p = new WakeParticles;
    if (p) {
        p->Init((Vec3_00474760*)param_1, (Vec3_00474760*)param_2, param_3, 1, 0);
        l->Add(index, p);
    }
}

// FUNCTION: 0x472d30 ?size@?$vector@UElem_00473500@@V?$allocator@UElem_00473500@@@std@@@std@@QBEIXZ
SizeFn_00473500 g_size_00472d30 = &Vec_00473500::size;

// Steps every element of the vector, dropping the ones the element's own test
// rejects (erase in place, so the element now at the cursor is tested again),
// then hands over to the virtual at +0x14, which asks the one at +0x10 to
// rebuild. Identical in shape to 0x473170, the same slot of the sibling class.
// FUNCTION: 0x472d50
void TeleportParticles::Update()
{
    std::vector<TeleportParticle>::iterator it = items.begin();

    while (it != items.end()) {
        it->Step();
        if (it->IsExpired(g_game->field_38a47)) {
            items.erase(it);
        } else {
            ++it;
        }
    }
    if (IsEmitDue()) {
        Emit();
    }
}

// Slot 5: whether it is time to emit again.
// FUNCTION: 0x472e00
int TeleportParticles::IsEmitDue()
{
    if (time <= deadline) {
        unsigned int game_val = g_game->field_38a47;
        if ((unsigned int)time <= game_val) {
            return 1;
        }
    }
    return 0;
}

// Calls 0x473590 on every element of the std::vector of 52-byte elements
// whose emptiness 0x472e70 tests.
// FUNCTION: 0x472e30
void TeleportParticles::Render(int p)
{
    for (std::vector<TeleportParticle>::iterator it = items.begin(); it != items.end(); ++it) {
        it->DrawParticle((void*)p, g_game->scrollX, g_game->scrollY);
    }
}

// Returns whether the particle vector is empty.
// FUNCTION: 0x472e70
int TeleportParticles::IsFinished()
{
    return items.empty();
}

// Slot 1: steps every item in the std::vector at +0xc, drops the ones whose
// endTime is below the current game tick, then asks the two virtuals at +0x14
// and +0x10 whether the container needs a rebuild. Sibling of 0x472d50, which
// only differs in the element size (0x34) and its two callees.
// FUNCTION: 0x472eb0
void NanoParticles::Update()
{
    std::vector<NanoParticle>::iterator it = items.begin();
    while (it != items.end()) {
        it->Step();
        if (it->IsExpired(g_game->ticks))
            it = items.erase(it);
        else
            ++it;
    }
    if (IsEmitDue())
        Emit();
}

// Slot 5: whether it is time to emit again.
// FUNCTION: 0x472f60
int NanoParticles::IsEmitDue()
{
    if (time <= deadline) {
        unsigned int game_val = g_game->ticks;
        if ((unsigned int)time <= game_val) {
            return 1;
        }
    }
    return 0;
}

// Slot 2: calls DrawParticle on every particle, passing the map scroll
// position as shorts.
// FUNCTION: 0x472f90
void NanoParticles::Render(int param_1)
{
    for (std::vector<NanoParticle>::iterator it = items.begin(); it != items.end(); ++it)
        it->DrawParticle(param_1, g_game->scrollX, g_game->scrollY);
}

// Slot 3: whether there are no particles.
// FUNCTION: 0x472fd0
int NanoParticles::IsFinished()
{
    return items.empty();
}

// Slot 1, Update: walks the std::vector of 0x3c-byte
// elements at +0xc, advances each one with Step, and erases every
// element IsExpired reports as expired, then, when virtual slot 5
// (0x4730c0) is true, calls virtual slot 4 (0x4743a0).
// FUNCTION: 0x473010
void ThrustParticles::Update()
{
    for (std::vector<ThrustParticle>::iterator it = items.begin(); it != items.end(); ) {
        it->Step();
        if (it->IsExpired(g_game->ticks))
            items.erase(it);
        else
            ++it;
    }
    if (IsEmitDue())
        Emit();
}

// Slot 5: whether it is time to emit again.
// FUNCTION: 0x4730c0
int ThrustParticles::IsEmitDue()
{
    if (time <= deadline) {
        unsigned int val = g_game->ticks;
        if ((unsigned int)time <= val) {
            return 1;
        }
    }
    return 0;
}

// Slot 2: calls DrawParticle on every particle with the argument and the map
// scroll position.
// FUNCTION: 0x4730f0
void ThrustParticles::Render(int param_1)
{
    for (std::vector<ThrustParticle>::iterator it = items.begin(); it != items.end(); ++it) {
        it->DrawParticle((void*)param_1, g_game->scrollX, g_game->scrollY);
    }
}

// Slot 3: whether there are no particles.
// FUNCTION: 0x473130
int ThrustParticles::IsFinished()
{
    return items.empty();
}

// Slot 1: steps every particle, dropping the ones the particle's own test
// rejects (erase in place, so the one now at the cursor is tested again),
// then, when slot 5 says so, calls slot 4 to emit more.
// FUNCTION: 0x473170
void WakeParticles::Update()
{
    std::vector<WakeParticle>::iterator it = items.begin();

    while (it != items.end()) {
        it->Step();
        if (it->IsExpired(g_game->ticks)) {
            items.erase(it);
        } else {
            ++it;
        }
    }
    if (IsEmitDue()) {
        Emit();
    }
}

// Slot 5: whether it is time to emit again.
// FUNCTION: 0x473220
int WakeParticles::IsEmitDue()
{
    if (time <= deadline && time <= g_game->now) {
        return 1;
    }
    return 0;
}

// Slot 2: calls DrawParticle on every particle with the argument and the map
// scroll position.
// FUNCTION: 0x473250
void WakeParticles::Render(int param_1)
{
    for (std::vector<WakeParticle>::iterator it = items.begin(); it != items.end(); ++it) {
        it->DrawParticle((void*)param_1, g_game->scrollX, g_game->scrollY);
    }
}

// Slot 3: whether there are no particles.
// FUNCTION: 0x473290
int WakeParticles::IsFinished()
{
    return items.empty();
}

// std::vector<Elem_00473500>::_Destroy(first, last) from MSVC 5's <vector>:
// empty, since the element type is trivial.
// FUNCTION: 0x4732d0 ?_Destroy@?$vector@UElem_00473500@@V?$allocator@UElem_00473500@@@std@@@std@@IAEXPAUElem_00473500@@0@Z
DestroyFn_00473500 Access_00473500::destroy = &Access_00473500::_Destroy;

// std::vector<Elem_00473500>::_Ucopy(first, last, dest): copies [first, last)
// into raw storage at dest and returns the end of the copies. The element type
// is a guess: any 4-byte trivially copyable type compiles to the same code.
// FUNCTION: 0x473500 ?_Ucopy@?$vector@UElem_00473500@@V?$allocator@UElem_00473500@@@std@@@std@@IAEPAUElem_00473500@@PBU3@0PAU3@@Z
UcopyFn_00473500 Access_00473500::ucopy = &Access_00473500::_Ucopy;

// std::vector<Elem_00473500>::_Ufill(first, n, value) from MSVC 5's <vector>:
// copy-constructs n copies of value into raw storage at first.
// FUNCTION: 0x473530 ?_Ufill@?$vector@UElem_00473500@@V?$allocator@UElem_00473500@@@std@@@std@@IAEXPAUElem_00473500@@IABU3@@Z
UfillFn_00473500 Access_00473500::ufill = &Access_00473500::_Ufill;

// Moves the position by its velocity (an inlined Vec3 operator+=) and steps
// the frame.
// The original calls this out of line from 0x472d50; in this file /Ob2 would inline it.
#pragma auto_inline(off)
// FUNCTION: 0x473560
void TeleportParticle::Step()
{
    pos1 += dir;
    frame = (frame + 1) % frameCount;
}
#pragma auto_inline(on)

// FUNCTION: 0x473590
void TeleportParticle::DrawParticle(void* dest, short px, short py)
{
    Pos_00473590* q = &posw;
    short sx = q->x - px + 0x80;
    short sy = q->y - (q->h >> 1) - py + 0x20;
    // Two locals with the same value. The original reads the map width twice
    // per arm, once for the bounds test and once for the index, and a single
    // pointer makes MSVC 5 fold one of the two away.
    Player_00473590* p = &g_game->players_00473590[g_game->playerIndex];
    Player_00473590* p2 = &g_game->players_00473590[g_game->playerIndex];
    int visible;
    if ((g_game->mapFlags & 2) == 2) {
        int col = posw.x >> 5;
        int row = (posw.y - (posw.h >> 1)) >> 5;
        // Local free on purpose: the width is re-read and the fog map pointer
        // folds into the add, which is what the original does.
        if (p->size.Contains(col, row) &&
            p->seen[p2->size.width * row + col] != 0)
            visible = 1;
        else
            visible = 0;
    } else {
        int col = posw.x >> 5;
        int row = (posw.y - (posw.h >> 1)) >> 5;
        visible = Identity_00473590(IsSeen_00473590(p, p2, col, row));
    }
    if (visible)
        DrawFrameBlended(dest, GetGafFrame(data, frame), sx, sy);
}

// Whether the tick has passed the spark's expiry.
// The original calls this out of line from 0x472d50; in this file /Ob2 would inline it.
#pragma auto_inline(off)
// FUNCTION: 0x4736c0
int TeleportParticle::IsExpired(int param_1)
{
    return param_1 > endTime;
}
#pragma auto_inline(on)

// Sibling of 0x4742c0: the same base call, the same two position copies, the
// same difference of the two and the same trailing virtual call, but this one
// divides the difference by how many 327680-unit segments its length holds
// instead of multiplying it by a reciprocal id.
// FUNCTION: 0x4736e0
void TeleportParticles::Init(Vec3_004736e0* a, Vec3_004736e0* b, int c)
{
    SetLifetime(c);
    pos1 = *a;
    pos2 = *b;
    dir = pos2 - pos1;
    int dist = dir.Length();
    // The quotient goes through a union so it sits in memory; the high half is read back.
    Fix_004736e0 scale;
    scale.whole = (int)(((__int64)dist << 16) / 327680);
    int step = scale.half[1];
    sparkLifetime = step;
    dir.x /= step;
    dir.y /= step;
    dir.z /= step;
    Emit();
}

// Slot 4. It first makes room for however many ten-tick
// units lie between the current tick and deadline with items.reserve(...),
// then appends one element built from the three positions plus a random value,
// and finally sets time ten ticks ahead of the current tick.
// FUNCTION: 0x4737c0
void TeleportParticles::Emit()
{
    int grow = (deadline - g_game->field_38a47 + 10) / 10;

    // The inlined vector::reserve itself is needed, not a hand-written block.
    if (grow > 0)
        items.reserve(grow + items.size());

    for (int i = 0; i < 1; i++) {
        TeleportParticle e;

        e.pos1 = pos1;
        e.pos2 = pos2;
        e.dir = dir;
        e.endTime = g_game->field_38a47 + sparkLifetime;
        e.data = g_game->unknown_147f3;
        e.frameCount = GetGafFrameCount(g_game->unknown_147f3) - 1;
        e.frame = (int)(((__int64)rand() * e.frameCount) / 0x8000);
        // Not push_back: calling through the List layout keeps the insert out of line.
        List_004737c0* v = (List_004737c0*)&items;
        v->insert(v->last, 1, e);
    }

    time = g_game->field_38a47 + 10;
}

// Advances the position by its velocity and steps a 1..7 animation counter
// kept in the low four bits of the flags word.
// The original calls this out of line from 0x472eb0; in this file /Ob2 would inline it.
#pragma auto_inline(off)
// FUNCTION: 0x4739b0
void NanoParticle::Step()
{
    short frame = (flags & 0xf) + 1;
    pos += vel;
    if (frame > 7)
        frame = 1;
    flags = (flags & 0xfff0) + frame;
}
#pragma auto_inline(on)

// Whether the tick has passed the spark's expiry.
// The original calls this out of line from 0x472eb0; in this file /Ob2 would inline it.
#pragma auto_inline(off)
// FUNCTION: 0x473b30
int NanoParticle::IsExpired(int value)
{
    return value > endTime;
}
#pragma auto_inline(on)

// Slot 6: takes the two segments and keeps the middle 3/11 of each.
// FUNCTION: 0x473b50
void NanoParticles::Init(Seg_00473b50* a, Seg_00473b50* b, int c)
{
    SetLifetime(c);
    seg_1c = *a;
    seg_34 = *b;
    SPLIT_SEG(seg_34);
    SPLIT_SEG(seg_1c);
    Emit();
}

// The three adds are an inlined vector operator+=.
// The original calls this out of line from 0x473010; in this file /Ob2 would inline it.
#pragma auto_inline(off)
// FUNCTION: 0x474130
void ThrustParticle::Step()
{
    pos0 += pos2;
    tick = (tick + 1) % period;
    if (tick == 0) {
        frame = (frame + 1) % frameCount;
    }
}
#pragma auto_inline(on)

// FUNCTION: 0x474170
void ThrustParticle::DrawParticle(void* dest, short px, short py)
{
    Pos_00473590* q = &posw;
    short sx = q->x - px + 0x80;
    short sy = q->y - (q->h >> 1) - py + 0x20;
    Player_00473590* p = &g_game->players_00473590[g_game->playerIndex];
    Player_00473590* p2 = &g_game->players_00473590[g_game->playerIndex];
    int visible;
    if ((g_game->mapFlags & 2) == 2) {
        int col = posw.x >> 5;
        int row = (posw.y - (posw.h >> 1)) >> 5;
        if (p->size.Contains(col, row) &&
            p->seen[p2->size.width * row + col] != 0)
            visible = 1;
        else
            visible = 0;
    } else {
        int col = posw.x >> 5;
        int row = (posw.y - (posw.h >> 1)) >> 5;
        visible = Identity_00474170(IsSeen_00474170(p, p2, col, row));
    }
    if (visible)
        DrawFrameBlended(dest, GetGafFrame(bitmask, frame), sx, sy);
}

// Whether the tick has passed the puff's expiry.
// The original calls this out of line from 0x473010; in this file /Ob2 would inline it.
#pragma auto_inline(off)
// FUNCTION: 0x4742a0
int ThrustParticle::IsExpired(int value)
{
    return value > endTime;
}
#pragma auto_inline(on)

// Slot 6 of ThrustParticles: keeps the two points it is given, puts their
// difference in a third one and scales that by the 16.16 reciprocal of the id
// ((1 << 32) / (id << 16) is 65536 / id), so the offset ends up divided by it.
// FUNCTION: 0x4742c0
void ThrustParticles::Init(Vec3_004742c0* p, Vec3_004742c0* q, int a, int b)
{
    this->SetLifetime(b);
    period = a;
    pos0 = *p;
    pos1 = *q;
    pos2 = pos1 - pos0;
    int scale = (int)(((__int64)1 << 32) / (b << 16));
    pos2.Scale(scale);
    Emit();
}

// Slot 4 (same 0x3c-byte record family as 0x474df0 and
// 0x4751c0): reserves room for the frames between g_game->frame and deadline,
// then appends one record holding the three positions at +0x20/+0x2c/+0x38,
// a pointer out of the game structure and deadline, and pushes the clock to
// g_game->frame + 1.
//
// The record has no leading image pointer: bitmask sits at +0x00, so the three
// positions land at +0x04/+0x10/+0x1c and deadline is the record's last dword
// at +0x38, inside the 0x3c bytes.
// FUNCTION: 0x4743a0
void ThrustParticles::Emit()
{
    int extra = deadline - g_game->ticks + 1;
    if (extra > 0) {
        items.reserve(extra + items.size());
    }
    Vec3_004742c0* p = &pos0;
    Vec_004743a0* v = &items;
    for (int i = 1; i != 0; i--) {
        ThrustParticle rec;
        rec.period = period;
        rec.endTime = deadline;
        rec.tick = 0;
        rec.pos0 = *p;
        rec.pos1 = pos1;
        rec.pos2 = pos2;
        rec.bitmask = g_game->bits_147f3;
        rec.frameCount = (int)GetGafFrameCount(g_game->unknown_147f3) - 1;
        rec.frame = 0;
        ((Class_00475bd0*)v)->insert(v->end(), 1, rec);
    }
    time = g_game->ticks + 1;
}

// Moves by the velocity, then every `period` ticks steps a value that wraps
// between min and max.
// The original calls this out of line from 0x473170; in this file /Ob2 would inline it.
#pragma auto_inline(off)
// FUNCTION: 0x474580
void WakeParticle::Step()
{
    pos += vel;
    tick = (tick + 1) % period;
    if (tick == 0) {
        value += step;
        if (value > max) value = min;
        if (value < min) value = max;
    }
}
#pragma auto_inline(on)

// The original calls this out of line from 0x473250; in this file /Ob2 would inline it.
#pragma auto_inline(off)
// FUNCTION: 0x4745e0
void WakeParticle::DrawParticle(void* surface, short px, short py)
{
    Rect_004b0510 r;
    short sx = posw.x - px;
    short sy = posw.y - py;
    r.x1 = sx + 0x80;
    r.y1 = sy - (posw.height >> 1) + 0x20;
    r.x2 = r.x1 + 1;
    r.y2 = r.y1 + 1;
    // Visible() stays a member: inlined here, the arms' pos loads are CSE'd
    // against the header's.
    if (posw.Visible())
        FillRectangle(surface, &r, value);
}
#pragma auto_inline(on)

// Expired once the tick has passed endTime or the spark is above the sea
// (its ground height not below the sea level).
// The original calls this out of line from 0x473170; in this file /Ob2 would inline it.
#pragma auto_inline(off)
// FUNCTION: 0x474720
int WakeParticle::IsExpired(int param_1)
{
    if (param_1 <= endTime) {
        int r = GetGroundHeight(&pos);
        if (r < g_game->seaLevel)
            return 0;
    }
    return 1;
}
#pragma auto_inline(on)

// Slot 6: stores the two points it is given, the vector between them scaled to
// a length of 32768 (2^31/len as 16.16), and the two arguments it passes on,
// then updates itself.
// FUNCTION: 0x474760
void WakeParticles::Init(Vec3_00474760* a, Vec3_00474760* b, int param_3,
                                  int param_4, int param_5)
{
    SetLifetime(param_4);
    period = param_3;
    pos_a = *a;
    pos_b = *b;
    // The difference must come from an inline operator- returning the struct by value.
    dir = pos_b - pos_a;
    // The divisor is twice the length, with no test for zero: two identical
    // points raise a divide exception here (0x474811, _alldiv).
    int scale = 0x100000000 / (int)(((__int64)dir.Length() * 0x20000) >> 16);
    dir.Scale(scale);
    ascending = param_5;
    Emit();
}

// Slot 4: makes room in the std::vector at +0xc for one 68-byte
// element per tick up to deadline (the period is 1), then appends
// one element built from the object's three points, with the first point
// jittered by up to 3 units on each axis, and finally sets time one tick
// ahead of the current tick.
// The element is a 12-byte point made of three 16.16 fixed-point pairs of
// shorts, so the jitter is three `+=` on the high half.
// FUNCTION: 0x474880
void WakeParticles::Emit()
{
    int grow = deadline - g_game->ticks + 1;

    if (grow > 0)
        items.reserve(grow + items.size());

    // Keep the do/while countdown with its counter (i = 1).
    int i = 1;
    do {
        WakeParticle e;

        e.tick = 0;
        e.period = period;
        e.endTime = g_game->ticks + period * 6;
        e.pos = pos_a;
        e.pos.xp.hi += (int)((__int64)rand() * 7 / 0x8000) - 3;
        e.pos.yp.hi += (int)((__int64)rand() * 7 / 0x8000) - 3;
        e.pos.zp.hi += (int)((__int64)rand() * 7 / 0x8000) - 3;
        e.pos2 = pos_b;
        e.vel = dir;
        e.data = g_game->unknown_147cf;
        e.min = 0x61;
        e.max = 0x67;
        if (ascending) {
            e.value = 0x61;
            e.step = 1;
        } else {
            e.value = 0x67;
            e.step = -1;
        }
        List_00474880* v = (List_00474880*)&items;
        v->insert(v->last, 1, e);
    } while (--i);

    time = g_game->ticks + 1;
}

// Drifts the puff by the game's per-tick counts (x, z by the wind times 8, y
// by the rise times 4; SmokeParticles' Update, 0x475340, inlines the same
// step) and, when the countdown runs out, counts one more round and restarts
// the countdown at half the period plus a random part of the other half.
// FUNCTION: 0x474b00
void SmokeParticle::Step()
{
    pos.x += g_game->windX * 8;
    pos.y += g_game->rise * 4;
    pos.z += g_game->windZ * 8;
    if (--timer == 0) {
        count++;
        int half = period / 2;
        timer = (int)((__int64)rand() * half / 0x8000) + half;
    }
}

// Whether the puff has counted all its rounds.
// FUNCTION: 0x474cb0
int SmokeParticle::IsExpired(int unused)
{
    return count >= limit;
}

// The constructor: an empty vector of particles, and the current tick as the
// next emit time. The base constructor is called out of line.
// Not inlined into the emitters: they called it out of line in their own
// files.

// Slot 6. Sibling of 0x475150 (same base call, position copy and virtual
// call).
// FUNCTION: 0x474d50
void SmokeParticles::Init(Vec3_00474d50* p, int limit, int a, int b, int c,
                                  int alt)
{
    SetLifetime(c);
    pos = *p;
    emitPeriod = a;
    altAnimation = alt;
    if (alt)
        maxFrame = GetGafFrameCount(g_game->unknown_147d3) - 1;
    else
        maxFrame = GetGafFrameCount(g_game->unknown_147cf) - 1;
    if (limit != 0)
        maxFrame = limit < maxFrame ? limit : maxFrame;
    if (b != 0)
        holdPeriod = b;
    else
        holdPeriod = 7;
    Emit();
}

// Slot 4: works out how many periods of emitPeriod have passed since
// deadline, reserves room for that many more particles, and appends one built
// from the position at +0x2c, holdPeriod and a random size. It then pushes
// the clock forward by emitPeriod.
// FUNCTION: 0x474df0
void SmokeParticles::Emit()
{
    int periods = (deadline - g_game->ticks + emitPeriod) / emitPeriod;
    if (periods > 0) {
        records.reserve(periods + records.size());
    }
    Vec3_00474d50* p = &pos;
    std::vector<SmokeParticle>* v = &records;
    for (int i = 1; i != 0; i--) {
        SmokeParticle rec;
        rec.pos = *p;
        rec.period = holdPeriod;
        rec.timer = holdPeriod;
        rec.data = altAnimation ? g_game->unknown_147d3 : g_game->unknown_147cf;
        rec.limit = (int)(((__int64)rand() * (maxFrame - 2)) / 0x8000) + 2;
        rec.count = 0;
        ((Class_00476210*)v)->insert(v->end(), 1, rec);
    }
    time = g_game->ticks + emitPeriod;
}

// Slot 3: true once there are no particles and the game time has passed the
// deadline.
// FUNCTION: 0x474f80
int SmokeParticles::IsFinished()
{
    int count;

    count = records.size();

    bool isZero = (count == 0);
    if (isZero) {
        if ((unsigned int)deadline < (unsigned int)g_game->ticks) {
            return 1;
        }
    }

    return 0;
}

// Drifts the 16.16 position by the wind (x, z) and a vertical rate (y), and
// when the timer runs out counts one more step and restarts the timer at a
// random value between period/2 and period. Slot 1 of TimedSubParticles
// (0x475600) inlines this; this out-of-line copy is never called. 0x474b00 is
// the same update for SmokeParticles's records (y * 4).
// FUNCTION: 0x474fc0
void TimedSubParticle::Step()
{
    pos.x += g_game->windX * 8;
    pos.y += g_game->rise * 16;
    pos.z += g_game->windZ * 8;
    if (--timer == 0) {
        count++;
        int half = period / 2;
        timer = (int)((__int64)rand() * half / 0x8000) + half;
    }
}

// Draws the particle's frame at its screen position, relative to the scroll
// position px, py.
// FUNCTION: 0x475040
void TimedSubParticle::DrawParticle(void* dest, short px, short py)
{
    short sy = posw.z - (posw.y >> 1) - py + 0x20;
    short sx = posw.x - px + 0x80;
    DrawFrameBlended(dest, GetGafFrame(data, count), sx, sy);
}

// Whether the particle has taken all its steps.
// FUNCTION: 0x475090
int TimedSubParticle::IsExpired(int unused)
{
    return count >= limit ? 1 : 0;
}

// The constructor: an empty vector of particles, and the current tick as the
// next emit time. The base constructor is called out of line.
// Not inlined into 0x472c50: it called it out of line in its own file.

// Slot 6.
// FUNCTION: 0x475150
void TimedSubParticles::Init(Vec3_00475150* p, int a, int b, int c)
{
    SetLifetime(c);
    pos = *p;
    emitPeriod = a;
    maxFrame = GetGafFrameCount(g_game->unknown_147cf) - 1;
    if (b != 0)
        holdPeriod = b;
    else
        holdPeriod = 7;
    Emit();
}

// Slot 4: appends one particle holding the effect named by
// g_game->unknown_147cf at this->pos, a random lifetime of 2 to maxFrame - 1
// periods, and a countdown of holdPeriod periods.
// FUNCTION: 0x4751c0
void TimedSubParticles::Emit()
{
    int missed = (deadline - g_game->ticks + emitPeriod) / emitPeriod;
    if (missed > 0)
        records.reserve(records.size() + missed);
    // The two pointers have to be locals: the original hoists both addresses
    // into callee saved registers before the loop, and reads the record's
    // position and the vector's _Last through them.
    Vec3_00475150* p = &pos;
    std::vector<TimedSubParticle>* v = &records;
    // One-trip countdown loop stays: the original keeps it as a counter.
    int i = 1;
    do {
        TimedSubParticle rec;
        rec.pos = *p;
        rec.period = holdPeriod;
        rec.timer = holdPeriod;
        rec.data = g_game->unknown_147cf;
        rec.limit = (int)((__int64)rand() * (maxFrame - 2) / 0x8000) + 2;
        rec.count = 0;
        ((Vec_00476490*)v)->insert(v->end(), 1, &rec);
    } while (--i);
    time = g_game->ticks + emitPeriod;
}

// Slot 3: this class always answers 0.
// FUNCTION: 0x475330
int TimedSubParticles::IsFinished()
{
    return 0;
}

// Slot 1: twin of 0x475600, which inlines the same body, so here the
// particles are walked by a loop instead. Each drifts by the game's per-tick
// counts (x, z by the wind times 8, y by the rise times 4) and, when its
// countdown runs out, counts one more round and restarts the countdown at
// half the period plus a random part of the other half. A particle that has
// counted as many rounds as its limit is erased. Then slot 5 says whether to
// emit more (slot 4).
// FUNCTION: 0x475340
void SmokeParticles::Update()
{
    std::vector<SmokeParticle>::iterator it = records.begin();
    while (it != records.end()) {
        it->pos.x += g_game->windX * 8;
        it->pos.y += g_game->rise * 4;
        it->pos.z += g_game->windZ * 8;
        if (--it->timer == 0) {
            it->count++;
            int half = it->period / 2;
            it->timer = (int)((__int64)rand() * half / 0x8000) + half;
        }
        if (it->count >= it->limit) {
            records.erase(it);
        } else {
            ++it;
        }
    }
    if (IsEmitDue())
        Emit();
}

// Slot 5: whether it is time to emit again.
// FUNCTION: 0x475440
int SmokeParticles::IsEmitDue()
{
    if (time <= deadline && (unsigned int)time <= (unsigned int)g_game->ticks) {
        return 1;
    }
    return 0;
}

// Slot 1: steps every particle with the body of 0x474fc0 inlined (drift by the
// game's per-tick counts, and when the countdown runs out count one more round
// and restart the countdown at half the period plus a random part of the
// other half). A particle that has counted as many rounds as its limit is
// erased. Then slot 5 says whether to emit more (slot 4).
// FUNCTION: 0x475600
void TimedSubParticles::Update()
{
    std::vector<TimedSubParticle>::iterator it = records.begin();
    while (it != records.end()) {
        it->pos.x += g_game->windX * 8;
        it->pos.y += g_game->rise * 16;
        it->pos.z += g_game->windZ * 8;
        if (--it->timer == 0) {
            it->count++;
            int half = it->period / 2;
            it->timer = (int)((__int64)rand() * half / 0x8000) + half;
        }
        if (it->count >= it->limit) {
            records.erase(it);
        } else {
            ++it;
        }
    }
    if (IsEmitDue())
        Emit();
}

// std::vector<NanoParticle>::reserve from MSVC 5's <vector>.
// FUNCTION: 0x475770 ?reserve@?$vector@UNanoParticle@@V?$allocator@UNanoParticle@@@std@@@std@@QAEXI@Z
ReserveFn_004739b0 Access_00475770::fn = &Access_00475770::reserve;

// The vector's own out-of-line size(): three words of a std::vector, the first
// pointer at +0x4 and the second at +0x8.
// FUNCTION: 0x475840
int Class_00475840::GetCount()
{
    return field_4 == 0 ? 0 : (field_8 - field_4) / 48;
}

// std::vector<NanoParticle>::_Destroy(first, last) from MSVC 5's <vector>:
// empty, since the element type is trivial.
// FUNCTION: 0x475870 ?_Destroy@?$vector@UNanoParticle@@V?$allocator@UNanoParticle@@@std@@@std@@IAEXPAUNanoParticle@@0@Z
DestroyFn_004739b0 Access_00475870::fn = &Access_00475870::_Destroy;

// std::vector<NanoParticle>::_Ucopy(first, last, dest) from MSVC 5's
// <vector>: copies [first, last) into raw storage at dest and returns the
// end of the copies.
// FUNCTION: 0x475880 ?_Ucopy@?$vector@UNanoParticle@@V?$allocator@UNanoParticle@@@std@@@std@@IAEPAUNanoParticle@@PBU3@0PAU3@@Z
UcopyFn_004739b0 Access_00475880::fn = &Access_00475880::_Ucopy;

// std::vector<NanoParticle>::_Ufill(first, n, value) from MSVC 5's <vector>:
// copy-constructs n copies of value into raw storage at first.
// FUNCTION: 0x476710 ?_Ufill@?$vector@UNanoParticle@@V?$allocator@UNanoParticle@@@std@@@std@@IAEXPAUNanoParticle@@IABU3@@Z
UfillFn_004739b0 Access_00476710::fn = &Access_00476710::_Ufill;


struct Shape_00472ab0 {
    Pair_00474880 v[3];
};

// Creates a ThrustParticles (vtable 0x4fd5d8) from the object pool after
// jittering the high short of each of the three pairs of the 12-byte point it
// is given, then initialises it through virtual slot 6 (0x4742c0) with that
// same point twice, a 1 and a fourth random number, then appends it to the
// list selected by the short index. Twin of 0x472330, which takes the four
// arguments of slot 6 as parameters instead of building them here. At the end
// of the file on purpose: in address order its declarations move 0x4745e0's
// symbol count, and its fog arm then picks the wrong SIB base.
// Suspected original bug: the same point is passed as both of the first two
// arguments of slot 6, so 0x4742c0 copies the same data into pos0 and
// pos1 and their difference (pos2, the vector from the first to
// the second point, scaled by 1/b) is always zero.
// FUNCTION: 0x472ab0
void __stdcall EmitJitteredThrustParticles(Shape_00472ab0* param_1, short index)
{
    // One struct copy, then three separate += statements, not a loop.
    Shape_00472ab0 s = *param_1;
    s.v[0].hi += (int)((__int64)rand() * 3 / 0x8000) - 1;
    s.v[1].hi += (int)((__int64)rand() * 3 / 0x8000) - 1;
    s.v[2].hi += (int)((__int64)rand() * 3 / 0x8000) - 1;
    // A statement of its own before the allocation: rand() runs before the g_game load.
    int r = (int)((__int64)rand() * 3 / 0x8000) + 1;
    Lists_00472330* lists = g_game->lists_00472330;
    ThrustParticles* p = new ThrustParticles;
    if (p) {
        p->Init(&s, &s, 1, r);
        lists->Add(index, p);
    }
}
