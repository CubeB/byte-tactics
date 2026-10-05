// Decompiled by Opus. Names are provisional.

struct Entry_004b8b30 {
    unsigned short value;
    char unknown_2[6];
};

struct Src_004b8b30 {
    unsigned short count;           // +0x0
    unsigned char kind;             // +0x2
    char unknown_3[0x2c - 3];
    Entry_004b8b30 entries[1];      // +0x2c
};

struct Ref_004b8b30 {
    unsigned short index;           // +0x0
    unsigned short value;           // +0x2
    unsigned char kind;             // +0x4
    char unknown_5[3];
    Src_004b8b30* src;              // +0x8
};

static inline unsigned short Value_004b8b30(Src_004b8b30* s, unsigned short i)
{
    if (s)
        return s->entries[i].value;
    return 0xffff;
}

// FUNCTION: 0x4b8b30
void __stdcall InitGafSequence(Ref_004b8b30* ref, Src_004b8b30* src, int index)
{
    ref->index = index < src->count ? index : 0;
    ref->src = src;
    ref->value = Value_004b8b30(src, ref->index);
    ref->kind = src->kind;
}
