// Decompiled by GPT-5.6-Terra, finished by LongCat 2.5 Preview Free. Names are provisional.
// Draws one side of a GUI entry's rectangle when bit 0 of param_3 is set; the
// side is chosen by bits 0/1/2 of the entry's flags and the colour comes from
// the colour table at obj+0x8b2.
//
// Best version (62.4%, 212 of 215 bytes). Everything from the prologue through
// the `mov [esp+0x1c], ebx` bottom spill and the `mov bl, [esp+0x2c]` param_3
// test matches the original byte for byte, and so do the two epilogues.
//
// What still differs, and why the obvious fixes make it worse:
// The original evaluates the colour `e->colours[(int)obj + 0x8b2]` inside each
// of the three flag branches, keeping `obj` live in edx the whole way, so each
// branch emits `mov eax,[eax+0x1f] / xor ebx,ebx / mov bl,[eax+edx+0x8b2]` and
// the surface is reloaded from `[edi+0xbc]` in branches A and in the B/C tail.
// Ours hoists the colour into a byte spill at [esp+0x2c] and the surface into
// edx before the chain, which clobbers edx. Writing the colour expression
// textually three times does produce the per-branch loads, but it makes MSVC
// scalarise the whole Rect: `sub esp,0x10` and the `mov [esp+0x1c], ebx` spill
// both disappear, and the result drops to 180 bytes / 36.8%. The hoisted form
// is the only one that keeps the 16-byte frame, and the frame is worth more
// bytes than the colour loads cost.
//
// Note for the next attempt: the earlier header's claim that the flag chain
// tests param_3 and that `mov ebx,[eax+0x1b]` is a dead load is wrong. The
// chain does test e->flags, loaded once into ebx by MSVC and re-tested on bl
// three times. Ours is correct here too: `[eax+31]` is 0x1f (colours) and
// `[eax+27]` is 0x1b (flags), so the `test al,1` really is testing the flags.
// The only genuine differences are the two hoists described above.
//
// Variants measured and rejected (all by /Fa listing markers plus a scored
// --sym build, no check.py run spent on them):
//   colour per branch, chain on e->flags, 3 or 4 params .... 180 B, 36.8%
//   same, 5 params (surface/flags/obj/e) .................. 207 B, 45.0%
//   same, obj passed as Class* instead of int .............. 207 B, 45.0%
//   colour via a byte pointer, deref in each branch ........ 169-202 B, 43-47%
//   colour hoisted, surface passed by value (current) ...... 212 B, 62.4%
//   colour hoisted, surface reloaded per branch ............ 179 B, 59.7%
//   colour hoisted into a local first (exact 215 B) ........ 215 B, 57.8%
//   chain on param_3 with a dead e->flags read ............. 117 B, 25.0%
//   rect inside a struct passed by reference ............... 180 B, 36.8%
//   rect fields as four plain int locals ................... 180 B, 36.8%
//   4- and 5-parameter orderings, Rect& and Rect*, int and
//   Class* obj, surface by value and per branch ........... all 180-207 B
// The screen used two free markers from the /Fa listing: the
// `mov DWORD PTR _rect$[...], ebx` bottom store plus `sub esp, 16` (the frame)
// and the count of `call FUN_004be950` sites (the original has two, ours three).
// Only the hoisted-colour, surface-by-value form has both.

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
