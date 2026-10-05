// Decompiled by Opus. Names are provisional.
// StartThread claims one of the 8 channel slots for an id and returns its
// index (or -1); this one then stores a value in the new slot.
struct Channel_004b0a10 {
    int used;                          // +0x0
    int id;                            // +0x4
    int unknown_8;                     // +0x8
    char unknown_c[0x1c - 0xc];
    int unknown_1c;                    // +0x1c
    int value;                         // +0x20
    char unknown_24[0xa4 - 0x24];
};

class CobScript {
public:
    char unknown_0[0x1c];
    Channel_004b0a10 channels[8];      // +0x1c
    int activeCount;                   // +0x53c

    int StartScriptByIndex(int id, int value, int update);
    int StartThread(int id);
    void RunThread(int channel, int param_2);
    void AnimatePieces(int param_1);
};

// FUNCTION: 0x4b0a10
int CobScript::StartScriptByIndex(int id, int value, int update)
{
    int i = ((CobScript*)this)->StartThread(id);
    if (i < 0)
        return 0;
    channels[i].value = value;
    if (update) {
        if (activeCount) {
            for (int j = 0; j < 8; j++)
                ((CobScript*)this)->RunThread(j, 0);
        }
        ((CobScript*)this)->AnimatePieces(0);
    }
    return 1;
}
