// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, finished by Sonnet 5.5. Names are provisional.
// Moves an object towards the cursor: decrements the hide counter, reads the
// cursor, stores it as the object's rectangle, offsets the object by the
// bitmap's half size, blits it from the screen with DrawSurface, then, if the
// counter is still not positive, copies the rectangle and draws the bitmap
// with DrawFrame. GetRect is the inlined copy helper (memcpy of the 24 byte
// rectangle at +0x196).
//
// The whole difference (the null test of the bitmap scheduled before the two
// cursor stores) came from the calling convention: the original file was built
// with /Gr, so the function is __fastcall. About 100 source shapes at the
// default convention never moved it (earlier notes listed many more).
#include <string.h>
#include <windows.h>

struct Rect_004c2870 {
    int x;                             // +0x0
    int y;                             // +0x4
    int data[4];
};

struct Bitmap_004c2870 {
    unsigned short width;              // +0x0
    unsigned short height;             // +0x2
    short dx;                          // +0x4
    short dy;                          // +0x6
};

#pragma pack(push, 1)
struct Obj_004c2870 {
    char unknown_0[0x196];
    Rect_004c2870 rect;                // +0x196
    int count;                         // +0x1ae
    Bitmap_004c2870* bmp;              // +0x1b2
    int x;                             // +0x1b6
    int y;                             // +0x1ba
    int* field_1be;                    // +0x1be
    char unknown_1c2[0x1ce - 0x1c2];
    int mode;                          // +0x1ce
};
#pragma pack(pop)

Obj_004c2870* GetDisplay(void);
void __stdcall DrawSurface(void* dst, void* bmp, int x, int y);
void __stdcall DrawFrame(void* dst, Bitmap_004c2870* bmp, int x, int y);

static inline void GetRect(Rect_004c2870* out)
{
    Obj_004c2870* p = GetDisplay();
    memcpy(out, &p->rect, sizeof(Rect_004c2870));
}

// FUNCTION: 0x4c2870
void __fastcall FUN_004c2870(void)
{
    Obj_004c2870* o = GetDisplay();
    if (o->mode != 1) {
        if (--o->count <= 0) {
            POINT pt;
            GetCursorPos(&pt);
            o->rect.x = pt.x;
            o->rect.y = pt.y;
            if (o->bmp != 0) {
                o->x = pt.x - o->bmp->dx;
                o->y = pt.y - o->bmp->dy;
                o->field_1be[0] = o->bmp->width;
                o->field_1be[1] = o->bmp->height;
                o->field_1be[2] = o->bmp->width;
            }
            DrawSurface(o->field_1be, 0, -o->x, -o->y);
            Obj_004c2870* o2 = GetDisplay();
            if (o2->mode != 1 && o2->count <= 0) {
                Rect_004c2870 r;
                GetRect(&r);
                DrawFrame(0, o2->bmp, r.x, r.y);
            }
        }
    }
}
