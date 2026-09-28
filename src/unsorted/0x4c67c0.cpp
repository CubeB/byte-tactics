// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Same input/mouse object family as 0x4c2870 (see that file): when the two
// enable flags at +0x1ce and +0x1d2 are set and a bitmap at +0x1b2 exists,
// fills the destination rectangle at +0x1be from the bitmap's size, offsets
// the object by the bitmap's half size, blits it from `dst` and then draws
// the bitmap at the stored rectangle.

#pragma pack(push, 1)
struct Bitmap_004c67c0 {
    unsigned short width;              // +0x0
    unsigned short height;             // +0x2
    short dx;                          // +0x4
    short dy;                          // +0x6
};

struct Obj_004c67c0 {
    char unknown_0[0x196];
    int rect_x;                        // +0x196
    int rect_y;                        // +0x19a
    char unknown_19e[0x1b2 - 0x19e];
    Bitmap_004c67c0* bmp;              // +0x1b2
    int x;                             // +0x1b6
    int y;                             // +0x1ba
    int* field_1be;                    // +0x1be
    char unknown_1c2[0x1ce - 0x1c2];
    int field_1ce;                     // +0x1ce
    int field_1d2;                     // +0x1d2
};
#pragma pack(pop)

void __stdcall FUN_004c6b70(void* dst, void* bmp, int x, int y);
void __stdcall FUN_004b7f90(void* dst, Bitmap_004c67c0* bmp, int x, int y);

// FUNCTION: 0x4c67c0
void __stdcall FUN_004c67c0(Obj_004c67c0* obj, void* dst)
{
    if (obj->field_1ce != 0 && obj->field_1d2 != 0 && obj->bmp != 0) {
        obj->field_1be[0] = obj->bmp->width;
        obj->field_1be[1] = obj->bmp->height;
        obj->field_1be[2] = obj->bmp->width;
        obj->x = obj->rect_x - obj->bmp->dx;
        obj->y = obj->rect_y - obj->bmp->dy;
        FUN_004c6b70(obj->field_1be, dst, -obj->x, -obj->y);
        FUN_004b7f90(dst, obj->bmp, obj->rect_x, obj->rect_y);
    }
}
