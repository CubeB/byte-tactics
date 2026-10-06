// Decompiled by Opus, Haiku, Sonnet, DeepSeek V4.1 Flash and Space Bunny Free. Names are provisional.
// ThrustParticles (vtable 0x4fd5d8, 0x44 bytes), derived from ParticleSystem
// (the family is listed in 0x471cc0.cpp): the exhaust behind a moving unit.
#include <stddef.h>
#include <stdlib.h>
#include <vector>

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

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x1431f];
    short scroll_x;                    // +0x1431f
    char unknown_14321[2];
    short scroll_y;                    // +0x14323
    char unknown_14325[0x147f3 - 0x14325];
    unsigned short* unknown_147f3;     // +0x147f3, the exhaust animation
    char unknown_147f7[0x38a47 - 0x147f7];
    int ticks;                         // +0x38a47
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall GetGafFrameCount(unsigned short* p);

class Class_00470ed0 {                 // the object pool (see 0x470ae0.cpp)
public:
    char unknown_0[4];
    void FreeSlot(void* p);            // returns an object to the pool
};

extern Class_00470ed0 DAT_0051e610;

// One particle, 0x3c bytes.
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

// The compiler-generated scalar deleting destructor. Its implicit destructor destroys the std::vector at +0xc
// (the inlined ~vector leaves the dead store of _First in the `push ecx`
// slot), then the inlined base destructor stores the base vtable, and the
// inlined operator delete returns the object to the pool.
//
// The class has no out-of-line constructor: 0x4715a0, 0x472330 and 0x472ab0
// create it with `new`, inlining it. None is decompiled yet, so the global
// below exists only to make the compiler emit the vtable and with it this
// COMDAT.
// FUNCTION: 0x4716a0 ??_GThrustParticles@@UAEPAXI@Z
static ThrustParticles* s_object = new ThrustParticles;

// Slot 1, Update: walks the std::vector of 0x3c-byte
// elements at +0xc, advances each one with Step, and erases every
// element IsExpired reports as expired, then, when virtual slot 5
// (0x4730c0) is true, calls virtual slot 4 (0x4743a0).
// The inlined vector::erase is what leaves the 0x3c-byte per-element
// `rep movsd` shift loop, the dead reload of the p + 1 local after it, and
// the reload of _Last in the loop test. The two callees have placeholder
// names from two different classes, so the second is reached by a cast.
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

// Slot 3: whether there are no particles; the bool from the inlined
// vector::empty() is widened to the int return value.
// FUNCTION: 0x473130
int ThrustParticles::FUN_00472e70()
{
    return items.empty();
}

// Slot 4 (same 0x3c-byte record family as 0x474df0 and
// 0x4751c0): reserves room for the frames between g_game->frame and field_4,
// then appends one record holding the three positions at +0x20/+0x2c/+0x38,
// a pointer out of the game structure and field_4, and pushes the clock to
// g_game->frame + 1.
//
// The record has no leading image pointer: bitmask sits at +0x00, so the three
// positions land at +0x04/+0x10/+0x1c and field_4 is the record's last dword
// at +0x38, inside the 0x3c bytes. The earlier attempt's phantom `image` at
// +0x00 shifted every store up by 4 and made the compiler drop field_4 as a
// separate dead local.
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
        rec.bitmask = g_game->unknown_147f3;
        rec.field_28 = (int)GetGafFrameCount(g_game->unknown_147f3) - 1;
        rec.field_2c = 0;
        ((Class_00475bd0*)v)->FUN_00475bd0(v->end(), 1, rec);
    }
    field_8 = g_game->ticks + 1;
}
