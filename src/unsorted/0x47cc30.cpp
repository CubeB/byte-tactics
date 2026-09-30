// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, further by deepseek-v4.1-flash. Names are provisional.
// deepseek-v4.1-flash pass (#1571): `tools/headers.py` found `<windows.h>` takes
// this from 64.5% to 66.5% (ours now 1202 bytes vs the original 1199). That is
// the best of the 128 header sets; the file keeps the include. All the remaining
// hunks are the same register-role differences listed below (the header only
// removed two bytes elsewhere). Free-scored this pass and rejected: a separate
// `int w = size.x` used for the bounds, inner counter, advance and grown.x (47.0,
// 1208 bytes), the same as `short w` (64.5, no change, the compiler CSEs it back
// to size.x), swapping the two negative checks (63.9), inverting the owner test
// to `active != 0 && type == 3` with the bad path in the else (62.6), using
// `g_game->cells` instead of the `game` local for the cell base (61.2), and
// dropping the `game` local entirely (61.2). Without windows.h those last two
// score 62.7, so the include is load bearing twice over.
//
// BEST SO FAR: 66.5%, ours 1202 bytes vs original 1199. Frame is now 0x18 and
// g_game is in ebp, both matching. What fixed it: introducing a named
// `Game_0047cc30* game = g_game;` local assigned right after the two negative
// checks. That gave the loaded pointer a live range starting at the bounds test,
// so MSVC allocated ebp to it instead of hoisting `xor ebp,ebp`; `test ax,ax`
// and the dead position.y spill at [esp+0x20] then fall out correctly.
//
// Still differs (all register roles, no structural diff):
//  * size.x should stay in ebx across the function (the original does
//    `movsx ebx,dx`, spills it to [esp+0x2c] only as a backup for loop 1, and
//    reloads it with `mov ebx,[esp+0x2c]` after each inner loop). Ours spills
//    size.x immediately and uses edx as the width accumulator; the height test
//    then takes ebx for size.y. Fixing this one allocation should cascade.
//  * The named `game` local makes MSVC emit a `mov edi,ebp` copy for the
//    owner lookup (original uses ebp directly), and it holds game in edi for
//    the cell computation instead of reloading `mov ebp,[g_game]`.
//  * Loop 1 roles: original has outer counter at [esp+0x10], inner counter in
//    edi, mask index in ebp, mask byte in bl. Ours has outer counter in ebx
//    (so `mov bl` clobbers it), inner counter in ebp, index in edi. Loops 2 and
//    3 likewise swap the row counter to the stack and reload.
//  * `mov cx, word [esi+0xa8]` vs ours `mov ax, ...` in the mask loop write.
// Tried and rejected: comparison operand swap `g_game->width <= x` (60.2%),
// separate `int sx = size.x; int sy = size.y;` used everywhere (46.1%, 1214
// bytes, adds locals), and declaring the Point size first (62.7%, kept the
// earlier ecx reuse but not the register roles).
// PARTIAL. The mask loop body itself matches byte for byte, including
// `inc ebp / mov bl,byte [ecx+ebp-1] / neg al / sbb eax,eax / and al,0xfe /
// add eax,4 / test bl,al`, which came from keeping `unsigned char m` a local
// inside the loop body and from `unsigned char bit = obj->bit2 ? 2 : 4;`.
#include <windows.h>
#pragma pack(push, 1)

struct Obj_0047cc30;

struct Owner_0047cc30 {
    char unknown_0[6];
    Obj_0047cc30* first;                // +0x6
};

struct Unit_0047cc30 {
    char unknown_0[0x14e];
    unsigned char* mask;                // +0x14e
};

struct Player_0047cc30 {
    int active;                         // +0x0
    char unknown_4[0x73 - 0x4];
    unsigned char type;                 // +0x73
};

struct UnitRec_0047cc30 {               // 0x118 bytes
    char unknown_0[0x92];
    void* def;                          // +0x92
    Player_0047cc30* owner;             // +0x96
    char unknown_9a[0x110 - 0x9a];
    unsigned int flags;                 // +0x110
    char unknown_114[0x118 - 0x114];
};

struct Cell_0047cc30 {
    unsigned short field_0;             // +0x0
    unsigned short field_2;             // +0x2
    char unknown_4[0xc - 0x4];
    unsigned char field_c;              // +0xc
};

struct Point_0047cc30 {
    short x;
    short y;
};

struct Position_0047cc30 {
    int x;
    int y;
    int z;
};

union Flags_0047cc30 {
    struct {
        unsigned int unknown_0 : 27;
        unsigned int flag27 : 1;
        unsigned int unknown_1 : 4;
    } bits;
    int all;
};

struct Obj_0047cc30 {
    unsigned char* field_0;             // +0x0
    char unknown_4[0x26 - 0x4];
    int field_26;                       // +0x26
    char unknown_2a[0x6a - 0x2a];
    Position_0047cc30 position;         // +0x6a
    Point_0047cc30 pos;                 // +0x76
    char unknown_7a[4];
    Point_0047cc30 size;                // +0x7e
    Owner_0047cc30* owner;              // +0x82
    int field_86;                       // +0x86
    char unknown_8a[4];
    Obj_0047cc30* next;                 // +0x8e
    Unit_0047cc30* unit;                // +0x92
    char unknown_96[0xa8 - 0x96];
    unsigned short field_a8;            // +0xa8
    char unknown_aa[0x10f - 0xaa];
    unsigned char bit0 : 1;             // +0x10f
    unsigned char bit1 : 1;
    unsigned char bit2 : 1;
    Flags_0047cc30 flags;               // +0x110
};

struct Game_0047cc30 {
    char unknown_0[0x14233];
    int width;                          // +0x14233
    int height;                         // +0x14237
    char unknown_1423b[0x14287 - 0x1423b];
    Cell_0047cc30* cells;               // +0x14287
    char unknown_1428b[0x1429f - 0x1428b];
    Owner_0047cc30* owners;             // +0x1429f
    int ownerCols;                      // +0x142a3
    char unknown_142a7[0x142b7 - 0x142a7];
    Owner_0047cc30* defaultOwner;       // +0x142b7
    char unknown_142bb[0x14357 - 0x142bb];
    UnitRec_0047cc30* units;            // +0x14357
    char unknown_1435b[0x38a47 - 0x1435b];
    int field_38a47;                    // +0x38a47
};
#pragma pack(pop)

extern Game_0047cc30* g_game;

void __stdcall FUN_00483210(Point_0047cc30 pos, Point_0047cc30 size);
void __stdcall FUN_00440a40(Point_0047cc30 pos, Point_0047cc30 size);


static void SetOwner_0047cc30(Obj_0047cc30* obj, Owner_0047cc30* nw)
{
    if (nw != obj->owner) {
        int fl = obj->field_86;
        if (fl == 0) {
            Owner_0047cc30* old = obj->owner;
            if (old != 0) {
                Obj_0047cc30** pp = &old->first;
                while (*pp != obj)
                    pp = &(*pp)->next;
                *pp = obj->next;
                obj->next = 0;
            }
            obj->next = nw->first;
            nw->first = obj;
        }
        obj->owner = nw;
    }
}

// FUNCTION: 0x47cc30
void __stdcall FUN_0047cc30(Obj_0047cc30* obj)
{
    Point_0047cc30 size = obj->size;
    if (obj->field_0 != 0)
        *(int*)(obj->field_0 + 0x26) = g_game->field_38a47;
    if (obj->pos.x < 0 || obj->pos.y < 0)
        goto remove;
    Game_0047cc30* game;
    if (obj->pos.x + size.x >= g_game->width || obj->pos.y + size.y >= g_game->height)
        goto remove;
    game = g_game;

    Position_0047cc30 pp = obj->position;
    SetOwner_0047cc30(obj, &game->owners[(pp.x >> 23) + (pp.z >> 23) * game->ownerCols]);
    {
        Cell_0047cc30* cell = &game->cells[game->width * obj->pos.y + obj->pos.x];
        unsigned int f = obj->flags.all;
        int index = 0;

        if (f & 0x20000000) {
            for (int j = size.y; j > 0; j--) {
                for (int i = size.x; i > 0; i--) {
                    unsigned char m = obj->unit->mask[index];
                    index++;
                    unsigned char bit = obj->bit2 ? 2 : 4;
                    UnitRec_0047cc30* rec;
                    if (m & bit) {
                        unsigned short id = cell->field_0;
                        if (id == 0)
                            goto a_write;
                        rec = &g_game->units[id];
                        if (rec->owner->active == 0)
                            goto a_bad;
                        if (rec->owner->type != 3)
                            goto a_bad;
                        rec->flags |= 0x8000000;
                        obj->flags.all |= 0x4000000;
                    a_write:
                        cell->field_0 = obj->field_a8;
                        goto a_next;
                    a_bad:
                        rec->flags |= 0x4000000;
                        obj->flags.all |= 0x8000000;
                    }
                a_next: ;
                    if (m & 1)
                        cell->field_c |= 2;
                    cell++;
                }
                cell += g_game->width - size.x;
            }
            Point_0047cc30 grown;
            grown.x = size.x + 2;
            grown.y = size.y + 2;
            Point_0047cc30 pad;
            pad.x = obj->pos.x - 1;
            pad.y = obj->pos.y - 1;
            FUN_00483210(pad, grown);
            FUN_00440a40(obj->pos, obj->size);
            return;
        }
        if ((f & 3) == 1) {
            for (int j = size.y; j > 0; j--) {
                for (int i = size.x; i > 0; i--) {
                    unsigned short id = cell->field_0;
                    if (id != 0) {
                        UnitRec_0047cc30* rec = &g_game->units[id];
                        if (rec->owner->active == 0 || rec->owner->type != 3) {
                            rec->flags |= 0x4000000;
                            obj->flags.all |= 0x8000000;
                            goto b_next;
                        }
                        rec->flags |= 0x8000000;
                        obj->flags.all |= 0x4000000;
                    }
                    cell->field_0 = obj->field_a8;
                b_next: ;
                    cell++;
                }
                cell += g_game->width - size.x;
            }
            return;
        }
        if ((f & 3) == 2) {
            for (int j = size.y; j > 0; j--) {
                for (int i = size.x; i > 0; i--) {
                    unsigned short id = cell->field_2;
                    if (id != 0) {
                        UnitRec_0047cc30* rec = &g_game->units[id];
                        if (rec->owner->active == 0 || rec->owner->type != 3) {
                            rec->flags |= 0x4000000;
                            obj->flags.all |= 0x8000000;
                            goto c_next;
                        }
                        rec->flags |= 0x8000000;
                        obj->flags.all |= 0x4000000;
                    }
                    cell->field_2 = obj->field_a8;
                c_next: ;
                    cell++;
                }
                cell += g_game->width - size.x;
            }
        }
    }
    return;

remove:
    SetOwner_0047cc30(obj, g_game->defaultOwner);
}
