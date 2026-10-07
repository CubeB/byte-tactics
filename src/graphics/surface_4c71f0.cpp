// Decompiled by DeepSeek V4.1 Flash and space-bunny-free, finished by deepseek-v4.1-flash and space-bunny-free, edited by deepseek-v4.1, finished by GPT-6.1-sol and space-bunny-free. Names are provisional.

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
        {
            // The distance is passed through a pointer to a local. It reads
            // like a leftover from the original, but it is what puts case 1's
            // global reload in the 5-byte accumulator form.
            int span = size - offset;
            int* spanp = &span;
            out->high = FUN_004b7381(at_high, *spanp, DAT_0051fe40);
        }
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