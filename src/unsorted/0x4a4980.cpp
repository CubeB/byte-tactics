// Decompiled by LongCat 2.5 Preview Free. Names are provisional.
// GAVE UP: 74.2% match. Register allocation and stack layout differences.
// Draws one gadget entry: builds the entry's bounding rect and a destination
// quad, then either blits a texture (field_be via FUN_004b7f30, or field_c2)
// onto it, or fills the rect with the colour at obj+0x8b9.

#pragma pack(push, 1)
struct Entry_004a4980 {
    unsigned char type;               // +0x00
    char unknown_01[0x13 - 0x01];
    short x;                          // +0x13
    short y;                          // +0x15
    short w;                          // +0x17
    short h;                          // +0x19
    char unknown_1b[0xbc - 0x1b];
    union {
        void* surface;                // +0xbc (entry 0 only)
        char unknown_bc[0xc2 - 0xbc]; // +0xbc to +0xc1
    };
    void* field_c2;                   // +0xc2
    short field_c6;                   // +0xc6
    char unknown_c8[0x15b - 0xc8];
};
#pragma pack(pop)

struct Holder_004a4980 {
    char unknown_0[4];
    Entry_004a4980* entries;           // +0x04
};

#pragma pack(push, 1)
struct Class_004a4980 {
    char unknown_0[0x18];
    Holder_004a4980* holder;           // +0x18
    char unknown_1c[0x8b9 - 0x1c];
    unsigned char field_8b9;           // +0x8b9
};
#pragma pack(pop)

struct Point_004a4980 {
    int x;
    int y;
};

struct Quad_004a4980 {
    Point_004a4980 p[4];
};

struct Rect_004a4980 {
    int x1;
    int y1;
    int x2;
    int y2;
};

struct Frame_004a4980 {
    unsigned short w;                 // +0x00
    unsigned short h;                 // +0x02
    short field_4;                    // +0x04
    short field_6;                    // +0x06
    char unknown_8;                   // +0x08
    unsigned char field_9;            // +0x09
};

void* __stdcall FUN_004b7f30(void* a, int b);
void __stdcall FUN_004b7f90(void* surface, void* frame, int x, int y);
void __stdcall FUN_004bf6f0(void* surface, Rect_004a4980* rect, int color);
void __stdcall FUN_004c7580(void* surf, void* entry, Quad_004a4980* dst, Quad_004a4980* src);

// FUNCTION: 0x4a4980
void __stdcall FUN_004a4980(Class_004a4980* obj, int index)
{
    Entry_004a4980* entries = obj->holder->entries;
    Entry_004a4980* e = &entries[index];

    Rect_004a4980 rect;
    rect.x1 = 0;
    rect.y1 = 0;
    if (e->type != 0) {
        rect.x1 = e->x;
        rect.y1 = e->y;
    }
    rect.x2 = e->w + rect.x1 - 1;
    rect.y2 = e->h + rect.y1 - 1;

    Quad_004a4980 dst;
    dst.p[0].x = rect.x1;
    dst.p[3].x = rect.x1;
    dst.p[0].y = rect.y1;
    dst.p[1].x = rect.x2;
    dst.p[1].y = rect.y1;
    dst.p[2].x = rect.x2;
    dst.p[2].y = rect.y2;
    dst.p[3].y = rect.y2;

    Quad_004a4980 src;
    src.p[0].x = 1;
    src.p[0].y = 1;
    src.p[3].x = 1;
    src.p[1].y = 1;

    void* field_be = *(void**)((char*)e + 0xbe);
    if (field_be != 0) {
        Frame_004a4980* result = (Frame_004a4980*)FUN_004b7f30(field_be, e->field_c6);
        if (result != 0) {
            src.p[1].x = result->w - 1;
            src.p[2].x = result->w - 1;
            src.p[2].y = result->h - 1;
            src.p[3].y = result->h - 1;
            if (result->field_9 == 0) {
                FUN_004c7580(*(void**)((char*)entries + 0xbc), result, &dst, &src);
            } else {
                FUN_004b7f90(*(void**)((char*)entries + 0xbc), result, result->field_4 + rect.x1, result->field_6 + rect.y1);
            }
        }
    } else if (e->field_c2 != 0) {
        Frame_004a4980* tex = (Frame_004a4980*)e->field_c2;
        src.p[1].x = tex->w - 1;
        src.p[2].x = tex->w - 1;
        src.p[2].y = tex->h - 1;
        src.p[3].y = tex->h - 1;
        FUN_004c7580(*(void**)((char*)entries + 0xbc), tex, &dst, &src);
    } else {
        FUN_004bf6f0(*(void**)((char*)entries + 0xbc), &rect, obj->field_8b9);
    }
}
