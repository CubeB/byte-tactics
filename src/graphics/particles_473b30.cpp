// Decompiled by Opus. Names are provisional.

class Class_00473b30 {
public:
    char unknown_0[0x2c];
    int field_2c;                      // +0x2c

    int IsExpired(int value);
};

// FUNCTION: 0x473b30
int Class_00473b30::IsExpired(int value)
{
    return value > field_2c;
}
