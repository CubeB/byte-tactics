// Decompiled by Space Bunny Free. Names are provisional.
// Slot 4 (Emit) of SmokeParticles (vtable 0x4fd618, see 0x474cd0.cpp):
// every frame it works out how many periods of unknown_1c have passed since
// field_4, reserves room for that many more 32-byte records, and appends one
// record built from the position at +0x2c, unknown_20 and a random size. It
// then pushes the clock forward by unknown_1c, so field_4 counts periods of
// unknown_1c from the moment this ran.
#include <stdlib.h>
#include <vector>

struct Vec3_00474cd0 {
    int x;
    int y;
    int z;
};

struct Record_00474cd0 {
    void* image;                                  // +0x00, the bitmap 0x475470 draws
    Vec3_00474cd0 pos;                            // +0x04, read back as three
                                                 //   16-bit values by 0x475470
    int size;                                     // +0x10
    int field_14;                                 // +0x14
    int field_18;                                 // +0x18
    int field_1c;                                 // +0x1c
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x147cf];
    void* unknown_147cf;
    void* unknown_147d3;
    char unknown_147d7[0x38a47 - 0x147d7];
    int frame;                                   // +0x38a47
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
};

typedef std::vector<Record_00474cd0> Vec_00474cd0;

// 0x476210 is std::vector<Record_00474cd0>::insert(iterator, size_type,
// const _Ty&), which the original leaves out of line; declaring it here keeps
// the call out of line too.
class Class_00476210 {
public:
    void FUN_00476210(Vec_00474cd0::iterator p, unsigned int m,
                     const Record_00474cd0& x);
};

// Vtable 0x4fd618, constructor 0x474cd0, ??_G 0x474d10; 0x38 bytes.
class SmokeParticles : public ParticleSystem {
public:
    int time;                                           // +0x8
    Vec_00474cd0 records;                              // +0xc (_First +0x10)
    int unknown_1c;                                     // +0x1c
    int unknown_20;                                     // +0x20
    int unknown_24;                                     // +0x24
    void* unknown_28;                                   // +0x28
    Vec3_00474cd0 pos;                                  // +0x2c

    SmokeParticles();
    virtual void Update();                              // slot 1, 0x475340
    virtual void FUN_00472e30(int);                     // slot 2, 0x475470
    virtual int FUN_00472e70();                         // slot 3, 0x474f80
    virtual void Emit();                                // slot 4, 0x474df0
};

// The two pointer locals are only there to get the address of the position and
// the address of the vector into ebp and esi, in that order, before the loop.
// FUNCTION: 0x474df0
void SmokeParticles::Emit()
{
    int periods = (field_4 - g_game->frame + unknown_1c) / unknown_1c;
    if (periods > 0) {
        records.reserve(periods + records.size());
    }
    Vec3_00474cd0* p = &pos;
    Vec_00474cd0* v = &records;
    for (int i = 1; i != 0; i--) {
        Record_00474cd0 rec;
        rec.pos = *p;
        rec.field_18 = unknown_20;
        rec.field_1c = unknown_20;
        rec.image = unknown_28 ? g_game->unknown_147d3 : g_game->unknown_147cf;
        rec.size = (int)(((__int64)rand() * (unknown_24 - 2)) / 0x8000) + 2;
        rec.field_14 = 0;
        ((Class_00476210*)v)->FUN_00476210(v->end(), 1, rec);
    }
    time = g_game->frame + unknown_1c;
}
