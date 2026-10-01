// Decompiled by DeepSeek V4.1 Flash and space-bunny-free, finished by deepseek-v4.1-flash and space-bunny-free, edited by deepseek-v4.1, finished by GPT-6.1-sol. Names are provisional.
// claude-sonnet-5-5 (#4423): still 80.0%. Permuter (12 min, 420 candidates) found nothing; all 24 orders of hi/lo/size/offset declared at the function top and assigned after the loop (the 0x4c90b0 lever) are byte-identical. Original case 1 evaluates in strict right-to-left order (G in eax, then size-offset, then at_high loaded last into ecx); case 2 loads at_low into edx before the first push and at_high into eax after the result store.
//
// 30-min checkpoint (DeepSeek V4.1 Flash, #4154): still 80.0%, 280 bytes, no
// MATCH; the body below is unchanged and remains the best. This pass measured
// several hundred fresh shapes with a fast local harness
// (build/scratch/0x4c71f0/harness.py, 0.3 s per compile, six workers in
// search1.py) and pinned the mechanism down further; nothing moved the two
// hunks, so none is kept. The shape of the residual is now clear and it is
// worth writing down for the next attempt.
//
// Mechanism (all four blocks agree, including the two that match):
//   * cl5 evaluates a call's arguments right to left. Experiment
//     `out->low = F(at_high, at_low - value, G);` with no store gives
//     `mov eax,[G]` (5-byte accumulator form), then arg2, then arg1, i.e. G
//     gets eax. Case 3 of this function is that same order and matches.
//   * A load is then emitted as early as its destination register allows:
//     a stack-parameter load may be hoisted above a pointer store (case 3
//     proves it), a global load may not (aliasing).
//   * The allocator gives each new value the register that was freed by the
//     instruction before its load. Case 0 (matching): at_low takes eax, the
//     register `sub ecx, eax` just freed. Case 3 (matching): at_high takes
//     edx at the top, then G takes ecx.
//   * Our case 1: at_low->edx, sub frees eax, at_high takes eax (hoisted
//     above the store, which the stack load is allowed to cross), store frees
//     edx, G takes edx. The original instead leaves eax to G and gives
//     at_high ecx, the register arg2's push frees; that one register choice
//     is the whole difference. Same in case 2: the original gives at_high
//     eax after `mov [ecx],eax` frees it; ours gives it edx before add esp.
//   * So both failing hunks are one allocator choice: the original hands the
//     register freed by the preceding instruction to the value loaded LAST
//     (arg1 / the after-call store), ours hoists the same value into the
//     first free register. The 1-byte length gap follows: only eax gets the
//     5-byte `mov eax, ds:[G]`.
//
// Measured this pass, every one 80.0% and byte-identical to the body below
// unless noted, so do not retry them:
//   - all six declaration orders of `int h = at_high; int d = size - offset;
//     int g = DAT_0051fe40;` declared after the store (the 0x461990 pattern,
//     where declaration order does drive the load order); declared before the
//     store they are 78.9% because the global load then crosses the store.
//   - comma forms `out->high = F(at_high, size - offset,
//     (out->low = at_low, DAT_0051fe40));`, the comma in argument 1, the two
//     statements joined by a comma, and the whole pair wrapped in parentheses:
//     the front end normalises the store back out, byte-identical.
//   - inline helpers owning the whole case body with the five parameters in
//     six orders, including reversed, and helpers copying the parameters to
//     locals first (the inliner re-orders the binding back).
//   - `const int at_high`, `const int at_low`, `long at_high`, `unsigned`
//     everywhere (77.8%): no change or worse.
//   - flags: /Ot /Ox /Og /Oi /Oy /GA /GE /G5 /GB are all byte-identical;
//     /G6 is 75.7%, /Oa and /Ow 59.4%, /Os and /O1 28.2%, /Od 16.1%.
//   - defining the sibling 0x4c70d0 or 0x4c7080 before or after this function
//     in the same file: byte-identical.
//   - `*(volatile int*)&at_high` for case 1's first argument: cl5 folds it,
//     byte-identical (no extra load).
//   - `default: return;` before case 3, braces around case 1, `break` instead
//     of `return`: byte-identical.
// Next attempt: the only lever left is the allocator's choice of the register
// for the value loaded last in a block that has a store before the call. It
// must be made busy at the top of the block so the load cannot hoist; every
// source spelling tried so far leaves eax free there. Do not spend budget on
// headers, flags, helpers or declaration orders again.
//
// GPT-6.1-sol retry in #3210: two checker invocations, one initial attempt returned no output. Best remains 80.0%; no MATCH. Existing passes already tried helper, header, declaration, and argument-order variants; case 1/2 argument scheduling still differs.
// #3006 retry by GPT-6.1-sol: two checks retained 80.0%; an inline helper for
// the first case's low output did not change the argument-evaluation mismatch.
//
// Pass (deepseek-v4.1-flash, #2437 retry): still 80.0%, 280 bytes. Four more
// source shapes, every one byte-identical to the body below, so none is kept:
// `int* ph = &at_high;` used as case 1's first argument (the address-taken
// load is folded straight back to the slot and still hoists into eax);
// inline `Range::SetLow`/`SetHigh` methods for the case 1 stores; the stores
// through `(Range*)(int)out` to defeat aliasing knowledge; and a `Pair`
// sub-object layout for Range (the 0x463610 member-sub-object lever). All four
// leave case 1 with at_high hoisted into eax before `mov [esi],edx` and the
// global in edx, and case 2 with at_low in ecx after `push ecx`. That closes
// the aliasing, inline-method and sub-object readings on top of the earlier
// passes, so the residual really is the arg1-vs-arg3 register priority tie
// already documented by the sibling 0x4c70d0.
//
// Pass (deepseek-v4.1, #2097 retry): still 80.0%, 280 bytes, six check.py runs.
// New shapes, every one byte-identical to the body below, so none is kept: the
// case 1 store folded into the call as a comma expression in argument 3,
// `(out->low = at_low, DAT_0051fe40)`, or in argument 1,
// `(out->low = at_low, at_high)`; the same comma in case 2,
// `(out->low = FUN_004b7381(at_low, offset, size), at_high)`; (int) casts on
// all three case 1 arguments; `at_low`/`at_high` copied to `lo_`/`hi_` locals at
// the function top and used everywhere in the switch; a `Range* p = out;`
// local; one dead `static int` before the function; and 256 dummy typedefs
// before the function to perturb compiler state. Since even those leave case 1
// with at_high hoisted into eax and the global loaded into edx, and case 2 with
// at_low in ecx and at_high in edx, the two remaining hunks below are an
// allocator/scheduler choice that statement, argument and declaration shape
// cannot move.
//
// Pass (deepseek-v4.1, #2097): 80.0%, 280 bytes, 9 check.py runs. Six new source
// shapes, all 80.0% and byte-identical to the body below (so none is kept):
//   - a `Mix1(a,b)` inline helper whose body loads DAT_0051fe40 into a local
//     FIRST and then calls FUN_004b7381(a,b,g) (hoping the inliner walks the
//     global before the first argument),
//   - a `Mix2(a,b,c)` helper with `int g = c;` as its first statement,
//   - case 1 with `int r;` for the call result stored afterwards,
//   - case 2 with `int r = FUN(...)` then `out->low = r; out->high = at_high;`,
//   - case 2 with DAT_0051fe40 inline as the third argument,
//   - case 1 with the first argument as `(int)(DAT_0051fe40, at_high)` and
//     case 2 with at_low through an `Id(x)` inline identity.
//   Reordering case 1 so the call precedes `out->low = at_low;` (v5) is 276
//   bytes / 77.8%: MSVC 5 sinks nothing, the store stays where it is written.
// Still differs (only these two hunks): case 1 wants the same bytes as the
// original (store, `mov eax,[DAT_0051fe40]` in the 5-byte moffs form, two
// pushes, then `mov ecx,[esp+0x24]` for at_high as the last push); ours hoists
// the at_high load above the store into eax and moves the global into edx.
// Case 2 wants at_low in edx before the first push and at_high in eax after
// `mov [ecx],eax`; ours loads at_low into ecx after `push ecx` and at_high
// into edx before `add esp,0xc`. The front end's argument walk order is the
// remaining unknown; helper shapes that reorder the parameter walk were tried
// above and did not move it.
//
// Fourth pass (space-bunny-free, #1795): still 80.0%, 280 bytes, no MATCH. Four
// new spellings, every one exactly 80.0% at 280 bytes, so none is kept:
//   - `Range& out` as the second parameter. It also settles the parameter type:
//     the original's own mangled name is ...YGXHPAURange@@HH@Z, and the
//     reference form compiles to ...HAAURange@@HH@Z, so the pointer is right.
//   - case 1's third argument through an inlined getter `SizeGlobal()` that
//     returns DAT_0051fe40, i.e. a call node whose result MSVC must put in eax.
//     It does not change the schedule (still hoists at_high into eax).
//   - `unsigned at_low, unsigned at_high`: no change at all.
//   - the tail as `if (size) { switch (i) { ... break; } }` instead of an early
//     return and four returns: no change.
// tools/headers.py over 128 sets: still nothing, flat 80.0%.
// NEW BYTE FACT, useful for the two failing blocks and for the sibling 0x4c70d0:
// the 280-byte window is 263 bytes of code, one 0x90 pad and the 16-byte jump
// table, and our code is ONE byte longer than the original's. The only length
// difference is case 1's global load: MSVC 5 emits `mov eax, ds:[G]` in the
// 5-byte accumulator form (A1 moffs32), but the same load into edx or ecx is 6
// bytes (`8B 15` / `8B 0D` + moffs32). That is why the original's `je 0x4c72f1`
// lands one byte before ours (`je 0x4c72f2`). So case 1 is not only a cosmetic
// rotation: getting DAT_0051fe40 into eax is also what makes the size right, and
// the same 1-byte rule applies to every case of 0x4c70d0.
//
// Second pass (#1547, deepseek-v4.1-flash): still 80.0% (280 bytes). Three
// genuinely new spellings, all 80.0% and the same two hunks, so skip them:
//   - the function declared as a __thiscall member (unused `this` in ecx): no
//     change at all, so ecx is not reserved for this when it is never read.
//   - case 1 with `int g = DAT_0051fe40;` declared BEFORE `out->low = at_low`
//     and used as the third argument: 276 bytes / 78.9%, worse (the declared
//     local is scheduled at its declaration, so the global load moves above
//     the store). The guide's sibling recipe wants the local AFTER the store,
//     which is already in the ruled-out list above.
//   - case 2 with `int lo = at_low;` before the call: no change, MSVC folds
//     the local back and still loads at_low into ecx after the first push.
//   - case 1 with an inlined `Late(a,b,c)` forwarder: no change.
// tools/headers.py over 128 sets: none match, closest 80.0% flat.
// Calling convention checked again (#1188): __stdcall ret 0x10 matches, callee 0x4b7381 is cdecl (add esp,0xc) and is declared so; not the cause. Tried (&at_low)[1] for at_high in case 1: 80.0%, same 280 bytes, case 2 diff unchanged.
//
// deepseek-v4.1-flash pass (#1210), no score change, 80.0% (280 bytes). The jump
// table was dumped from the exe (0x4c72f8 = 0x4c7260, 0x4c7284, 0x4c72ad,
// 0x4c72cf), so the case labels are confirmed correct and the two mismatches are
// purely at_high's materialisation. New spellings tried, all 80.0% unless said:
//   - reordering the case bodies in the source (0,2,1,3 and 1,2,0,3): 74.6% and
//     56.2%, so body emission order is load-bearing here as in 0x4c70d0.
//   - writing the case 1 store through `int& low = out->low` or `Range& r = *out`
//     (the 0x41ba60 lever for a load MSVC hoists above a store the original keeps
//     first): unchanged, so that lever does not apply to a hoisted stack-parameter
//     load.
//   - a `const int&` alias for at_high, and a local `int h = at_high;` placed
//     after the store: unchanged.
//   - an inline three-argument forwarder `Forward(a,b,c)` returning
//     FUN_004b7381(a,b,c), and a reversed one `Rev(c,b,a)` calling FUN(a,b,c):
//     both 80.0%, so the inlined-call boundary does not change the schedule here.
//   - `int* out` with out[0]/out[1] instead of Range*: 80.0%, identical bytes.
// The 80.4% variant that passes `value` for case 1 stays rejected: it reads
// parameter 1, not parameter 4 (see the stack arithmetic below).
// Claude Sonnet 5.5 pass (#624): compiler state ruled out (0 to 400 unused `extern
// int` declarations in steps of 8, all 80.0% and 280 bytes). More source shapes
// scored, none moved case 1 or case 2: an inline helper `Scale(a, b)` that reads
// DAT_0051fe40 itself and calls FUN_004b7381(a, b, DAT_0051fe40) in all four cases
// (80.0%; with the global copied to a local first it is 260 bytes and 58.8%), the
// last two parameters as one by-value `Range at` (80.0%, identical bytes), and in
// case 1 a local for size - offset, a `Range* o` alias for the stores, a local for the
// call result, a local copy of at_high, the call as the last statement or the
// first (all 80.0%; passing `size` instead of DAT_0051fe40 or putting the call
// first is 276 bytes, 78.9 and 77.8%). What the original does differently in cases
// 1 and 2: it loads at_low into edx at the top of the case, before the first push
// (case 2 `mov edx,[esp+0x18]; push ecx; push eax; push edx`), and in case 1 it
// loads at_high into ecx only after `push eax(global); push ecx(size-offset)`, i.e.
// as the last push; ours loads the second parameter early into eax and moves the
// global into edx. In case 2 the store of at_high is `mov eax,[esp+0x1c]` after the
// call; ours hoists that load above `add esp, 0xc`.
//
// Partial: 80.0%. The search loop, the tail arithmetic, the size test, the
// jump table, case 0 and case 3 match byte for byte, and cases 1 and 2 differ
// only because of one thing, case 1 (see below).
//
// Case 1's first argument is the FOURTH parameter, at_high, not value. The
// stack arithmetic settles it: the prologue pushes three registers, so esp is
// entry-0xc, and by the time of the load two arguments have been pushed, so
// `mov ecx, [esp + 0x24]` reads entry+0x10, which is parameter 4. The same
// holds at `mov edi, [esp + 0x10]` in the prologue (entry+4, parameter 1) and
// at `mov esi, [esp + 0x14]` / `mov edx, [esp + 0x18]` in case 2 (entry+8 and
// entry+0xc, parameters 2 and 3), so the offsets are all consistent.
// An earlier version of this note claimed it was parameter 1 and changed the
// argument to `value`; that scores 80.4% against this file's 80.0% but is the
// wrong parameter, and the 0.4% is not worth reading the wrong slot. Passing
// `at_high` is also what gives the right size, 280 bytes against 276.
//
// What still differs is one thing in case 1, and it is a placement decision
// rather than a missing value. The original never keeps at_high in a register:
// it re-loads the slot at its point of use, into ecx, after the third and
// second arguments have already been pushed, because ecx is still holding the
// second argument until then.
//     mov eax, [DAT_0051fe40]      ; third argument
//     push eax
//     push ecx                     ; second argument (size - offset)
//     mov ecx, [esp + 0x24]        ; at_high, reloaded here
//     push ecx
//     call FUN_004b7381
// Here MSVC 5 hoists the at_high load to the top of the block into eax and
// leaves ecx free, so the global ends up in edx and the pushes come out in a
// different order. Cases 0, 2 and 3 and all the pre-switch code match.
//
// Tried on top of this version, none of which moved it:
//   - `volatile int at_high` as the parameter, which is the natural reading of
//     "re-loaded from its slot on every use" and is the exception the current
//     AGENTS.md allows. It changes nothing: the load is still hoisted into eax.
//   - an `int g = DAT_0051fe40;` local read after `out->low = at_low;`, to try
//     to force the global into eax the way the original has it. Unchanged.
//   - `value` as the argument (80.4%, 276 bytes): wrong parameter, see above.
// The rest of the ruled-out list from the previous pass follows.
//
// Tried and measured, all on top of this version (80.4% unless said):
//   - an inlined search helper that owns the loop and takes `value` as its
//     own parameter (the 0x4523e0 pattern from the guide): the pre-switch
//     code still matches byte for byte, but the parameter copy is propagated
//     and case 1 still gets `push edi`.
//   - the same helper also returning the offset (272 bytes, 47%: it breaks
//     the tail's register order) or the offset and the size (80.4%, still
//     propagated).
//   - re-reading the parameter as `*(int*)&value`, `*(long*)&value`,
//     `*(unsigned*)&value`, through a `const int&` local, through an
//     `int*` local, through a reference or pointer parameter of an inlined
//     helper, `value + zero`, `(int)(long)value`, `sizeof(int) ? value : 0`,
//     a nested comma assignment, a local for the distance, a local for the
//     call result, a `Range&` for the stores: all propagated to the edi copy.
//   - `at_high` as the anchor (80.0%, 280 bytes: the right size but the
//     wrong value), reversed statements, comma expressions, `out[0]/out[1]`.
//   - tools/headers.py over 768 header sets (each of windows.h, stdio.h,
//     stdlib.h, string.h crossed with none and the C++ headers): all 80.4%.
//   - `*(volatile int*)&value` gives the wanted load and register but also a
//     second, mandatory load of the same slot at the top of the case, so it
//     is 280 bytes and still 80.0%; not a real reading of the original, since
//     a volatile parameter would also be re-read inside the loop.

// GLOBAL: 0x51fe98
extern int DAT_0051fe98;
// GLOBAL: 0x51fea0
extern int DAT_0051fea0[];
// GLOBAL: 0x51fef8
extern struct Chunk* DAT_0051fef8;
// GLOBAL: 0x51fef4
extern int DAT_0051fef4;
// GLOBAL: 0x51fe40
extern int DAT_0051fe40;

struct Chunk {
    int field_0;
    int field_4;
};

struct Range {
    int low;
    int high;
};

int __cdecl FUN_004b7381(int a, int b, int c);

// FUNCTION: 0x4c71f0
void __stdcall FUN_004c71f0(int value, Range* out, int at_low, int at_high)
{
    int i = DAT_0051fe98;
    Chunk* table = DAT_0051fef8;
    int j = DAT_0051fea0[i];
    DAT_0051fef4 = j;
    while (value > table[j].field_4) {
        j = DAT_0051fea0[j];
        i = DAT_0051fea0[i];
        DAT_0051fef4 = j;
        DAT_0051fe98 = i;
    }
    int hi = table[j].field_4;
    int lo = table[i].field_4;
    int size = hi - lo;
    int offset = hi - value;
    DAT_0051fe40 = size;
    if (size == 0) {
        return;
    }
    switch (i) {
    case 0:
        out->low = FUN_004b7381(at_low, size - offset, size);
        out->high = 0;
        return;
    case 1:
        out->low = at_low;
        out->high = FUN_004b7381(at_high, size - offset, DAT_0051fe40);
        return;
    case 2:
        out->low = FUN_004b7381(at_low, offset, size);
        out->high = at_high;
        return;
    case 3:
        out->low = 0;
        out->high = FUN_004b7381(at_high, offset, DAT_0051fe40);
        return;
    }
}