// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
#include <windows.h>
#include <mmsystem.h>
#include <string.h>

extern int g_cdNextTrackTimer;
extern int g_cdFadeTimer;

extern void __stdcall RemoveTimer(int);

class Class_004cda00 {
public:
    void QueryDisc();
};

class Class_004cdb40 {
public:
    void PlayNextTrack();
};

struct CdAudio_004ce030 {
    char unknown_0[0x200];
    int field_200;                     // +0x200
    char unknown_204[0x208 - 0x204];
    int field_208;                     // +0x208
    int field_20c;                     // +0x20c
    char unknown_210[0x284 - 0x210];
    int field_284;                     // +0x284
    char unknown_288[0x28c - 0x288];
    void (*callback)();                // +0x28c
};

extern CdAudio_004ce030* g_cdPlayer;

// FUNCTION: 0x4ce030
void __cdecl HandleCdMessage(int param_1, int param_2, int param_3)
{
    char buf[64];

    switch (param_1) {
    case 0x219: {
        CdAudio_004ce030* obj = g_cdPlayer;
        mciSendStringA("stop cdaudio", 0, 0, 0);
        if (obj->field_200)
            obj->field_208 = 1;
        else
            obj->field_208 = 0;
        obj->field_20c = 0;
        obj->field_284 = 0;
        RemoveTimer(g_cdNextTrackTimer);
        RemoveTimer(g_cdFadeTimer);
        g_cdNextTrackTimer = g_cdFadeTimer = -1;
        if (param_2 == 0x8000) {
            ((Class_004cda00*)g_cdPlayer)->QueryDisc();
            if (g_cdPlayer->callback)
                g_cdPlayer->callback();
        }
        break;
    }
    case 0x3b9:
        if (param_2 == 1 && g_cdPlayer->field_20c == 1) {
            int playing;
            if (mciSendStringA("status cdaudio mode", buf, 0x40, 0) == 0)
                playing = strcmp(buf, "playing") == 0;
            else
                playing = 0;
            if (!playing)
                ((Class_004cdb40*)g_cdPlayer)->PlayNextTrack();
        }
        break;
    }
}
