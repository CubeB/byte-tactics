// Decompiled by Space Bunny Free. Names are provisional.
#pragma pack(push, 1)

struct Rect_004b0510 {
    int x1;                          // +0x0
    int y1;                          // +0x4
    int x2;                          // +0x8
    int y2;                          // +0xc
};

struct Player_00473a00 {
    char unknown_0[0x7c];
    unsigned char* fogMap;           // +0x7c
    unsigned int mapWidth;           // +0x80
    unsigned int mapHeight;          // +0x84
    char unknown_88[0x14a - 0x88];
};

struct Game_00473a00 {
    char unknown_0[0x1b63];
    Player_00473a00 players[10];     // +0x1b63
    char unknown_2847[0x2a43 - 0x2847];
    unsigned char playerIndex;       // +0x2a43
    char unknown_2a44[0x14273 - 0x2a44];
    unsigned short* visibilityMask;  // +0x14273
    char unknown_14277[0x14281 - 0x14277];
    unsigned char flags;             // +0x14281, bit 1 (mask 2)
};

class Class_00473a00 {
public:
    char unknown_0[0x2];
    short x;                         // +0x2
    char unknown_4[0x2];
    short height;                    // +0x6
    char unknown_8[0x2];
    short y;                         // +0xa
    char unknown_c[0x28 - 0xc];
    int color;                       // +0x28
    char unknown_2c[0x30 - 0x2c];

    void FUN_00473a00(int param_1, short x, short y);
};
#pragma pack(pop)

extern Game_00473a00* g_game;

void __stdcall FUN_004bf6f0(void* surface, Rect_004b0510* rect, int color);

// FUNCTION: 0x473a00
void Class_00473a00::FUN_00473a00(int param_1, short x, short y)
{
    Rect_004b0510 r;
    r.x1 = (short)(this->x - x) + 0x80;
    r.y1 = (short)(this->y - y) - (this->height >> 1) + 0x20;
    r.x2 = r.x1 + 1;
    r.y2 = r.y1 + 1;

    Player_00473a00* p = &g_game->players[g_game->playerIndex];
    int visible;
    if ((g_game->flags & 2) == 2) {
        int col = this->x >> 5;
        int row = (this->y - (this->height >> 1)) >> 5;
        unsigned char* cell = p->fogMap + p->mapWidth * row;
        visible = (unsigned int)col < p->mapWidth && (unsigned int)row < p->mapHeight &&
                  cell[col] != 0;
    } else {
        int col = this->x >> 5;
        int row = (this->y - (this->height >> 1)) >> 5;
        visible = (g_game->visibilityMask[p->mapWidth * row + col] &
                   (1 << g_game->playerIndex)) != 0;
    }

    if (visible)
        FUN_004bf6f0((void*)param_1, &r, this->color);
}
