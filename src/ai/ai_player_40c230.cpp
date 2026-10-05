// Decompiled by Haiku. Names are provisional.

extern void* g_playerAI;

// FUNCTION: 0x40c230
int __stdcall GetBuilderCount(int param_1)
{
    char* base = (char*)&g_playerAI;
    void* ptr = *(void**)(base + param_1 * 4);
    return *(int*)((char*)ptr + 0x75);
}
