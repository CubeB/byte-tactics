// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Writes the "filename" field of an object as "filename=<text>;" on its own
// line, indented by the given number of tabs; the reading counterpart is
// 0x4ae4b0.
#include <stdio.h>
#include <string.h>

struct Class_004bbbe0 {
    FILE* file;                        // +0x0
    int error;                         // +0x4
};

struct Obj_004ae4e0 {
    char unknown_0[0xb6];
    char text[0x80];                   // +0xb6
};

unsigned int __stdcall FUN_004bbbe0(Class_004bbbe0* param_1, void* param_2, unsigned int param_3);

// FUNCTION: 0x4ae4e0
void __stdcall FUN_004ae4e0(Obj_004ae4e0* obj, Class_004bbbe0* out, int indent)
{
    char tab;
    char line[100];
    tab = '\t';
    for (int i = 0; i < indent; i++)
        FUN_004bbbe0(out, &tab, 1);
    sprintf(line, "%s=%s;\n", "filename", obj->text);
    FUN_004bbbe0(out, line, strlen(line));
}
