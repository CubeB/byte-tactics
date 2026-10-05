// Decompiled by Space Bunny Free. Names are provisional.
// Slot 1 (FUN_00472d50) of Class_004717e0 (vtable 0x4fd5f8, see 0x472530.cpp
// and 0x471cc0.cpp for the family). Steps every element of the vector at
// +0xc, dropping the ones the element's own test rejects (erase in place, so
// the element now at the cursor is tested again), then, when virtual slot 5
// (0x473220) is true, calls virtual slot 4 (0x474880) to rebuild.
#include <stddef.h>
#include <vector>

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x38a47];
    int field_38a47;                      // +0x38a47
};
#pragma pack(pop)

extern Game* g_game;

class Class_00474580 {
public:
    char unknown_0[0x44];

    void FUN_00474580();
};

class Class_00474720 {
public:
    char unknown_0[0x44];

    int FUN_00474720(int param_1);
};

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

// Vtable 0x4fd5f8, ??_G 0x4717e0; 0x48 bytes.
class Class_004717e0 : public Class_00471cc0 {
public:
    int field_8;                                        // +0x8
    std::vector<Class_00474580> items;                  // +0xc (_First +0x10)
    char unknown_1c[0x48 - 0x1c];

    virtual void FUN_00472d50();                        // slot 1, 0x473170
    virtual void FUN_00472e30(int);                     // slot 2, 0x473250
    virtual int FUN_00472e70();                         // slot 3, 0x473290
    virtual void FUN_00474880();                        // slot 4, 0x474880
    virtual int FUN_00473220();                         // slot 5, 0x473220
    virtual void FUN_00474760(int, int, int, int, int); // slot 6, 0x474760
};

// FUNCTION: 0x473170
void Class_004717e0::FUN_00472d50()
{
    std::vector<Class_00474580>::iterator it = items.begin();

    while (it != items.end()) {
        it->FUN_00474580();
        if (((Class_00474720*)it)->FUN_00474720(g_game->field_38a47)) {
            items.erase(it);
        } else {
            ++it;
        }
    }
    if (FUN_00473220()) {
        FUN_00474880();
    }
}
