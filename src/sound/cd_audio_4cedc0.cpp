// Decompiled by Opus. Names are provisional.
#include <windows.h>
#include <mmsystem.h>

extern int g_cdNextTrackTimer;
extern int g_cdFadeTimer;

extern void __stdcall RemoveTimer(int);

class Class_004cedc0 {
public:
    char unknown_0[0x200];
    int unknown_200;                   // +0x200
    char unknown_204[0x208 - 0x204];
    int unknown_208;                   // +0x208
    int unknown_20c;                   // +0x20c
    char unknown_210[0x27c - 0x210];
    int enabled;                       // +0x27c
    char unknown_280[0x284 - 0x280];
    int unknown_284;                   // +0x284

    void EnableCdAudio(int on);
};

// FUNCTION: 0x4cedc0
void Class_004cedc0::EnableCdAudio(int on)
{
    enabled = on;
    if (on == 0) {
        mciSendStringA("stop cdaudio", 0, 0, 0);
        if (unknown_200)
            unknown_208 = 1;
        else
            unknown_208 = 0;
        unknown_20c = 0;
        unknown_284 = 0;
        RemoveTimer(g_cdNextTrackTimer);
        RemoveTimer(g_cdFadeTimer);
        g_cdNextTrackTimer = g_cdFadeTimer = -1;
    }
}
