// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Writes the "maxchars" and "text" fields of an object as separate
// "<name>=<value>;\n" lines, each indented by the given number of tabs.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct FileHandle {
    FILE* file;                        // +0x0
    int error;                         // +0x4
};

#pragma pack(push, 1)
struct Obj_004ade20 {
    char unknown_0[0xb6];
    char text[0x82];                   // +0xb6
    short maxchars;                    // +0x138
};
#pragma pack(pop)

unsigned int __stdcall HAPI_WriteFile(FileHandle* param_1, void* param_2, unsigned int param_3);

// FUNCTION: 0x4ade20
void __stdcall WriteTextInputFields(Obj_004ade20* obj, FileHandle* out, int indent)
{
    char tab;
    char line[100];
    char num[100];
    int i;
    char* value = _itoa(obj->maxchars, num, 10);
    tab = '\t';
    for (i = 0; i < indent; i++)
        HAPI_WriteFile(out, &tab, 1);
    sprintf(line, "%s=%s;\n", "maxchars", value);
    HAPI_WriteFile(out, line, strlen(line));
    tab = '\t';
    for (i = 0; i < indent; i++)
        HAPI_WriteFile(out, &tab, 1);
    sprintf(line, "%s=%s;\n", "text", obj->text);
    HAPI_WriteFile(out, line, strlen(line));
}
