// Decompiled by space-bunny-free. Names are provisional.
// Steps every element of the vector, dropping the ones the element's own test
// rejects (erase in place, so the element now at the cursor is tested again),
// then hands over to the virtual at +0x14, which asks the one at +0x10 to
// rebuild. Identical in shape to 0x473170, the same slot of the sibling class.
// MSVC 5's std::vector keeps an 8-byte allocator in front of _First, so a
// member at +0x8 puts _First at +0x10 and _Last at +0x14.
#include <vector>

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x38a47];
    int field_38a47;                      // +0x38a47
};
#pragma pack(pop)

extern Game* g_game;

class Class_00473560 {                   // vector element, 0x34 bytes
public:
    char unknown_0[0x34];

    void Step();
};

class Class_004736c0 {                   // the same element, ageing test
public:
    char unknown_0[0x34];

    int IsExpired(int param_1);
};

// Vtable 0x4fd588, ??_G 0x471430; 0x44 bytes. The class is the one the
// constructor at 0x471cc0 news up (see 0x471430.cpp).
class TeleportParticles {
public:
    char unknown_0[8];                   // +0x0 vptr, +0x4 field_4
    std::vector<Class_00473560> items;   // +0x8 (_First +0x10, _Last +0x14)
    char unknown_1c[0x44 - 0x1c];

    virtual void FUN_00471430();         // +0x00
    virtual void Update();               // +0x04
    virtual void FUN_00472e30(int);      // +0x08
    virtual int FUN_00472e70();          // +0x0c
    virtual void Emit();                 // +0x10
    virtual int FUN_00472e00();          // +0x14
    virtual void FUN_004736e0(int, int, int); // +0x18
};

// FUNCTION: 0x472d50
void TeleportParticles::Update()
{
    std::vector<Class_00473560>::iterator it = items.begin();

    while (it != items.end()) {
        it->Step();
        if (((Class_004736c0*)it)->IsExpired(g_game->field_38a47)) {
            items.erase(it);
        } else {
            ++it;
        }
    }
    if (FUN_00472e00()) {
        Emit();
    }
}
