// Decompiled by Sonnet. Names are provisional.

struct Struct_0040ba80 {
    int a, b, c;
};

extern void* g_playerAI[];

// FUNCTION: 0x40ba80
void __stdcall FUN_0040ba80(int index, Struct_0040ba80* out)
{
    Struct_0040ba80* entry = (Struct_0040ba80*)((char*)g_playerAI[index] + 0x35);
    *out = *entry;
}
