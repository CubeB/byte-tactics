// Decompiled by Opus. Names are provisional.

struct Sound_004ce5e0 {
    char unknown_0[0x278];
    int field_278;                     // +0x278
    char unknown_27c[0x284 - 0x27c];
    int step;                          // +0x284
};

class Class_004d00d0 {
public:
    void FUN_004d00d0(int level, int flag);
};

class Class_004cdb40 {
public:
    void FUN_004cdb40();
};

extern Sound_004ce5e0* DAT_0051ff14;
extern int DAT_0051ff10;
extern int DAT_0050b544;
extern int DAT_0050b540;

void __stdcall RemoveTimer(int param_1);
int __stdcall AddTimer(int delay, int param, void (__stdcall* callback)(void*));
void __stdcall FUN_004ce5b0(void*);

// Timer callback: steps the level by the object's step; once it reaches zero
// the timer is killed and either a new one is started or FUN_004cdb40 runs.
// FUNCTION: 0x4ce5e0
void __stdcall FUN_004ce5e0(void*)
{
    DAT_0051ff10 += DAT_0051ff14->step;
    if (DAT_0051ff10 <= 0) {
        RemoveTimer(DAT_0050b544);
        DAT_0050b544 = -1;
        DAT_0051ff10 = 0;
        DAT_0051ff14->step = 0;
        ((Class_004d00d0*)DAT_0051ff14)->FUN_004d00d0(DAT_0051ff10, 1);
        if (DAT_0051ff14->field_278 == 0)
            DAT_0050b540 = AddTimer(0x78, 0, FUN_004ce5b0);
        else
            ((Class_004cdb40*)DAT_0051ff14)->FUN_004cdb40();
    } else {
        ((Class_004d00d0*)DAT_0051ff14)->FUN_004d00d0(DAT_0051ff10, 1);
    }
}
