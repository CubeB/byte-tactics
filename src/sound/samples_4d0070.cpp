// Decompiled by Opus. Names are provisional.
#include <windows.h>
#include <mmsystem.h>

class Class_004d0070 {
public:
    char unknown_0[0x10];
    int wave_devices;                  // +0x10

    int SetWaveVolume(int volume);
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

// Sets the volume of every wave-out device, clamped to 0..0xffff. Returns
// nonzero if any device failed.
// FUNCTION: 0x4d0070
int Class_004d0070::SetWaveVolume(int volume)
{
    int v = ClampVolume(volume);
    int failed = 0;
    for (int i = 0; i < wave_devices; i++) {
        if (waveOutSetVolume((HWAVEOUT)i, (v << 16) | v) != 0) {
            failed = 1;
        }
    }
    return failed;
}
