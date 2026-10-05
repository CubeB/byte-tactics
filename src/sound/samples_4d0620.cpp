// Decompiled by Haiku. Names are provisional.

class Class_004d02a0 {
public:
    void OpenSample(int, int, int, int);
};

class Sound {
public:
    void LoadSample(char*);
};

// FUNCTION: 0x4d0620
void Sound::LoadSample(char* param_1)
{
    ((Class_004d02a0*)this)->OpenSample((int)param_1, 0, 0, 0);
}
