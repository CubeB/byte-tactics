// Decompiled by space-bunny-free. Names are provisional.
// Sends the MCI "play cdaudio"/"pause cdaudio" commands for the CD player
// object at g_game+0x10, with the from/to position range and the "notify"
// keyword so MCI posts a message back to the main window. Returns 1 when
// there was nothing to send, otherwise whether MCI accepted the command.
// The mciSendStringA result goes through the `err` local: comparing the call
// itself emits neg/sbb/inc in the epilogue instead of test/sete.
#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct App_004ce910 {
    char unknown_0[0x40];
    HWND hwnd;                         // +0x40
};

extern App_004ce910* g_display;
extern App_004ce910* GetDisplay();

class Class_004ce910 {
public:
    int unknown_0[0x200 / 4];
    int field_200;                     // +0x200
    int unknown_204[2];
    int field_20c;                     // +0x20c
    int unknown_210[(0x27c - 0x210) / 4];
    int field_27c;                     // +0x27c

    int PauseCdAudio(int pause);
};

// FUNCTION: 0x4ce910
int Class_004ce910::PauseCdAudio(int pause)
{
    char cmd[100];
    char buf[200];
    char ret[200];
    int track;

    if (field_27c == 0)
        return 1;
    if (field_20c == 0)
        return 1;

    HWND hwnd = GetDisplay()->hwnd;

    if (pause == 0) {
        sprintf(cmd, "status cdaudio current track");
        mciSendStringA(cmd, ret, 200, hwnd);
        track = atoi(ret);
        sprintf(buf, "play cdaudio");
        if (track < field_200) {
            strcat(buf, " from ");
            sprintf(cmd, "status cdaudio position");
            mciSendStringA(cmd, ret, 200, hwnd);
            strcat(buf, ret);
            sprintf(cmd, "status cdaudio position track %i", track + 1);
            mciSendStringA(cmd, ret, 200, hwnd);
            strcat(buf, " to ");
            strcat(buf, ret);
        }
        strcat(buf, " notify");
        field_20c = 1;
    } else {
        sprintf(buf, "pause cdaudio");
        field_20c = 2;
    }

    MCIERROR err = mciSendStringA(buf, 0, 0, hwnd);
    return err == 0;
}
