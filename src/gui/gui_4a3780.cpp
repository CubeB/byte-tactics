// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, finished by GPT-6, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, reworked by Claude Sonnet 5.5, finished by deepseek-v4.1-flash, finished by Space Bunny Free, rewritten by claude-opus-5-5, finished by DeepSeek V4.1 Flash, notes by claude-opus-5-5, checked by GPT-6, improved by claude-opus-5-5, matched by claude-opus-5-5. Names are provisional.
// MATCH (#5337). The last difference (obj reloaded into ecx at the
// field_cca store instead of kept in esi across the join, plus two scratch
// registers in the scroll paths) was the shared `return 1` block. In C2's
// joint live-range split (FUN_00438f79) every block joins the region of its
// first predecessor, in block order, that still has a free register, and a
// split point goes at the end of each predecessor in another region. With
// one label, the return block's first predecessor was the first arm's
// `!(flags & 0x200)` test, so a split was put at the end of the 0x40 test
// and cut obj's live range before the store. The original has two labels
// at the return: the `field_ba < 0` exit goes to `above:`, which the 0x40
// test falls into, and the other two first-arm exits go to `ret1:` just
// after it. `above:` stays an empty block until after allocation; its first
// predecessor with a region is the 0x40 test (the `field_ba < 0` block has
// no free register), so the split lands at the end of the empty block,
// where obj is not live. The labels in the other order, or any other pair
// of exits on `above:`, stay at 93.4%; an empty statement between the two
// labels, or both labels at the end of the function, also match.
// Load-bearing, from earlier passes (#5158 and before):
//  - The flag8 block declares `k` after the pointer choice and indexes the
//    rows with me->field_bc directly; with `int k = me->field_bc;` before
//    the `if (flag8)` the flags local takes ecx and loses its frame slot.
//  - `int remain` before `int n2 = 0;`, and the 0x10 clamps written plainly
//    (`>= field_c0 - 1`, then `if (me->field_ba < 0) me->field_ba = 0;`).
//  - The rectangle is the inlined GetGadgetRect (matched), called as
//    `GetGadgetRect(&entries[index], &r)` like the sibling 0x4a1b40; with
//    `me` the y1 lea's operands swap.
//  - `if (me->field_c0 == 0) goto skip0;` with skip0 at the end of the
//    in-rect block: the original reloads point.x on that edge only.
//  - The scroll-up/scroll-down tails share one DrawListBox/FUN_004a2be0
//    pair through `goto finish` (MSVC 5 does not merge return blocks, so
//    the first arm's exits are gotos too).
//  - The 0x10 line computation stores straight into me->field_ba and tests
//    the field; the SkipTextLines result goes through `char* s`; the sync
//    loop clamp is min().
//  - The 0x20 test is `flags & 0x20 | 0x80`, an original bug kept as is: it
//    parses as `(flags & 0x20) | 0x80` and is always true (docs/bugs.md).
#include <string.h>
#include <windows.h>

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
    char* field_c2;                    // +0xc2
    void* field_c6;                    // +0xc6
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

extern Holder_004a3780* g_guiContext;
extern char DAT_00502a20[];

void __stdcall SetFont(int id);
int GetFontHeight();
int __stdcall GetGafFrame(unsigned short* param_1, int param_2);
char* __stdcall SkipTextLines(char* text, int n);
int GetTicks();
int __stdcall IsDoubleClickMessage(Object_004a3780* obj, unsigned char buttons);
int __stdcall IsMouseButtonMessage(Object_004a3780* obj, unsigned char buttons);
int __stdcall FUN_004ab5b0(Object_004a3780* obj, unsigned int mask);
void __stdcall FUN_004ab690(Object_004a3780* obj, int param_2);
void __stdcall FUN_0049fc50(Object_004a3780* obj, int index);
void __stdcall DrawListBox(Object_004a3780* obj, int index);
void __stdcall FUN_004a2be0(Object_004a3780* obj, int index);

struct Row_004a3780 {
    short unknown_0;
    unsigned short height;             // +0x02
    char unknown_4[0x18 - 0x4];
};

struct Item_004a3780 {
    char unknown_0[0x28];
    Row_004a3780* row;                 // +0x28
};

void __stdcall GetGadgetRect(Entry_004a3780* entry, Rect_004a3780* rect)
{
    if (entry->type == 0) {
        rect->x0 = 0;
        rect->y0 = 0;
    } else {
        rect->x0 = entry->field_13;
        rect->y0 = entry->field_15;
    }
    rect->x1 = entry->field_17 + rect->x0 - 1;
    rect->y1 = entry->field_19 + rect->y0 - 1;
}

struct Glyph_004a3780 {
    unsigned short width;              // +0x00
    unsigned short height;             // +0x02
};

static inline int LineHeight_004a3780()
{
    if (0 == g_guiContext->list)
        return GetFontHeight();
    return ((Glyph_004a3780*)GetGafFrame(g_guiContext->list->field_0c, 0x49))->height + 2;
}

// FUNCTION: 0x4a3780
int __stdcall HandleListBoxInput(Object_004a3780* obj, int index, int param_3)
{
    if (obj->field_60 != -1)
        return 0;
    Entry_004a3780* entries = obj->holder->entries;
    Entry_004a3780* me = &entries[index];
    int orig_sel = me->field_ba;
    if (me->field_c0 == 0)
        return 0;
    Rect_004a3780 r;
    unsigned int flags;
    GetGadgetRect(&entries[index], &r);
    int n = 0;
    r.y0 += 2;
    r.y1 -= 3;
    Point_004a3780 point = obj->point;
    point.x -= entries[0].field_13;
    point.y -= entries[0].field_15;
    int i;
    for (i = 1; i < entries[0].count + 1; i++) {
        if (entries[i].type == 7) {
            if (n == me->group) {
                SetFont(entries[i].field_d6);
                break;
            }
            n++;
        }
    }
    if (i == entries[0].count + 1)
        SetFont(g_guiContext->current);

    int size = LineHeight_004a3780();
    short da = me->field_da;
    int span = (da != 0) ? da : size + 1;
    int step = (me->field_19 - 2) / span;
    char* s;

    if (IsDoubleClickMessage(obj, 1)) {
        if (point.x >= r.x0 && point.x <= r.x1 && point.y >= r.y0 && point.y <= r.y1) {
            if (me->field_c0 == 0) goto skip0;
            if (!(me->flags & 0x200))
                goto ret1;
            me->field_ba = (point.y - r.y0) / span + me->field_bc;
            if (me->field_ba < 0)
                goto above;
            if (me->field_ba - me->field_bc > step - 1)
                me->field_ba = me->field_bc + step - 1;
            if (me->field_ba >= me->field_c0 - 1)
                me->field_ba = me->field_c0 - 1;
            s = SkipTextLines(me->field_c2, me->field_ba);
            if (strncmp(DAT_00502a20, s, 2) != 0)
                goto ret1;
            me->field_ba = orig_sel;
            return 0;
skip0:;
        }
    } else if (IsMouseButtonMessage(obj, 1)) {
        if (point.x >= r.x0 && point.x <= r.x1 && point.y >= r.y0 && point.y <= r.y1) {
            FUN_0049fc50(obj, index);
            FUN_004ab690(obj, 1);
        }
    } else if (IsMouseButtonMessage(obj, 2)) {
        if (point.x >= r.x0 && point.x <= r.x1 && point.y >= r.y0 && point.y <= r.y1) {
            FUN_0049fc50(obj, index);
            FUN_004ab690(obj, 2);
        }
    }

    if (obj->focus != index)
        goto end;
    if (!FUN_004ab5b0(obj, 3))
        obj->focus = -1;
    if (point.x >= r.x0 && point.x <= r.x1 && point.y >= r.y0 && point.y <= r.y1) {
        obj->holder->field_20 = index;
        flags = me->flags;
        if (flags & 0x10) {
            me->field_ba = (point.y - r.y0) / span + me->field_bc;
            if (me->field_ba >= 0) {
                if (me->field_ba - me->field_bc > step - 1)
                    me->field_ba = me->field_bc + step - 1;
                if (me->field_ba >= me->field_c0 - 1)
                    me->field_ba = me->field_c0 - 1;
                if (me->field_ba < 0)
                    me->field_ba = 0;
                if (flags & 0x200) {
                    s = SkipTextLines(me->field_c2, me->field_ba);
                    if (strncmp(DAT_00502a20, s, 2) == 0)
                        me->field_ba = orig_sel;
                }
                for (i = 1; i <= entries[0].count; i++) {
                    if (entries[i].type == 2 && entries[i].kind == me->kind)
                        entries[i].field_ba = min(entries[i].field_c0 - 1, me->field_ba);
                }
            } else {
                me->field_ba = orig_sel;
            }
        } else if (flags & 0x20 | 0x80) {
            // Original bug, kept: `(flags & 0x20) | 0x80` is always true
            // (and ecx,0x20 / or cl,0x80 / test cl,cl at 0x4a3c29).
            int flag8 = (flags >> 7) & 1;
            Row_004a3780* fixed;
            Item_004a3780** ip;
            if (flag8)
                fixed = &((Row_004a3780*)me->field_c6)[me->field_bc];
            else
                ip = &((Item_004a3780**)me->field_c6)[me->field_bc];
            int k = me->field_bc;
            int remain = point.y - r.y0 - 2;
            int n2 = 0;
            for (;;) {
                Row_004a3780* row = flag8 ? fixed : (*ip)->row;
                if (me->field_da != 0)
                    remain -= span;
                else
                    remain -= row->height;
                if (remain <= 0) {
                    me->field_ba = n2 + me->field_bc;
                    break;
                }
                if (flag8)
                    fixed++;
                else
                    ip++;
                n2++;
                k++;
                if (k > me->field_c0 - 1)
                    break;
            }
        }
        if (orig_sel != me->field_ba) {
            DrawListBox(obj, index);
            if (me->field_ce)
                me->field_ce(obj, me);
        }
        if (me->flags & 0x40) {
above:
ret1:
            return 1;
        }
        obj->field_cca = 1;
    } else if (point.y < r.y0) {
        if (me->field_bc > 0 && me->field_b6 < GetTicks()) {
            me->field_b6 = GetTicks() + 2;
            if (me->field_ba > me->field_bc)
                me->field_ba = me->field_bc;
            me->field_bc--;
            me->field_ba--;
            short sel = me->field_ba;
            if (me->field_c2 != 0) {
                s = SkipTextLines(me->field_c2, sel < 0 ? 0 : sel);
                if (strncmp(DAT_00502a20, s, 2) == 0)
                    me->field_ba = orig_sel;
            }
            goto finish;
        }
        if (me->field_ba > 0) {
            me->field_ba = 0;
            goto finish;
        }
    } else if (point.y > r.y1) {
        if (me->field_bc < me->field_be && me->field_b6 < GetTicks()) {
            me->field_b6 = GetTicks() + 2;
            me->field_bc++;
            me->field_ba = me->field_bc + step - 1;
            if (me->field_c2 != 0) {
                s = SkipTextLines(me->field_c2, me->field_ba);
                if (strncmp(DAT_00502a20, s, 2) == 0)
                    me->field_ba = orig_sel;
            }
finish:
            DrawListBox(obj, index);
            FUN_004a2be0(obj, index);
        }
    }
end:
    return obj->field_60 != -1;
}
