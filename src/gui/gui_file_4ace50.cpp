// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Writes a "common gadget" record out as TDF text: id, assoc, name, xpos,
// ypos, width, height, attribs, colorf, colorb, texturenumber, fontnumber,
// active, commonattribs, help and gaffile, each indented by `indent` tabs.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Class_004bbbe0 {
    FILE* file;                        // +0x0
    int error;                         // +0x4
};

#pragma pack(push, 1)
struct Common_004ad350 {
    unsigned char id;                  // +0x00
    unsigned char assoc;               // +0x01
    char name[0x11];                   // +0x02
    short xpos;                        // +0x13
    short ypos;                        // +0x15
    short width;                       // +0x17
    short height;                      // +0x19
    int attribs;                       // +0x1b
    int colorf;                        // +0x1f
    int colorb;                        // +0x23
    unsigned char texturenumber;       // +0x27
    unsigned char fontnumber;          // +0x28
    unsigned char active;              // +0x29
    unsigned char commonattribs;       // +0x2a
    char unknown_2b[8];                // +0x2b
    char help[0x81];                   // +0x33
    unsigned short gaffile;            // +0xb4
};
#pragma pack(pop)

extern int DAT_0051fba8;

unsigned int __stdcall FUN_004bbbe0(Class_004bbbe0* out, void* buf, unsigned int len);
void __stdcall FUN_004accd0(Class_004bbbe0* out, int indent);
void __stdcall FUN_004acde0(Class_004bbbe0* out, char* name, char* value, int indent);

// FUNCTION: 0x4ace50
void __stdcall FUN_004ace50(Common_004ad350* obj, Class_004bbbe0* out, int indent)
{
    char tab;
    char* value;
    char line[100];
    char num[100];
    int i;

    value = _itoa(obj->id, num, 10);
    tab = '\t';
    for (i = 0; i < indent; i++)
        FUN_004bbbe0(out, &tab, 1);
    sprintf(line, "%s=%s;\n", "id", value);
    FUN_004bbbe0(out, line, strlen(line));

    value = _itoa(obj->assoc, num, 10);
    tab = '\t';
    for (i = 0; i < indent; i++)
        FUN_004bbbe0(out, &tab, 1);
    sprintf(line, "%s=%s;\n", "assoc", value);
    FUN_004bbbe0(out, line, strlen(line));

    strncpy(num, obj->name, 16);
    num[16] = 0;
    tab = '\t';
    for (i = 0; i < indent; i++)
        FUN_004bbbe0(out, &tab, 1);
    sprintf(line, "%s=%s;\n", "name", num);
    FUN_004bbbe0(out, line, strlen(line));

    value = _itoa(obj->xpos, num, 10);
    FUN_004accd0(out, indent);
    sprintf(line, "%s=%s;\n", "xpos", value);
    FUN_004bbbe0(out, line, strlen(line));

    if (obj->id == 0 && DAT_0051fba8 != 0 && obj->ypos >= 0)
        obj->ypos -= 0x1e0;

    value = _itoa(obj->ypos, num, 10);
    FUN_004accd0(out, indent);
    sprintf(line, "%s=%s;\n", "ypos", value);
    FUN_004bbbe0(out, line, strlen(line));

    value = _itoa(obj->width, num, 10);
    FUN_004accd0(out, indent);
    sprintf(line, "%s=%s;\n", "width", value);
    FUN_004bbbe0(out, line, strlen(line));

    value = _itoa(obj->height, num, 10);
    FUN_004accd0(out, indent);
    sprintf(line, "%s=%s;\n", "height", value);
    FUN_004bbbe0(out, line, strlen(line));

    value = _itoa(obj->attribs, num, 10);
    FUN_004accd0(out, indent);
    sprintf(line, "%s=%s;\n", "attribs", value);
    FUN_004bbbe0(out, line, strlen(line));

    value = _itoa(obj->colorf, num, 10);
    FUN_004accd0(out, indent);
    sprintf(line, "%s=%s;\n", "colorf", value);
    FUN_004bbbe0(out, line, strlen(line));

    value = _itoa(obj->colorb, num, 10);
    FUN_004accd0(out, indent);
    sprintf(line, "%s=%s;\n", "colorb", value);
    FUN_004bbbe0(out, line, strlen(line));

    value = _itoa((signed char)obj->texturenumber, num, 10);
    FUN_004accd0(out, indent);
    sprintf(line, "%s=%s;\n", "texturenumber", value);
    FUN_004bbbe0(out, line, strlen(line));

    value = _itoa((signed char)obj->fontnumber, num, 10);
    FUN_004accd0(out, indent);
    sprintf(line, "%s=%s;\n", "fontnumber", value);
    FUN_004bbbe0(out, line, strlen(line));

    value = _itoa((signed char)obj->active, num, 10);
    FUN_004accd0(out, indent);
    sprintf(line, "%s=%s;\n", "active", value);
    FUN_004bbbe0(out, line, strlen(line));

    FUN_004acde0(out, "commonattribs", _itoa((signed char)obj->commonattribs, num, 10), indent);
    FUN_004acde0(out, "help", obj->help, indent);
    FUN_004acde0(out, "gaffile", _itoa(obj->gaffile & 1, num, 10), indent);
}
