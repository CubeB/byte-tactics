// Decompiled by GPT-5.6-Terra, finished by LongCat 2.5 Preview Free, finished by deepseek-v4.1-flash. Names are provisional.
// Draws one side of a GUI entry's rectangle when bit 0 of param_3 is set; the
// side is chosen by bits 0/1/2 of the entry's flags and the colour comes from
// the index `(int)obj + 0x8b2` into the entry's colour table at +0x1f.
//
// The breakthrough (from 62.4% to MATCH) was the colour parameter of
// FUN_004be950: declared `int`, not `unsigned char`. An unsigned char argument
// gives a bare `mov bl,[eax+edx+0x8b2]`; the int parameter forces the
// zero-extension `xor ebx,ebx / mov bl,[...]` the original has, and that extra
// use of ebx is what pushes obj into edx and spills y2, producing the 16-byte
// frame the unsigned-char versions could not reproduce. With the correct type
// the three calls in the if/else-if chain are written out in full (the last
// two tail-merge into one call site) and everything falls into place.

#pragma pack(push, 1)
struct Entry_004a4c90 {                // 0x15b bytes
    char type;                         // +0x00
    char unknown_1[0x12];
    short x;                           // +0x13
    short y;                           // +0x15
    short w;                           // +0x17
    short h;                           // +0x19
    int flags;                         // +0x1b
    unsigned char* colours;            // +0x1f
    char unknown_23[0xbc - 0x23];
    void* surface;                     // +0xbc
    char unknown_c0[0x15b - 0xc0];
};
#pragma pack(pop)

struct Holder_004a4c90 {
    char unknown_0[4];
    Entry_004a4c90* entries;           // +0x4
};

struct Class_004a4c90 {
    char unknown_0[0x18];
    Holder_004a4c90* holder;           // +0x18
};

struct Rect_004a4c90 {
    int x1, y1, x2, y2;
};

void __stdcall FUN_004be950(void* surface, int x1, int y1, int x2, int y2,
                            int color);

static inline void FillRect_004a4c90(Entry_004a4c90* e, Rect_004a4c90* r)
{
    if (e->type == 0) {
        r->x1 = 0;
        r->y1 = 0;
    } else {
        r->x1 = e->x;
        r->y1 = e->y;
    }
    r->x2 = e->w - 1 + r->x1;
    r->y2 = e->h - 1 + r->y1;
}

// FUNCTION: 0x4a4c90
void __stdcall FUN_004a4c90(Class_004a4c90* obj, int index, unsigned char param_3)
{
    Entry_004a4c90* entries = obj->holder->entries;
    Entry_004a4c90* e = (Entry_004a4c90*)((char*)entries + index * 0x15b);
    Rect_004a4c90 rect;
    FillRect_004a4c90(e, &rect);
    if (param_3 & 1) {
        if (e->flags & 1)
            FUN_004be950(entries->surface, rect.x1, rect.y1, rect.x2, rect.y1,
                         e->colours[(int)obj + 0x8b2]);
        else if (e->flags & 2)
            FUN_004be950(entries->surface, rect.x1, rect.y1, rect.x1, rect.y2,
                         e->colours[(int)obj + 0x8b2]);
        else if (e->flags & 4)
            FUN_004be950(entries->surface, rect.x1, rect.y1, rect.x2, rect.y2,
                         e->colours[(int)obj + 0x8b2]);
    }
}
