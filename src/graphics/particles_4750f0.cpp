// Decompiled by Haiku. Names are provisional.
// Slot 5 of TimedSubParticles: whether it is time to emit again. The rest of the
// class is in particles_4750b0.cpp; this stays apart because it returns bool,
// where Update tests the result as an int.

struct UnknownStruct;
extern UnknownStruct* g_game;

class TimedSubParticles {
public:
    char unknown_0[8];
    unsigned int emitTime;             // +0x8
    bool IsEmitDue();
};

// FUNCTION: 0x4750f0
bool TimedSubParticles::IsEmitDue()
{
    return this->emitTime <= *(unsigned int*)((char*)g_game + 0x38a47);
}
