// Decompiled by space-bunny-free. Names are provisional.
// Writes one object's settings as text: the "totalgadgets" count, a
// "[VERSION]" block holding the major/minor/revision bytes, then the "panel",
// "crdefault", "escdefault" and "defaultfocus" strings. Every line is
// preceded by `indent` tabs, except the three lines inside [VERSION], which
// get one more, and the "}" that closes the block, which also gets one more.
//
// `d` is a copy of `indent` that is bumped once, so the [VERSION] body and its
// closing brace sit one level deeper.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct FileHandle {
    FILE* file;                        // +0x0
    int error;                         // +0x4
};

struct Obj_004ad4f0 {
    char unknown_0[0xb6];
    short totalgadgets;                // +0xb6
    char unknown_0xb8[0x11];
    signed char major;                 // +0xc9
    signed char minor;                 // +0xca
    signed char revision;              // +0xcb
    char crdefault[16];                // +0xcc
    char escdefault[16];               // +0xdc
    char defaultfocus[16];             // +0xec
    char panel[16];                    // +0xfc
};

unsigned int __stdcall HAPI_WriteFile(FileHandle* out, void* buf, unsigned int len);
void __stdcall WriteTabs(FileHandle* out, int indent);
void __stdcall WriteKeyValue(FileHandle* out, char* name, char* value, int indent);

// FUNCTION: 0x4ad4f0
void __stdcall WritePanelFields(Obj_004ad4f0* obj, FileHandle* out, int indent)
{
    // All three locals are needed for the frame size.
    char tab;
    char line[100];
    char num[100];
    // Holds the _itoa results: keeps d and i in registers.
    char* value;
    // Copy of indent: the parameter itself must stay unmodified.
    int d = indent;
    int i;

    value = _itoa(obj->totalgadgets, num, 10);
    tab = '\t';
    for (i = 0; i < indent; i++)
        HAPI_WriteFile(out, &tab, 1);
    sprintf(line, "%s=%s;\n", "totalgadgets", value);
    HAPI_WriteFile(out, line, strlen(line));

    d++;
    sprintf(line, "[%s]", "VERSION");
    tab = '\t';
    for (i = 0; i < d - 1; i++)
        HAPI_WriteFile(out, &tab, 1);
    HAPI_WriteFile(out, line, strlen(line));
    HAPI_WriteFile(out, "\n", 1);
    WriteTabs(out, d);
    HAPI_WriteFile(out, "{\n", 2);

    value = _itoa(obj->major, num, 10);
    tab = '\t';
    for (i = 0; i < d; i++)
        HAPI_WriteFile(out, &tab, 1);
    sprintf(line, "%s=%s;\n", "major", value);
    HAPI_WriteFile(out, line, strlen(line));

    value = _itoa(obj->minor, num, 10);
    tab = '\t';
    for (i = 0; i < d; i++)
        HAPI_WriteFile(out, &tab, 1);
    sprintf(line, "%s=%s;\n", "minor", value);
    HAPI_WriteFile(out, line, strlen(line));

    value = _itoa(obj->revision, num, 10);
    tab = '\t';
    for (i = 0; i < d; i++)
        HAPI_WriteFile(out, &tab, 1);
    sprintf(line, "%s=%s;\n", "revision", value);
    HAPI_WriteFile(out, line, strlen(line));

    tab = '\t';
    for (i = 0; i < d; i++)
        HAPI_WriteFile(out, &tab, 1);
    HAPI_WriteFile(out, "}\n", 2);

    strncpy(num, obj->panel, 16);
    num[16] = 0;
    tab = '\t';
    for (i = 0; i < indent; i++)
        HAPI_WriteFile(out, &tab, 1);
    sprintf(line, "%s=%s;\n", "panel", num);
    HAPI_WriteFile(out, line, strlen(line));

    tab = '\t';
    for (i = 0; i < indent; i++)
        HAPI_WriteFile(out, &tab, 1);
    sprintf(line, "%s=%s;\n", "crdefault", obj->crdefault);
    HAPI_WriteFile(out, line, strlen(line));
    WriteTabs(out, indent);

    sprintf(line, "%s=%s;\n", "escdefault", obj->escdefault);
    HAPI_WriteFile(out, line, strlen(line));

    WriteKeyValue(out, "defaultfocus", obj->defaultfocus, indent);
}
