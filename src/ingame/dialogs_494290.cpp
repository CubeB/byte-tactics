// Decompiled by Opus. Names are provisional.
// Draws a gadget's image at the gadget's position (FUN_004a1630 fills its
// bounding rectangle).

#pragma pack(push, 1)
struct Entry_00494290 {
    char unknown_0[0xba];
    void* image;                       // +0xba
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Owner_00494290 {
    char unknown_0[0xbc];
    void* surface;                     // +0xbc
};
#pragma pack(pop)

struct Inner_00494290 {
    int unknown_0;
    Owner_00494290* entries;           // +0x4
};

struct Gadget_00494290 {
    char unknown_0[0x18];
    Inner_00494290* inner;             // +0x18
};

struct Rect_00494290 {
    int x1;
    int y1;
    int x2;
    int y2;
};

void __stdcall FUN_004a1630(Entry_00494290* entry, Rect_00494290* rect);
void __stdcall DrawSurface(void* dest, void* image, int x, int y);

// FUNCTION: 0x494290
void __stdcall FUN_00494290(Gadget_00494290* gadget, Entry_00494290* entry)
{
    if (entry->image) {
        Rect_00494290 r;
        FUN_004a1630(entry, &r);
        DrawSurface(gadget->inner->entries->surface, entry->image, r.x1, r.y1);
    }
}
