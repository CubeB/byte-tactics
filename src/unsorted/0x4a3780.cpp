// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free. Names are provisional.
// PARTIAL, 34.9% (ours 1771 bytes against 1832). Every basic block, call site and
// entry/exit sequence is transcribed and the frame is now 0x3c with the point copy
// at [esp+0x34], matching the original. What is still wrong is ONE allocator state:
// which of the four long-lived values gets a callee-saved register. The original
// dedicates esi to obj (then recycles it), edi to point.y, ebx to y0 and ebp to me,
// and keeps entries, point.x, x0, x1, y1, span, step and flags in memory. Ours still
// hands a register to point.x and puts y0 in memory. Fixing that one choice would
// align the whole slot map (orig_sel 0x10, entries 0x14, n/span 0x18, step/flag8
// 0x1c, flags 0x20, x0 0x24, y0 0x28, x1 0x2c, y1 0x30) and most of the diff with
// it. What moved the number, in order:
//   - Point_004a3780 had to be 0x18 bytes (rep movsd x6), not 0xc: that alone took
//     the frame from 0x38 to the original's 0x3c, +5.5 points.
//   - hoisting `entries[0].count` into a local `cnt` (3 references to `entries`
//     became 2) demoted `entries` out of ebx and promoted obj into esi: +3.5.
//   - the `i < entries[0].count + 1` loop bound and the if/else spelling of `span`
//     reproduce the original's `jle` pre-test and its single span store: +0.5.
//   - the two `if (v == 0x7fffffff && v == 0x7fffffff)` lines are PURE ALLOCATION
//     LEVERS, the technique from the brief of adding a throwaway live reference
//     (one on y0 in the row walk, one on point.x in the 570 arm). They are folded
//     away by the front end but they change the variable weights. They are NOT
//     in the original; if point.x or y0 ever equals 0x7fffffff they would take a
//     different path, so a matcher should try to replace them with a construct
//     that is unobservable.
// Tried and did NOT work: plain int rel_x/rel_y instead of struct field updates
// (18.1%, the rep movsd disappears); a separate loop base pointer to demote
// `entries` (no change); a separate `Entry* e0` for entry 0 (no change); hoisting
// the loop bound into `last` (26.5%, it changes the latch); flags at function scope
// (folded away, no slot); declaring every local at the top of the function
// (no change); spelling the vertical test as `point.y - y0 > y1 - y0` (28.0%).
#include <string.h>

#pragma pack(push, 1)
struct Entry_004a3780 {                // 0x15b bytes
    unsigned char type;                // +0x00
    unsigned char kind;                // +0x01
    char unknown_02[0x13 - 0x02];
    short field_13;                    // +0x13
    short field_15;                    // +0x15
    short field_17;                    // +0x17
    short field_19;                    // +0x19
    int flags;                         // +0x1b
    char unknown_1f[0x28 - 0x1f];
    char group;                        // +0x28
    char unknown_29[0xb6 - 0x29];
    union {
        short count;                   // +0xb6 (entry 0)
        int field_b6;                  // +0xb6 (the scroll repeat timer)
    };
    short field_ba;                    // +0xba
    short field_bc;                    // +0xbc
    short field_be;                    // +0xbe
    short field_c0;                    // +0xc0
    void* field_c2;                    // +0xc2
    int field_c6;                      // +0xc6
    char unknown_ca[0xce - 0xca];
    void (__stdcall* field_ce)(void*, void*);  // +0xce
    char unknown_d2[0xd6 - 0xd2];
    int field_d6;                      // +0xd6
    short field_da;                    // +0xda
    char unknown_dc[0x15b - 0xdc];
};

struct List_004a3780 {
    char unknown_0[0xc];
    unsigned short* field_0c;          // +0x0c
};

struct Holder_004a3780 {
    int current;                       // +0x00
    Entry_004a3780* entries;           // +0x04
    char unknown_08[0x14 - 0x08];
    List_004a3780* list;               // +0x14
    char unknown_18[0x20 - 0x18];
    int field_20;                      // +0x20
};

struct Point_004a3780 {                // 0x18 bytes, copied with rep movsd x6
    int x;                             // +0x00
    int y;                             // +0x04
    int unknown_08[4];                 // +0x08
};

struct Object_004a3780 {
    char unknown_00[0x18];
    Holder_004a3780* holder;           // +0x18
    char unknown_1c[0x3c - 0x1c];
    Point_004a3780 point;              // +0x3c
    char unknown_54[0x60 - 0x54];
    int field_60;                      // +0x60
    int focus;                         // +0x64
    char unknown_68[0xcca - 0x68];
    int field_cca;                     // +0xcca
};
#pragma pack(pop)

extern Holder_004a3780* DAT_0051fba4;
extern char DAT_00502a20[];

void __stdcall FUN_004c1420(int id);
int FUN_004c1450();
void* __stdcall FUN_004b7f30(unsigned short* param_1, int param_2);
char* __stdcall FUN_004b6af0(void* text, int line);
int FUN_004b6340();
int __stdcall FUN_004ab570(Object_004a3780* obj, int mask);
int __stdcall FUN_004ab510(Object_004a3780* obj, int mask);
int __stdcall FUN_004ab5b0(Object_004a3780* obj, int mask);
void __stdcall FUN_004ab690(Object_004a3780* obj, int param_2);
void __stdcall FUN_0049fc50(Object_004a3780* obj, int index);
void __stdcall FUN_004a1b40(Object_004a3780* obj, int index);
void __stdcall FUN_004a2be0(Object_004a3780* obj, int index);

// FUNCTION: 0x4a3780
int __stdcall FUN_004a3780(Object_004a3780* obj, int index, int param_3)
{
    if (obj->field_60 != -1)
        return 0;
    Entry_004a3780* entries = obj->holder->entries;
    Entry_004a3780* me = &entries[index];
    int orig_sel = me->field_ba;
    if (me->field_c0 == 0)
        return 0;
    int x0;
    int y0;
    if (me->type == 0) {
        x0 = 0;
        y0 = 0;
    } else {
        x0 = me->field_13;
        y0 = me->field_15;
    }
    int x1 = me->field_17 + x0 - 1;
    int n = 0;
    int y1 = me->field_19 + y0 - 4;
    y0 += 2;
    Point_004a3780 point = obj->point;
    point.x -= entries[0].field_13;
    point.y -= entries[0].field_15;
    int i;
    int cnt = entries[0].count;
    for (i = 1; i < cnt + 1; i++) {
        if (entries[i].type == 7) {
            if (n == me->group) {
                FUN_004c1420(entries[i].field_d6);
                break;
            }
            n++;
        }
    }
    if (i == cnt + 1)
        FUN_004c1420(DAT_0051fba4->current);

    int size = (DAT_0051fba4->list == 0) ? FUN_004c1450()
        : (*(unsigned short*)((char*)FUN_004b7f30(DAT_0051fba4->list->field_0c, 0x49) + 2) + 2);
    int span;
    if (me->field_da == 0)
        span = size + 1;
    else
        span = me->field_da;
    int step = (me->field_19 - 2) / span;
    int flag8 = 0;

    if (FUN_004ab570(obj, 1)) {
        if (point.x < x0 || point.x > x1 || point.y < y0 || point.y > y1)
            goto after;
        if (point.x == 0x7fffffff && point.x == 0x7fffffff) goto after;
        if (me->field_c0 != 0) {
            if (!(me->flags & 0x200))
                return 1;
            {
                int line = (point.y - y0) / span + me->field_bc;
                me->field_ba = (short)line;
                if ((short)line < 0)
                    return 1;
                int off = (short)line - me->field_bc;
                if (off > step - 1)
                    me->field_ba = (short)(step + me->field_bc - 1);
                if ((short)me->field_ba >= me->field_c0 - 1)
                    me->field_ba = me->field_c0 - 1;
                char* s = FUN_004b6af0(me->field_c2, me->field_ba);
                if (strncmp(DAT_00502a20, s, 2) != 0)
                    return 1;
                me->field_ba = orig_sel;
                return 0;
            }
        }
    } else if (FUN_004ab510(obj, 1)) {
        if (point.x >= x0 && point.x <= x1 && point.y >= y0 && point.y <= y1) {
            FUN_0049fc50(obj, index);
            FUN_004ab690(obj, 1);
        }
    } else if (FUN_004ab510(obj, 2)) {
        if (point.x >= x0 && point.x <= x1 && point.y >= y0 && point.y <= y1) {
            FUN_0049fc50(obj, index);
            FUN_004ab690(obj, 2);
        }
    }

after:
    if (obj->focus != index)
        goto end;
    if (!FUN_004ab5b0(obj, 3))
        obj->focus = -1;
    if (point.x < x0)
        goto out;
    if (point.x > x1)
        goto out;
    if (point.y < y0)
        goto scroll_up;
    if (point.y > y1)
        goto out;
    {
        int flags = me->flags;
        obj->holder->field_20 = index;
        if (flags & 0x10) {
            int line = (point.y - y0) / span + me->field_bc;
            me->field_ba = (short)line;
            if ((short)line < 0) {
                me->field_ba = orig_sel;
            } else {
                int off = line - me->field_bc;
                if (off > step - 1)
                    me->field_ba = (short)(step + me->field_bc - 1);
                if (me->field_ba >= me->field_c0 - 1)
                    me->field_ba = me->field_c0 - 1;
                if (me->field_ba < 0)
                    me->field_ba = 0;
                if (flags & 0x200) {
                    char* s = FUN_004b6af0(me->field_c2, me->field_ba);
                    if (strncmp(DAT_00502a20, s, 2) == 0)
                        me->field_ba = orig_sel;
                }
                for (i = 1; i <= cnt; i++) {
                    Entry_004a3780* e = &entries[i];
                    if (e->type == 2 && e->kind == me->kind) {
                        short v = e->field_c0;
                        v--;
                        if (me->field_ba < v)
                            v = me->field_ba;
                        e->field_ba = v;
                    }
                }
            }
        } else if ((flags & 0x20) || (flags & 0x80)) {
            flag8 = (flags >> 7) & 1;
            int bc = me->field_bc;
            char* fixed = (char*)(me->field_c6 + bc * 0x18);
            int* itemp = (int*)(me->field_c6 + bc * 4);
            int remain = point.y - y0 - 2;
            if (remain == 0x7fffffff) remain = point.y - y0 - 2;
            int n2 = 0;
            int k = bc;
            while (1) {
                char* row = flag8 ? fixed : (char*)((*itemp) + 0x28);
                int h = (me->field_da != 0) ? span : *(unsigned short*)(row + 2);
                remain -= h;
                if (remain <= 0) {
                    me->field_ba = (short)(n2 + bc);
                    break;
                }
                n2++;
                k++;
                fixed += 0x18;
                itemp++;
                if (k > me->field_c0 - 1)
                    break;
            }
        }
    }

select_check:
    if (orig_sel != me->field_ba) {
        FUN_004a1b40(obj, index);
        if (me->field_ce)
            me->field_ce(obj, me);
    }
    if (me->flags & 0x40)
        return 1;
    obj->field_cca = 1;
    goto end;

out:
    if (point.y >= y0)
        goto scroll_down;

scroll_up:
    if (me->field_bc > 0 && me->field_b6 < FUN_004b6340()) {
        me->field_b6 = FUN_004b6340() + 2;
        if (me->field_ba > me->field_bc)
            me->field_ba = me->field_bc;
        me->field_ba--;
        me->field_bc--;
        if (me->field_c2 != 0) {
            int sel = me->field_ba;
            char* s = FUN_004b6af0(me->field_c2, (sel < 0) ? 0 : sel);
            if (strncmp(DAT_00502a20, s, 2) == 0)
                me->field_ba = orig_sel;
        }
        goto finish;
    }
    if (me->field_ba > 0) {
        me->field_ba = 0;
        goto finish;
    }
    goto end;

scroll_down:
    if (point.y <= y1)
        goto end;
    if (me->field_bc >= me->field_be)
        goto end;
    if (me->field_b6 >= FUN_004b6340())
        goto end;
    me->field_b6 = FUN_004b6340() + 2;
    me->field_bc++;
    {
        int sel = flag8 + me->field_bc - 1;
        me->field_ba = (short)sel;
        if (me->field_c2 != 0) {
            char* s = FUN_004b6af0(me->field_c2, sel);
            if (strncmp(DAT_00502a20, s, 2) == 0)
                me->field_ba = orig_sel;
        }
    }

finish:
    FUN_004a1b40(obj, index);
    FUN_004a2be0(obj, index);

end:
    return obj->field_60 != -1;
}
