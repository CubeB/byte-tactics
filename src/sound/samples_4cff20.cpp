// Decompiled by Haiku. Names are provisional.

class Sound {
public:
    int HasNoDriver();
};

// FUNCTION: 0x4cff20
int Sound::HasNoDriver()
{
    return *(int*)((char*)this + 0x290);
}
