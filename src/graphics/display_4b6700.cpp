// Decompiled by Haiku. Names are provisional.

extern void* DAT_0051fbd0;

struct GlobalObj {
    char unknown_0[0xd4];
    int field_d4;
};

// FUNCTION: 0x4b6700
int FUN_004b6700()
{
    GlobalObj* obj = (GlobalObj*)DAT_0051fbd0;
    return obj->field_d4;
}
