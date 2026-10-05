// Decompiled by Sonnet. Names are provisional.

class Class_004d02a0 {
public:
    void OpenSample(const char* param1, int param2, int param3, int param4);
};

class Sound {
public:
    void PlaySample(const char* param1, int param2, int param3);
};

// FUNCTION: 0x4d0640
void Sound::PlaySample(const char* param1, int param2, int param3)
{
    ((Class_004d02a0*)this)->OpenSample(param1, 1, param2, param3);
}
