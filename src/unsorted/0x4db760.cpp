// Decompiled by deepseek-v4.1-flash. Names are provisional.

// A lazily initialised function-local static command-line switch object;
// the function returns its state.

class Class_004d9fe0 {
public:
    char on;                           // +0x0
    Class_004d9fe0(char* name, int a, char def, char* onSwitch,
                   char* offSwitch, char* onSwitch2, char* offSwitch2);
};

// FUNCTION: 0x4db760
char FUN_004db760(void)
{
    static Class_004d9fe0 DAT_00528a20("backalign", 1, 1, 0, "-memfrontalign", 0, 0);
    return DAT_00528a20.on;
}
