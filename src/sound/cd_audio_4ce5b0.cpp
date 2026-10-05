// Decompiled by Haiku. Names are provisional.

extern int g_cdNextTrackTimer;
extern void* g_cdPlayer;

extern void __stdcall RemoveTimer(int);

class Class_004cdb40 {
public:
    void PlayNextTrack();
};

// FUNCTION: 0x4ce5b0
void __stdcall OnNextTrackTimer(void*)
{
    int temp = g_cdNextTrackTimer;
    RemoveTimer(temp);
    g_cdNextTrackTimer = 0xffffffff;
    ((Class_004cdb40*)g_cdPlayer)->PlayNextTrack();
}
