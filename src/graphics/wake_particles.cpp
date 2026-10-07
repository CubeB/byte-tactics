// Decompiled by Opus, Sonnet, DeepSeek V4.1 Flash and Space Bunny Free. Names are provisional.
// WakeParticles (vtable 0x4fd5f8, 0x48 bytes), derived from ParticleSystem
// (the family is listed in 0x471cc0.cpp): the wake behind a moving ship.
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <vector>

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

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x1431f];
    short scroll_x;                    // +0x1431f
    char unknown_14321[2];
    short scroll_y;                    // +0x14323
    char unknown_14325[0x147cf - 0x14325];
    void* unknown_147cf;               // +0x147cf, the wake animation
    char unknown_147d3[0x38a47 - 0x147d3];
    union {
        int ticks;                     // +0x38a47
        unsigned int now;
    };
};
#pragma pack(pop)

extern Game* g_game;

class Class_00470ed0 {                 // the object pool (see 0x470ae0.cpp)
public:
    char unknown_0[4];
    void FreeSlot(void* p);            // returns an object to the pool
};

extern Class_00470ed0 DAT_0051e610;

// One particle, 0x44 bytes.
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

// The compiler-generated scalar deleting destructor.
// The global below exists only to make the compiler emit the vtable and this COMDAT.
// FUNCTION: 0x4717e0 ??_GWakeParticles@@UAEPAXI@Z
static WakeParticles* s_object = new WakeParticles;

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
