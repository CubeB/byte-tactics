// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1, finished by space-bunny-free, finished by GPT-6.1-sol, finished by deepseek-v4.1-flash, edited by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by claude-sonnet-5-5. Names are provisional.
//
// Partial: 53.1%, 2180 bytes versus 2164 (was 46.0% / 2204).
//
// claude-sonnet-5-5 session, structural fixes that raised 46.0 to 53.1:
//   * case 12 is written textually LAST in the switch (block layout), and case 1
//     jumps into it (`goto setdirty;`) because the original shares the
//     `if (layer) layer->dirty = 1` tail between cases 1 and 12;
//   * the post-loop code never uses the cached `entries` local: the original
//     re-reads menu->layer->entries (help-text search, strcpy, field_68 pointer,
//     and the final sel block each get a fresh load);
//   * the "select current" tails (case 5 and the final sel block) read
//     `int current = menu->layer->current;` back after storing it, test
//     `layer->entries[current].type == 3` and then load `ents` again;
//   * case 13 and the loop body use plain `entries[i]` / `menu->layer->entries[i]`
//     (the compiler derives the p / bias strength reduction itself);
//   * case 5's name search has no `found == count + 1` fix-up (it was dead code
//     that cost 20 bytes): `found` is -1 unless the strncmp matched.
//
// Still differs (all register allocation / scheduling, nothing structural):
//   1. Prologue: the original saves ebx, ebp, esi, edi before the early
//      `layer == 0` return (no shrink-wrap); ours pushes only ebp first.
//   2. Entry loop: the original keeps i in EBX and the strength-reduced entry
//      pointer in EDI (spill slots [esp+0x10]/[esp+0x14]) and reloads `entries`
//      from [esp+0x18]; ours keeps `entries` in EBX up to the loop head so i and
//      the pointer stay in memory with ESI/EDI/EBX used as temps.
//   3. First entry clamp: the original loads point.y into EDI early and spills
//      right/bottom to [esp+0x34]/[esp+0x38].
//   4. The `layer->entries[current].type` tests: the original loads the entries
//      pointer before the focus/current stores (EDX) and again for the body
//      (EBX); ours merges the two loads.
// Measured with no effect on any of these (do not repeat): declaration order of
// i/entries/sel/key at function top (24 orders, all 51.9), `register int i`,
// an alias `first` for the first-section pointer (copy-propagated, identical
// bytes), moving the first clamp into an inline helper (identical bytes),
// dropping the `e` pointer in favour of `entries[i].` (same size), and a 15
// minute tools/permute.py run (53.1 -> 53.1). Earlier sessions also measured
// `int i` inside the for header (-1.6), fresh-read spellings of the entry-loop
// bound, `int result` single-exit shape (44.1), `short y` and an address-taken
// entries slot (40.4).

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

// FUNCTION: 0x4a9fd0
int __stdcall FUN_004a9fd0(Menu_004a9fd0* menu)
{
    int i = 1;

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
    if (menu->layer != 0) {
        if (menu->field_cca == 1) {
            menu->field_cca = 0;
            FUN_004a81e0(menu, menu->layer->flags | 0x40);
        }

        Entry_004a9fd0* entries = menu->layer->entries;
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
                        int found = -1;
                        int j;
                        for (j = 1; j < entries->u_b6.anim.count + 1; j++) {
                            if (strncmp(entries[j].name2, e->u136.name, 0x10) == 0) {
                                found = j;
                                break;
                            }
                        }
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
                            } else if (me->field_29 == 0) {
                                sel = -1;
                            } else if (me->type == 4 && me->field_157 != 0) {
                                sel = -1;
                            } else {
                                menu->focus = -1;
                                menu->layer->current = found;
                                int current = menu->layer->current;
                                if (menu->layer->entries[current].type == 3) {
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
            char* ptr;
            if (menu->field_68 == -1) {
                ptr = DAT_005119b8;
            } else {
                menu->field_6c = menu->field_68;
                ptr = (char*)&menu->layer->entries[menu->field_68] + 0x33;
            }
            Entry_004a9fd0* ents = menu->layer->entries;
            int found = 1;
            for (; found < ents->u_b6.anim.count + 1; found++) {
                if (strncmp((char*)ents + found * 0x15b + 2, DAT_005098c4, 0x10) == 0)
                    break;
            }
            if (found == ents->u_b6.anim.count + 1)
                found = -1;
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
            int current = menu->layer->current;
            if (menu->layer->entries[current].type == 3) {
                Entry_004a9fd0* ents = menu->layer->entries;
                Entry_004a9fd0* me = &ents[current];
                FUN_004c13a0(menu->palette[me->u1f.colourIndex], FUN_004c13f0());
                int grp = 0;
                int t;
                for (t = 1; t < ents->u_b6.anim.count + 1; t++) {
                    if (ents[t].type == 7) {
                        if (grp == me->group) {
                            FUN_004c1420(ents[t].language);
                            break;
                        }
                        grp++;
                    }
                }
                if (t == ents->u_b6.anim.count + 1)
                    FUN_004c1420(DAT_0051fba4->group);
                FUN_0049fc50(menu, current);
                menu->layer->current = current;
                FUN_004ab6c0(menu, current, me->u_b6.text, me->u136.c.field_138, 0);
                FUN_004c1a40();
            }
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
    return 1;
}