// Decompiled by LongCat 2.5 Preview Free. Names are provisional.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Class_004bbbe0 {
    FILE* file;                        // +0x0
    int error;                         // +0x4
};

#pragma pack(push, 1)
struct Obj_004ae170 {
    char unknown_0[0xb6];
    char text[0x80];                   // +0xb6
    short range;                       // +0x136
    char unknown_138[4];               // +0x138
    int thick;                         // +0x13c
    short knobpos;                     // +0x140
    short knobsize;                    // +0x142
    int field_144;                     // +0x144
};
#pragma pack(pop)

unsigned int __stdcall FUN_004bbbe0(Class_004bbbe0* param_1, void* param_2, unsigned int param_3);

// FUNCTION: 0x4adf90
void __stdcall FUN_004adf90(Obj_004ae170* obj, Class_004bbbe0* out, int indent)
{
    char tab;
    char line[100];
    char num[100];
    int i;

    char* value = _itoa(obj->range, num, 10);
    tab = '\t';
    for (i = 0; i < indent; i++)
        FUN_004bbbe0(out, &tab, 1);
    sprintf(line, "%s=%s;\n", "range", value);
    FUN_004bbbe0(out, line, strlen(line));

    value = _itoa(obj->thick, num, 10);
    tab = '\t';
    for (i = 0; i < indent; i++)
        FUN_004bbbe0(out, &tab, 1);
    sprintf(line, "%s=%s;\n", "thick", value);
    FUN_004bbbe0(out, line, strlen(line));

    value = _itoa(obj->knobpos, num, 10);
    tab = '\t';
    for (i = 0; i < indent; i++)
        FUN_004bbbe0(out, &tab, 1);
    sprintf(line, "%s=%s;\n", "knobpos", value);
    FUN_004bbbe0(out, line, strlen(line));

    value = _itoa(obj->knobsize, num, 10);
    tab = '\t';
    for (i = 0; i < indent; i++)
        FUN_004bbbe0(out, &tab, 1);
    sprintf(line, "%s=%s;\n", "knobsize", value);
    FUN_004bbbe0(out, line, strlen(line));
}
