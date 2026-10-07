// Decompiled by Space Bunny Free, finished by Claude Sonnet 5.5 and deepseek-v4.1-flash, finished by space-bunny-free, edited by deepseek-v4.1, re-tried by space-bunny-free. Names are provisional.
// Caller, for whoever names this: the only caller is the thunk 0x47dac0, which
// forwards its own two arguments unchanged (`push arg2; push arg1; call`, so the
// last push is the callee's first parameter) and, when the scan succeeds, clears
// bit 2 of the byte at obj+0x10f, sets it from `flag & 1`, sets
// obj->[0x110] |= 0x8000000, calls 0x47c790(obj) and then 0x440a40 with the
// object's point at +0x76 and its point at +0x7e pushed by value. So arg1 is the
// object being placed and arg2 is a flag, as declared here.
//
// The windows.h include stays: without it both sums change operand order.
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

struct Game {
    char unknown_0[0x14233];
    int width;                          // +0x14233
    int height;                         // +0x14237
    char unknown_1423b[0x14287 - 0x1423b];
    Cell_0047d970* cells;               // +0x14287
};
#pragma pack(pop)

extern Game* g_game;

// Scans the rectangle the object covers, and fails if any cell the object's
// mask marks with `flag ? 2 : 4` belongs to a unit other than this one.
// FUNCTION: 0x47d970
int __stdcall IsFootprintClear(Obj_0047d970* obj, int flag)
{
    short* pp = &obj->pos.x;                  // x end reads pos.x through the alias
    Point_0047d970* q = &obj->pos;            // y end reads pos.y through this one
    Point_0047d970* r = &obj->size;
    short xend = pp[0] + obj->size.x;
    short yend = q->y + r->y;
    // Stays a 4-byte Point copy, not two short locals.
    Point_0047d970 p = obj->pos;
    if (p.x < 1 || p.y < 1 || xend >= g_game->width || yend >= g_game->height)
        return 0;
    int width = g_game->width;
    unsigned char bit = flag ? 2 : 4;
    int n = 0;
    for (int y = p.y; y < yend; y++) {
        Cell_0047d970* c = g_game->cells + y * width;
        for (int x = p.x; x < xend; x++) {
            // The post-increment stays inside the mask read, not at the loop bottom.
            if ((obj->unit->mask[n++] & bit) && c[x].field_0 != 0 && c[x].field_0 != obj->field_a8)
                return 0;
        }
    }
    return 1;
}
