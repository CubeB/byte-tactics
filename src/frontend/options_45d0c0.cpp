// Decompiled by Opus. Names are provisional.
// Reads the "TRACKNUM" setting from the settings block at g_game+0x519;
// when it differs from the current track number (+0x208 of the object at
// g_game+0x10) it records the current one in DAT_00512fe0, calls
// UpdateTrackGadgets and marks the settings block for saving (FUN_0049fa90).
#include <stdlib.h>

struct Dialog;

class Sound {
public:
    int GetCurrentTrack();
};

struct Game {
    char unknown_0[0x10];
    Sound* x10;                        // +0x10
    char unknown_14[0x519 - 0x14];
    char settings[1];                  // +0x519
};

extern Game* g_game;
extern int DAT_00512fe0;

int __stdcall GetGadgetText(void* settings, const char* key, char* out);
void __cdecl UpdateTrackGadgets();
void __stdcall FUN_0049fa90(Dialog* obj);

// FUNCTION: 0x45d0c0
void FUN_0045d0c0()
{
    char value[20];
    GetGadgetText(g_game->settings, "TRACKNUM", value);
    int track = atoi(value);
    if (track != g_game->x10->GetCurrentTrack()) {
        DAT_00512fe0 = g_game->x10->GetCurrentTrack();
        UpdateTrackGadgets();
        FUN_0049fa90((Dialog*)g_game->settings);
    }
}
