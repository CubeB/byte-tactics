// Decompiled by Opus. Names are provisional.
// Returns a function-local static Class_004e0570; the empty inline destructor
// makes MSVC register the empty atexit thunk FUN_004e1450.

class Class_004e0570 {
public:
    char unknown_0[0x7c];
    Class_004e0570();
    ~Class_004e0570() {}
};

// FUNCTION: 0x4e1410
Class_004e0570* FUN_004e1410()
{
    static Class_004e0570 timer;
    return &timer;
}
