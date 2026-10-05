// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Empties the given list, then walks the local player's team list at
// g_game + player*0x14b + 0x1b63 and appends every unit whose bit 4 flag at
// +0x110 is set. The vector clear and push_back are inlined; vector::insert
// leaves the STL helpers (_Ucopy, _Ufill, _Destroy, size) and operator
// new/delete out of line.
#include <vector>

#pragma pack(push, 1)
struct Unit {
    char unknown_0[0x110];
    union {
        unsigned int raw;
        struct {
            unsigned int a : 4;
            unsigned int b : 1;
            unsigned int c : 27;
        } bits;
    } flags;                              // +0x110
    char unknown_114[0x118 - 0x114];
};

struct Team {
    char unknown_0[0x67];
    Unit* first;                          // +0x67
    Unit* last;                           // +0x6b
    char unknown_6f[0x14b - 0x6f];
};

struct Game {
    char unknown_0[0x1b63];
    Team teams[10];                       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char player;                 // +0x2a42
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x48ca20
void __stdcall FUN_0048ca20(std::vector<Unit*>* list)
{
    list->clear();
    Team* team = &g_game->teams[g_game->player];
    for (Unit* u = team->first; u <= team->last; u++) {
        if (u->flags.bits.b)
            list->push_back(u);
    }
}
