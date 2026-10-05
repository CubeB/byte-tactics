// Decompiled by Haiku. Names are provisional.

class Sound {
public:
    char unknown_0[0x2c];
    int field_2c;

    void SetMaxBuffers(int val);
};

// FUNCTION: 0x4cf210
void Sound::SetMaxBuffers(int val)
{
    field_2c = val;
}
