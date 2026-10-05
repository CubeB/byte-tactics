// Decompiled by Opus. Names are provisional.
// Creates the 0x68-byte object held at g_game+0x2a30.

extern char* g_game;

class UnitSync {
public:
    char unknown_0[0x68];

    UnitSync(int param_1);
};

// FUNCTION: 0x46c8e0
void __stdcall CreateUnitSync(int param_1)
{
    *(UnitSync**)(g_game + 0x2a30) = new UnitSync(param_1);
}
