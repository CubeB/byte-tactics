// Decompiled by space-bunny-free. Names are provisional.
//
// 83.5% (300-byte original, ours 305). Prologue, player-pointer arithmetic, the
// two bounds checks, the reloaded map width and the tail call all match. What
// still differs, all inside the two test arms:
//   - fog arm: ours keeps the map width in edi and hoists its load above the
//     `sub eax, edx`; the original uses edx and loads it after.
//   - fog arm: ours builds the index in edi with fogMap in ebx, so the
//     address is [edi+ecx]; the original builds it in edx with fogMap in the
//     just-dead eax, giving [edx+eax].
//   - fog arm: ours ends the taken path with `mov edx,1; xor eax,eax;
//     test edx,edx; setne al`; the original has `mov eax,1`. The `if (visible)`
//     correction below is what stops MSVC folding the 1/0 into a setcc, and
//     dropping it costs about 17 points.
//   - mask arm: ours puts the `visible = 0` block after the taken path and
//     tests `jae` twice; the original puts it between the tests and inverts
//     the second test to `jb`.
// The register roles above are the whole remaining gap; no source spelling
// tried (about 100k variants over rect order, argument types, index spelling,
// duplicate vs helper width reads, Pos sub-struct two-way reads, block shape,
// statement order) moves them.
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
    char unknown_88[0x14b - 0x88];   // stride 331, not 330: the original's
                                     // lea is base + i + 330*i
};

struct Game_00473a00 {
    char unknown_0[0x1b63];
    Player_00473a00 players[11];     // +0x1b63, stride 0x14b
    char unknown_299c[0x2a43 - 0x299c];
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
    // The original reads the map width twice per arm, once for the bounds test
    // and once for the index; a second pointer with the same value keeps the
    // two loads apart. A helper taking the pointer does not: MSVC 5 CSEs them.
    Player_00473a00* q = &g_game->players[g_game->playerIndex];
    int visible;
    if ((g_game->flags & 2) == 2) {
        int col = this->x >> 5;
        int row = (this->y - (this->height >> 1)) >> 5;
        visible = 0;
        if (((unsigned int)col < p->mapWidth && (unsigned int)row < p->mapHeight) &&
            p->fogMap[q->mapWidth * row + col] != 0)
            visible = 1;
        if (visible)
            visible = 1;
        else
            visible = 0;
    } else {
        int col = this->x >> 5;
        int row = (this->y - (this->height >> 1)) >> 5;
        if (((unsigned int)col < p->mapWidth && (unsigned int)row < p->mapHeight)) {
            unsigned short m = g_game->visibilityMask[q->mapWidth * row + col];
            visible = (m & (1 << g_game->playerIndex)) != 0;
        } else {
            visible = 0;
        }
    }

    if (visible)
        FUN_004bf6f0((void*)param_1, &r, this->color);
}
