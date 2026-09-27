// Decompiled by Opus. Names are provisional.
#include <windows.h>
#include <mmsystem.h>

class Class_004d00d0 {
public:
    char unknown_0[0x14];
    unsigned int aux_device;           // +0x14
    char unknown_18[0x20 - 0x18];
    int aux_volume_set;                // +0x20
    char unknown_24[0x284 - 0x24];
    int field_284;                     // +0x284

    int FUN_004d00d0(int volume, int temporary);
};

static inline int ClampVolume(int v)
{
    if (v < 0) {
        v = 0;
    }
    if (v > 0xffff) {
        v = 0xffff;
    }
    return v;
}

// Sets the aux (CD) volume, clamped to 0..0xffff; unless `temporary`, it is
// also remembered. Returns nonzero on success.
// FUNCTION: 0x4d00d0
int Class_004d00d0::FUN_004d00d0(int volume, int temporary)
{
    if (field_284 != 0 && temporary == 0) {
        return 1;
    }
    int v = ClampVolume(volume);
    if (temporary == 0) {
        aux_volume_set = v;
    }
    return auxSetVolume(aux_device, (v << 16) | v) == 0;
}
