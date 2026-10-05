// Decompiled by Opus. Names are provisional.

struct Sound_004ce5e0 {
    char unknown_0[0x278];
    int field_278;                     // +0x278
    char unknown_27c[0x284 - 0x27c];
    int step;                          // +0x284
};

class Class_004d00d0 {
public:
    void SetAuxVolume(int level, int flag);
};

class Class_004cdb40 {
public:
    void PlayNextTrack();
};

extern Sound_004ce5e0* g_cdPlayer;
extern int g_cdFadeVolume;
extern int g_cdFadeTimer;
extern int g_cdNextTrackTimer;

void __stdcall RemoveTimer(int param_1);
int __stdcall AddTimer(int delay, int param, void (__stdcall* callback)(void*));
void __stdcall OnNextTrackTimer(void*);

// Timer callback: steps the level by the object's step; once it reaches zero
// the timer is killed and either a new one is started or PlayNextTrack runs.
// FUNCTION: 0x4ce5e0
void __stdcall OnCdFadeTimer(void*)
{
    g_cdFadeVolume += g_cdPlayer->step;
    if (g_cdFadeVolume <= 0) {
        RemoveTimer(g_cdFadeTimer);
        g_cdFadeTimer = -1;
        g_cdFadeVolume = 0;
        g_cdPlayer->step = 0;
        ((Class_004d00d0*)g_cdPlayer)->SetAuxVolume(g_cdFadeVolume, 1);
        if (g_cdPlayer->field_278 == 0)
            g_cdNextTrackTimer = AddTimer(0x78, 0, OnNextTrackTimer);
        else
            ((Class_004cdb40*)g_cdPlayer)->PlayNextTrack();
    } else {
        ((Class_004d00d0*)g_cdPlayer)->SetAuxVolume(g_cdFadeVolume, 1);
    }
}
