// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Sound object (see 0x4cf8a0): releases finished buffers, then creates a new
// buffer set in the first free slot and starts it; frees the set again if
// starting fails. Returns 1 on success.
#include <windows.h>
#include <dsound.h>

class Class_004cf0b0 {
public:
    void ReapFinishedBuffers();
};

class Class_004cf230 {
public:
    IDirectSoundBuffer** CreateSampleFromMemory(int a, int b, int c, int d, int e);
};

class Class_004cf4d0 {
public:
    void ReleaseSampleSet(IDirectSoundBuffer** set);
};

class Class_004cf570 {
public:
    int PlaySampleSet(IDirectSoundBuffer** set, int a, int b);
};

class Class_004cf800 {
public:
    char unknown_0[0x1c4];
    IDirectSoundBuffer** sets[8];           // +0x1c4

    int PlayMemorySample(int a, int b, int c, int d, int e, int f, int g);
};

// FUNCTION: 0x4cf800
int Class_004cf800::PlayMemorySample(int a, int b, int c, int d, int e, int f, int g)
{
    ((Class_004cf0b0*)this)->ReapFinishedBuffers();
    for (int i = 0; i < 8; i++) {
        if (sets[i] == 0) {
            sets[i] = ((Class_004cf230*)this)->CreateSampleFromMemory(a, b, c, d, e);
            if (sets[i] == 0)
                return 0;
            if (((Class_004cf570*)this)->PlaySampleSet(sets[i], f, g) == 0) {
                ((Class_004cf4d0*)this)->ReleaseSampleSet(sets[i]);
                sets[i] = 0;
                return 0;
            }
            return 1;
        }
    }
    return 0;
}
