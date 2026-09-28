// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
//
// Partial: 80.0%. The search loop, the range test, the jump table, case 0 and
// case 3 match byte for byte, and the total size is right (280 bytes). Case 1
// and case 2 are register-rotated:
//   case 1: the original leaves eax free for the third argument (the global
//           reload is the 5-byte `mov eax, ds:0x51fe40`) and loads at_high
//           late into ecx, after arg2 (in ecx) has been pushed:
//             sub ecx,eax; mov [esi],edx; mov eax,ds:0x51fe40; push eax;
//             push ecx; mov ecx,[esp+0x24]; push ecx; call
//           Here the compiler hoists the at_high load into eax, so the global
//           goes to edx as `mov edx,[0x51fe40]`:
//             sub ecx,eax; mov eax,[esp+0x1c]; mov [esi],edx;
//             mov edx,[0x51fe40]; push edx; push ecx; push eax; call
//   case 2: the original loads at_low into edx before `push ecx` and loads
//           at_high into eax after `add esp,0xc`; here at_low goes to ecx
//           after the push and at_high to edx before the `add esp`.
// Both are pure scheduling/register choices: the expressions, the push order,
// the pushed addresses and the store positions are already correct. Tested
// locally and with scratch scoring: swapping the two assignments, splitting
// the FUN call into a temporary (`int r = ...`), passing the global instead of
// the `size` local in each subset of cases, passing `DAT_0051fe40 - offset`,
// caching at_low/at_high/size-offset in top-level temporaries, an inline
// wrapper for FUN_004b7381, comma operators and `out[0]/out[1]` indexing. The
// sibling function 0x4c70d0 in this family has the identical unresolved
// rotation (see its file). Passing the global only in cases 1 and 3 (below) is
// the best measured form.

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
