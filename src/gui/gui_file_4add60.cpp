// Decompiled by Opus. Names are provisional.
// Writes the "itemheight" field of an object as "itemheight=<n>;" on its own
// line, indented by the given number of tabs.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Class_004bbbe0 {
    FILE* file;                        // +0x0
    int error;                         // +0x4
};

struct Struct_004add60 {
    char unknown_0[0xda];
    short itemheight;                  // +0xda
};

unsigned int __stdcall FUN_004bbbe0(Class_004bbbe0* param_1, void* param_2, unsigned int param_3);

// FUNCTION: 0x4add60
void __stdcall WriteListBoxFields(Struct_004add60* obj, Class_004bbbe0* out, int indent)
{
    char tab;
    char line[100];
    char num[100];
    char* value = _itoa(obj->itemheight, num, 10);
    tab = '\t';
    for (int i = 0; i < indent; i++)
        FUN_004bbbe0(out, &tab, 1);
    sprintf(line, "%s=%s;\n", "itemheight", value);
    FUN_004bbbe0(out, line, strlen(line));
}
