// Decompiled by DeepSeek V4.1 Flash, finished by space-bunny-free. Names are provisional.
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

int FUN_004b7381(int a, int b, int c);

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
