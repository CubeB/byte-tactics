// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Draws one side of a GUI entry's rectangle when bit 0 of param_3 is set; the
// side is chosen by bits 0/1/2 of the entry's flags and the colour comes from
// the colour table at obj+0x8b2 index `index`.
//
// Best version (62.4%): the whole rect/frame is right, but the colour index
// computation (`e->colours[(int)obj + 0x8b2]`) is hoisted into a callee-saved
// register and spilled here, where the original keeps `obj` live in edx and
// loads the byte inside each flag branch. Passing a rect by reference to an
// inline FillRect helper reproduces the original's sub esp,0x10 frame and the
// bottom spill to [esp+0x1c].

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
