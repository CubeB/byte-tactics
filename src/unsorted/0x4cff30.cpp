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
// `mov [reg+0x1c],eax` before `pop edi`. The three registers rotate together
// (esi is always the loop counter): probing shows that removing a wave loop
// that reads `this` rotates the phase to `mov ebp,ecx`, and loading the counts
// into outer locals rotates it far enough to reach `mov ebx,ecx`, but every
// shape that reaches ebx drops the in-loop reload of `[this+0x10]` that the
// original has. Nothing else reachable from the file moves it: the getters as
// member/static/free/inline/out-of-line, value/int&/Class&/Class* parameters
// (each gives one of the three rotations but never the right one), casts to
// the sibling classes 0x4cfff0/0x4d0040, a base class, virtuals, early-return
// range checks, explicit outer checks, do/while vs for, the full
// Class_004cee50 layout plus its real constructor as a preceding neighbour,
// definition order, tools/headers.py (128 and 768 sets), int/void* parameters,
// explicit FARPROC locals and the rtm toolchain all give the same result.
// Like 0x4732e0.cpp this is compiler state from the original translation unit,
// reachable only after the regroup-into-TUs phase.
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
