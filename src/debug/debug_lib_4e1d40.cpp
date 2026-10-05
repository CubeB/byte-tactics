// Decompiled by Opus. Names are provisional.
// A second constructor of the timer object of 0x4e1d20: starts timing
// (0x4e1d60) with both values given, without the first reading.

class Class_004e1d60 {
public:
    void FUN_004e1d60(int a, int b);
};

class Class_004e1d20 {
public:
    char unknown_0[0x54];

    Class_004e1d20(int a, int b);
};

// FUNCTION: 0x4e1d40
Class_004e1d20::Class_004e1d20(int a, int b)
{
    ((Class_004e1d60*)this)->FUN_004e1d60(a, b);
}
