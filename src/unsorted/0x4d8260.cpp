// Decompiled by Opus. Names are provisional.
// The "-memset" fill value, a hex or decimal command-line setting that
// defaults to 0xdeadbeef (see 0x4d8200 for the "setmemory" switch).
class Class_004da040 {
public:
    int value;                         // +0x0
    Class_004da040(char* name, int a, int def, char* sw);
    ~Class_004da040() {}
};

// FUNCTION: 0x4d8260
int FUN_004d8260()
{
    static Class_004da040 setValue("setvalue", 0xdeadbeef, 0xdeadbeef, "-memset");
    return setValue.value;
}
