// Decompiled by space-bunny-free. Names are provisional.
//
// NOT a match: 84.6 percent, 309 of 303 bytes. Three check.py runs in all (one
// on the 84.0 version this file started from, two to confirm this one), plus
// over 150 scratch scorings of variants, including a full 24x3 grid of the
// declaration orders inside the two arms. The prologue, the pre-branch block,
// the branch, both arms' compare chains, the mask arm's block layout and the
// whole call sequence are byte-identical. Six instructions of difference are
// left, in three groups:
//   * fog arm: the original REMATERIALISES the two fields it needs after the
//     tests (`mov ebx,[edx+0x80]; mov edx,[edx+0x7c]; imul ebx,ecx;
//     add ebx,edi; cmp byte [ebx+edx],0`). This source keeps `seen` and `w` in
//     named locals, so it loads `seen` into ecx, parks it in the dead `dest`
//     argument slot at [esp+0x18] (`mov ecx,[edx+0x7c]; mov [esp+0x18],ecx`)
//     and reloads it after the tests (`mov edx,[esp+0x18]`) instead of one
//     `mov edx,[edx+0x7c]`. The reload into edx and the whole tail after it
//     (`add ebx,edi; cmp byte [ebx+edx],0`) are already identical to the
//     original. Also missing: the original's second width read at 0x474c04,
//     which only exists because its width is not a local (see THE MECHANISM).
//     Declaring `seen` before `col`/`row` rather than after them is what buys
//     the reload-into-a-register instead of a fold of the spill slot into the
//     add (`add ebx,[esp+0x18]`, the 84.0 version) - worth 0.6 points and one
//     line of diff, and the only reason this ordering is used.
//   * mask arm: the same for the width local (`mov ebx,[edx+0x80]; mov
//     [esp+0x1c],ebx` before the tests, `mov edx,[esp+0x1c]` after them)
//     where the original just re-reads `mov edx,[edx+0x80]`.
//   * one jmp shorter: the two `xor edx,edx; jmp` fail blocks are tail-merged
//     into one that sits between the mask arm's tests and its body. The
//     original keeps one per arm, the fog one right after the fog arm's
//     `mov edx,1`. Almost certainly a symptom of the two spill stores.
//
// THE MECHANISM, which is new and is the thing to know before spending more
// budget here. The arms are NOT wrong because of CSE, and the two width loads
// are NOT one value that MSVC splits. In every spelling with no width local,
// including the plain `map->size.width * row + col`, the fog arm already
// emits TWO loads of the width: one materialised for `cmp edi,ebx` and one
// inside the `imul`. The only difference is the SECOND one: MSVC folds it
// (`imul ecx,[edi+0x80]; add ecx,[edi+0x7c]; cmp byte [ecx+edx],0`) where the
// original materialises it. The fold is possible only because those spellings
// put the player pointer in EDI and the index accumulator in EDX, so
// `imul edx,[edi+0x80]` has a base register distinct from its destination. In
// the original the pointer is in EDX and the accumulator reuses EDX, and MSVC
// does not fold a memory operand whose base is the destination register, so
// it has to materialise `mov edx,[edx+0x80]` first - and the same for the fog
// arm's `seen`, which becomes the base register of the final `cmp`. So the
// rematerialisation in the original is not a source-level trick at all: it is
// a consequence of the player pointer landing in EDX, and the arms' load
// placement is already correct in the local-free shape. Everything reduces to
// ONE question: how does the player pointer get into EDX with four values
// live across each arm's test.
//
// WHY THE FOUR-VALUE SPELLING CANNOT GET EDX. Every local-free spelling puts
// the player pointer in EDI and the index in ECX, and the whole pre-branch
// allocation rotates with it (g_game hoisted into ebx at the top of the
// prologue, the index staying in ecx, the tail reloading dest into eax).
// Those spellings score 24 to 47 percent, never 84. The cause is now known,
// and it is NOT register priority: a throwaway extra use of `g_game` or of
// `g_game->playerIndex` inside either arm changes nothing at all, the code
// stays byte-identical (84.0 with the locals, 39.4 without), so the guide's
// "the original may have used that variable once more in a way that folds
// away" is a dead end here. The cause is that MSVC hoists the `pos.x` load
// that both arms share into the pre-branch block (`movsx edx,[eax+6]` lands
// between `cmp cl,2` and the `jne`), which makes SIX values live at the
// branch instead of five and rotates the pre-block registers. That hoist is a
// BACKEND pass, so no source spelling stops it: a fresh `Pos* r = &pos` in
// each arm, `CellX(&pos)` helpers, one `static inline` helper per arm that
// computes col and row itself, an accessor method on the player
// (`map->At(col,row)`), and a pointer-to-MapSize spelling all give the same
// 287-byte code. Only register pressure in the arms stops it, and the `w` and
// `seen` locals are exactly that pressure - the same two locals that then
// have to be spilled. One requirement, not two, and the original satisfies it
// somehow with four values per arm and no hoist; that is what is missing.
//
// INLINED-CALLEE IDEA: TESTED AND DEAD, do not repeat it. Splitting the
// width read into a second inlined helper beside `Contains`, with the local
// and the helper in either order, changes nothing: the two reads already
// survive, so there is nothing for a callee boundary to break. An inlined
// function boundary is also NOT a CSE boundary in MSVC 5 - the cross-arm
// hoist above runs after inlining, so putting each arm in its own
// `static inline` function changes nothing either. Confirms the same result
// reached independently at 0x4745e0.
//
// ALSO ESTABLISHED, for this family (0x4745e0, 0x474170, 0x473590, 0x473a00
// all have this header and this pair of arms):
//   * the mask arm must keep the `if (!Contains) visible = 0; else visible =
//     expr;` shape and the fog arm the positive `&&` with `visible = 1` /
//     `visible = 0`; a ternary or a nested if merges the two arms' fail
//     blocks and costs 1 to 4 bytes of shape (298-311).
//   * the declaration order inside the arms is not free. The mask arm must
//     be col, row, w: with the fog arm fixed, col/row/w is 84.6 and moving `w`
//     to either other position is 33.2. In the fog arm the best of all 24
//     orders is seen, col, row, w (84.6); col, row, seen, w is the 84.0
//     version and the rest are 80.8 to 83.0.
//   * all three locals are needed, and each arm needs its own: with the other
//     two kept, the fog arm without `seen` is 37.1 and without `w` is 38.0,
//     the mask arm without `w` is 39.0, and the fully local-free spelling is
//     39.4. Nothing scored between 40 and 70.
//   * col/row must be `int`; `unsigned int col` is 70.6. `row * w` instead of
//     `w * row` is 303 bytes but makes the shift 16-bit (movsx ebx,bx).
//   * the header reads the position through a `Pos* q` and the arms through
//     `pos.x`, or the other way round: either direction gives the three
//     `movsx` per arm. Stride 0x14b, `short height` in the record with
//     MapSize's height `unsigned int`, a `short*` alias identical to the
//     nested Pos split, and `(fogFlags & 2) == 2` all fixed.
//   * the map pointer must be computed after the sx/sy header (68.0 the other
//     way), and the width local must be declared before the test (37-74).
//   * headers.py is pointless here: all 128 sets give 84.0.
#include <stddef.h>

void* __stdcall FUN_004b7f30(void* a, int b);
void __stdcall FUN_004b8500(void* dest, void* src, int x, int y);

#pragma pack(push, 1)
struct Pos_00474b80 {
    short x;                       // +0x00 (record +0x06)
    char unknown_2[0x4 - 0x2];
    short h;                       // +0x04 (record +0x0a)
    char unknown_6[0x8 - 0x6];
    short y;                       // +0x08 (record +0x0e)
};

struct MapSize_00474b80 {
    unsigned int width;            // +0x80
    unsigned int height;           // +0x84

    int Contains(int col, int row)
    {
        return (unsigned int)col < width && (unsigned int)row < height;
    }
};

struct Player_00474b80 {
    char unknown_0[0x7c];
    unsigned char* seen;           // +0x7c
    MapSize_00474b80 size;         // +0x80
    char unknown_88[0x14b - 0x88];
};

struct Game_00474b80 {
    char unknown_0[0x1b63];
    Player_00474b80 players[10];   // +0x1b63, stride 0x14b
    char unknown_2851[0x2a43 - 0x2851];
    unsigned char playerIndex;     // +0x2a43
    char unknown_2a44[0x14273 - 0x2a44];
    unsigned short* visibilityMask;// +0x14273
    char unknown_14277[0x14281 - 0x14277];
    unsigned char fogFlags;        // +0x14281
};

struct Record_00474b80 {
    void* data;                    // +0x00
    char unknown_4[0x6 - 0x4];
    Pos_00474b80 pos;              // +0x06
    char unknown_10[0x14 - 0x10];
    int field_14;                  // +0x14
    void FUN_00474b80(void* dest, short px, short py);
};
#pragma pack(pop)

extern Game_00474b80* g_game;

// FUNCTION: 0x474b80
void Record_00474b80::FUN_00474b80(void* dest, short px, short py)
{
    // The header reads the position through the pointer, the test through the
    // sub-struct: two different expression trees, so MSVC keeps both load nodes
    // and re-reads the three shorts per arm instead of sharing the header's.
    // (Either direction works: header-via-pointer or header-via-member.)
    Pos_00474b80* q = &pos;
    short sx = q->x - px + 0x80;
    short sy = q->y - (q->h >> 1) - py + 0x20;
    Player_00474b80* map = &g_game->players[g_game->playerIndex];
    int visible;
    if ((g_game->fogFlags & 2) == 2) {
        // Not redundant, and the order is not arbitrary: `seen` first, then
        // col and row, then the width. Naming the two fields here is what
        // stops MSVC hoisting the arms' shared pos.x load and what puts the
        // player pointer in edx (see the note above); the order is what makes
        // the fog arm reload `seen` into a register instead of folding the
        // spill slot into the add.
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
        FUN_004b8500(dest, FUN_004b7f30(data, field_14), sx, sy);
}
