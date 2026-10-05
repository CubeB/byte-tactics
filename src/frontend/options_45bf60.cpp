// Decompiled by Opus. Names are provisional.
// Formats "<label> (<speed>)" for a game-speed setting, then passes the
// original name and label (not the formatted text) to FUN_004a0bf0 on the
// settings block at g_game+0x519.
#include <stdio.h>

extern char* g_game;

struct Object_004a0bf0;

void __stdcall FUN_004a0bf0(Object_004a0bf0* obj, char* name, int param_3, int param_4);

// FUNCTION: 0x45bf60
void __stdcall SetGameSpeedLabel(char* name, char* label, int speed, int normal)
{
    char buf[200];
    char* text;
    if (speed == normal) {
        text = "Normal";
    } else if (speed < normal / 4) {
        text = "Slow";
    } else if (speed < normal / 2) {
        text = "Slower";
    } else if (speed > normal * 3 / 4) {
        text = "Fast";
    } else {
        text = "Faster";
    }
    sprintf(buf, "%s (%s)", label, text);
    FUN_004a0bf0((Object_004a0bf0*)(g_game + 0x519), name, (int)label, 0);
}
