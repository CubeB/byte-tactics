// Decompiled by Opus. Names are provisional.
// A command-line switch: `on` is set from the default, then forced on or off
// by the switch strings (see 0x4df160 for the "fpufussy" twin).
class Class_004d9fe0 {
public:
    char on;                           // +0x0
    Class_004d9fe0(char* name, int a, char def, char* onSwitch,
                   char* offSwitch, char* onSwitch2, char* offSwitch2);
    ~Class_004d9fe0() {}
};

extern char DAT_005289b4;
extern char DAT_005289b8;

// FUNCTION: 0x4d80d0
char FUN_004d80d0()
{
    DAT_005289b4 = 1;
    static Class_004d9fe0 memFussy("memfussy", 1, DAT_005289b8, "-memfussy", "-memnofussy", "-memfrontalign", 0);
    return memFussy.on;
}
