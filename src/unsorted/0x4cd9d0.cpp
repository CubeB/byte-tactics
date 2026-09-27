// Decompiled by Opus. Names are provisional.
// Stores a callback, refreshes the CD audio state (0x4cda00) and runs the
// callback.

class Class_004cda00 {
public:
    void FUN_004cda00();
};

class Class_004cd9d0 {
public:
    char unknown_0[0x28c];
    void (*callback)();                // +0x28c

    int FUN_004cd9d0(void (*cb)());
};

// FUNCTION: 0x4cd9d0
int Class_004cd9d0::FUN_004cd9d0(void (*cb)())
{
    callback = cb;
    ((Class_004cda00*)this)->FUN_004cda00();
    if (callback != 0)
        callback();
    return 1;
}
