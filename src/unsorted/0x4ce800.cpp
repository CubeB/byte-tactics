// Decompiled by Opus. Names are provisional.
// Returns 1 when MCI reports the CD audio device as "playing".
#include <windows.h>
#include <mmsystem.h>
#include <string.h>

// FUNCTION: 0x4ce800
int FUN_004ce800()
{
    char buf[64];
    if (mciSendStringA("status cdaudio mode", buf, 64, 0) == 0)
        return strcmp(buf, "playing") == 0;
    return 0;
}
