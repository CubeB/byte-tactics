// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// PARTIAL, 63.3% (629 bytes, exact size), up from 56.0%. Two changes from the
// previous version: the 0x20 arm reads e->field_c0 into a 32-bit local before
// testing it (the original is `movsx eax, word [ebx+0xc0]; test eax,eax`, a
// 32-bit test, while a direct `e->field_c0 > 0` gives a 16-bit `test ax,ax`),
// and the two compares that the original makes against the zero register use
// `lines` instead of the literal 0 (`if (found != lines)`,
// `if (e->field_da != lines)`) even though the front end still folds both to a
// literal test. Those two together are worth about 7 points; on their own the
// c0 local is worth 3 and the `!= lines` compares lose 1.4.
// The list gadget's scroll-up step, the mirror image of 0x4a99c0: find the entry
// of type 2 whose +0x01 byte equals this entry's, then, by the flag bits 0x10,
// 0x20 and 0x80 of that entry, recompute the size of a line (+0x142) and the
// scroll position (+0x136), and refresh the gadget with FUN_004a2580.
// The one allocator state that is left, and it explains every diff in the file:
// the original has esi=me, edi=the group loop counter, ebx=the found entry,
// ebp=entries and the CONSTANT ZERO in ecx. Here it is esi=me, edi=the found
// entry, ebx=the loop counter, ebp=the constant zero and entries in ecx, which
// is then spilled to [esp+0x14]. So exactly ONE variable too many is holding a
// callee-saved register: if the zero would stop needing one, `i` moves to edi,
// `e` to ebx, `entries` to ebp and the zero to ecx, all five at once, which is
// the whole diff. The zero gets a callee-saved register because MSVC treats
// `lines` as a variable with a live range covering the whole function; the
// original instead value-numbers it as the constant 0 and rematerialises it
// (that is why the 0x20 arm can divide by whatever happens to be in ecx).
// Written tries that did NOT produce that, all free scratch scores:
// - `if (found != lines)` and `if (e->field_da != lines)`: the front end folds
//   the compare to the literal 0, so it still emits `xor ebp,ebp; test eax,eax`
//   (56.0% and 54.6%, 631 bytes).
// - One variable for the group counter AND the 0x20 divisor: MSVC then puts the
//   counter in memory as a literal 0, spills `entries`, and duplicates the whole
//   FUN_004a2580 tail into both arms of the 0x10 arm (19.7%, 706 bytes).
// - The divisor as a local of the 0x20 arm: 41.1%, 561 bytes.
// - Inlining the span ternary into the step expression (as `(field_da > size+1)
//   ? field_da : size+1`, the operand order the original's `cmp edx,edi; jle`
//   needs), inlining the two loads of the 0x20 arm, the size ternary as an
//   if/else, `unsigned int lines`, and a `holder` local: byte-identical to the
//   version above, so none of them is a lever on their own.
// What the retry found (each one is worth a lot, check them before anything else):
// - The entry search is an INLINE FUNCTION with `return i` inside the loop and
//   `return 0` after it (the original has `xor eax,eax` on the not-found path and
//   a join, no compare). A `found = i; break;` loop gives a different shape.
// - The float block is float arithmetic: `(float)step / last * (me->field_19 - 3)`
//   gives `fild/fidiv/fimul`; with `(double)` casts MSVC emits `fild/fmulp`.
// - The 0x10 arm's tail is `if (last <= step) field_136 = 0; else field_136 =
//   me->field_19 - me->field_142 - 3;` (the false arm falls through, `jg` to the
//   true one), and the arms share the tail `sub edi,eax; mov [esi+0x136],di`.
// - A zero-initialised local declared right AFTER the search call and assigned
//   later in the 0x20 arm (`int lines = 0;`) is what makes MSVC keep a zero in a
//   register for the whole function: `xor ecx,ecx; cmp eax,ecx`, `n = 0` stored
//   from it, `cmp dx,cx` in the 0x80 arm and `lines` living in ecx in the 0x20
//   arm. Declared before the search, or inside the arm, it does not happen.
// What still differs: the original keeps `entries` in ebp, `me` in esi, the found
// entry in ebx and the zero in ecx (sharing it with the kind byte cl before it).
// Here the zero takes ebp and `entries` lives in ecx and is spilled to
// [esp+0x14]. Declaring `lines` as `char` instead gives the original's allocation
// for the first 40 instructions (77.2%, 633 bytes) but changes what the code
// computes (the divisor is truncated to a byte), so it is not used. Moving the
// declarations of every other local (about 600 random placements), the type of
// `lines`, a `zero` local used for the compares, and `n` at function scope did
// not help. The original also loads param_1 before `sub esp,8` and re-reads both
// parameters from their stack slots; ours loads param_2 into edx early.
//
// Suspected original bug: in the 0x20 arm the divisor is left as the zero that
// the zero register holds when `e->field_c0 <= 0` (the jle at 0x4a40b2 jumps
// over the setup), and 0x4a40d1 divides by it. The 0x80 arm guards its divisors
// with `test`, this arm does not.

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

static inline int Find_004a3ef0(Entry_004a3ef0* entries, unsigned char kind)
{
    for (int i = 1; i < entries->count + 1; i++) {
        if (entries[i].type == 2 && entries[i].kind == kind)
            return i;
    }
    return 0;
}

// FUNCTION: 0x4a3ef0
void __stdcall FUN_004a3ef0(Class_004a3ef0* param_1, int param_2)
{
    Entry_004a3ef0* entries = param_1->holder->entries;
    Entry_004a3ef0* me = &entries[param_2];
    unsigned char kind = me->kind;
    int found = Find_004a3ef0(entries, kind);
    int lines = 0;
    if (found != lines) {
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
                int rows = (int)((float)step / last * (me->field_19 - 3));
                me->field_142 = rows;
                if (me->field_142 < 10) {
                    me->field_142 = 10;
                }
                if (last <= step) {
                    me->field_136 = 0;
                } else {
                    me->field_136 = me->field_19 - me->field_142 - 3;
                }
            } else if (e->field_1b & 0x20) {
                int c0 = e->field_c0;
                if (c0 > 0) {
                    int a = *(int*)e->field_c6;
                    int b = *(int*)(a + 0x28);
                    lines = *(unsigned short*)(b + 2) * c0;
                }
                int s = e->field_19 * me->field_19 / lines;
                me->field_142 = s;
                if (*(unsigned char*)((char*)me + 0x1b) & 1) {
                    me->field_136 = me->field_17 - s;
                } else {
                    me->field_136 = me->field_19 - s;
                }
            } else if (e->field_1b & 0x80) {
                if (e->field_da != lines && e->field_c0 != 0) {
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
