// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// PARTIAL: best scoring variant, 78.6% (was 76.1% with the do/while outer loop),
// same 402 byte length, every instruction present. What still differs:
//   1. The frame slots of the three loop-carried locals are cyclically rotated:
//      the original has mapY at -24 (E-0x18), src at -28 (E-0x1c) and dst at
//      -32 (E-0x20); ours has dst=-24, mapY=-28, src=-32. Every other slot
//      (halfHeight -4, t -8, mask -12, i -16, j -20, fog -33) already matches,
//      and so does the whole prologue up to the pointer stores.
//      This does not respond to declaration order at all: reordering the
//      declarations of src/dst/mapY, moving them into nested blocks, changing
//      the pixel local to char, ternary vs if/else and pre/post increments all
//      leave the same three slots on the same three variables. It looks like
//      the register allocator's spill order, not a source-visible property.
//   2. Because of 1 the loop body's scratch registers rotate one place: the
//      seenMap chain is edx/eax here vs eax/edx there, the pixel lives in al vs
//      cl, and the store is `mov [ecx],al` vs `mov [edx],cl`.
//   3. The loop-bottom schedule differs slightly (the original loads i, src,
//      dst then interleaves the three increments around the store).
// What is confirmed matching in the 78.6% version: the bit 2 test/clear and
// bit 1 (pending) set, the esi/ebp/ebx/edi save order, `height > 0` guard,
// the outer loop as `for (i = 0; i < height; i++)` with `mapY = i * halfHeight`
// (the accumulator spelling produced the same code plus a spurious prologue
// store), the inner `for` rotated into a guarded form, `x / 2` as cdq/sub/sar,
// the players indexing `lea edx,[eax+edx*2+0x1b63]` and the fog sunk into the
// else branch. The `unsigned short` cast on the visibility test is required
// (without it MSVC emits `test edx,eax`).
// /Gz (__stdcall) makes no difference for a no-arg function; the sibling
// 0x466780 matched because it was declared __stdcall, not because of the flag.

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
        Team_00466c20* t = &g_game->players[g_game->viewTeam];
        unsigned int mask = 1 << g_game->viewTeam;
        unsigned char* dst = *(unsigned char**)((char*)g_game->mappedSurface + 0xc);
        unsigned char* src = *(unsigned char**)((char*)g_game->pictureSurface + 0xc);
        int halfWidth = g_game->rowWidth / 2;
        int halfHeight = g_game->mapHeight2 / 2;
        for (int i = 0; i < g_game->height; i++) {
            int mapY = i * halfHeight;
            int mapX = 0;
            for (int j = 0; j < g_game->width; j++, src++, mapX += halfWidth) {
                int index = (mapY / g_game->height) * halfWidth + mapX / g_game->width;
                unsigned char c;
                if (!(unsigned short)(g_game->visibilityMask[index] & mask)) {
                    c = fog;
                } else if (t->seenMap[index]) {
                    c = *src;
                } else {
                    c = g_game->fx->colorMap[*src];
                }
                *dst = c;
                dst++;
            }
        }
        g_game->pending = 1;
    }
}