// Decompiled by Sonnet. Names are provisional.

extern unsigned char DAT_0051e634;

class Class_00470b80 {
public:
    void Destroy();
};

extern Class_00470b80 DAT_0051e610;

// FUNCTION: 0x471ca0
void ExitParticlePool()
{
    if ((DAT_0051e634 & 1) == 0) {
        DAT_0051e634 |= 1;
        DAT_0051e610.Destroy();
    }
}
