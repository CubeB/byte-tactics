// Decompiled by space-bunny-free, finished by LongCat 2.5 Preview Free,
// GPT-6.1-sol (#3152 retry): baseline rechecked at 79.8% (294/306); bool-return, caller-early-return, pointer-local and mask-helper trials did not improve it. No MATCH.
// deepseek-v4.1-flash and space-bunny-free, edited by deepseek-v4.1. Names are provisional.
// deepseek-v4.1-flash (#2963 retry): still 79.8% (294 of 306). Fog arm only: the
// original rematerialises width into edx (`mov edx,[edx+0x80]; imul edx,ecx`) and
// keeps a separate per-arm fail block; ours folds width into ebp and tail-merges
// the fail blocks. Restructuring Visible() to an assigned `visible` local is
// byte-identical; moving/removing the width local and hand-spelling the compare
// score 39.4-49.5. The two requirements (a value live across Contains to pin
// `this` to eax, and the width rematerialisation) are mutually exclusive here.
// DEEPSEEK-V4.1-FLASH (sixth pass, 2026-09-30, retry): 9 scratch variants, no
// gain, best stays 79.8 [294]. The one genuinely new lever this pass was
// tools/headers.py run ON the fog-arm-exact variant (the pure 0x408090 body,
// 296 bytes / 26.3 percent): all 128 header sets are flat at 26.3, so the
// pre-branch rotation is not compiler symbol state. Also measured dead:
// a `MapSize*` local (26.3, byte-identical to the pure body, MSVC forwards the
// pointer so no pressure appears), a `Map*` copy local (same), a type-punned
// `*(unsigned int*)((char*)p + 0x80)` width read with the m local (49.5 [298],
// byte-identical to the fresh member read), and `m` declared before the test
// with `w` declared after it (49.5 [298]). So `m` alone with any fresh width
// gives 49.5, `w`+`m` gives 79.8 but merges, and no shape has both. The wall
// is confirmed allocator-only.
// DEEPSEEK-V4.1 (fifth pass, 2026-09-30): 8 scratch variants, no gain, 79.8
// [294] remains best. Two new data points on the fog arm rotation:
//   * a reference to the player record (`Map_004745e0& pl = g_game->
//     players[g_game->playerIndex]`, Contains/index through pl, mask through
//     g_game) is 26.3 [296] and rotates exactly like the no-local body:
//     `this` -> edx, p -> esi, playerIndex -> ecx, rect and pre-branch with
//     them (prologue `mov edx,ecx`). Same byte count and score as the
//     no-local arm, so the rotation follows the fog arm's pointer shape, not
//     the missing live value alone.
//   * a `Game* g = g_game` local live across Contains with fresh
//     `g->visibilityMask[p->explored.size.width * row + col]` and
//     `1 << g->playerIndex` is 18.2 [301], worse (extra register pressure
//     spills the rect).
// Also re-measured, all byte-identical to the file below at 79.8 [294]: a
// `unsigned short` cell local, an `int` cell local, and the m/w declaration
// order swapped (m first). A live w with a hand-spelled Contains against w
// (fresh width in the index) is 39.4 [294], the same rotation as the 49.5
// m-local variant: any spelling that drops the fresh first width read moves
// the pre-branch.

// DEEPSEEK-V4.1 (fourth pass, 9 variants, no code change: the file below is the
// best measured shape, 79.8 percent, 294 of 306 bytes, mask arm byte-identical).
// New measurements, all with check.py --sym on scratch copies, all byte-compare:
//   * the pure 0x408090 body in the fog arm (col, row, Contains, then
//     `g_game->visibilityMask[p->explored.size.width * row + col]`) is 26.3
//     [296] and its fog arm IS the original's shape: fresh width into edx, mask
//     pointer loaded from g_game in edi between the imul and the add, cell into
//     edi (`xor edi,edi; mov di,...`) then copied to edx. But the whole function
//     rotates: this moves to edx, p to esi, playerIndex to ecx, and the rect and
//     pre-branch with them. The `m` local is what pins this to eax, and where it
//     is declared is what pins the load position; no spelling found gives both.
//   * the same with the mask local kept hoisted and a fresh `p->size.width` in
//     the index is 49.5 [298]: the m load stays hoisted into edi, p is esi, and
//     the fresh width FOLDS, `imul edx, dword ptr [esi+0x80]` (the fold lands in
//     the row register, so no separate load and no `mov edx,edi`); a second
//     width local (v8) and the col,row,m,w order (v9) are byte-identical to the
//     file below at 79.8 [294], so the second width read is CSE'd or spilled.
//   * no spelling of the index (index local, address local, `col + w*row`,
//     `row * w + col`, cell local, two width locals) changes the file's bytes.
// The remaining 12 bytes are the allocator picking the file's index temp (ebp,
// the compare's dead width register) instead of edx (the map pointer's).
// SPACE-BUNNY-FREE, third pass. Same 79.8 percent (290 of 306 bytes), but the
// two remaining arms are no longer the same problem: the mask arm is now
// BYTE-IDENTICAL, and all that is left is the fog arm. The lever was the one
// the matched sibling 0x407e90 / 0x475470 already use: the +0x7c cell map is a
// ByteMap {data, size} with the member `unsigned char Get(int x, int y) { return
// data[size.width * y + x]; }`, NOT a `seen` pointer plus a `width` local.
// With Get() inlined, the mask arm compiles to exactly the original's nine
// instructions, including the two late loads (`mov edi,[edx+0x80]`,
// `mov edx,[edx+0x7c]`) and the unfolded `imul edi,ecx; add edi,esi; cmp byte
// [edi+edx],0`. That supersedes three rounds of conclusions below: the four
// mask-arm locals (seen, col, row, w) were never load-bearing, they were a
// workaround for the same lack of register pressure, and the mask pointer
// local in the fog arm is the only one of the two that really is.
//
// WHY THE FOUR LOCALS WERE A DEAD END: they made the allocator put `w` in a
// register and the index temp then multiplies in place into it (`imul
// ebp,ecx`). The original cannot do that: `imul edx,[edx+0x80]` is not
// encodable, so a width that is still in memory has to be materialised into a
// register of its own, and MSVC 5 picks the map pointer's dying register. A
// named local is the one shape that always has a register, so any spelling with
// a local for the width is dead by construction.
//
// WHAT IS LEFT (the fog arm only, 4 instructions):
//   ours: mov edi,[edi+0x14273] (hoisted); ... imul ebp,ecx; add ebp,ebx;
//          xor edx,edx; mov dx,word ptr [edi+ebp*2]
//   orig: ... mov edx,[edx+0x80]; imul edx,ecx; mov ecx,[edi+0x14273];
//          add edx,ebx; xor edi,edi; mov di,word ptr [ecx+edx*2]; mov edx,edi
// The original's fog arm is literally the body of the matched 0x408090
// (width re-read into the dying map pointer register, mask pointer loaded
// after the multiply into the dead row register, cell materialised into dead
// g_game's register with the zero-then-16-bit-load pair), and with no locals
// at all that is exactly what the compiler emits here too, but the frame then
// rotates: the map pointer is computed before the branch into edx and `this`
// ends up in edx as well, so the whole pre-branch block (`xor edx,edx`,
// `mov dl,[edi+0x2a43]`, the 33*index and 0x14b-stride leas) comes out
// differently. Something live across the fog arm's Contains call is what keeps
// `this` in eax, and the mask pointer local hoisted into edi at the top of the
// arm is the only spelling measured that does it (39.4 / 49.5 / 19.2 percent
// for the five alternatives below, all of which rotate the pre-branch block).
// MEASURED THIS PASS, all in this file, all byte-compare with check.py --sym:
//   ByteMap::Get in the mask arm, fog arm col,row,w,m (the current file) 79.8
//     [294], mask arm exact;
//   the same with no fog locals at all (the pure 0x408090 body) 26.3 [296];
//   with only the m local 49.5 [298]; with the m local declared after the
//     Contains call 19.2 [292]; with no m local (g_game->visibilityMask
//     indexed directly) 19.2 [292]; with `unsigned int* wp = &...width` in
//     place of w 49.5 [298]; with the fog compare hand-spelled as
//     `(unsigned int)col >= w` and a fresh width in the index 39.4 [294].
//   So the fog arm needs a value live across Contains AND a width read that is
//   not in a register, and no single declaration gives both.
// DEEPSEEK-V4.1-FLASH: re-confirmed the 79.8 percent file is the optimum of the
// two documented levers. Screened four more arm spellings (scratch only, no new
// file runs): the path arm with no `seen` and no `w` locals (fully folded index)
// is 79.6 [290], the same arm with `seen` only is 76.1 [292], with `w` only is
// 70.1 [288], and moving the mask pointer local after the Contains test, or
// dropping it for `g_game->visibilityMask[...]` directly, is 68.3 [292] and
// rotates the pre-branch block. So both named locals in the path arm and the
// mask-pointer local in the mask arm are load-bearing; nothing beats 79.8.
// DEEPSEEK-V4.1-FLASH, third pass (second-pass retry). Still 79.8, 290 of 306.
// Screened the one lever the notes left open, pushing the index temp into edx,
// and it is now measured dead. `uv run tools/headers.py 0x4745e0` tried all 128
// header sets: best 79.8, no set changes the match, so the header lever is
// exhausted. The mask arm with the width local used only by the compare and a
// fresh `p->size.width` re-read in the index (Contains spelled out by hand) is
// 76.1 [292]: it does force the re-read, but the `w` local is exactly what puts
// the width in edi, so a fresh read loses that register, `seen` moves into
// edi, and the index folds anyway. With no mask-arm locals at all the fresh
// index folds to `imul ecx,[edx+0x80]; add ecx,[edx+0x7c]; cmp [ecx+esi],0`
// (79.6 [290]). Reordering the mask arm's locals to col,row,seen,w is 78.8
// [290]. In the fog arm a fresh `p->size.width` in the index is 45.5 [294].
// So the remaining 7 lines are genuinely a register-allocator choice: the
// original rematerialises width into edi and loads seen into edx in the body
// while ours keeps seen in ebx and copies the width to ebp, and every source
// shape that changes one of those rotates the pre-branch block.
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
//   SUPERSEDED FOR THE MASK ARM by ByteMap::Get (see the top of this file);
//   what is written below still describes the fog arm.
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

struct ByteMap_004745e0 {
    unsigned char* data;            // +0x7c
    MapSize_004745e0 size;          // +0x80

    unsigned char Get(int tx, int ty) { return data[size.width * ty + tx]; }
};

struct Map_004745e0 {               // one entry of g_game->players
    char unknown_0[0x7c];
    ByteMap_004745e0 explored;      // +0x7c
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
// its own sub-struct here, not through the record's fields. The mask arm must
// go through ByteMap::Get (see ByteMap_004745e0) or its index folds and it
// stops matching; the mask pointer local `m` in the fog arm is what buys edi
// for g_game and holds the pre-branch block in place, and the two arms' local
// orders are not free. See the top of the file for what is still open.
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
            int col = x >> 5;
            int row = (y - (height >> 1)) >> 5;
            if (p->explored.size.Contains((unsigned int)col, (unsigned int)row) &&
                p->explored.Get(col, row))
                return 1;
            return 0;
        }
        int col = x >> 5;
        int row = (y - (height >> 1)) >> 5;
        unsigned int w = p->explored.size.width;
        unsigned short* m = g_game->visibilityMask;
        if (!p->explored.size.Contains((unsigned int)col, (unsigned int)row))
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
