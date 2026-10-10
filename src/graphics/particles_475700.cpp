// Decompiled by Opus, Haiku and Sonnet. Names are provisional.
// Stays in its own file: merged with the module's second part the inlined draw
// rotates its temporaries (particles.cpp).
// TimedSubParticles (vtable 0x4fd638, 0x34 bytes), derived from ParticleSystem
// (the family is listed in particles.cpp): the same shape as SmokeParticles,
// without the fog culling.
// Needed: without <windows.h> slot 2 subtracts the scroll y before the half height.
#include <windows.h>
#include <stddef.h>
#include <stdlib.h>
#include <vector>

// The TimedSubParticles 16.16 position and the class.
#include "timed_sub_particles.h"

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
    int gravity;                       // +0x14263
    char unknown_14267[0x1431f - 0x14267];
    int scrollX;                       // +0x1431f
    int scrollY;                       // +0x14323
    char unknown_14327[0x147cf - 0x14327];
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
class Vec_00476490 {
public:
    void insert(TimedSubParticle* pos, int count, const TimedSubParticle* src);
};

#include "particle_system.h"

// FUNCTION: 0x475700
void TimedSubParticles::Render(int dest)
{
    for (std::vector<TimedSubParticle>::iterator it = records.begin(); it != records.end(); ++it) {
        it->Draw((void*)dest, g_game->scrollX, g_game->scrollY);
    }
}
