// Decompiled by Opus, Sonnet and Space Bunny Free. Names are provisional.
// The AI profile parser: reads values and keys out of an AI profile buffer.
// Class_00428c90 and Class_00428d10 are two views of one object (the token
// scanner at 0x428d10 is called through `this` at offset 0), so the four
// members are one class.

class Class_00428d10 {
public:
    int FUN_00428d10();
};

#include <stdio.h>
#include <stdlib.h>

class Class_004356c0 {
public:
    int FUN_004356c0(int param_1);
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x391e9];
    Class_004356c0* field_391e9;       // +0x391e9
};
#pragma pack(pop)
extern Game* g_game;

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

    Class_00428c90();
    void FUN_00428c90(char* param_1, int param_2);
    int FUN_00428cd0();
    int FUN_00428e90();
};

// FUNCTION: 0x428c60
Class_00428c90::Class_00428c90()
{
    field_80 = 0;
    field_84 = 0;
    field_88 = 0;
    field_8c = 0;
    field_90 = 0;
    field_94 = 0x102;
    flag = 0;
}

// FUNCTION: 0x428c90
void Class_00428c90::FUN_00428c90(char* param_1, int param_2)
{
    field_80 = param_1;
    field_84 = param_1 + param_2;
    field_88 = param_1;
    field_8c = 0;
    field_90 = 0;
    field_94 = 0x102;
    flag = 0;
}

// FUNCTION: 0x428cd0
int Class_00428c90::FUN_00428cd0()
{
    if (field_8c != 0)
        return 0x102;
    if (field_90 != 0) {
        field_90 = 0;
        return field_94;
    }
    field_94 = ((Class_00428d10*)this)->FUN_00428d10();
    return field_94;
}

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
