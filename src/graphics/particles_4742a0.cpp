// Decompiled by Opus. Names are provisional.

class Class_004742a0 {
public:
    char unknown_0[0x38];
    int field_38;                      // +0x38

    int IsExpired(int value);
};

// FUNCTION: 0x4742a0
int Class_004742a0::IsExpired(int value)
{
    return value > field_38;
}
