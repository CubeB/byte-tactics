// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Writes the "filename" string of an object as "filename=<text>;" on its own
// line, indented by the given number of tabs; the reading counterpart is
// 0x4ae4b0. Sibling of 0x4ae380.
#include <stdio.h>
#include <string.h>

struct FileHandle {
    FILE* file;                        // +0x0
    int error;                         // +0x4
};

struct Obj_004ae440 {
    char unknown_0[0xb6];
    char text[0x80];                   // +0xb6
};

unsigned int __stdcall HAPI_WriteFile(FileHandle* param_1, void* param_2, unsigned int param_3);

// FUNCTION: 0x4ae440
void __stdcall FUN_004ae440(Obj_004ae440* obj, FileHandle* out, int indent)
{
    char tab;
    char line[100];
    tab = '\t';
    for (int i = 0; i < indent; i++)
        HAPI_WriteFile(out, &tab, 1);
    sprintf(line, "%s=%s;\n", "filename", obj->text);
    HAPI_WriteFile(out, line, strlen(line));
}
