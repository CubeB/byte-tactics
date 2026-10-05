// Decompiled by Opus. Names are provisional.
// Same as 0x416420 with bit 1 of the flags word at +0x37f06.

class Class_00437c80 {
public:
    void FUN_00437c80();
};

#pragma pack(push, 1)
struct Game {
    char pad0[0x1437b];
    Class_00437c80* obj;               // +0x1437b
    char pad1[0x37f06 - 0x1437f];
    unsigned short pad_bits : 1;       // +0x37f06
    unsigned short toggle : 1;
    unsigned short rest : 14;
};
#pragma pack(pop)

extern Game* g_game;

void FUN_00430f00();

// FUNCTION: 0x416510
void __stdcall FUN_00416510(int unused)
{
    g_game->toggle = !g_game->toggle;
    g_game->obj->FUN_00437c80();
    FUN_00430f00();
}
