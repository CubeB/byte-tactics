// Decompiled by Haiku. Names are provisional.

class Sound {
public:
    int GetCurrentTrack();
};

// FUNCTION: 0x4ce7f0
int Sound::GetCurrentTrack()
{
    return *(int*)((char*)this + 0x208);
}
