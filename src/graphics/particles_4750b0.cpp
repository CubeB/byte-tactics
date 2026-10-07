// Decompiled by Opus, Haiku and Sonnet. Names are provisional.
// Class_004750b0 (vtable 0x4fd638, 0x34 bytes), derived from ParticleSystem
// (the family is listed in 0x471cc0.cpp): the same shape as SmokeParticles,
// without the fog culling.
// Needed: without <windows.h> slot 2 subtracts the scroll y before the half height.
#include <windows.h>
#include <stddef.h>
#include <stdlib.h>
#include <vector>

struct Vec3_00475150 {
    int x;
    int y;
    int z;
};

struct Position_00475150 {             // 16.16 fixed point; only high words read
    short xFrac;
    short x;                           // +0x2
    short yFrac;
    short y;                           // +0x6, the height
    short zFrac;
    short z;                           // +0xa
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x14263];
    int rise;                          // +0x14263
    char unknown_14267[0x1431f - 0x14267];
    short scrollX;                     // +0x1431f
    char unknown_14321[2];
    short scrollY;                     // +0x14323
    char unknown_14325[0x147cf - 0x14325];
    void* unknown_147cf;               // +0x147cf, the animation
    char unknown_147d3[0x37ecc - 0x147d3];
    int windX;                         // +0x37ecc
    char unknown_37ed0[4];
    int windZ;                         // +0x37ed4
    char unknown_37ed8[0x38a47 - 0x37ed8];
    int ticks;                         // +0x38a47
};
#pragma pack(pop)

extern Game* g_game;

// Declared returning int: the original uses the full eax without masking it.
int __stdcall GetGafFrameCount(void* ptr);
void* __stdcall GetGafFrame(void* a, int b);
void __stdcall DrawFrameBlended(void* dest, void* src, int x, int y);

// One particle, 0x20 bytes.
struct Class_00474fc0 {
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
    void DrawParticle(int param_1, short param_2, short param_3);
    int IsExpired(int param_1);
    void Draw(void* dest, short px, short py)
    {
        short sy = posw.z - (posw.y >> 1) - py + 0x20;
        short sx = posw.x - px + 0x80;
        DrawFrameBlended(dest, GetGafFrame(data, count), sx, sy);
    }
};

// The particle vector, to call its out-of-line insert under this name.
class Class_00476490 {
public:
    void FUN_00476490(Class_00474fc0* pos, int count, const Class_00474fc0* src);
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

class Class_004750b0 : public ParticleSystem {
public:
    int time;                                           // +0x8, the next emit tick
    std::vector<Class_00474fc0> records;                // +0xc (_First +0x10)
    int unknown_1c;                                     // +0x1c, the emit period
    int unknown_20;                                     // +0x20
    int unknown_24;                                     // +0x24, the frame count - 1
    Vec3_00475150 pos;                                  // +0x28

    Class_004750b0();
    virtual void Update();                              // slot 1, 0x475600
    virtual void FUN_00472e30(int);                     // slot 2, 0x475700
    virtual int FUN_00472e70();                         // slot 3, 0x475330
    virtual void Emit();                                // slot 4, 0x4751c0
    // In particles_4750f0.cpp: it is defined returning bool, and Update tests
    // its result as an int.
    virtual int FUN_004750f0();                         // slot 5, 0x4750f0
    virtual void FUN_00475150(Vec3_00475150* pos, int a, int b, int c); // slot 6, 0x475150
};

// The constructor: an empty vector of particles, and the current tick as the
// next emit time. The base constructor is called out of line.
// FUNCTION: 0x4750b0
// FUNCTION: 0x475110 ??_GClass_004750b0@@UAEPAXI@Z
Class_004750b0::Class_004750b0()
{
    time = g_game->ticks;
}

// Slot 6.
// FUNCTION: 0x475150
void Class_004750b0::FUN_00475150(Vec3_00475150* p, int a, int b, int c)
{
    SetLifetime(c);
    pos = *p;
    unknown_1c = a;
    unknown_24 = GetGafFrameCount(g_game->unknown_147cf) - 1;
    if (b != 0)
        unknown_20 = b;
    else
        unknown_20 = 7;
    Emit();
}

// Slot 4: appends one particle holding the effect named by
// g_game->unknown_147cf at this->pos, a random lifetime of 2 to unknown_24 - 1
// periods, and a countdown of unknown_20 periods.
// FUNCTION: 0x4751c0
void Class_004750b0::Emit()
{
    int missed = (field_4 - g_game->ticks + unknown_1c) / unknown_1c;
    if (missed > 0)
        records.reserve(records.size() + missed);
    // The two pointers have to be locals: the original hoists both addresses
    // into callee saved registers before the loop, and reads the record's
    // position and the vector's _Last through them.
    Vec3_00475150* p = &pos;
    std::vector<Class_00474fc0>* v = &records;
    // One-trip countdown loop stays: the original keeps it as a counter.
    int i = 1;
    do {
        Class_00474fc0 rec;
        rec.pos = *p;
        rec.period = unknown_20;
        rec.timer = unknown_20;
        rec.data = g_game->unknown_147cf;
        rec.limit = (int)((__int64)rand() * (unknown_24 - 2) / 0x8000) + 2;
        rec.count = 0;
        ((Class_00476490*)v)->FUN_00476490(v->end(), 1, &rec);
    } while (--i);
    time = g_game->ticks + unknown_1c;
}

// Slot 3: this class always answers 0.
// FUNCTION: 0x475330
int Class_004750b0::FUN_00472e70()
{
    return 0;
}

// Slot 1: steps every particle with the body of 0x474fc0 inlined (drift by the
// game's per-tick counts, and when the countdown runs out count one more round
// and restart the countdown at half the period plus a random part of the
// other half). A particle that has counted as many rounds as its limit is
// erased. Then slot 5 says whether to emit more (slot 4).
// FUNCTION: 0x475600
void Class_004750b0::Update()
{
    std::vector<Class_00474fc0>::iterator it = records.begin();
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
    if (FUN_004750f0())
        Emit();
}

// Slot 2: draws every particle relative to the game's scroll position, with
// the particle's draw method (0x475040) inlined.
// FUNCTION: 0x475700
void Class_004750b0::FUN_00472e30(int dest)
{
    for (std::vector<Class_00474fc0>::iterator it = records.begin(); it != records.end(); ++it) {
        it->Draw((void*)dest, g_game->scrollX, g_game->scrollY);
    }
}
