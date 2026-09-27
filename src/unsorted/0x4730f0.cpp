// Decompiled by Opus. Names are provisional.

#include <vector>

#pragma pack(push, 1)
struct Game_004730f0 {
    char unknown_0[0x1431f];
    short f_1431f;                     // +0x1431f
    char unknown_14321[2];
    short f_14323;                     // +0x14323
};
#pragma pack(pop)

extern Game_004730f0* g_game;

class Class_00474170 {
public:
    char unknown_0[0x3c];

    void FUN_00474170(int param_1, short param_2, short param_3);
};

class Class_004730f0 {
public:
    char unknown_0[0xc];
    std::vector<Class_00474170> items;  // +0xc (_First +0x10, _Last +0x14)

    void FUN_004730f0(int param_1);
};

// FUNCTION: 0x4730f0
void Class_004730f0::FUN_004730f0(int param_1)
{
    for (std::vector<Class_00474170>::iterator it = items.begin(); it != items.end(); ++it) {
        it->FUN_00474170(param_1, g_game->f_1431f, g_game->f_14323);
    }
}
