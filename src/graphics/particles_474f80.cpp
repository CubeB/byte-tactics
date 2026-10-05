// Decompiled by Sonnet. Names are provisional.
// Slot 3 (FUN_00472e70) of SmokeParticles (vtable 0x4fd618, see 0x474cd0.cpp
// and 0x471cc0.cpp for the family): true once the record vector at +0xc is
// empty and the game time has passed the deadline in field_4.
#include <stddef.h>

extern char* g_game;

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

struct Vec3_00474d50;

// Vtable 0x4fd618, constructor 0x474cd0, ??_G 0x474d10; 0x38 bytes. The
// record vector at +0xc is spelled out as its raw pointers here.
class SmokeParticles : public ParticleSystem {
public:
    int time;                                           // +0x8
    char unknown_c[4];
    int field_10;                                       // +0x10, records _First
    int field_14;                                       // +0x14, records _Last
    char unknown_18[0x38 - 0x18];

    SmokeParticles();
    virtual void Update();                              // slot 1, 0x475340
    virtual void FUN_00472e30(int);                     // slot 2, 0x475470
    virtual int FUN_00472e70();                         // slot 3, 0x474f80
    virtual void Emit();                                // slot 4, 0x474df0
    virtual int FUN_00475440();                         // slot 5, 0x475440
    virtual void FUN_00474d50(Vec3_00474d50* pos, int limit, int a, int b, int c,
                              int alt);                 // slot 6, 0x474d50
};

// FUNCTION: 0x474f80
int SmokeParticles::FUN_00472e70()
{
    int count;

    if (field_10 == 0) {
        count = 0;
    } else {
        count = (field_14 - field_10) >> 5;
    }

    bool isZero = (count == 0);
    if (isZero) {
        if ((unsigned int)field_4 < *(unsigned int*)(g_game + 0x38a47)) {
            return 1;
        }
    }

    return 0;
}
