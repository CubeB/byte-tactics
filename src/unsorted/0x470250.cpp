// Decompiled by Haiku. Names are provisional.

class Class_00470250 {
public:
    char unknown_0[4];
    int first;
    char unknown_8[4];
    int last;

    int FUN_00470250();
};

// FUNCTION: 0x470250
int Class_00470250::FUN_00470250()
{
    if (!first) {
        return 0;
    }
    return (last - first) >> 2;
}
