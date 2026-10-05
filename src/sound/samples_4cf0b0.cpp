// Decompiled by Opus. Names are provisional.
#include <windows.h>
#include <dsound.h>

class Sound {
public:
    char unknown_0[0x30];
    int count;                              // +0x30
    char unknown_34[4];
    IDirectSoundBuffer* buffers[0x20];      // +0x38
    char unknown_b8[0x1c4 - 0xb8];
    IDirectSoundBuffer** sets[8];           // +0x1c4
    int field_1e4;                          // +0x1e4

    void ReapFinishedBuffers();
    void ReleaseSampleSet(IDirectSoundBuffer** set);
    void UpdateStream();
};

// Releases every sound buffer (and buffer set) that has stopped playing.
// FUNCTION: 0x4cf0b0
void Sound::ReapFinishedBuffers()
{
    DWORD status;
    int i;
    for (i = 0; i < 8; i++) {
        if (sets[i] != 0) {
            if (sets[i][0]->GetStatus(&status) != 0 || status == 0) {
                ((Sound*)this)->ReleaseSampleSet(sets[i]);
                sets[i] = 0;
            }
        }
    }
    for (i = 0; i < 0x20; i++) {
        if (buffers[i] != 0) {
            if (buffers[i]->GetStatus(&status) != 0 || status == 0) {
                buffers[i] = 0;
                count--;
            }
        }
    }
    if (field_1e4 != 0)
        ((Sound*)this)->UpdateStream();
}
