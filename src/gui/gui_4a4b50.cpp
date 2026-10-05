// Decompiled by deepseek-v4.1-flash. Names are provisional.
// GUI hit test for menu entry `index` (0x15b-byte entries in the object's table
// at +0x18 -> +4). The entry's rectangle comes from its header (x,y,width,
// height; a type-0 header uses origin 0,0), the entry may have a callback at
// +0xb6, and bit 0 of +0xc8 enables mouse handling. A left click (or a right
// click when there is no left) selects the entry and stores 1/2 via
// FUN_004ab690; when the entry was already focused and FUN_004ab5b0 says no
// button of mask 3 is down, focus is cleared. Returns 1 when the click landed
// inside the rectangle of the entry whose focus was just cleared.
//
// Register notes that decide the match: the rectangle is a 16-byte stack
// struct (that is what makes the frame `sub esp,0x10`), the entry is reached
// as `entries[index]` rather than a stored pointer, and each hit test copies
// the object's position (+0x3c) into a local Point first. The last test uses a
// Point* so its two loads stay after the focus store, as in the original.
#pragma pack(push, 1)

struct Class_004a4b50;
struct Point_004a4b50 { int x, y; };

struct Entry_004a4b50 {                       // 0x15b bytes
    unsigned char type;                       // +0x0
    char unknown_1[0x13 - 0x1];
    short x;                                  // +0x13
    short y;                                  // +0x15
    short width;                              // +0x17
    short height;                             // +0x19
    char unknown_1b[0xb6 - 0x1b];
    void (__stdcall* callback)(Class_004a4b50*, Entry_004a4b50*);  // +0xb6
    char unknown_ba[0xc8 - 0xba];
    unsigned char flags;                      // +0xc8
    char unknown_c9[0x15b - 0xc9];
};

struct Table_004a4b50 {
    char unknown_0[4];
    Entry_004a4b50* entries;                  // +0x4
};

struct Class_004a4b50 {
    char unknown_0[0x18];
    Table_004a4b50* table;                    // +0x18
    char unknown_1c[0x3c - 0x1c];
    Point_004a4b50 pos;                       // +0x3c
    char unknown_44[0x64 - 0x44];
    int focus;                                // +0x64
};
#pragma pack(pop)

extern int __stdcall IsMouseButtonMessage(Class_004a4b50* obj, unsigned char buttons);
extern int __stdcall FUN_004ab5b0(Class_004a4b50* obj, unsigned int mask);
extern void __stdcall FUN_004ab690(Class_004a4b50* obj, int value);
extern int __stdcall FUN_0049fc50(Class_004a4b50* obj, int index);

// FUNCTION: 0x4a4b50
int __stdcall FUN_004a4b50(Class_004a4b50* obj, int index)
{
    Entry_004a4b50* entries = obj->table->entries;
    struct Rect { int left, top, right, bottom; } r;
    if (entries[index].type == 0) {
        r.left = 0;
        r.top = 0;
    } else {
        r.left = entries[index].x;
        r.top = entries[index].y;
    }
    r.right = entries[index].width - 1 + r.left;
    r.bottom = entries[index].height - 1 + r.top;
    if (entries[index].callback)
        entries[index].callback(obj, &entries[index]);
    if (entries[index].flags & 1) {
        if (IsMouseButtonMessage(obj, 1)) {
            Point_004a4b50 p = obj->pos;
            if (p.x >= r.left && p.x <= r.right && p.y >= r.top && p.y <= r.bottom) {
                FUN_0049fc50(obj, index);
                FUN_004ab690(obj, 1);
            }
        } else if (IsMouseButtonMessage(obj, 2)) {
            Point_004a4b50 p = obj->pos;
            if (p.x >= r.left && p.x <= r.right && p.y >= r.top && p.y <= r.bottom) {
                FUN_0049fc50(obj, index);
                FUN_004ab690(obj, 2);
            }
        }
        if (obj->focus == index && !FUN_004ab5b0(obj, 3)) {
            obj->focus = -1;
            Point_004a4b50* pp = &obj->pos;
            int py2 = pp->y;
            int px2 = pp->x;
            if (px2 >= r.left && px2 <= r.right && py2 >= r.top && py2 <= r.bottom)
                return 1;
        }
    }
    return 0;
}
