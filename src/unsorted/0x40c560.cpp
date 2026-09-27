// Decompiled by Haiku. Names are provisional.

class Class_0040c560 {
public:
    char unknown_0[4];
    int first;
    int last;

    int FUN_0040c560();
};

// FUNCTION: 0x40c560
int Class_0040c560::FUN_0040c560()
{
    if (!first) {
        return 0;
    }
    return (last - first) >> 2;
}
