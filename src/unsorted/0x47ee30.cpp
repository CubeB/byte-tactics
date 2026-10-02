// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, reworked by Claude Sonnet 5.5. Names are provisional.
// Status: best 91.2% (169/171), no MATCH. What still differs (exactly two hunks):
// (a) the original rematerialises `lea edi, [ebp+0x99]` at 0x47ee79, the shift
//     loop preheader; this version keeps edi live and emits no lea there;
// (b) the loop back edge: the original forwards the decrement (`mov eax, ecx`
//     at 0x47eeaf) where this version reloads through the base
//     (`mov eax, [ebp+0x99]`), because our loop condition is the base node and
//     our store is the pointer node. See the full shape history below.
// Claude Sonnet 5.5 pass (#746): nothing beat 91.2% and 169 bytes. Compiler state is
// ruled out (declaration-count sweep 0 to 400 in steps of 8 and all 128 header sets
// of headers.py: 91.2% everywhere). About 40 more shapes were scored:
//  * every access spelled `list->count` in a guarded do/while (`if (list->count > 0)
//    do { ... } while (--list->count > 0);`), the same with `while`, or every access
//    spelled `entries[9].field_0` (the count is entries[9].field_0, 9 * 0x11 = 0x99):
//    167 bytes, 67.7%: the compiler folds the count into the list base (list in edi,
//    a zero in ebp, `cmp [edi+0x99], ebp`), so there is no separate count pointer;
//  * a separate `int* count = &DAT_0051e68c->count` (from the global, not from the
//    list) with the do/while `--*count > 0` back edge: 161 bytes, 83.2%. The back
//    edge then forwards (`mov eax, ecx`) as in the original, but the prologue loads
//    the global into eax first and copies it (`lea edi,[eax+0x99]; mov ebp,eax;
//    cmp [edi],0`), and the redundant `lea edi,[ebp+0x99]` is still missing. Which of
//    the guard, `i = ... - 1`, the shift loop's condition and the decrement go through
//    the count pointer or through `list->count` makes no difference (all 8
//    combinations 83.2%, one 80.9%), nor does the guard through a local, through
//    `(char*)list + 0x99` or through `entries[9].field_0`, nor placing the pointer
//    inside the if;
//  * the shift loop or the whole removal (free + shift) as a `static inline` helper
//    taking the list (the hoped-for source of the second `lea edi,[ebp+0x99]`):
//    83.2%, unchanged;
//  * this file's while loop with the back edge as `if (--*count <= 0) break;` (83.7%)
//    or as a `for (;;)` with the guard as an early break (91.2%, same bytes).
// So the two wanted things (a `list` in ebp with `count` derived from it, and a
// forwarded back edge) still exclude each other in every spelling tried.
// Best: 91.2%. The prologue and the tail now match. Two codegen differences remain:
//  - the loop back-edge reloads the count through the base ([ebp+0x99]) where the
//    original keeps the decremented value in eax (`mov eax, ecx`);
//  - the original rematerialises `lea edi, [ebp+0x99]` after the free call, ours
//    keeps edi live instead.
// A natural alternative with the same prologue and back-edge but a one-byte
// prologue difference (DAT is loaded into eax and copied to ebp) is a guarded
// do/while: `int* count = &DAT->count; Entry* entries = DAT->entries;
// if (entries[9].field_0 > 0) { do { ... } while (*count > 0); }`.
//
// Retry by deepseek-v4.1-flash: about 45 shapes were scored in scratch. They
// split into two families and neither reaches 100%:
//  - guard/condition written through the base (`*(int*)((char*)entries+0x99)`,
//    `((List*)entries)->count`, `entries[9].field_0`) keeps the prologue exact
//    (ebp = DAT, edi = &count) but the back edge reloads [ebp+0x99] instead of
//    forwarding the decrement into eax;
//  - condition written as `*count` (the same lvalue as `(*count)--`) makes the
//    back edge forward (`mov eax, ecx`) but the compiler then computes the
//    count pointer first, from a scratch load of the global:
//    `mov eax,[DAT]; ...; mov ebp,eax; lea edi,[eax+0x99]`, and the guard is
//    folded to `cmp [edi], 0`.
// Deriving count from `list`/`entries` (`&list->count`, `(char*)entries+0x99`)
// always folds the pointer away, so count must come from the global to stay a
// separate edi. The original needs both the ebp-based prologue and the
// forwarded back edge at once, plus the redundant `lea edi,[ebp+0x99]` before
// the shift loop, which none of the scored shapes produced.
//
// space-bunny-free second pass (1958): the file below still scores 91.2%, but the
// do/while family is now known to reproduce the body EXACTLY, so the whole
// problem is the prologue, not the loop. Scored here (check.py --sym, no runs):
//   guarded `if (base > 0) do { ... (*count)--; } while (*count > 0);` with
//   `int* count = &DAT_0051e68c->count; Entry* entries = DAT_0051e68c->e;`
//   and a base-form guard (`*(int*)((char*)entries+0x99)`, `entries[9].f0`, or
//   `DAT_0051e68c->count`): 166 bytes, 85.7% in all three spellings, and its
//   diff against the original is EXACTLY two hunks:
//     1. the prologue: ours hoists `mov eax,[DAT]` above the four pushes, copies
//        it with `mov ebp,eax` and does `lea edi,[eax+0x99]`, the original does
//        `mov ebp,[DAT]` and `lea edi,[ebp+0x99]` (+2 bytes);
//     2. the missing rematerialised `lea edi,[ebp+0x99]` at 0x47ee79 (-6).
//   The forwarded back edge (`mov eax,ecx`) and the entire body, including the
//   shift loop, are byte-identical in that shape. So the store and the test MUST
//   be the pointer node; the guard must be the base node; and the count address
//   must be derived from ebp. MSVC derives the count address from ebp only when
//   the global load lands in ebp by itself, which it only does when nothing else
//   needs a register for the global first: with `List* list = DAT; Entry*
//   entries = list->e;` the guard is `mov ecx,[ebp+0x99]` (ecx, not eax) and the
//   `lea edi` is sunk INSIDE the guard block (from eax) -- 166 bytes, 82.5%.
//   New data points, all worse: `for(;;)` with a base-form early break and a
//   pointer-form tail (91.2%, byte-identical to this file, so the two-exit form
//   is not what the do/while gets its shape from); `entries` declared before
//   `count` with the guard `entries[9].f0` (83.2%, the guard folds to
//   `cmp [edi],0`); the count from a second `List*` local (85.7%); the guard
//   reading `DAT_0051e68c->count` directly (83.2%); a second count pointer used
//   only as the shift loop's limit (44.6%, 189 bytes); `*count = i` instead of
//   `(*count)--` (65.1%). All 128 header sets stay at 91.2% (headers.py).
//
// space-bunny-free pass: the two remaining deltas are exactly, and only,
//   (a) class (c), an instruction present in the original and absent here:
//       `lea edi,[ebp+0x99]` at 0x47ee79, the preheader of the shift loop
//       (6 bytes). The original emits that lea twice, at 0x47ee40 and at
//       0x47ee79, so it is a rematerialisation of one address temp, not two
//       source expressions: every `[edi]` use in the original (0x47ee4a, 71,
//       a6, ac, b1) is the same value. The matched 0x47f8c0 has the
//       byte-identical shift loop with the same six registers live across it
//       and does NOT rematerialise, so this is an allocator decision and not a
//       spelling: about 35 shapes (a second block-scoped `&DAT->count` for the
//       shift loop, a copy of the pointer, a `while` shift loop with `j`
//       declared outside, block-scoped pointers, `int* const`, a hoisted
//       `int i`) produced no lea at all.
//   (b) class (a), a register copy where we reload: `mov eax,ecx` (2 bytes) at
//       0x47eeaf against our `mov eax,dword ptr [ebp+0x99]` (6 bytes). Every
//       other difference in the file is a jump target, shifted only by the
//       -2 net byte count of these two.
// Root cause of (b), now pinned down from the matched sibling 0x47f990
// (`Class_0047f990::FUN_0047f990`, whose `while (count > 0) Remove(count-1);`
// with `count` a MEMBER compiles to the byte-identical tail
// `mov ecx,[count]; dec ecx; mov eax,ecx; mov [count],ecx; cmp; jg`): MSVC only
// forwards a reload to the store in front of it when the two lvalue trees are
// the SAME node. There both are `[edi+0x99]`. Here the store is `[edi]` and our
// reload is `[ebp+0x99]`, two different address forms, so the value is
// reloaded. So the original's loop condition must be spelled through the count
// POINTER (same node as the store) and its guard through the list BASE, which
// in C is a guarded do/while, not a `while`.
// That shape does produce the tail (`do {...} while (--(*count) > 0)`, 166
// bytes, 85.7%), but it costs two extra bytes in the prologue: the `*DAT`
// temporary lands in eax and `entries` copies it out
// (`mov eax,[DAT]; push...; mov ebp,eax; ...; lea edi,[eax+0x99]`). Both order
// variants are worse: `count` first keeps the guard unfolded but puts the temp
// in eax, `entries` first folds the guard to `cmp [edi],0` and still puts the
// temp in eax. So the conjunction to satisfy, not a list of shapes, is:
//   the count pointer must come from the global (derive it from a local and it
//   folds to the base form and edi dies), must be declared before `entries`
//   (otherwise the guard folds), and must be materialised late enough for the
//   CSE to rewrite its base to the entries register (which only happens while
//   the loop condition is the base form, which is exactly what blocks the
//   forwarded back edge). Counting the call, the frame and the loop back edge
//   confirms all three are otherwise right: one call to FUN_004d85a0 (cdecl,
//   `push eax; call; add esp,4`), no locals, and the back edge is a `jg` to the
//   loop top with the decrement in the preceding block.
// Calling convention ruled out by measurement, not by reading the declaration:
// on the do/while shape /O2 /Ob2 /MT, /O2 /Ob1 /MT, /O2 /Ob2 /MT /Gd,
// /O2 /Ob2 /MT /Gs all give 85.7%, /Gz gives 84.8% and /Gr gives 80.6%.
// Shapes scored this session, all with the same declarations (so none of these
// needs repeating): base-cast guard with the shift condition or the decrement
// or `i` through the base 71.4/73.0/60.3; `entries[9].field_0` guard 88.9 (the
// current file's score); the decrement through an `int&` alias 57.6, and the
// same through a `char*` cast of entries 52.4 (both kill the edi pointer, 176
// bytes); do/while with the entries declaration first 83.2, with the shift loop
// as a `while` 85.7, with `if (entries[i].data)` truthiness 85.7, with a `List*`
// local or a hoisted `int i` 85.7/85.7, with a list argument to a static inline
// helper 83.2; `while (*count > 0)` 83.2 at 161 bytes; do/while with a
// trailing `if (--(*count) <= 0) break;` 76.2; a second block-scoped
// `&DAT->count` for the shift loop 40.0 at 193 bytes; a `List*` local with
// `&list->count` 66.7 (everything folds onto one register).
//
// deepseek-v4.1-flash second pass: the do/while shape (guard through the base,
// back edge through `*count`) does reproduce the forwarded back edge
// (`mov eax,ecx`) and the whole body, but its prologue always loads the global
// into a scratch and copies it (`mov eax,[DAT]; mov ebp,eax; lea edi,[eax+0x99];
// mov eax,[ebp+0x99]`), 166 bytes, 85.7%, and the redundant preheader
// `lea edi,[ebp+0x99]` never appears. Declaring `count` inside the guard
// (so its first use is after the test), deriving it from `entries`, spelling
// the guard as `entries[9].field_0`, using a `goto`/`for(;;)` guard, or a
// static inline shift helper all leave that prologue unchanged. Conversely the
// base-form `while` (this file) keeps the exact prologue but reloads the count
// through `[ebp+0x99]` on the back edge. The two are an allocator trade-off,
// not a spelling: 91.2% is still the best found.

// deepseek-v4.1-flash retry: 20 more shapes scored with --sym, none beat 91.2.
// Guarded do/while with a trailing `(*count)--` and a base-form guard, for-loops,
// `&entries[9].field_0` count pointers, count declared inside the loop or the for-init
// clause, and `if (base) while (*count)`: all 85.7 / 83.2 / 67.7. A condition spelled
// through the count pointer always forces the eax detour; one through the base always
// reloads on the back edge. The two deltas (missing remat `lea` at 0x47ee79, back edge
// reload instead of `mov eax,ecx`) are the allocator trade-off already documented above.
// 91.2% remains best.

// space-bunny-free pass 3 (checkpoint, 31 min): the file below is unchanged and
// still scores 91.2% (169/171). What this pass added, all measured with
// check.py --sym (no check.py runs):
// * A prologue/tail differ (build/scratch/0x47ee30/screen.py compares our /Fa
//   listing's first 8 instructions and its last 6 against the original's) makes
//   the three regimes exact, and there are only three:
//     - count POINTER from the global + base-form loop test (this file):
//       prologue exact, back edge reloads. 169 bytes, 91.2%.
//     - count POINTER from the global + pointer-form test (guarded do/while):
//       whole body byte-exact including the forwarded `mov eax,ecx`, but the
//       prologue takes the detour. 166 bytes, 85.7%.
//     - count pointer DERIVED from a local (`&list->count`, `(char*)entries+0x99`):
//       prologue exact, but either everything folds onto the base (167 bytes,
//       67.7%, `mov edi,[DAT]; xor ebp,ebp; cmp [edi+0x99],ebp`) or, spelled
//       `*count--` and declared inside the guard, MSVC 5 strength-reduces the
//       count ADDRESS into an induction variable and DROPS THE STORE: 164
//       bytes, 87.8%, exact prologue, tail `mov eax,[edi-4]; sub edi,4` with no
//       write to the count at all. That is a real VC5 codegen bug (the loop
//       would walk off the list), and every other decrement spelling
//       (`(*count)--`, `--(*count)`, `(*count) -= 1`, `*count = *count - 1`,
//       `--*count`) folds instead. Worth knowing before anyone tries it.
// * The detour is not about statement order: 48 combinations of count-pointer
//   source (5) x entries source (3) x guard spelling (6), plus the count
//   declared inside/outside the guard, never give `mov ebp,[DAT]` together
//   with a pointer-form test. Nor do N = 1..8 uncalled inline functions, dead
//   stores or self-assignments at six positions, inline helpers returning
//   `&DAT->count` / `DAT->entries` / the count value, static inline helpers
//   taking the list or (entries, count), a nested `{count, field_9d}` tail
//   struct, or five spellings of the count-pointer expression.
// * Byte arithmetic for the do/while shape: the original's `mov ebp,[0x51e68c]`
//   is the 6-byte 8B 2D form; ours is the 5-byte A1 form plus a 2-byte
//   `mov ebp,eax`, so fixing the prologue alone would give 165 bytes and leave
//   only the remat `lea`.
// * Exe scan (lever 8): `lea edi,[ebp+0x99]` occurs exactly 4 times in the
//   whole exe, at 0x47ee40/0x47ee79 here and 0x47eefa/0x47ef33 in 0x47eee0, so
//   there is no matched sibling with the remat to copy. The matched 0x47f8c0
//   has the same declarations and the same `lea <reg>,[edi+0x99]` structure
//   but no remat, and the current 0x47eee0 file on this branch also produces
//   the eax detour, so this is shared with the issue's other function.
// * What the matched siblings say, read directly: 0x47f8c0 (MATCH) has the
//   same declarations as this file and emits `mov edi,[DAT]; mov eax,[edi+0x99];
//   lea esi,[edi+0x99]`, i.e. it has BOTH the base+disp pre-test and a
//   pointer-form loop test (`while (i < *count)`), with the count pointer
//   surviving in its own register and the list straight in edi. What it has
//   that this function cannot have is a stack argument, a live register index
//   in the condition, and an `else i++` branch. 0x480020 is the same loop as a
//   member (count a member, `this` in ecx) and also matches, and 0x47fca0 has
//   `if (count == 0) return; ... for (i...) entries[i] = entries[i+1]; count--;`
//   with the count as a member. So the shape is right; the register the global
//   lands in is what differs.
// * Two more shapes worth knowing: declaring the count pointer INSIDE the
//   guard block puts the `lea` after the pre-test load (the order the original
//   has) but still derives it from the eax temporary and puts the pre-test
//   load in ecx: 82.5%, 166 bytes. Dropping the `entries` variable altogether
//   and writing `DAT_0051e68c->entries[j]` inside the loop is 34.6% at 179
//   bytes: the global is reloaded on every iteration.
// * Permuter, two runs, no gain: the do/while shape from 85.7% (seed 11, 3099
//   candidates) and this file from 91.2% (seed 12, 4404 candidates).
// * Still to try: a loop whose condition carries a live register (0x47f8c0's
//   `i < *count`), which needs an index that our source does not have.

void __cdecl FUN_004d85a0(int* param_1);

#pragma pack(push, 1)
struct Entry_0047f8c0 {                // 0x11 bytes
    int field_0;                       // +0x0
    int field_4;                       // +0x4
    int field_8;                       // +0x8
    int* data;                         // +0xc
    char field_10;                     // +0x10
};

struct List_0047f8c0 {
    Entry_0047f8c0 entries[9];         // +0x0
    int count;                         // +0x99
    int field_9d;                      // +0x9d
};
#pragma pack(pop)

struct Table_0047ee30 {                // 0x18 bytes
    int field_0;                       // +0x0
    int unknown_4;                     // +0x4
    int unknown_8;                     // +0x8
    int field_c;                       // +0xc
    int unknown_10;                    // +0x10
    int unknown_14;                    // +0x14
};

extern List_0047f8c0* DAT_0051e68c;
extern Table_0047ee30 DAT_005086e0[24];

// The count is reached through its own pointer and the guard reads it through the
// base: that keeps the list in ebp and the count pointer in edi, as in the
// original, instead of folding the count into the list base.
// FUNCTION: 0x47ee30
void FUN_0047ee30()
{
    int* count = &DAT_0051e68c->count;
    Entry_0047f8c0* entries = DAT_0051e68c->entries;
    while (*(int*)((char*)entries + 0x99) > 0) {
        int i = *count - 1;
        if (entries[i].data != 0) {
            FUN_004d85a0(entries[i].data);
            entries[i].data = 0;
        }
        for (int j = i; j < *count; j++)
            entries[j] = entries[j + 1];
        (*count)--;
    }
    ((List_0047f8c0*)entries)->field_9d = 0;
    for (int k = 0; k < 24; k++)
        DAT_005086e0[k].field_c = 0;
}
