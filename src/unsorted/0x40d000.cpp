// Decompiled by Haiku. Names are provisional.

class Class_0040d000 {
public:
    char unknown_0[4];
    int first;
    int last;

    int FUN_0040d000();
};

// FUNCTION: 0x40d000
int Class_0040d000::FUN_0040d000()
{
    if (!first) {
        return 0;
    }
    return (last - first) >> 1;
}
