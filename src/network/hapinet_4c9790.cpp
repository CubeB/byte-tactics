// Decompiled by Sonnet. Names are provisional.

extern void __cdecl HapinetTrace(const char*);
extern int g_guaranteePackets;

// FUNCTION: 0x4c9790
int __stdcall HAPINET_guaranteepackets(int param_1)
{
    HapinetTrace("HAPINET_guaranteepackets\n");
    int result = g_guaranteePackets;
    g_guaranteePackets = param_1;
    return result;
}
