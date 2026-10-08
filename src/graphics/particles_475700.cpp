// Decompiled by Opus, Haiku and Sonnet. Names are provisional.
// Stays in its own file: merged with the module's second part the inlined draw
// rotates its temporaries (particles_472630.cpp).
// Class_004750b0 (vtable 0x4fd638, 0x34 bytes), derived from ParticleSystem
// (the family is listed in particles_470a40.cpp): the same shape as SmokeParticles,
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

// FUNCTION: 0x475700
void Class_004750b0::FUN_00472e30(int dest)
{
    for (std::vector<Class_00474fc0>::iterator it = records.begin(); it != records.end(); ++it) {
        it->Draw((void*)dest, g_game->scrollX, g_game->scrollY);
    }
}
