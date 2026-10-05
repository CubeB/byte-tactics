// Decompiled by Haiku. Names are provisional.

struct Class_00482110 {
    char unknown_0[0x1c];
    unsigned int field_1c;
};

extern void* g_game;

// FUNCTION: 0x482110
int __stdcall FUN_00482110(Class_00482110* obj)
{
    unsigned int field_val = obj->field_1c;
    unsigned int cmp_val = *(unsigned int*)((char*)g_game + 0x38a47);
    return field_val < cmp_val ? 1 : 0;
}
