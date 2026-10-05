// Decompiled by Space Bunny Free. Names are provisional.
// Sound start-up: reads the two ini switches, falls back to the Windows mixer
// when asked, and allocates the channel table (0x47eee0 walks it).

#include <stddef.h>

class Class_004cef90 {
public:
    int InitDirectSound(int rate, int bits, int channels, int handle);
};

class Class_004ce260 {
public:
    void OpenCdAudio();
};

class Class_004cff20 {
public:
    int HasNoDriver();
};

class SoundParams_0047ed40 {
public:
    char unknown_0[0x40];
    int field_40;
};

struct Game {
    char unknown_0[0xc];
    SoundParams_0047ed40* field_0c;
    Class_004cef90* field_10;
};

#pragma pack(push, 1)
class Sound_0047ed40 {
public:
    char unknown_0[0x99];
    int field_99;
    int field_9d;
    int field_a1;                     // +0xa1
    int field_a5;                     // +0xa5

    Sound_0047ed40()
    {
        field_99 = 0;
        field_9d = 0;
        field_a1 = 0x1e;
        field_a5 = 0x96;
    }
};
#pragma pack(pop)

extern Game* g_game;
extern int g_noDirectSound;          // NoDirectSound
extern int g_useWindowsSound;        // UseWindowsSound
extern Sound_0047ed40* DAT_0051e68c;

extern char DAT_00508ab4[]; // "NoDirectSound"
extern char DAT_00508aa4[]; // "UseWindowsSound"
extern char DAT_00508a78[]; // "Error:  Sound system initialization failed."

unsigned int __stdcall GetPreferenceInt(char* key, int defaultValue);
int IsWindowsSoundAvailable(void);
void __stdcall FatalError(char* text);
void* __cdecl operator new(size_t size);

// The direct sound buffer's result goes into a local: comparing the call
// directly with 0 gives `test eax, eax`, the original compares with the
// zero register instead.
// FUNCTION: 0x47ed40
void InitSound(void)
{
    if (GetPreferenceInt(DAT_00508ab4, 0))
        g_noDirectSound = 1;
    if (GetPreferenceInt(DAT_00508aa4, 0))
        g_useWindowsSound = 1;
    if (g_useWindowsSound) {
        if (!IsWindowsSoundAvailable())
            g_useWindowsSound = 0;
        g_noDirectSound = 1;
    }
    if (!g_noDirectSound) {
        int hr = g_game->field_10->InitDirectSound(0x2b11, 0x10, 2, g_game->field_0c->field_40);
        if (hr == 0) {
            if (((Class_004cff20*)g_game->field_10)->HasNoDriver())
                g_noDirectSound = 1;
            else
                FatalError(DAT_00508a78);
        }
    }
    ((Class_004ce260*)g_game->field_10)->OpenCdAudio();
    DAT_0051e68c = new Sound_0047ed40();
}
