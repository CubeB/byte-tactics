// Decompiled by Opus. Names are provisional.
#include <windows.h>
#include <mmsystem.h>

extern int DAT_0050b540;
extern int DAT_0050b544;

extern void __stdcall FUN_004b64d0(int);

class Class_004ced40 {
public:
    char unknown_0[0x200];
    int unknown_200;                   // +0x200
    char unknown_204[0x208 - 0x204];
    int unknown_208;                   // +0x208
    int unknown_20c;                   // +0x20c
    char unknown_210[0x284 - 0x210];
    int unknown_284;                   // +0x284

    int FUN_004ced40();
};

// Stops CD audio playback; returns 1 when the MCI command succeeded.
// FUNCTION: 0x4ced40
int Class_004ced40::FUN_004ced40()
{
    MCIERROR err = mciSendStringA("stop cdaudio", 0, 0, 0);
    if (unknown_200)
        unknown_208 = 1;
    else
        unknown_208 = 0;
    unknown_20c = 0;
    unknown_284 = 0;
    FUN_004b64d0(DAT_0050b540);
    FUN_004b64d0(DAT_0050b544);
    DAT_0050b540 = DAT_0050b544 = -1;
    return err == 0 ? 1 : 0;
}
