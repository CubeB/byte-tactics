// Decompiled by Space Bunny Free, finished by Claude Sonnet 5.5. Names are provisional.
//
// PARTIAL: 98.0%, 322 of 322 bytes. One instruction pair differs: the first sum.
// The original computes the x end as `mov ax, [pos.x]; add ax, [size.x]` and the
// y end as `mov dx, [size.y]; add dx, [pos.y]`; ours loads `size.x` first for x
// (the y sum matches). Everything else is byte-identical, including the frame,
// the strength-reduced row pointer, the 13-byte cell stride and all the register
// choices.
//
// The fix that took it from 57.6 to 98.0 percent (Claude Sonnet 5.5, #571): write
// the mask read with the post-increment inside it, `mask[n++] & bit`, instead of
// `n++` at the bottom of the loop. The original increments right after the load
// (`mov ecx,[n]; mov bl,[ecx+edi]; inc ecx; test bl,bl; mov [n],ecx`), and with
// the increment merged MSVC keeps `n` in the dead `flag` argument slot and gives
// esi to the cell pointer, as the original does. With `n++` at the bottom it puts
// `n` in esi and spills `bit`, which cascaded through the whole loop.
//
// What was tried on the remaining x sum, none of which moved it: `size.x + pos.x`,
// the two sums in the other order, `xend = pos.x; xend += size.x`, `size.y +
// pos.y`, explicit `(short)` casts, `int` locals (56.3 percent, worse);
// tools/headers.py, all 128 sets (best 98.0, the empty set too); and N unused
// `extern int dummyK;` lines in front of the first pragma, K = 0 to 200 step 4,
// with and without <windows.h>: 98.0 (K <= 56) or 96.0 (K >= 60) with it, 98.0,
// 96.0 or 95.0 without, never MATCH. So it is not reachable by the declaration
// count or the headers tried; the operand order of a memory+memory 16-bit add may
// depend on state left by the original file's earlier functions.
//
// <windows.h> is kept because it is what makes the y sum keep its operand in dx.
#include <windows.h>
#pragma pack(push, 1)

struct Point_0047d970 {
    short x;
    short y;
};

struct Cell_0047d970 {
    short field_0;                      // +0x0, id of the unit owning the cell
    char unknown_2[0xd - 0x2];
};

struct Unit_0047d970 {
    char unknown_0[0x14e];
    unsigned char* mask;                // +0x14e, one byte per footprint cell
};

struct Obj_0047d970 {
    char unknown_0[0x76];
    Point_0047d970 pos;                 // +0x76
    char unknown_7a[4];
    Point_0047d970 size;                // +0x7e
    char unknown_82[0x92 - 0x82];
    Unit_0047d970* unit;                // +0x92
    char unknown_96[0xa8 - 0x96];
    short field_a8;                     // +0xa8, the owner's own id
};

struct Game_0047d970 {
    char unknown_0[0x14233];
    int width;                          // +0x14233
    int height;                         // +0x14237
    char unknown_1423b[0x14287 - 0x1423b];
    Cell_0047d970* cells;               // +0x14287
};
#pragma pack(pop)

extern Game_0047d970* g_game;

// Scans the rectangle the object covers, and fails if any cell the object's
// mask marks with `flag ? 2 : 4` belongs to a unit other than this one.
// FUNCTION: 0x47d970
int __stdcall FUN_0047d970(Obj_0047d970* obj, int flag)
{
    short xend = obj->pos.x + obj->size.x;
    short yend = obj->pos.y + obj->size.y;
    Point_0047d970 p = obj->pos;
    if (p.x < 1 || p.y < 1 || xend >= g_game->width || yend >= g_game->height)
        return 0;
    int width = g_game->width;
    unsigned char bit = flag ? 2 : 4;
    int n = 0;
    for (int y = p.y; y < yend; y++) {
        Cell_0047d970* c = g_game->cells + y * width;
        for (int x = p.x; x < xend; x++) {
            if ((obj->unit->mask[n++] & bit) && c[x].field_0 != 0 && c[x].field_0 != obj->field_a8)
                return 0;
        }
    }
    return 1;
}
