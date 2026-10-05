// Decompiled by Space Bunny Free. Names are provisional.
// Sound start-up: reads the two ini switches, falls back to the Windows mixer
// when asked, and allocates the channel table (0x47eee0 walks it).

#include <stddef.h>

class Class_004cef90 {
public:
    int FUN_004cef90(int rate, int bits, int channels, int handle);
};

class Class_004ce260 {
public:
    void FUN_004ce260();
};

class Class_004cff20 {
public:
    int FUN_004cff20();
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
extern int DAT_0051e690;             // NoDirectSound
extern int DAT_0051e694;             // UseWindowsSound
extern Sound_0047ed40* DAT_0051e68c;

extern char DAT_00508ab4[]; // "NoDirectSound"
extern char DAT_00508aa4[]; // "UseWindowsSound"
extern char DAT_00508a78[]; // "Error:  Sound system initialization failed."

unsigned int __stdcall FUN_0049f5a0(char* key, int defaultValue);
int FUN_0049f610(void);
void __stdcall FUN_004b6290(char* text);
void* __cdecl operator new(size_t size);

// The direct sound buffer's result goes into a local: comparing the call
// directly with 0 gives `test eax, eax`, the original compares with the
// zero register instead.
// FUNCTION: 0x47ed40
void FUN_0047ed40(void)
{
    if (FUN_0049f5a0(DAT_00508ab4, 0))
        DAT_0051e690 = 1;
    if (FUN_0049f5a0(DAT_00508aa4, 0))
        DAT_0051e694 = 1;
    if (DAT_0051e694) {
        if (!FUN_0049f610())
            DAT_0051e694 = 0;
        DAT_0051e690 = 1;
    }
    if (!DAT_0051e690) {
        int hr = g_game->field_10->FUN_004cef90(0x2b11, 0x10, 2, g_game->field_0c->field_40);
        if (hr == 0) {
            if (((Class_004cff20*)g_game->field_10)->FUN_004cff20())
                DAT_0051e690 = 1;
            else
                FUN_004b6290(DAT_00508a78);
        }
    }
    ((Class_004ce260*)g_game->field_10)->FUN_004ce260();
    DAT_0051e68c = new Sound_0047ed40();
}
