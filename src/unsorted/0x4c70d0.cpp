// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
//
// Partial: 78.9% (276 bytes; the original is 280). The search loop, the tail
// arithmetic, the range test, the jump table and case 3 match byte for byte.
// The best form found switches case 0's third argument to DAT_0051fef0, which
// adds the global reload the original has there and keeps case 3 matching.
// Still different, all three a pure scratch-register rotation with the push
// order, the pushed addresses and the store positions already correct:
//   case 0: the original computes `size - offset`, stores `out->low = 0`,
//           then reloads DAT_0051fef0 into eax and loads at_high late into
//           ecx. Here at_high is loaded early into eax and the global reload
//           lands in edx, so the block is rotated by one.
//   case 1: the original loads at_low into edx before any push, here it is
//           loaded into ecx after the first push (order of the two is swapped).
//   case 2: same rotation as case 0: the original stores out->low from ecx,
//           reloads DAT_0051fef0 into edx and loads at_high late into eax;
//           here size is pushed from ecx first and at_high is loaded early.
//
// Measured this pass, all on top of the base 78.7% form unless said:
//   - case 0's third argument = DAT_0051fef0: 78.9%, 276 bytes (this file).
//   - case 0 with `int d = size - offset;` before the store: same 78.9%.
//   - DAT_0051fef0 as the third argument of cases 0 and 2: 74.6%, and it
//     breaks case 3 too (the rotation moves into case 3), so case 2's global
//     was reverted.
//   - the global in all four cases: 74.6%. Global in cases 0, 1 and 2: 74.6%.
//   - explicit locals for the arguments (d, g, bound) and for the result,
//     comma expressions, `out[0]`/`out[1]`, and an `Interp`/`Ref` inline
//     wrapper: no change or worse.
//   - swapping the source order of the arguments via a reversed-argument
//     inline helper: 78.9% (280 bytes) but the same four mismatch hunks.
//   - reordering the case labels in the source: 59.0%.
//   - declaring `offset` before `size`: 32.8% (breaks the pre-switch).
//   - tools/headers.py --cpp: all 768 sets are 78.7%.
//   - defining the neighbour 0x4c71f0 before this function in the same file:
//     74.3% (compiler state does change the code, but not toward the original).
//
// The other three cases stay a register rotation. The sibling 0x4c71f0 has the
// same failure on its case 1; there the original reloads at_high late into ecx
// after two pushes and MSVC 5 hoists it into eax.

// GLOBAL: 0x51fe48
extern int DAT_0051fe48[];
// GLOBAL: 0x51fef0
extern int DAT_0051fef0;

struct Chunk {
    int field_0;
    int field_4;
};

struct Range {
    int low;
    int high;
};

// GLOBAL: 0x51fef8
extern Chunk* DAT_0051fef8;
// GLOBAL: 0x51fefc
extern int DAT_0051fefc;
// GLOBAL: 0x51ff00
extern int DAT_0051ff00;

int FUN_004b7381(int a, int b, int c);

// FUNCTION: 0x4c70d0
void __stdcall FUN_004c70d0(int value, Range* out, int at_low, int at_high)
{
    int i = DAT_0051ff00;
    Chunk* table = DAT_0051fef8;
    int j = DAT_0051fe48[i];
    DAT_0051fefc = j;
    while (value > table[j].field_4) {
        j = DAT_0051fe48[j];
        i = DAT_0051fe48[i];
        DAT_0051fefc = j;
        DAT_0051ff00 = i;
    }
    int hi = table[j].field_4;
    int lo = table[i].field_4;
    int size = hi - lo;
    int offset = hi - value;
    DAT_0051fef0 = size;
    if (size == 0) {
        return;
    }
    switch (i) {
    case 0:
        out->low = 0;
        out->high = FUN_004b7381(at_high, size - offset, DAT_0051fef0);
        return;
    case 1:
        out->low = FUN_004b7381(at_low, offset, size);
        out->high = 0;
        return;
    case 2:
        out->low = at_low;
        out->high = FUN_004b7381(at_high, offset, size);
        return;
    case 3:
        out->low = FUN_004b7381(at_low, size - offset, size);
        out->high = at_high;
        return;
    }
}
