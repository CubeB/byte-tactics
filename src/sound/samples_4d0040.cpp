// Decompiled by Opus. Names are provisional.
#include <windows.h>
#include <mmsystem.h>

class Class_004d0040 {
public:
    char unknown_0[0x14];
    int aux_device;                    // +0x14, negative when there is none

    int FUN_004d0040();
};

// Returns the aux (CD) volume of the left channel, 0..0xffff, or -1 when
// there is no aux device or its volume cannot be read. The setter is
// 0x4d00d0.
// FUNCTION: 0x4d0040
int Class_004d0040::FUN_004d0040()
{
    DWORD volume;
    if (aux_device >= 0 && auxGetVolume(aux_device, &volume) == 0) {
        return volume & 0xffff;
    }
    return -1;
}
