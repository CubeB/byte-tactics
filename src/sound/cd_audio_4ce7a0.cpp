// Decompiled by Haiku. Names are provisional.

struct Class_004ce7a0
{
public:
    char unknown_0[0x1fc];
    int field_1fc;

    int SetPlaybackOrder(int value);
};

// FUNCTION: 0x4ce7a0
int Class_004ce7a0::SetPlaybackOrder(int value)
{
    field_1fc = value;
    return 1;
}
