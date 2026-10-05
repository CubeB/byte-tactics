// The processor's performance counters that the profiler (0x4e1be0) can
// show: the Pentium's (family 5) and the Pentium Pro's. Each entry is the
// event's id as the GDPERF driver takes it (the family in the top byte), its
// name, which counters can count it (bit 0: counter 0, bit 1: counter 1), and
// the id of a second event to show with it, such as the total a miss count is
// a share of.

struct PerfEvent {
    unsigned int id;                // +0x0
    const char* name;               // +0x4
    int counters;                   // +0x8
    unsigned int related;           // +0xc
};

// GLOBAL: 0x50d980
PerfEvent g_pentiumEvents[8] = {
    {0x50000300, "Data Cache Reads", 3, 0},
    {0x50010300, "Data Cache Writes", 3, 0},
    {0x500b0300, "Misaligned Data Memory Reference", 3, 0x50280300},
    {0x50220300, "Floating Point Operations", 3, 0},
    {0x50280300, "Data reads or writes", 3, 0},
    {0x50370100, "Returns Predicted Incorrectly", 3, 0x50370200},
    {0x50370200, "Returns Predicted", 3, 0},
    {0x503f0300, "Clockticks", 3, 0},
};

// GLOBAL: 0x50da00
PerfEvent g_pentiumProEvents[17] = {
    {0x60050300, "Misaligned Data Memory Reference", 3, 0x60430300},
    {0x60100100, "FP Computational Op.", 1, 0},
    {0x60110200, "FP Microcode Exceptions", 2, 0},
    {0x60120200, "Multiplies", 2, 0},
    {0x60130200, "Divides", 2, 0},
    {0x60140200, "Cycles Divider Busy", 2, 0},
    {0x60290301, "L2 Cache Read Misses", 3, 0x6029030f},
    {0x6029030f, "L2 Cache Reads", 3, 0},
    {0x602a0301, "L2 Cache Write Misses", 3, 0x602a030f},
    {0x602a030f, "L2 Cache Writes", 3, 0},
    {0x60430300, "Data Memory References", 3, 0},
    {0x60790300, "Clockticks", 3, 0},
    {0x60800300, "Total Instruction Fetches", 3, 0},
    {0x60810300, "Total Instruction Fetch Misses", 3, 0},
    {0x60c10100, "FP operations retired", 1, 0},
    {0x60c40300, "Branch Instructions Retired", 3, 0},
    {0x60c50300, "Branch Mispredictions Retired", 3, 0x60c40300},
};
