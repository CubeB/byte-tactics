// Decompiled by Haiku. Names are provisional.

extern void* DAT_0051fbd0;

struct GlobalObj {
    char unknown_0[0xd8];
    int field_d8;
};

// FUNCTION: 0x4b6710
int FUN_004b6710()
{
    GlobalObj* obj = (GlobalObj*)DAT_0051fbd0;
    return obj->field_d8;
}
