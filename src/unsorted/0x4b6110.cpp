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

extern int DAT_0051fbd0;

void FUN_004c1aa0(void);
void FUN_004c2cc0(void);
void __stdcall FUN_004ba5f0(Display_004b6110* obj);
void __stdcall FUN_004ba640(Display_004b6110* obj);
void __stdcall FUN_004ba690(Display_004b6110* obj);
void __stdcall FUN_004ba6e0(Display_004b6110* obj);
void __stdcall FUN_004ba730(Display_004b6110* obj);
void __stdcall FUN_004be070(void* item);
void __stdcall FUN_004b4ff0(Display_004b6110* obj);
void FUN_004d85a0(void* p);

// FUNCTION: 0x4b6110
void __stdcall FUN_004b6110(Display_004b6110* d)
{
    FUN_004c1aa0();
    FUN_004c2cc0();
    for (int i = 0; i < d->item_count; i++)
        FUN_004be070(d->items[i]);
    if (d->items)
        FUN_004d85a0(d->items);
    if (d->has_obj_c4)
        FUN_004ba640(d);
    if (d->has_obj_c0)
        FUN_004ba5f0(d);
    if (d->has_obj_c8)
        FUN_004ba690(d);
    if (d->has_obj_cc)
        FUN_004ba6e0(d);
    if (d->has_obj_d0)
        FUN_004ba730(d);
    FUN_004b4ff0(d);
    if (d->dc)
        DeleteDC(d->dc);
    if (d->bitmap)
        DeleteObject(d->bitmap);
    if (d->old_palette)
        DeleteObject(d->old_palette);
    d->dc = 0;
    d->bitmap = 0;
    d->old_palette = 0;
    SystemParametersInfoA(0x5d, (unsigned int)*(int*)((char*)DAT_0051fbd0 + 0xec), 0, 1);
}
