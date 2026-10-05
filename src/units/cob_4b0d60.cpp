// Decompiled by Opus. Names are provisional.
// Updates every channel with the value (compare the tail of 0x4b0a10).

class CobScript {
public:
    char unknown_0[0x53c];
    int activeCount;                   // +0x53c

    void RunScripts(int param_1);
    void RunThread(int channel, int param_2);
    void AnimatePieces(int param_1);
};

// FUNCTION: 0x4b0d60
void CobScript::RunScripts(int param_1)
{
    if (activeCount) {
        for (int j = 0; j < 8; j++)
            ((CobScript*)this)->RunThread(j, param_1);
    }
    ((CobScript*)this)->AnimatePieces(param_1);
}
