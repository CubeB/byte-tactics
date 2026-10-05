// Decompiled by Opus. Names are provisional.
// Calls 0x473590 on every element of the std::vector of 52-byte elements
// whose emptiness 0x472e70 tests.
#include <vector>

class Class_00473590 {
public:
    char unknown_0[0x34];

    void DrawParticle(void* p, short a, short b);
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x1431f];
    short field_1431f;                 // +0x1431f
    char unknown_14321[2];
    short field_14323;                 // +0x14323
};
#pragma pack(pop)

extern Game* g_game;

struct Class_00472e30 {
    char unknown_0[0xc];
    std::vector<Class_00473590> items;  // +0xc (_First +0x10, _Last +0x14)

    void FUN_00472e30(void* p);
};

// FUNCTION: 0x472e30
void Class_00472e30::FUN_00472e30(void* p)
{
    for (std::vector<Class_00473590>::iterator it = items.begin(); it != items.end(); ++it) {
        it->DrawParticle(p, g_game->field_1431f, g_game->field_14323);
    }
}
