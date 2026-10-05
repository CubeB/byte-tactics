// Decompiled by space-bunny-free. Names are provisional.
// Dumps the playback statistics smackw32.dll collected for the open movie into
// stats.txt, one per line. The frame rate and the playback time are worked out
// in integer arithmetic from the millisecond totals the DLL recorded, and the
// playback time comes out as 1000 for every movie, the original's arithmetic.
#include <stdio.h>
#include <windows.h>

// The 0x54 byte statistics block SmackSummary fills in. Six of the twenty-one
// fields are never printed, and the seven printed before TotalBlitTime are
// read from further in than the DLL writes.
struct SmkStats_0047c530 {
    unsigned int totalTime;            // +0x00 divisor of the frame rate
    unsigned int unknown_04;           // +0x04
    unsigned int openTime;             // +0x08 "Time to Open File"
    unsigned int frames;               // +0x0c "Total Frames Played"
    unsigned int framesSkipped;        // +0x10 "SkippedFrames"
    unsigned int unknown_14;           // +0x14
    unsigned int timeBlit;             // +0x18 "TotalBlitTime"
    unsigned int readTime;             // +0x1c "TotalReadTime"
    unsigned int decompTime;           // +0x20 "TotalDecompTime"
    unsigned int unknown_24;           // +0x24
    unsigned int readSpeed;            // +0x28 "TotalReadSpeed"
    unsigned int slowestFrameTime;     // +0x2c "SlowestFrameTime"
    unsigned int slowest2FrameTime;    // +0x30 "Slowest2FrameTime"
    unsigned int unknown_34;           // +0x34
    unsigned int unknown_38;           // +0x38
    unsigned int averageFrameSize;     // +0x3c "AverageFrameSize"
    unsigned int highest1SecRate;      // +0x40 "Highest1SecRate"
    unsigned int unknown_44;           // +0x44
    unsigned int highestMemAmount;     // +0x48 "HighestMemAmount"
    unsigned int totalExtraMemory;     // +0x4c "TotalExtraMemory"
    unsigned int highestExtraUsed;     // +0x50 "HighestExtraUsed"
};

// smackw32.dll ordinal 20, called through its import slot.
extern "C" __declspec(dllimport) void __stdcall SmackSummary(void* smk, SmkStats_0047c530* stats);

class Class_0047c6c0 {
public:
    void* smack;                        // +0x00
    int unknown_4;                      // +0x04
    int stopped;                        // +0x08
    HWND hwnd;                          // +0x0c

    void WriteSmackStats();
};

// FUNCTION: 0x47c530
void Class_0047c6c0::WriteSmackStats()
{
    SmkStats_0047c530 stats;
    SmackSummary(smack, &stats);
    FILE* file = fopen("stats.txt", "w");
    if (file) {
        fprintf(file, "Frames Per Sec\t%d\n", 1000 * stats.frames / stats.totalTime);
        fprintf(file, "Total Playback Time\t%d\n", 1000 * stats.totalTime / stats.totalTime);
        fprintf(file, "Time to Open File\t%d\n", stats.openTime);
        fprintf(file, "Total Frames Played\t%d\n", stats.frames);
        fprintf(file, "SkippedFrames\t%d\n", stats.framesSkipped);
        fprintf(file, "TotalBlitTime\t%d\n", stats.timeBlit);
        fprintf(file, "TotalReadTime\t%d\n", stats.readTime);
        fprintf(file, "TotalDecompTime\t%d\n", stats.decompTime);
        fprintf(file, "TotalReadSpeed\t%d bytes/sec\n", stats.readSpeed);
        fprintf(file, "SlowestFrameTime\t%d\n", stats.slowestFrameTime);
        fprintf(file, "Slowest2FrameTime\t%d\n", stats.slowest2FrameTime);
        fprintf(file, "AverageFrameSize\t%d\n", stats.averageFrameSize);
        fprintf(file, "Highest1SecRate\t%d\n", stats.highest1SecRate);
        fprintf(file, "HighestMemAmount\t%d\n", stats.highestMemAmount);
        fprintf(file, "TotalExtraMemory\t%d\n", stats.totalExtraMemory);
        fprintf(file, "HighestExtraUsed\t%d\n", stats.highestExtraUsed);
        fclose(file);
    }
}
