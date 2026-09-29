// Decompiled by deepseek-v4.1-flash. Names are provisional.
// PARTIAL, 22.3% (ours 1857 bytes against 1832). The full control flow is
// transcribed and the entry struct offsets are right. What still differs is
// the stack frame and register allocation, which are one and the same:
//   - the original frame is 0x3c and its point copy lives at [esp+0x34]; ours
//     is 0x38 with the copy at [esp+0x30]. The missing 4-byte local is the
//     original's dead y0 home at [esp+0x28] (never read or written anywhere in
//     the exe, but it shifts x1 to 0x2c, y1 to 0x30 and the point to 0x34).
//   - the original's slot map is orig_sel 0x10, entries 0x14, n/span 0x18,
//     step/flag 0x1c, flags 0x20, x0 0x24, dead y0 0x28, x1 0x2c, y1 0x30,
//     point 0x34. Ours is n 0x14, orig_sel 0x18, x0 0x1c, entries 0x28,
//     point 0x30, with no y0 home.
//   - the original keeps x0 in memory and y0 (top) in ebx; ours keeps x0 in
//     memory but y0 in eax, so the type==0 arm has an extra `xor ecx,ecx`.
// Everything else historically in this family (see 0x4a99c0, 0x4a3ef0,
// 0x4a7290) is in place: the type-7 group search + FUN_004c1420(DAT current)
// fallback, the fungl/field_da span, the &G guard, the type-2 row sync loop,
// the fixed-stride vs item-pointer row walk, and the up/down scroll bodies.
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
    short field_da;                    // +0xda, unused by this function
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

struct Point_004a3780 {
    int x;                             // +0x00
    int y;                             // +0x04
    int unknown_08[4];
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
    int orig_sel;
    Entry_004a3780* entries = obj->holder->entries;
    Entry_004a3780* me = &entries[index];
    int n;
    int span;
    int step;
    int flags;
    int x0;
    int y0;
    int x1;
    int y1;
    Point_004a3780 point;

    orig_sel = me->field_ba;
    if (me->field_c0 == 0)
        return 0;
    if (me->type == 0) {
        x0 = 0;
        y0 = 0;
    } else {
        x0 = me->field_13;
        y0 = me->field_15;
    }
    x1 = me->field_17 + x0 - 1;
    n = 0;
    y1 = me->field_19 + y0 - 4;
    y0 += 2;
    point = obj->point;
    int rel_x = point.x - entries[0].field_13;
    int rel_y = point.y - entries[0].field_15;
    int i;
    for (i = 1; i < entries[0].count + 1; i++) {
        if (entries[i].type == 7) {
            if (n == me->group) {
                FUN_004c1420(entries[i].field_d6);
                break;
            }
            n++;
        }
    }
    if (i == entries[0].count + 1)
        FUN_004c1420(DAT_0051fba4->current);

    int size = (DAT_0051fba4->list == 0) ? FUN_004c1450()
        : (*(unsigned short*)((char*)FUN_004b7f30(DAT_0051fba4->list->field_0c, 0x49) + 2) + 2);
    size++;
    span = me->field_da;
    if (span == 0)
        span = size;
    step = (me->field_19 - 2) / span;

    if (FUN_004ab570(obj, 1)) {
        if (rel_x < x0 || rel_x > x1 || rel_y < y0 || rel_y > y1)
            goto after;
        if (me->field_c0 == 0)
            goto after;
        if (!(me->flags & 0x200))
            return 1;
        {
            int line = (rel_y - y0) / span + me->field_bc;
            me->field_ba = line;
            if ((short)line < 0)
                return 1;
            int off = (short)line - me->field_bc;
            if (off > step - 1)
                me->field_ba = step - 1 + me->field_bc;
            if ((short)me->field_ba >= me->field_c0 - 1)
                me->field_ba = me->field_c0 - 1;
            char* s = FUN_004b6af0(me->field_c2, me->field_ba);
            if (strncmp(DAT_00502a20, s, 2) != 0)
                return 1;
            me->field_ba = orig_sel;
            return 0;
        }
    } else if (FUN_004ab510(obj, 1)) {
        if (rel_x >= x0 && rel_x <= x1 && rel_y >= y0 && rel_y <= y1) {
            FUN_0049fc50(obj, index);
            FUN_004ab690(obj, 1);
        }
    } else if (FUN_004ab510(obj, 2)) {
        if (rel_x >= x0 && rel_x <= x1 && rel_y >= y0 && rel_y <= y1) {
            FUN_0049fc50(obj, index);
            FUN_004ab690(obj, 2);
        }
    }

after:
    if (obj->focus != index)
        goto end;
    if (!FUN_004ab5b0(obj, 3))
        obj->focus = -1;
    if (rel_x < x0)
        goto out;
    if (rel_x > x1)
        goto out;
    if (rel_y < y0)
        goto scroll_up;
    if (rel_y > y1)
        goto out;
    {
        flags = me->flags;
        obj->holder->field_20 = index;
        if (flags & 0x10) {
            short bc = me->field_bc;
            short line = (short)((rel_y - y0) / span + bc);
            me->field_ba = line;
            if (line < 0) {
                me->field_ba = orig_sel;
            } else {
                int off = line - bc;
                if (off > step - 1)
                    me->field_ba = step - 1 + bc;
                if (me->field_ba >= me->field_c0 - 1)
                    me->field_ba = me->field_c0 - 1;
                if (me->field_ba < 0)
                    me->field_ba = 0;
                if (flags & 0x200) {
                    char* s = FUN_004b6af0(me->field_c2, me->field_ba);
                    if (strncmp(DAT_00502a20, s, 2) == 0)
                        me->field_ba = orig_sel;
                }
                for (i = 1; i <= entries[0].count; i++) {
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
            int flag8 = (flags >> 7) & 1;
            int bc = me->field_bc;
            int remain = rel_y - y0 - 2;
            int n2 = 0;
            int k = bc;
            if (flag8) {
                for (;;) {
                    char* row = (char*)(me->field_c6 + k * 0x18);
                    int h = (me->field_da != 0) ? span
                        : *(unsigned short*)(row + 2);
                    remain -= h;
                    if (remain <= 0) {
                        me->field_ba = (short)(n2 + bc);
                        break;
                    }
                    n2++;
                    k++;
                    if (k > me->field_c0 - 1)
                        break;
                }
            } else {
                for (;;) {
                    char* row = (char*)(*(int*)(me->field_c6 + k * 4) + 0x28);
                    int h = (me->field_da != 0) ? span
                        : *(unsigned short*)(row + 2);
                    remain -= h;
                    if (remain <= 0) {
                        me->field_ba = (short)(n2 + bc);
                        break;
                    }
                    n2++;
                    k++;
                    if (k > me->field_c0 - 1)
                        break;
                }
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
    if (rel_y >= y0)
        goto scroll_down;

scroll_up:
    if (me->field_bc > 0 && !(FUN_004b6340() <= me->field_b6)) {
        me->field_b6 = FUN_004b6340() + 2;
        short bc = me->field_bc;
        if (me->field_ba > bc)
            me->field_ba = bc;
        me->field_ba--;
        me->field_bc = bc - 1;
        short sel = me->field_ba;
        if (me->field_c2 != 0) {
            int arg = (sel < 0) ? 0 : sel;
            char* s = FUN_004b6af0(me->field_c2, arg);
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
    if (rel_y <= y1)
        goto end;
    if (me->field_bc >= me->field_be)
        goto end;
    if (me->field_b6 >= FUN_004b6340())
        goto end;
    me->field_b6 = FUN_004b6340() + 2;
    me->field_bc++;
    {
        int sel = ((me->flags >> 7) & 1) + me->field_bc - 1;
        me->field_ba = (short)sel;
        if (me->field_c2 != 0) {
            char* s = FUN_004b6af0(me->field_c2, (short)sel);
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
