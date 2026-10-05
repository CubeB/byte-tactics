// Decompiled by Opus. Names are provisional.
// Returns the CD audio position reported by MCI, or 0 on error.
#include <windows.h>
#include <mmsystem.h>
#include <stdlib.h>

// FUNCTION: 0x4ce880
int FUN_004ce880()
{
    char buf[32];
    if (mciSendStringA("status cdaudio position", buf, 32, 0) == 0)
        return atoi(buf);
    return 0;
}
