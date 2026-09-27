// Decompiled by Opus. Names are provisional.
// Frees a sound channel: picks the first active buffer whose flag bit 0 is
// clear, then the one of those with the lowest priority, stops it and
// removes it from the table (layout as in 0x4cee50 and 0x4cf0b0).
#include <windows.h>
#include <dsound.h>

class Class_004cf180 {
public:
    char unknown_0[0x30];
    int count;                              // +0x30
    char unknown_34[4];
    IDirectSoundBuffer* buffers[0x20];      // +0x38
    int priority[0x20];                     // +0xb8
    int flags[0x20];                        // +0x138

    void FUN_004cf180();
};

// FUNCTION: 0x4cf180
void Class_004cf180::FUN_004cf180()
{
    int i;
    for (i = 0; i < 0x20; i++) {
        if (buffers[i] != 0 && (flags[i] & 1) == 0)
            break;
    }
    for (int j = i + 1; j < 0x20; j++) {
        if (buffers[j] != 0 && (flags[j] & 1) == 0 && priority[j] < priority[i])
            i = j;
    }
    buffers[i]->Stop();
    buffers[i] = 0;
    count--;
}
