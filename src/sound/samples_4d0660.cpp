// Decompiled by Haiku. Names are provisional.

class Class_004d02a0 {
public:
    void OpenSample(char* p1, int p2, int p3, int p4);
};

class Sound {
public:
    void StreamSample(char* a, int b);
};

// FUNCTION: 0x4d0660
void Sound::StreamSample(char* a, int b)
{
    ((Class_004d02a0*)this)->OpenSample(a, 2, b, 0);
}
