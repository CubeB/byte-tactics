// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash, edited by deepseek-v4.1, finished by GPT-6.1-sol, finished by deepseek-v4.1-flash, finished by space-bunny-free. finished by Claude Sonnet 5.5. Names are provisional.
// Plays the sound at soundIds[index] when the
// position is visible to the local player: explored (fog) map when
// g_game->flags_14281 has bit 1 set, the shared per-player visibility mask
// otherwise. Sends the 0x13 packet first when param_3 is set, and picks the near
// (-585) or far (-1585) variant depending on whether the position is inside the
// screen rectangle.
#include <windows.h>
// Plays the sound at soundIds[index] when the position is visible to the local
// player: explored (fog) map when g_game->flags_14281 has bit 1 set, the shared
// per-player visibility mask otherwise. Sends the 0x13 packet first when
// param_3 is set, and picks the near (-585) or far (-1585) variant depending on
// whether the position is inside the screen rectangle.

class Sound {
public:
    void Set3DDistances(float a, float b);
    int PlaySampleSet(int a, int b, void* c);
    int Is3DEnabled();
    // Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
    void SetMaxBuffers(int count);
    void PlayLooping(int sample, int volume);
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

struct Game {
    char unknown_0[0x10];
    Sound* sound;             // +0x10
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

    Player_0047f300* Current() { return &players[playerIndex]; }
    Player_0047f300* Current2() { int i = playerIndex; return &players[i]; }
};
#pragma pack(pop)

extern Game* g_game;
extern int g_noDirectSound;
extern int g_useWindowsSound;

int __cdecl GetLocalDpid();
int __stdcall BroadcastPacket(int player, void* data, int size);
int __stdcall PlaySoundByIndex(int index, int param_2);
struct Cell_0047f300;
Cell_0047f300* __stdcall GetMapCell(int x, int y);







static inline int MapContains(unsigned int w, unsigned int h, int tx, int ty)
{
    return tx < w && ty < h;
}
// FUNCTION: 0x47f300
int __stdcall PlaySoundAt(int index, Pos_0047f300* pos, int param_3)
{
    if (g_useWindowsSound)
        return PlaySoundByIndex(index, param_3);
    if (index == 0xffff)
        return 0;
    if (g_game->field_37f0c == 0)
        return 0;
    if ((g_game->flags_37f19 & 7) == 0)
        return 0;
    if (g_noDirectSound != 0)
        return 0;

    int sound = g_game->soundIds[index];

    if (param_3) {
        Packet_0047f300 packet;
        packet.type = 0x13;
        packet.flag = 1;
        packet.index = index;
        packet.pos = *pos;
        BroadcastPacket(GetLocalDpid(), &packet, 0x12);
    }

    if (GetMapCell(pos->xVal / (1 << 20), pos->zVal / (1 << 20)) == 0)
        return 0;

    // pi stays an int; the player address goes through the inline Current() to get the original's lea.
    int pi = g_game->playerIndex;
    Player_0047f300* player = g_game->Current();
    Player_0047f300* player2 = g_game->Current();
    // vis stays an int (char or bool changes the 1/0 pair); the flag is compared to 2 explicitly.
    int vis;
    if ((g_game->flags_14281 & 2) == 2) {
        int tx = pos->x >> 5;
        int ty = (pos->z - (pos->y >> 1)) >> 5;
        if (tx < player->exploredWidth && ty < player->exploredHeight
            && player->explored[player2->exploredWidth * ty + tx] != 0)
            vis = 1;
        else
            vis = 0;
    } else {
        int tx = pos->x >> 5;
        int ty = (pos->z - (pos->y >> 1)) >> 5;
        if (!MapContains(player->exploredWidth, player->exploredHeight, tx, ty))
            vis = 0;
        else
            vis = (g_game->visibilityMask[player2->exploredWidth * ty + tx]
                & (1 << pi)) ? 1 : 0;
    }
    if (vis != 0) {
        if (((Sound*)g_game->sound)->Is3DEnabled()) {
            Vector3_0047f300 p;
            p.x = pos->x - g_game->scrollX - (g_game->screenTilesX / 2) * 16;
            p.z = g_game->scrollY + (g_game->screenTilesY / 2) * 16
                + (pos->y >> 1) - pos->z;
            // p.z is written before p.y: the store order follows the original.
            p.y = 0;
            ((Sound*)g_game->sound)->Set3DDistances(
                (float)(((g_game->screenTilesX + g_game->screenTilesY) / 2) * 16),
                (float)((g_game->width + g_game->height) * 16));
            return g_game->sound->PlaySampleSet(sound, -585, &p);
        } else {
            if (g_game->scrollX > pos->x || g_game->scrollY > pos->z
                || g_game->scrollX + g_game->screenTilesX * 16 < pos->x
                || g_game->scrollY + g_game->screenTilesY * 16 < pos->z)
                return g_game->sound->PlaySampleSet(sound, -1585, 0);
            return g_game->sound->PlaySampleSet(sound, -585, 0);
        }
    }
    return 0;
}