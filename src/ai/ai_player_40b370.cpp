// Decompiled by Haiku. Names are provisional.

struct Class_0040a7b0 {
    void BuildFeatureCells();
};

extern void* g_playerAI[];

// FUNCTION: 0x40b370
void __stdcall RebuildFeatureCells(int param_1)
{
    ((Class_0040a7b0*)g_playerAI[param_1])->BuildFeatureCells();
}
