// Decompiled by Space Bunny Free. Names are provisional.
// Shuts a display object down: stops the input thread, frees the item list and
// the five cached objects, releases the DirectDraw surfaces and the GDI
// objects, then asks for the work area again.

#include <windows.h>

#pragma pack(push, 2)
struct Display_004b6110 {
    char unknown_0[0x44];
    HGDIOBJ old_palette;              // +0x44
    HDC dc;                           // +0x48
    HGDIOBJ bitmap;                   // +0x4c
    char unknown_50[0xf0 - 0x50];
    unsigned short unknown_bits : 5;  // +0xf0, bits 0 to 4
    unsigned short has_obj_c0 : 1;    // +0xf0, bit 5
    unsigned short has_obj_c4 : 1;    // +0xf0, bit 6
    unsigned short has_obj_c8 : 1;    // +0xf0, bit 7
    unsigned short has_obj_cc : 1;    // +0xf1, bit 0
    unsigned short has_obj_d0 : 1;    // +0xf1, bit 1
    char unknown_f2[0x618 - 0xf2];
    void** items;                     // +0x618
    int item_count;                   // +0x61c
};
#pragma pack(pop)

extern int g_display;

void DestroyPreCleanup(void);
void ShutdownMouse(void);
void __stdcall FreeAlphaTable(Display_004b6110* obj);
void __stdcall FreeShadeTable(Display_004b6110* obj);
void __stdcall FreeLightTable(Display_004b6110* obj);
void __stdcall FreeGrayTable(Display_004b6110* obj);
void __stdcall FreeBlueTable(Display_004b6110* obj);
void __stdcall HAPI_CloseArchive(void* item);
void __stdcall ReleaseDirectDraw(Display_004b6110* obj);
void __cdecl FUN_004d85a0(void* p);

// FUNCTION: 0x4b6110
void __stdcall ShutdownEnvironment(Display_004b6110* d)
{
    DestroyPreCleanup();
    ShutdownMouse();
    for (int i = 0; i < d->item_count; i++)
        HAPI_CloseArchive(d->items[i]);
    if (d->items)
        FUN_004d85a0(d->items);
    if (d->has_obj_c4)
        FreeShadeTable(d);
    if (d->has_obj_c0)
        FreeAlphaTable(d);
    if (d->has_obj_c8)
        FreeLightTable(d);
    if (d->has_obj_cc)
        FreeGrayTable(d);
    if (d->has_obj_d0)
        FreeBlueTable(d);
    ReleaseDirectDraw(d);
    if (d->dc)
        DeleteDC(d->dc);
    if (d->bitmap)
        DeleteObject(d->bitmap);
    if (d->old_palette)
        DeleteObject(d->old_palette);
    d->dc = 0;
    d->bitmap = 0;
    d->old_palette = 0;
    SystemParametersInfoA(0x5d, (unsigned int)*(int*)((char*)g_display + 0xec), 0, 1);
}
