// Decompiled by space-bunny-free. Names are provisional.
//
// Copy this to 0x474170, only the // FUNCTION: annotation changes.
//
// 70.7 percent, 297 of 301 bytes. The whole prologue/epilogue, the argument
// pushes and the call sequence match; what is left is six instructions.
//
// WHAT IS SOLVED, and each piece was needed to get the next one:
// - Player stride is 0x14b (331), not 0x14a. `lea edx,[ebx+edx*2+0x1b63]`
//   with ebx = g_game + i is i + 330i, and the extra `lea ebx,[ecx+edi]` is
//   not dead code, it is the base of that address. Stride 0x14a gives a
//   different, wrong multiply chain.
// - `height` is a plain `short` (+0xa), read with a 16-bit load and `sar`, as
//   in the matched 0x475040.cpp. The `jae` comes from the map width being
//   `unsigned int` in `MapSize::Contains`, not from height.
// - The header and the two arms must read the position through two
//   structurally different expression trees, or MSVC's (global) load CSE hands
//   the arms the header's 16-bit values and the original's three fresh
//   `movsx` per arm never appear (that is the 16.7 percent shape). The nested
//   `Pos` sub-struct for the header plus a `Pos* q = &pos` local for the arms
//   does it, and so does the reverse, and so does `short* s = (short*)&pos` in
//   the header (that last one compiles to byte-identical code, so the sub-struct
//   is the honest spelling rather than the only one). What does not work: a
//   union of the two spellings, a 16.16 `Pos` at +4, the member in both, or any
//   statement order.
// - The arms need `int` col/row compared as `(unsigned int)`: that is what
//   gives the 32-bit `sar edi,5` rather than a 16-bit shift plus `movsx`.
//   `unsigned int col` gives 301 bytes but the wrong (16-bit) shift.
// - `MapSize::Contains` used inline in the body, as in 0x408090.cpp, is what
//   duplicates the width load; the mask arm needs `if (!Contains) ... else ...`
//   for its `jae` / `jb`-into-body, the fog arm needs the `&&` chain for its
//   `mov reg,1` / `xor reg,reg`.
// - The header's `sx` statement must come before `sy`.
//
// THE ONE HACK, and it is not the original's source: the extra `else if (row)`
// in the mask arm. It adds a basic block and nothing else, and that alone moves
// the whole register allocation onto the original's: g_game into ecx (not
// hoisted to the top), the index copied into edi, `lea ebx,[ecx+edi]`, the
// flags byte into cl, `this` still in eax, sx finished in place in bp. Without
// it the same source scores 40.0 percent with g_game hoisted into ebx and every
// register below the branch shifted by one. The original has no such test, so
// this shape cannot be the true source; it is here because it reproduces far
// more of the original's code than anything else found so far, and the block
// boundary it creates is the thing to reproduce some other way.
//
// WHAT STILL DIFFERS (297 vs 301 bytes):
// - `mov [esp+0xc],ebp` / `mov ebp,[esp+0xc]`: the mask arm reuses ebp for
//   col, so sx is spilled to the py parameter slot. The original instead keeps
//   sx in ebp and spills col to that same slot
//   (`mov dword ptr [esp+0x18],ecx` ... `mov ebx,[esp+0x18]`). One value of
//   too much pressure in the mask arm, and it picks the other victim.
// - `test ecx,ecx` / `je` and one `jmp`: the row test from the hack above.
// - The mask arm reuses the `Contains` width load for its `imul`; the original
//   loads it again (`mov edx,[edx+0x80]; imul edx,ecx`). The fog arm does the
//   same thing but folds the second load into the `imul` instead.
// Note for whoever lifts this: declaring an extra `unsigned int` local between
// the arm's `col`/`row` declarations and the arm's expression (a `w1` in the fog
// arm, an index temp in the mask arm) makes MSVC 5 die with no diagnostic at
// all, so that spelling is not available.
// The fog arm already matches instruction for instruction apart from the width
// reload, which is folded into the `imul` here.
//
// Ruled out, all measured here: static inline getters per g_game field, g_game
// in a local, flags/playerIndex in locals, the map pointer via a Player* plus
// &p->map, computing the player pointer in each arm (318 bytes, the address is
// not shared), the visible test as a member of the record (not inlined at all),
// the test as two static inline arm helpers (39.6 percent), the header inside
// `if (visible)` (20.9), the header in a static inline helper (15.8, the CSE
// comes straight back), tools/headers.py over all 128 header sets (39.6 for
// every one of them), and attacking the hoisted g_game load by any spelling.
#include <stddef.h>

int __stdcall FUN_004b7f30(void* a, int b);
void __stdcall FUN_004b8500(void* dest, void* src, int x, int y);

#pragma pack(push, 1)
struct MapSize_00473590 {
    unsigned int width;                 // +0x0
    unsigned int height;                // +0x4

    int Contains(unsigned int tx, unsigned int ty)
    {
        return tx < width && ty < height;
    }
};

struct Map_00473590 {
    char unknown_0[0x7c];
    unsigned char* fogMap;              // +0x7c
    MapSize_00473590 size;              // +0x80
};

struct Player_00473590 {
    Map_00473590 map;                   // +0x00, 0x88 bytes
    char unknown_88[0x14b - 0x88];
};

struct Game_00473590 {
    char unknown_0[0x1b63];
    Player_00473590 players[11];        // +0x1b63, stride 0x14b
    char unknown_299c[0x2a43 - 0x299c];
    unsigned char playerIndex;          // +0x2a43
    char unknown_2a44[0x14273 - 0x2a44];
    unsigned short* visibilityMask;     // +0x14273
    char unknown_14277[0x14281 - 0x14277];
    unsigned char flags;                // +0x14281, bit 1 (mask 2)
};
#pragma pack(pop)

extern Game_00473590* g_game;

struct Pos_00473590 {
    short x;                            // +0
    char unknown_2[2];
    short h;                            // +4
    char unknown_6[2];
    short y;                            // +8
    char unknown_a[4];
};

class Class_00473590 {
public:
    void* data;                         // +0x00
    char unknown_4[0x6 - 0x4];
    Pos_00473590 pos;                   // +0x06
    int field_2c;                       // +0x2c
    char unknown_30[0x34 - 0x30];

    void FUN_00473590(void* dest, short px, short py);
};

// FUNCTION: 0x473590
void Class_00473590::FUN_00473590(void* dest, short px, short py)
{
    short sx = pos.x - px + 0x80;
    short sy = pos.y - (pos.h >> 1) - py + 0x20;
    Pos_00473590* q = &pos;
    Map_00473590* m = &g_game->players[g_game->playerIndex].map;
    int visible;
    if ((g_game->flags & 2) == 2) {
        int col = q->x >> 5;
        int row = (q->y - (q->h >> 1)) >> 5;
        visible = m->size.Contains(col, row) && m->fogMap[m->size.width * row + col] != 0;
    } else {
        int col = q->x >> 5;
        int row = (q->y - (q->h >> 1)) >> 5;
        unsigned int w2 = m->size.width;
        if (!m->size.Contains(col, row)) {
            visible = 0;
        } else if (row) {
            visible = (g_game->visibilityMask[w2 * row + col] &
                       (1 << g_game->playerIndex)) != 0;
        } else {
            visible = 0;
        }
    }
    if (visible) {
        FUN_004b8500(dest, (void*)FUN_004b7f30(data, field_2c), sx, sy);
    }
}
