// Decompiled by Opus. Names are provisional.

class Class_00437c80 {
public:
    void FlushCache();
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x1437b];
    Class_00437c80* field_1437b;       // +0x1437b
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x416710
void __stdcall CmdRCache(int unused)
{
    g_game->field_1437b->FlushCache();
}
