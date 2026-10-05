// Decompiled by DeepSeek V4.1 Flash. Names are provisional.

struct Base_004d8ae0 {
    char unknown_0[0x8c];
};

class RunningStats : public Base_004d8ae0 {
public:
    int count;                       // 0x8c
    unsigned int min;                // 0x90
    unsigned int max;                // 0x94
    char name[0x20];                 // 0x98
    unsigned __int64 total;          // 0xb8

    void FUN_004d8b30(const char* param_1);

    RunningStats(const Base_004d8ae0& src, const char* name);
};

// FUNCTION: 0x4d8ae0
RunningStats::RunningStats(const Base_004d8ae0& src, const char* name)
    : Base_004d8ae0(src), count(0), min(0), max(0), total(0)
{
    FUN_004d8b30(name);
}
