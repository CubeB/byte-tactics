// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, GPT-6.1-sol and space-bunny-free, finished by GPT-6.1-sol and mimo-v2.6-pro, finished by Space Bunny Free. Names are provisional.

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

int __cdecl FUN_004b7381(int a, int b, int c);

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
    case 0: {
        // The distance is passed through a pointer to a local. It reads like a
        // leftover from the original, but it is what makes cl 5 schedule case 0
        // the way the original does.
        int span = size - offset;
        int* spanp = &span;
        out->low = 0;
        out->high = FUN_004b7381(at_high, *spanp, DAT_0051fef0);
        return; }
    case 1:
        out->low = FUN_004b7381(at_low, offset, size);
        out->high = 0;
        return;
    case 2:
        out->low = at_low;
        out->high = FUN_004b7381(at_high, offset, DAT_0051fef0);
        return;
    case 3:
        out->low = FUN_004b7381(at_low, size - offset, size);
        out->high = at_high;
        return;
    }
}