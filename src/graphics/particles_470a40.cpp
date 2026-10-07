// Decompiled by Opus, space-bunny-free, Sonnet, Haiku, DeepSeek V4.1 Flash, Claude Opus 5.5, Space Bunny Free, GPT-6 and claude-opus-5-5. Names are provisional.
// The particle systems and the object pool they live in, from the std::copy of
// a 14-byte element (0x470a40) to the bubble emitter (0x472530): the ObjectPool
// arena (vtable 0x4fd580), the ten per-index lists, the ParticleSystem base
// (vtable 0x4fd5a8) and its Teleport, Nano, Thrust and Wake subclasses. The
// lists and the pool are reached through g_game.
//
// 0x470c10 stays in particles_470c10.cpp: its Grow needs the hand-written
// <vector> view of the pool. 0x471160, 0x471820 and 0x471a50 stay in their
// files: they see a list as a std::vector<Elem_00473500> whose inlined insert
// cannot agree with the std::vector<ParticleSystem*> view used here. 0x471de0
// stays in particles_471de0.cpp: its SIB byte follows the file's symbol total
// (docs/c2-regalloc.md).
#include <windows.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <vector>
#include <cstdlib>

void __cdecl FUN_004d85a0(void* p);

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

// The pool object at DAT_0051e610 (vtable 0x4fd580). 0x470c10, its Grow,
// stays in particles_470c10.cpp.
class Class_00470c10 {
public:
    int Grow(int param_1, int param_2);
};

class Class_00470eb0 {                 // the pool's allocation method
public:
    char unknown_0[0x14];
    void* field_14;                    // +0x14, the slot table
    char unknown_18[4];
    int field_1c;                      // +0x1c, the slot count
    int field_20;                      // +0x20, slots handed out

    int AllocSlot(int unused);
};

class Class_00470ed0 {                 // the object pool (see 0x470ae0.cpp)
public:
    char unknown_0[0x14];
    int* field_14;                     // +0x14, the slot table
    char unknown_18[8];
    int field_20;                      // +0x20, slots handed out

    void FreeSlot(int param_1);
};

class ObjectPool {
public:
    std::vector<Item_00470ae0*> items;  // +0x4
    void* field_14;                     // +0x14
    int field_18;                       // +0x18
    int field_1c;                       // +0x1c
    int field_20;                       // +0x20

    ObjectPool(int param_1, int param_2)
    {
        field_14 = 0;
        field_18 = 0;
        field_1c = 0;
        field_20 = 0;
        if (param_1 != 0 && param_2 != 0)
            ((Class_00470c10*)this)->Grow(param_1, param_2);
    }
    virtual ~ObjectPool()
    {
        if (field_14 != 0)
            FUN_004d85a0(field_14);
        std::vector<Item_00470ae0*>::iterator it = items.begin();
        while (it != items.end()) {
            FUN_004d85a0(*it);
            items.erase(it);
        }
    }
    void FreeBlocks();
};

class Class_00470a90 {
public:
    ObjectPool* Construct(int param_1, int param_2);
};

class Class_00470b80 {
public:
    void Destroy();
};

// A point of TeleportParticles' sparks.
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
};

union Fix_004736e0 {
    int whole;
    short half[2];
};

// One spark, 0x34 bytes.
class Class_00473560 {
public:
    void* field_0;                     // +0x0, the animation
    Vec3_004736e0 pos1;                // +0x4
    Vec3_004736e0 pos2;                // +0x10
    Vec3_004736e0 dir;                 // +0x1c
    int field_28;                      // +0x28, the frame count - 1
    int field_2c;                      // +0x2c
    int field_30;                      // +0x30, the tick it expires

    void Step();
    void DrawParticle(void* p, short a, short b);
    int IsExpired(int param_1);
};

// The particle vector seen as its four words, to call its out-of-line insert.
class List_004737c0 {
public:
    char* head;                        // +0x00
    char* first;                       // +0x04
    char* last;                        // +0x08
    char* end;                         // +0x0c
    void FUN_004758c0(char* where, int count, const Class_00473560& val);
};

// One particle of NanoParticles, 0x30 bytes.
class Class_004739b0 {
public:
    char unknown_0[0x2c];
    int field_2c;                      // +0x2c, the tick it expires

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

// One particle of ThrustParticles, 0x3c bytes.
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
    void Scale(int s)
    {
        x = (int)(((__int64)x * s) >> 16);
        y = (int)(((__int64)y * s) >> 16);
        z = (int)(((__int64)z * s) >> 16);
    }
};

class Class_00474130 {
public:
    unsigned short* bitmask;           // +0x00, the animation
    Vec3_004742c0 pos0;                // +0x04
    Vec3_004742c0 pos1;                // +0x10
    Vec3_004742c0 pos2;                // +0x1c
    int field_28;                      // +0x28, the frame count - 1
    int field_2c;                      // +0x2c
    int field_30;                      // +0x30
    int field_34;                      // +0x34
    int field_38;                      // +0x38

    void Step();
    void DrawParticle(int param_1, short param_2, short param_3);
    int IsExpired(int param_1);
};

typedef std::vector<Class_00474130> Vec_004743a0;

// The particle vector, to call its out-of-line insert under this name.
class Class_00475bd0 {
public:
    void* FUN_00475bd0(Class_00474130* at, unsigned int n, const Class_00474130& x);
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
};

static inline Vec3_00474760 operator-(const Vec3_00474760& p, const Vec3_00474760& q)
{
    Vec3_00474760 r;
    r.x = p.x - q.x;
    r.y = p.y - q.y;
    r.z = p.z - q.z;
    return r;
}

// One particle of WakeParticles, 0x44 bytes.
class Class_00474580 {
public:
    void* data;                        // +0x00
    Vec3_00474760 pos;                 // +0x04
    Vec3_00474760 pos2;                // +0x10
    Vec3_00474760 vel;                 // +0x1c
    int field_28;                      // +0x28
    int field_2c;                      // +0x2c
    int field_30;                      // +0x30
    int field_34;                      // +0x34
    int field_38;                      // +0x38
    int field_3c;                      // +0x3c
    int field_40;                      // +0x40

    void Step();
    void DrawParticle(int param_1, short param_2, short param_3);
    int IsExpired(int param_1);
};

// The particle vector seen as its four words, to call its out-of-line insert.
class List_00474880 {
public:
    char* head;                        // +0x00
    char* first;                       // +0x04
    char* last;                        // +0x08
    char* end;                         // +0x0c
    void FUN_00475ef0(char* where, int count, const Class_00474580& val);
};

// Vtable 0x4fd5a8, constructor 0x471cc0, destructor 0x471d00, ??_G 0x471cd0.
// The base of the family: the pool's operator new/delete, the three virtual
// slots and the lifetime field at +0x4. The derived classes' files list the
// whole family in particle_system.cpp's comment at 0x471cc0.
class ParticleSystem {
public:
    int field_4;                                        // +0x4

    ParticleSystem();
    virtual ~ParticleSystem();                          // slot 0
    virtual void Update() = 0;                          // slot 1
    virtual void FUN_00472e30(int) = 0;                 // slot 2
    virtual int FUN_00472e70() = 0;                     // slot 3
    static void* __stdcall operator new(size_t size);   // 0x471d10
    static void __stdcall operator delete(void* p);     // 0x471d50
    void SetLifetime(int ticks);
};

// Vtable 0x4fd588, ??_G 0x471430; 0x44 bytes.
class TeleportParticles : public ParticleSystem {
public:
    int field_8;                                        // +0x8, the next emit tick
    std::vector<Class_00473560> items;                  // +0xc (_First +0x10)
    int field_1c;                                       // +0x1c, the sparks' lifetime
    Vec3_004736e0 pos1;                                 // +0x20
    Vec3_004736e0 pos2;                                 // +0x2c
    Vec3_004736e0 dir;                                  // +0x38

    TeleportParticles() {}
    virtual void Update();                              // slot 1, 0x472d50
    virtual void FUN_00472e30(int);                     // slot 2, 0x472e30
    virtual int FUN_00472e70();                         // slot 3, 0x472e70
    virtual void Emit();                                // slot 4, 0x4737c0
    virtual int FUN_00472e00();                         // slot 5, 0x472e00
    virtual void FUN_004736e0(Vec3_004736e0* a, Vec3_004736e0* b, int c);  // slot 6
};

// Vtable 0x4fd5b8, ??_G 0x471560; 0x4c bytes.
class NanoParticles : public ParticleSystem {
public:
    int field_8;                                        // +0x8, the next emit tick
    std::vector<Class_004739b0> items;                  // +0xc (_First +0x10)
    Seg_00473b50 seg_1c;                                // +0x1c, centre and box
    Seg_00473b50 seg_34;                                // +0x34, target and box

    NanoParticles() {}
    virtual void Update();                              // slot 1, 0x472eb0
    virtual void FUN_00472e30(int);                     // slot 2, 0x472f90
    virtual int FUN_00472e70();                         // slot 3, 0x472fd0
    virtual void Emit();                                // slot 4, 0x473d50
    virtual int FUN_00472f60();                         // slot 5, 0x472f60
    virtual void FUN_00473b50(Seg_00473b50* a, Seg_00473b50* b, int c);  // slot 6
};

// Vtable 0x4fd5d8, ??_G 0x4716a0; 0x44 bytes.
class ThrustParticles : public ParticleSystem {
public:
    int field_8;                                        // +0x8, the next emit tick
    Vec_004743a0 items;                                 // +0xc (_First +0x10)
    int field_1c;                                       // +0x1c
    Vec3_004742c0 pos0;                                 // +0x20
    Vec3_004742c0 pos1;                                 // +0x2c
    Vec3_004742c0 pos2;                                 // +0x38

    ThrustParticles() {}
    virtual void Update();                              // slot 1, 0x473010
    virtual void FUN_00472e30(int);                     // slot 2, 0x4730f0
    virtual int FUN_00472e70();                         // slot 3, 0x473130
    virtual void FUN_004743a0();                        // slot 4, 0x4743a0
    virtual int FUN_004730c0();                         // slot 5, 0x4730c0
    // In particles_4742c0.cpp: here its fixed-point multiply pushes its
    // operands in the other order, at every symbol count and header set tried.
    virtual void FUN_004742c0(Vec3_004742c0* p, Vec3_004742c0* q, int a, int b);  // slot 6
};

// Vtable 0x4fd5f8, ??_G 0x4717e0; 0x48 bytes.
class WakeParticles : public ParticleSystem {
public:
    int field_8;                                        // +0x8, the next emit tick
    std::vector<Class_00474580> items;                  // +0xc (_First +0x10)
    int field_1c;                                       // +0x1c
    Vec3_00474760 pos_a;                                // +0x20
    Vec3_00474760 pos_b;                                // +0x2c
    Vec3_00474760 dir;                                  // +0x38
    int field_44;                                       // +0x44

    WakeParticles() {}
    virtual void Update();                              // slot 1, 0x473170
    virtual void FUN_00472e30(int);                     // slot 2, 0x473250
    virtual int FUN_00472e70();                         // slot 3, 0x473290
    virtual void Emit();                                // slot 4, 0x474880
    virtual int FUN_00473220();                         // slot 5, 0x473220
    virtual void FUN_00474760(Vec3_00474760* a, Vec3_00474760* b, int param_3,
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

struct Vec3 {
    int x, y, z;
};

// The 24-byte struct slot 6 (0x473b50) copies both to the object, at +0x1c
// and +0x34; here both halves are initialised from the position argument.
struct Pos_00472200 {
    Vec3 a;
    Vec3 b;
};

struct Class_00472200 {                // the ten lists (see 0x471d90)
    std::vector<ParticleSystem*> lists[10];             // 0xa0 bytes

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

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x1431f];
    union {
        short scroll_x;                // +0x1431f
        short field_1431f;
    };
    char unknown_14321[2];
    union {
        short scroll_y;                // +0x14323
        short field_14323;
    };
    char unknown_14325[0x147cf - 0x14325];
    void* unknown_147cf;               // +0x147cf, the wake animation
    char unknown_147d3[0x147f3 - 0x147d3];
    union {
        void* unknown_147f3;           // +0x147f3, the spark animation
        unsigned short* bits_147f3;
    };
    char unknown_147f7[0x38a47 - 0x147f7];
    union {
        int field_38a47;               // +0x38a47, the tick
        int ticks;
        int time;
        unsigned int now;
    };
    char unknown_38a4b[0x38d77 - 0x38a4b];
    union {
        Class_00472200* lists;         // +0x38d77, the ten per-index lists
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
extern Class_00470ed0 DAT_0051e610;
extern char DAT_0051e608;
extern unsigned char DAT_0051e634;
extern void __stdcall ExitParticlePool();

int __stdcall GetGafFrameCount(void* ptr);

// std::copy for 14-byte elements, compiled with __stdcall as the default
// convention (same shape as 0x4702d0). Its one caller copies one vector's
// elements into another. It is the second std::copy instantiation, so
// data/aliases.csv lists 0x470a40 for std::copy (0x4256a0 is the first).
// FUNCTION: 0x470a40
Elem_00470a40* __stdcall FUN_00470a40(Elem_00470a40* first, Elem_00470a40* last, Elem_00470a40* dest)
{
    for (; first != last; ++dest, ++first)
        *dest = *first;
    return dest;
}

// The original calls Construct and Destroy out of line from 0x471c80 and
// 0x471ca0; in this file /Ob2 would inline them.
#pragma auto_inline(off)
// FUNCTION: 0x470a90
ObjectPool* Class_00470a90::Construct(int param_1, int param_2)
{
    ((ObjectPool*)this)->ObjectPool::ObjectPool(param_1, param_2);
    return (ObjectPool*)this;
}
#pragma auto_inline(on)

// The compiler-generated scalar deleting destructor of the class whose
// vtable (one slot) is at 0x4fd580. Its constructor is 0x470a90 and its
// out-of-line destructor 0x470b80. It frees field_14, then frees each item of
// a std::vector while erasing it from the front, then the vector's own
// destructor frees the storage.
//
// The game's instance is the function-local static DAT_0051e610 constructed
// with (1000, 0x4c) at 0x471c80. The static object below must stay: it makes
// the compiler emit the vtable and with it this COMDAT.
// FUNCTION: 0x470ae0 ??_GObjectPool@@UAEPAXI@Z
static ObjectPool s_obj(1000, 0x4c);

// The original calls this out of line from 0x471ca0.
#pragma auto_inline(off)
// FUNCTION: 0x470b80
void Class_00470b80::Destroy()
{
    ((ObjectPool*)this)->ObjectPool::~ObjectPool();
}
#pragma auto_inline(on)

// FUNCTION: 0x470e50
void ObjectPool::FreeBlocks()
{
    if (field_14 != 0) {
        FUN_004d85a0(field_14);
    }
    std::vector<Item_00470ae0*>::iterator it = items.begin();
    while (it != items.end()) {
        FUN_004d85a0(*it);
        items.erase(it);
    }
}

// The pool's methods are called out of line from the inlined operator new and
// operator delete bodies; in this file /Ob2 would inline them too.
#pragma auto_inline(off)
// FUNCTION: 0x470eb0
int Class_00470eb0::AllocSlot(int unused)
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
void Class_00470ed0::FreeSlot(int param_1)
{
    int eax = field_20 - 1;
    field_20 = eax;
    field_14[eax] = param_1;
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
void __stdcall FUN_00470f60(int* param_1, int* param_2)
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
            if (l->FUN_00472e70()) {
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
            (*it)->FUN_00472e30((int)param);
    }
}

// Passes param to slot 2 (the draw) of every particle system in one list.
// FUNCTION: 0x471120
void ParticleLists::DrawList(void* param, short index)
{
    std::vector<ParticleSystem*>* s = &lists[index];
    for (std::vector<ParticleSystem*>::iterator p = s->begin(); p != s->end(); p++)
        (*p)->FUN_00472e30((int)param);
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
        p->FUN_004736e0((Vec3_004736e0*)param_1, (Vec3_004736e0*)param_2, param_3);
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
        p->FUN_00473b50((Seg_00473b50*)param_1, (Seg_00473b50*)param_2, param_3);
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
        p->FUN_004742c0((Vec3_004742c0*)param_1, (Vec3_004742c0*)param_2, param_3, param_4);
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
        p->FUN_00474760((Vec3_00474760*)param_1, (Vec3_00474760*)param_2, param_3, param_4, param_6);
        Add(index, p);
    }
}

// The compiler-generated scalar deleting destructor of WakeParticles.
// The global below exists only to make the compiler emit the vtable and this COMDAT.
// FUNCTION: 0x4717e0 ??_GWakeParticles@@UAEPAXI@Z
static WakeParticles* s_wake = new WakeParticles;

// FUNCTION: 0x471c80
void FUN_00471c80()
{
    ((Class_00470a90*)&DAT_0051e610)->Construct(0x3e8, 0x4c);
    atexit((void (__cdecl*)(void))ExitParticlePool);
}

// FUNCTION: 0x471ca0
void ExitParticlePool()
{
    if ((DAT_0051e634 & 1) == 0) {
        DAT_0051e634 |= 1;
        ((Class_00470b80*)&DAT_0051e610)->Destroy();
    }
}

// Constructor of ParticleSystem, the base of a family of objects allocated
// from the pool DAT_0051e610 through the class's own operator new (0x471d10)
// and operator delete (0x471d50). The base vtable 0x4fd5a8 holds the virtual
// destructor and three pure virtuals; every derived class overrides those
// three and adds three more of its own (slot 6 initialises the object and
// calls 0x471d70, which sets field_4).
//
//   class           vtable    constructor  ??_G      slots 1-6
//   ParticleSystem  0x4fd5a8  0x471cc0     0x471cd0  _purecall x3
//   TeleportParticles  0x4fd588  inline       0x471430  0x472d50 0x472e30 0x472e70 0x4737c0 0x472e00 0x4736e0
//   NanoParticles  0x4fd5b8  inline       0x471560  0x472eb0 0x472f90 0x472fd0 0x473d50 0x472f60 0x473b50
//   ThrustParticles  0x4fd5d8  inline       0x4716a0  0x473010 0x4730f0 0x473130 0x4743a0 0x4730c0 0x4742c0
//   WakeParticles  0x4fd5f8  inline       0x4717e0  0x473170 0x473250 0x473290 0x474880 0x473220 0x474760
//   SmokeParticles  0x4fd618  0x474cd0     0x474d10  0x475340 0x475470 0x474f80 0x474df0 0x475440 0x474d50
//   Class_004750b0  0x4fd638  0x4750b0     0x475110  0x475600 0x475700 0x475330 0x4751c0 0x4750f0 0x475150
//
// The base destructor is 0x471d00. An override keeps the name of the base
// slot it overrides, so slots 1-3 of every derived class carry the names of
// TeleportParticles's (0x472d50, 0x472e30, 0x472e70).
// FUNCTION: 0x471cc0
// FUNCTION: 0x471cd0 ??_GParticleSystem@@UAEPAXI@Z
ParticleSystem::ParticleSystem()
{
    field_4 = 0;
}

// The out-of-line destructor of ParticleSystem (the family is listed at
// the constructor): an empty body, so only the vtable store is left.
// FUNCTION: 0x471d00
ParticleSystem::~ParticleSystem()
{
}

// The class-specific operator new of ParticleSystem (the family is listed
// at the constructor): takes a zeroed object from the pool DAT_0051e610, or
// returns null while DAT_0051e608 is set. Its operator delete is 0x471d50.
// FUNCTION: 0x471d10
void* __stdcall ParticleSystem::operator new(size_t size)
{
    if (DAT_0051e608)
        return 0;
    void* p = (void*)((Class_00470eb0*)&DAT_0051e610)->AllocSlot(size);
    if (p)
        memset(p, 0, size);
    return p;
}

// The class-specific operator delete of ParticleSystem (the family is listed
// at the constructor): returns the object to the pool DAT_0051e610.
// FUNCTION: 0x471d50
void __stdcall ParticleSystem::operator delete(void* p)
{
    DAT_0051e610.FreeSlot((int)p);
}

// SetLifetime, called out of line from slot 6 of the derived classes below.
#pragma auto_inline(off)
// FUNCTION: 0x471d70
void ParticleSystem::SetLifetime(int ticks)
{
    field_4 = g_game->time + ticks;
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
        p->FUN_004736e0((Vec3_004736e0*)param_1, (Vec3_004736e0*)param_2, param_3);
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
        q->FUN_00473b50((Seg_00473b50*)&ctx.a, (Seg_00473b50*)param_2, 1);
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
    Class_00472200* lists = g_game->lists;
    NanoParticles* p = new NanoParticles;
    if (p) {
        p->FUN_00473b50((Seg_00473b50*)param_1, (Seg_00473b50*)&pos, 1);
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
        p->FUN_004742c0((Vec3_004742c0*)param_1, (Vec3_004742c0*)param_2, param_3, param_4);
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
        p->FUN_00474760((Vec3_00474760*)param_1, (Vec3_00474760*)param_2, param_3, 1, 1);
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
        p->FUN_00474760((Vec3_00474760*)param_1, (Vec3_00474760*)param_2, param_3, 1, 0);
        l->Add(index, p);
    }
}

// Steps every element of the vector, dropping the ones the element's own test
// rejects (erase in place, so the element now at the cursor is tested again),
// then hands over to the virtual at +0x14, which asks the one at +0x10 to
// rebuild. Identical in shape to 0x473170, the same slot of the sibling class.
// FUNCTION: 0x472d50
void TeleportParticles::Update()
{
    std::vector<Class_00473560>::iterator it = items.begin();

    while (it != items.end()) {
        it->Step();
        if (it->IsExpired(g_game->field_38a47)) {
            items.erase(it);
        } else {
            ++it;
        }
    }
    if (FUN_00472e00()) {
        Emit();
    }
}

// Slot 5: whether it is time to emit again.
// FUNCTION: 0x472e00
int TeleportParticles::FUN_00472e00()
{
    if (field_8 <= field_4) {
        unsigned int game_val = g_game->field_38a47;
        if ((unsigned int)field_8 <= game_val) {
            return 1;
        }
    }
    return 0;
}

// Calls 0x473590 on every element of the std::vector of 52-byte elements
// whose emptiness 0x472e70 tests.
// FUNCTION: 0x472e30
void TeleportParticles::FUN_00472e30(int p)
{
    for (std::vector<Class_00473560>::iterator it = items.begin(); it != items.end(); ++it) {
        it->DrawParticle((void*)p, g_game->field_1431f, g_game->field_14323);
    }
}

// Returns whether the particle vector is empty.
// FUNCTION: 0x472e70
int TeleportParticles::FUN_00472e70()
{
    return items.empty();
}

// Slot 1: steps every item in the std::vector at +0xc, drops the ones whose
// field_2c is below the current game tick, then asks the two virtuals at +0x14
// and +0x10 whether the container needs a rebuild. Sibling of 0x472d50, which
// only differs in the element size (0x34) and its two callees.
// FUNCTION: 0x472eb0
void NanoParticles::Update()
{
    std::vector<Class_004739b0>::iterator it = items.begin();
    while (it != items.end()) {
        it->Step();
        if (it->IsExpired(g_game->ticks))
            it = items.erase(it);
        else
            ++it;
    }
    if (FUN_00472f60())
        Emit();
}

// Slot 5: whether it is time to emit again.
// FUNCTION: 0x472f60
int NanoParticles::FUN_00472f60()
{
    if (field_8 <= field_4) {
        unsigned int game_val = g_game->ticks;
        if ((unsigned int)field_8 <= game_val) {
            return 1;
        }
    }
    return 0;
}

// Slot 2: calls DrawParticle on every particle, passing the map scroll
// position as shorts.
// FUNCTION: 0x472f90
void NanoParticles::FUN_00472e30(int param_1)
{
    for (std::vector<Class_004739b0>::iterator it = items.begin(); it != items.end(); ++it)
        it->DrawParticle(param_1, g_game->scroll_x, g_game->scroll_y);
}

// Slot 3: whether there are no particles.
// FUNCTION: 0x472fd0
int NanoParticles::FUN_00472e70()
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
    for (std::vector<Class_00474130>::iterator it = items.begin(); it != items.end(); ) {
        it->Step();
        if (it->IsExpired(g_game->ticks))
            items.erase(it);
        else
            ++it;
    }
    if (FUN_004730c0())
        FUN_004743a0();
}

// Slot 5: whether it is time to emit again.
// FUNCTION: 0x4730c0
int ThrustParticles::FUN_004730c0()
{
    if (field_8 <= field_4) {
        unsigned int val = g_game->ticks;
        if ((unsigned int)field_8 <= val) {
            return 1;
        }
    }
    return 0;
}

// Slot 2: calls DrawParticle on every particle with the argument and the map
// scroll position.
// FUNCTION: 0x4730f0
void ThrustParticles::FUN_00472e30(int param_1)
{
    for (std::vector<Class_00474130>::iterator it = items.begin(); it != items.end(); ++it) {
        it->DrawParticle(param_1, g_game->scroll_x, g_game->scroll_y);
    }
}

// Slot 3: whether there are no particles.
// FUNCTION: 0x473130
int ThrustParticles::FUN_00472e70()
{
    return items.empty();
}

// Slot 1: steps every particle, dropping the ones the particle's own test
// rejects (erase in place, so the one now at the cursor is tested again),
// then, when slot 5 says so, calls slot 4 to emit more.
// FUNCTION: 0x473170
void WakeParticles::Update()
{
    std::vector<Class_00474580>::iterator it = items.begin();

    while (it != items.end()) {
        it->Step();
        if (it->IsExpired(g_game->ticks)) {
            items.erase(it);
        } else {
            ++it;
        }
    }
    if (FUN_00473220()) {
        Emit();
    }
}

// Slot 5: whether it is time to emit again.
// FUNCTION: 0x473220
int WakeParticles::FUN_00473220()
{
    if (field_8 <= field_4 && field_8 <= g_game->now) {
        return 1;
    }
    return 0;
}

// Slot 2: calls DrawParticle on every particle with the argument and the map
// scroll position.
// FUNCTION: 0x473250
void WakeParticles::FUN_00472e30(int param_1)
{
    for (std::vector<Class_00474580>::iterator it = items.begin(); it != items.end(); ++it) {
        it->DrawParticle(param_1, g_game->scroll_x, g_game->scroll_y);
    }
}

// Slot 3: whether there are no particles.
// FUNCTION: 0x473290
int WakeParticles::FUN_00472e70()
{
    return items.empty();
}

// Sibling of 0x4742c0: the same base call, the same two position copies, the
// same difference of the two and the same trailing virtual call, but this one
// divides the difference by how many 327680-unit segments its length holds
// instead of multiplying it by a reciprocal id.
// FUNCTION: 0x4736e0
void TeleportParticles::FUN_004736e0(Vec3_004736e0* a, Vec3_004736e0* b, int c)
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
    field_1c = step;
    dir.x /= step;
    dir.y /= step;
    dir.z /= step;
    Emit();
}

// Slot 4. It first makes room for however many ten-tick
// units field_4 has fallen behind the current tick with items.reserve(...),
// then appends one element built from the three positions plus a random value,
// and finally sets field_8 ten ticks ahead of the current tick.
// FUNCTION: 0x4737c0
void TeleportParticles::Emit()
{
    int grow = (field_4 - g_game->field_38a47 + 10) / 10;

    // The inlined vector::reserve itself is needed, not a hand-written block.
    if (grow > 0)
        items.reserve(grow + items.size());

    for (int i = 0; i < 1; i++) {
        Class_00473560 e;

        e.pos1 = pos1;
        e.pos2 = pos2;
        e.dir = dir;
        e.field_30 = g_game->field_38a47 + field_1c;
        e.field_0 = g_game->unknown_147f3;
        e.field_28 = GetGafFrameCount(g_game->unknown_147f3) - 1;
        e.field_2c = (int)(((__int64)rand() * e.field_28) / 0x8000);
        // Not push_back: calling through the List layout keeps the insert out of line.
        List_004737c0* v = (List_004737c0*)&items;
        v->FUN_004758c0(v->last, 1, e);
    }

    field_8 = g_game->field_38a47 + 10;
}

// Slot 6: takes the two segments and keeps the middle 3/11 of each.
// FUNCTION: 0x473b50
void NanoParticles::FUN_00473b50(Seg_00473b50* a, Seg_00473b50* b, int c)
{
    SetLifetime(c);
    seg_1c = *a;
    seg_34 = *b;
    SPLIT_SEG(seg_34);
    SPLIT_SEG(seg_1c);
    Emit();
}

// Slot 4 (same 0x3c-byte record family as 0x474df0 and
// 0x4751c0): reserves room for the frames between g_game->frame and field_4,
// then appends one record holding the three positions at +0x20/+0x2c/+0x38,
// a pointer out of the game structure and field_4, and pushes the clock to
// g_game->frame + 1.
//
// The record has no leading image pointer: bitmask sits at +0x00, so the three
// positions land at +0x04/+0x10/+0x1c and field_4 is the record's last dword
// at +0x38, inside the 0x3c bytes.
// FUNCTION: 0x4743a0
void ThrustParticles::FUN_004743a0()
{
    int extra = field_4 - g_game->ticks + 1;
    if (extra > 0) {
        items.reserve(extra + items.size());
    }
    Vec3_004742c0* p = &pos0;
    Vec_004743a0* v = &items;
    for (int i = 1; i != 0; i--) {
        Class_00474130 rec;
        rec.field_34 = field_1c;
        rec.field_38 = field_4;
        rec.field_30 = 0;
        rec.pos0 = *p;
        rec.pos1 = pos1;
        rec.pos2 = pos2;
        rec.bitmask = g_game->bits_147f3;
        rec.field_28 = (int)GetGafFrameCount(g_game->unknown_147f3) - 1;
        rec.field_2c = 0;
        ((Class_00475bd0*)v)->FUN_00475bd0(v->end(), 1, rec);
    }
    field_8 = g_game->ticks + 1;
}

// Slot 6: stores the two points it is given, the vector between them scaled to
// a length of 32768 (2^31/len as 16.16), and the two arguments it passes on,
// then updates itself.
// FUNCTION: 0x474760
void WakeParticles::FUN_00474760(Vec3_00474760* a, Vec3_00474760* b, int param_3,
                                  int param_4, int param_5)
{
    SetLifetime(param_4);
    field_1c = param_3;
    pos_a = *a;
    pos_b = *b;
    // The difference must come from an inline operator- returning the struct by value.
    dir = pos_b - pos_a;
    // The divisor is twice the length, with no test for zero: two identical
    // points raise a divide exception here (0x474811, _alldiv).
    int scale = 0x100000000 / (int)(((__int64)dir.Length() * 0x20000) >> 16);
    dir.Scale(scale);
    field_44 = param_5;
    Emit();
}

// Slot 4: makes room in the std::vector at +0xc for one 68-byte
// element per tick field_4 has fallen behind (the period is 1), then appends
// one element built from the object's three points, with the first point
// jittered by up to 3 units on each axis, and finally sets field_8 one tick
// ahead of the current tick.
// The element is a 12-byte point made of three 16.16 fixed-point pairs of
// shorts, so the jitter is three `+=` on the high half.
// FUNCTION: 0x474880
void WakeParticles::Emit()
{
    int grow = field_4 - g_game->ticks + 1;

    if (grow > 0)
        items.reserve(grow + items.size());

    // Keep the do/while countdown with its counter (i = 1).
    int i = 1;
    do {
        Class_00474580 e;

        e.field_38 = 0;
        e.field_3c = field_1c;
        e.field_40 = g_game->ticks + field_1c * 6;
        e.pos = pos_a;
        e.pos.xp.hi += (int)((__int64)rand() * 7 / 0x8000) - 3;
        e.pos.yp.hi += (int)((__int64)rand() * 7 / 0x8000) - 3;
        e.pos.zp.hi += (int)((__int64)rand() * 7 / 0x8000) - 3;
        e.pos2 = pos_b;
        e.vel = dir;
        e.data = g_game->unknown_147cf;
        e.field_28 = 0x61;
        e.field_2c = 0x67;
        if (field_44) {
            e.field_30 = 0x61;
            e.field_34 = 1;
        } else {
            e.field_30 = 0x67;
            e.field_34 = -1;
        }
        List_00474880* v = (List_00474880*)&items;
        v->FUN_00475ef0(v->last, 1, e);
    } while (--i);

    field_8 = g_game->ticks + 1;
}
