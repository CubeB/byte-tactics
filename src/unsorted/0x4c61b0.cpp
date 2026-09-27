// Decompiled by Opus. Names are provisional.
// Sets bit 0 of the display flags word at +0xf0; returns 0 only when the
// bit is set while the field at +0x9c is still zero.

#pragma pack(push, 1)
struct Obj_004c61b0 {
    char unknown_0[0x9c];
    int field_9c;                      // +0x9c
    char unknown_a0[0xf0 - 0xa0];
    unsigned short flag0 : 1;          // +0xf0 bit 0
    unsigned short bit1_15 : 15;
};
#pragma pack(pop)

extern Obj_004c61b0* FUN_004b6220(void);

// FUNCTION: 0x4c61b0
int __stdcall FUN_004c61b0(int enable)
{
    Obj_004c61b0* obj = FUN_004b6220();
    obj->flag0 = enable;
    if (enable && obj->field_9c == 0) {
        return 0;
    }
    return 1;
}
