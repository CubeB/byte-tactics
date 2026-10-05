// Decompiled by Haiku. Names are provisional.

extern void* g_display;

struct GlobalObj {
    char unknown_0[0xd4];
    int field_d4;
};

// FUNCTION: 0x4b6700
int GetScreenWidth()
{
    GlobalObj* obj = (GlobalObj*)g_display;
    return obj->field_d4;
}
