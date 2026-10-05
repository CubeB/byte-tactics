// Decompiled by Opus. Names are provisional.
// Redraws the rectangle of GUI entry `index` on the list's surface (entry 0
// holds the surface at +0xbc); an entry of type 0 is placed at (0, 0).

struct Surface_004a1ab0;

#pragma pack(push, 1)
struct Entry_004a1ab0 {                // 0x15b bytes
    unsigned char type;                // +0x0
    char unknown_1[0x13 - 0x1];
    short x;                           // +0x13
    short y;                           // +0x15
    short width;                       // +0x17
    short height;                      // +0x19
    char unknown_1b[0xbc - 0x1b];
    Surface_004a1ab0* surface;         // +0xbc (only meaningful in entry 0)
    char unknown_c0[0x15b - 0xc0];
};
#pragma pack(pop)

struct Holder_004a1ab0 {
    char unknown_0[4];
    Entry_004a1ab0* entries;           // +0x4
};

struct Dialog {
    char unknown_0[0x18];
    Holder_004a1ab0* holder;           // +0x18
};

struct Rect_004a1ab0 {
    int left;                          // +0x0
    int top;                           // +0x4
    int right;                         // +0x8
    int bottom;                        // +0xc
};

void __stdcall GrayRectangle(Surface_004a1ab0* dst, Rect_004a1ab0* rect);
void __stdcall FadeRectangle(Surface_004a1ab0* dst, Rect_004a1ab0* rect, int level);

// FUNCTION: 0x4a1ab0
void __stdcall RedrawGadgetRect(Dialog* obj, int index)
{
    Entry_004a1ab0* entries = obj->holder->entries;
    Entry_004a1ab0* e = &entries[index];
    Rect_004a1ab0 rect;
    if (e->type == 0) {
        rect.left = 0;
        rect.top = 0;
    } else {
        rect.left = e->x;
        rect.top = e->y;
    }
    rect.right = e->width + rect.left - 1;
    rect.bottom = e->height + rect.top - 1;
    GrayRectangle(entries->surface, &rect);
    FadeRectangle(entries->surface, &rect, -0x14);
}
