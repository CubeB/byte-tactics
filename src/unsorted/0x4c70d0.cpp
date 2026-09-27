// Decompiled by space-bunny-free. Names are provisional.
//
// Partial: 78.7%. The search loop, the tail arithmetic, the range test, the
// jump table and case 3 match byte for byte. Still different:
//   case 0: the original computes `size - offset` first, stores `out->low = 0`
//           next, then reloads DAT_0051fef0 into eax for the third argument;
//           here the third argument is pushed from ecx and the store is sunk
//           to just before the call.
//   case 1: the original loads the at_low argument into edx before any push,
//           here it is loaded into ecx after the first push.
//   case 2: same two differences as case 0 (store sunk, global reloaded into
//           ecx instead of edx).
// Passing DAT_0051fef0 instead of the `size` local for the third argument
// gives the wanted reload and instruction order in cases 0 and 2, but then
// case 3 and the total size stop matching, so the `size` local was kept.
// A second pass measured these: DAT_0051fef0 as the third argument of cases
// 0 and 2 only, 74.6% (it does produce the reload, but into edx instead of
// eax, and it loads at_high early instead of late, so the block rotates the
// other way and drops); the global in all four cases, 74.6%; the global in
// cases 0, 1 and 2, 74.6%; the second argument of case 0 in a local, 74.6%;
// the `out->low` store moved around the call, 74.6%. All 268 bytes except
// the best form's 280. The pre-switch code is already at its best: declaring
// `size` at the top of the function and assigning it later scores 32.8%, and
// dropping the local for `hi - lo` and `DAT_0051fef0` directly does not
// compile into this shape. So the remaining three cases are again a pure
// register rotation (eax/ecx/edx off by one) with the push order, the pushed
// addresses and the store positions all already correct, and no source
// change to the live-value count in those blocks moves it.

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
        out->high = FUN_004b7381(at_high, size - offset, size);
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
