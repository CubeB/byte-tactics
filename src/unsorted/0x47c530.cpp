// Decompiled by space-bunny-free. Names are provisional.
#include <stdio.h>

// Stats the Smacker player fills in on request, 21 dwords, five of which
// this function never prints.
struct Summary_0047c530 {
    unsigned int totalTime;             // +0x00 divisor of the frame rate
    unsigned int playbackCount;         // +0x04 printed as 1000*count/count
    unsigned int openTime;              // +0x08 "Time to Open File"
    unsigned int framesPlayed;          // +0x0c "Total Frames Played"
    unsigned int skippedFrames;         // +0x10
    unsigned int unknown_14;
    unsigned int totalBlitTime;         // +0x18
    unsigned int totalReadTime;         // +0x1c
    unsigned int totalDecompTime;       // +0x20
    unsigned int unknown_24;
    unsigned int readSpeed;             // +0x28
    unsigned int slowestFrameTime;      // +0x2c
    unsigned int slowest2FrameTime;     // +0x30
    unsigned int unknown_34;
    unsigned int unknown_38;
    unsigned int averageFrameSize;      // +0x3c
    unsigned int highest1SecRate;       // +0x40
    unsigned int unknown_44;
    unsigned int highestMemAmount;      // +0x48
    unsigned int totalExtraMemory;      // +0x4c
    unsigned int highestExtraUsed;      // +0x50
};

// smackw32.dll ordinal 20, called through its import slot.
extern "C" __declspec(dllimport) void __stdcall SmackSummary(void* smack, Summary_0047c530* summary, int unused);

class Class_0047c530 {
public:
    void* smack;                        // +0x00

    void FUN_0047c530();
};

// FUNCTION: 0x47c530
void Class_0047c530::FUN_0047c530()
{
    Summary_0047c530 summary;
    int flag;
    SmackSummary(smack, &summary, flag);
    FILE* file = fopen("stats.txt", "w");
    if (file) {
        fprintf(file, "Frames Per Sec\t%d\n", 1000 * summary.framesPlayed / summary.totalTime);
        fprintf(file, "Total Playback Time\t%d\n", 1000 * summary.playbackCount / summary.playbackCount);
        fprintf(file, "Time to Open File\t%d\n", summary.openTime);
        fprintf(file, "Total Frames Played\t%d\n", summary.framesPlayed);
        fprintf(file, "SkippedFrames\t%d\n", summary.skippedFrames);
        fprintf(file, "TotalBlitTime\t%d\n", summary.totalBlitTime);
        fprintf(file, "TotalReadTime\t%d\n", summary.totalReadTime);
        fprintf(file, "TotalDecompTime\t%d\n", summary.totalDecompTime);
        fprintf(file, "TotalReadSpeed\t%d bytes/sec\n", summary.readSpeed);
        fprintf(file, "SlowestFrameTime\t%d\n", summary.slowestFrameTime);
        fprintf(file, "Slowest2FrameTime\t%d\n", summary.slowest2FrameTime);
        fprintf(file, "AverageFrameSize\t%d\n", summary.averageFrameSize);
        fprintf(file, "Highest1SecRate\t%d\n", summary.highest1SecRate);
        fprintf(file, "HighestMemAmount\t%d\n", summary.highestMemAmount);
        fprintf(file, "TotalExtraMemory\t%d\n", summary.totalExtraMemory);
        fprintf(file, "HighestExtraUsed\t%d\n", summary.highestExtraUsed);
        fclose(file);
    }
}
