// Decompiled by Opus. Names are provisional.

extern double GetTimeSeconds();

// The timer of 0x4e2160..0x4e21c0. This one keeps the last five sample times
// and returns a rate: five samples divided by the time since the oldest one
// (0 when no time has passed), then records `now` as the newest sample.
class Class_004e21f0 {
public:
    double time;           // +0x00
    char unknown_8[0x10];
    double history[5];     // +0x18
    char unknown_40[8];
    char stopped;          // +0x48

    double FUN_004e21f0();
};

// FUNCTION: 0x4e21f0
double Class_004e21f0::FUN_004e21f0()
{
    double now = GetTimeSeconds();
    double rate;
    if (now - history[0] > 0.0)
        rate = 5.0 / (now - history[0]);
    else
        rate = 0.0;
    for (int i = 0; i < 4; i++)
        history[i] = history[i + 1];
    history[4] = now;
    return rate;
}
