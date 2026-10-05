// Decompiled by Opus. Names are provisional.
// Updates every channel with the value (compare the tail of 0x4b0a10).

class CobScript {
public:
    char unknown_0[0x53c];
    int activeCount;                   // +0x53c

    void RunScripts(int param_1);
};

class Class_004b0da0 {
public:
    void RunThread(int channel, int param_2);
};

class Class_004b1c00 {
public:
    void AnimatePieces(int param_1);
};

// FUNCTION: 0x4b0d60
void CobScript::RunScripts(int param_1)
{
    if (activeCount) {
        for (int j = 0; j < 8; j++)
            ((Class_004b0da0*)this)->RunThread(j, param_1);
    }
    ((Class_004b1c00*)this)->AnimatePieces(param_1);
}
