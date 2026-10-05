// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1, finished by space-bunny-free, finished by GPT-6.1-sol, finished by deepseek-v4.1-flash, edited by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by claude-sonnet-5-5, finished by Space Bunny Free, finished by claude-sonnet-5-5, finished by DeepSeek V4.1 Flash, finished by claude-opus-5-5. Names are provisional.
//
// MATCH. claude-opus-5-5 (#4890), from 94.4%:
//  - The first hit test builds a 16-byte Rect local (`box`, block scoped). Its
//    left/top live in eax/ecx and its right/bottom spill into box's own slots,
//    which share the frame with the later `point` copy: [esp+0x34]/[esp+0x38]
//    are box.right/box.bottom (point is [esp+0x2c..0x43]). Two named ints
//    could never land there. 94.4 -> 95.0.
//  - Case 13 is the real FUN_004a4890 (matched) inlined, written with the
//    entry array in its own local first (`entries = menu->layer->entries;
//    e = &entries[i];`, the way FUN_004a7190 starts). The standalone 0x4a4890
//    matches with either that or `e = &obj->table->entries[i]`, but inlined
//    here the one-expression form scores 91.6%: the layer load is right but
//    value/max, the loop-end reload of `entries` and the whole help-text
//    block each come out one register along an edx->eax->ecx rotation.
//    95.0 -> MATCH.
//  - The tail is the real FUN_004a7830 (matched) inlined with the real
//    FUN_004a7190 inside it, then the real FUN_004a9660 (the layer pop); the
//    help-text block is the real FUN_004a0090. These give the same bytes as
//    the hand-written forms they replace. The FUN_004a1810 group scan stays a
//    call on the case 5 path only, so that path keeps its own helper
//    (SelectCurrentByName) with the explicit call; writing it as
//    FUN_004a7830 -> FUN_004a7190 -> FUN_004a1810 inlines the scan there
//    too (2292 bytes), even one inline level deeper.
//  - Case 1/12's shared `menu->layer->dirty = 1` tail is MSVC's own tail
//    merge: writing it in both cases gives the same bytes as a goto.
//
// Older notes (DeepSeek V4.1 Flash and earlier), still true:
//   * Real early returns: the original has a full prologue, then `test layer;
//     jne body; xor eax,eax; pop x4; ret 4`. MSVC 5 shrink-wraps a lone early
//     return but not once the function has further early `return`s, so the
//     middle of the function uses them too.
//   * `entries` is read after the field_cca block, not at the top.
//   * The first hit test loads the point's y early (`int ptY = pt.y;` before
//     right and bottom); without it, and with a PtInBox-style helper, the
//     score drops to 79-82%.
//   * `int k; Entry* e;` declared at the top of the function.
//   * Case 5 is `if (found != sel) { big } else { sel = i; }`, and the
//     non-type-1 arm nests `if (field_29 != 0) { if (type == 4 && field_157 !=
//     0) sel = -1; else {...} } else sel = -1;`.
#include <windows.h>
#include <string.h>

#pragma pack(push, 1)
struct Menu_004a9fd0;
struct Layer_004a9fd0;

struct Entry_004a9fd0;                // 0x15b bytes

struct EntryAnim_004a9fd0 {           // +0xb6
    short count;                      // +0xb6 (entry 0 only)
    char unknown_b8[2];
    int field_ba;                     // +0xba
    int field_be;                     // +0xbe
    int field_c2;                     // +0xc2
    int field_c6;                     // +0xc6
    float field_ca;                   // +0xca
    int field_ce;                     // +0xce
    int field_d2;                     // +0xd2
};

struct EntryTail_004a9fd0 {
    unsigned char field_136;          // +0x136
    unsigned char field_137;          // +0x137
    short field_138;                  // +0x138
    char unknown_13a[2];
    unsigned char field_13c;          // +0x13c
    char unknown_13d[0x146 - 0x13d];
};

struct Entry_004a9fd0 {
    unsigned char type;               // +0x00
    char unknown_01;
    char name2[0x10];                 // +0x02
    char unknown_12;
    short x;                          // +0x13
    short y;                          // +0x15
    short w;                          // +0x17
    short h;                          // +0x19
    int align;                        // +0x1b
    union {
        int colourIndex;       // +0x1f
        int timer;                    // +0x1f
    } u1f;
    char unknown_23[0x28 - 0x23];
    char group;                       // +0x28
    char field_29;                    // +0x29
    char unknown_2a[0x33 - 0x2a];
    char helpKey[0xb6 - 0x33];        // +0x33
    union {
        char text[0x20];              // +0xb6
        EntryAnim_004a9fd0 anim;      // +0xb6
    } u_b6;
    int language;                     // +0xd6
    char unknown_da[0x136 - 0xda];
    union {
        char name[0x10];              // +0x136
        EntryTail_004a9fd0 c;
    } u136;
    char unknown_146[0x157 - 0x146];
    int field_157;                    // +0x157
};

struct Layer_004a9fd0 {
    Layer_004a9fd0* prev;             // +0x00
    Entry_004a9fd0* entries;          // +0x04
    void (__stdcall* cb8)(Menu_004a9fd0*);   // +0x08
    char unknown_0c[4];
    int flags;                        // +0x10
    int dirty;                        // +0x14
    void* field_18;                   // +0x18
    void (__stdcall* cb1c)();         // +0x1c
    int current;                      // +0x20
    char unknown_24[4];
    char text[0xe];                   // +0x28
    char field_36;                    // +0x36
    char unknown_37[0x3b - 0x37];
    void (__stdcall* cb3b)(Menu_004a9fd0*);  // +0x3b
};

struct Point_004a9fd0 {
    int x;                            // +0x00
    int y;                            // +0x04
    int unknown_8[4];
};

struct Rect_004a9fd0 {
    int left;
    int top;
    int right;
    int bottom;
};

struct Menu_004a9fd0 {
    char unknown_00[0x18];
    Layer_004a9fd0* layer;            // +0x18
    char unknown_1c[0x3c - 0x1c];
    Point_004a9fd0 point;             // +0x3c
    char unknown_54[0x60 - 0x54];
    int field_60;                     // +0x60
    int focus;                        // +0x64
    int field_68;                     // +0x68
    int field_6c;                     // +0x6c
    char unknown_70[0x96 - 0x70];
    int field_96;                     // +0x96
    int field_9a;                     // +0x9a
    char unknown_9e[0xa2 - 0x9e];
    int field_a2;                     // +0xa2
    char unknown_a6[0x8b2 - 0xa6];
    unsigned char palette[0x100];
    char unknown_9b2[0xcca - 0x9b2];
    int field_cca;                    // +0xcca
};

struct Class_0051fba4 {
    int group;                        // +0x00
    char unknown_04[0x14 - 0x04];
};
#pragma pack(pop)

extern Class_0051fba4* DAT_0051fba4;
extern int DAT_0051fbb4;
extern char DAT_005119b8[];

int FUN_004b6340();
void __stdcall FUN_004ab5d0(Menu_004a9fd0*);
int FUN_004c1b00();
int FUN_004c1ab0();
int __stdcall FUN_004a9b90(Menu_004a9fd0*, int);
int __cdecl toupper(int);
void __stdcall FUN_004a81e0(Menu_004a9fd0*, unsigned int);
void __stdcall FUN_004ab440(Menu_004a9fd0*, int);
int __stdcall FUN_004c1b80(int);
int __stdcall FUN_004a6ae0(Menu_004a9fd0*, int, int);
int __stdcall FUN_004a3780(Menu_004a9fd0*, int, int);
int __stdcall FUN_004a7290(Menu_004a9fd0*, int, int);
void __stdcall FUN_004a4170(Menu_004a9fd0*, int);
int __stdcall FUN_004a4440(Menu_004a9fd0*, int, int);
int __stdcall FUN_004a4b50(Menu_004a9fd0*, int);
void __stdcall FUN_004a5f40(Menu_004a9fd0*, int);
void __stdcall FUN_004a5e50(Menu_004a9fd0*, int);
void __stdcall FUN_004a4660(Menu_004a9fd0*, int);
int FUN_004c13f0();
void __stdcall FUN_004c13a0(int, int);
int __stdcall FUN_004a1810(Entry_004a9fd0*, int);
int __stdcall FUN_0049fc50(Menu_004a9fd0*, int);
void __stdcall FUN_004ab6c0(Menu_004a9fd0*, int, char*, int, int);
void FUN_004c1a40();
void __stdcall FUN_004c1420(int);
char* __stdcall FUN_004c5740(void*);
void FUN_004c2470();
void FUN_004c2870();
void __cdecl FUN_004d85a0(Layer_004a9fd0*);

static inline int FindEntry(Entry_004a9fd0* entries, char* name)
{
    for (int i = 1; i < entries->u_b6.anim.count + 1; i++) {
        if (strncmp(entries[i].name2, name, 0x10) == 0)
            return i;
    }
    return -1;
}

// FUN_004a7190's body with its group scan left as a call to FUN_004a1810, as
// on the case 5 path here (and twice in FUN_004a7290).
static inline void SelectCurrentByName(Menu_004a9fd0* menu, Entry_004a9fd0* entries, int sel)
{
    Entry_004a9fd0* entry = &entries[sel];
    FUN_004c13a0(menu->palette[entry->u1f.colourIndex], FUN_004c13f0());
    FUN_004a1810(entries, sel);
    FUN_0049fc50(menu, sel);
    menu->layer->current = sel;
    FUN_004ab6c0(menu, sel, entry->u_b6.text, entry->u136.c.field_138, 0);
    FUN_004c1a40();
}

// The real FUN_004a0090 (matched in 0x4a0090.cpp), inlined here by /Ob2.
static inline void FUN_004a0090(Menu_004a9fd0* obj)
{
    char* text = DAT_005119b8;
    if (obj->field_68 != -1) {
        obj->field_6c = obj->field_68;
        text = obj->layer->entries[obj->field_68].helpKey;
    }
    int found = FindEntry(obj->layer->entries, "HELPTEXT");
    if (found != -1) {
        strcpy(obj->layer->entries[found].u_b6.text, FUN_004c5740(text));
        obj->field_cca = 1;
    }
}

// The real FUN_004a4890 (matched in 0x4a4890.cpp), inlined here by /Ob2.
static inline void FUN_004a4890(Menu_004a9fd0* menu, int i)
{
    Entry_004a9fd0* entries = menu->layer->entries;
    Entry_004a9fd0* e = &entries[i];
    if (e->u_b6.anim.field_ce && e->u_b6.anim.field_ba < e->u_b6.anim.field_be) {
        if (FUN_004b6340() > e->u_b6.anim.field_c6) {
            e->u_b6.anim.field_ba += (int)e->u_b6.anim.field_ca;
            if (e->u_b6.anim.field_ba > e->u_b6.anim.field_be) {
                e->u_b6.anim.field_ba = e->u_b6.anim.field_be;
                e->u_b6.anim.field_ce = 0;
            }
            e->u_b6.anim.field_c6 = FUN_004b6340() + e->u_b6.anim.field_c2;
        }
        FUN_004a4660(menu, i);
    }
}

// The real FUN_004a7190 (matched in 0x4a7190.cpp), inlined here by /Ob2.
static inline void FUN_004a7190(Menu_004a9fd0* obj, int index)
{
    Entry_004a9fd0* entries = obj->layer->entries;
    Entry_004a9fd0* target = &entries[index];

    FUN_004c13a0(obj->palette[target->u1f.colourIndex], FUN_004c13f0());

    int n = 0;
    int i = 1;
    for (; i < entries->u_b6.anim.count + 1; i++) {
        if (entries[i].type == 7) {
            if (n == target->group) {
                FUN_004c1420(entries[i].language);
                break;
            }
            n++;
        }
    }
    if (i == entries->u_b6.anim.count + 1)
        FUN_004c1420(DAT_0051fba4->group);

    FUN_0049fc50(obj, index);
    obj->layer->current = index;
    FUN_004ab6c0(obj, index, target->u_b6.text, target->u136.c.field_138, 0);
    FUN_004c1a40();
}

// The real FUN_004a7830 (matched in 0x4a7830.cpp), inlined here by /Ob2.
static inline void FUN_004a7830(Menu_004a9fd0* menu, int index)
{
    Entry_004a9fd0* first = menu->layer->entries;
    menu->focus = -1;
    menu->layer->current = index;
    if (first[menu->layer->current].type == 3)
        FUN_004a7190(menu, menu->layer->current);
}

// The real FUN_004a9660 (matched in 0x4a9660.cpp), inlined here by /Ob2.
static inline void FUN_004a9660(Menu_004a9fd0* gui)
{
    if (gui->layer) {
        unsigned int flags = gui->layer->flags;
        gui->field_68 = gui->field_60 = gui->focus = -1;
        if (gui->layer->cb8)
            gui->layer->cb8(gui);
        FUN_004c2470();
        FUN_004a81e0(gui, 2);
        FUN_004c2870();
        Layer_004a9fd0* old = gui->layer;
        gui->layer = old->prev;
        if (gui->layer)
            gui->layer->dirty = 1;
        FUN_004d85a0(old);
        if (flags & 0x800)
            FUN_004a81e0(gui, 0x40);
    }
}

// FUNCTION: 0x4a9fd0
int __stdcall FUN_004a9fd0(Menu_004a9fd0* menu)
{
    int k;
    Entry_004a9fd0* e;
    int i;
    Entry_004a9fd0* entries = 0;

    if (menu->layer == 0)
        return 0;

    int now = FUN_004b6340();
    menu->field_9a = now - menu->field_96;
    menu->field_96 = now;
    FUN_004ab5d0(menu);

    int key;
    if (menu->layer->field_18 == 0) {
        key = FUN_004c1b00();
        if (key >= 0xe2 && key <= 0xeb)
            key = 0;
    } else {
        key = FUN_004c1ab0();
    }

    if (menu->layer->field_18 != 0 && key != 0 && menu->field_a2 != 0) {
        key = FUN_004a9b90(menu, key);
        if (key != 0) {
            for (int n = 0; n < 0xe; n++)
                menu->layer->text[n] = menu->layer->text[n + 1];
            menu->layer->field_36 = (char)toupper(key);
            if (menu->layer->cb3b != 0)
                menu->layer->cb3b(menu);
            menu->field_60 = -1;
        }
    }

    int sel = menu->field_60;
    if (menu->layer == 0)
        return 1;
    if (menu->field_cca == 1) {
        menu->field_cca = 0;
        FUN_004a81e0(menu, menu->layer->flags | 0x40);
    }

    entries = menu->layer->entries;
    if (entries == 0)
        return 1;

    Point_004a9fd0& pt = menu->point;
    {
        Rect_004a9fd0 box;
        box.left = entries->x;
        box.top = entries->y;
        if (entries->type != 0) {
            box.left *= 2;
            box.top *= 2;
        }
        int ptY = pt.y;
        box.right = entries->w + box.left - 1;
        box.bottom = entries->h + box.top - 1;
        FUN_004ab440(menu, pt.x >= box.left && pt.x <= box.right &&
                           ptY >= box.top && ptY <= box.bottom);
    }

    int saved = menu->field_68;
    menu->field_68 = -1;

    Point_004a9fd0 point;
    memcpy(&point, &menu->point, 24);
    point.x -= entries->x;
    point.y -= entries->y;

    int elapsed;
    if (FUN_004b6340() - DAT_0051fbb4 > 0) {
        elapsed = 1;
        DAT_0051fbb4 = FUN_004b6340();
    } else {
        elapsed = 0;
    }

    i = 1;
    for (; i < entries->u_b6.anim.count + 1; i++) {
        e = &entries[i];
        if (e->field_29 != 0) {
            int x, y;
            if (e->type == 0) {
                x = 0;
                y = 0;
            } else {
                x = e->x;
                y = e->y;
            }
            int right = e->w + x - 1;
            int bottom = e->h + y - 1;
            if (point.x >= x && point.x <= right && point.y >= y && point.y <= bottom)
                menu->field_68 = i;

            k = FUN_004c1b80(0xfb) == 0 ? key : 0;
            switch (e->type) {
            case 1:
                if (FUN_004a6ae0(menu, i, key) == 1)
                    sel = i;
                if (e->u1f.timer != 0 && elapsed) {
                    e->u1f.timer -= 2;
                    if (e->u1f.timer < 0)
                        e->u1f.timer = 0;
                    if (menu->layer != 0)
                        menu->layer->dirty = 1;
                }
                break;
            case 2:
                if (FUN_004a3780(menu, i, 0) == 1)
                    sel = i;
                break;
            case 3:
                if (FUN_004a7290(menu, i, k) == 1)
                    sel = i;
                break;
            case 4:
                FUN_004a4170(menu, i);
                break;
            case 5:
                if (FUN_004a4440(menu, i, key) != 0) {
                    sel = -1;
                    int found = FindEntry(entries, e->u136.name);
                    if (found != sel) {
                        Entry_004a9fd0* me;
                        sel = found;
                        me = &entries[found];
                        if (me->type == 1) {
                            if (me->field_29 == 0 || (me->u136.c.field_13c & 1) != 0) {
                                sel = -1;
                            } else {
                                me->u136.c.field_137++;
                                if (me->u136.c.field_137 >= me->u136.c.field_136)
                                    me->u136.c.field_137 = 0;
                                FUN_004a5f40(menu, found);
                            }
                        } else {
                            if (me->field_29 != 0) {
                                if (me->type == 4 && me->field_157 != 0) {
                                    sel = -1;
                                } else {
                                    Entry_004a9fd0* entriesNow = menu->layer->entries;
                                    menu->focus = -1;
                                    menu->layer->current = found;
                                    if (entriesNow[menu->layer->current].type == 3)
                                        SelectCurrentByName(menu, menu->layer->entries, menu->layer->current);
                                }
                            } else {
                                sel = -1;
                            }
                        }
                    } else {
                        sel = i;
                    }
                }
                break;
            case 6:
                if (FUN_004a4b50(menu, i) == 1)
                    sel = i;
                break;
            case 13:
                FUN_004a4890(menu, i);
                break;
            case 12:
                if (e->u1f.timer != 0 && elapsed) {
                        e->u1f.timer--;
                        FUN_004a5e50(menu, i);
                        if (menu->layer != 0)
                            menu->layer->dirty = 1;
                    }
                break;
            }
        }
        if (sel != -1)
            break;
    }

    if (menu->field_68 != saved)
        FUN_004a0090(menu);

    if (menu->layer->cb1c != 0)
        menu->layer->cb1c();

    if (sel != -1) {
        menu->field_60 = sel;
        FUN_004a7830(menu, sel);
        if (menu->layer->cb8 != 0)
            menu->layer->cb8(menu);
        if (menu->field_60 != -1)
            FUN_004a9660(menu);
    }
    return 1;
}