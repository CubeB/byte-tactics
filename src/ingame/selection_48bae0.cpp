// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Builds g_game->list (short array at +0x1435f, count at +0x14367) with the
// ids of every unit whose screen bounding box intersects the limit rect at
// +0x37e27 and which the local player can see.
//
// The unit type stores a 3D box in six shorts at +0x160..+0x174, four bytes
// apart: x1, y1, z1, x2, y2, z2. The screen box is (x + xOff - scrollX,
// z + zOff - scrollY - (y + yOff) / 2), so the top corner pairs x1 with y2
// and z1 and the bottom corner pairs x2 with y1 and z2.

#pragma pack(push, 1)

struct Rect_0048bae0 {
    int left;                          // +0x0
    int top;                           // +0x4
    int right;                         // +0x8
    int bottom;                        // +0xc
};

// Positions are read through this union (whole part at +2): gives the movsx loads.
union Fixed_0048bae0 {
    int value;
    struct {
        unsigned short fraction;
        short whole;
    } parts;
};

struct Vec3_0048bae0 {
    int x;
    int y;
    int z;
};

struct Cell_0048bae0 {
    char unknown_0[4];
    unsigned char height;              // +0x4
    char unknown_5[8];
};

struct UnitType_0048bae0 {
    char unknown_0[0x160];
    short f160;                        // +0x160
    char unknown_162[2];
    short f164;                        // +0x164
    char unknown_166[2];
    short f168;                        // +0x168
    char unknown_16a[2];
    short f16c;                        // +0x16c
    char unknown_16e[2];
    short f170;                        // +0x170
    char unknown_172[2];
    short f174;                        // +0x174
};

struct Player_0048bae0 {
    char unknown_0[0x14b];
};

struct Unit {
    char unknown_0[0x6a];
    Fixed_0048bae0 pos_x;              // +0x6a
    Fixed_0048bae0 pos_y;              // +0x6e
    Fixed_0048bae0 pos_z;              // +0x72
    char unknown_76[0x92 - 0x76];
    UnitType_0048bae0* def;            // +0x92
    char unknown_96[0xa6 - 0x96];
    short field_a6;                    // +0xa6
    unsigned short id;                 // +0xa8
    char unknown_aa[0xff - 0xaa];
    unsigned char owner;               // +0xff
    char unknown_100[0x110 - 0x100];
    unsigned int flags;                // +0x110
    char unknown_114[0x118 - 0x114];
};

struct Game {
    char unknown_0[0x1b63];
    Player_0048bae0 players[10];       // +0x1b63, stride 0x14b
    char unknown_2851[0x2a43 - 0x2851];
    unsigned char playerIndex;         // +0x2a43
    char unknown_2a44[0x1431f - 0x2a44];
    int scrollX;                       // +0x1431f
    int scrollY;                       // +0x14323
    char unknown_14327[0x14357 - 0x14327];
    Unit* units;                       // +0x14357
    Unit* unitsEnd;                    // +0x1435b
    unsigned short* list;              // +0x1435f
    char unknown_14363[0x14367 - 0x14363];
    int count;                         // +0x14367
    char unknown_1436b[0x37e27 - 0x1436b];
    Rect_0048bae0 rect;                // +0x37e27
};

#pragma pack(pop)

extern Game* g_game;

Cell_0048bae0* __stdcall GetMapCellAtPosition(Vec3_0048bae0* pos);
int __stdcall FUN_00465ac0(Player_0048bae0* player, Unit* unit);

// FUNCTION: 0x48bae0
void FUN_0048bae0(void)
{
    int count = 0;
    unsigned short* out = g_game->list;
    Rect_0048bae0* rect = &g_game->rect;
    Player_0048bae0* player = &g_game->players[g_game->playerIndex];
    for (Unit* u = g_game->units; u <= g_game->unitsEnd; u++) {
        if (u->field_a6 != 0) {
            UnitType_0048bae0* def = u->def;
            int ux = u->pos_x.parts.whole;
            int uy = u->pos_y.parts.whole;
            int camy = g_game->scrollY;
            int x1 = def->f160 + ux - g_game->scrollX;
            int y2 = def->f170 + uy;
            int z1 = def->f168 + u->pos_z.parts.whole - camy;
            int x2 = def->f16c + ux - g_game->scrollX;
            int y1 = def->f164 + uy;
            int z2 = def->f174 + u->pos_z.parts.whole - g_game->scrollY;
            if ((u->flags & 3) != 1) {
                Cell_0048bae0* c = GetMapCellAtPosition((Vec3_0048bae0*)&u->pos_x);
                if (c != 0) {
                    int h = c->height;
                    if (y1 > h)
                        y1 = h;
                }
            }
            // Named locals, with top declared before right: keeps register use and the final adds.
            int left = x1 + 0x80;
            int top = z1 - (y2 >> 1) + 0x20;
            int right = x2 + 0x80;
            int bottom = z2 - (y1 >> 1) + 0x20;
            if (left <= rect->right && right >= rect->left
                && top <= rect->bottom && bottom >= rect->top) {
                if (u->owner == g_game->playerIndex
                    || FUN_00465ac0(player, u)) {
                    *out++ = u->id;
                    count++;
                }
            }
        }
    }
    g_game->count = count;
}
