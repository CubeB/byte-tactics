// Decompiled by Opus. Names are provisional.
// Calls PlaySampleSet on the same object with the global flag at 0x51ff48 set
// for the duration of the call.

class Class_004cf570 {
public:
    void PlaySampleSet(int a, int b, int c);
};

class Class_004cf540 {
public:
    void PlayLooping(int a, int b);
};

extern int g_playBufferLooping;

// FUNCTION: 0x4cf540
void Class_004cf540::PlayLooping(int a, int b)
{
    g_playBufferLooping = 1;
    ((Class_004cf570*)this)->PlaySampleSet(a, b, 0);
    g_playBufferLooping = 0;
}
