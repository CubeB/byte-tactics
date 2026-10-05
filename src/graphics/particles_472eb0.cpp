// Decompiled by space-bunny-free. Names are provisional.
// Slot 1 of NanoParticles (vtable 0x4fd5b8, family listed in 0x471cc0.cpp).
// Steps every item in the std::vector at +0xc, drops the ones whose field_2c is
// below the current game tick (the vector erase is inlined, so the shift down is
// the rep movsd loop), then asks the two virtuals at +0x14 and +0x10 whether the
// container needs a rebuild. Sibling of 0x472d50, which only differs in the
// element size (0x34) and its two callees.
#include <stddef.h>
#include <vector>

// The vector element is 0x30 bytes (see 0x4739b0.cpp and 0x473b30.cpp).
class Class_004739b0 {
public:
    char unknown_0[0x30];
    void Step();
};

class Class_00473b30 {
public:
    char unknown_0[0x2c];
    int field_2c;                      // +0x2c
    int IsExpired(int value);
};

extern char* g_game;                   // 0x511de8, ticks at +0x38a47

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
    std::vector<Class_004739b0> items;                  // +0xc (_First +0x10)
    char unknown_1c[0x4c - 0x1c];

    NanoParticles() {}
    virtual void Update();                              // slot 1, 0x472eb0
    virtual void FUN_00472e30(int);                     // slot 2, 0x472f90
    virtual int FUN_00472e70();                         // slot 3, 0x472fd0
    virtual void Emit();                                // slot 4, 0x473d50
    virtual int FUN_00472f60();                         // slot 5, 0x472f60
    virtual void FUN_00473b50(int, int, int);           // slot 6, 0x473b50
};

// FUNCTION: 0x472eb0
void NanoParticles::Update()
{
    std::vector<Class_004739b0>::iterator it = items.begin();
    while (it != items.end()) {
        it->Step();
        if (((Class_00473b30*)it)->IsExpired(*(int*)(g_game + 0x38a47)))
            it = items.erase(it);
        else
            ++it;
    }
    if (FUN_00472f60())
        Emit();
}
