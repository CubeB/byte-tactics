// Decompiled by Haiku. Names are provisional.

#include <cstdlib>

struct Class_00470a90 {
    void Construct(int, int);
};

extern Class_00470a90 DAT_0051e610;
extern void __cdecl ExitParticlePool();

// FUNCTION: 0x471c80
void FUN_00471c80()
{
    DAT_0051e610.Construct(0x3e8, 0x4c);
    atexit(ExitParticlePool);
}
