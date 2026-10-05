// Decompiled by Sonnet, rewritten without volatile and class family
// consolidated by Opus. Names are provisional.
// The compiler-generated scalar deleting destructor of Class_004750b0
// (vtable 0x4fd638), derived from ParticleSystem (the family is listed in
// 0x471cc0.cpp); the same shape as 0x474d10. Its implicit destructor destroys
// the std::vector at +0xc (the inlined ~vector leaves the dead store of
// _First in the `push ecx` slot) and calls the base destructor (0x471d00);
// the class's operator delete (0x471d50) frees it. This file did not see
// their definitions, so both are called out of line.
//
// The implicit destructor never stores this class's vtable, so the
// constructor (0x4750b0, matched in 0x4750b0.cpp) is defined again below,
// unannotated, to emit the vtable and with it this COMDAT.
#include <stddef.h>
#include <vector>

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

struct Record_004750b0 {
    int unknown[8];
};

struct Vec3_00475150;

// Vtable 0x4fd638, constructor 0x4750b0, ??_G 0x475110; 0x34 bytes.
class Class_004750b0 : public ParticleSystem {
public:
    int time;                                           // +0x8
    std::vector<Record_004750b0> records;               // +0xc (_First +0x10)
    char unknown_1c[0x34 - 0x1c];

    Class_004750b0();
    virtual void Update();                              // slot 1, 0x475600
    virtual void FUN_00472e30(int);                     // slot 2, 0x475700
    virtual int FUN_00472e70();                         // slot 3, 0x475330
    virtual void Emit();                                // slot 4, 0x4751c0
    virtual int FUN_004750f0();                         // slot 5, 0x4750f0
    virtual void FUN_00475150(Vec3_00475150* pos, int a, int b, int c); // slot 6, 0x475150
};

// FUNCTION: 0x475110 ??_GClass_004750b0@@UAEPAXI@Z
Class_004750b0::Class_004750b0()
{
    time = *(int*)(g_game + 0x38a47);
}
