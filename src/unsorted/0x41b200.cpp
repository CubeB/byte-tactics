// Decompiled by Opus. Names are provisional.

struct Obj_0041b200 {
    char unknown_0[0x110];
    unsigned int bits_0 : 22;
    unsigned int flag_22 : 1;          // +0x110 bit 22
    unsigned int value_23 : 3;         // +0x110 bits 23-25
};

// FUNCTION: 0x41b200
int __stdcall FUN_0041b200(Obj_0041b200* obj)
{
    if (obj->flag_22) {
        return obj->value_23;
    }
    return 0;
}
