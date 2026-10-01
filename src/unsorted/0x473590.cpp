// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol. Names are provisional.
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
// in 0x4745e0.cpp and 0x475470.cpp, so that both index reads become
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

struct Game_00473590 {
    char unknown_0[0x1b63];
    Player_00473590 players[10];   // +0x1b63, stride 0x14b
    char unknown_2851[0x2a43 - 0x2851];
    unsigned char playerIndex;     // +0x2a43
    char unknown_2a44[0x14273 - 0x2a44];
    unsigned short* visibilityMask;// +0x14273
    char unknown_14277[0x14281 - 0x14277];
    unsigned char fogFlags;        // +0x14281
};

class Class_00473590 {
public:
    void* data;                    // +0x00
    char unknown_4[0x6 - 0x4];
    Pos_00473590 pos;              // +0x06
    int field_2c;                  // +0x2c
    void FUN_00473590(void* dest, short px, short py);
};
#pragma pack(pop)

extern Game_00473590* g_game;

// FUNCTION: 0x473590
void Class_00473590::FUN_00473590(void* dest, short px, short py)
{
    // The header reads the position through the pointer, the arms read the
    // member: two different expression trees, so MSVC keeps both load nodes and
    // re-reads the three shorts per arm instead of sharing the header's.
    Pos_00473590* q = &pos;
    short sx = q->x - px + 0x80;
    short sy = q->y - (q->h >> 1) - py + 0x20;
    Player_00473590* map = &g_game->players[g_game->playerIndex];
    int visible;
    if ((g_game->fogFlags & 2) == 2) {
        int col = pos.x >> 5;
        int row = (pos.y - (pos.h >> 1)) >> 5;
        // `seen` and `w` are the arm pressure that keeps the pre-branch
        // allocation on the original's registers; both are spilled, see above.
        unsigned char* seen = map->seen;
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
