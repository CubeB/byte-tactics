// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Returns the length of the given CD audio track as reported by MCI, or 0 on
// error.
#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include <stdlib.h>

// FUNCTION: 0x4ce530
int __stdcall GetTrackLength(int track)
{
    char buf[32];
    char cmd[64];
    sprintf(cmd, "status cdaudio length track %i", track);
    if (mciSendStringA(cmd, buf, 0x20, 0) == 0)
        return atoi(buf);
    return 0;
}
