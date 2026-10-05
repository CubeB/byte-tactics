// Decompiled by Opus. Names are provisional.
// Same frame-sequence layout as 0x4b8b30: advances the reference by `step`
// time units, moving to the next entry whenever its countdown runs out and
// wrapping around (or dropping the sequence when it doesn't loop).

struct Entry_004b8bf0 {
    unsigned short value;
    char unknown_2[6];
};

struct Src_004b8bf0 {
    unsigned short count;           // +0x0
    unsigned char kind;             // +0x2
    char unknown_3[0x2c - 3];
    Entry_004b8bf0 entries[1];      // +0x2c
};

struct Ref_004b8bf0 {
    unsigned short index;           // +0x0
    short value;                    // +0x2
    unsigned char kind;             // +0x4
    char unknown_5[3];
    Src_004b8bf0* src;              // +0x8
};

static inline unsigned short Value_004b8bf0(Ref_004b8bf0* r)
{
    if (r->src)
        return r->src->entries[r->index].value;
    return 0xffff;
}

// FUNCTION: 0x4b8bf0
void __stdcall FUN_004b8bf0(Ref_004b8bf0* ref, short step)
{
    Src_004b8bf0* src = ref->src;
    if (src == 0 || src->count <= 1) {
        return;
    }
    ref->value -= step;
    while (ref->value <= 0) {
        ref->index++;
        if (ref->index >= src->count) {
            if (!ref->kind) {
                ref->src = 0;
                return;
            }
            ref->index = 0;
        }
        ref->value += Value_004b8bf0(ref);
    }
}
