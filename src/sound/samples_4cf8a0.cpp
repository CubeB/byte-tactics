// Decompiled by Opus. Names are provisional.
// Sound object (see 0x4cf0b0): releases finished buffers, then creates a new
// buffer set in the first free slot and starts it; frees the set again if
// starting fails. Returns 1 on success.
#include <windows.h>
#include <dsound.h>

class Sound {
public:
    char unknown_0[0x1c4];
    IDirectSoundBuffer** sets[8];           // +0x1c4

    int PlayFileSample(int a, int b, int c, int d, int e, int f, int g);
    void ReapFinishedBuffers();
    IDirectSoundBuffer** CreateSampleFromFile(int a, int b, int c, int d, int e);
    void ReleaseSampleSet(IDirectSoundBuffer** set);
    int PlaySampleSet(IDirectSoundBuffer** set, int a, int b);
};

// FUNCTION: 0x4cf8a0
int Sound::PlayFileSample(int a, int b, int c, int d, int e, int f, int g)
{
    ((Sound*)this)->ReapFinishedBuffers();
    for (int i = 0; i < 8; i++) {
        if (sets[i] == 0) {
            sets[i] = ((Sound*)this)->CreateSampleFromFile(a, b, c, d, e);
            if (sets[i] == 0)
                return 0;
            if (((Sound*)this)->PlaySampleSet(sets[i], f, g) == 0) {
                ((Sound*)this)->ReleaseSampleSet(sets[i]);
                sets[i] = 0;
                return 0;
            }
            return 1;
        }
    }
    return 0;
}
