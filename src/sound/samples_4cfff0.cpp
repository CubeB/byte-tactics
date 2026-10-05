// Decompiled by Opus. Names are provisional.
#include <windows.h>
#include <mmsystem.h>

class Class_004cfff0 {
public:
    char unknown_0[0x10];
    int wave_devices;                  // +0x10
    int QueryWaveVolume();
};

// Returns the left-channel volume of the first wave-out device that reports
// one, or -1. The dllimport call is what makes MSVC keep the import address
// in ebx for the loop.
// FUNCTION: 0x4cfff0
int Class_004cfff0::QueryWaveVolume()
{
    DWORD volume;
    for (int i = 0; i < wave_devices; i++) {
        if (waveOutGetVolume((HWAVEOUT)i, &volume) == 0)
            return volume & 0xffff;
    }
    return -1;
}
