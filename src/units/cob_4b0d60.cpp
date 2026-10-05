// Decompiled by Opus. Names are provisional.
// Updates every channel with the value (compare the tail of 0x4b0a10).

class Class_004b0d60 {
public:
    char unknown_0[0x53c];
    int activeCount;                   // +0x53c

    void FUN_004b0d60(int param_1);
};

class Class_004b0da0 {
public:
    void FUN_004b0da0(int channel, int param_2);
};

class Class_004b1c00 {
public:
    void FUN_004b1c00(int param_1);
};

// FUNCTION: 0x4b0d60
void Class_004b0d60::FUN_004b0d60(int param_1)
{
    if (activeCount) {
        for (int j = 0; j < 8; j++)
            ((Class_004b0da0*)this)->FUN_004b0da0(j, param_1);
    }
    ((Class_004b1c00*)this)->FUN_004b1c00(param_1);
}
