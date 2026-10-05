// Decompiled by Opus. Names are provisional.
// Clamps a time in milliseconds to 4000..60000 and stores it converted to
// 30 Hz ticks, rounded up (compare 0x4628a0).

class Class_00462860 {
public:
    char unknown_0[0x18];
    unsigned int ticks;                // +0x18

    void SetMinRetainMs(unsigned int ms);
};

// FUNCTION: 0x462860
void Class_00462860::SetMinRetainMs(unsigned int ms)
{
    if (ms > 60000)
        ms = 60000;
    else if (ms < 4000)
        ms = 4000;
    ticks = (ms * 30 + 999) / 1000;
}
