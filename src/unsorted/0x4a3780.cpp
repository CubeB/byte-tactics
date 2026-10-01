// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, finished by GPT-6, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, reworked by Claude Sonnet 5.5. Names are provisional.
// PARTIAL: 52.3%, 1806 bytes against 1832, frame 0x3c like the original.
//
// What the Sonnet 5.5 pass (#3246) found, 35.4 to 52.3:
//  1. x0, y0, x1, y1 are ONE `Rect` local, not four ints. The original's slots
//     0x24 x0, 0x28 (never touched: y0 lives in ebx), 0x2c x1, 0x30 y1 are four
//     consecutive dwords, and the dead one is the member of a struct local whose
//     other members are in memory. With four separate ints the frame was 0x34
//     and every [esp+N] was 8 low; with the struct the frame is exactly 0x3c.
//  2. The three early `return 1` paths (no 0x200 flag, negative line, strncmp
//     mismatch) all jump to the ONE `mov eax,1; ret` block that belongs to
//     `if (me->flags & 0x40) return 1;` at the end, while the `return 0` ones are
//     duplicated epilogues. `goto ret1;` to a label inside that last `if` gives
//     the shared block (1841 to 1806 bytes). `int flags` has to be declared above
//     the gotos (C2362).
//  3. `short da = me->field_da; int span = (da != 0) ? da : size + 1;` gives the
//     original's `mov cx,[da]; test cx,cx; movsx ecx,cx; jne; lea ecx,[eax+1]`.
//  4. The second-loop clamp is int arithmetic (`int v = e->field_c0 - 1;
//     if (v >= ba) v = ba;`), not `short v`.
//  5. 46.4 to 52.3: the duplicate `|| point.x < r.x0` / `|| point.x > r.x1` in the
//     two `goto out` tests after `after:`. The duplicate folds late and the four
//     jumps become the original's `jl out / jg out / jl scroll_up / jg out`.
//     The same duplicates in the first chains or inside an `&&` chain do nothing.
// Structure still to apply (scratch z4, 49.0%, 1784 bytes: the block layout is
// the original's but a register web moves and the score drops): after the three
// compare blocks the original is
//     if (obj->focus == index) {
//         if (!FUN_004ab5b0(obj, 3)) obj->focus = -1;
//         if (px >= x0 && px <= x1 && py >= y0 && py <= y1) { flags block;
//             select check; if (flags & 0x40) { ret1: return 1; } field_cca = 1; }
//         else if (py < y0) { scroll up, tail FUN_004a1b40 + FUN_004a2be0 }
//         else if (py > y1) { scroll down, same tail }
//     }
//     return obj->field_60 != -1;
// (scroll up is laid out BEFORE scroll down; with `goto out` the compiler puts
// scroll down first). Also the second block's condition is `flags & 0x20 | 0x80`
// (always true, compiles to `and ecx,0x20 / or cl,0x80 / test cl,cl`, +6 bytes),
// and scroll up's argument is `short sel = ba; (sel < 0) ? 0 : sel`.
// What still differs: ONE register rotation. The original keeps py in edi and
// the loop index / obj / px reloads in esi; this compiles with py in esi and the
// others in edi (a toy test shows MSVC 5 hands esi, edi, ebx, ebp in that order
// to the webs with the most references, loops weigh little: a loop counter with
// 5 references loses to a variable with 6 straight-line ones). Also the
// original has a short local `nsel = me->field_c0` in esi in the first block
// (`mov si,[ebp+0xc0] / test si,si`, a `mov esi,[esp+0x34]` reload on the
// field_c0 == 0 path); every spelling here folds it to a memory compare, and the
// reload block is what keeps the two FUN_004ab690 call tails from merging.
// Tried without effect (flat or worse): `int i = 1` in four positions, duplicate
// `||` uses of i, n, py, `do/while(0)` and while forms of the loop, pointer walk,
// inline Rect::Set (37.8%, whole function reallocates), dummy extern-int sweep
// 0..400 step 8 (flat).
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

struct Rect_004a3780 { int x0, y0, x1, y1; };

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
    Rect_004a3780 r;
    if (me->type == 0) {
        r.x0 = 0;
        r.y0 = 0;
    } else {
        r.x0 = me->field_13;
        r.y0 = me->field_15;
    }
    r.x1 = me->field_17 + r.x0 - 1;
    int n = 0;
    r.y1 = me->field_19 + r.y0 - 4;
    r.y0 += 2;
    Point_004a3780 point = obj->point;
    point.y -= entries[0].field_15;
    point.x -= entries[0].field_13;
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
    short da = me->field_da;
    int span = (da != 0) ? da : size + 1;
    int step = (me->field_19 - 2) / span;
    int flag8 = 0;

    if (FUN_004ab570(obj, 1)) {
        if (point.x < r.x0 || point.x > r.x1 || point.y < r.y0 || point.y > r.y1)
            goto after;
        if (me->field_c0 != 0) {
            if (!(me->flags & 0x200))
                goto ret1;
            {
                int line = (point.y - r.y0) / span + me->field_bc;
                me->field_ba = (short)line;
                if ((short)line < 0)
                    goto ret1;
                int off = (short)line - me->field_bc;
                if (off > step - 1)
                    me->field_ba = (short)(step + me->field_bc - 1);
                if ((short)me->field_ba >= me->field_c0 - 1)
                    me->field_ba = me->field_c0 - 1;
                char* s = FUN_004b6af0(me->field_c2, me->field_ba);
                if (strncmp(DAT_00502a20, s, 2) != 0)
                    goto ret1;
                me->field_ba = orig_sel;
                return 0;
            }
        }
    } else if (FUN_004ab510(obj, 1)) {
        if (point.x >= r.x0 && point.x <= r.x1 && point.y >= r.y0 && point.y <= r.y1) {
            FUN_0049fc50(obj, index);
            FUN_004ab690(obj, 1);
        }
    } else if (FUN_004ab510(obj, 2)) {
        if (point.x >= r.x0 && point.x <= r.x1 && point.y >= r.y0 && point.y <= r.y1) {
            FUN_0049fc50(obj, index);
            FUN_004ab690(obj, 2);
        }
    }

after:
    if (obj->focus != index)
        goto end;
    if (!FUN_004ab5b0(obj, 3))
        obj->focus = -1;
    if (point.x < r.x0 || point.x < r.x0)
        goto out;
    if (point.x > r.x1 || point.x > r.x1)
        goto out;
    if (point.y < r.y0)
        goto scroll_up;
    if (point.y > r.y1)
        goto out;
    {
        obj->holder->field_20 = index;
        int flags = me->flags;
        if (flags & 0x10) {
            int line = (point.y - r.y0) / span + me->field_bc;
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
                        int v = e->field_c0 - 1;
                        if (v >= me->field_ba)
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
            int remain = point.y - r.y0 - 2;
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
    if (me->flags & 0x40) {
ret1:
        return 1;
    }
    obj->field_cca = 1;
    goto end;

out:
    if (point.y >= r.y0)
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
    if (point.y <= r.y1)
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