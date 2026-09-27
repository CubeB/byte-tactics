// Decompiled by Opus. Names are provisional.
// Calls FUN_00473a00 on every 48-byte element of the vector at +0xc (the
// same object as 0x472fd0), passing the map scroll position as shorts.
#include <vector>

#pragma pack(push, 1)
struct Game_00472f90 {
    char unknown_0[0x1431f];
    int scroll_x;                      // +0x1431f
    int scroll_y;                      // +0x14323
};
#pragma pack(pop)

extern Game_00472f90* g_game;

class Class_00473a00 {
public:
    char unknown_0[0x30];
    void FUN_00473a00(int param_1, short x, short y);
};

struct Class_00472fd0 {
    char unknown_0[0xc];
    std::vector<Class_00473a00> items;  // +0xc (_First +0x10, _Last +0x14)

    void FUN_00472f90(int param_1);
};

// FUNCTION: 0x472f90
void Class_00472fd0::FUN_00472f90(int param_1)
{
    for (std::vector<Class_00473a00>::iterator it = items.begin(); it != items.end(); ++it)
        it->FUN_00473a00(param_1, g_game->scroll_x, g_game->scroll_y);
}
