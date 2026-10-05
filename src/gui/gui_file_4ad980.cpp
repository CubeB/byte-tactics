// Decompiled by Opus. Names are provisional.
// Writes the "status" field of an object as "status=<n>;" on its own line,
// indented by the given number of tabs.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct FileHandle {
    FILE* file;                        // +0x0
    int error;                         // +0x4
};

struct Struct_004ad980 {
    char unknown_0[0xb6];
    short status;                      // +0xb6
};

unsigned int __stdcall HAPI_WriteFile(FileHandle* param_1, void* param_2, unsigned int param_3);

// FUNCTION: 0x4ad980
void __stdcall WriteStatusField(Struct_004ad980* obj, FileHandle* out, int indent)
{
    char tab;
    char line[100];
    char num[100];
    char* value = _itoa(obj->status, num, 10);
    tab = '\t';
    for (int i = 0; i < indent; i++)
        HAPI_WriteFile(out, &tab, 1);
    sprintf(line, "%s=%s;\n", "status", value);
    HAPI_WriteFile(out, line, strlen(line));
}
