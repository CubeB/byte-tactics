// Decompiled by Haiku. Names are provisional.

class Sound {
public:
    int GetMaxBuffers();
};

// FUNCTION: 0x4cf220
int Sound::GetMaxBuffers()
{
    return *(int*)((char*)this + 0x2c);
}
