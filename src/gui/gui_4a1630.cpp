// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Fills the bounding rectangle of a gadget entry: the entry's position and
// size when it is active, or a degenerate rectangle at the origin otherwise.

#pragma pack(push, 1)
struct Entry_004a1630 {
    char active;                       // +0x0
    char unknown_1[0x12];
    short x;                           // +0x13
    short y;                           // +0x15
    short w;                           // +0x17
    short h;                           // +0x19
};
#pragma pack(pop)

struct Rect_004a1630 {
    int x1;
    int y1;
    int x2;
    int y2;
};

// FUNCTION: 0x4a1630
void __stdcall GetGadgetRect(Entry_004a1630* entry, Rect_004a1630* rect)
{
    if (entry->active == 0) {
        rect->x1 = 0;
        rect->y1 = 0;
    } else {
        rect->x1 = entry->x;
        rect->y1 = entry->y;
    }
    rect->x2 = entry->w + rect->x1 - 1;
    rect->y2 = entry->h + rect->y1 - 1;
}
