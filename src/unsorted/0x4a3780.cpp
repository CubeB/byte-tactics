// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, finished by GPT-6. Names are provisional.
// Gave up at 34.7% (1841 bytes against 1832). Remove artificial allocation
// expression. Scroll down selects using visible-row count. Variable-height
// rows dereference the bitmap pointer at +0x28. Preserve short narrowing and
// read flags after writing the current index. Remaining local slots and
// callee-saved-register allocation differ throughout.
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
        obj->holder->field_20 = index;
        int flags = me->flags;
        if (flags & 0x10) {
            int line = (point.y - y0) / span + me->field_bc;
            me->field_ba = (short)line;
            if ((short)line < 0) {
                me->field_ba = orig_sel;
            } else {
                int off = (short)line - me->field_bc;
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
            flag8 = (flags >> 7) & 1;
            int bc = me->field_bc;
            char* fixed = (char*)(me->field_c6 + bc * 0x18);
            int* itemp = (int*)(me->field_c6 + bc * 4);
            int remain = point.y - y0 - 2;
            int n2 = 0;
            int k = bc;
            while (1) {
                char* row = flag8 ? fixed : *(char**)((*itemp) + 0x28);
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
        short sel = (short)(step + me->field_bc - 1);
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
