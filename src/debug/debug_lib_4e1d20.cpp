// Decompiled by Opus. Names are provisional.
// A timer object: starts timing (0x4e1d60, which saves and raises the thread
// priority) and discards a first elapsed-time reading (0x4e20a0).

class Class_004e1d60 {
public:
    void FUN_004e1d60(int a, int b);
};

class Class_004e20a0 {
public:
    double FUN_004e20a0();
};

class Class_004e1d20 {
public:
    char unknown_0[0x54];

    Class_004e1d20(int param_1);
};

// FUNCTION: 0x4e1d20
Class_004e1d20::Class_004e1d20(int param_1)
{
    ((Class_004e1d60*)this)->FUN_004e1d60(0, param_1);
    ((Class_004e20a0*)this)->FUN_004e20a0();
}
