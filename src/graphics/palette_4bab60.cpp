// Decompiled by Opus. Names are provisional.
// Sibling of 0x4bab00: copies a 256-byte table into the buffer at +0xcc
// when bit 8 of the flags word at +0xf0 is set.
#include <string.h>

#pragma pack(push, 1)
struct Obj_004bab60 {
    char unknown_0[0xcc];
    unsigned int* buffer;              // +0xcc
    char unknown_d0[0xf0 - 0xd0];
    unsigned short bit0_7 : 8;         // +0xf0
    unsigned short flag8 : 1;
    unsigned short bit9_15 : 7;
};
#pragma pack(pop)

extern Obj_004bab60* GetDisplay(void);

// FUNCTION: 0x4bab60
void __stdcall SetGrayTable(unsigned int* param_1)
{
    Obj_004bab60* obj = GetDisplay();
    if (obj->flag8) {
        memcpy(obj->buffer, param_1, 0x40 * 4);
    }
}
