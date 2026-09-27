// Decompiled by Opus. Names are provisional.
// Sibling of 0x4bab00 and 0x4bab60: copies a 64 KB table into the buffer at
// +0xc0 when bit 5 of the flags word at +0xf0 is set.
#include <string.h>

#pragma pack(push, 1)
struct Obj_004baad0 {
    char unknown_0[0xc0];
    unsigned int* buffer;              // +0xc0
    char unknown_c4[0xf0 - 0xc4];
    unsigned short bit0_4 : 5;         // +0xf0
    unsigned short flag5 : 1;
    unsigned short bit6_15 : 10;
};
#pragma pack(pop)

extern Obj_004baad0* FUN_004b6220(void);

// FUNCTION: 0x4baad0
void __stdcall FUN_004baad0(unsigned int* param_1)
{
    Obj_004baad0* obj = FUN_004b6220();
    if (obj->flag5) {
        memcpy(obj->buffer, param_1, 0x4000 * 4);
    }
}
