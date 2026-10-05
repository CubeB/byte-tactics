// Decompiled by Opus. Names are provisional.
// Returns whether the point stored at +0x3c/+0x40 lies inside the rectangle
// (x, y, width, height) of the object's entry; 0 when there is no entry.

struct Rect_004b6720 {
    int left;
    int top;
    int right;
    int bottom;
};

#pragma pack(push, 1)
struct Info_004aafe0 {
    char unknown_0[0x13];
    short x;                           // +0x13
    short y;                           // +0x15
    short width;                       // +0x17
    short height;                      // +0x19
};
#pragma pack(pop)

struct Entry_004aafe0 {
    char unknown_0[4];
    Info_004aafe0* info;               // +0x4
};

struct Object_004aafe0 {
    char unknown_0[0x18];
    Entry_004aafe0* entry;             // +0x18
    char unknown_1c[0x20];
    int x;                             // +0x3c
    int y;                             // +0x40
};

int __stdcall PointInRect(Rect_004b6720* r, int x, int y);

// FUNCTION: 0x4aafe0
int __stdcall IsPointInEntryRect(Object_004aafe0* obj)
{
    if (!obj->entry)
        return 0;
    Info_004aafe0* info = obj->entry->info;
    Rect_004b6720 r;
    r.left = info->x;
    r.top = info->y;
    r.right = r.left + info->width - 1;
    r.bottom = r.top + info->height - 1;
    return PointInRect(&r, obj->x, obj->y);
}
