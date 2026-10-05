// Decompiled by Opus. Names are provisional.
// Writes the "hotornot" flag of an object as "hotornot=<n>;" on its own
// line, indented by the given number of tabs; the reading counterpart is
// 0x4ae410.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct FileHandle {
    FILE* file;                        // +0x0
    int error;                         // +0x4
};

struct Obj_004ae380 {
    char unknown_0[0xc8];
    unsigned int hotornot : 1;         // +0xc8 bit 0
};

unsigned int __stdcall HAPI_WriteFile(FileHandle* param_1, void* param_2, unsigned int param_3);

// FUNCTION: 0x4ae380
void __stdcall WriteHotOrNotField(Obj_004ae380* obj, FileHandle* out, int indent)
{
    char tab;
    char line[100];
    char num[100];
    char* value = _itoa(obj->hotornot, num, 10);
    tab = '\t';
    for (int i = 0; i < indent; i++)
        HAPI_WriteFile(out, &tab, 1);
    sprintf(line, "%s=%s;\n", "hotornot", value);
    HAPI_WriteFile(out, line, strlen(line));
}
