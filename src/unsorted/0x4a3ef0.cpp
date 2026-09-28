// Decompiled by space-bunny-free. Names are provisional.
// PARTIAL, 652 bytes against 629, everything below the entry search matches by
// construction but nothing is byte identical yet. The gap is register
// allocation only, and this is what is known about it.
//
// What the function is: the list gadget's scroll-up step, the mirror image of
// 0x4a99c0 (scroll down). It finds the entry of type 2 whose +0x01 byte equals
// this entry's, then, according to which of the flag bits 0x10, 0x20 and 0x80
// that entry carries, rescales the scroll window (flag 0x10), recomputes the
// pixel size of a line from the glyph table (0x20) or from two window fields
// (0x80), and finally refreshes the gadget with FUN_004a2580. All three arms
// store +0x142 (the new line size) and +0x136 (the new scroll position).
//
// Facts worth keeping:
// - The array index is param_2, not param_1. At 0x4a3f00 only three pushes have
//   happened, so [esp+0x1c] is the second argument; at 0x4a414c all four have,
//   and the tail pushes [esp+0x20] then [esp+0x1c], that is
//   FUN_004a2580(param_1, param_2). Both callers push the object first.
// - The entry stride is 0x15b and the entry fields used here are +0x00 type
//   byte, +0x01 the byte the search matches, +0x17, +0x19, +0x1b (read as a
//   dword here, and as a byte for bit 0 in the other two arms), +0x28 group,
//   +0xb6 count (entry 0 only), +0xc0, +0xc6, +0xd6 id, +0xda, +0x136, +0x142.
// - MSVC 5 spells `x == 0` as a compare against a zeroed register here, twice:
//   `xor ecx,ecx; cmp eax,ecx` at 0x4a3f4e and `cmp dx,cx` at 0x4a40fb (ecx is
//   still the zero from 0x4a3f4e). Keep those as `== 0`, not `!x`.
// - The type 7 loop at 0x4a3f92 is exactly the shape that matched in 0x4a99c0
//   and 0x4a30c0: counter spilled to [esp+0x10], cursor in eax, group reloaded
//   inside the loop, `if (i == count + 1) FUN_004c1420(current)` after it.
// - The float block stores an int temp at [esp+0x10], divides by the int at
//   [esp+0x14], then overwrites [esp+0x14] with `me->field_19 - 3` and
//   multiplies, so the source is one double expression, not three statements.
// - The tail at 0x4a4143/0x4a4145 is shared by all three arms:
//   `field_136 = field_19 - <size in eax>`; the 0x10 arm reaches 0x4a4145 with
//   `field_19 - field_142 - 3` already subtracted, so it must be spelled
//   `me->field_19 - me->field_142 - 3` and not a parenthesised sum.
//
// What still differs: the original keeps `entries` in ebp, `me` in esi, the
// found entry in ebx, the loop index in eax, the loop cursor in edi, count+1 in
// edx, and loads `me->kind` into cl before the first loop, and it re-reads both
// parameters from their stack slots instead of ever copying a parameter into a
// register. Its live set inside the first loop is therefore six values in seven
// registers. Every spelling tried here gives seven live values, because MSVC
// copies param_2 into ebx for the final call, and one value too many costs two
// spills: `entries` is spilled to [esp+0x10] and reloaded inside the first loop
// (because the byte compare has to use dl), and the type 7 loop spills and
// reloads its cursor around the counter increment.
// Shapes tried, all worse or equal: `found` as a separate variable set from a
// separate index; the index itself as the answer with an
// `if (found == count + 1) found = 0;` fixup (this adds a compare and a second
// `add edi, 0x15b`); a `while` loop with the cursor walked by hand; an
// uninitialised `found`; an explicit cursor versus indexing `entries[i]`; both
// orders of the two comparisons in the search condition; `int`/`char`/`unsigned
// char` for the kind local; the loop index declared before or after `found`.
// A sweep of N unused `extern int dummyN;` declarations in front of this source,
// N = 0 to 320 in steps of 16, changed neither the code nor the score, so the
// compiler state is not the lever here and the source shape is.
//
// Suspected original bug: in the 0x20 arm the divisor is left as the zero that
// `xor ecx,ecx` at 0x4a3f4e put in ecx when `e->field_c0 <= 0`
// (0x4a40b2 jumps over the setup), and 0x4a40d1 divides by it. The 0x80 arm
// guards its divisors with `test`, this arm does not.

#pragma pack(push, 1)
struct Entry_004a3ef0 {                // 0x15b bytes
    unsigned char type;                // +0x00
    unsigned char kind;                // +0x01
    char unknown_02[0x17 - 0x02];
    short field_17;                    // +0x17
    short field_19;                    // +0x19
    int field_1b;                      // +0x1b (read as a dword here)
    char unknown_1f[0x28 - 0x1f];
    char group;                        // +0x28
    char unknown_29[0xb6 - 0x29];
    short count;                       // +0xb6 (only meaningful in entry 0)
    char unknown_b8[0xc0 - 0xb8];
    short field_c0;                    // +0xc0
    char unknown_c2[0xc6 - 0xc2];
    int* field_c6;                     // +0xc6
    char unknown_ca[0xd6 - 0xca];
    int id;                            // +0xd6
    short field_da;                    // +0xda
    char unknown_dc[0x136 - 0xdc];
    short field_136;                   // +0x136
    char unknown_138[0x140 - 0x138];
    short field_140;                   // +0x140
    short field_142;                   // +0x142
    char unknown_144[0x15b - 0x144];
};
#pragma pack(pop)

struct List_004a3ef0 {
    char unknown_0[0x0c];
    unsigned short* field_0c;          // +0x0c
};

struct Holder_004a3ef0 {
    int current;                       // +0x00
    Entry_004a3ef0* entries;           // +0x04
    char unknown_08[0x14 - 0x08];
    List_004a3ef0* list;               // +0x14
};

struct Class_004a3ef0 {
    char unknown_0[0x18];
    Holder_004a3ef0* holder;           // +0x18
};

extern Holder_004a3ef0* DAT_0051fba4;

void __stdcall FUN_004c1420(int id);
int __stdcall FUN_004b7f30(unsigned short* param_1, int param_2);
int FUN_004c1450();
void __stdcall FUN_004a2580(Class_004a3ef0* param_1, int param_2);

// FUNCTION: 0x4a3ef0
void __stdcall FUN_004a3ef0(Class_004a3ef0* param_1, int param_2)
{
    Entry_004a3ef0* entries = param_1->holder->entries;
    Entry_004a3ef0* me = &entries[param_2];
    unsigned char kind = me->kind;
    Entry_004a3ef0* entry = entries + 1;
    int i;
    int found = 0;
    for (i = 1; i < entries->count + 1; i++, entry++) {
        if (entry->type == 2 && entry->kind == kind) {
            found = i;
            break;
        }
    }
    if (found != 0) {
        Entry_004a3ef0* e = &entries[found];
        if (e->type == 2) {
            if (e->field_1b & 0x10) {
                int i = 1;
                int n = 0;
                for (; i < entries->count + 1; i++) {
                    if (entries[i].type == 7) {
                        if (n == e->group) {
                            FUN_004c1420(entries[i].id);
                            break;
                        }
                        n++;
                    }
                }
                if (i == entries->count + 1) {
                    FUN_004c1420(DAT_0051fba4->current);
                }
                int size = (DAT_0051fba4->list == 0) ? FUN_004c1450()
                    : (*(unsigned short*)(FUN_004b7f30(DAT_0051fba4->list->field_0c, 0x49) + 2) + 2);
                int span = (size + 1 > e->field_da) ? size + 1 : e->field_da;
                int step = (e->field_19 - 2) / span;
                int last = e->field_c0;
                int rows = (int)((double)step / (double)last * (me->field_19 - 3));
                me->field_142 = rows;
                if (me->field_142 < 10) {
                    me->field_142 = 10;
                }
                if (last > step) {
                    me->field_136 = me->field_19 - me->field_142 - 3;
                } else {
                    me->field_136 = 0;
                }
            } else if (e->field_1b & 0x20) {
                int lines = 0;
                if (e->field_c0 > 0) {
                    int a = *(int*)e->field_c6;
                    int b = *(int*)(a + 0x28);
                    lines = *(unsigned short*)(b + 2) * e->field_c0;
                }
                int s = e->field_19 * me->field_19 / lines;
                me->field_142 = s;
                if (*(unsigned char*)((char*)me + 0x1b) & 1) {
                    me->field_136 = me->field_17 - s;
                } else {
                    me->field_136 = me->field_19 - s;
                }
            } else if (e->field_1b & 0x80) {
                if (e->field_da != 0 && e->field_c0 != 0) {
                    int s = e->field_19 / e->field_da * me->field_19 / e->field_c0;
                    me->field_142 = s;
                    if (*(unsigned char*)((char*)me + 0x1b) & 1) {
                        me->field_136 = me->field_17 - s;
                    } else {
                        me->field_136 = me->field_19 - s;
                    }
                }
            }
        }
    }
    FUN_004a2580(param_1, param_2);
}
