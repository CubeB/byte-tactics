// FrameTimers: the game's frame-time profile (Thaldren's FrameTimers), 0x50
// bytes, embedded at g_game+0x38d85. last is the tick of the last sample, and
// each of the nine phases has its running total in values and its accumulator
// in acc; AccumulateProfileTime adds the time since last to one phase.
// info_panel.cpp defines the method, and the files that call it (or take the
// profile by value) include this.
#ifndef FRAME_TIMERS_H
#define FRAME_TIMERS_H

struct FrameTimers {
    unsigned long last;                // +0x0
    int total;                         // +0x4
    int values[9];                     // +0x8
    int acc[9];                        // +0x2c
    void AccumulateProfileTime(int i);
};

#endif
