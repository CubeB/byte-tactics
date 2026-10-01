// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1, finished by space-bunny-free, finished by GPT-6.1-sol, finished by deepseek-v4.1-flash, edited by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by claude-sonnet-5-5, finished by Space Bunny Free. Names are provisional.
//
// Partial: 73.5%, 2108 bytes versus 2164 (was 53.1% / 2180).
//
// Space Bunny Free session, 53.1 -> 73.5. Seven structural fixes:
//   * the "select current" block at the tail of the function is 0x4a76b0's
//     `static inline void DoSelect(menu, entries, sel)` (a MATCH) called as
//     `if (menu->layer->entries[menu->layer->current].type == 3)
//          DoSelect(menu, menu->layer->entries, menu->layer->current);`
//     It reproduces the group scan (the type 7 walk with the `n` counter,
//     entries[i].language, and the DAT_0051fba4->group fallback), the
//     FUN_0049fc50 call, the re-store of menu->layer->current = sel and the
//     FUN_004ab6c0 call. With the next point: 53.1 -> 61.1;
//   * case 5's entry-name search is 0x4a76b0's `FindEntry` helper, and case 5's
//     own type-3 test reads the cached local `entries` (as 0x4a76b0 does).
//     61.1 -> 61.4;
//   * `i` is initialised with `i = 1;` as a statement right before the loop
//     instead of in the declaration, and the first clamp reads the point
//     through a reference `Point_004a9fd0& pt = menu->point;`. 61.4 -> 62.1;
//   * `entries` is read (`entries = menu->layer->entries;`) immediately after
//     the null test, with the declaration at the top of the function.
//     62.1 -> 70.9;
//   * the help-text block gives `ptr` its initialiser at the declaration
//     (`char* ptr = DAT_005119b8;` with the branch flipped) and spells the
//     count check `>=`; the search itself is the same FindEntry shape as
//     0x4a76b0 (`FindHelp`), which drops the dead count+1 re-test after the
//     loop. 70.9 -> 72.6;
//   * case 5's two give-up arms are one test
//     (`me->field_29 == 0 || (me->type == 4 && me->field_157 != 0)`) so both
//     share the `sel = -1` store. 72.6 -> 72.7;
//   * the early `if (menu->layer == 0) return 0;` becomes
//     `if (menu->layer != 0) { ... }` with the single `return 1` afterwards.
//     MSVC 5 then has no early return at the entry block to shrink-wrap, so
//     it saves ebx, ebp, esi, edi up front exactly as the original does, and
//     every [esp+N] frame slot lines up. 72.7 -> 73.5.
//
// Still differs (register allocation / scheduling, nothing structural):
//   1. The null test's taken edge: the original falls through to the early
//      `return 0` block (xor eax,eax / pop x4 / add esp,0x34 / ret 4) with
//      `jne` into the body; ours jumps to the shared tail with `je`. The
//      single-exit form is what stops the shrink-wrap, so the two shapes pull
//      against each other and this is where a MATCH would have to give.
//   2. Entry loop: the original keeps i in EBX and the strength-reduced entry
//      pointer in EDI (spill slots [esp+0x10]/[esp+0x14]) and reloads `entries`
//      from [esp+0x18] at the loop head; ours keeps `entries` in a register up
//      to the loop head, so i and the pointer stay in memory with ESI/EDI/EBX
//      used as temps.
//   3. First entry clamp: the original loads point.y into EDI early and spills
//      BOTH `right` and `bottom` to their own frame slots ([esp+0x34]/[esp+0x38]),
//      comparing against the memory operands; ours gives EDI to `bottom` and
//      reloads point.y for the second half of the test.
//   4. The two `type == 3` select tests (case 5 and the tail): the original
//      loads menu->layer->entries into EDX before the focus/current stores (for
//      the test) and reloads it into EBX for the helper body; ours merges the
//      two loads.
//
// Measured with no effect or worse (do not repeat): all 128 header sets
// (tools/headers.py, best is the written <windows.h> <string.h>), declaration
// order of i/key/sel at the function top (all six orders, 58.2 to 61.1),
// a `layer` local used for every `menu->layer` (28.1), `sel` read above the
// null test, `int i = 1` in the for header (67.2), `for (i = 1; ...; i++, e++)`
// (60.2), `++i` (60.0), `register` on i and e (52.5), the loop bound read
// through `menu->layer->entries` (70.8), `e = &menu->layer->entries[i]` (60.6),
// a hoisted `int last = count + 1` bound (67.5), the loop as a `while` with the
// increment at the bottom (73.5, identical), case 5's select tail as its own
// inline helper (59.6), the tail block's test on the cached `entries` (69.8),
// both select tests fresh (59.6), the first clamp through px/py locals (72.5),
// through a copied `Point pt` (72.4), in an inline helper (73.5, identical),
// with `bottom` computed before `right` (72.5 / 73.3), with the `pt` reference
// declared after the bounds (70.2 / 70.4), with a nested `inside` flag (72.6),
// the help-text branch with the non-null arm first (67.0), case 5's search with
// the not-found arm first (55.8), `menu->layer = menu->layer->prev` (identical),
// `notnull` cached before the test (72.7), a `layer` local used only by the test
// (72.7), the give-up arms reordered or with `field_29` in a temp (72.5 / 71.7),
// `entries` read after the `now`/key blocks instead of right after the test
// (71.6 / 70.8), and a 15 minute tools/permute.py run at the 61.4 baseline
// (61.4 -> 62.4) and another at 70.9 (70.9 -> 71.6 before it was stopped),
// both below this file and both holding only `tmp0`/`inl0` artifacts and dead
// `goto` skips that read as implausible source.

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
    char unknown_2a[0xb6 - 0x2a];
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
extern char DAT_005098c4[];

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

// The type==3 body shared with 0x4a7190/0x4a76b0/0x4a7830/0x4a7960.
static inline void SelectCurrent(Menu_004a9fd0* menu, Entry_004a9fd0* entries, int sel)
{
    Entry_004a9fd0* entry = &entries[sel];
    FUN_004c13a0(menu->palette[entry->u1f.colourIndex], FUN_004c13f0());

    int n = 0;
    int i = 1;
    for (; i < entries->u_b6.anim.count + 1; i++) {
        if (entries[i].type == 7) {
            if (n == entry->group) {
                FUN_004c1420(entries[i].language);
                break;
            }
            n++;
        }
    }
    if (i == entries->u_b6.anim.count + 1)
        FUN_004c1420(DAT_0051fba4->group);

    FUN_0049fc50(menu, sel);
    menu->layer->current = sel;
    FUN_004ab6c0(menu, sel, entry->u_b6.text, entry->u136.c.field_138, 0);
    FUN_004c1a40();
}

// The same body with the group scan left out and the entry lookup in front.
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

static inline int FindHelp(Entry_004a9fd0* entries)
{
    for (int i = 1; i < entries->u_b6.anim.count + 1; i++) {
        if (strncmp(entries[i].name2, DAT_005098c4, 0x10) == 0)
            return i;
    }
    return -1;
}

// FUNCTION: 0x4a9fd0
int __stdcall FUN_004a9fd0(Menu_004a9fd0* menu)
{
    int i;
    Entry_004a9fd0* entries = 0;

    if (menu->layer != 0) {
    entries = menu->layer->entries;

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
    if (menu->layer != 0) {
        if (menu->field_cca == 1) {
            menu->field_cca = 0;
            FUN_004a81e0(menu, menu->layer->flags | 0x40);
        }

        if (entries != 0) {

        Point_004a9fd0& pt = menu->point;
        int x = entries->x;
        int y = entries->y;
        if (entries->type != 0) {
            x *= 2;
            y *= 2;
        }
        int right = entries->w + x - 1;
        int bottom = entries->h + y - 1;
        FUN_004ab440(menu, pt.x >= x && pt.x <= right &&
                           pt.y >= y && pt.y <= bottom);

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
            Entry_004a9fd0* e = &entries[i];
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

                int k = FUN_004c1b80(0xfb) == 0 ? key : 0;
                switch (e->type) {
                case 1:
                    if (FUN_004a6ae0(menu, i, key) == 1)
                        sel = i;
                    if (e->u1f.timer != 0 && elapsed) {
                        e->u1f.timer -= 2;
                        if (e->u1f.timer < 0)
                            e->u1f.timer = 0;
                        goto setdirty;
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
                        if (found == sel) {
                            sel = i;
                        } else {
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
                            } else if (me->field_29 == 0 ||
                                       (me->type == 4 && me->field_157 != 0)) {
                                sel = -1;
                            } else {
                                menu->focus = -1;
                                menu->layer->current = found;
                                int current = menu->layer->current;
                                if (entries[current].type == 3) {
                                    Entry_004a9fd0* activeEntries = menu->layer->entries;
                                    me = &activeEntries[current];
                                    FUN_004c13a0(menu->palette[me->u1f.colourIndex],
                                                 FUN_004c13f0());
                                    FUN_004a1810(activeEntries, current);
                                    FUN_0049fc50(menu, current);
                                    menu->layer->current = current;
                                    FUN_004ab6c0(menu, current, me->u_b6.text,
                                                 me->u136.c.field_138, 0);
                                    FUN_004c1a40();
                                }
                            }
                        }
                    }
                    break;
                case 6:
                    if (FUN_004a4b50(menu, i) == 1)
                        sel = i;
                    break;
                case 13:
                    {
                        Entry_004a9fd0* me = &menu->layer->entries[i];
                        if (me->u_b6.anim.field_ce != 0 && me->u_b6.anim.field_ba < me->u_b6.anim.field_be) {
                            if (FUN_004b6340() > me->u_b6.anim.field_c6) {
                                me->u_b6.anim.field_ba += (int)me->u_b6.anim.field_ca;
                                if (me->u_b6.anim.field_ba > me->u_b6.anim.field_be) {
                                    me->u_b6.anim.field_ba = me->u_b6.anim.field_be;
                                    me->u_b6.anim.field_ce = 0;
                                }
                                me->u_b6.anim.field_c6 = FUN_004b6340() + me->u_b6.anim.field_c2;
                            }
                            FUN_004a4660(menu, i);
                        }
                    }
                    break;
                case 12:
                    if (e->u1f.timer != 0 && elapsed) {
                        e->u1f.timer--;
                        FUN_004a5e50(menu, i);
                    setdirty:
                        if (menu->layer != 0)
                            menu->layer->dirty = 1;
                    }
                    break;
                }
            }
            if (sel != -1)
                break;
        }

        if (menu->field_68 != saved) {
            char* ptr = DAT_005119b8;
            if (menu->field_68 != -1) {
                menu->field_6c = menu->field_68;
                ptr = (char*)&menu->layer->entries[menu->field_68] + 0x33;
            }
            Entry_004a9fd0* ents = menu->layer->entries;
            int found = FindHelp(ents);
            if (found != -1) {
                char* text = FUN_004c5740(ptr);
                strcpy((char*)&menu->layer->entries[found] + 0xb6, text);
                menu->field_cca = 1;
            }
        }

        if (menu->layer->cb1c != 0)
            menu->layer->cb1c();

        if (sel != -1) {
            menu->field_60 = sel;
            menu->focus = -1;
            menu->layer->current = sel;
            if (menu->layer->entries[menu->layer->current].type == 3)
                SelectCurrent(menu, menu->layer->entries, menu->layer->current);
            if (menu->layer->cb8 != 0)
                menu->layer->cb8(menu);
            if (menu->field_60 != -1) {
                if (menu->layer != 0) {
                    int flags = menu->layer->flags;
                    menu->focus = -1;
                    menu->field_60 = -1;
                    menu->field_68 = -1;
                    if (menu->layer->cb8 != 0)
                        menu->layer->cb8(menu);
                    FUN_004c2470();
                    FUN_004a81e0(menu, 2);
                    FUN_004c2870();
                    Layer_004a9fd0* old = menu->layer;
                    menu->layer = old->prev;
                    if (menu->layer != 0)
                        menu->layer->dirty = 1;
                    FUN_004d85a0(old);
                    if ((flags & 0x800) != 0)
                        FUN_004a81e0(menu, 0x40);
                }
            }
        }
    }
    }
    }
    return 1;
}