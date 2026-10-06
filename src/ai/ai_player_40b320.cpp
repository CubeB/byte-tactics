// Decompiled by Opus. Names are provisional.

class PlayerAI {
public:
    char unknown_0[0x10d];
    PlayerAI(unsigned char player);
    void ComputeBaseWeights();
};

extern PlayerAI* g_playerAI[];

// FUNCTION: 0x40b320
void __stdcall CreatePlayerAI(int player)
{
    PlayerAI*& slot = g_playerAI[player];
    slot = new PlayerAI(player);
    slot->ComputeBaseWeights();
}
