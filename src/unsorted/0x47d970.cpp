// Decompiled by space-bunny-free. Names are provisional.
//
// PARTIAL: 57.6%. The semantics are recovered and the frame, the bounds checks,
// the strength-reduced row pointer, the 13-byte cell stride and the whole
// nested loop shape match. What is left is register allocation: the prologue
// picks different registers, which cascades into the loop body.
//
// What still differs:
//  - the two bound sums. The original loads one operand of each into a
//    register and folds the other: `mov ax, [pos.x]; add ax, [size.x]` and
//    `mov dx, [size.y]; add dx, [pos.y]`. Ours always loads the size operand
//    (`mov ax, [size.x]; add ax, [pos.x]`, `mov dx, [size.y]; add dx, [pos.y]`),
//    so only the y sum lines up. Ruled out: swapping the source operand order,
//    splitting each sum into an assignment plus `+=`, taking one operand from a
//    scalar short local, taking both from a Point local, and declaring the
//    Point before or after the sums; all of them compile to the same form, so
//    MSVC fixes the operand order of a memory+memory add here;
//  - `g_game` goes in edi in ours and esi in the original (esi is later reused
//    for the cell pointer), which also moves `push edi` and the dword load of
//    the position one instruction later;
//  - the mask index `n` lives in esi in ours and in the (dead) second argument
//    slot in the original, and our `bit` is spilled to a byte-sized copy of
//    that slot instead of staying in dl for the whole loop. The two locals
//    compete for that slot; declaring them in either order, making `bit` an
//    int or a char and making `n` unsigned all give the same code, so the
//    original's choice (the counter in the slot, the mask byte in dl) is not
//    reachable from the source shapes tried;
//  - so the cell pointer is in ecx in ours and esi in the original, and the
//    original does `mov bl, [ecx+edi]; and bl, dl; test bl, bl` where ours
//    folds the load into `test byte ptr [esi+edi], dl`.
//
// <windows.h> is included because headers.py names it (with <stdlib.h> and
// <math.h>) as the closest header set: it is what makes the y sum keep its
// operand in dx, 55.6% without it.
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
            if ((obj->unit->mask[n] & bit) && c[x].field_0 != 0 && c[x].field_0 != obj->field_a8)
                return 0;
            n++;
        }
    }
    return 1;
}
