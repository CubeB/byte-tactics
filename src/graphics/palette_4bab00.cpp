// Decompiled by Sonnet. Names are provisional.

#include <string.h>

#pragma pack(push, 1)
struct Obj {
    char unknown_0[0xc4];
    unsigned int* buffer; // +0xc4
    char unknown_c9[0xf0 - 0xc8 - 1];
    char pad_odd;
    unsigned short bit0_5 : 6; // +0xf0
    unsigned short flag6 : 1;
    unsigned short bit7_15 : 9;
};
#pragma pack(pop)

extern int GetDisplay(void);

// FUNCTION: 0x4bab00
void __stdcall SetShadeTable(unsigned int* param_1)
{
    Obj* obj = (Obj*)GetDisplay();
    if (obj->flag6) {
        memcpy(obj->buffer, param_1, 0x800 * 4);
    }
}
