// Decompiled by Opus, class family consolidated by Opus. Names are provisional.
// Constructor of Class_004750b0 (vtable 0x4fd638, ??_G 0x475110), derived
// from Class_00471cc0 (the family is listed in 0x471cc0.cpp); the same shape
// as 0x474cd0: an empty std::vector of 32-byte records at +0xc, and the
// current game time at +0x8. This file did not see the base class's
// definitions, so the base constructor is called out of line.
#include <stddef.h>
#include <vector>

extern char* g_game;

// Vtable 0x4fd5a8, constructor 0x471cc0, destructor 0x471d00, ??_G 0x471cd0.
class Class_00471cc0 {
public:
    int field_4;                                        // +0x4

    Class_00471cc0();
    virtual ~Class_00471cc0();                          // slot 0
    virtual void FUN_00472d50() = 0;                    // slot 1
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
class Class_004750b0 : public Class_00471cc0 {
public:
    int time;                                           // +0x8
    std::vector<Record_004750b0> records;               // +0xc (_First +0x10)
    char unknown_1c[0x34 - 0x1c];

    Class_004750b0();
    virtual void FUN_00472d50();                        // slot 1, 0x475600
    virtual void FUN_00472e30(int);                     // slot 2, 0x475700
    virtual int FUN_00472e70();                         // slot 3, 0x475330
    virtual void FUN_004751c0();                        // slot 4, 0x4751c0
    virtual int FUN_004750f0();                         // slot 5, 0x4750f0
    virtual void FUN_00475150(Vec3_00475150* pos, int a, int b, int c); // slot 6, 0x475150
};

// FUNCTION: 0x4750b0
Class_004750b0::Class_004750b0()
{
    time = *(int*)(g_game + 0x38a47);
}
