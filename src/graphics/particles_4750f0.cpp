// Decompiled by Haiku. Names are provisional.
// Slot 5 of TimedSubParticles: whether it is time to emit again. The rest of the
// class is in particles_4750b0.cpp; this stays apart because it returns bool,
// where Update tests the result as an int.

struct UnknownStruct;
extern UnknownStruct* g_game;

class TimedSubParticles {
public:
    bool FUN_004750f0();
};

// FUNCTION: 0x4750f0
bool TimedSubParticles::FUN_004750f0()
{
    return *(unsigned int*)((char*)this + 8) <= *(unsigned int*)((char*)g_game + 0x38a47);
}
