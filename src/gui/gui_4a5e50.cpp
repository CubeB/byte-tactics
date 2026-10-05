// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Draws GUI layout entry `index`: it turns on the manager's redraw flag, then
// blits the entry's glyph at the entry's rectangle offset, with the extra
// style argument when the entry's +0x1f count is positive, and finally fades
// the same rectangle when entry flag +0xbc has bit 0 set. The glyph's own x/y
// come from the loaded glyph's +4/+6.
//
// The rectangle must be one struct local (`Rect_004a5e50 rect;`): as four
// separate scalars MSVC dead-store-eliminates right/bottom (only `left`'s
// address is taken) and reuses the parameter slots, losing the `sub esp,0x10`
// frame entirely. The +0xbc field is a union because entry 0 holds the
// destination surface pointer there while every other entry holds flag bits.

#pragma pack(push, 1)
struct Glyph_004a5e50 {
    unsigned short width;              // +0x0
    unsigned short height;             // +0x2
    short x;                           // +0x4
    short y;                           // +0x6
};

struct Surface_004a5e50;

struct Entry_004a5e50 {                // 0x15b bytes
    unsigned char type;                // +0x00
    char unknown_1[0x13 - 1];
    short left;                        // +0x13
    short top;                         // +0x15
    short width;                       // +0x17
    short height;                      // +0x19
    char unknown_1b[0x1f - 0x1b];
    int count;                         // +0x1f
    char unknown_23[0xb8 - 0x23];
    Glyph_004a5e50* glyph;             // +0xb8
    union {
        Surface_004a5e50* surface;     // +0xbc (entry 0)
        unsigned char flag;            // +0xbc
    };
    char unknown_c0[0x15b - 0xc0];
};

struct Holder_004a5e50 {
    char unknown_0[4];
    Entry_004a5e50* entries;           // +0x4
    char unknown_8[0x14 - 8];
    int field_14;                      // +0x14
};

struct Obj_004a5e50 {
    char unknown_0[0x18];
    Holder_004a5e50* holder;           // +0x18
};
#pragma pack(pop)

struct Rect_004a5e50 {
    int left;                          // +0x0
    int top;                           // +0x4
    int right;                         // +0x8
    int bottom;                        // +0xc
};

void __stdcall FUN_004b7f90(void* surface, void* glyph, int x, int y);
void __stdcall FUN_004b8310(void* surface, void* glyph, int x, int y, int style);
int __stdcall FUN_004bf4d0(Surface_004a5e50* surface, Rect_004a5e50* rect, int level);

// FUNCTION: 0x4a5e50
void __stdcall FUN_004a5e50(Obj_004a5e50* obj, int index)
{
    if (obj->holder != 0)
        obj->holder->field_14 = 1;
    Entry_004a5e50* entries = obj->holder->entries;
    Entry_004a5e50* e = &entries[index];
    Glyph_004a5e50* glyph = e->glyph;
    if (glyph == 0)
        return;
    Rect_004a5e50 rect;
    if (e->type == 0) {
        rect.left = 0;
        rect.top = 0;
    } else {
        rect.left = e->left;
        rect.top = e->top;
    }
    rect.right = e->width + rect.left - 1;
    rect.bottom = e->height + rect.top - 1;
    int count = e->count;
    if (count > 0) {
        FUN_004b8310(entries->surface, glyph, glyph->x + rect.left, glyph->y + rect.top, count);
    } else {
        FUN_004b7f90(entries->surface, glyph, glyph->x + rect.left, glyph->y + rect.top);
    }
    if (e->flag & 1) {
        FUN_004bf4d0(entries->surface, &rect, -0x1c);
    }
}
