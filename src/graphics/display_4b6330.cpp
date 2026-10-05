// Decompiled by Haiku. Names are provisional.

extern void* DAT_0051fbd0;

struct GlobalObj {
    char unknown_0[0xe8];
    int field_e8;
};

// FUNCTION: 0x4b6330
int FUN_004b6330()
{
    GlobalObj* obj = (GlobalObj*)DAT_0051fbd0;
    return obj->field_e8;
}
