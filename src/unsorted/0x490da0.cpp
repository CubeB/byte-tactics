// Decompiled by Opus. Names are provisional.

class Class_00435100 {
public:
    int FUN_00435100();
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x38a43];
    int field_38a43;                   // +0x38a43
    char unknown_38a47[0x38a4b - 0x38a47];
    short field_38a4b;                 // +0x38a4b
    short field_38a4d;                 // +0x38a4d
    char unknown_38a4f[0x391e9 - 0x38a4f];
    Class_00435100* field_391e9;       // +0x391e9
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x490da0
void FUN_00490da0()
{
    if (g_game->field_391e9->FUN_00435100() == 3) {
        g_game->field_38a4b = 10;
        g_game->field_38a4d = 10;
    }
    g_game->field_38a43 = 0;
}
