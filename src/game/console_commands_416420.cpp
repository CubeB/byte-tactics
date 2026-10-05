// Decompiled by Sonnet. Names are provisional.

class CMemoryCache {
public:
    void FlushCache();
};

#pragma pack(push, 1)
struct Game {
    char pad0[0x1437b];
    CMemoryCache* obj;           // +0x1437b
    char pad1[0x23b87];
    unsigned short pad_bits : 5; // +0x37f06
    unsigned short toggle : 1;
    unsigned short rest : 10;
};
#pragma pack(pop)

// GLOBAL: 0x511de8
extern Game* g_game;

void SaveSettings();

// FUNCTION: 0x416420
void __stdcall CmdShading(int unused)
{
    g_game->toggle = !g_game->toggle;
    g_game->obj->FlushCache();
    SaveSettings();
}
