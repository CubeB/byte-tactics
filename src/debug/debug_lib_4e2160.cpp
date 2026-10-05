// Decompiled by Haiku. Names are provisional.

extern double GetTimeSeconds();

class Timer {
public:
    double value;
    char unknown_8[0x40];
    char flag_48;

    void ResumeTimer();
};

// FUNCTION: 0x4e2160
void Timer::ResumeTimer()
{
    if (flag_48 != 0) {
        double fVar1 = GetTimeSeconds();
        value = fVar1 - value;
        flag_48 = 0;
    }
}
