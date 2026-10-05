// Decompiled by GPT-6-Luna, finished by Space Bunny Free, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by deepseek-v4.1-flash, finished by Space Bunny Free, finished by Space Bunny Free, finished by Space Bunny Free, finished by DeepSeek V4.1 Flash, checked by GPT-6, finished by claude-opus-5-5. Names are provisional.
// MATCH (claude-opus-5-5, #5131). The earlier passes (96.1% at best) were
// missing two things, and each one alone changes nothing:
//  - The cell index reads the position through a helper taking a
//    `const Point&`. Written inline, `obj->pos.y` in the index and
//    `pad.y = obj->pos.y - 1` below become one common subexpression in C2
//    (c2prio lists a temp on both lines), and while it exists the multiply
//    never takes the width as a memory operand. Through the reference the two
//    reads stay separate and `imul eax, [ebx+0x14233]` can fold.
//  - `#include <windows.h>` (the full header, as in the matched sibling
//    0x47cc30). Whether C2 folds that multiply also depends on how many types
//    are declared before the function: in a small test it folds only while
//    that count has bit 14 set (about 2300 to 4650 and 7000 to 9300
//    one-member structs), and the full windows.h lands in such a window
//    where the lean one does not. Without the header this file is 79.0%.
//    That count is g_game's symbol id (`c2prio.py --symbols g_game`,
//    docs/c2-regalloc.md "Symbol ids"; a one-member struct takes 7 ids):
//    29019 with the full header, 12192 with the lean one; 3748 extra
//    declarations before g_game (id 32767) still match, 3749 (32768) do
//    not, and the function's own id or the file total crossing 32768 do
//    nothing.
// With both, the guard is the obj-first compare and the width-pointer local
// of the 96.1% version is not needed.

#include <windows.h>

#pragma pack(push, 1)

struct Point {
    short x;
    short y;
};

struct Cell_0047db20 {
    short field_0;
    short field_2;
    char unknown_4[0xc - 0x4];
    unsigned char field_c;
};

struct Unit_0047db20 {
    char unknown_0[0x14e];
    unsigned char* mask;
};

union Flags_0047db20 {
    struct {
        unsigned int unknown_0 : 26;
        unsigned int flag26 : 1;
        unsigned int unknown_1 : 5;
    } bits;
    int all;
};

struct Obj_0047db20 {
    char unknown_0[0x76];
    Point pos;
    char unknown_7a[4];
    Point size;
    int field_82;
    char unknown_86[0x92 - 0x86];
    Unit_0047db20* unit;
    char unknown_96[0xa8 - 0x96];
    short field_a8;
    char unknown_aa[0x110 - 0xaa];
    Flags_0047db20 flags;
};

struct Game_0047db20 {
    char unknown_0[0x14233];
    int width;
    char unknown_14237[0x14287 - 0x14237];
    Cell_0047db20* cells;
    char unknown_1428b[0x142b7 - 0x1428b];
    int field_142b7;
};

class Class_0047db20 {
public:
    virtual void FUN_0047ed30();
};
#pragma pack(pop)

extern Game_0047db20* g_game;
extern Class_0047db20 DAT_004fd660[];

void __stdcall FUN_00483210(Point pos, Point size);
void __stdcall FUN_0047e5c0(Point pos, Point size, Class_0047db20* visitor);
void __stdcall FUN_00440a70(Obj_0047db20* obj);

static inline Cell_0047db20* CellAt(const Point& p)
{
    return &g_game->cells[p.y * g_game->width + p.x];
}

// FUNCTION: 0x47d0e0
void __stdcall FUN_0047d0e0(Obj_0047db20* obj)
{
    if (obj->field_82 != g_game->field_142b7) {
        Point size = obj->size;
        Cell_0047db20* cell = CellAt(obj->pos);
        int index = 0;
        if (obj->flags.all & 0x20000000) {
            for (int j = size.y; j > 0; j--) {
                for (int i = size.x; i > 0; i--) {
                    unsigned char m = obj->unit->mask[index];
                    index++;
                    if (cell->field_0 == obj->field_a8) cell->field_0 = 0;
                    if (m & 1) cell->field_c &= 0xfd;
                    cell++;
                }
                cell += g_game->width - size.x;
            }
            Point grown;
            grown.x = size.x + 2;
            grown.y = size.y + 2;
            Point pad;
            pad.x = obj->pos.x - 1;
            pad.y = obj->pos.y - 1;
            FUN_00483210(pad, grown);
        } else if ((obj->flags.all & 3) == 1) {
            for (int j = size.y; j > 0; j--) {
                for (int i = size.x; i > 0; i--) {
                    if (cell->field_0 == obj->field_a8) cell->field_0 = 0;
                    cell++;
                }
                cell += g_game->width - size.x;
            }
        } else if ((obj->flags.all & 3) == 2) {
            for (int j = size.y; j > 0; j--) {
                for (int i = size.x; i > 0; i--) {
                    if (cell->field_2 == obj->field_a8) cell->field_2 = 0;
                    cell++;
                }
                cell += g_game->width - size.x;
            }
        }
    }
    obj->flags.all &= ~0x08000000;
    if (obj->flags.bits.flag26) {
        obj->flags.all &= ~0x04000000;
        Class_0047db20 visitor;
        FUN_0047e5c0(obj->pos, obj->size, &visitor);
    }
    FUN_00440a70(obj);
}
