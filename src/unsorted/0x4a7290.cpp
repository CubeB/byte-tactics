// Decompiled by space-bunny-free, finished by muse-spark-1.3-free, finished by deepseek-v4.1-flash. Names are provisional.
// Best so far 55.7% (753 of our bytes against 706; the 52.6% first session
// below described the 724-byte version).  All 24 relocated
// references (the 11 callees, DAT_0051fba4) land at the original's offsets and
// in the original's order, the frame is 0x2c with the same slot map
// ([esp+0x10] the group counter in the dead first argument slot, [esp+0x14] the
// rect, [esp+0x24] the 24-byte point copy) and the two inlined button bodies
// are instruction-for-instruction the original's.  What still differs is the
// top of the function, and it is one decision: the original loads argument 1
// into ebx and argument 2 into ebp BEFORE computing index * 0x15b, and the
// scaled index then coalesces into ebp, which leaves esi for `entries` and edi
// for the loop counter.  This file gets `index` into ebp too, but only after
// the pushes, so ebx is loaded second, the multiply runs in ecx before the
// `entries` load, and the loop counter ends up in esi with the group value in
// edi.  Fixing that needs the parameter to be allocated ebp before the
// `entries` local; the group search had to become a static inline helper with
// its own `index` parameter to get that far (see the guide's 0x4523e0 note:
// 50.9% to 51.9%), and nothing tried since moved it further.  This session: chained `rect.x0 = rect.y0 = 0` (right to left stores) 51.9% to 52.6%; GetEntries inline accessor, holder local plus idx copy, and the faithful break plus post-loop `if (i == count + 1)` all at or below 51.9% (the break form is 49 bytes bigger at 50.9%).  Tried and
// rejected, all at or below 51.9%: no `entries` local at all (MSVC then folds
// base+offset into one pointer, losing the original's two-register
// [esi+ebp+off] addressing, 35% on the free differ); `entries` declared after
// the rect; `int k = index * 0x15b` named explicitly, before or after
// `entries`, with a static inline accessor; a pointer induction variable in the
// group loop; `unsigned int index`; argument 3 as `int` or `void*`; and moving
// the `n`/`i`/`point`/`rel_x`/`rel_y` declarations around (declaration order
// changed nothing at all).  A second session added, all worse or equal:
// writing the group search directly in the body instead of as the inline
// helper (51.9%, byte-identical output, so the helper is now free either way);
// an explicit `p`/`end` pointer pair in the group loop, which drops the
// redundant counter the original keeps alongside its induction pointer
// (45.3%); the loop bound in a named `int count` local (38.3%); the compared
// group byte in a named `int g` local (38.0%, and it costs a reload of
// `entries` inside the loop); `char entryType` read into a local for the `if`
// at the top (51.9%, no effect); and the faithful-looking `break` plus a
// post-loop `if (i > entries->data.count)` (50.6%, 33 bytes bigger), even
// though the original's tail test `cmp edi,edx / jne` does compare the final
// `i` against count+1, so the `return` spelling that scores 51.9% is not what
// the original said.  Free harness in build/scratch/0x4a7290 (b.sh compiles a
// variant and free-scores it with no check.py run, d.sh shows the diff).
// Third session (deepseek-v4.1-flash): 52.6% to 55.7% by taking the group
// search out from behind the SelectGroup helper and writing it directly in the
// body with `int n = 0; int i;` declared at FUNCTION scope, the faithful
// `break` plus the post-loop `if (i == entries->data.count + 1)` (declaring
// n/i inside the block, or calling the helper, keeps the found path in an
// out-of-line tail and scores 50.9%).  headers.py tried 128 sets, all 52.6%,
// and the N-unused-externs sweep 0..400 in steps of 4 is flat at 52.6% (on the
// 52.6% source), so this is source shape, not compiler state.  Still differs:
// after the top the allocator gives `entries` ebp and spills the scaled index
// to [esp+0x40] (reloaded in both button bodies), the loop counter goes to esi
// and the raw index is kept in ecx; the original instead spills `entries` to
// [esp+0x10], keeps the scaled index in ebp, the counter in edi and reloads
// the raw index from its stack slot.  So `entries` outranks the scaled index
// where the original ranked it below; the swap is the one remaining cause.
// Tried and worse: helper with goto/return (52.6%), `Entry*&`/`int&` reference
// parameters (52.6%), a helper returning the found index (50.9%), a `bool` or
// `int found` flag (42 to 44%), a while form and declaration reorderings (all
// 50.9 to 55.7%).
#include <string.h>

#pragma pack(push, 1)

struct Entry_004a7290 {                // 0x15b bytes
    unsigned char type;                // +0x00
    char unknown_01[0x13 - 0x01];
    short x0;                          // +0x13
    short y0;                          // +0x15
    short x1;                          // +0x17
    short y1;                          // +0x19
    char unknown_1b[0x1f - 0x1b];
    int colourIndex;                   // +0x1f
    int field_23;                      // +0x23
    char unknown_27[0x28 - 0x27];
    char group;                        // +0x28
    char unknown_29[0xb6 - 0x29];
    union {
        short count;                   // +0xb6 (entry 0: number of entries)
        char text[0x82];               // +0xb6 (a text control)
        struct {
            char pad[0x20];
            int id;                    // +0xd6 (a list entry)
        } list;
    } data;
    short maxLength;                   // +0x138
    char unknown_13a[0x15b - 0x13a];
};

struct Holder_004a7290 {
    int unknown_00;
    Entry_004a7290* entries;           // +0x04
    char unknown_08[0x14 - 0x08];
    int field_14;                      // +0x14
    char unknown_18[0x20 - 0x18];
    int field_20;                      // +0x20
};

struct Point_004a7290 {
    int x;                             // +0x00
    int y;                             // +0x04
    int unknown_08[4];
};

struct Object_004a7290 {
    char unknown_00[0x18];
    Holder_004a7290* holder;           // +0x18
    char unknown_1c[0x3c - 0x1c];
    Point_004a7290 point;              // +0x3c
    char unknown_54[0x64 - 0x54];
    int focus;                         // +0x64
    int field_68;                      // +0x68
    char unknown_6c[0x8b2 - 0x6c];
    unsigned char colors[16];          // +0x8b2
};
#pragma pack(pop)

struct Out_4a15c0 {
    int x0;
    int y0;
    int x1;
    int y1;
};

struct Class_0051fba4 {
    int current;                       // +0x00
};

extern Class_0051fba4* DAT_0051fba4;

int FUN_004c13f0();
void __stdcall FUN_004c13a0(int colour, int font);
void __stdcall FUN_004c1420(int id);
void __stdcall FUN_004a15c0(char* entries, int index, Out_4a15c0* rect);
void __stdcall FUN_004a1810(Entry_004a7290* entries, int index);
int __stdcall FUN_004ab510(Object_004a7290* obj, unsigned char buttons);
void __stdcall FUN_004ab690(Object_004a7290* obj, int param_2);
void __stdcall FUN_004ab6c0(Object_004a7290* obj, int index, char* text,
                            int maxLength, int clear);
int __stdcall FUN_004ab720(Object_004a7290* obj, int index, char* text);
int __stdcall FUN_0049fc50(Object_004a7290* obj, int index);
void FUN_004c1a40();

// The group search is FUN_004a1810's body, the same one 0x4a1810.cpp holds,
// written out as a static inline helper: its `index` is then a variable of its
// own, which is what the original's register choice needs.
static inline void SelectGroup(Entry_004a7290* entries, int index)
{
    int n = 0;
    int i;
    for (i = 1; i < entries->data.count + 1; i++) {
        if (entries[i].type == 7) {
            if (n == entries[index].group) {
                FUN_004c1420(entries[i].data.list.id);
                return;
            }
            n++;
        }
    }
    FUN_004c1420(DAT_0051fba4->current);
}

// FUNCTION: 0x4a7290
int __stdcall FUN_004a7290(Object_004a7290* obj, int index, char* text)
{
    int n = 0;
    int i;
    Entry_004a7290* entries = obj->holder->entries;
    Out_4a15c0 rect;
    if (entries[index].type == 0) {
        rect.x0 = rect.y0 = 0;
    } else {
        rect.x0 = entries[index].x0;
        rect.y0 = entries[index].y0;
    }
    rect.x1 = entries[index].x1 + rect.x0 - 1;
    rect.y1 = entries[index].y1 + rect.y0 - 1;

    for (i = 1; i < entries->data.count + 1; i++) {
        if (entries[i].type == 7) {
            if (n == entries[index].group) {
                FUN_004c1420(entries[i].data.list.id);
                break;
            }
            n++;
        }
    }
    if (i == entries->data.count + 1)
        FUN_004c1420(DAT_0051fba4->current);

    Point_004a7290 point;
    memcpy(&point, &obj->point, 24);
    int rel_x = point.x - entries->x0;
    int rel_y = point.y - entries->y0;

    if (rel_x < rect.x0 || rel_x > rect.x1 || rel_y < rect.y0 || rel_y > rect.y1)
        goto check;

    obj->field_68 = index;
    if (FUN_004ab510(obj, 1)) {
        FUN_004a15c0((char*)obj->holder->entries, index, &rect);
        FUN_004c13a0(obj->colors[obj->holder->entries[index].colourIndex], FUN_004c13f0());
        FUN_004a1810(obj->holder->entries, index);
        FUN_0049fc50(obj, index);
        obj->holder->field_20 = index;
        FUN_004ab6c0(obj, index, obj->holder->entries[index].data.text,
                     obj->holder->entries[index].maxLength, 0);
        FUN_004c1a40();
        FUN_004ab690(obj, 1);
    } else if (FUN_004ab510(obj, 2)) {
        FUN_004a15c0((char*)obj->holder->entries, index, &rect);
        FUN_004c13a0(obj->colors[obj->holder->entries[index].colourIndex], FUN_004c13f0());
        FUN_004a1810(obj->holder->entries, index);
        FUN_0049fc50(obj, index);
        obj->holder->field_20 = index;
        FUN_004ab6c0(obj, index, obj->holder->entries[index].data.text,
                     obj->holder->entries[index].maxLength, 0);
        FUN_004c1a40();
        FUN_004ab690(obj, 2);
    }

check:
    if (obj->focus == index) {
        FUN_004c13a0(obj->colors[entries[index].colourIndex],
                     obj->colors[entries[index].field_23]);
        int r = FUN_004ab720(obj, index, text);
        if (r == 13) {
            obj->focus = -1;
            return 1;
        }
        if (r == 27) {
            obj->focus = -1;
            entries[index].data.text[0] = 0;
            return 1;
        }
        if (obj->holder)
            obj->holder->field_14 = 1;
    }
    return 0;
}
