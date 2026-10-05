// Decompiled by Opus. Names are provisional.
// Virtual method (slot 5 of the vtable at 0x4fd5f8).

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x38a47];
    unsigned int now;                  // +0x38a47
};
#pragma pack(pop)

extern Game* g_game;

class Class_00473220 {
public:
    char unknown_0[4];
    int field_4;                       // +0x4
    int field_8;                       // +0x8

    int FUN_00473220();
};

// FUNCTION: 0x473220
int Class_00473220::FUN_00473220()
{
    if (field_8 <= field_4 && field_8 <= g_game->now) {
        return 1;
    }
    return 0;
}
