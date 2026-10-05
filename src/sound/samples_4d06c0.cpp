// Decompiled by Opus. Names are provisional.
// Remembers a name and a value, then starts a timer whose callback
// (0x4d0680) hands them back to this object.
#include <string.h>

void __stdcall FUN_004d0680(int unused1);
int __stdcall AddTimer(int delay, int param, void (__stdcall* callback)(int));

extern int DAT_0051ff58;
extern char DAT_0051ff60[];

class Class_004d06c0 {
public:
    char unknown_0[0x288];
    int timer;                           // +0x288

    int FUN_004d06c0(char* name, int value, int delay);
};

// FUNCTION: 0x4d06c0
int Class_004d06c0::FUN_004d06c0(char* name, int value, int delay)
{
    strcpy(DAT_0051ff60, name);
    DAT_0051ff58 = value;
    timer = AddTimer(delay, 0, FUN_004d0680);
    return 1;
}
