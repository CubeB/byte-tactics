// Decompiled by Opus. Names are provisional.
// Slot 3 (FUN_00472e70) of NanoParticles (vtable 0x4fd5b8, see 0x472200.cpp
// and 0x471cc0.cpp for the family): returns whether the std::vector of
// 48-byte elements at +0xc is empty; the bool from the inlined
// vector::empty() is widened to the int return value.
#include <stddef.h>
#include <vector>

struct Elem_00472fd0 {
    char unknown_0[0x30];
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

// Vtable 0x4fd5b8, ??_G 0x471560; 0x4c bytes.
class NanoParticles : public ParticleSystem {
public:
    int field_8;                                        // +0x8
    std::vector<Elem_00472fd0> items;                   // +0xc (_First +0x10)
    char unknown_1c[0x4c - 0x1c];

    virtual void Update();                              // slot 1, 0x472eb0
    virtual void FUN_00472e30(int);                     // slot 2, 0x472f90
    virtual int FUN_00472e70();                         // slot 3, 0x472fd0
    virtual void Emit();                                // slot 4, 0x473d50
    virtual int FUN_00472f60();                         // slot 5, 0x472f60
    virtual void FUN_00473b50(void*, void*, int);       // slot 6, 0x473b50
};

// FUNCTION: 0x472fd0
int NanoParticles::FUN_00472e70()
{
    return items.empty();
}
