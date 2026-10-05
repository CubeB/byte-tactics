// Decompiled by Opus. Names are provisional.

// A command-line switch: `on` is set from the default, then forced on or off
// by the switch strings.
class Class_004d9fe0 {
public:
    char on;                           // +0x0
    Class_004d9fe0(char* name, int a, char def, char* onSwitch,
                   char* offSwitch, char* onSwitch2, char* offSwitch2);
    ~Class_004d9fe0() {}
};

char __cdecl FUN_004de180(int param);

extern void* DAT_00528ab4;
extern void* DAT_00528acc;

// True when "imagehlplines" is on, imagehlp loaded, and a line lookup
// function is available.
// FUNCTION: 0x4de4d0
char FUN_004de4d0()
{
    static Class_004d9fe0 lines("imagehlplines", 1, 1, "-enableimagehlplines",
                                "-disableimagehlplines", 0, 0);
    if (lines.on && FUN_004de180(1) && (DAT_00528ab4 || DAT_00528acc))
        return 1;
    return 0;
}
