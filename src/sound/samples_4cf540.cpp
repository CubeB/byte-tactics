// Decompiled by Opus. Names are provisional.
// Calls PlaySampleSet on the same object with the global flag at 0x51ff48 set
// for the duration of the call.

class Sound {
public:
    void PlayLooping(int a, int b);
    void PlaySampleSet(int a, int b, int c);
};

extern int g_playBufferLooping;

// FUNCTION: 0x4cf540
void Sound::PlayLooping(int a, int b)
{
    g_playBufferLooping = 1;
    ((Sound*)this)->PlaySampleSet(a, b, 0);
    g_playBufferLooping = 0;
}
