// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free. Names are provisional.
//
// Class_004cff30 is a prefix of Sound (the 0x294-byte sound object);
// 0x4cff30 is its "open the devices and cache the volumes" method. The exe's
// 0x4cfff0 (wave volume) and 0x4d0040 (aux volume) are inlined here.
#include <windows.h>
#include <mmsystem.h>

class Class_004cff30 {
public:
    char unknown_0[0x10];
    int wave_devices;                  // +0x10
    int aux_device;                    // +0x14
    int wave_volume;                   // +0x18
    int aux_volume;                    // +0x1c

    // Returns the left-channel volume of the first wave-out device that
    // reports one, or -1.
    int GetWaveVolume()
    {
        DWORD volume;
        for (int i = 0; i < wave_devices; i++) {
            if (waveOutGetVolume((HWAVEOUT)i, &volume) == 0)
                return volume & 0xffff;
        }
        return -1;
    }

    // Returns the left-channel volume of the selected auxiliary device, or -1.
    int GetAuxVolume()
    {
        DWORD volume;
        if (aux_device >= 0 && auxGetVolume(aux_device, &volume) == 0)
            return volume & 0xffff;
        return -1;
    }

    void InitMixerVolumes();
};

// Picks the first CD-audio auxiliary device, then caches the wave-out and
// auxiliary volumes (-1 when unavailable).
// FUNCTION: 0x4cff30
void Class_004cff30::InitMixerVolumes()
{
    AUXCAPSA caps;

    wave_devices = waveOutGetNumDevs();
    aux_device = -1;

    int aux_count = auxGetNumDevs();
    for (int i = 0; i < aux_count; i++) {
        // Result held in a named local: sets the callee-saved register order.
        int result = auxGetDevCapsA(i, &caps, sizeof(caps));
        if (result == 0 && caps.wTechnology == AUXCAPS_CDAUDIO) {
            aux_device = i;
            break;
        }
    }

    wave_volume = GetWaveVolume();
    aux_volume = GetAuxVolume();
}
