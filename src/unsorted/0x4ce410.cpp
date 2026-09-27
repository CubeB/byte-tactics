// Decompiled by Opus. Names are provisional.
// Stops and closes the CD audio device if it is open.
#include <windows.h>
#include <mmsystem.h>

class Class_004ce410 {
public:
    int open;                          // +0x0

    void FUN_004ce410();
};

// FUNCTION: 0x4ce410
void Class_004ce410::FUN_004ce410()
{
    if (open != 0) {
        mciSendStringA("stop cdaudio", 0, 0, 0);
        mciSendStringA("close cdaudio", 0, 0, 0);
        open = 0;
    }
}
