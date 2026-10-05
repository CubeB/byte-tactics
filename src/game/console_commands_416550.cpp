// Decompiled by Opus. Names are provisional.
// Same as 0x416510 with bit 2 of the flags word at +0x37f06.

class Class_00437c80 {
public:
    void FlushCache();
};

#pragma pack(push, 1)
struct Game {
    char pad0[0x1437b];
    Class_00437c80* obj;               // +0x1437b
    char pad1[0x37f06 - 0x1437f];
    unsigned short pad_bits : 2;       // +0x37f06
    unsigned short toggle : 1;
    unsigned short rest : 13;
};
#pragma pack(pop)

extern Game* g_game;

void SaveSettings();

// FUNCTION: 0x416550
void __stdcall CmdShadow(int unused)
{
    g_game->toggle = !g_game->toggle;
    g_game->obj->FlushCache();
    SaveSettings();
}
