// Decompiled by Opus and Haiku. Names are provisional.
// The particle of Class_004750b0 (the family listed in 0x471cc0.cpp): stepped
// by Step, drawn by DrawParticle, and dropped once IsExpired.
#include <stdlib.h>

void* __stdcall GetGafFrame(void* a, int b);
void __stdcall DrawFrameBlended(void* dest, void* src, int x, int y);

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x14263];
    int rise;                          // +0x14263
    char unknown_14267[0x37ecc - 0x14267];
    int windX;                         // +0x37ecc
    char unknown_37ed0[4];
    int windZ;                         // +0x37ed4
};
#pragma pack(pop)

extern Game* g_game;

// The integer halves of the 16.16 position.
struct Pos_00474fc0 {
    char unknown_0[0x2];
    short x;                           // +0x2
    char unknown_4[0x2];
    short height;                      // +0x6
    char unknown_8[0x2];
    short y;                           // +0xa
};

// One particle, 0x20 bytes.
class Class_00474fc0 {
public:
    void* data;                        // +0x0, the animation
    union {
        struct {
            int x;                     // +0x4
            int y;                     // +0x8
            int z;                     // +0xc
        };
        Pos_00474fc0 posw;
    };
    int limit;                         // +0x10, the steps it lives
    int count;                         // +0x14, the steps so far (the frame)
    int period;                        // +0x18
    int timer;                         // +0x1c

    void Step();
    void DrawParticle(void* dest, short px, short py);
    int IsExpired(int unused);
};

// Drifts the 16.16 position by the wind (x, z) and a vertical rate (y), and
// when the timer runs out counts one more step and restarts the timer at a
// random value between period/2 and period. Slot 1 of Class_004750b0
// (0x475600) inlines this; this out-of-line copy is never called. 0x474b00 is
// the same update for SmokeParticles's records (y * 4).
// FUNCTION: 0x474fc0
void Class_00474fc0::Step()
{
    x += g_game->windX * 8;
    y += g_game->rise * 16;
    z += g_game->windZ * 8;
    if (--timer == 0) {
        count++;
        int half = period / 2;
        timer = (int)((__int64)rand() * half / 0x8000) + half;
    }
}

// Draws the particle's frame at its screen position, relative to the scroll
// position px, py.
// FUNCTION: 0x475040
void Class_00474fc0::DrawParticle(void* dest, short px, short py)
{
    short sy = posw.y - (posw.height >> 1) - py + 0x20;
    short sx = posw.x - px + 0x80;
    DrawFrameBlended(dest, GetGafFrame(data, count), sx, sy);
}

// Whether the particle has taken all its steps.
// FUNCTION: 0x475090
int Class_00474fc0::IsExpired(int unused)
{
    return count >= limit ? 1 : 0;
}
