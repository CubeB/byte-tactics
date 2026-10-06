// Decompiled by Opus, DeepSeek V4.1 Flash, Haiku and Claude Opus 5.5. Names are provisional.
// NanoParticles (vtable 0x4fd5b8, 0x4c bytes), derived from ParticleSystem
// (the family is listed in 0x471cc0.cpp): the nano-lathe sparks that fly from
// the box around one point to the box around another.
#include <stddef.h>
#include <vector>

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x1431f];
    int scroll_x;                      // +0x1431f
    int scroll_y;                      // +0x14323
    char unknown_14327[0x38a47 - 0x14327];
    int ticks;                         // +0x38a47
};
#pragma pack(pop)

extern Game* g_game;

class Class_00470ed0 {                 // the object pool (see 0x470ae0.cpp)
public:
    char unknown_0[4];
    void FreeSlot(void* p);            // returns an object to the pool
};

extern Class_00470ed0 DAT_0051e610;

struct Vec3_00473b50 {
    int x;
    int y;
    int z;
};

struct Seg_00473b50 {
    Vec3_00473b50 start;
    Vec3_00473b50 end;
};

// One particle, 0x30 bytes.
class Class_004739b0 {
public:
    char unknown_0[0x2c];
    int field_2c;                      // +0x2c, the tick it expires

    void Step();
    void DrawParticle(int param_1, short x, short y);
    int IsExpired(int value);
};

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
    // In particles_473d50.cpp: it needs a hand-written std::vector, so that
    // reserve and size stay calls, and that cannot share a file with <vector>.
    virtual void Emit();                                // slot 4, 0x473d50
    virtual int FUN_00472f60();                         // slot 5, 0x472f60
    virtual void FUN_00473b50(Seg_00473b50* a, Seg_00473b50* b, int c);  // slot 6
};

// The base destructor and operator delete, defined again, unannotated, because
// the original file defined them and /Ob2 inlined them into the scalar
// deleting destructor below.
ParticleSystem::~ParticleSystem()
{
}

void __stdcall ParticleSystem::operator delete(void* p)
{
    DAT_0051e610.FreeSlot(p);
}

// The compiler-generated scalar deleting destructor. The implicit destructor
// destroys the std::vector at +0xc (the inlined ~vector leaves the dead store
// of _First in the `push ecx` slot), then the inlined base destructor stores
// the base vtable, and the inlined operator delete returns the object to the
// pool. The class has no out-of-line constructor: 0x471470, 0x4720d0 and
// 0x472200 create it with `new`, inlining it. None is decompiled yet, so the
// global below exists only to make the compiler emit the vtable and with it
// this COMDAT.
// FUNCTION: 0x471560 ??_GNanoParticles@@UAEPAXI@Z
static NanoParticles* s_object = new NanoParticles;

// Slot 1: steps every item in the std::vector at +0xc, drops the ones whose field_2c is
// below the current game tick (the vector erase is inlined, so the shift down is
// the rep movsd loop), then asks the two virtuals at +0x14 and +0x10 whether the
// container needs a rebuild. Sibling of 0x472d50, which only differs in the
// element size (0x34) and its two callees.
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

// Slot 3: whether there are no particles; the bool from the inlined
// vector::empty() is widened to the int return value.
// FUNCTION: 0x472fd0
int NanoParticles::FUN_00472e70()
{
    return items.empty();
}

// Slot 6: takes the two segments and keeps the middle 3/11 of each.
// MATCH (Claude Opus 5.5, #4321). What every earlier pass was missing: the
// original reads and writes the segment fields as direct members of `this`
// (macro-expanded code, see SPLIT_SEG), not through Vec3& or int& parameters.
// With direct members MSVC knows the fields do not alias, so it schedules each
// e.x/e.y store after the next component's loads and runs the first segment's
// last store into the second segment's loads, which no reference spelling
// does. Each component has to read its start into v first and then subtract
// the field again (`d = e - s`, a CSE use of v: `mov edx, esi; sub ecx, edx`
// in the z part); `d = e - v` drops 4 bytes and the CSE copy.
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
