// Decompiled by Opus. Names are provisional.
// Calls FUN_004745e0 on every 68-byte element of the std::vector at +0xc
// (compare 0x473290) with the argument and two shorts from g_game.
#include <vector>

#pragma pack(push, 1)
struct Game_00473250 {
    char unknown_0[0x1431f];
    short x;                           // +0x1431f
    char unknown_14321[2];
    short y;                           // +0x14323
};
#pragma pack(pop)

extern Game_00473250* g_game;

class Class_004745e0 {
public:
    char unknown_0[0x44];

    void FUN_004745e0(int param_1, short param_2, short param_3);
};

class Class_00473250 {
public:
    char unknown_0[0xc];
    std::vector<Class_004745e0> items;   // +0xc (_First +0x10, _Last +0x14)

    void FUN_00473250(int param_1);
};

// FUNCTION: 0x473250
void Class_00473250::FUN_00473250(int param_1)
{
    for (std::vector<Class_004745e0>::iterator it = items.begin(); it != items.end(); ++it) {
        it->FUN_004745e0(param_1, g_game->x, g_game->y);
    }
}
