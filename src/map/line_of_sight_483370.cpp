// Decompiled by Opus. Names are provisional.
// Calls FUN_00483210 on the whole map, from (0, 0) with the map's size.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x14233];
    int mapWidth;                    // +0x14233
    int mapHeight;                   // +0x14237
};
#pragma pack(pop)

struct Point16_00483370 {
    short x;
    short y;
};

extern Game* g_game;

void __stdcall FUN_00483210(Point16_00483370 pos, Point16_00483370 size);

static inline Point16_00483370 MakePoint(int x, int y)
{
    Point16_00483370 p;
    p.x = x;
    p.y = y;
    return p;
}

// FUNCTION: 0x483370
void FUN_00483370()
{
    FUN_00483210(MakePoint(0, 0), MakePoint(g_game->mapWidth, g_game->mapHeight));
}
