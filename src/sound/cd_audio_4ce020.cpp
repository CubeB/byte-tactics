// Decompiled by Haiku. Names are provisional.

class Sound {
public:
    int GetPlayState();
};

// FUNCTION: 0x4ce020
int Sound::GetPlayState()
{
    return *(int*)((char*)this + 0x20c);
}
