// Decompiled by space-bunny-free, finished by GPT-6, finished by GPT-6.1-sol, edited by deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
// Retry (deepseek-v4.1-flash, issue 3781, 2 scored runs): still 93.1 pct / 629
// bytes. A named span local (int span = size + 1; then span > field_da ? span
// : field_da) restores the original jle polarity but scores the same; the
// field_da temp still lands in edi and the 0x20 arm still loads field_c6 after
// the test.
// deepseek-v4.1-flash retry (issue 3704, 10 min, 4 scored variants): best stays 93.1 pct.
// The swapped ternary (size + 1 < e->field_da) is byte-identical at 93.1;
// dropping the count parameter from the inline LineSize helper is 85.3. The
// 0x10 denominator registers and the 0x20 field_c6 load-before-test are untouched,
// as every source shape for them either folds or reshapes the prologue.
// GPT-6.1-sol (#3140 retry): six checks, best remains 93.1% (629 bytes), no MATCH.
// The 0x10 denominator selection still has different registers and shorter code;
// in the 0x20 arm the field_c6 load remains after the count test and zero-extends
// through edx plus a copy into ecx. Pointer-hoist variants damaged global allocation.
// Retry (deepseek-v4.1-flash, issue 3076): best stays 93.1% (629 bytes both).
// The two-statement 0x10 denominator is now proven exact (631 bytes / 90.6%
// alone); only the 0x20 arm remains, needing `e->field_c6` in edx before
// `test eax,eax`. Every hoist shape reshapes the global allocation (e: ebx->edi,
// shared zero leaves ecx; 602-604 bytes / 36%); arm-local `lines2` keeps ecx=0
// but the load stays after the test (631 / 90.6); a signed `short` gives
// 627 / 91.0 but is semantically wrong.
// BUG: `idiv ecx` at 0x4a40d1 divides by the zero register on the
// `e->field_c0 <= 0` path (the jle at 0x4a40b2 skips the divisor setup), an
// unguarded divide-by-zero, unlike the 0x80 arm which tests both divisors.
// deepseek-v4.1-flash retry (issue 2905, 10 min, ~25 free --sym variants, all
// deepseek-v4.1-flash (#2961 retry): still 93.1%. The 0x10 arm needs the
// two-statement denominator (+2 -> 631 bytes/90.6%). The 0x20 arm needs
// `e->field_c6` hoisted to edx before `test eax,eax` (-2); every source shape
// that makes a live local/param there kills the function-wide shared ecx constant
// zero and reshapes the prologue (602-604 bytes, ~35%). Single-expression spelling
// compiles byte-identically to the base. headers.py 128 sets flat.
// Suspected bug: the 0x20 arm divides by the zero register at 0x4a40d1 when
// `e->field_c0 <= 0` (the jle at 0x4a40b2 skips the setup), an unguarded
// divide-by-zero, unlike the 0x80 arm which tests both divisors.
// scored with check.py --sym). No variant beat the 93.1% already in this file.
// Confirmed the whole diff is two 2-byte arms that cancel in total size:
//   * 0x10 arm denominator: the two-statement `int other = e->field_da; int
//     denominator = size + 1; if (other > denominator) denominator = other;`
//     gives the original `movsx edx / lea edi,[eax+1] / cmp edx,edi / jle /
//     mov edi,edx` (+2 bytes vs the ternary here).
//   * 0x20 arm: the original loads `e->field_c6` into edx BEFORE `test eax,eax`
//     and zero-extends into ecx (`xor ecx,ecx; mov cx,[edx+2]; imul ecx,eax`);
//     every source shape here loads it after the test into ecx and then copies
//     to ecx (`mov ecx,[ebx+0xc6]; ... xor edx,edx; mov dx,...; mov ecx,edx`),
//     +2 bytes. The copy disappears only when b lands in edx, which needs ecx
//     to hold the live zero at the load, which needs the pointer load hoisted
//     before the test.
// Tried and rejected (free --sym scores): helper with an unconditional
// pointer/value local (602 bytes, 34.7%, reshapes the prologue so the search's
// `kind` leaves cl); passing the pointer as a helper parameter (same reshape);
// function-scope `int lines = 0` with a pointer local or an unconditional `a`
// load (633 bytes, 60.1%, lines falls to ebp); function-scope `int lines = 0`
// with an inline `if (count > 0)` body (635 bytes, 61.4%, lines in ebp); a
// ternary divisor (629 bytes, 92.6%, turns `test eax,eax` into `cmp eax,ecx`);
// arm-local int/unsigned-short temporaries (633 bytes, 74.9%); union vs plain
// int and every helper body spelling (629 bytes, 93.1%, unchanged). headers.py
// (128 sets) changes nothing. The 0x10 fix plus the 0x20 fix would be a MATCH.
// Retry #1758 (deepseek-v4.1, issue 2461): best stays 93.1% (629 bytes both).
// The 0x10 arm is now solved exactly: `int other = e->field_da; int
// denominator = size + 1; if (other > denominator) denominator = other;`
// gives the original movsx edx / lea edi,[eax+1] / cmp edx,edi / jle / mov
// edi,edx, but it costs +2 bytes so it scores 90.6% alone (every jump after
// the arm shifts by 2) until the 0x20 arm also loses its 2 extra bytes.
// The 0x20 arm needs `mov edx,[ebx+0xc6]` BEFORE `test eax,eax` and the
// zero-extension in ecx (`xor ecx,ecx; mov cx,[edx+2]; imul ecx,eax`).
// A pointer local in an inlined helper rewrites the whole entry allocation
// (kind leaves cl, 34.7 to 36.3%); a pointer local in the arm keeps the
// allocation but the union needs the unconditional helper assignment, and
// `?: 0` hoists the pointer into ecx (not edx) plus a redundant join xor
// (80.4%). Original bug kept: 0x4a40d1 divides by ecx even when the jle at
// 0x4a40b2 skipped the setup, so `lines` is 0 there and this is a divide by
// zero (the 0x80 arm guards its divisors with test, this arm does not).
// Earlier passes below; the best variant is the one in this file.
// Retry #1758: GPT-6.1-sol best is 93.1% after refinement; latest best check confirmed no MATCH. A single-use helper for the conditional divisor fixes shared-zero stack setup. The 0x10 denominator register choice and 0x20 pointer/divisor sequence still differ.
// GPT-6.1-sol pass: best measured score 80.9% (650 source bytes vs 629,
// nine checker runs). The 0x10 arm improved by computing its numerator before
// selecting the denominator. Remaining differences include zero initialization
// stores and register allocation around the shared zero, plus the 0x20 arm's
// divisor load/zero-extension and the 0x80 arm's zero comparison.
// GPT-6 retry: 78.6%, not MATCH. A zero-initialized full-width union with
// a short view preserves the complete 32-bit divisor, fixing the byte
// truncation in the previous attempt while improving the score.
// Historical measurements and semantic warnings below refer to older code.
// PARTIAL, 77.2% (633 bytes against 629). The prologue, the entry search, the
// 0x10 arm and the 0x80 arm now match line for line; the 0x20 arm is off only
// because of the width of its zero register.
//
// deepseek-v4.1-flash (#3076 retry): still 93.1%. The 0x10 arm is now exact
// with the two-statement denominator (631/90.6 alone). The 0x20 arm is the only
// residual: it needs `e->field_c6` in edx loaded before `test eax,eax`. Every
// hoist shape (helper local, arm local, pointer param, unsigned value, hoisted
// deref) reshapes the global allocation (e: ebx->edi, shared zero leaves ecx;
// 602-604 bytes/36%). An arm-inline body with an arm-local `lines2` keeps the
// zero in ecx but the load stays after the test (631/90.6). Signed `short`
// drops the zero-extend (627/91.0) but is semantically wrong.
//
// deepseek-v4.1-flash pass (10 minutes; every figure below measured with
// `check.py --sym`, which is free). The whole remaining difference is the
// WIDTH of the one zero register. The original materialises a 32-bit zero in
// ecx at the compare (0x4a3f4e `xor ecx,ecx; cmp eax,ecx`), keeps it in ecx
// and reuses it for the 0x10 arm's `n` initial value (0x4a3f8a
// `mov [esp+0x10],ecx`) and for the 0x20 arm's divisor fallback (0x4a40d1
// `idiv ecx`). Declaring `lines` as `char` gets the allocation right: MSVC
// then only needs a BYTE zero (`xor cl,cl`, and it rematerialises the 32-bit
// zero as the immediate `mov dword ptr [esp+0x10],0`), which leaves ebp free
// for `entries`. With `int lines` the allocator takes ebp for `lines` and
// pushes `entries` into ecx and then into a spill slot, which rewrites the
// whole prologue and drops the score to 56.0 percent.
//
// WARNING: `char lines` is semantically WRONG. In the 0x20 arm the original
// loads a 16-bit value into cx, multiplies into the full 32-bit ecx
// (0x4a40bb `mov cx,[edx+2]`, 0x4a40bf `imul ecx,eax`) and divides by it,
// which a char cannot represent. This shape is kept only because it isolates
// the single remaining decision (a 32-bit zero register) from the allocation
// of `entries`. The semantically correct `int lines` version is
// build/scratch/0x4a3ef0/v0.cpp and scores 56.0 percent.
//
// What the next attempt needs: the source shape that keeps `entries` in ebp
// AND `lines` a 32-bit zero in ecx. Measured dead ends (free --sym scores):
// `unsigned int lines` 55.5, `short lines` 57.7, `unsigned short lines` 38.9,
// `long lines` 56.0, `if (found != lines)` 56.0 (the front end value-numbers
// lines to 0 and folds it to `test eax,eax`), `int n = lines` 56.0,
// `int lines; lines = 0;` 56.0, `lines` declared before `found` 47.2,
// `lines` declared before `me` 47.2, `lines = lines + 0` 56.0, `(int)lines`
// at the divide 56.0, an address-taken `lines` 46.0. The front end folds every
// `found == 0`-equivalent compare to a literal test, so the ecx zero must come
// from a real variable the allocator will not spill.
//
// deepseek-v4.1-flash retry (issue 1644, 10 min, ~20 free --sym variants; see
// build/scratch/0x4a3ef0/ledger.md for the full table). Confirmed the cause of
// the int-version regression: `int lines` is linear-live from the compare
// 0x4a3f4e across the 0x10 arm's calls (0x4a3fd3, 0x4a3fed, 0x4a3fff/0x4a400c),
// so MSVC gives it a callee-saved register (ebp) and spills `entries`, which
// rewrites the prologue; the char version folds `lines` and has no allocation
// pressure, which is the only reason its prologue matches. New dead ends (all
// free --sym): 0x80 arm comparing `field_da != lines` in the char version folds
// back to `test dx,dx` (77.2, unchanged); `int n = lines`; lines declared
// inside the found block (36.8); inside the 0x20 arm (32.6); n hoisted to
// function scope (16 to 18); the `(field_da > size+1) ? ...` span ternary
// (56.0, byte-identical); hoisting `field_c0`/`field_c6` before the 0x20 test
// (30, so the original's load order there is not a hoisted local). Nothing
// keeps `entries` in ebp and a 32-bit `lines` in ecx at once.
//
// The previous pass's notes, still accurate for the int version:
//
// PARTIAL, 56.0% (635 bytes against 629), up from 15.0% (Sonnet 5.5 retry, #1080).
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

// deepseek-v4.1-flash (#3076 retry): still 93.1%. The 0x10 arm is now exact
// with the two-statement denominator (631/90.6 measured alone). The 0x20 arm is
// the only residual: it needs `e->field_c6` in edx loaded before `test eax,eax`
// so the chain is `mov ecx,[edx]; mov edx,[ecx+0x28]; xor ecx,ecx; mov cx,[edx+2]`.
// Every hoist shape (helper local, arm local, pointer param, unsigned value,
// hoisted deref) reshapes the global allocation (e moves ebx->edi and the shared
// zero leaves ecx; 602-604 bytes/36%). An arm-inline body with an arm-local
// `lines2` keeps the zero in ecx and fixes the <=0 divisor fallback but the load
// stays after the test (631/90.6). Signed `short` drops the zero-extend
// (627/91.0) but is semantically wrong (the original zero-extends with `mov cx`).
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

static inline int LineSize_004a3ef0(Entry_004a3ef0* e, int count)
{
    int lines = 0;
    if (count > 0) {
        int a = *(int*)e->field_c6;
        int b = *(int*)(a + 0x28);
        lines = *(unsigned short*)(b + 2) * count;
    }
    return lines;
}

// FUNCTION: 0x4a3ef0
void __stdcall FUN_004a3ef0(Class_004a3ef0* param_1, int param_2)
{
    Entry_004a3ef0* entries = param_1->holder->entries;
    Entry_004a3ef0* me = &entries[param_2];
    unsigned char kind = me->kind;
    int found = Find_004a3ef0(entries, kind);
    union { int full; short word; } lines;
    lines.full = 0;
    lines.word = 0;
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
                int numerator = e->field_19 - 2;
                int denominator = (e->field_da > size + 1) ? e->field_da : size + 1;
                int step = numerator / denominator;
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
                int count = e->field_c0;
                lines.full = LineSize_004a3ef0(e, count);
                int s = e->field_19 * me->field_19 / lines.full;
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
