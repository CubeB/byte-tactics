// Decompiled by Opus. Names are provisional.

class CMemoryCache {
public:
    void FlushCache();
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x1437b];
    CMemoryCache* field_1437b;         // +0x1437b
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x416710
void __stdcall CmdRCache(int unused)
{
    g_game->field_1437b->FlushCache();
}
