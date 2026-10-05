// Decompiled by Haiku. Names are provisional.

struct Timer {
    int field_0;
    int field_4;
    char unknown_8[0x40];
    unsigned char field_48;

    void ResetTimer();
};

// FUNCTION: 0x4e2180
void Timer::ResetTimer()
{
    field_0 = 0;
    field_4 = 0;
    field_48 = 1;
}
