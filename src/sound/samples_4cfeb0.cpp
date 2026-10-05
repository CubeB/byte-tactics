// Decompiled by Haiku. Names are provisional.

class Sound {
public:
    char unknown_0[8];
    int field_8;                              // +0x8
    int field_c;                              // +0xc

    void Set3DDistances(int param_1, int param_2);
};

// FUNCTION: 0x4cfeb0
void Sound::Set3DDistances(int param_1, int param_2)
{
    field_8 = param_1;
    field_c = param_2;
}
