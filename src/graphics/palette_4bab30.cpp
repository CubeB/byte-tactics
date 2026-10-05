// Decompiled by Opus. Names are provisional.
// Sibling of 0x4bab00: copies an 8 KB table into the buffer at +0xc8 when
// bit 7 of the flags word at +0xf0 is set.
#include <string.h>

#pragma pack(push, 1)
struct Obj_004bab30 {
    char unknown_0[0xc8];
    unsigned int* buffer;              // +0xc8
    char unknown_cc[0xf0 - 0xcc];
    unsigned short bit0_6 : 7;         // +0xf0
    unsigned short flag7 : 1;
    unsigned short bit8_15 : 8;
};
#pragma pack(pop)

extern Obj_004bab30* FUN_004b6220(void);

// FUNCTION: 0x4bab30
void __stdcall FUN_004bab30(unsigned int* param_1)
{
    Obj_004bab30* obj = FUN_004b6220();
    if (obj->flag7) {
        memcpy(obj->buffer, param_1, 0x800 * 4);
    }
}
