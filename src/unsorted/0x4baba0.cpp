// Decompiled by Opus. Names are provisional.
// Sibling of 0x4bab60: copies a 256-byte table into the buffer at +0xd0
// when bit 9 of the flags word at +0xf0 is set.
#include <string.h>

#pragma pack(push, 1)
struct Obj_004baba0 {
    char unknown_0[0xd0];
    unsigned int* buffer;              // +0xd0
    char unknown_d4[0xf0 - 0xd4];
    unsigned short bit0_8 : 9;         // +0xf0
    unsigned short flag9 : 1;
    unsigned short bit10_15 : 6;
};
#pragma pack(pop)

extern Obj_004baba0* FUN_004b6220(void);

// FUNCTION: 0x4baba0
void __stdcall FUN_004baba0(unsigned int* param_1)
{
    Obj_004baba0* obj = FUN_004b6220();
    if (obj->flag9) {
        memcpy(obj->buffer, param_1, 0x40 * 4);
    }
}
