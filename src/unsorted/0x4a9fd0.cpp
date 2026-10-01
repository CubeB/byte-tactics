// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1, finished by space-bunny-free, finished by GPT-6.1-sol, finished by deepseek-v4.1-flash. Names are provisional.
//
// Partial: 46.0%, 2204 bytes versus 2164. Best so far; every earlier attempt is
// in build/scratch/0x4a9fd0/.
//
// deepseek-v4.1-flash retry (#3478, 10-minute timebox): +1.3 to 46.0 with two
// changes in the first entry clamp, both now faithful to the original:
//   * `Point_004a9fd0& pt = menu->point;` instead of direct menu->point reads.
//     Alone it is byte-neutral at 44.7 but makes the compiler emit the
//     original's `lea esi,[ebp+0x3c]` early and `mov eax,[esi+4]` for point.y.
//   * with that in place, the faithful `int y = entries->y;` beats the
//     `short y` metric artifact it had replaced (46.0 versus 44.7; the
//     artifact's `mov cx / movsx ecx,cx` pair no longer buys alignment).
// Still measured worse, do not repeat: removing the redundant
// `if (menu->layer != 0)` wrapper is now 44.7 (2196 bytes), so the extra
// `test eax,eax / je` and `mov ebx,eax` it introduces still net-align; a
// named `int py = pt.y;` is 45.3 (2212 bytes), so the early
// `mov edi,[esi+4]` placement is scheduler output, not reachable by naming
// the read. The remaining clamp diff (right in edi instead of [esp+0x34],
// bottom at [esp+0x20] instead of [esp+0x38]) is still the entries-in-ebx
// allocation state described below.
//
// Retry by deepseek-v4.1-flash (#3905, 10-minute timebox): baseline re-verified
// at 46.0% (2204 vs 2164 bytes), no variant beaten. Diff re-read hunk by hunk:
// every hunk past the prologue is downstream of the same two allocator facts
// (entries memory resident in the original, i in ebx / p in edi, ours reversed),
// including the entry-clamp edi-vs-[esp+0x34] and [esp+0x20]-vs-[esp+0x38]
// edge homes and the 0x4aa1a2 `mov ebx,1; mov [esp+0x10],ebx` pair that ours
// hoists to a top-of-function `mov [esp+4],1`. Nothing new to try within the box.
//
// deepseek-v4.1-flash session: the only change from the previous best is
// `short y = entries->y;` in the first clamp (was `int y`), worth +1.0 in the
// checker (43.7 to 44.7). It likely is NOT the original shape: the original
// sign-extends the field once, `movsx ecx, word ptr [ebx+0x15]`, with no
// separate word load, whereas a `short y` local adds `mov cx,[ebx+0x15]` and
// `movsx ecx,cx`. Treat the gain as a metric artifact until the prologue and
// the entries/i/ebx rotation below are fixed. The faithful spelling is `int y`
// at 43.7%; that is what the notes below describe. Other shapes measured this
// session, none better: `short x`, `unsigned short`, `(short)` casts, swapping
// the point.x/point.y subtraction order, a `lay` local, a count local `n`, a
// reference `Entry*& entries`, `while` loop, i declared after `sel` or right
// before the loop, and a 0..400 dummy-extern compiler-state sweep (all 43.7 or
// lower). headers.py found no header set above 43.7%.
//
// What still differs, in order of how much it is worth:
//
// 1. The prologue, and it is still one allocation state, not six bugs. The
//    original does sub esp,0x34 / push ebx / push ebp / mov ebp,[esp+0x40] /
//    push esi / push edi and its early "layer == 0" exit pops all four. Ours
//    pushes only ebp at the top (to home the parameter) and sinks push edi /
//    push esi / push ebx below that early return, so every address in the body
//    is 4 bytes low. MSVC5 shrink-wraps the saves whenever ebx, esi and edi
//    are all dead in the entry block, and they are, in every spelling tried:
//    `if (layer == 0) return 0;` first, last, inverted, and with locals
//    declared before it. Note the original's order, push ebx BEFORE push ebp:
//    ebp is saved second because it is being used to home the stack parameter,
//    so this is the shape of a function whose register allocator gave ebp the
//    parameter and ebx, esi, edi to body variables.
// 2. Register roles in the entry loop. The original keeps the induction
//    variable i in EBX and the walk pointer p in EDI (each with a spill slot,
//    [esp+0x10] and [esp+0x14], reloaded after any call at 0x4aa541), and it
//    keeps `entries` MEMORY RESIDENT, reloading it from [esp+0x18] every
//    iteration. Ours keeps `entries` in EBX for the whole loop, so i is memory
//    resident and p lands in ESI. That single difference also explains the
//    inner clamp: with `entries` gone from a register the original can keep
//    both derived edges live (right in EDX, bottom in ESI, 0x4aa1fa..0x4aa214)
//    whereas ours spills one of them and reloads point.y from [ebp+0x40]. The
//    lever is still "get `entries` out of EBX", not the clamp spelling.
// 3. Slot assignment. The original uses [esp+0x10] for i and [esp+0x48] for
//    sel. [esp+0x48] is not a frame local at all: post-prologue esp is
//    esp0-0x44, so [esp+0x44] is the return address and [esp+0x48] is the
//    stack argument slot, which MSVC reuses for a scratch spill once the
//    parameter has been homed into ebp. Ours puts sel at [esp+0x10] and i at
//    [esp+0x48], so exactly one variable is in the argument slot in both, but
//    it is the wrong one. Every other slot already agrees (p 0x14, entries
//    0x18, key 0x1c, elapsed 0x20, bias 0x24, saved 0x28, point 0x2c/0x30).
// 4. The first entry clamp. The original spills both derived edges, right to
//    [esp+0x34] and bottom to [esp+0x38], and loads point.y into EDI before
//    building them (0x4aa0fd); ours keeps right in EDI and reloads point.y
//    from [ebp+0x40] after the call argument is built. This is (2) again.
// 5. The 14-byte text shift. The original builds the address with
//    `lea ecx,[eax+edx]`, keeping the layer base in EDX and the index in EAX.
//    Spelling it as `menu->layer->text[n] = menu->layer->text[n+1]` changes
//    nothing; giving the base its own `char*` local makes MSVC strength-reduce
//    the whole loop into a pointer walk and is much worse.
// 6. Case 5 (type 5) matches the original's shape: sel is set to -1 before the
//    name search, so the "not found" test compares against the same
//    materialised -1 the three deselect arms use. Writing the constant out as
//    a separate `int notfound = -1` scores identically, so the spelling is
//    not pinned down.
//
// This session (space-bunny-free) moved 42.2 -> 43.7 with two source changes,
// both about WHERE a variable's live range starts:
//   * hoisting `int i = 1;` out of the `for` header and declaring it next to
//     `int sel`, so i is a real local and not a front-end loop temp: +1.0;
//   * moving that same `int i = 1;` ABOVE the early `if (menu->layer == 0)
//     return 0;`, so its live range starts at function entry: +0.5. MSVC5
//     still shrink-wraps, but the shape changes favourably.
//
// Things that did NOT work, so nobody repeats them:
//   * Giving the text shift a `char*` base local (strength-reduces the loop).
//   * Hoisting a `Layer* lay = menu->layer;` local above the early return to
//     try to pin the prologue: score neutral at 43.2.
//   * Declaring `int i;` above the early return but keeping `for (i = 1; ...)`:
//     42.2, the gain really does need the initialiser there.
//   * Moving `int sel = menu->field_60;` above the early return too (both
//     variables live from entry): 42.7, worse, and it drops 4 bytes.
//   * An `int elapsed = 0` pre-initialiser (the original assigns 0 only in the
//     else arm, and the extra store is a real byte); naming pt->x and pt->y as
//     locals before the first clamp; two 768-set header sweeps (earlier
//     sessions); removing the `saved` copy of menu->field_68.
//   * This retry: moving an uninitialized `sel` declaration above the early
//     return tied at 43.7%; taking the address of `entries` and using that
//     slot to form the current entry fell to 40.4%.
//
// deepseek-v4.1-flash retry #3535 (10-minute timebox): nothing beat 46.0, so
// here is a measured divergence map for the next worker. Line up tools/ctx.py's
// call order with check.py's `?  +0xNNN` reference list; the numbers below are
// original offset minus ours (original offset minus 0x4a9fd0):
//   +6 at the very first call, down to +1 by FUN_004ab440, so the prologue and
//   the early-return region are the only excess up there;
//   +1 at case 1, then +11/+19/+27/+31 at the FUN_004a3780, FUN_004a7290,
//   FUN_004a4170 and FUN_004a4440 calls, so each of those small case bodies
//   grows by about 8 bytes in ours;
//   +41 at case 5's strncmp and +68 at FUN_004a5f40, so the case 5 block
//   carries the single largest excess (then -16 back at FUN_004c13f0, which is
//   the `sel = found` path re-joining);
//   +34 at case 6's FUN_004a4b50 but +100 at case 13's FUN_004b6340, so the
//   case 12/13 tail adds about 66 more.
// That is where the 40 extra bytes are; the early loop and the first clamp are
// already within a few bytes. Measured this session, all 46.0 and byte neutral,
// so do not repeat them: an `entries` local declared at function entry and
// assigned at the load (alone, and combined with a top `sel`), and hoisting the
// `right`/`bottom` declarations out of the first clamp.

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

        int bias = 0xffffffe1 - (int)entries;
        unsigned char* p = (unsigned char*)entries + 0x17a;
        for (; i < entries->u_b6.anim.count + 1; i++) {
            Entry_004a9fd0* e = (Entry_004a9fd0*)(p - 0x1f);
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
                        int found = -1;
                        int j;
                        for (j = 1; j < entries->u_b6.anim.count + 1; j++) {
                            if (strncmp(entries[j].name2, e->u136.name, 0x10) == 0) {
                                found = j;
                                break;
                            }
                        }
                        if (found == entries->u_b6.anim.count + 1)
                            found = -1;
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
                                Entry_004a9fd0* activeEntries = menu->layer->entries;
                                int current = menu->layer->current;
                                me = &activeEntries[current];
                                if (me->type == 3) {
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
                case 12:
                    if (e->u1f.timer != 0 && elapsed) {
                        e->u1f.timer--;
                        FUN_004a5e50(menu, i);
                        if (menu->layer != 0)
                            menu->layer->dirty = 1;
                    }
                    break;
                case 13:
                    {
                        Entry_004a9fd0* me = (Entry_004a9fd0*)((char*)entries + bias + (int)(char*)p);
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
                }
            }
            if (sel != -1)
                break;
            p += 0x15b;
        }

        if (menu->field_68 != saved) {
            char* ptr;
            if (menu->field_68 == -1) {
                ptr = DAT_005119b8;
            } else {
                menu->field_6c = menu->field_68;
                ptr = (char*)&entries[menu->field_68] + 0x33;
            }
            int found = 1;
            for (; found < entries->u_b6.anim.count + 1; found++) {
                if (strncmp((char*)entries + found * 0x15b + 2, DAT_005098c4, 0x10) == 0)
                    break;
            }
            if (found == entries->u_b6.anim.count + 1)
                found = -1;
            if (found != -1) {
                char* text = FUN_004c5740(ptr);
                strcpy((char*)&entries[found] + 0xb6, text);
                menu->field_cca = 1;
            }
        }

        if (menu->layer->cb1c != 0)
            menu->layer->cb1c();

        if (sel != -1) {
            menu->field_60 = sel;
            menu->focus = -1;
            menu->layer->current = sel;
            Entry_004a9fd0* me = &entries[sel];
            if (me->type == 3) {
                FUN_004c13a0(menu->palette[me->u1f.colourIndex], FUN_004c13f0());
                int grp = 0;
                int t;
                for (t = 1; t < entries->u_b6.anim.count + 1; t++) {
                    if (entries[t].type == 7) {
                        if (grp == me->group) {
                            FUN_004c1420(entries[t].language);
                            break;
                        }
                        grp++;
                    }
                }
                if (t == entries->u_b6.anim.count + 1)
                    FUN_004c1420(DAT_0051fba4->group);
                FUN_0049fc50(menu, sel);
                menu->layer->current = sel;
                FUN_004ab6c0(menu, sel, me->u_b6.text, me->u136.c.field_138, 0);
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
