// Decompiled by deepseek-v4.1-flash. Names are provisional.
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

    void FUN_004cff30();
};

// Picks the first CD-audio auxiliary device, then caches the wave-out and
// auxiliary volumes (-1 when unavailable). Both volume getters (the exe's
// 0x4cfff0 and 0x4d0040) are inlined here, which is what produces the shared
// `or eax,-1` store and the two epilogues.
//
// Still differs (77.3%): every instruction matches but for one register
// permutation. The original keeps `this` in ebx, the aux device count in ebp
// and the cached dllimport address in edi; this build keeps `this` in edi, the
// count in ebx and the import address in ebp, and that swap moves the final
// `mov [reg+0x1c],eax` before `pop edi`. Nothing reachable from the file moves
// it: declaring the count at the top, a local object pointer or reference, the
// getters as static/free helpers or out-of-line, a __fastcall object pointer,
// int/void* object parameters, explicit FARPROC locals, the full
// Class_004cee50 layout, definition order, the rtm toolchain, tools/headers.py
// (768 sets) and 0..8000 unused declarations all give the same `mov edi,ecx`.
// Like 0x4732e0.cpp this looks like compiler state from the original
// translation unit, reachable only after the regroup-into-TUs phase.
// FUNCTION: 0x4cff30
void Class_004cff30::FUN_004cff30()
{
    AUXCAPSA caps;

    wave_devices = waveOutGetNumDevs();
    aux_device = -1;

    int aux_count = auxGetNumDevs();
    for (int i = 0; i < aux_count; i++) {
        if (auxGetDevCapsA(i, &caps, sizeof(caps)) == 0 &&
            caps.wTechnology == AUXCAPS_CDAUDIO) {
            aux_device = i;
            break;
        }
    }

    wave_volume = GetWaveVolume();
    aux_volume = GetAuxVolume();
}
