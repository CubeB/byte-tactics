// Decompiled by Opus. Names are provisional.

class PlayerAI {
public:
    char unknown_0[0x10d];
    PlayerAI(unsigned char player);
};

class Class_00409730 {
public:
    void FUN_00409730();
};

extern PlayerAI* g_playerAI[];

// FUNCTION: 0x40b320
void __stdcall FUN_0040b320(int player)
{
    PlayerAI*& slot = g_playerAI[player];
    slot = new PlayerAI(player);
    ((Class_00409730*)slot)->FUN_00409730();
}
