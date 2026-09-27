// Decompiled by Space Bunny Free. Names are provisional.
//
// NOT A MATCH: 66.3 percent (35 instruction lines still differ). Up from the
// 60.3 percent this file held before: the whole gain is one line of source,
// the `w` local at the top of the fog arm. The notes below say why it is there
// and what is still missing.
//
// WHAT MATCHES NOW
//   * the whole prologue, the rect, the epilogue and the call sequence;
//   * the pre-branch block, bar one register: the player index is born in edx
//     (`mov dl,[g_game+0x2a43]`), copied to esi, the player pointer ends up in
//     edx and the flags byte test gets cl, all as in the original. Those roles
//     are what the earlier attempts could not get, and they are also what puts
//     the `r.x2` store in the original's order, so the r.x2 lead closes here;
//   * the mask arm's test chain and its two `jae`s to one shared fail block;
//   * the fog arm's `jae fail; jb body` pair and its separate `xor edx,edx`
//     fail block (no tail merging, as in the original).
//
// THE ONE LEVER: the fog arm's `unsigned int w`
//   Every spelling of the fog arm that reads the map width inline, inside the
//   guarded expression, compiles with the player index left in cl and the
//   player pointer in esi, which costs about six lines in the pre-branch block
//   and then poisons every `[edx+0x80]`, `[edx+0x7c]` and `[edx+0x84]` in
//   both arms: 52.5 percent, and that is the spelling the two matched
//   siblings 0x407e90 and 0x408090 use. Declaring `unsigned int w =
//   p->size.width;` at the top of the arm, before the bounds test, moves the
//   index into edx/esi and the pointer into edx, which is worth 14 points. It
//   also materialises the fog arm's index multiply (`imul ebx,ecx` where the
//   inline form folds it into `imul ebx,[edx+0x80]`), which is what the
//   original does. g_game in edi and the index in edx look exclusive: every
//   shape that gets one loses the other, and no spelling moved g_game off ebp
//   while keeping the index in edx.
//
// WHAT IS LEFT, and it is all register allocation in the arms
//   * `g_game` sits in ebp where the original has it in edi. That single
//     register is worth about 14 lines, because in the fog arm it cascades:
//     with g_game in edi the original can put height>>1 and the width in ebp,
//     col in ebx, the mask pointer in ecx (row's dead register) and the cell
//     in edi (g_game's dead register), and every one of those then matches.
//     Nothing tried moved it: index hoisted before the test (60.3), width and
//     mask locals in the body (52.5), both at the top (49.5, g_game in ecx),
//     the mask pointer local after the test (52.5, g_game in edi but the index
//     back in cl), the arms as methods of the map struct (66.3, same as here),
//     the index passed as a parameter of either arm, a named game pointer
//     (61.7, g_game in esi), the width hoisted to Visible's scope and passed
//     in (36.2), col/row hoisted out of the arms (20.8), and the two arms
//     swapped in the source (56.9).
//   * the mask arm's body folds its two loads (`imul ecx,[edx+0x80]; add
//     ecx,[edx+0x7c]`) where the original materialises them (`mov edi,[edx
//     +0x80]; mov edx,[edx+0x7c]; imul edi,ecx`). The matched 0x407e90 has
//     the same expression materialised, so something in the original's source
//     stops the fold, and no local spelling does: `w` or `s` in the body, both
//     in the body, both before the test, the index into a local, a MapSize*
//     CSE breaker, `0 != ...`, unsigned col/row, both multiply orders and
//     `(p->seen + w*row)[col]` all still fold. Only an entry-block local
//     materialises, and that merges the two width loads into one and adds a
//     register copy, which costs more than the fold (65.7, 64.0, 59.3, 56.2).
//     Note the mask arm's local choice is otherwise score-neutral here: the
//     plain form and every body-local form all score 66.3, so unlike on
//     0x474b80 the mask arm does not need an extra local of its own.
//   * the fog arm's height>>1 and col are in ebx and edi where the original
//     has ebp and ebx, and the cell goes to cx with the `1` in edi where the
//     original has di and edi. All of it follows from g_game being in ebp.
//
// THE INLINED-CALLEE IDEA (tested here, does not work on this function)
//   Splitting the test's width read into a separate inlined helper, so that it
//   is not part of the enclosing body's CSE and the body's own read can
//   survive beside it, changes nothing: the local and the test's read still
//   merge into one load (checked with a second `InBounds` member beside
//   `Contains`, with the helper and with the width local in either order).
//   The two width loads already survive in the plain spelling, so the CSE is
//   not what keeps them apart here, and the fold is not a local effect: a
//   body-block local still folds. Consistent with the sibling's result that
//   MSVC 5 can only spill a named local, never rematerialise it.
//
// Ruled out with numbers, all in this round: the `Contains` spelling (every
// form is byte-identical), the load-CSE breaker for the position (a Pos* q
// local read in one place and the member in the other, which is what the
// three fresh movsx per arm need, already in place here), unsigned col/row,
// the two multiply orders, the index formed before the mask arm's test, the
// cell pointer formed before the fog arm's test, the index as a parameter, a
// Pos-level or class-level Visible with the arms as methods (17 to 22 percent,
// `this` gets clobbered into ebp), a VisExplored taking the map pointer, and
// hoisting col/row out of the arms.
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

// The two arms of the visibility test. The mask arm is the spelling the
// matched 0x407e90 uses for the same test; the fog arm is the spelling the
// matched 0x408090 uses, plus the `w` local described at the top of the file,
// which is what puts the player index in edx and the player pointer in edx
// rather than in cl and esi.
#pragma pack(push, 1)
static inline int ArmA(Map_004745e0* p, int col, int row)
{
    if (p->size.Contains((unsigned int)col, (unsigned int)row) &&
        p->seen[p->size.width * row + col] != 0)
        return 1;
    return 0;
}

static inline int ArmB(Map_004745e0* p, int col, int row)
{
    unsigned int w = p->size.width;
    if (!p->size.Contains((unsigned int)col, (unsigned int)row))
        return 0;
    return (g_game->visibilityMask[w * row + col] &
            (1 << g_game->playerIndex)) != 0;
}

// The record's position, with the inlined visibility test that reads it. The
// two arms re-read x, height and y from the record instead of reusing the
// values the caller just computed, so the position has to be reached through
// its own sub-struct here, not through the record's fields.
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
            return ArmA(p, col, row);
        }
        int col = x >> 5;
        int row = (y - (height >> 1)) >> 5;
        return ArmB(p, col, row);
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
