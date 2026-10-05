// Decompiled by Sonnet. Names are provisional.
// Slot 0 (IsSatisfied) of the "victory timer runs out" victory condition
// (vtable 0x4fd830): met once the game's tick count reaches the limit.

extern void* g_game;

class Class_0048f610 {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8
    unsigned int field_c;                // +0xc

    virtual int FUN_0048ea00();
};

// FUNCTION: 0x48f610
int Class_0048f610::FUN_0048ea00()
{
    unsigned int game_val = *(unsigned int*)((char*)g_game + 0x38a47);
    return game_val >= field_c;
}
