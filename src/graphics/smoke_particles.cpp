// Decompiled by Opus, Haiku, Sonnet, DeepSeek V4.1 Flash and Space Bunny Free. Names are provisional.
// SmokeParticles (vtable 0x4fd618, 0x38 bytes), derived from ParticleSystem
// (the family is listed in 0x471cc0.cpp): smoke that drifts with the wind.
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
struct Class_00474b00 {
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

typedef std::vector<Class_00474b00> Vec_00474cd0;

// The particle vector, to call its out-of-line insert under this name.
class Class_00476210 {
public:
    void FUN_00476210(Vec_00474cd0::iterator p, unsigned int m,
                     const Class_00474b00& x);
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

class SmokeParticles : public ParticleSystem {
public:
    int time;                                           // +0x8, the next emit tick
    Vec_00474cd0 records;                               // +0xc (_First +0x10)
    int unknown_1c;                                     // +0x1c, the emit period
    int unknown_20;                                     // +0x20
    int unknown_24;                                     // +0x24, the frame count - 1
    int unknown_28;                                     // +0x28, the other animation
    Vec3_00474d50 pos;                                  // +0x2c

    SmokeParticles();
    virtual void Update();                              // slot 1, 0x475340
    virtual void FUN_00472e30(int);                     // slot 2, 0x475470
    virtual int FUN_00472e70();                         // slot 3, 0x474f80
    virtual void Emit();                                // slot 4, 0x474df0
    virtual int FUN_00475440();                         // slot 5, 0x475440
    virtual void FUN_00474d50(Vec3_00474d50* pos, int limit, int a, int b, int c,
                              int alt);                 // slot 6, 0x474d50
};

// The constructor: an empty vector of particles, and the current tick as the
// next emit time. The base constructor is called out of line.
// FUNCTION: 0x474cd0
// FUNCTION: 0x474d10 ??_GSmokeParticles@@UAEPAXI@Z
SmokeParticles::SmokeParticles()
{
    time = g_game->ticks;
}

// Slot 6. Sibling of 0x475150 (same base call, position copy and virtual
// call).
// FUNCTION: 0x474d50
void SmokeParticles::FUN_00474d50(Vec3_00474d50* p, int limit, int a, int b, int c,
                                  int alt)
{
    SetLifetime(c);
    pos = *p;
    unknown_1c = a;
    unknown_28 = alt;
    if (alt)
        unknown_24 = GetGafFrameCount(g_game->unknown_147d3) - 1;
    else
        unknown_24 = GetGafFrameCount(g_game->unknown_147cf) - 1;
    if (limit != 0)
        unknown_24 = limit < unknown_24 ? limit : unknown_24;
    if (b != 0)
        unknown_20 = b;
    else
        unknown_20 = 7;
    Emit();
}

// Slot 4: works out how many periods of unknown_1c have passed since
// field_4, reserves room for that many more particles, and appends one built
// from the position at +0x2c, unknown_20 and a random size. It then pushes
// the clock forward by unknown_1c.
// FUNCTION: 0x474df0
void SmokeParticles::Emit()
{
    int periods = (field_4 - g_game->ticks + unknown_1c) / unknown_1c;
    if (periods > 0) {
        records.reserve(periods + records.size());
    }
    Vec3_00474d50* p = &pos;
    Vec_00474cd0* v = &records;
    for (int i = 1; i != 0; i--) {
        Class_00474b00 rec;
        rec.pos = *p;
        rec.period = unknown_20;
        rec.timer = unknown_20;
        rec.data = unknown_28 ? g_game->unknown_147d3 : g_game->unknown_147cf;
        rec.limit = (int)(((__int64)rand() * (unknown_24 - 2)) / 0x8000) + 2;
        rec.count = 0;
        ((Class_00476210*)v)->FUN_00476210(v->end(), 1, rec);
    }
    time = g_game->ticks + unknown_1c;
}

// Slot 3: true once there are no particles and the game time has passed the
// deadline in field_4.
// FUNCTION: 0x474f80
int SmokeParticles::FUN_00472e70()
{
    int count;

    count = records.size();

    bool isZero = (count == 0);
    if (isZero) {
        if ((unsigned int)field_4 < (unsigned int)g_game->ticks) {
            return 1;
        }
    }

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
    std::vector<Class_00474b00>::iterator it = records.begin();
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
    if (FUN_00475440())
        Emit();
}

// Slot 5: whether it is time to emit again.
// FUNCTION: 0x475440
int SmokeParticles::FUN_00475440()
{
    if (time <= field_4 && (unsigned int)time <= (unsigned int)g_game->ticks) {
        return 1;
    }
    return 0;
}

// Slot 2, the fog-culled twin of Class_004750b0::FUN_00472e30 (0x475700). Every
// particle is drawn through a record method that computes the screen position
// first and only draws when the particle's world cell is visible to the local
// player: the player's explored byte map (+0x7c data, +0x80 width, +0x84
// height) when bit 1 of the flag byte at +0x14281 is set, else that player's
// bit in the global short map at +0x14273 (the matched 0x408090). The same
// explored-map test appears in 0x407e90 (0x407f74), 0x465ac0 (0x465b6a) and
// 0x473a00.
// FUNCTION: 0x475470
void SmokeParticles::FUN_00472e30(int dest)
{
    for (std::vector<Class_00474b00>::iterator it = records.begin(); it != records.end();
         ++it) {
        it->DrawIfVisible((void*)dest, g_game->scrollX, g_game->scrollY);
    }
}
