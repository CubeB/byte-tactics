// Decompiled by Opus. Names are provisional.

class PlayerAI {
public:
    char unknown_0[0x10d];
    PlayerAI(unsigned char player);
};

class Class_00409730 {
public:
    void ComputeBaseWeights();
};

extern PlayerAI* g_playerAI[];

// FUNCTION: 0x40b320
void __stdcall CreatePlayerAI(int player)
{
    PlayerAI*& slot = g_playerAI[player];
    slot = new PlayerAI(player);
    ((Class_00409730*)slot)->ComputeBaseWeights();
}
