// Decompiled by Opus. Names are provisional.
// Returns 1 when MCI reports the CD audio device as "playing". A method of
// the object at g_game+0x10 (its one caller, 0x497f40, loads ecx from
// there) that never uses `this`.
#include <windows.h>
#include <mmsystem.h>
#include <string.h>

class Class_004ce800 {
public:
    int IsCdPlaying();
};

// FUNCTION: 0x4ce800
int Class_004ce800::IsCdPlaying()
{
    char buf[64];
    if (mciSendStringA("status cdaudio mode", buf, 64, 0) == 0)
        return strcmp(buf, "playing") == 0;
    return 0;
}
