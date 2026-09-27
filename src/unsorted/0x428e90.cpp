// Decompiled by Space Bunny Free. Names are provisional.
// Pushes back the current token, or reports a parse error if the parser
// wanted an integer and got something else; otherwise returns atoi of the
// text buffer at offset 0.
#include <stdio.h>
#include <stdlib.h>

class Class_00428d10 {
public:
    int FUN_00428d10();
};

class Class_004356c0 {
public:
    int FUN_004356c0(int param_1);
};

#pragma pack(push, 1)
struct Game_00428e90 {
    char unknown_0[0x391e9];
    Class_004356c0* field_391e9;       // +0x391e9
};
#pragma pack(pop)

extern Game_00428e90* g_game;

class Class_00428c90 {
public:
    char flag;                          // +0
    char unknown_1[0x7f];
    char* field_80;                     // +0x80
    char* field_84;                     // +0x84
    char* field_88;                     // +0x88
    int field_8c;                       // +0x8c
    int field_90;                       // +0x90
    int field_94;                       // +0x94

    int FUN_00428e90();
};

// FUNCTION: 0x428e90
int Class_00428c90::FUN_00428e90()
{
    char buffer[256];
    int token;
    if (field_8c != 0) {
        token = 0x102;
    } else if (field_90 != 0) {
        token = field_94;
        field_90 = 0;
    } else {
        token = ((Class_00428d10*)this)->FUN_00428d10();
        field_94 = token;
    }
    if (token != 0x101) {
        if (field_8c == 0) {
            sprintf(buffer, "parse error reading AI profile %s\n%s\nlast string =",
                    (char*)g_game->field_391e9->FUN_004356c0(7),
                    "expecting int", this);
        }
        field_8c = 1;
        return 0;
    }
    return atoi((char*)this);
}
