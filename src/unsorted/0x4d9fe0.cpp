// Decompiled by Opus. Names are provisional.
// A command-line switch: `on` is set from the default, then forced on by
// either on-switch and off by either off-switch (off wins).

char* __cdecl FUN_004d9f60(char* name);

class Class_004d9fe0 {
public:
    char on;                           // +0x0
    Class_004d9fe0(char* name, int a, char def, char* onSwitch,
                   char* offSwitch, char* onSwitch2, char* offSwitch2);
};

// FUNCTION: 0x4d9fe0
Class_004d9fe0::Class_004d9fe0(char* name, int a, char def, char* onSwitch,
                               char* offSwitch, char* onSwitch2, char* offSwitch2)
{
    on = def;
    if (FUN_004d9f60(onSwitch) || FUN_004d9f60(onSwitch2))
        on = 1;
    if (FUN_004d9f60(offSwitch) || FUN_004d9f60(offSwitch2))
        on = 0;
}
