// Decompiled by space-bunny-free. Names are provisional.
// PARTIAL: check.py reports 67.2% (identical 402 byte length, wrong register and
// frame slot choices). What still differs, from the original:
//  1. `&g_game->players[team]` compiles to `mov eax,esi; add eax,ecx`, the
//     original uses `lea eax,[esi+ecx]`.
//  2. Frame slots: the original keeps the destination pointer at +0x4, the
//     source at +0x8 and the row accumulator at +0xc; ours keeps source at
//     +0x4, row at +0x8, destination at +0xc (the other six locals already
//     land on the original's offsets). Swapping the two declarations, or
//     declaring the row accumulator in the outer scope, does not move them.
//  3. `visibilityMask[index] & mask` compiles to `test eax,edx`; the original
//     loads the mask into edi first and does `and eax,edi; test ax,ax`.
//  4. Because of 3 the mask lands in edx here and in edi there, and the pixel
//     value then lives in al here and in cl there, which also reorders the
//     `fx->colorMap[*src]` chain (index in edx vs ecx).
// Structure that does match: bit 2 test/clear, bit 1 (pending) set at the end,
// the esi/ebp/ebx/edi save order, the `height > 0` wrapper, the outer loop as a
// do/while (no first iteration guard, counter stored before the wrapper) with
// the inner `for` rotated into a guarded form, `x / 2` as cdq/sub/sar, and the
// `fog` load sunk into the else branch.

#pragma pack(push, 1)
struct Fx_00466c20 {
    char unknown_0[0xcc];
    unsigned char* colorMap;         // +0xcc
};

struct Team_00466c20 {
    char unknown_0[0x7c];
    unsigned char* seenMap;          // +0x7c
    char unknown_80[0x14b - 0x80];
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Game {
    char unknown_0[0xc];
    Fx_00466c20* fx;                 // +0xc
    char unknown_10[0xdcb - 0x10];
    unsigned char fogColor;          // +0xdcb
    char unknown_dcc[0x1b63 - 0xdcc];
    Team_00466c20 players[1];        // +0x1b63, 0x14b bytes each
    char unknown_1cae[0x2a43 - 0x1cae];
    unsigned char viewTeam;          // +0x2a43
    char unknown_2a44[0x1422b - 0x2a44];
    int mapWidth;                    // +0x1422b
    int mapHeight;                   // +0x1422f
    int rowWidth;                    // +0x14233
    int mapHeight2;                  // +0x14237
    char unknown_1423b[0x14273 - 0x1423b];
    unsigned short* visibilityMask;  // +0x14273
    char unknown_14277[0x142df - 0x14277];
    void* mappedSurface;             // +0x142df
    void* pictureSurface;            // +0x142e3
    short posX;                      // +0x142e7
    short posY;                      // +0x142e9
    short width;                     // +0x142eb
    short height;                    // +0x142ed
    short blinkTimer;                // +0x142ef
    unsigned short blinkOn : 1;      // +0x142f1, bit 0
    unsigned short pending : 1;
    unsigned short mapChanged : 1;
    unsigned short rest : 13;
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x466c20
void FUN_00466c20()
{
    if (g_game->mapChanged) {
        g_game->mapChanged = 0;
        unsigned char fog = g_game->fogColor;
        int team = g_game->viewTeam;
        Team_00466c20* t = &g_game->players[team];
        unsigned int mask = 1 << team;
        unsigned char* dst = *(unsigned char**)((char*)g_game->mappedSurface + 0xc);
        unsigned char* src = *(unsigned char**)((char*)g_game->pictureSurface + 0xc);
        int halfWidth = g_game->rowWidth / 2;
        int halfHeight = g_game->mapHeight2 / 2;
        int i = 0;
        if (g_game->height > 0) {
            int mapY = 0;
            do {
                int mapX = 0;
                for (int j = 0; j < g_game->width; j++) {
                    int index = (mapY / g_game->height) * halfWidth + mapX / g_game->width;
                    unsigned char c;
                    if (g_game->visibilityMask[index] & mask) {
                        if (t->seenMap[index]) {
                            c = *src;
                        } else {
                            c = g_game->fx->colorMap[*src];
                        }
                    } else {
                        c = fog;
                    }
                    *dst = c;
                    src++;
                    dst++;
                    mapX += halfWidth;
                }
                mapY += halfHeight;
            } while (++i < g_game->width);
        }
        g_game->pending = 1;
    }
}
