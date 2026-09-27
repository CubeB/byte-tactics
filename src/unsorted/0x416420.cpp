// Decompiled by Sonnet. Names are provisional.

class Class_00437c80 {
public:
    void FUN_00437c80();
};

#pragma pack(push, 1)
struct GameState {
    char pad0[0x1437b];
    Class_00437c80* obj;         // +0x1437b
    char pad1[0x23b87];
    unsigned short pad_bits : 5; // +0x37f06
    unsigned short toggle : 1;
    unsigned short rest : 10;
};
#pragma pack(pop)

// GLOBAL: 0x511de8
extern GameState* g_game;

void FUN_00430f00();

// FUNCTION: 0x416420
void __stdcall FUN_00416420(int unused)
{
    g_game->toggle = !g_game->toggle;
    g_game->obj->FUN_00437c80();
    FUN_00430f00();
}
