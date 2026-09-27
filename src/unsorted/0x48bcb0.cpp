// Decompiled by Opus. Names are provisional.
// Returns 1 when the unit's type id (+0xa8) is in the game's list of ids at
// +0x1435f (count at +0x14367).

extern char* g_game;

// FUNCTION: 0x48bcb0
int __stdcall FUN_0048bcb0(char* unit)
{
    short* ids = *(short**)(g_game + 0x1435f);
    int n = *(int*)(g_game + 0x14367);
    for (int i = 0; i < n; i++) {
        if (ids[i] == *(short*)(unit + 0xa8))
            return 1;
    }
    return 0;
}
