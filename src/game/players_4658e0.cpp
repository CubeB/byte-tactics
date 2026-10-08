// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// Is point (x, y) or point (x+dx, y+dy) visible to the local player?  The
// 12-byte Position local (6 shorts, x/y/z among them) is zeroed with an
// inlined memset and then filled from the arguments. When bit 1 of the game
// flags word at g_game+0x14281 is set the player's explored byte map at +0x7c
// (width +0x80, height +0x84) is used, otherwise the shared visibility bit
// mask at +0x14273 with this player's bit (g_game+0x2a43).
#include <memory.h>
#pragma pack(push, 1)
struct MapSize_004658e0 {
    unsigned int width;
    unsigned int height;
    int Contains(unsigned int tx, unsigned int ty) { return tx < width && ty < height; }
};
struct ByteMap_004658e0 {
    unsigned char* data;
    MapSize_004658e0 size;
    unsigned char Get(int x, int y) { return data[size.width * y + x]; }
};
struct Map_004658e0 {
    char unknown_0[0x7c];
    ByteMap_004658e0 explored;
};
struct Game {
    char unknown_0[0x2a43];
    unsigned char playerIndex;
    char unknown_2a44[0x14273 - 0x2a44];
    unsigned short* visibilityMask;
    char unknown_14277[0x14281 - 0x14277];
    unsigned short flags;
};
#pragma pack(pop)
extern Game* g_game;
struct Pos_004658e0 {
    short xFrac;
    short x;
    short yFrac;
    short y;
    short zFrac;
    short z;
};
static inline int IsExplored(Map_004658e0* map, Pos_004658e0* pos)
{
    int tx = pos->x >> 5;
    int ty = (pos->z - (pos->y >> 1)) >> 5;
    if (map->explored.size.Contains(tx, ty) && map->explored.Get(tx, ty) != 0)
        return 1;
    return 0;
}
static inline int IsSeen(Map_004658e0* map, Pos_004658e0* pos)
{
    int tx = pos->x >> 5;
    int ty = (pos->z - (pos->y >> 1)) >> 5;
    if (!map->explored.size.Contains(tx, ty))
        return 0;
    return (g_game->visibilityMask[map->explored.size.width * ty + tx] &
            (1 << g_game->playerIndex)) != 0;
}
// FUNCTION: 0x4658e0
int __stdcall IsFootprintVisible(Map_004658e0* map, int x, int y, int dx, int dy, short size)
{
    // Local copy of map: keeps the pointer in eax across both evaluations.
    Map_004658e0* m = map;
    // unsigned int: char or int changes the test and the register assignment.
    unsigned int vis;
    Pos_004658e0 pos;
    // Kept even though the stores are dead: reproduces the original prologue.
    memset(&pos, 0, sizeof(pos));
    pos.x = (short)(x << 4);
    pos.y = size;
    pos.z = (short)(y << 4);
    if ((g_game->flags & 2) == 2)
        vis = IsExplored(m, &pos);
    else
        vis = IsSeen(m, &pos);
    if ((int)vis)
        return 1;
    pos.x = (short)(pos.x + (dx << 4));
    pos.z = (short)(pos.z + (dy << 4));
    if ((g_game->flags & 2) == 2)
        return IsExplored(m, &pos);
    return IsSeen(m, &pos);
}

