// Decompiled by Opus, Haiku, Sonnet, DeepSeek V4.1 Flash and Space Bunny Free. Names are provisional.
// Stays in its own file: merged with the module's second part its fog-culled
// loop walks from the wrong field (particles.cpp).
// SmokeParticles (vtable 0x4fd618, 0x38 bytes), derived from ParticleSystem
// (the family is listed in particles.cpp): smoke that drifts with the wind.
#include <windows.h>   // only for its symbol ids
#include <stddef.h>
#include <stdlib.h>
#include <vector>
// Only for its symbol ids: the fog-culled draw (slot 2) matches only in a
// window of the symbol count.
#include <io.h>

struct Vec3_00474d50 {
    int x;
    int y;
    int z;
};

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

#pragma pack(push, 1)
struct Player_00475470 {
    char unknown_0[0x7c];
    ByteMap_00475470 explored;         // +0x7c
    char unknown_88[0x14b - 0x88];
};

struct Game {
    char unknown_0[0x1b63];
    Player_00475470 players[10];       // +0x1b63
    char unknown_2851[0x2a43 - 0x2851];
    unsigned char playerIndex;         // +0x2a43
    char unknown_2a44[0x14263 - 0x2a44];
    int rise;                          // +0x14263
    char unknown_14267[0x14273 - 0x14267];
    unsigned short* visibilityMask;    // +0x14273
    char unknown_14277[0x14281 - 0x14277];
    unsigned char flags;               // +0x14281
    char unknown_14282[0x1431f - 0x14282];
    short scrollX;                     // +0x1431f
    char unknown_14321[2];
    short scrollY;                     // +0x14323
    char unknown_14325[0x147cf - 0x14325];
    void* unknown_147cf;               // +0x147cf, the smoke animation
    void* unknown_147d3;               // +0x147d3, the other smoke animation
    char unknown_147d7[0x37ecc - 0x147d7];
    int windX;                         // +0x37ecc
    char unknown_37ed0[4];
    int windZ;                         // +0x37ed4
    char unknown_37ed8[0x38a47 - 0x37ed8];
    int ticks;                         // +0x38a47
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall GetGafFrameCount(void* ptr);
void* __stdcall GetGafFrame(void* a, int b);
void __stdcall DrawFrameBlended(void* dest, void* src, int x, int y);

static inline int IsExplored(Player_00475470* map, Position_00475470* pos)
{
    int tx = pos->x >> 5;
    int ty = (pos->z - (pos->y >> 1)) >> 5;
    if (map->explored.size.Contains(tx, ty) && map->explored.Get(tx, ty))
        return 1;
    return 0;
}

// The body of the matched 0x408090.
static inline int IsSeen(Player_00475470* map, Position_00475470* pos)
{
    int tx = pos->x >> 5;
    int ty = (pos->z - (pos->y >> 1)) >> 5;
    if (!map->explored.size.Contains(tx, ty)) {
        return 0;
    }
    // Pointer local declared after the Contains test: the extra register
    // makes the width load materialise.
    ByteMap_00475470* b = &map->explored;
    return (g_game->visibilityMask[b->Index(tx, ty)] &
            (1 << g_game->playerIndex)) != 0;
}

static inline int IsVisible(Player_00475470* map, Position_00475470* pos)
{
    if ((g_game->flags & 2) == 2)
        return IsExplored(map, pos);
    return IsSeen(map, pos);
}

// The 32-byte record; its unculled draw method is 0x475040.
// One particle, 0x20 bytes.
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
    void DrawParticle(int param_1, short param_2, short param_3);
    int IsExpired(int param_1);
    void DrawIfVisible(void* dest, short px, short py)
    {
        short sx = posw.x - px + 0x80;
        short sy = posw.z - (posw.y >> 1) - py + 0x20;
        if (IsVisible(&g_game->players[g_game->playerIndex], &posw))
            DrawFrameBlended(dest, GetGafFrame(data, count), sx, sy);
    }
};

typedef std::vector<SmokeParticle> Vec_00474cd0;

// The particle vector, to call its out-of-line insert under this name.
class Class_00476210 {
public:
    void FUN_00476210(Vec_00474cd0::iterator p, unsigned int m,
                     const SmokeParticle& x);
};

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

class SmokeParticles : public ParticleSystem {
public:
    int time;                                           // +0x8, the next emit tick
    Vec_00474cd0 records;                               // +0xc (_First +0x10)
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

// FUNCTION: 0x475470
void SmokeParticles::Render(int dest)
{
    for (std::vector<SmokeParticle>::iterator it = records.begin(); it != records.end();
         ++it) {
        it->DrawIfVisible((void*)dest, g_game->scrollX, g_game->scrollY);
    }
}
