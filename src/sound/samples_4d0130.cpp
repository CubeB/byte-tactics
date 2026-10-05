// Decompiled by Opus. Names are provisional.

#include <windows.h>
#include <mmsystem.h>

class Class_004d0130 {
public:
    char unknown_0[0x10];
    int wave_devices;                  // +0x10
    unsigned int aux_device;           // +0x14
    int wave_volume;                   // +0x18
    int aux_volume;                    // +0x1c
    int aux_volume_set;                // +0x20
    char unknown_24[0x284 - 0x24];
    int field_284;                     // +0x284

    void RestoreMixerVolumes();
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

// FUNCTION: 0x4d0130
void Class_004d0130::RestoreMixerVolumes()
{
    if (wave_volume >= 0) {
        int v = ClampVolume(wave_volume);
        for (int i = 0; i < wave_devices; i++) {
            waveOutSetVolume((HWAVEOUT)i, (v << 16) | v);
        }
    }
    if (aux_volume >= 0 && field_284 == 0) {
        int v = ClampVolume(aux_volume);
        aux_volume_set = v;
        auxSetVolume(aux_device, (v << 16) | v);
    }
}
