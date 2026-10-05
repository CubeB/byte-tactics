// Decompiled by Opus. Names are provisional.
// GetRect is the body of FUN_004c2340 (copies the 24-byte rectangle at +0x196
// with memcpy); a struct assignment computes the source with mov/add, not lea.
#include <string.h>
struct Rect_004c2380 {
    int x;                             // +0x0
    int y;                             // +0x4
    int data[4];
};

#pragma pack(push, 1)
struct Obj_004c2380 {
    char unknown_0[0x196];
    Rect_004c2380 rect;                // +0x196
    int count;                         // +0x1ae
    short* image;                      // +0x1b2
    char unknown_1b6[0x1ce - 0x1b6];
    int mode;                          // +0x1ce
};
#pragma pack(pop)

Obj_004c2380* GetDisplay();
void __stdcall DrawFrame(void* param_1, short* param_2, int x, int y);

static inline void GetRect(Rect_004c2380* out)
{
    Obj_004c2380* p = GetDisplay();
    memcpy(out, &p->rect, sizeof(Rect_004c2380));
}

// FUNCTION: 0x4c2380
void FUN_004c2380()
{
    Obj_004c2380* obj = GetDisplay();
    if (obj->mode != 1 && obj->count <= 0) {
        Rect_004c2380 r;
        GetRect(&r);
        DrawFrame(0, obj->image, r.x, r.y);
    }
}
