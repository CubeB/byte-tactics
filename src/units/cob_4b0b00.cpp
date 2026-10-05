// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Index-taking twin of 0x4b0a70 (the name lookup version): StartThread claims
// one of the 8 channel slots, this stores the value plus a four-deep call
// frame on the slot's own stack, truncates the slot's stack pointer to
// param_4 - 1 and refreshes the channels when asked. Compare 0x4b0a10 (same
// shape without the frame) and 0x4b0c40.
//
// The channel array is viewed from `this + i * 0xa4` because that is the base
// register the original uses (`lea eax, [edi + edx*4]`) with the fields at
// +0x24 (stack pointer, starts at -1), +0x3c (value) and +0x40 (stack). The
// real 0x1c-byte channel header sits 0x1c before this view, which is why the
// array is declared at offset 0 here; the offsets are what the code reads.

struct Channel_004b0b00 {              // 0xa4 bytes, indexed from this + i*0xa4
    char unknown_0[0x24];
    int sp;                            // +0x24, starts at -1 (see 0x4b08c0)
    char unknown_28[0x14];
    int value;                         // +0x3c
    int stack[0x19];                   // +0x40
};

class ScriptCallback;

class Class_004b0b00 {
public:
    Channel_004b0b00 channels[8];      // +0x0 (see the note above)
    char unknown_520[0x1c];
    int activeCount;                   // +0x53c

    int StartScriptWithArgsByIndex(int index, ScriptCallback* param_2, int param_3,
                     int param_4, int param_5, int param_6, int param_7,
                     int param_8);
};

class ScriptCallback {
public:
    virtual void Complete(int result);
};

class CobScript {
public:
    int StartThread(int id);
};

class Class_004b0da0 {
public:
    void RunThread(int channel, int param_2);
};

class Class_004b1c00 {
public:
    void AnimatePieces(int param_1);
};

// FUNCTION: 0x4b0b00
int Class_004b0b00::StartScriptWithArgsByIndex(int index, ScriptCallback* param_2,
                                 int param_3, int param_4, int param_5,
                                 int param_6, int param_7, int param_8)
{
    int i = ((CobScript*)this)->StartThread(index);
    if (i < 0) {
        if (param_2)
            param_2->Complete(0);
        return 0;
    }
    Channel_004b0b00* c = &channels[i];
    c->value = (int)param_2;
    c->stack[++c->sp] = param_5;
    c->stack[++c->sp] = param_6;
    c->stack[++c->sp] = param_7;
    c->stack[++c->sp] = param_8;
    // Spelled through channels[i] (not c) so MSVC keeps the fourth push's
    // increment instead of folding it into the array store.
    channels[i].sp = param_4 - 1;
    if (param_3) {
        if (activeCount) {
            for (int j = 0; j < 8; j++)
                ((Class_004b0da0*)this)->RunThread(j, 0);
        }
        ((Class_004b1c00*)this)->AnimatePieces(0);
    }
    return 1;
}
