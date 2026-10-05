// Decompiled by Opus. Names are provisional.

class Class_00437c80 {
public:
    void FUN_00437c80();
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x1437b];
    Class_00437c80* field_1437b;       // +0x1437b
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x416710
void __stdcall FUN_00416710(int unused)
{
    g_game->field_1437b->FUN_00437c80();
}
