// SPACE-BUNNY-FREE, ninth pass (issue 4241): MATCH, 301 of 301 bytes. This is the
// 0x473a00/0x473590 recipe applied to this sibling: the fog arm drops both arm
// locals and reads the fog map through two player pointers, and the mask arm is
// Identity(IsSeen(p, p2, col, row)) with the helper holding the whole arm. The
// variant was verified by the 0x473590 worker in build/scratch/0x473590/ and is
// promoted here unchanged.
// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, third pass by space-bunny-free, finished by GPT-6.1-sol. Names are provisional.
// GPT-6.1-sol retry pass: best remains 85.4% (307/301 bytes). Explicit nested
// fog checks and scoped row-index locals scored lower; no MATCH. Remaining gap
// is the arm-local loads/spills described below.
//
// SPACE-BUNNY-FREE, third pass (ten minute timebox). Tried the ByteMap {data,
// size} shape with a member Get() that the matched 0x407e90 and 0x475470 use
// (the suggestion from 0x4745e0), on the theory that routing the width and
// seen reads through a member would make them rematerialisable instead of homed
// locals. It does produce the original's own index shape in the fog arm
// (`mov ebx,[edx+0x80]; imul ebx,ecx; add ebx,[edx+0x7c]`) with no homed
// `seen`, but the whole prologue rotates: g_game is hoisted into ebx before the
// pushes, the player index is built in ecx, the map pointer lands in edi and
// the flags byte test moves to `dl`, and the score falls to 46.4 percent (291
// bytes). Scratch only, `build/scratch/0x474170/v1.cpp`. So the ByteMap member
// method is NOT the missing ingredient here: as at 0x475470 the shape is right
// and the arm-local pressure is what the original needs, and here, as there,
// the two cannot both be had. The 85.4 percent version below stays best. This
// also means the same for the sibling 0x473590, whose code is identical.
//
// DEEPSEEK-V4.1-FLASH, second pass. Still not a match at 85.4 percent, 307 of
// 301 bytes. Confirmed the basin below is the optimum for the two known levers
// and closed two more escape routes (all measured with `check.py --sym` on
// scratch copies, no new runs in the file's own history):
//   * splitting the two arms' position expressions so they are not CSE-able
//     (fog reads through a `Pos* q`, mask through `pos.`, and the mirror with
//     the header on the other spelling) scores 20.1 [310]; the pointer local
//     itself rotates the whole pre-branch block, so this is much worse than
//     the shared member load it was meant to break.
//   * dropping the mask arm's `w` local (original re-reads the width, so this
//     looked right) scores 39.2 [291]; dropping the fog arm's `seen` only is
//     37.8 [297], dropping the fog arm's `w` only is 40.0 [297].
// GPT-6 tried all 128 header combinations with no change from 85.4%.
// So the two spills are load-bearing: no arm spelling without its locals keeps
// the player pointer in edx, exactly as the note below concludes.
// 85.4 percent, 307 of 301 bytes. Same function as 0x473590 (the two originals
// are instruction-for-instruction identical apart from branch targets), so this
// file also describes 0x473590.
//
// The previous version (70.7 percent) was the `else if (row)` hack copied from
// 0x473590.cpp. This version instead uses the arm-local pressure spelling that
// 0x474b80 documents: `seen` and `w` names in the test arms are what stop MSVC
// hoisting `g_game` into ebx and the shared `pos.x` load into edx. With those
// locals the whole prologue, the pre-branch block, the branch, both arms' test
// chains, the mask arm's block layout and the call sequence are byte-identical.
//
// The struct bug in the old file is also fixed: `pos` is 0x26 bytes in the
// record, so `field_2c` really sits at +0x2c. The old Pos padding of 4 bytes put
// it at +0x14 (the 0x475040 record's offset) and the tail read the wrong field.
//
// WHAT STILL DIFFERS: the two named locals are spilled instead of being
// rematerialised, in two places.
//   * fog arm: ours loads `seen` up front and parks it in the py argument slot
//     (`mov ebx,[edx+0x7c]; mov [esp+0x18],ebx`), then adds it from there
//     (`add ebx,[esp+0x18]`). The original has no `seen` load at all: it reloads
//     the width a second time (`mov ebx,[edx+0x80]`) and folds the fog-map
//     pointer into the add (`add ebx,[edx+0x7c]`), indexing with col in edi.
//   * mask arm: ours materialises the width before the test and spills it
//     (`mov ebx,[edx+0x80]; mov [esp+0x1c],ebx`), then reloads it for the
//     multiply; the original just re-reads it (`mov edx,[edx+0x80]; imul
//     edx,ecx`).
// Both are the same single problem: every spelling without a live local gets
// the arm registers right (pointer in edx, col in edi) only if the local exists,
// and any named local live across the test is spilled by MSVC 5. Removing the
// `seen` local, or moving the mask width read after the test, hoists g_game into
// ebx and the shared pos.x load into edx (37 to 43 percent). Same wall the
// sibling 0x474b80 (84.6 percent) is stuck on.
//
// Ruled out here, all measured: header via member vs via a Pos* local (same
// 85.4), a Map* instead of a Player* map pointer (same), the `seen` local only
// (41.8), `w` only (37.8), neither (39.6), `seen` declared after col/row
// (85.4), after w (82.0), const-qualified locals (84.0), `unsigned int seen`
// (compile error), the mask width read moved after the test (39.2), two player
// pointers (38.1), and the old `else if (row)` hack (70.7).
//
// DEEPSEEK-V4.1-FLASH, third pass (ten minute timebox, scratch scorings only):
// re-confirmed the basin. Removing either arm local rotates the prologue even
// when the other arm keeps its locals: fog arm with the `w` local but the index
// back to `map->seen[map->size.width * row + col]` is 37.8 [297], with both
// locals and that member-width index is 38.2 [309], with only the `seen` local
// and the member-width index is 40.0 [297]. A `Player_00473590& map` reference
// instead of the pointer is byte-identical at 85.4. So the seen spill really is
// load-bearing and the best version stays this one.
//
// DEEPSEEK-V4.1-FLASH, fourth pass (900s timebox). No code change: 85.4 percent
// remains the best. Confirmed the local-optimum is allocator-only and closed the
// remaining escape routes with scratch `check.py --sym` scores (no new runs in
// the file's own history):
//   * N-declarations sweep: the fully local-free shape (both arms use fresh
//     `map->seen[map->size.width*row+col]`) is 39.6 percent for every N from 0
//     to 400 in steps of 4, so it is not compiler symbol state. See
//     build/scratch/0x474170/sweep_results.txt.
//   * defining the real preceding matched function 0x474130 above the file (with
//     its own annotation) still scores exactly 85.4, so compiler state from the
//     original TU is not the missing piece either.
//   * modelling the whole test as an in-class inline `Pos::Visible()` (the
//     0x4745e0 shape) is 43.3 [311]; moving either arm's locals after its
//     Contains test drops to 37.6-39.2; fresh fog + w,m mask is 45.8 [306];
//     `bool visible` is 82.4 [304]; unsigned col/row is 70.0 [311]; Contains
//     with unsigned parameters or signed compares is byte-identical at 85.4.
// The two spills stay load-bearing: any spelling that removes either arm local
// rotates `g_game` into ebx and the map pointer into edi.
//
// DEEPSEEK-V4.1-FLASH, fifth pass (900s timebox). No code change: 85.4 percent
// remains the best. Re-derived the wall from the disassembly and closed three
// more spellings, all scored with check.py --sym on scratch copies (no new runs
// in the file's own history):
//   * the 0x473a00 sibling recipe (two identical player pointers, the bounds
//     through one and the index through the other, so the width is re-read) is
//     38.1 [291] with the header through `Pos* q` and 38.1 again with two field
//     references; the width is indeed re-read, but the arms carry no live value,
//     so the backend hoists the shared `movsx edx,[eax+6]` above the `jne`,
//     which pushes playerIndex out of edx, which puts g_game in ebx. The
//     re-read and the pre-branch pin cannot both be had from those pointers.
//   * field references (`unsigned int& w`, `unsigned char*& seen`) and
//     pointer-to-field locals (`unsigned int* w = &map->size.width`) are 38.1
//     [291]: MSVC treats the reference as a plain memory alias, so the index
//     re-reads and no pressure appears, same rotation as the local-free body.
//   * a nested fog `if` with `seen` declared inside the success block (short
//     live range) is 39.2 [288]; a second player pointer used only for the
//     bounds while the fog/mask `w` locals stay is byte-identical at 85.4.
// So this pass reproduces the earlier conclusion independently: the only source
// shape that holds the pre-block is the pair of live arm locals, and MSVC 5 can
// only spill a named local, never rematerialise it. 85.4 percent stays.
//
// DEEPSEEK-V4.1-FLASH, sixth pass (60 minute timebox, ~90 scratch scorings on
// top of the file's own history, no code change: 85.4 percent remains best).
// The one genuinely new lever is that the pre-block pin is NOT specific to the
// `seen`/`w` locals: a 16-bit value live across the arm's Contains test pins
// it too. Measured with check.py --sym on scratch copies in build/scratch/0x474170:
//   * mask arm `unsigned short cell = g_game->visibilityMask[map->size.width *
//     row + col];` then `if (cell & (1 << playerIndex))`: 74.5 [307]. The
//     prologue, pre-block, branch AND the fog arm's first width load come out
//     byte-identical to the original. What still differs is that MSVC hoists
//     the cell load above the bounds test and the fog index then folds to
//     `imul ecx,[edx+0x80]` because ebx is free.
//   * the same pin in the fog arm: 70.1 [311]; in both arms: 70.1 [311].
//   * the pin needs the 16-bit width: `unsigned char cell` is 39.4 [288],
//     `int cell` 42.0 [308] (fog) and 53.2 [315] (mask), `MapSize* sz =
//     &map->size` 37.3, a `w` used only in the compare 37.3. Only the 16-bit
//     cell fills the seventh register across the test.
//   * the pin also needs the Contains inline: spelling the bounds out by hand
//     in the pinned fog arm drops it from 70.1 to 41.0.
//   * the ByteMap {data, size} Get() form (the matched 0x407e90 idiom) gives
//     the original's width materialisation `mov ebx,[edx+0x80]; imul ebx,ecx`
//     in the fog arm but computes the address as data + (width*row + col)
//     (`mov edx,[edx+0x7c]; add ebx,edi; cmp [ebx+edx]`) where the original
//     folds the pointer (`add ebx,[edx+0x7c]; cmp [ebx+edi]`). With the mask
//     cell pin it is 75.8 [311], the best non-85.4 shape; with a free mask arm
//     46.4 [291].
//   * combining the cell pin with the seen/w locals is worse (43.9 to 85.4),
//     and the 0x473a00 self-correction (bool b = visible; if (b) ...) is 19.8
//     to 30.4 here, so that sibling's lever does not transfer.
//   * compiler symbol state is not the missing piece: 0..520 pad declarations
//     (step 5) leave the local-free body flat at 39.6 and the pinned body flat
//     at 85.4.
//   * also re-measured dead: index operand orders (rw / col+w*row / row*w+col
//     are all byte-identical on the cell-pinned shape), declaration orders
//     (seen first 84.0, w first 81.2, mask w first 34.5/39.0), two pointers
//     computed from g_game inside the arms (25.5), `unsigned short* m` mask
//     pointer (28.7), a height local (38.6/38.8), short-circuit cell
//     assignments (39.6), no-op self-assignments (30.1 to 39.6), inline
//     IsSeen/IsVisible helpers (25.5/39.6), a flags/fog local before the map
//     (all 39.6), spelled-out Contains in either arm (33.2 to 41.0; the
//     inline Contains is load-bearing), index locals (37.1 to 45.9), the
//     header moved inside `if (visible)` (13.9/33.5), and `q->x` vs `pos.x`
//     per arm (17.3 to 20.3).
// NEXT ATTEMPT'S BEST LEAD: the pin only needs a 16-bit live value at the
// test, so find a spelling whose cell read is placed AFTER the bounds check
// yet still live across it, or accept the cell hoist and find what makes MSVC
// materialise the width in the fog arm (`mov ebx,[edx+0x80]; imul ebx,ecx`)
// instead of folding it. The two goals meet in the ByteMap Get() fog arm,
// which already has the right materialisation.
#include <stddef.h>

void* __stdcall GetGafFrame(void* a, int b);
void __stdcall DrawFrameBlended(void* dest, void* src, int x, int y);

#pragma pack(push, 1)
struct Pos_00473590 {
    short x;                       // +0x00 (record +0x06)
    char unknown_2[0x4 - 0x2];
    short h;                       // +0x04 (record +0x0a)
    char unknown_6[0x8 - 0x6];
    short y;                       // +0x08 (record +0x0e)
    char unknown_a[0x26 - 0xa];    // pos is 0x26 bytes
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

struct Game {
    char unknown_0[0x1b63];
    Player_00473590 players[10];   // +0x1b63, stride 0x14b
    char unknown_2851[0x2a43 - 0x2851];
    unsigned char playerIndex;     // +0x2a43
    char unknown_2a44[0x14273 - 0x2a44];
    unsigned short* visibilityMask;// +0x14273
    char unknown_14277[0x14281 - 0x14277];
    unsigned char fogFlags;        // +0x14281
};

class Class_00474170 {
public:
    void* data;                    // +0x00
    char unknown_4[0x6 - 0x4];
    Pos_00473590 pos;              // +0x06
    int field_2c;                  // +0x2c
    void DrawParticle(void* dest, short px, short py);
};
#pragma pack(pop)

extern Game* g_game;

static inline int Identity_00474170(int v) { return v; }

static inline int IsSeen_00474170(Player_00473590* p, Player_00473590* q, int col, int row)
{
    if (!p->size.Contains(col, row))
        return 0;
    return (g_game->visibilityMask[q->size.width * row + col] &
            (1 << g_game->playerIndex)) != 0;
}

// FUNCTION: 0x474170
void Class_00474170::DrawParticle(void* dest, short px, short py)
{
    Pos_00473590* q = &pos;
    short sx = q->x - px + 0x80;
    short sy = q->y - (q->h >> 1) - py + 0x20;
    Player_00473590* p = &g_game->players[g_game->playerIndex];
    Player_00473590* p2 = &g_game->players[g_game->playerIndex];
    int visible;
    if ((g_game->fogFlags & 2) == 2) {
        int col = pos.x >> 5;
        int row = (pos.y - (pos.h >> 1)) >> 5;
        if (p->size.Contains(col, row) &&
            p->seen[p2->size.width * row + col] != 0)
            visible = 1;
        else
            visible = 0;
    } else {
        int col = pos.x >> 5;
        int row = (pos.y - (pos.h >> 1)) >> 5;
        visible = Identity_00474170(IsSeen_00474170(p, p2, col, row));
    }
    if (visible)
        DrawFrameBlended(dest, GetGafFrame(data, field_2c), sx, sy);
}

