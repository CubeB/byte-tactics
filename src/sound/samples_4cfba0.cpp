// Decompiled by Haiku. Names are provisional.

class Sound {
public:
    char unknown_0[0x1e4];
    int field_1e4;
    char unknown_1e8[0xa0];
    int field_288;

    int IsStreamActive(void);
};

// FUNCTION: 0x4cfba0
int Sound::IsStreamActive(void)
{
    if (field_1e4 == 0 && field_288 == -1) {
        return 0;
    }
    return 1;
}
