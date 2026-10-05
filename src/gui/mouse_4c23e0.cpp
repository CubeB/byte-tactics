// Decompiled by Opus. Names are provisional.
// Draws an object's bitmap at its position: locks the screen surface
// (FUN_004c5ff0), blits with the hand-written routine FUN_004cbbe0, then
// unlocks with the rectangle that was drawn. Without <windows.h> (or
// <stdio.h>/<string.h>) MSVC schedules the rectangle stores differently.
#include <windows.h>

struct Surface_004c23e0 {
    int data[12];
};

struct Bitmap_004c23e0 {
    int width;                         // +0x0
    int height;                        // +0x4
};

#pragma pack(push, 1)
struct Obj_004c23e0 {
    char unknown_0[0x1b6];
    int x;                             // +0x1b6
    int y;                             // +0x1ba
    Bitmap_004c23e0* bitmap;           // +0x1be
    char unknown_1c2[0x1d2 - 0x1c2];
    int visible;                       // +0x1d2
};
#pragma pack(pop)

int __stdcall FUN_004c5ff0(Surface_004c23e0* s);
int __stdcall FUN_004c60d0(Surface_004c23e0* s, RECT* r, int b);
void __cdecl FUN_004cbbe0(Surface_004c23e0* dst, Bitmap_004c23e0* src, int x, int y);

// FUNCTION: 0x4c23e0
void __stdcall FUN_004c23e0(Obj_004c23e0* obj)
{
    if (obj->visible) {
        Bitmap_004c23e0* bmp = obj->bitmap;
        Surface_004c23e0 s;
        if (FUN_004c5ff0(&s)) {
            FUN_004cbbe0(&s, bmp, obj->x, obj->y);
            RECT r;
            r.left = obj->x;
            r.top = obj->y;
            r.right = r.left + bmp->width;
            r.bottom = r.top + bmp->height;
            FUN_004c60d0(&s, &r, 0);
        }
    }
}
