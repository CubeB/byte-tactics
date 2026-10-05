// Decompiled by Sonnet. Names are provisional.
// Slot 0 (IsSatisfied) of the "death timer runs out" defeat condition
// (vtable 0x4fd7a8): met once the game's tick count reaches the limit.

extern char* g_game;

class DefeatDeathTimerRunsOut {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8
    int field_c;                         // +0xc

    virtual int IsSatisfied();
};

// FUNCTION: 0x48fd50
int DefeatDeathTimerRunsOut::IsSatisfied()
{
    unsigned int game_val = *(unsigned int*)(g_game + 0x38a47);
    return game_val >= (unsigned int)field_c;
}
