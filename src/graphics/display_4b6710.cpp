// Decompiled by Haiku. Names are provisional.

extern void* g_display;

struct GlobalObj {
    char unknown_0[0xd8];
    int field_d8;
};

// FUNCTION: 0x4b6710
int GetScreenHeight()
{
    GlobalObj* obj = (GlobalObj*)g_display;
    return obj->field_d8;
}
