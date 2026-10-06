// Decompiled by Opus, space-bunny-free, deepseek-v4.1-flash, GPT-6.1-sol and Haiku. Names are provisional.
// The teleport spark: moved by Step, drawn by DrawParticle when the local
// player can see it, and dropped once IsExpired.
#include <stddef.h>

void* __stdcall GetGafFrame(void* a, int b);
void __stdcall DrawFrameBlended(void* dest, void* src, int x, int y);

struct Vec3_00473560 {
    int x;
    int y;
    int z;
    Vec3_00473560& operator+=(const Vec3_00473560& o)
    {
        x += o.x;
        y += o.y;
        z += o.z;
        return *this;
    }
};

#pragma pack(push, 1)
// The integer halves of the 16.16 position.
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

// One teleport spark (the element of TeleportParticles' vector), 0x34 bytes.
class Class_00473560 {
public:
    void* data;                    // +0x00, the animation
    union {
        struct {
            Vec3_00473560 pos;     // +0x04
            Vec3_00473560 pos2;    // +0x10
            Vec3_00473560 vel;     // +0x1c
        };
        struct {
            char unknown_4[0x6 - 0x4];
            Pos_00473590 posw;     // +0x06
        };
    };
    int count;                     // +0x28, the frame count
    int field_2c;                  // +0x2c, the frame
    int field_30;                  // +0x30, the tick it expires

    void Step();
    void DrawParticle(void* dest, short px, short py);
    int IsExpired(int param_1);
};
#pragma pack(pop)

extern Game* g_game;

// The allocation lever, as at 0x473a00: the wrap pins the prologue, the
// pre-branch block and the mask arm's register choice without emitting a
// single extra instruction. IsSeen alone, or Identity around only the mask
// test, rotates the pre-branch instead.
static inline int Identity_00473590(int v) { return v; }

// The second arm, inlined. The two player parameters are not a typo: the
// Contains test reads the map width through `p` and the index reads it through
// `q`, two address nodes, which is what stops the width load folding into the
// imul and keeps the original's `mov edx,[edx+0x80]; imul edx,ecx` pair.
static inline int IsSeen_00473590(Player_00473590* p, Player_00473590* q, int col, int row)
{
    if (!p->size.Contains(col, row))
        return 0;
    return (g_game->visibilityMask[q->size.width * row + col] &
            (1 << g_game->playerIndex)) != 0;
}

// Moves the position by its velocity (an inlined Vec3 operator+=) and steps
// the frame.
// FUNCTION: 0x473560
void Class_00473560::Step()
{
    pos += vel;
    field_2c = (field_2c + 1) % count;
}

// SPACE-BUNNY-FREE, ninth pass (issue 4241): MATCH, 301 of 301 bytes, byte for
// byte the original. The answer to the wall every note below describes is the
// 0x473a00 recipe, matched in this same issue: the two arm locals that pinned
// the pre-branch allocation are not needed, an inline helper wrapped in a
// trivial Identity() inline pins it instead, and the fog arm can then go back
// to the local-free shape the original actually has.
//   * the fog arm drops `seen` and `w` and reads the fog map through two player
//     pointers: `p->size.Contains(col,row) && p->seen[p2->size.width*row+col]`.
//     The width is then read once for the bounds test and once for the index,
//     and the fog pointer folds into the add, which is exactly the original's
//     `mov ebx,[edx+0x80]; imul ebx,ecx; add ebx,[edx+0x7c]; cmp byte
//     [ebx+edi],0`. Both fail blocks are separate as well, so the tail merge
//     every if/else spelling could not break disappears with the locals.
//   * the mask arm is `visible = Identity_00473590(IsSeen_00473590(p, p2, col,
//     row));`. IsSeen holds the whole arm (the Contains test included) and
//     takes the two player pointers, as at 0x473a00; both details are load
//     bearing. The Contains test reads the width through `p` and the index
//     reads it through `p2`, which is what stops the load folding into the
//     imul and keeps the original's `mov reg,[reg+0x80]; imul` pair.
// Measured here with check.py on scratch copies (build/scratch/0x473590,
// gen9.py; b1 is the shape of the previous 91.9 percent file, b5 is this one):
//   b2 fog arm local free, mask arm still written out inline  38.1 [291]
//   b3 the same with both player pointers declared at the top 38.1 [291]
//   b4 fog local free + Identity(IsSeen(map,qq))              86.3 [299]
//   b6 Identity helper but the old local (seen, w) fog arm     51.2 [311]
//   b7 Identity with one pointer in both roles                  33.0 [291]
//   b9 Identity with the local fog arm, one pointer             32.0 [303]
// So for this family the lever is the two together: the helper only pays when
// the fog arm is free of locals, and only the mask arm's helper pins the
// allocation. Checked on the two siblings that share this code, by applying
// the same recipe to copies in build/scratch/0x473590/ (their own files are
// not mine, so this is a measured lead, not a change):
//   sib_0x474170.cpp   this shape verbatim, with the class name swapped:
//                      MATCH, 301 of 301 bytes. 0x474170 is this function
//                      instruction for instruction apart from its branch
//                      targets, so the file needs nothing but the two helper
//                      inlines and the local-free fog arm.
//   sib_0x474b80.cpp   the recipe alone: 87.4 percent, its fog arm wanting the
//                      other index shape (see below).
//   sib_0x474b80_get.cpp  the recipe plus the ByteMap `Get()` inline in the fog
//                      arm (Player reshaped to `unsigned char* data` at +0x7c
//                      with the size at +0x80, as 0x473a00 has it): MATCH, 303
//                      of 303 bytes. 0x474b80's fog arm loads the fog pointer
//                      into a register before the imul instead of folding it
//                      into the add, and Get() is what produces that; where
//                      the original folds the pointer into the add (here,
//                      0x474170), Get() must not be used. So the shape of the
//                      fog index is decided by the original's own instruction
//                      order, and the Identity/IsSeen pair is common to all
//                      four.
//
// SPACE-BUNNY-FREE, eighth pass (issue 4241): 91.9 percent, 301 of 301 bytes, from
// tools/permute.py (6028 candidates, 14 min, --jobs 4). The body below is the
// permuter's best with its leftovers tidied by hand (the inl0/inl1 helpers,
// tmp0, the `do {} while (0)` and the single-line if/else are gone; every
// re-checked after each edit). What it bought is only the ORDER of the fog
// arm's instructions: declaring `seen` first and assigning it after `col` and
// `row` (`unsigned char* seen; int col = ...; int row = ...; seen = map->seen;`)
// moves the `mov reg,[edx+0x7c]` and its spill store to just after
// `sub ecx,ebx`, instead of ahead of the three `movsx`. That declaration split
// is load bearing: merging it back (`unsigned char* seen = map->seen;`) drops
// the function to 90.9.
// WHAT STILL DIFFERS, exactly as the 90.9 notes above say:
//   1. the fog arm spills `seen` (`mov ebx,[edx+0x7c]; mov [esp+0x18],ebx`)
//      where the original rematerialises it as `add ebx,[edx+0x7c]` and
//      re-reads the width instead of reusing the Contains one, so the arm
//      spends 3 instructions the original does not and omits its second
//      `mov ebx,[edx+0x80]`;
//   2. the fog arm's fail block is tail merged with the mask arm's (all three
//      `jae`/`je` go to the mask arm's `xor edx,edx`), where the original
//      keeps the two blocks at 0x47362d and 0x47365f.
// New this pass, all measured with check.py --sym on scratch copies:
//   * the `seen` spill is not removable by any local-free spelling: the arm is
//     byte exact (modulo registers) with a p/q pointer pair (`p->size.Contains
//     (col,row) && p->seen[p2->size.width*row+col]`), with the ByteMap `Get`
//     form the matched sibling 0x407e90 uses, or with the index width read
//     through a second map pointer, but every one of those rotates the
//     pre-branch to `mov ebx,[g_game]` at the top (g_game into ebx, map into
//     edi, playerIndex into ecx, `py` re-read from the stack): 15.0 to 38.3
//     percent, 289 to 311 bytes. So the two named locals really are the price
//     of the register allocation that holds the pre-branch in place.
//   * `Get()` in the ByteMap shape does reproduce the original's two late
//     width loads (`mov ebx,[edi+0x80]; imul ebx,ecx` with no fold), and the
//     `unsigned char Get(int x,int y) { return *(data + size.width*y + x); }`
//     association matters for `add reg,[edx+0x7c]` vs a register load, but it
//     rotates the pre-branch too (37.9).
//   * the tail merge survives every if/else spelling of the fog arm: plain
//     if/else, braces, `if (!(c && s)) v=0; else v=1;`, `v = c && s ? 1 : 0`,
//     a nested `if`, `if (!c) v=0; else if (!s) v=0; else v=1;`, a goto, a
//     named condition and `visible = false` (all 90.9 and 301 bytes, f1..fa in
//     build/scratch/0x473590/gen_fail.py); only the two 302/298 byte forms move
//     the block, and they lose the pre-branch.
//   * best lead for the next attempt (not in the file, it scores 88.9): the p/q
//     fog arm plus two `unsigned short` pins (c16, r16) feeding BOTH the
//     Contains test and the mask index in the mask arm gives the pre-branch
//     AND the whole fog arm byte identical, with the mask arm off by exactly
//     the two zero extensions `and ebx,0xffff` / `and ecx,0xffff` that the
//     `unsigned short -> int` conversions cost (309 bytes, p1 in
//     build/scratch/0x473590/gen_pin.py). Splitting the pins between the test
//     and the index, one pin instead of two, signed `short` pins, 32-bit pins,
//     an `int` cell pin and pins in the fog arm all rotate the pre-branch
//     again (37.4 to 40.0), so the next attempt should look for a pressure
//     node that is not a 16-bit truncation: one that pins the pre-branch
//     without widening anything in the mask arm.
//

// GPT-6.1-sol (#2520 retry): kept the 85.4% best. Rechecked baseline and tried
// a SeenMap::Get helper (37.8%) plus loading `seen` only inside the successful
// bounds branch (74.7%). No MATCH. Remaining differences are the two fog/mask
// arm spills and the resulting branch targets described below.
// GPT-6 tested the 0x474170 arm layout here. It improves the checker score
// from 84.0% to 85.4%; the remaining arm spills and branch targets still differ.
// A 128-combination header sweep did not improve this version.
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
// So the two spills are load-bearing: no arm spelling without its locals keeps
// the player pointer in edx, exactly as the note below concludes.
// 85.4 percent, 307 of 301 bytes. Same function as 0x473590 (the two originals
// are instruction-for-instruction identical apart from branch targets), so this
// file also describes 0x473590.
//
// The previous version (70.7 percent) was the `else if (row)` hack copied from
// particles_473560.cpp. This version instead uses the arm-local pressure
// spelling that
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
// DEEPSEEK-V4.1-FLASH, third pass (issue 1474), scratch scores only. The file
// below is still the best at 85.4 percent; the two spill stores are the whole
// difference. Ruled out two more routes: spelling the two arms' position
// reads differently (mask `q->x` while fog keeps `pos.x`, or the mirror)
// rotates the pre-branch and scores 18.8 to 24.5 (315 to 323 bytes); a
// local-free fog arm with the mask arm keeping its `w` local is 40.0 (297
// bytes), and `seen` declared between col and row is 84.4 (307 bytes).
//
// SPACE-BUNNY-FREE, fourth pass (issue 1831), scratch scores only, no
// improvement: the file below is still the best at 85.4 percent, 307 of 301
// bytes, and the 128 header sets are exhausted (headers.py: no set beats
// 85.4). Tried the ByteMap `Get()` route that fixed 0x4745e0, with the
// Player record reshaped to `unsigned char* data + MapSize size` at +0x7c as
// in 0x4745e0.cpp and smoke_particles.cpp, so that both index reads become
// rematerialisable: no `seen`/`w` locals at all, fog arm through
// `map->explored.Get(col,row)` = `data[size.width*row+col]`, mask arm through
// a fresh `map->explored.size.width * row + col`.
//   * both arms local free: 46.4 percent, 291 bytes.
//   * fog arm local free, mask arm keeping its `w` local: 39.6 percent.
//   * both local free plus a `MapSize& size` reference for the tests: 44.4.
//   * both local free plus an `unsigned char who = g_game->playerIndex` header
//     local feeding both the players index and the mask shift: 45.9, 303 bytes.
// So the Get() shape that made 0x4745e0's mask arm byte-exact does NOT transfer
// here: `Get()` is what kills the arm locals, and the arm locals are exactly
// what pins the pre-branch block. With the arms local free MSVC hoists
// `mov ebx,[g_game]` to the top, keeps the player index in ecx instead of
// edx/edi, puts the map pointer in edi instead of edx, loads fogFlags into dl
// after the map lea instead of cl before it, and the whole pre-branch rotates
// (that is where the 40 to 46 percent comes from, not from the arms).
// Confirms the conclusion already at the top of this file from the other
// direction: the two spill stores are not removable, they are the price of the
// register allocation that holds the pre-branch in place.
//
// SPACE-BUNNY-FREE, fifth pass (issue 3441), scratch scores only, still 85.4
// percent and the code below is unchanged. Re-ran the 4x2 arm sweep (fog arm
// decls in {seen+w, w only, seen only, none, w+seen reversed} x mask arm `w`
// local at the top of the arm or declared inside the else) and reproduced
// every number in the notes above exactly, so they stand:
//   fog seen+w / mask top 85.4 [307]   fog seen+w / mask else 39.2 [291]
//   fog w     / mask top 37.8 [297]   fog w     / mask else 37.3 [287]
//   fog seen  / mask top 40.0 [297]   fog seen  / mask else 39.6 [287]
//   fog none  / mask top 40.0 [297]   fog none  / mask else 39.6 [287]
//   fog w+seen reversed / mask top 82.0 [313]  / mask else 41.0 [297]
// New datum: moving the mask arm's `w` local into the else block does NOT
// recover the fog arm (39.2 at best, so the fog arm's own locals are what hold
// the pre-branch, not the mask arm's), and reversing the two fog decls costs
// 3.4 points, so the `seen`-then-`w` order is load bearing too.
// The mechanism, from the no-local diff: with the arms local free MSVC keeps
// `g_game` itself in ebx from the very top (`mov ebx,[0x511de8]` right after
// the first push) and puts the map pointer in edi. That is impossible in the
// original because ebx is the width/address register inside BOTH arms
// (0x4735fc, 0x473614 and 0x473644), so g_game has to be a short-lived scratch
// in ecx there (0x4735ab). Any arm local at all is enough to stop that hoist,
// but a pointer local then costs a stack slot. What is still missing is a
// pressure node that is neither a pointer nor a spilled scalar.
// DEEPSEEK-V4.1-FLASH, sixth pass (issue 4241): 90.9 percent, 301 of 301
// bytes (the byte count now equals the original). Two changes from the 85.4
// version above, both found by sweeping source shapes:
//   * the mask arm now names a SECOND map pointer (`qq`) and reads the width
//     through it (`qq->size.width * row + col`). That makes the mask arm
//     byte-identical (the original re-reads the width into edx: `mov
//     edx,[edx+0x80]; imul edx,ecx`) and keeps the pre-branch block.
//   * the fog arm's declaration order is now seen, col, row, w (the order
//     0x474b80 documents). All 24 permutations were scored: seen,col,row,w is
//     the only one that holds the pre-branch (90.9); the old col,row,seen,w is
//     87.3, seen,row,col,w and seen,row,w,col are 87.9, seen,col,w,row and
//     seen,w,col,row are 83.0, the rest 79.8 to 86.9.
// WHAT STILL DIFFERS (both inside the fog arm):
//   1. `seen` is spilled, where the original rematerialises it as a memory
//      operand. ours: mov ecx,[edx+0x7c]; mov [esp+0x18],ecx; ... imul ebx,ecx;
//      mov edx,[esp+0x18]; add ebx,edi; cmp byte [ebx+edx],0
//      orig: ... mov ebx,[edx+0x80]; imul ebx,ecx; add ebx,[edx+0x7c];
//      cmp byte [ebx+edi],0
//      (ours spends 3 instructions the original does not and omits the width
//      re-read, so the two sizes still come out equal)
//   2. the fog arm's fail block is tail-merged with the mask arm's; the
//      original keeps two (`xor edx,edx; jmp` at 0x47362d and 0x47365f).
// Every local-free fog spelling rotates the pre-branch to `mov ebx,[g_game]`
// at the top (g_game into ebx, map into edi, playerIndex into ecx) and also
// folds the width into the imul (`imul ecx,[edi+0x80]`): member Contains,
// hand-spelled bounds, ByteMap::Get, inline Fog/Vis helpers, p/q pointer
// aliases, a `who` local, a `flags` local, reordering map/sx/sy, index
// operand order, int/long width types. Only the two index-used locals
// `seen`+`w` pin the pre-branch, and they are exactly what spills.
// Measured dead this pass: all 24 fog declaration permutations, a 601-value
// dummy-extern compiler-state sweep, a 201-value dummy-function sweep (both
// flat), tools/headers.py (128 sets, flat at 90.9), bool `visible` (296 bytes,
// dl instead of edx), `register` on either local, two `seen` aliases (coalesce),
// two width aliases, `w` used in a hand-spelled test, an `idx` local, an extra
// height local, an inverted fog condition, an inverted mask condition (86.9),
// swapped arms (73.1), and every `visible = 0` spelling (`!1`, `1-1`, `2-2`,
// `(int)false`, `0&1`) to break the fail-block merge.
//
// BEST LEAD FOR THE NEXT ATTEMPT (found in this pass, not yet combined): the
// fog arm's exact instructions ARE reachable. With a 16-bit value live across
// the fog test (the pin 0x474170 documents) and the p/q pointer pair in the fog
// arm, the fog arm compiles byte-for-byte to the original:
//     Player* p  = &g_game->players[g_game->playerIndex];
//     Player* q2 = &g_game->players[g_game->playerIndex];
//     if (p->size.Contains(col, row) && p->seen[p2->size.width * row + col] != 0)
// The two pointers coalesce into edx but their width reads do not, so the
// original's `mov ebx,[edx+0x80]; imul ebx,ecx; add ebx,[edx+0x7c];
// cmp byte [ebx+edi],0` comes out exactly, and the pre-branch stays pinned.
// The pin used was a `unsigned short cell = g_game->visibilityMask[...]` in the
// mask arm (v46a in build/scratch/0x473590, 78.2 [309]): that pin costs the
// mask arm (MSVC hoists the cell load above the bounds test and spills row),
// which is why it is not the file. What is missing is a pin that leaves the
// mask arm alone, or a mask-arm spelling that keeps the cell load after the
// test (a comma/short-circuit assignment sinks the load and loses the pin; a
// 16-bit height local or a `pi` player-index local break the test instead).
// The 16-bit pin works only as a 16-bit value (an 8-bit cell is 39.4, an int
// cell 53.2 in the mask arm); with the p/q fog arm plus the mask `qq` trick and
// no pin the pre-branch rotates to `mov ebx,[g_game]` (38.1).
//
// DEEPSEEK-V4.1-FLASH, seventh pass (60 minute timebox, ~55 scratch scorings,
// no new best; the code below is still the 90.9 file). The lead above was
// reproduced and then taken as far as it goes: p/q fog arm + 16-bit pin in the
// mask arm is 88.9 percent, 309 of 301 bytes, with the pre-branch AND the whole
// fog arm byte-identical. The ONLY difference left is two zero-extension `and`s
// in the mask arm:
//   ours: mov ebx,[esp+0x18]; and ebx,0xffff; sar ecx,5; cmp ebx,[edx+0x80] ...
//         and ecx,0xffff; cmp ecx,[edx+0x84]
//   orig: mov ebx,[esp+0x18]; sar ecx,5; cmp ebx,[edx+0x80] ...
//         cmp ecx,[edx+0x84]
// The pin is `unsigned short c16 = (unsigned short)col; unsigned short r16 =
// (unsigned short)row;` feeding BOTH the Contains test and the index; both must
// be 16-bit (one 16-bit value is 37.9 to 39.8 and rotates the pre-branch) and
// both must be the values used in the index. The `and`s are the price of
// `unsigned short -> unsigned int` conversion, so this spelling can never be a
// byte match; the mask arm's col/row are 32-bit in the original (movsx/sar with
// no masks anywhere). Closed this pass, all measured with check.py --sym:
//   * 32-bit masks instead of 16-bit types (col & 0xffff, a c32/r32 pair): 37.4
//     to 39.6, no pin. A 16-bit copy that is coalesced (`int c = col`) is the
//     plain local-free 38.1, and `short c = (short)col` is 37.8.
//   * 16-bit pins elsewhere: playerIndex (`unsigned short pi`, also in the
//     shift), the height, the width, the cell through a pointer, a precomputed
//     16-bit index, `short h16` in the row expression, 16-bit header locals for
//     x/h/y: 20.5 to 48.5, all rotate. Only position-derived unsigned shorts in
//     the mask arm pin.
//   * 16-bit pins in the fog arm (c16/r16 in the test or the index): 39.6 to
//     40.2, and the fog arm loses its exact form.
//   * hand-spelled Contains in the pinned mask arm, direct boolean assignments,
//     `visible = 0` pre-initialisation, positive Contains, nested braces: no
//     change (88.9) or rotate (34.7 to 37.1).
//   * header pins (16-bit x/h/y locals used in both arms, a `pi` index local,
//     the map pointer built inside the arms): 14.8 to 37.8.
// So the wall is now exactly two instructions wide, and the pin that supplies
// the pressure is also the thing that costs them. NEXT ATTEMPT: find a 16-bit
// pin whose zero-extension the mask arm does not have to pay for, or a
// non-16-bit pressure node that leaves the mask arm's 32-bit arithmetic alone.
// The tail-merge of the two fail blocks is independent and unfixed by every
// `visible = 0` spelling tried so far.
// FUNCTION: 0x473590
void Class_00473560::DrawParticle(void* dest, short px, short py)
{
    Pos_00473590* q = &posw;
    short sx = q->x - px + 0x80;
    short sy = q->y - (q->h >> 1) - py + 0x20;
    // Two locals with the same value. The original reads the map width twice
    // per arm, once for the bounds test and once for the index, and a single
    // pointer makes MSVC 5 fold one of the two away.
    Player_00473590* p = &g_game->players[g_game->playerIndex];
    Player_00473590* p2 = &g_game->players[g_game->playerIndex];
    int visible;
    if ((g_game->fogFlags & 2) == 2) {
        int col = posw.x >> 5;
        int row = (posw.y - (posw.h >> 1)) >> 5;
        // Local free on purpose: the width is re-read and the fog map pointer
        // folds into the add, which is what the original does.
        if (p->size.Contains(col, row) &&
            p->seen[p2->size.width * row + col] != 0)
            visible = 1;
        else
            visible = 0;
    } else {
        int col = posw.x >> 5;
        int row = (posw.y - (posw.h >> 1)) >> 5;
        visible = Identity_00473590(IsSeen_00473590(p, p2, col, row));
    }
    if (visible)
        DrawFrameBlended(dest, GetGafFrame(data, field_2c), sx, sy);
}

// Whether the tick has passed the spark's expiry.
// FUNCTION: 0x4736c0
int Class_00473560::IsExpired(int param_1)
{
    return param_1 > field_30;
}
