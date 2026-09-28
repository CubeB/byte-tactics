// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
//
// 84.0 percent, 309 of 301 bytes (up from the 70.7 percent of the earlier
// space-bunny-free attempt). The prologue, the sx/sy header, the player-pointer
// arithmetic and the flags test are byte-identical; the whole call sequence is
// identical. Two spots in the two test arms are left, and they are the same
// two spots that the matched sibling family fights with (0x474b80, 0x4745e0,
// 0x473a00, 0x474170 all have this header and this pair of arms).
//
// WHY THIS SHAPE: the arms must be under enough register pressure to stop the
// backend hoisting the shared `pos.x` load out of the branch. That hoist makes
// six values live at the `jne` instead of five and rotates every pre-branch
// register (it is what made the local-free spellings sit at 39-40 percent,
// with `g_game` hoisted into ebx at the top). Naming the arm fields in locals
// is the only thing found that stops it: the fog arm needs `seen`, col, row, w
// in that order and the mask arm needs col, row, w. Removing any one drops
// straight back to 38-43 percent. The cost is the two spills below.
//
// THE TWO REMAINING DIFFS:
//   * fog arm: ours keeps `seen` live across the bounds test, so it parks it
//     in the dead px slot (`mov [esp+0x18],ecx`) and reloads it for the final
//     byte compare (`mov edx,[esp+0x18]; add ebx,edi; cmp byte [ebx+edx],0`).
//     The original rematerialises: `mov ebx,[edx+0x80]` a second time for the
//     imul and `add ebx,[edx+0x7c]` with the seen byte as a memory operand,
//     then `cmp byte [ebx+edi],0`. The original's width is also loaded twice;
//     ours loads it once because of the `w` local.
//   * mask arm: ours spills the `w` local to [esp+0x1c] and reloads it for the
//     imul; the original just re-reads `mov edx,[edx+0x80]`. Everything else
//     in the mask arm, including the col spill to [esp+0x18] and the
//     `jae` / `jb`-into-body shape, matches instruction for instruction.
//
// The root cause is one question the family notes have not answered: how the
// player pointer lands in EDX with five values live at the branch and no arm
// local, since the no-local spellings fold `imul reg,[edx+0x80]` while the
// original materialises `mov edx,[edx+0x80]; imul edx,ecx`. The local-free
// form, one arm helper per arm, a fresh Pos* per arm, explicit >= / || bounds
// tests, a body-local w, and inlined width helpers were all measured here and
// none beats this. The header reads the position through `q` and the arms
// through `pos`, which is what keeps the three `movsx` per arm (either
// direction of that split works); `int` col/row and the `(unsigned int)`
// casts in Contains are required for the 32-bit `sar`.
#include <stddef.h>

void* __stdcall FUN_004b7f30(void* a, int b);
void __stdcall FUN_004b8500(void* dest, void* src, int x, int y);

#pragma pack(push, 1)
struct Pos_00473590 {
    short x;                       // +0x00 (record +0x06)
    char unknown_2[0x4 - 0x2];
    short h;                       // +0x04 (record +0x0a)
    char unknown_6[0x8 - 0x6];
    short y;                       // +0x08 (record +0x0e)
};

struct MapSize_00473590 {
    unsigned int width;            // +0x80
    unsigned int height;           // +0x84

    int Contains(int col, int row)
    {
        return (unsigned int)col < width && (unsigned int)row < height;
    }
};

struct Player_00473590 {
    char unknown_0[0x7c];
    unsigned char* seen;           // +0x7c
    MapSize_00473590 size;         // +0x80
    char unknown_88[0x14b - 0x88];
};

struct Game_00473590 {
    char unknown_0[0x1b63];
    Player_00473590 players[10];   // +0x1b63, stride 0x14b
    char unknown_2851[0x2a43 - 0x2851];
    unsigned char playerIndex;     // +0x2a43
    char unknown_2a44[0x14273 - 0x2a44];
    unsigned short* visibilityMask;// +0x14273
    char unknown_14277[0x14281 - 0x14277];
    unsigned char fogFlags;        // +0x14281, bit 1 (mask 2)
};

struct Class_00473590 {
    void* data;                    // +0x00
    char unknown_4[0x6 - 0x4];
    Pos_00473590 pos;              // +0x06
    char unknown_10[0x2c - 0x10];
    int field_2c;                  // +0x2c
    void FUN_00473590(void* dest, short px, short py);
};
#pragma pack(pop)

extern Game_00473590* g_game;

// FUNCTION: 0x473590
void Class_00473590::FUN_00473590(void* dest, short px, short py)
{
    // The header reads the position through the pointer, the arms through the
    // member: two different expression trees, so the arms re-read the three
    // shorts with movsx instead of sharing the header's 16-bit loads.
    Pos_00473590* q = &pos;
    short sx = q->x - px + 0x80;
    short sy = q->y - (q->h >> 1) - py + 0x20;
    Player_00473590* map = &g_game->players[g_game->playerIndex];
    int visible;
    if ((g_game->fogFlags & 2) == 2) {
        // seen, col, row, w: this order and these locals are what keep the
        // player pointer in edx and stop the backend hoisting pos.x.
        unsigned char* seen = map->seen;
        int col = pos.x >> 5;
        int row = (pos.y - (pos.h >> 1)) >> 5;
        unsigned int w = map->size.width;
        if (map->size.Contains(col, row) && seen[w * row + col] != 0)
            visible = 1;
        else
            visible = 0;
    } else {
        int col = pos.x >> 5;
        int row = (pos.y - (pos.h >> 1)) >> 5;
        unsigned int w = map->size.width;
        if (!map->size.Contains(col, row))
            visible = 0;
        else
            visible = (g_game->visibilityMask[w * row + col] &
                       (1 << g_game->playerIndex)) != 0;
    }
    if (visible)
        FUN_004b8500(dest, FUN_004b7f30(data, field_2c), sx, sy);
}
