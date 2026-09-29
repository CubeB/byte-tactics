// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
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
