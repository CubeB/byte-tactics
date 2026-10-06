// Decompiled by DeepSeek V4.1 Flash, Haiku and Opus. Names are provisional.

// The fields are called min and max.
#define NOMINMAX
#include <windows.h>

struct Base_004d8ae0 {
    char unknown_0[0x8c];
};

class RunningStats : public Base_004d8ae0 {
public:
    int count;                         // +0x8c
    unsigned int min;                  // +0x90
    unsigned int max;                  // +0x94
    char name[0x20];                   // +0x98
    unsigned __int64 total;            // +0xb8

    RunningStats(const Base_004d8ae0& src, const char* name);
    void FUN_004d8b30(const char* param_1);
    void FUN_004d8b60(unsigned int value);
};

// FUNCTION: 0x4d8ae0
RunningStats::RunningStats(const Base_004d8ae0& src, const char* name)
    : Base_004d8ae0(src), count(0), min(0), max(0), total(0)
{
    FUN_004d8b30(name);
}

// The original calls this from the constructor rather than inlining it.
#pragma auto_inline(off)
// FUNCTION: 0x4d8b30
void RunningStats::FUN_004d8b30(const char* param_1)
{
    if (param_1 != 0) {
        lstrcpynA(name, param_1, 0x20);
    } else {
        name[0] = 0;
    }
}
#pragma auto_inline(on)

// Adds a sample to running statistics: count, minimum, maximum and a 64-bit
// total.
// FUNCTION: 0x4d8b60
void RunningStats::FUN_004d8b60(unsigned int value)
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
