// Decompiled by Opus. Names are provisional.
// Same frame-sequence layout as 0x4b8b30: counts the current entry's timer
// down, and when it runs out moves to the next entry (wrapping around, or
// dropping the sequence when it doesn't loop). Returns 1 when the entry
// changed.

struct Entry_004b8b90 {
    unsigned short value;
    char unknown_2[6];
};

struct Src_004b8b90 {
    unsigned short count;           // +0x0
    unsigned char kind;             // +0x2
    char unknown_3[0x2c - 3];
    Entry_004b8b90 entries[1];      // +0x2c
};

struct Ref_004b8b90 {
    unsigned short index;           // +0x0
    unsigned short value;           // +0x2
    unsigned char kind;             // +0x4
    char unknown_5[3];
    Src_004b8b90* src;              // +0x8
};

// FUNCTION: 0x4b8b90
int __stdcall StepGafSequence(Ref_004b8b90* ref)
{
    Src_004b8b90* src = ref->src;
    if (src) {
        if (ref->value < 2) {
            ref->index++;
            if (ref->index >= src->count) {
                if (ref->kind) {
                    ref->index = 0;
                } else {
                    ref->src = 0;
                    return 1;
                }
            }
            ref->value = src->entries[ref->index].value;
            return 1;
        }
        ref->value--;
    }
    return 0;
}
