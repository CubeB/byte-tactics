// Decompiled by GPT-5.6-Terra, finished by LongCat 2.5 Preview Free, finished by deepseek-v4.1-flash. Names are provisional.
// Draws one side of a GUI entry's rectangle when bit 0 of param_3 is set; the
// side is chosen by bits 0/1/2 of the entry's flags and the colour comes from
// the index `(int)obj + 0x8b2` into the entry's colour table at +0x1f.
//
// Best version (62.4%, 212 of 215 bytes). Everything from the prologue through
// the `mov [esp+0x1c], ebx` bottom spill and the `mov bl, [esp+0x2c]` param_3
// test matches the original byte for byte, and so do the two epilogues.
//
// What still differs: the original evaluates the colour `e->colours[(int)obj +
// 0x8b2]` inside each of the three flag branches (keeping `obj` live in edx and
// loading `entries->surface` from [edi+0xbc] per branch); ours hoists the
// colour into a byte spill and the surface into edx before the chain.
//
// Attempts, all scored with check.py --sym (no real run spent):
//   colour written inline in each branch (direct FUN calls) ... 180 B, 36.8%
//     same with a `char* base = (char*)obj` alias ............. 180 B
//     same with an `int c = e->colours[...]` local per branch . 196 B, 49.1%
//     same inside a three-way inlined Draw(..., mode, color) .. 259 B, 53.0%
//     same with three one-edge helpers ........................ 180 B
//     same with an early return in branch A ................... 180 B
//   DrawAll(entries, Rect&, e, obj) with colour inside ........ 180 B
//   DrawAll(entries, Rect*, e, obj) with colour inside ........ 180 B
//   DrawAll(surface, Rect&, flags, e, obj) 5 params ........... 207 B, 45.0%
//   current (colour hoisted at the call site) ................. 212 B, 62.4%
//   colour hoisted into a local, rest as current .............. 215 B, 57.8%
// Finding: the 16-byte frame appears only when the colour load stays a live
// value across the flag chain; with the colour inline MSVC reloads obj instead
// of spilling rect.bottom, so the frame and its [esp+0x1c] store disappear.
// The mode-helper variant DOES keep the frame but puts obj in ebp (needs a
// push ebp) and clobbers edi with the surface load. Whoever retries: the goal
// is obj in edx (loaded before `sub esp,0x10`), flags in ebx reused as the
// colour, and surface loaded from [edi+0xbc] inside each branch.

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
                            unsigned char color);

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

static inline void DrawAll_004a4c90(void* surface, Rect_004a4c90& r, int flags,
                                    unsigned char color)
{
    if (flags & 1)
        FUN_004be950(surface, r.x1, r.y1, r.x2, r.y1, color);
    else if (flags & 2)
        FUN_004be950(surface, r.x1, r.y1, r.x1, r.y2, color);
    else if (flags & 4)
        FUN_004be950(surface, r.x1, r.y1, r.x2, r.y2, color);
}

// FUNCTION: 0x4a4c90
void __stdcall FUN_004a4c90(Class_004a4c90* obj, int index, unsigned char param_3)
{
    Entry_004a4c90* entries = obj->holder->entries;
    Entry_004a4c90* e = (Entry_004a4c90*)((char*)entries + index * 0x15b);
    Rect_004a4c90 rect;
    FillRect_004a4c90(e, &rect);
    if (param_3 & 1) {
        DrawAll_004a4c90(entries->surface, rect, e->flags,
                         e->colours[(int)obj + 0x8b2]);
    }
}
