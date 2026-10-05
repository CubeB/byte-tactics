// Decompiled by Haiku. Names are provisional.

extern void* g_display;

struct GlobalObj {
    char unknown_0[0xe8];
    int field_e8;
};

// FUNCTION: 0x4b6330
int GetTickRate()
{
    GlobalObj* obj = (GlobalObj*)g_display;
    return obj->field_e8;
}
