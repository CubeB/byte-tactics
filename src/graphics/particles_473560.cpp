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
