// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Writes the "text" and "link" strings of an object as "text=<text>;" and
// "link=<link>;" lines, each indented by the given number of tabs; the
// writing siblings are 0x4ae4e0/0x4ae440.
#include <stdio.h>
#include <string.h>

struct FileHandle {
    FILE* file;                        // +0x0
    int error;                         // +0x4
};

struct Obj_004ae230 {
    char unknown_0[0xb6];
    char text[0x80];                   // +0xb6
    char link[0x80];                   // +0x136
};

unsigned int __stdcall HAPI_WriteFile(FileHandle* param_1, void* param_2, unsigned int param_3);

static inline void WriteTabs(FileHandle* out, int indent)
{
    char tab = '\t';
    for (int i = 0; i < indent; i++)
        HAPI_WriteFile(out, &tab, 1);
}

// FUNCTION: 0x4ae230
void __stdcall WriteTextLinkFields(Obj_004ae230* obj, FileHandle* out, int indent)
{
    char line[100];
    WriteTabs(out, indent);
    sprintf(line, "%s=%s;\n", "text", obj->text);
    HAPI_WriteFile(out, line, strlen(line));
    WriteTabs(out, indent);
    sprintf(line, "%s=%s;\n", "link", obj->link);
    HAPI_WriteFile(out, line, strlen(line));
}
