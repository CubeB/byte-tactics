// Decompiled by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, ninth pass by space-bunny-free, finished by opus. Names are provisional.
//
// What the function does: a visibility test for one unit. It returns 1 at once
// if the unit's map pointer (f96) is the map asked about, or 0 if flag 4 of the
// unit's f10e is set. Otherwise it builds a 16.16 position from the unit's
// position plus the def's offsets, bails out when the position is below the
// game-wide limit, and then asks "is this point visible" four times: at p, at
// p + (def->f176,0,0), at p + (f176,-f17a,f17e), and finally at p again. The
// first three go through IsPointVisible unless the player's flags say to use the
// explored byte map instead; the fourth inlines the shared visibility bit mask.
#pragma pack(push, 1)
struct MapSize_00465ac0 {
    unsigned int width;
    unsigned int height;
    int Contains(unsigned int tx, unsigned int ty) { return tx < width && ty < height; }
};
struct ByteMap_00465ac0 {
    unsigned char* data;
    MapSize_00465ac0 size;
    unsigned char Get(int x, int y) { return data[size.width * y + x]; }
};
struct Map_00465ac0 {
    char unknown_0[0x7c];
    ByteMap_00465ac0 explored;           // +0x7c
};
struct Game {
    char unknown_0[0x2a43];
    unsigned char playerIndex;           // +0x2a43
    char unknown_2a44[0x14273 - 0x2a44];
    unsigned short* visibilityMask;      // +0x14273
    char unknown_14277[0x1427f - 0x14277];
    unsigned char limitY;                // +0x1427f
    char debugMode;
    unsigned char flags;                 // +0x14281
};
struct UnitDef_00465ac0 {
    char unknown_0[0x15e];
    int f15e;
    char unknown_162[0x166 - 0x162];
    int f166;
    char unknown_16a[0x16e - 0x16a];
    int f16e;
    char unknown_172[0x176 - 0x172];
    int f176;
    int f17a;
    int f17e;
};
struct Vec3_00465ac0 {
    int x, y, z;
};
struct Unit {
    char unknown_0[0x6a];
    Vec3_00465ac0 pos;                   // +0x6a
    char unknown_76[0x92 - 0x76];
    UnitDef_00465ac0* def;               // +0x92
    Map_00465ac0* f96;                   // +0x96
    char unknown_9a[0x10e - 0x9a];
    unsigned char f10e;                  // +0x10e
    char unknown_10f[0x110 - 0x10f];
    unsigned int f110;                   // +0x110
};
struct Position_00465ac0 {              // 16.16 fixed point; only high words read
    short xFrac;
    short x;                            // +0x2
    short yFrac;
    short y;                            // +0x6
    short zFrac;
    short z;                            // +0xa
};
struct Pos_00465ac0 {                   // 16.16 fixed point
    int x, y, z;
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall IsPointVisible(Map_00465ac0* map, Position_00465ac0* pos);

// the player's explored byte map, inlined where the game flags ask for it
static inline int IsExplored(Map_00465ac0* map, Position_00465ac0* pos)
{
    int tx = pos->x >> 5;
    int ty = (pos->z - (pos->y >> 1)) >> 5;
    if (map->explored.size.Contains(tx, ty)) {
        if (map->explored.Get(tx, ty) != 0)
            return 1;
    }
    return 0;
}
static inline int IsSeen(Map_00465ac0* map, Position_00465ac0* pos)
{
    int tx = pos->x >> 5;
    int ty = (pos->z - (pos->y >> 1)) >> 5;
    if (!map->explored.size.Contains(tx, ty))
        return 0;
    return (g_game->visibilityMask[map->explored.size.width * ty + tx] &
            (1 << g_game->playerIndex)) != 0;
}

// Plain index here, Get method in IsExplored: one helper for tests 1 to 3
// does not match.
static inline int IsExplored2(Map_00465ac0* map, Position_00465ac0* pos)
{
    int tx = pos->x >> 5;
    int ty = (pos->z - (pos->y >> 1)) >> 5;
    if (map->explored.size.Contains(tx, ty)) {
        if (map->explored.data[map->explored.size.width * ty + tx] != 0)
            return 1;
    }
    return 0;
}
// IsVisible and IsVisible2 stay single ternaries, not an if with two returns.
static inline int IsVisible(Map_00465ac0* map, Position_00465ac0* pos)
{
    return (g_game->flags & 2) == 2 ? IsExplored(map, pos) : IsPointVisible(map, pos);
}
static inline int IsVisible2(Map_00465ac0* map, Position_00465ac0* pos)
{
    return (g_game->flags & 2) == 2 ? IsExplored2(map, pos) : IsPointVisible(map, pos);
}
// Branchy shape stays: lets nothing range-track the value.
static inline int IsExplored3(Map_00465ac0* map, Position_00465ac0* pos)
{
    int tx = pos->x >> 5;
    int ty = (pos->z - (pos->y >> 1)) >> 5;
    if (map->explored.size.Contains(tx, ty)) {
        unsigned char* d = map->explored.data + map->explored.size.width * ty;
        if (d[tx] != 0)
            return 1;
    }
    return 0;
}


// Stays an if: gives the normalisation on all four exits.
static inline int IsVisible3(Map_00465ac0* map, Position_00465ac0* pos)
{
    if ((g_game->flags & 2) == 2)
        return IsExplored3(map, pos);
    return IsSeen(map, pos);
}
// FUNCTION: 0x465ac0
int __stdcall IsUnitVisibleToPlayer(Map_00465ac0* map, Unit* u)
{
    if (u->f96 == map)
        return 1;
    if (u->f10e & 4)
        return 0;
    Pos_00465ac0 p;
    p.x = u->def->f15e + u->pos.x;
    p.y = u->def->f16e + u->pos.y;
    p.z = u->def->f166 + u->pos.z;
    if (!(u->f110 & 0x200)) {
        if (p.y < (g_game->limitY << 16))
            return 0;
    }
    if (IsVisible(map, (Position_00465ac0*)&p))
        return 1;
    p.x += u->def->f176;
    if (IsVisible2(map, (Position_00465ac0*)&p))
        return 1;
    p.z += u->def->f17e;
    p.y -= u->def->f17a;
    if (IsVisible2(map, (Position_00465ac0*)&p))
        return 1;
    p.x -= u->def->f176;
    if (IsVisible3(map, (Position_00465ac0*)&p))
        return 1;
    return 0;
}
