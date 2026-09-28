// Decompiled by LongCat 2.5 Preview Free. Names are provisional.
// Writes the "status", "text", "quickkey", "grayedout" and "stages" fields
// of an object as separate "<name>=<value>;\n" lines, each indented by the
// given number of tabs.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Class_004bbbe0 {
    FILE* file;                        // +0x0
    int error;                         // +0x4
};

#pragma pack(push, 2)
struct Obj_004ada40 {
    char unknown_0[0xb6];
    char text[0x80];                   // +0xb6
    unsigned char stages;              // +0x136
    short status;                      // +0x138
    signed char quickkey;              // +0x13a
    char padding_0x13b;                // +0x13b
    unsigned char grayedout;           // +0x13c
};
#pragma pack(pop)

unsigned int __stdcall FUN_004bbbe0(Class_004bbbe0* param_1, void* param_2, unsigned int param_3);

// FUNCTION: 0x4ada40
void __stdcall FUN_004ada40(Obj_004ada40* obj, Class_004bbbe0* out, int indent)
{
    char tab;
    char line[100];
    char num[100];
    int i;
    char* value;

    value = _itoa(obj->status, num, 10);
    tab = '\t';
    for (i = 0; i < indent; i++)
        FUN_004bbbe0(out, &tab, 1);
    sprintf(line, "%s=%s;\n", "status", value);
    FUN_004bbbe0(out, line, strlen(line));

    tab = '\t';
    for (i = 0; i < indent; i++)
        FUN_004bbbe0(out, &tab, 1);
    sprintf(line, "%s=%s;\n", "text", obj->text);
    FUN_004bbbe0(out, line, strlen(line));

    value = _itoa(obj->quickkey, num, 10);
    tab = '\t';
    for (i = 0; i < indent; i++)
        FUN_004bbbe0(out, &tab, 1);
    sprintf(line, "%s=%s;\n", "quickkey", value);
    FUN_004bbbe0(out, line, strlen(line));

    value = _itoa(obj->grayedout & 1, num, 10);
    tab = '\t';
    for (i = 0; i < indent; i++)
        FUN_004bbbe0(out, &tab, 1);
    sprintf(line, "%s=%s;\n", "grayedout", value);
    FUN_004bbbe0(out, line, strlen(line));

    value = _itoa(obj->stages, num, 10);
    tab = '\t';
    for (i = 0; i < indent; i++)
        FUN_004bbbe0(out, &tab, 1);
    sprintf(line, "%s=%s;\n", "stages", value);
    FUN_004bbbe0(out, line, strlen(line));
}
