// Decompiled by Haiku. Names are provisional.

extern void __cdecl HapinetTrace(const char*);

// FUNCTION: 0x4c9d10
int __stdcall HAPINET_justone(int p1, int p2, int p3, int p4, void* p5)
{
    HapinetTrace("HAPINET_justone\n");
    int* addr = (int*)p5;
    addr[0x12] = p1;
    return 0;
}
