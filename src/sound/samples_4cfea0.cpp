// Decompiled by Haiku. Names are provisional.

class Sound {
public:
    int Is3DEnabled();
};

// FUNCTION: 0x4cfea0
int Sound::Is3DEnabled()
{
    return *(int*)((char*)this + 4);
}
