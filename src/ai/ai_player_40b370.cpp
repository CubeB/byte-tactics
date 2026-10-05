// Decompiled by Haiku. Names are provisional.

struct PlayerAI {
    void BuildFeatureCells();
};

extern void* g_playerAI[];

// FUNCTION: 0x40b370
void __stdcall RebuildFeatureCells(int param_1)
{
    ((PlayerAI*)g_playerAI[param_1])->BuildFeatureCells();
}
