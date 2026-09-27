// Decompiled by space-bunny-free. Names are provisional.
// PARTIAL (18.2%, best of 3 check.py runs; see notes below).
// Two lookups: is the cell at (x,y) or at (x+dx,y+dy) visible in the player's
// fog map?  When the game flag at g_game+0x14281 has bit 1 set the per-player
// byte map at +0x7c is used, otherwise the shared visibility bit mask at
// +0x14273 with this player's bit (g_game+0x2a43).
// What still differs from the original:
//  - The original zeroes three dword locals in its prologue that it never
//    reads (sub esp,0xc; xor eax,eax; mov [esp],eax; ... mov [esp+8],eax;
//    ... mov [esp+0x14],eax, interleaved with the register pushes).  No
//    source I tried reproduces those three dead stores.
//  - It loads the flags as a word (mov ax, word ptr [ecx+0x14281]) while
//    every spelling of `flags & 2` I tried narrows that load to a byte; the
//    original's 16-bit and/compare/store say the field is 16-bit there.
//  - It shifts 32-bit (movsx edx,di; sar edx,5; movsx ecx,bx; sub ecx,esi;
//    sar ecx,5); mine narrows the shift to 16 bits and sign-extends the
//    result instead, whichever of int or short locals I use.
//  - It materialises the first point's 0/1 result branchily into edx
//    (mov edx,1 / xor edx,edx / test edx,edx) with one shared zero block
//    for both range checks and the grid byte; mine uses setne.

#pragma pack(push, 1)

struct Map_004658e0 {
    char unknown_0[0x7c];
    unsigned char* grid;               // +0x7c
    unsigned int width;                // +0x80
    unsigned int height;               // +0x84
};

struct Game_004658e0 {
    char unknown_0[0x2a43];
    unsigned char playerIndex;         // +0x2a43
    char unknown_2a44[0x14273 - 0x2a44];
    unsigned short* visibilityMask;    // +0x14273
    char unknown_14277[0x14281 - 0x14277];
    unsigned short flags;              // +0x14281
};

#pragma pack(pop)

extern Game_004658e0* g_game;

// FUNCTION: 0x4658e0
int __stdcall FUN_004658e0(Map_004658e0* map, int x, int y, int dx, int dy, short size)
{
    int sx = (short)(x << 4);
    int sy = (short)(y << 4);
    unsigned short flag = g_game->flags & 2;
    int vis;
    if (flag == 2) {
        vis = (unsigned int)(sx >> 5) < map->width &&
              (unsigned int)((sy - (size >> 1)) >> 5) < map->height
                  ? map->grid[map->width * ((sy - (size >> 1)) >> 5) + (sx >> 5)] != 0
                  : 0;
    } else {
        vis = (unsigned int)(sx >> 5) < map->width &&
              (unsigned int)((sy - (size >> 1)) >> 5) < map->height
                  ? (g_game->visibilityMask[map->width * ((sy - (size >> 1)) >> 5) + (sx >> 5)] &
                     (1 << g_game->playerIndex)) != 0
                  : 0;
    }
    if (vis)
        return 1;

    sx = (short)(sx + (dx << 4));
    sy = (short)(sy + (dy << 4));
    if (flag == 2) {
        if ((unsigned int)(sx >> 5) < map->width &&
            (unsigned int)((sy - (size >> 1)) >> 5) < map->height)
            return map->grid[map->width * ((sy - (size >> 1)) >> 5) + (sx >> 5)] != 0;
        return 0;
    }
    if ((unsigned int)(sx >> 5) < map->width &&
        (unsigned int)((sy - (size >> 1)) >> 5) < map->height)
        return (g_game->visibilityMask[map->width * ((sy - (size >> 1)) >> 5) + (sx >> 5)] &
                (1 << g_game->playerIndex)) != 0;
    return 0;
}
