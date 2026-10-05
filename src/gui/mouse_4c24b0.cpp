// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <windows.h>

struct Surface_004c24b0 {
    int data[12];
};

struct Bitmap_004c24b0 {
    unsigned short width;              // +0x0
    unsigned short height;             // +0x2
    short dx;                          // +0x4
    short dy;                          // +0x6
};

#pragma pack(push, 1)
struct Obj_004c24b0 {
    char unknown_0[0x196];
    int rect_x;                        // +0x196
    int rect_y;                        // +0x19a
    char unknown_19e[0x1b2 - 0x19e];
    Bitmap_004c24b0* bmp;              // +0x1b2
    int x;                             // +0x1b6
    int y;                             // +0x1ba
    int* field_1be;                    // +0x1be
    char unknown_1c2[0x1d2 - 0x1c2];
    int visible;                       // +0x1d2
};
#pragma pack(pop)

int __stdcall FUN_004c5ff0(Surface_004c24b0* s);
void __cdecl FUN_004cbbe0(void* dst, void* src, int x, int y);
void __stdcall FUN_004b7f90(void* dst, Bitmap_004c24b0* bmp, int x, int y);
int __stdcall FUN_004c60d0(Surface_004c24b0* s, RECT* r, int b);

// FUNCTION: 0x4c24b0
void __stdcall FUN_004c24b0(Obj_004c24b0* obj)
{
    if (obj->visible) {
        Surface_004c24b0 s;
        if (FUN_004c5ff0(&s)) {
            RECT r;
            POINT pt;
            GetCursorPos(&pt);
            obj->rect_x = pt.x;
            obj->rect_y = pt.y;
            obj->x = pt.x - obj->bmp->dx;
            obj->y = pt.y - obj->bmp->dy;
            obj->field_1be[0] = obj->bmp->width;
            obj->field_1be[1] = obj->bmp->height;
            obj->field_1be[2] = obj->bmp->width;
            FUN_004cbbe0(obj->field_1be, &s, -obj->x, -obj->y);
            FUN_004b7f90(&s, obj->bmp, obj->rect_x, obj->rect_y);
            r.left = obj->x;
            r.top = obj->y;
            r.right = r.left + obj->bmp->width;
            r.bottom = r.top + obj->bmp->height;
            FUN_004c60d0(&s, &r, 0);
        }
    }
}
