// Decompiled by space-bunny-free, finished by LongCat 2.5 Preview Free. Names are provisional.
// LONG-CAT 2.5 PREVIEW FREE, second pass: 66.3 -> 79.8 percent, 290 of 306
// bytes. Four check.py runs on the file, about 80 scratch scorings of variants
// under build/scratch/0x4745e0/. NOT A MATCH, but the whole prologue, the rect,
// the pre-branch block, the flags test, both arms' compare chains, the mask
// arm's register choices and the entire call sequence are now byte-identical,
// and the first divergence left is a jump target.
//
// THE TWO FACTS THAT UNLOCKED IT, both about register PRESSURE, not spelling
//   1. Write the two arm bodies straight into `Visible`, and give the MASK arm
//      the sibling 0x474b80's four locals in its order: seen, col, row, w.
//      That alone is 68.3 [292] and it is what finally puts `g_game` in edi.
//      Nothing in three rounds had managed that, and the pre-branch block was
//      otherwise already byte-identical, so every difference in both arms came
//      from that one register. The combination is what matters, not either
//      part: the same four locals in a `static inline` helper score 64.0, and
//      the inline shape with no locals scores 19.4.
//   2. Add a fifth local to the FOG arm, `unsigned short* m =
//      g_game->visibilityMask`. Worth 11.5 more points (68.3 -> 79.8, 292 ->
//      290) and it is the first thing that has ever moved the fog arm at all
//      in this file. Its declaration order in the arm is a real lever and only
//      two orders reach 79.8: col,row,w,m and col,row,m,w. With w before the
//      mask pointer but col/row/w permuted otherwise it drops to 75.8, and any
//      order starting with w is 45 or worse.
//   Together they also make the arms' col and width land in the original's
//   registers: the fog arm has col in ebx, width in ebp, exactly as the
//   original, where before this pass they were swapped.
//
// WHAT IS LEFT (7 lines, all in the two index computations)
//   * mask arm: the cost of the four locals. Ours loads `seen` before the
//     tests (`mov ebx,[edx+0x7c]`) and copies the width for the compare
//     (`mov ebp,edi`; `cmp esi,ebp`); the original loads the width again after
//     the tests into the register the width already had (`mov edi,[edx+0x80]`,
//     so `cmp esi,edi` matches) and dies its ptr register into `seen` (`mov
//     edx,[edx+0x7c]`, so `cmp byte [edi+edx],0`). The index expression itself
//     is now right: `imul edi,ecx; add edi,esi` is the original's association.
//   * fog arm: the same thing one step on. Ours keeps the compare's width
//     register for the index (`imul ebp,ecx; add ebp,ebx`) and folds the
//     `m` load early into edi; the original re-reads the width into the dying
//     ptr register (`mov edx,[edx+0x80]; imul edx,ecx`), loads `m` into ecx
//     after the multiply, and materialises the 16-bit cell into di (g_game's
//     dead register) and copies it (`xor edi,edi; mov di,...; mov edx,edi`),
//     where ours zeroes edx and loads dx directly.
//   * so one lever is left in both arms: the index temp has to land in the
//     register the ptr dies in (edx), which is also what forbids MSVC folding
//     the two memory operands into the imul and the add.
//
// MEASURED AND REJECTED THIS ROUND (byte counts in brackets; all of these were
// screened with `check.py --sym` on a scratch file, they are not real runs)
//   * the sibling 0x474b80's own file shape: one function, `map` and
//     `int visible` locals at the top, both arms inline, seen/col/row/w in the
//     mask arm. 20.9 to 24.5 [271-292]. The top-level `visible` local is what
//     rotates the pre-branch block. Do not retry it here.
//   * the inline shape with no mask-arm locals: 19.4 [288]; the same shape with
//     a reference or a pointer to the size sub-struct: 72.1 and 64.6 [288/292];
//     a `const` map pointer does not compile in VC5 source mode.
//   * mask arm local sets and orders, all 24 orders of seen/col/row/w: seen,
//     col, row, w is the only one that reaches 68.3, the rest 62.3 to 67.3
//     [292]. With col,row only: 79.6 [290], two tenths under the best and with
//     a folded index. col,row,w: 70.1 [288]. seen,col,row: 76.1 [292].
//   * mask arm shapes: nested `if (Contains) return ...` (which is what the
//     original's single shared fail block looks like) 69.7 to 78.4 [294] and
//     it emits `jb`+`setne`, not the original's `je`; the same with a body
//     local for `seen` 69.7; `seen` assigned inside the condition 70.1 [288];
//     a `MapSize&` local 72.1 [288]; `Contains` spelled out by hand so the
//     `w` local is shared with the compare 72.1, with no `seen` local 42.1, and
//     with the height also named 70.7; a nested `if` with no locals at all 24.0
//     [298]; the index as a local before the test 60.3, after it 66.3.
//   * fog arm local sets: col,row 52.5, col,row,w 68.3, col,row,m 45.5,
//     col,row,w,m and col,row,m,w 79.8, col,row,hh,m 45.5, and a fifth local
//     (player index 60.3, height, index, cell value or cell address) either
//     byte-identical or worse. All 24 orders of col,row,w,m were measured: 79.8
//     for the two above, 75.8 for every order that starts col,row, 45 for the
//     rest.
//   * types and spellings that are byte-identical here and can be ignored:
//     `int w` in either arm, `const unsigned char* seen`,
//     `const unsigned short* m`, `col + w*row` and `row * w + col` instead of
//     `w*row + col`, an index local in the fog arm, a 16-bit `cell` local in
//     the fog arm, a cell-address local in the mask arm, `&0xff` on the seen
//     test, `unsigned int Contains`, `int Contains` with the casts inside, and
//     `int i = w*row+col` in both arms.
//   * the OLD helper shape, all still 66.3 or less: unsigned col/row 60.0
//     [301], a local `int hy` for height>>1 56.9 [291], a `unsigned short* m`
//     local for the mask 49.5 [294], a `seen` local 64.0 [299], `w`+`seen`
//     65.7 [297], a `Game*` parameter for the fog arm, an index local, a
//     nested `if`, col/row computed before the map pointer 17.9 [271], the
//     arms as methods of the map struct, arms taking the map by reference
//     (does not compile), `wcr` and `cwr` orders in the fog arm 47.0 [302].
//
// DEAD ENDS, do not repeat
//   * An inlined function boundary is not a CSE boundary in MSVC 5: splitting
//     the width read into a helper, or putting each arm in its own
//     `static inline`, changes nothing about which loads survive. Two
//     independent rounds and the sibling agree.
//   * MSVC 5 can only spill a named local, never rematerialise it. So a local
//     always costs a `mov` (or a fold) and never buys the second read the
//     original has; and no spelling of a local-free index avoids the fold,
//     because the fold is decided by the register the index temp gets, not by
//     the source.
//   * The arguments are settled, do not re-check them: the decorated name is
//     `?FUN_004745e0@Class_004745e0@@QAEXPAXFF@Z` and in MSVC's mangling F is
//     `short`, not float, so (void*, short, short) is right; `ret 0xc` and the
//     three dword slots at [esp+0x24], [esp+0x28], [esp+0x2c] confirm the
//     order surface, px, py, and the px/py reads really are 16-bit.
//   * From the previous rounds, all still valid: the position must be reached
//     through its own sub-struct in the arms while the header reads the
//     members (or the other way round), the stride is 0x14b, the fog-flags
//     test is `(flags & 2) == 2`, and col/row must be `int` computed with
//     `>> 5`.
#include <stddef.h>

struct Rect_004b0510 {
    int x1;                          // +0x0
    int y1;                          // +0x4
    int x2;                          // +0x8
    int y2;                          // +0xc
};

#pragma pack(push, 1)
struct MapSize_004745e0 {
    unsigned int width;             // +0x80
    unsigned int height;            // +0x84

    int Contains(unsigned int tx, unsigned int ty)
    {
        return tx < width && ty < height;
    }
};

struct Map_004745e0 {               // one entry of g_game->players
    char unknown_0[0x7c];
    unsigned char* seen;            // +0x7c
    MapSize_004745e0 size;
    char unknown_88[0x14b - 0x88];
};

struct Game_004745e0 {
    char unknown_0[0x1b63];
    Map_004745e0 players[1];         // +0x1b63
    char unknown_1[0x2a43 - 0x1b63 - 0x14b];
    unsigned char playerIndex;       // +0x2a43
    char unknown_2a44[0x14273 - 0x2a44];
    unsigned short* visibilityMask;  // +0x14273, one bit per player
    char unknown_14277[0x14281 - 0x14277];
    unsigned char flags;             // +0x14281, bit 1 (mask 2)
};
#pragma pack(pop)

extern Game_004745e0* g_game;

void __stdcall FUN_004bf6f0(void* surface, Rect_004b0510* rect, int color);

// The record's position, with the inlined visibility test that reads it. The
// two arms re-read x, height and y from the record instead of reusing the
// values the caller just computed, so the position has to be reached through
// its own sub-struct here, not through the record's fields. The four locals in
// the mask arm (seen, col, row, w, in that order) and the mask pointer local
// in the fog arm are what buy edi for g_game, and they are worth 13.5 points
// between them; see the top of the file. They must be spelled here, not in an
// inlined helper, and the two arms' local orders are not free.
#pragma pack(push, 1)
struct Pos_004745e0 {
    short x;                        // +0
    char unknown_2[2];
    short height;                   // +4
    char unknown_6[2];
    short y;                        // +8

    int Visible()
    {
        Map_004745e0* p = &g_game->players[g_game->playerIndex];
        if ((g_game->flags & 2) == 2) {
            unsigned char* seen = p->seen;
            int col = x >> 5;
            int row = (y - (height >> 1)) >> 5;
            unsigned int w = p->size.width;
            if (p->size.Contains((unsigned int)col, (unsigned int)row) &&
                seen[w * row + col] != 0)
                return 1;
            return 0;
        }
        int col = x >> 5;
        int row = (y - (height >> 1)) >> 5;
        unsigned int w = p->size.width;
        unsigned short* m = g_game->visibilityMask;
        if (!p->size.Contains((unsigned int)col, (unsigned int)row))
            return 0;
        return (m[w * row + col] &
                (1 << g_game->playerIndex)) != 0;
    }
};
#pragma pack(pop)

class Class_004745e0 {              // vector element (see 0x473250.cpp)
public:
    char unknown_0[6];
    Pos_004745e0 pos;               // +0x6
    char unknown_10[0x30 - 0x10];
    int color;                      // +0x30
    char unknown_34[0x44 - 0x34];

    void FUN_004745e0(void* surface, short px, short py);
};

// Draws this record's one-pixel mark at its projected screen position, but
// only when its world cell is visible to the local player: the per-player
// byte map (players[i] +0x7c, +0x80, +0x84) when bit 1 of the flag byte at
// +0x14281 is set, else that player's bit in the global short map at
// +0x14273.
// FUNCTION: 0x4745e0
void Class_004745e0::FUN_004745e0(void* surface, short px, short py)
{
    Rect_004b0510 r;
    short sx = pos.x - px;
    short sy = pos.y - py;
    r.x1 = sx + 0x80;
    r.y1 = sy - (pos.height >> 1) + 0x20;
    r.x2 = r.x1 + 1;
    r.y2 = r.y1 + 1;
    if (pos.Visible())
        FUN_004bf6f0(surface, &r, color);
}
