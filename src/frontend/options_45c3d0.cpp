// Decompiled by Opus. Names are provisional.

class Class_004ce580 {
public:
    void FUN_004ce580(int value);
};

struct Game_0045c3d0 {
    char unknown_0[0x10];
    Class_004ce580* field_10;          // +0x10
};

extern Game_0045c3d0* g_game;

// FUNCTION: 0x45c3d0
void __stdcall FUN_0045c3d0(int value)
{
    g_game->field_10->FUN_004ce580(value);
}
