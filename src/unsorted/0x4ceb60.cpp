// Decompiled by space-bunny-free. Names are provisional.
// Sends one MCI "play cdaudio" command for the CD player object at +0 of the
// sound class: "status cdaudio mode" first, and if the drive is already
// playing the same track (which is remembered in the field at +0x208) the
// command is skipped and 1 is returned. A seek of 0 stops the CD through
// FUN_004cdb40 and returns 1. Otherwise the position is offset by the field
// at +0x280, the CD volume is set for the duration, the time format is
// switched to tmsf, "play cdaudio from %i" is built (with " to %i" when the
// position is inside the last track) plus " notify" for the main window, and
// the time format is switched back to milliseconds. Returns whether the
// mciSendStringA of the play command succeeded.
// The mode test is a conditional expression, not `&&`: `a && b` short-circuits
// into one shared exit block, while `a ? b == 0 : 0` keeps the strcmp's two
// exits, each with its own test/sete pair, and lands on the mci result in eax.
// The 3 text buffers (20, 64 and 200 bytes) plus the four 4-byte homes of
// same, hwnd, err and this give the 0x11c frame.
// " notify" is a separate strcat statement, not an argument of the
// mciSendStringA: nesting it moves the push of hwnd ahead of the first strlen
// and turns the second lea into a reuse of edx.
// The mciSendStringA result goes through the `err` local: comparing the call
// itself emits neg/sbb/inc in the epilogue instead of test/sete.
#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include <string.h>

struct App_004b6220 {
    char unknown_0[0x40];
    HWND hwnd;                         // +0x40
};

extern App_004b6220* FUN_004b6220();

class Class_004cdb40 {
public:
    void FUN_004cdb40();
};

class Class_004d00d0 {
public:
    int FUN_004d00d0(int volume, int temporary);
};

class Class_004ceb60 {
public:
    char unknown_0[0x20];
    int field_20;                      // +0x20
    char unknown_24[0x1fc - 0x24];
    int field_1fc;                     // +0x1fc
    int field_200;                     // +0x200 tracks on the disc
    int field_204;                     // +0x204
    int field_208;                     // +0x208 track last asked for
    int field_20c;                     // +0x20c
    int field_210;                     // +0x210
    char arr_214[100];                 // +0x214
    int field_278;                     // +0x278
    int field_27c;                     // +0x27c nonzero when the CD is open
    int field_280;                     // +0x280 start of the disc in ms
    int field_284;                     // +0x284
    char unknown_288[4];
    int field_28c;                     // +0x28c

    int FUN_004ceb60(int index, int flag);
};

// FUNCTION: 0x4ceb60
int Class_004ceb60::FUN_004ceb60(int index, int flag)
{
    char to[20];
    char status[64];
    char cmd[200];
    int same;
    MCIERROR err;
    HWND hwnd;

    if (field_27c == 0)
        return 1;
    field_20c = 1;
    if (index == 0) {
        ((Class_004cdb40*)this)->FUN_004cdb40();
        return 1;
    }
    same = mciSendStringA("status cdaudio mode", status, 0x40, 0) == 0
            ? strcmp(status, "playing") == 0
            : 0;
    if (same && index == field_208)
        return 1;
    field_208 = index;
    index += field_280;
    hwnd = FUN_004b6220()->hwnd;
    ((Class_004d00d0*)this)->FUN_004d00d0(field_20, 1);
    if (mciSendStringA("set cdaudio time format tmsf", 0, 0, 0) != 0)
        return 0;
    sprintf(cmd, "play cdaudio from %i", index);
    if (index < field_200) {
        sprintf(to, " to %i", index + 1);
        strcat(cmd, to);
    }
    strcat(cmd, " notify");
    err = mciSendStringA(cmd, 0, 0, hwnd);
    mciSendStringA("set cdaudio time format milliseconds", 0, 0, 0);
    return err == 0;
}
