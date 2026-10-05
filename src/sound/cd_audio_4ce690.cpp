// Decompiled by DeepSeek V4.1 Flash. Names are provisional.

extern int g_cdFadeVolume;
extern int DAT_0051ff20[];
extern int g_cdFadeTimer;
extern int g_cdNextTrackTimer;

extern int __stdcall RemoveTimer(int handle);
extern int __stdcall AddTimer(int delay, int id, void (__stdcall* callback)(void*));
extern void __stdcall OnCdFadeTimer(void* unused);

class Class_004cdb40 {
public:
    void PlayNextTrack();
};

class Class_004d00d0 {
public:
    int SetAuxVolume(int volume, int temporary);
};

class Sound {
public:
    char unknown_0[0x20];
    int field_20;                      // +0x20
    char unknown_24[0x1fc - 0x24];
    int field_1fc;                     // +0x1fc
    char unknown_200[0x208 - 0x200];
    int field_208;                     // +0x208
    char unknown_20c[0x278 - 0x20c];
    int field_278;                     // +0x278
    char unknown_27c[0x284 - 0x27c];
    int field_284;                     // +0x284

    void SetTrackCategory(int mode);
};

// FUNCTION: 0x4ce690
void Sound::SetTrackCategory(int mode)
{
    int old = field_278;
    if (old == mode)
        return;
    if (old >= 0)
        DAT_0051ff20[old] = field_208;
    field_278 = mode;
    if (field_1fc == 4 || mode == 2 || mode == 3) {
        g_cdFadeVolume = field_20;
        if (old == 4) {
            if (g_cdFadeTimer >= 0) {
                RemoveTimer(g_cdFadeTimer);
                g_cdFadeTimer = -1;
            }
            if (g_cdNextTrackTimer >= 0) {
                RemoveTimer(g_cdNextTrackTimer);
                g_cdNextTrackTimer = -1;
            }
            ((Class_004d00d0*)this)->SetAuxVolume(field_20, 0);
            ((Class_004cdb40*)this)->PlayNextTrack();
        } else {
            if (g_cdFadeTimer >= 0) {
                RemoveTimer(g_cdFadeTimer);
                g_cdFadeTimer = -1;
                RemoveTimer(g_cdNextTrackTimer);
                g_cdNextTrackTimer = -1;
                ((Class_004cdb40*)this)->PlayNextTrack();
            } else {
                field_284 = field_20 / -18;
                g_cdFadeTimer = AddTimer(2, 0, OnCdFadeTimer);
            }
        }
    }
}
