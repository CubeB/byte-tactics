// Decompiled by Sonnet. Names are provisional.

struct Obj {
    char unknown_0[0xf0];
    unsigned short bit0 : 1;
    unsigned short flag : 1;  // +0xf0, mask 2
};

extern Obj* g_display;
extern void __stdcall SetFullScreen(int);

// FUNCTION: 0x4b5910
void ToggleFullScreen()
{
    if (g_display->flag) {
        SetFullScreen(0);
    } else {
        SetFullScreen(1);
    }
}
