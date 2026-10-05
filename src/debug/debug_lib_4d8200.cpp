// Decompiled by Opus. Names are provisional.
// A command-line switch: `on` is set from the default, then forced on or off
// by the switch strings (see 0x4d80d0 for the "memfussy" twin).
class Class_004d9fe0 {
public:
    char on;                           // +0x0
    Class_004d9fe0(char* name, int a, char def, char* onSwitch,
                   char* offSwitch, char* onSwitch2, char* offSwitch2);
    ~Class_004d9fe0() {}
};

// FUNCTION: 0x4d8200
char FUN_004d8200()
{
    static Class_004d9fe0 memSet("setmemory", 1, 0, "-memset", "-memnoset", 0, 0);
    return memSet.on;
}
