// Decompiled by Opus. Names are provisional.
// Reads the "TRACKNUM" setting from the settings block at g_game+0x519;
// when it differs from the current track number (+0x208 of the object at
// g_game+0x10) it records the current one in DAT_00512fe0, calls
// FUN_0045c3f0 and marks the settings block for saving (FUN_0049fa90).
#include <stdlib.h>

struct Class_0049fa90;

class Class_004ce7f0 {
public:
    int GetCurrentTrack();
};

struct Game {
    char unknown_0[0x10];
    Class_004ce7f0* x10;               // +0x10
    char unknown_14[0x519 - 0x14];
    char settings[1];                  // +0x519
};

extern Game* g_game;
extern int DAT_00512fe0;

int __stdcall GetGadgetText(void* settings, const char* key, char* out);
void __cdecl FUN_0045c3f0();
void __stdcall FUN_0049fa90(Class_0049fa90* obj);

// FUNCTION: 0x45d0c0
void FUN_0045d0c0()
{
    char value[20];
    GetGadgetText(g_game->settings, "TRACKNUM", value);
    int track = atoi(value);
    if (track != g_game->x10->GetCurrentTrack()) {
        DAT_00512fe0 = g_game->x10->GetCurrentTrack();
        FUN_0045c3f0();
        FUN_0049fa90((Class_0049fa90*)g_game->settings);
    }
}
