// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free. Names are provisional.
//
// Class_004cff30 is a prefix of Class_004cee50 (the 0x294-byte sound object);
// 0x4cff30 is its "open the devices and cache the volumes" method. The exe's
// 0x4cfff0 (wave volume) and 0x4d0040 (aux volume) are inlined here by /Ob2,
// which is what produces the shared `or eax,-1` store and the two epilogues.
//
// The one thing that finally decided the callee-saved register permutation:
// holding the auxGetDevCapsA RESULT in a named local inside the device loop
//     int r = auxGetDevCapsA(i, &caps, sizeof(caps));
//     if (r == 0 && caps.wTechnology == AUXCAPS_CDAUDIO) { ... }
// rather than comparing the call inline. Inline, MSVC 5 gives the values the
// callee-saved registers in the order (this, aux_count, import address); with
// the named result the order becomes (import address, this, aux_count), which
// is the original's: import in edi, `this` in ebx, count in ebp. It is the
// only one of about twenty forms tried that moves it, and it moves it all the
// way. The same named result in the WAVE loop instead makes things worse
// (75.5% at 189 bytes), so it is specifically the aux loop.
//
// Ruled out by measurement along the way, so nobody repeats them: named temps
// for waveOutGetNumDevs, for the two volume getter results, `AUXCAPSA *pc =
// &caps`, a named `int tech` for caps.wTechnology, `__inline` member
// accessors, `Class_004cff30 *p = this`, source-level local function pointers
// for the dllimports, the aux search in its own inlined member, do/while loops
// and UINT index/count types. All 77.3% or worse.
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
// auxiliary volumes (-1 when unavailable).
// FUNCTION: 0x4cff30
void Class_004cff30::FUN_004cff30()
{
    AUXCAPSA caps;

    wave_devices = waveOutGetNumDevs();
    aux_device = -1;

    int aux_count = auxGetNumDevs();
    for (int i = 0; i < aux_count; i++) {
        int result = auxGetDevCapsA(i, &caps, sizeof(caps));
        if (result == 0 && caps.wTechnology == AUXCAPS_CDAUDIO) {
            aux_device = i;
            break;
        }
    }

    wave_volume = GetWaveVolume();
    aux_volume = GetAuxVolume();
}
