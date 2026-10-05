// Decompiled by space-bunny-free. Names are provisional.
// Slot 1 of SmokeParticles (vtable 0x4fd618, the family is listed in
// 0x471cc0.cpp): twin of 0x475600, which inlines the same body, so here the
// records of the vector at +0xc are walked by a loop instead. Each 32-byte
// record drifts by the game's per-tick counts (x, z by the wind times 8, y by
// the rise times 4) and, when the countdown runs out, counts one more round
// and restarts the countdown at half the period plus a random part of the other
// half. A record that has counted as many rounds as its limit is erased. Then
// the record-collection tests (slot 5) and redraws (slot 4) if there is
// anything new to show.
#include <stdlib.h>
#include <stddef.h>
#include <vector>

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

// Vtable 0x4fd5a8, constructor 0x471cc0, destructor 0x471d00, ??_G 0x471cd0.
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
};

struct Record_00474cd0 {
    void* data;                        // +0x00
    int x;                             // +0x04
    int y;                             // +0x08
    int z;                             // +0x0c
    int limit;                         // +0x10
    int count;                         // +0x14
    int period;                        // +0x18
    int timer;                         // +0x1c
};

struct Vec3_00474d50;

// Vtable 0x4fd618, constructor 0x474cd0, ??_G 0x474d10; 0x38 bytes.
class SmokeParticles : public ParticleSystem {
public:
    int time;                                           // +0x8
    std::vector<Record_00474cd0> records;               // +0xc (_First +0x10)
    char unknown_1c[0x38 - 0x1c];

    SmokeParticles();
    virtual void Update();                              // slot 1, 0x475340
    virtual void FUN_00472e30(int);                     // slot 2, 0x475470
    virtual int FUN_00472e70();                         // slot 3, 0x474f80
    virtual void Emit();                                // slot 4, 0x474df0
    virtual int FUN_00475440();                         // slot 5, 0x475440
    virtual void FUN_00474d50(Vec3_00474d50* pos, int limit, int a, int b, int c,
                              int alt);                 // slot 6, 0x474d50
};

// FUNCTION: 0x475340
void SmokeParticles::Update()
{
    std::vector<Record_00474cd0>::iterator it = records.begin();
    while (it != records.end()) {
        it->x += g_game->windX * 8;
        it->y += g_game->rise * 4;
        it->z += g_game->windZ * 8;
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
