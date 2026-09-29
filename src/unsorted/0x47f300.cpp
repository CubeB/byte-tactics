// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <windows.h>
// Plays the sound at soundIds[index] when the position is visible to the local
// player: explored (fog) map when g_game->flags_14281 has bit 1 set, the shared
// per-player visibility mask otherwise. Sends the 0x13 packet first when
// param_3 is set, and picks the near (-585) or far (-1585) variant depending on
// whether the position is inside the screen rectangle.

class Class_004cf570 {
public:
    int FUN_004cf570(int a, int b, void* c);
};

class Class_004cfea0 {
public:
    int FUN_004cfea0();
};

class Class_004cfeb0 {
public:
    void FUN_004cfeb0(float a, float b);
};

struct Vector3_0047f300 {
    int x, y, z;
};

union Pos_0047f300 {
    struct {
        short xFrac;                   // +0x0
        short x;                       // +0x2
        short yFrac;                   // +0x4
        short y;                       // +0x6
        short zFrac;                   // +0x8
        short z;                       // +0xa
    };
    struct {
        int xVal;                      // +0x0
        int yVal;                      // +0x4
        int zVal;                      // +0x8
    };
};

#pragma pack(push, 1)
struct Packet_0047f300 {
    unsigned char type;                // +0x0
    unsigned char flag;                // +0x1
    int index;                         // +0x2
    Pos_0047f300 pos;                  // +0x6
};

struct Player_0047f300 {
    char unknown_0[0x7c];
    unsigned char* explored;           // +0x7c
    unsigned int exploredWidth;        // +0x80
    unsigned int exploredHeight;       // +0x84
    char unknown_88[0x14b - 0x88];
};

struct Game_0047f300 {
    char unknown_0[0x10];
    Class_004cf570* sound;             // +0x10
    char unknown_14[0x1b63 - 0x14];
    Player_0047f300 players[10];       // +0x1b63
    char unknown_2851[0x2a43 - 0x2851];
    unsigned char playerIndex;         // +0x2a43
    char unknown_2a44[0x14233 - 0x2a44];
    int width;                         // +0x14233
    int height;                        // +0x14237
    int screenTilesX;                  // +0x1423b
    int screenTilesY;                  // +0x1423f
    char unknown_14243[0x14273 - 0x14243];
    unsigned short* visibilityMask;    // +0x14273
    char unknown_14277[0x14281 - 0x14277];
    unsigned char flags_14281;         // +0x14281
    char unknown_14282[0x1431f - 0x14282];
    int scrollX;                       // +0x1431f
    int scrollY;                       // +0x14323
    char unknown_14327[0x33a13 - 0x14327];
    int soundIds[1];                   // +0x33a13
    char unknown_33a17[0x37f0c - 0x33a17];
    int field_37f0c;                   // +0x37f0c
    char unknown_37f10[0x37f19 - 0x37f10];
    unsigned char flags_37f19;         // +0x37f19
};
#pragma pack(pop)

extern Game_0047f300* g_game;
extern int DAT_0051e690;
extern int DAT_0051e694;

int __cdecl FUN_0044fdb0();
int __stdcall FUN_00451df0(int player, void* data, int size);
int __stdcall FUN_0047f0c0(int index, int param_2);
struct Cell_0047f300;
Cell_0047f300* __stdcall FUN_00481550(int x, int y);

// FUNCTION: 0x47f300
int __stdcall FUN_0047f300(int index, Pos_0047f300* pos, int param_3)
{
    if (DAT_0051e694)
        return FUN_0047f0c0(index, param_3);
    if (index == 0xffff)
        return 0;
    if (g_game->field_37f0c == 0)
        return 0;
    if ((g_game->flags_37f19 & 7) == 0)
        return 0;
    if (DAT_0051e690 != 0)
        return 0;

    int sound = g_game->soundIds[index];

    if (param_3) {
        Packet_0047f300 packet;
        packet.type = 0x13;
        packet.flag = 1;
        packet.index = index;
        packet.pos = *pos;
        FUN_00451df0(FUN_0044fdb0(), &packet, 0x12);
    }

    if (FUN_00481550(pos->xVal / (1 << 20), pos->zVal / (1 << 20)) == 0)
        return 0;

    unsigned char pi = g_game->playerIndex;
    Player_0047f300* player = &g_game->players[pi];
    int vis;
    if ((g_game->flags_14281 & 2) == 2) {
        int tx = pos->x >> 5;
        int ty = (pos->z - (pos->y >> 1)) >> 5;
        if (tx < player->exploredWidth && ty < player->exploredHeight
            && player->explored[player->exploredWidth * ty + tx] != 0)
            vis = 1;
        else
            vis = 0;
    } else {
        int tx = pos->x >> 5;
        int ty = (pos->z - (pos->y >> 1)) >> 5;
        if (tx < player->exploredWidth && ty < player->exploredHeight)
            vis = (g_game->visibilityMask[player->exploredWidth * ty + tx]
                & (1 << pi)) ? 1 : 0;
        else
            vis = 0;
    }
    if (vis != 0) {
        if (((Class_004cfea0*)g_game->sound)->FUN_004cfea0()) {
            Vector3_0047f300 p;
            p.x = pos->x - g_game->scrollX - (g_game->screenTilesX / 2) * 16;
            p.y = 0;
            p.z = g_game->scrollY + (g_game->screenTilesY / 2) * 16
                + (pos->y >> 1) - pos->z;
            ((Class_004cfeb0*)g_game->sound)->FUN_004cfeb0(
                (float)(((g_game->screenTilesX + g_game->screenTilesY) / 2) * 16),
                (float)((g_game->width + g_game->height) * 16));
            return g_game->sound->FUN_004cf570(sound, -585, &p);
        } else {
            if (g_game->scrollX > pos->x || g_game->scrollY > pos->z
                || g_game->scrollX + g_game->screenTilesX * 16 < pos->x
                || g_game->scrollY + g_game->screenTilesY * 16 < pos->z)
                return g_game->sound->FUN_004cf570(sound, -1585, 0);
            return g_game->sound->FUN_004cf570(sound, -585, 0);
        }
    }
    return 0;
}
