// Decompiled by space-bunny-free, finished by LongCat 2.5 Preview Free,
// GPT-6.1-sol, finished by mimo-v2.6-pro. Names are provisional.
// MATCH (306 of 306 bytes, both references resolved).
//
// SPACE-BUNNY-FREE, tenth pass (issue 4299, orchestrator recipe): MATCH. The
// answer to the wall six models described as the register-allocation wall is the
// recipe the sibling 0x474b80 of this same issue matched with (and 0x473590 and
// 0x473a00 before it): no arm carries a map local, the fog arm reads the byte
// map through the ByteMap `Get()` inline, and the mask arm is a value-returning
// `static inline IsSeen(p, p2, col, row)` (the Contains test included) whose
// result goes through the trivial `Identity(v) { return v; }`. Identity is the
// code-free pin: it emits no instruction, but MSVC 5 allocates what it returns
// as a fresh live range, which is what holds this -> eax, the player pointer ->
// edx and the player index -> esi across the pre-branch block, and keeps the
// phi in edx. Every code-emitting pin tried before it (a redundant
// `(unsigned char)visible`, 96.6 percent / 312 bytes) emitted the 6-byte
// `and edx, 0xff` the original does not have, which was the whole residual.
//   * Two player pointers, p and p2, both `&g_game->players[g_game->playerIndex]`:
//     the bounds test reads p, the mask index re-reads the width through p2. One
//     pointer folds one of the two width loads the original has.
//   * The fog arm keeps no `seen` / `w` local and reads through
//     `ByteMap::Get(x, y) { return data[size.width * y + x]; }` (data at +0x7c,
//     size at +0x80), because this original materialises the fog pointer into a
//     register (`mov edx,[edx+0x7c]; imul edi,ecx; add edi,esi; cmp byte
//     [edi+edx],0`). Where an original folds the pointer into the add, as
//     0x473590 does, the flat `p->explored.data[p2->...width*row+col]` index is
//     the right spelling instead; check the disassembly before choosing.
//   * `Contains` takes `(int col, int row)` and casts inside, as at 0x474b80.
//   * The visibility code stays inside a `Pos_004745e0::Visible()` member. That
//     boundary is load bearing: inlined into the draw function instead, MSVC
//     CSEs the arms' `pos.x` / `pos.y` loads against the header's (the header
//     does its 16-bit arithmetic through `short sx` / `short sy`), so the arms
//     reuse hoisted registers and no longer re-read the three shorts the way
//     the original does. That inline shape scores 18.9 percent, and it is also
//     why the sibling's header goes through a `Pos* q` local.
//
// WHAT THE EARLIER PASSES HAD MEASURED, kept because it explains the shape:
//   * The pin is a USE of the phi, not a truncation: `visible = ~0 + visible;`
//     emits a single `dec edx`, pins the frame identically and still fails at
//     96.6 percent / 307 bytes, so the original's pin must cost zero bytes.
//     Every code-free self-use is DSE'd by VC5 before allocation and the frame
//     rotates to 23.1 percent (`visible = visible`, `^ 0`, `& -1`, `+ 0`, `- 0`,
//     int/unsigned casts either way, `(visible, visible)`, the self ternary,
//     `if (visible) { }`, and an identity helper applied to the phi directly).
//   * The arms' spelling matters as much as the pin: mask-expression respellings
//     (hoisted word, hoisted bit, hoisted player index, `(1u << ...)`), a bool
//     Get, an int-parameter Contains, a nested if in arm A, a ternary in arm B,
//     `visible = 0; if (...) visible = ...`, swapping the arms, hoisting col/row
//     or the player index into a local, moving Visible() into the outer class,
//     and the 0x4658e0-style IsExplored/IsSeen helper pair with `unsigned int
//     vis` all scored 18.9 to 63.0 percent, none of them better than 96.6.
//   * tools/permute.py over the draw function plus Contains, Get and Visible
//     (14 minutes, 6 jobs) found nothing above 96.6 percent, so the wall was not
//     reachable from mutations of the old file.
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

    int Contains(int col, int row)
    {
        return (unsigned int)col < width && (unsigned int)row < height;
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

// The pin the earlier passes were looking for: a helper that returns its
// argument. It emits no instruction, but MSVC 5 allocates what it returns as a
// fresh live range, which is what holds the frame in place. Remove it and the
// whole frame rotates (see the notes above).
static inline int Identity_004745e0(int v) { return v; }

// The whole mask arm, the Contains test included; the index re-reads the width
// through the second pointer, which keeps both of the original's width loads.
static inline int IsSeen_004745e0(Map_004745e0* p, Map_004745e0* q, int col, int row)
{
    if (!p->explored.size.Contains(col, row))
        return 0;
    return (g_game->visibilityMask[q->explored.size.width * row + col] &
            (1 << g_game->playerIndex)) != 0;
}

#pragma pack(push, 1)
struct Pos_004745e0 {
    short x;                        // +0
    char unknown_2[2];
    short height;                   // +4
    char unknown_6[2];
    short y;                        // +8

    // Is this record's position inside the explored byte map of the local
    // player (flags bit 1 set), or inside their bit of the shared visibility
    // mask (bit clear)? The two arms keep their own fail block, as the original
    // does.
    int Visible()
    {
        Map_004745e0* p = &g_game->players[g_game->playerIndex];
        Map_004745e0* p2 = &g_game->players[g_game->playerIndex];
        int visible;
        if ((g_game->flags & 2) == 2) {
            int col = x >> 5;
            int row = (y - (height >> 1)) >> 5;
            if (p->explored.size.Contains(col, row) &&
                p->explored.Get(col, row) != 0)
                visible = 1;
            else
                visible = 0;
        } else {
            int col = x >> 5;
            int row = (y - (height >> 1)) >> 5;
            visible = Identity_004745e0(IsSeen_004745e0(p, p2, col, row));
        }
        return visible;
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

// Draws the record's one-pixel marker at (surface, px, py) offset by its own
// position, when that position is visible to the local player. The marker is a
// 1x1 rectangle centred on the sprite's origin, 0x80/0x20 to the right of it.
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
