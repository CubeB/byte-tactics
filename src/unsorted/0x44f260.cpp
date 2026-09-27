// Decompiled by Sonnet. Names are provisional.
// Returns *this on success (constructor-like "copies ecx into eax and
// returns it" shape), or null when the flag bit isn't set or the game's
// budget field can't cover the requested amount.

extern char* g_game;

class Class_0044f260 {
public:
    char unknown_0[0x60];
    unsigned int field_60;
    unsigned char field_64;

    Class_0044f260* FUN_0044f260();
};

// FUNCTION: 0x44f260
Class_0044f260* Class_0044f260::FUN_0044f260()
{
    if (field_64 & 2) {
        unsigned int limit = *(unsigned int*)(g_game + 0x38a47);
        if (limit >= field_60 + 0x3c) {
            field_60 = limit;
            return this;
        }
    }
    return 0;
}
