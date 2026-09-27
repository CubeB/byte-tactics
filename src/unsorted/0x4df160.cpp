// Decompiled by Opus. Names are provisional.
#include <float.h>

// A command-line switch: `on` is set from the default, then forced on or off
// by the switch strings. The empty inline destructor is what makes MSVC
// register the (empty) atexit thunk FUN_004df1d0 for the static local.
class Class_004d9fe0 {
public:
    char on;                           // +0x0
    Class_004d9fe0(char* name, int a, char def, char* onSwitch,
                   char* offSwitch, char* onSwitch2, char* offSwitch2);
    ~Class_004d9fe0() {}
};

// FUNCTION: 0x4df160
void FUN_004df160()
{
    static Class_004d9fe0 fussy("fpufussy", 1, 0, "-fpufussy", "-fpunofussy", 0, 0);
    if (fussy.on)
        _controlfp(0, _EM_ZERODIVIDE | _EM_INVALID);
    else
        _controlfp(_EM_ZERODIVIDE | _EM_INVALID, _EM_ZERODIVIDE | _EM_INVALID);
}
