// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// PARTIAL: the original is 402 bytes, ours is 402 bytes; the whole prologue is
// instruction-identical and only the frame slot numbers and the resulting
// register rotation differ.
// What is now fixed (was 67.2%):
//  * `lea eax,[esi+ecx]`: index `g_game->players` with `g_game->viewTeam`
//    directly (a byte-typed reload), not an `int team` local. With an int local
//    MSVC emits `mov eax,esi; add eax,ecx`; with the byte index it emits the
//    original's 2-register LEA.
//  * `and eax,edi; test ax,ax`: cast the visibility test to `unsigned short`,
//    `(unsigned short)(g_game->visibilityMask[index] & mask)`. Without the cast
//    MSVC emits `test edx,eax` and puts the pixel in al.
// What still differs:
//  1. Frame slot rotation. The original is dst=+0x4, src=+0x8, mapY=+0xc and
//     the inner counter j=+0x10. Ours is src=+0x4, mapY=+0x8, dst=+0xc,
//     j=+0x10 (i, mask, t and halfHeight already land where the original has
//     them). Every declaration/scope/type/name permutation tried leaves this
//     rotation unchanged; it appears to be the register allocator's spill
//     order, not anything source-visible.
//  2. Because of 1 the loop body's scratch registers rotate one place:
//     the seenMap chain is `edx/ eax` here vs `eax/edx` there, the pixel lives
//     in al vs cl, and the store is `mov [ecx],al` vs `mov [edx],cl`.
//  3. The loop-bottom schedule (original loads i, src, dst then interleaves the
//     three increments around the store; ours loads dst, src, j).
// Structure that does match: bit 2 test/clear, bit 1 (pending) set at the end,
// the esi/ebp/ebx/edi save order, the `height > 0` wrapper, the outer loop as a
// do/while (counter stored before the wrapper) with the inner `for` rotated
// into a guarded form, `x / 2` as cdq/sub/sar, and `fog` sunk into the else
// branch.

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
        int i = 0;
        if (g_game->height > 0) {
            int mapY = 0;
            do {
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
                mapY += halfHeight;
            } while (++i < g_game->height);
        }
        g_game->pending = 1;
    }
}
