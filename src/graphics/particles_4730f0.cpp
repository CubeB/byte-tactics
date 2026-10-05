// Decompiled by Opus. Names are provisional.
// Slot 2 (FUN_00472e30) of ThrustParticles (vtable 0x4fd5d8, see 0x472ab0.cpp
// and 0x471cc0.cpp for the family): calls DrawParticle on every 60-byte
// element of the vector at +0xc with the argument and the two shorts at
// g_game+0x1431f and +0x14323.
#include <stddef.h>
#include <vector>

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x1431f];
    short f_1431f;                     // +0x1431f
    char unknown_14321[2];
    short f_14323;                     // +0x14323
};
#pragma pack(pop)

extern Game* g_game;

class Class_00474170 {
public:
    char unknown_0[0x3c];

    void DrawParticle(int param_1, short param_2, short param_3);
};

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

// Vtable 0x4fd5d8, ??_G 0x4716a0; 0x44 bytes.
class ThrustParticles : public ParticleSystem {
public:
    int field_8;                                        // +0x8
    std::vector<Class_00474170> items;                  // +0xc (_First +0x10)
    char unknown_1c[0x44 - 0x1c];

    virtual void Update();                              // slot 1, 0x473010
    virtual void FUN_00472e30(int);                     // slot 2, 0x4730f0
    virtual int FUN_00472e70();                         // slot 3, 0x473130
    virtual void FUN_004743a0();                        // slot 4, 0x4743a0
    virtual int FUN_004730c0();                         // slot 5, 0x4730c0
};

// FUNCTION: 0x4730f0
void ThrustParticles::FUN_00472e30(int param_1)
{
    for (std::vector<Class_00474170>::iterator it = items.begin(); it != items.end(); ++it) {
        it->DrawParticle(param_1, g_game->f_1431f, g_game->f_14323);
    }
}
