// Decompiled by Opus. Names are provisional.
// Adds a sample to running statistics: count, minimum, maximum and a 64-bit
// total.

class Class_004d8b60 {
public:
    char unknown_0[0x8c];
    int count;                         // +0x8c
    unsigned int min;                  // +0x90
    unsigned int max;                  // +0x94
    char unknown_98[0x20];
    unsigned __int64 total;            // +0xb8

    void FUN_004d8b60(unsigned int value);
};

// FUNCTION: 0x4d8b60
void Class_004d8b60::FUN_004d8b60(unsigned int value)
{
    if (count == 0) {
        min = max = value;
    } else {
        if (value < min)
            min = value;
        if (value > max)
            max = value;
    }
    total += value;
    count++;
}
