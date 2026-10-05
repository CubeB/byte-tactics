// Decompiled by Opus. Names are provisional.
// Slot 3 (FUN_00472e70) of WakeParticles (vtable 0x4fd5f8, see 0x472530.cpp
// and 0x471cc0.cpp for the family): returns whether the std::vector of
// 68-byte elements at +0xc is empty; the bool from the inlined
// vector::empty() is widened to the int return value.
#include <stddef.h>
#include <vector>

struct Elem_00473290 {
    char unknown_0[0x44];
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

// Vtable 0x4fd5f8, ??_G 0x4717e0; 0x48 bytes.
class WakeParticles : public ParticleSystem {
public:
    int field_8;                                        // +0x8
    std::vector<Elem_00473290> items;                   // +0xc (_First +0x10)
    char unknown_1c[0x48 - 0x1c];

    virtual void Update();                              // slot 1, 0x473170
    virtual void FUN_00472e30(int);                     // slot 2, 0x473250
    virtual int FUN_00472e70();                         // slot 3, 0x473290
    virtual void Emit();                                // slot 4, 0x474880
    virtual int FUN_00473220();                         // slot 5, 0x473220
    virtual void FUN_00474760(int, int, int, int, int); // slot 6, 0x474760
};

// FUNCTION: 0x473290
int WakeParticles::FUN_00472e70()
{
    return items.empty();
}
