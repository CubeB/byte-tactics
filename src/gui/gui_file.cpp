// Decompiled by Opus, deepseek-v4.1-flash, Space Bunny Free, space-bunny-free, Sonnet, LongCat 2.5 Preview Free, GPT-6, deepseek-v4.1, GPT-6.1-sol and opus. Names are provisional.
//
// The gui_file module: the writers and readers of a GUI file's gadget records
// (0x15b bytes each), in address order, and the GAF loaders for the gadget
// panel's images. Each writer pairs with the reader of the record it wrote.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "../util/tdf.h"

struct FileHandle {
    FILE* file;                        // +0x0
    int error;                         // +0x4
};

extern char DAT_005119b8[];
extern int DAT_0051fba8;

unsigned int __stdcall HAPI_WriteFile(FileHandle* file, void* data, unsigned int size);
void __stdcall WriteTabs(FileHandle* out, int indent);
char* __stdcall ChangeExtension(char* name, char* out, const char* ext);
int __stdcall HAPI_FileLengthByName(char* path);
void __stdcall RemoveFile(char* path);
void __stdcall RenameFile(char* from, char* to);
FileHandle* __stdcall HAPI_CreateFile(char* path);
void __stdcall HAPI_CloseFile(FileHandle* file);
char* __stdcall Translate(char* text);
void* __stdcall LoadGaf(char* path);
void* __stdcall GetGafFrame(void* table, int index);
void __cdecl GameFreeThunk(int* param_1);

// The tab writer at 0x4accd0 (dialogs_4abb20.cpp), inlined where the original
// inlined it. The out-of-line WriteTabs above is the same function called by
// the writers whose original did not inline it.
static inline void WriteTabsInline(FileHandle* file, int depth)
{
    char tab = '\t';
    for (int i = 0; i < depth; i++)
        HAPI_WriteFile(file, &tab, 1);
}

// Writes a section header: "[name]" on its own line indented by depth - 1
// tabs, then "{" indented by depth tabs. The two tab runs are the inlined
// tab writer (0x4accd0), each with its own tab character.
// FUNCTION: 0x4acd00
void __stdcall WriteSectionHeader(FileHandle* file, char* name, int depth)
{
    char line[100];
    sprintf(line, "[%s]", name);
    WriteTabsInline(file, depth - 1);
    HAPI_WriteFile(file, line, strlen(line));
    HAPI_WriteFile(file, "\n", 1);
    WriteTabsInline(file, depth);
    HAPI_WriteFile(file, "{\n", 2);
}

// Writes `depth` tabs and then a closing brace line.
// FUNCTION: 0x4acda0
void __stdcall WriteSectionEnd(FileHandle* file, int depth)
{
    char tab = '\t';
    for (int i = 0; i < depth; i++)
        HAPI_WriteFile(file, &tab, 1);
    HAPI_WriteFile(file, "}\n", 2);
}

// Writes "<key>=<value>;" on its own line, indented by `depth` tabs.
// The original calls this out of line from WriteCommonFields, WritePanelFields
// and WriteGuiFile, so it must not be inlined into them now that all live in
// one file.
#pragma auto_inline(off)
// FUNCTION: 0x4acde0
void __stdcall WriteKeyValue(FileHandle* file, char* key, char* value, int depth)
{
    char tab = '\t';
    char line[100];
    for (int i = 0; i < depth; i++)
        HAPI_WriteFile(file, &tab, 1);
    sprintf(line, "%s=%s;\n", key, value);
    HAPI_WriteFile(file, line, strlen(line));
}
#pragma auto_inline(on)

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

// Writes a "common gadget" record out as TDF text: id, assoc, name, xpos,
// ypos, width, height, attribs, colorf, colorb, texturenumber, fontnumber,
// active, commonattribs, help and gaffile, each indented by `indent` tabs.
// The original calls this out of line from WriteGuiFile, so it must not be
// inlined into it now that both live in one file.
#pragma auto_inline(off)
// FUNCTION: 0x4ace50
void __stdcall WriteCommonFields(Common_004ad350* obj, FileHandle* out, int indent)
{
    char tab;
    char* value;
    char line[100];
    char num[100];
    int i;

    value = _itoa(obj->id, num, 10);
    tab = '\t';
    for (i = 0; i < indent; i++)
        HAPI_WriteFile(out, &tab, 1);
    sprintf(line, "%s=%s;\n", "id", value);
    HAPI_WriteFile(out, line, strlen(line));

    value = _itoa(obj->assoc, num, 10);
    tab = '\t';
    for (i = 0; i < indent; i++)
        HAPI_WriteFile(out, &tab, 1);
    sprintf(line, "%s=%s;\n", "assoc", value);
    HAPI_WriteFile(out, line, strlen(line));

    strncpy(num, obj->name, 16);
    num[16] = 0;
    tab = '\t';
    for (i = 0; i < indent; i++)
        HAPI_WriteFile(out, &tab, 1);
    sprintf(line, "%s=%s;\n", "name", num);
    HAPI_WriteFile(out, line, strlen(line));

    value = _itoa(obj->xpos, num, 10);
    WriteTabs(out, indent);
    sprintf(line, "%s=%s;\n", "xpos", value);
    HAPI_WriteFile(out, line, strlen(line));

    if (obj->id == 0 && DAT_0051fba8 != 0 && obj->ypos >= 0)
        obj->ypos -= 0x1e0;

    value = _itoa(obj->ypos, num, 10);
    WriteTabs(out, indent);
    sprintf(line, "%s=%s;\n", "ypos", value);
    HAPI_WriteFile(out, line, strlen(line));

    value = _itoa(obj->width, num, 10);
    WriteTabs(out, indent);
    sprintf(line, "%s=%s;\n", "width", value);
    HAPI_WriteFile(out, line, strlen(line));

    value = _itoa(obj->height, num, 10);
    WriteTabs(out, indent);
    sprintf(line, "%s=%s;\n", "height", value);
    HAPI_WriteFile(out, line, strlen(line));

    value = _itoa(obj->attribs, num, 10);
    WriteTabs(out, indent);
    sprintf(line, "%s=%s;\n", "attribs", value);
    HAPI_WriteFile(out, line, strlen(line));

    value = _itoa(obj->colorf, num, 10);
    WriteTabs(out, indent);
    sprintf(line, "%s=%s;\n", "colorf", value);
    HAPI_WriteFile(out, line, strlen(line));

    value = _itoa(obj->colorb, num, 10);
    WriteTabs(out, indent);
    sprintf(line, "%s=%s;\n", "colorb", value);
    HAPI_WriteFile(out, line, strlen(line));

    value = _itoa((signed char)obj->texturenumber, num, 10);
    WriteTabs(out, indent);
    sprintf(line, "%s=%s;\n", "texturenumber", value);
    HAPI_WriteFile(out, line, strlen(line));

    value = _itoa((signed char)obj->fontnumber, num, 10);
    WriteTabs(out, indent);
    sprintf(line, "%s=%s;\n", "fontnumber", value);
    HAPI_WriteFile(out, line, strlen(line));

    value = _itoa((signed char)obj->active, num, 10);
    WriteTabs(out, indent);
    sprintf(line, "%s=%s;\n", "active", value);
    HAPI_WriteFile(out, line, strlen(line));

    WriteKeyValue(out, "commonattribs", _itoa((signed char)obj->commonattribs, num, 10), indent);
    WriteKeyValue(out, "help", obj->help, indent);
    WriteKeyValue(out, "gaffile", _itoa(obj->gaffile & 1, num, 10), indent);
}
#pragma auto_inline(on)

struct Tree_004ad350 {
    char unknown_0[4];
    TdfRecord* current;           // +0x4
};

// FUNCTION: 0x4ad350
void __stdcall ReadCommonSection(Common_004ad350* obj, Tree_004ad350* tree)
{
    if (((TdfFile*)tree)->SelectRecord("COMMON") == 1) {
        obj->id = (unsigned char)tree->current->GetFieldInt("id", 0);
        obj->assoc = (unsigned char)tree->current->GetFieldInt("assoc", 0);
        tree->current->GetFieldString(obj->name, "name", 0x10, DAT_005119b8);
        obj->xpos = (short)tree->current->GetFieldInt("xpos", 0);
        obj->ypos = (short)tree->current->GetFieldInt("ypos", 0);
        obj->width = (short)tree->current->GetFieldInt("width", 0);
        obj->height = (short)tree->current->GetFieldInt("height", 0);
        obj->attribs = tree->current->GetFieldInt("attribs", 0);
        obj->colorf = (unsigned short)tree->current->GetFieldInt("colorf", 0);
        obj->colorb = (unsigned short)tree->current->GetFieldInt("colorb", 0);
        obj->texturenumber = (unsigned char)tree->current->GetFieldInt("texturenumber", 0);
        obj->fontnumber = (unsigned char)tree->current->GetFieldInt("fontnumber", 0);
        obj->active = (unsigned char)tree->current->GetFieldInt("active", 0);
        obj->commonattribs = (unsigned char)tree->current->GetFieldInt("commonattribs", 0);
        tree->current->GetFieldString(obj->help, "help", 0x80, DAT_005119b8);
        memset(obj->help, 0, 0x81);
        strncpy(obj->help, Translate(obj->help), 0x80);
        int gf = tree->current->GetFieldInt("gaffile", 0);
        obj->gaffile = (gf ^ obj->gaffile) & 1 ^ obj->gaffile;
    }
}

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

// Writes one object's settings as text: the "totalgadgets" count, a
// "[VERSION]" block holding the major/minor/revision bytes, then the "panel",
// "crdefault", "escdefault" and "defaultfocus" strings. Every line is
// preceded by `indent` tabs, except the three lines inside [VERSION], which
// get one more, and the "}" that closes the block, which also gets one more.
//
// `d` is a copy of `indent` that is bumped once, so the [VERSION] body and its
// closing brace sit one level deeper.
// The original calls this out of line from WriteGuiFile, so it must not be
// inlined into it now that both live in one file.
#pragma auto_inline(off)
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
#pragma auto_inline(on)

struct Source_004ad890 {
    char unknown_0[4];
    TdfRecord* tdf;                    // +0x4
};

#pragma pack(push, 1)
struct Obj_004ad890 {
    char unknown_0[0xb6];
    short field_b6;                    // +0xb6
    char unknown_b8[0x11];             // +0xb8
    char major;                        // +0xc9
    char minor;                        // +0xca
    char revision;                     // +0xcb
    char crdefault[0x10];              // +0xcc
    char escdefault[0x10];             // +0xdc
    char defaultfocus[0x10];           // +0xec
    char panel[0x10];                  // +0xfc
};
#pragma pack(pop)

// FUNCTION: 0x4ad890
void __stdcall ReadPanelFields(Obj_004ad890* obj, Source_004ad890* src)
{
    obj->field_b6 = (short)src->tdf->GetFieldInt("totalgadgets", 0);
    src->tdf->GetFieldString(obj->panel, "panel", 0x10, DAT_005119b8);
    src->tdf->GetFieldString(obj->crdefault, "crdefault", 0x10, DAT_005119b8);
    src->tdf->GetFieldString(obj->escdefault, "escdefault", 0x10, DAT_005119b8);
    src->tdf->GetFieldString(obj->defaultfocus, "defaultfocus", 0x10, DAT_005119b8);
    if (((TdfFile*)src)->SelectRecord("VERSION") == 1) {
        obj->major = (char)src->tdf->GetFieldInt("major", 0);
        obj->minor = (char)src->tdf->GetFieldInt("minor", 0);
        obj->revision = (char)src->tdf->GetFieldInt("revision", 0);
    }
}

struct Struct_004ad980 {
    char unknown_0[0xb6];
    short status;                      // +0xb6
};

// Writes the "status" field of an object as "status=<n>;" on its own line,
// indented by the given number of tabs.
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

struct StructA_004ada10 {
    char unknown_0[0xb6];
    short field_b6;
};

struct StructB_004ada10 {
    char unknown_0[4];
    TdfRecord* field_4;
};

// FUNCTION: 0x4ada10
void __stdcall ReadStatusField(StructA_004ada10* a, StructB_004ada10* b)
{
    a->field_b6 = (short)b->field_4->GetFieldInt("status", 0);
}

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

// Writes the "status", "text", "quickkey", "grayedout" and "stages" fields
// of an object as separate "<name>=<value>;\n" lines, each indented by the
// given number of tabs.
// FUNCTION: 0x4ada40
void __stdcall WriteButtonFields(Obj_004ada40* obj, FileHandle* out, int indent)
{
    char tab;
    char line[100];
    char num[100];
    int i;
    char* value;

    value = _itoa(obj->status, num, 10);
    tab = '\t';
    for (i = 0; i < indent; i++)
        HAPI_WriteFile(out, &tab, 1);
    sprintf(line, "%s=%s;\n", "status", value);
    HAPI_WriteFile(out, line, strlen(line));

    tab = '\t';
    for (i = 0; i < indent; i++)
        HAPI_WriteFile(out, &tab, 1);
    sprintf(line, "%s=%s;\n", "text", obj->text);
    HAPI_WriteFile(out, line, strlen(line));

    value = _itoa(obj->quickkey, num, 10);
    tab = '\t';
    for (i = 0; i < indent; i++)
        HAPI_WriteFile(out, &tab, 1);
    sprintf(line, "%s=%s;\n", "quickkey", value);
    HAPI_WriteFile(out, line, strlen(line));

    value = _itoa(obj->grayedout & 1, num, 10);
    tab = '\t';
    for (i = 0; i < indent; i++)
        HAPI_WriteFile(out, &tab, 1);
    sprintf(line, "%s=%s;\n", "grayedout", value);
    HAPI_WriteFile(out, line, strlen(line));

    value = _itoa(obj->stages, num, 10);
    tab = '\t';
    for (i = 0; i < indent; i++)
        HAPI_WriteFile(out, &tab, 1);
    sprintf(line, "%s=%s;\n", "stages", value);
    HAPI_WriteFile(out, line, strlen(line));
}

struct Source_004adc70 {
    char unknown_0[4];
    TdfRecord* tdf;                    // +0x4
};

struct Obj_004adc70 {
    char unknown_0[0xb6];
    char text[0x80];                   // +0xb6
    unsigned char stages;              // +0x136
    char unknown_137;                  // +0x137
    short status;                      // +0x138
    unsigned char quickkey;            // +0x13a
    char unknown_13b;                  // +0x13b
    unsigned short grayedout;          // +0x13c
};

// Reads a GUI control's TDF entry: status, text, quickkey, grayedout and
// stages, into the control object. Argument 1 is the control, argument 2 is
// the TDF source (its +4 is the key/value table).
// FUNCTION: 0x4adc70
void __stdcall ReadButtonFields(Obj_004adc70* obj, Source_004adc70* src)
{
    obj->status = (short)src->tdf->GetFieldInt("status", 0);
    memset(obj->text, 0, 0x80);
    src->tdf->GetFieldString(obj->text, "text", 0x80, DAT_005119b8);
    strncpy(obj->text, Translate(obj->text), 0x80);
    char local[20];
    src->tdf->GetFieldString(local, "quickkey", 0x13, DAT_005119b8);
    if (IsCharAlphaA(local[0]))
        obj->quickkey = (unsigned char)local[0];
    else
        obj->quickkey = (unsigned char)atoi(local);
    int gray = src->tdf->GetFieldInt("grayedout", 0);
    obj->grayedout = (gray ^ obj->grayedout) & 1 ^ obj->grayedout;
    obj->stages = (unsigned char)src->tdf->GetFieldInt("stages", 0);
}

struct Struct_004add60 {
    char unknown_0[0xda];
    short itemheight;                  // +0xda
};

// Writes the "itemheight" field of an object as "itemheight=<n>;" on its own
// line, indented by the given number of tabs.
// FUNCTION: 0x4add60
void __stdcall WriteListBoxFields(Struct_004add60* obj, FileHandle* out, int indent)
{
    char tab;
    char line[100];
    char num[100];
    char* value = _itoa(obj->itemheight, num, 10);
    tab = '\t';
    for (int i = 0; i < indent; i++)
        HAPI_WriteFile(out, &tab, 1);
    sprintf(line, "%s=%s;\n", "itemheight", value);
    HAPI_WriteFile(out, line, strlen(line));
}

struct Param2_004addf0 {
    char unknown_0[4];
    TdfRecord* obj;
};

#pragma pack(push, 2)
struct Struct_004addf0 {
    char unknown_0[0xce];
    int field_ce;
    char unknown_d2[4];
    int field_d6;
    unsigned short field_da;
};
#pragma pack(pop)

// FUNCTION: 0x4addf0
void __stdcall ReadListBoxFields(Struct_004addf0* param1, Param2_004addf0* param2)
{
    param1->field_ce = 0;
    param1->field_d6 = 0;
    param1->field_da = (unsigned short)param2->obj->GetFieldInt("itemheight", 0);
}

#pragma pack(push, 1)
struct Obj_004ade20 {
    char unknown_0[0xb6];
    char text[0x82];                   // +0xb6
    short maxchars;                    // +0x138
};
#pragma pack(pop)

// Writes the "maxchars" and "text" fields of an object as separate
// "<name>=<value>;\n" lines, each indented by the given number of tabs.
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

struct Source_004adf10 {
    char unknown_0[4];
    TdfRecord* tdf;                    // +0x4
};

#pragma pack(push, 1)
struct Obj_004adf10 {
    char unknown_0[0xb6];
    char text[0x82];                   // +0xb6
    short maxchars;                    // +0x138
};
#pragma pack(pop)

// FUNCTION: 0x4adf10
void __stdcall ReadTextInputFields(Obj_004adf10* obj, Source_004adf10* src)
{
    obj->maxchars = src->tdf->GetFieldInt("maxchars", 0);
    if (obj->maxchars > 0x80)
        obj->maxchars = 0x80;
    src->tdf->GetFieldString(obj->text, "text", 0x80, DAT_005119b8);
    strcpy(obj->text, Translate(obj->text));
}

#pragma pack(push, 1)
struct Obj_004ae170 {
    char unknown_0[0xb6];
    char text[0x80];                   // +0xb6
    short range;                       // +0x136
    char unknown_138[4];               // +0x138
    int thick;                         // +0x13c
    short knobpos;                     // +0x140
    short knobsize;                    // +0x142
    void* sliderCallback;              // +0x144 (a typed function pointer here takes symbol ids)
};
#pragma pack(pop)

// FUNCTION: 0x4adf90
void __stdcall WriteSliderFields(Obj_004ae170* obj, FileHandle* out, int indent)
{
    char tab;
    char line[100];
    char num[100];
    int i;

    char* value = _itoa(obj->range, num, 10);
    tab = '\t';
    for (i = 0; i < indent; i++)
        HAPI_WriteFile(out, &tab, 1);
    sprintf(line, "%s=%s;\n", "range", value);
    HAPI_WriteFile(out, line, strlen(line));

    value = _itoa(obj->thick, num, 10);
    tab = '\t';
    for (i = 0; i < indent; i++)
        HAPI_WriteFile(out, &tab, 1);
    sprintf(line, "%s=%s;\n", "thick", value);
    HAPI_WriteFile(out, line, strlen(line));

    value = _itoa(obj->knobpos, num, 10);
    tab = '\t';
    for (i = 0; i < indent; i++)
        HAPI_WriteFile(out, &tab, 1);
    sprintf(line, "%s=%s;\n", "knobpos", value);
    HAPI_WriteFile(out, line, strlen(line));

    value = _itoa(obj->knobsize, num, 10);
    tab = '\t';
    for (i = 0; i < indent; i++)
        HAPI_WriteFile(out, &tab, 1);
    sprintf(line, "%s=%s;\n", "knobsize", value);
    HAPI_WriteFile(out, line, strlen(line));
}

struct Source_004ae170 {
    char unknown_0[4];
    TdfRecord* tdf;                    // +0x4
};

// FUNCTION: 0x4ae170
void __stdcall ReadSliderFields(Obj_004ae170* obj, Source_004ae170* src)
{
    obj->range = src->tdf->GetFieldInt("range", 0);
    obj->thick = (short)src->tdf->GetFieldInt("thick", 0);
    obj->knobpos = src->tdf->GetFieldInt("knobpos", 0);
    obj->knobsize = src->tdf->GetFieldInt("knobsize", 0);
    obj->sliderCallback = 0;
    src->tdf->GetFieldString(obj->text, "text", 0x80, DAT_005119b8);
    strcpy(obj->text, Translate(obj->text));
}

struct Obj_004ae230 {
    char unknown_0[0xb6];
    char text[0x80];                   // +0xb6
    char link[0x80];                   // +0x136
};

// Writes the "text" and "link" strings of an object as "text=<text>;" and
// "link=<link>;" lines, each indented by the given number of tabs; the
// writing siblings are 0x4ae4e0/0x4ae440.
// FUNCTION: 0x4ae230
void __stdcall WriteTextLinkFields(Obj_004ae230* obj, FileHandle* out, int indent)
{
    char line[100];
    WriteTabsInline(out, indent);
    sprintf(line, "%s=%s;\n", "text", obj->text);
    HAPI_WriteFile(out, line, strlen(line));
    WriteTabsInline(out, indent);
    sprintf(line, "%s=%s;\n", "link", obj->link);
    HAPI_WriteFile(out, line, strlen(line));
}

struct Source_004ae300 {
    char unknown_0[4];
    TdfRecord* tdf;                    // +0x4
};

#pragma pack(push, 1)
struct Obj_004ae300 {
    char unknown_0[0xb6];
    char text[0x80];                   // +0xb6
    char link[0x11];                   // +0x136
    char field_147;                    // +0x147
};
#pragma pack(pop)

// FUNCTION: 0x4ae300
void __stdcall ReadTextLinkFields(Obj_004ae300* obj, Source_004ae300* src)
{
    obj->field_147 = 0;
    obj->link[0] = 0;
    memset(obj->text, 0, sizeof(obj->text));
    src->tdf->GetFieldString(obj->text, "text", 0x80, DAT_005119b8);
    strncpy(obj->text, Translate(obj->text), 0x7f);
    src->tdf->GetFieldString(obj->link, "link", 0x10, DAT_005119b8);
}

struct Obj_004ae380 {
    char unknown_0[0xc8];
    unsigned int hotornot : 1;         // +0xc8 bit 0
};

// Writes the "hotornot" flag of an object as "hotornot=<n>;" on its own
// line, indented by the given number of tabs; the reading counterpart is
// 0x4ae410.
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

struct Source_004ae410 {
    char unknown_0[4];
    TdfRecord* tdf;                    // +0x4
};

struct Obj_004ae410 {
    char unknown_0[0xc8];
    unsigned int hotornot : 1;         // +0xc8 bit 0
};

// Reads the "hotornot" flag from a section into a 1-bit bitfield. The call
// result goes through an int local; assigning it directly keeps the old field
// value in a separate register (esi).
// FUNCTION: 0x4ae410
void __stdcall ReadHotOrNotField(Obj_004ae410* obj, Source_004ae410* src)
{
    int value = src->tdf->GetFieldInt("hotornot", 0);
    obj->hotornot = value;
}

struct Obj_004ae440 {
    char unknown_0[0xb6];
    char text[0x80];                   // +0xb6
};

// Writes the "filename" string of an object as "filename=<text>;" on its own
// line, indented by the given number of tabs; the reading counterpart is
// 0x4ae4b0. Sibling of 0x4ae380.
// FUNCTION: 0x4ae440
void __stdcall WriteFontFilenameField(Obj_004ae440* obj, FileHandle* out, int indent)
{
    char tab;
    char line[100];
    tab = '\t';
    for (int i = 0; i < indent; i++)
        HAPI_WriteFile(out, &tab, 1);
    sprintf(line, "%s=%s;\n", "filename", obj->text);
    HAPI_WriteFile(out, line, strlen(line));
}

struct Source_004ae4b0 {
    char unknown_0[4];
    TdfRecord* tdf;                    // +0x4
};

#pragma pack(push, 1)
struct Obj_004ae4b0 {
    char unknown_0[0xb6];
    char text[0x80];                   // +0xb6
};
#pragma pack(pop)

// Reads the "filename" key of a TDF entry (sibling of 0x4ae300).
// FUNCTION: 0x4ae4b0
void __stdcall ReadFontFilenameField(Obj_004ae4b0* obj, Source_004ae4b0* src)
{
    src->tdf->GetFieldString(obj->text, "filename", 0x20, DAT_005119b8);
}

struct Obj_004ae4e0 {
    char unknown_0[0xb6];
    char text[0x80];                   // +0xb6
};

// Writes the "filename" field of an object as "filename=<text>;" on its own
// line, indented by the given number of tabs; the reading counterpart is
// 0x4ae4b0.
// FUNCTION: 0x4ae4e0
void __stdcall WriteSurfFilenameField(Obj_004ae4e0* obj, FileHandle* out, int indent)
{
    char tab;
    char line[100];
    tab = '\t';
    for (int i = 0; i < indent; i++)
        HAPI_WriteFile(out, &tab, 1);
    sprintf(line, "%s=%s;\n", "filename", obj->text);
    HAPI_WriteFile(out, line, strlen(line));
}

struct Source_004ae550 {
    char unknown_0[4];
    TdfRecord* tdf;                    // +0x4
};

#pragma pack(push, 1)
struct Obj_004ae550 {
    char unknown_0[0xb6];
    char text[0x80];                   // +0xb6
};
#pragma pack(pop)

// Reads the "filename" key of a TDF entry (byte-identical to 0x4ae4b0).
// FUNCTION: 0x4ae550
void __stdcall ReadSurfFilenameField(Obj_004ae550* obj, Source_004ae550* src)
{
    src->tdf->GetFieldString(obj->text, "filename", 0x20, DAT_005119b8);
}

#pragma pack(push, 1)
struct Obj_004ae580 {
    char unknown_0[0xb6];
    int field_b6;                      // +0xb6
};
#pragma pack(pop)

// Writes the "nuttin" int field (offset 0xb6) as "nuttin=<n>;" on its own
// line, indented by the given number of tabs; the reading counterpart is
// 0x4ae610. Byte-for-byte twin of 0x4ae380.
// FUNCTION: 0x4ae580
void __stdcall WriteNuttinField(Obj_004ae580* obj, FileHandle* out, int indent)
{
    char tab;
    char line[100];
    char num[100];
    char* value = _itoa(obj->field_b6, num, 10);
    tab = '\t';
    for (int i = 0; i < indent; i++)
        HAPI_WriteFile(out, &tab, 1);
    sprintf(line, "%s=%s;\n", "nuttin", value);
    HAPI_WriteFile(out, line, strlen(line));
}

#pragma pack(push, 1)
struct StructA_004ae610 {
    char unknown_0[0xb6];
    int field_b6;                      // +0xb6
};
#pragma pack(pop)

struct StructB_004ae610 {
    char unknown_0[4];
    TdfRecord* field_4;                // +0x4
};

// FUNCTION: 0x4ae610
void __stdcall ReadNuttinField(StructA_004ae610* a, StructB_004ae610* b)
{
    a->field_b6 = b->field_4->GetFieldInt("nuttin", 0);
}

// Writes a whole GUI file: for each of the object's records, its header, its
// COMMON section and the fields of its type, as TDF text; the file it would
// overwrite is renamed to .BGU first.
// FUNCTION: 0x4ae630
void __stdcall WriteGuiFile(char* obj, char* name)
{
    int index;
    char button[100];
    char slider[100];
    char header[100];
    char common[100];
    char gadget[100];
    char path[256];
    char backup[256];
    char hot[100];
    char edit[100];
    char empty[100];
    char list[100];
    ChangeExtension(name, path, "GUI");
    if (HAPI_FileLengthByName(path)) {
        ChangeExtension(name, backup, "BGU");
        RemoveFile(backup);
        RenameFile(path, backup);
    }
    FileHandle* out = HAPI_CreateFile(path);
    char* p = obj;
    for (index = 0; index < *(short*)(obj + 0xb6) + 1; index++, p += 0x15b) {
        sprintf(gadget, "GADGET%d", index);
        sprintf(header, "[%s]", gadget);
        HAPI_WriteFile(out, header, strlen(header));
        HAPI_WriteFile(out, "\n", 1);
        WriteTabs(out, 1);
        HAPI_WriteFile(out, "{\n", 2);
        sprintf(common, "[%s]", "COMMON");
        // Block-scope char with a plain for loop: the three tab chars share one frame slot.
        {
            char t1 = '\t';
            for (int i = 0; i < 1; i++) HAPI_WriteFile(out, &t1, 1);
        }
        HAPI_WriteFile(out, common, strlen(common));
        HAPI_WriteFile(out, "\n", 1);
        WriteTabs(out, 2);
        HAPI_WriteFile(out, "{\n", 2);
        WriteCommonFields((Common_004ad350*)p, out, 2);
        // Counter and char defined in this block, in this order: emitted in source order.
        {
            int j = 2;
            char t2 = '\t';
            do { HAPI_WriteFile(out, &t2, 1); } while (--j);
        }
        HAPI_WriteFile(out, "}\n", 2);
        switch (*(unsigned char*)p) {
        case 0:
            WritePanelFields((Obj_004ad4f0*)p, out, 1);
            break;
        case 1:
            WriteKeyValue(out, "status", _itoa(*(short*)(p + 0x138), button, 10), 1);
            WriteKeyValue(out, "text", p + 0xb6, 1);
            WriteKeyValue(out, "quickkey", _itoa(*(signed char*)(p + 0x13a), button, 10), 1);
            WriteKeyValue(out, "grayedout", _itoa(*(unsigned char*)(p + 0x13c) & 1, button, 10), 1);
            WriteKeyValue(out, "stages", _itoa(*(unsigned char*)(p + 0x136), button, 10), 1);
            break;
        case 2:
            WriteKeyValue(out, "itemheight", _itoa(*(short*)(p + 0xda), list, 10), 1);
            break;
        case 3:
            WriteKeyValue(out, "maxchars", _itoa(*(short*)(p + 0x138), edit, 10), 1);
            WriteKeyValue(out, "text", p + 0xb6, 1);
            break;
        case 4:
            WriteKeyValue(out, "range", _itoa(*(short*)(p + 0x136), slider, 10), 1);
            WriteKeyValue(out, "thick", _itoa(*(int*)(p + 0x13c), slider, 10), 1);
            WriteKeyValue(out, "knobpos", _itoa(*(short*)(p + 0x140), slider, 10), 1);
            WriteKeyValue(out, "knobsize", _itoa(*(short*)(p + 0x142), slider, 10), 1);
            break;
        case 5:
            WriteKeyValue(out, "text", p + 0xb6, 1);
            WriteKeyValue(out, "link", p + 0x136, 1);
            break;
        case 6:
            WriteKeyValue(out, "hotornot", _itoa(*(unsigned int*)(p + 0xc8) & 1, hot, 10), 1);
            break;
        case 7:
            WriteKeyValue(out, "filename", p + 0xb6, 1);
            break;
        case 8:
            WriteKeyValue(out, "filename", p + 0xb6, 1);
            break;
        case 10:
            WriteKeyValue(out, "nuttin", _itoa(*(int*)(p + 0xb6), empty, 10), 1);
            break;
        }
        {
            char t3 = '\t';
            for (int i = 0; i < 1; i++) HAPI_WriteFile(out, &t3, 1);
        }
        HAPI_WriteFile(out, "}\n", 2);
    }
    HAPI_CloseFile(out);
}

#pragma pack(push, 1)
struct Sub2_004aeac0 {
    char pad0[0xce - 0xb6];
    int field_ce;                      // +0xce
    char pad1[0xd6 - 0xce - 4];
    int field_d6;                      // +0xd6
    short itemheight;                  // +0xda
    char pad2[0x136 - 0xdc];
};

struct Sub6_004aeac0 {
    char pad0[0xc8 - 0xb6];
    unsigned int hotornot : 1;         // +0xc8, bit 0
    char pad1[0x136 - 0xcc];
};

union Body_004aeac0 {
    char text[0x80];                   // +0xb6
    int nuttin;                        // +0xb6
    short total;                       // +0xb6
    Sub2_004aeac0 s2;
    Sub6_004aeac0 s6;
};

struct Sub34_004aeac0 {
    short range;                       // +0x136
    short maxchars;                    // +0x138
    char pad0[0x13c - 0x13a];
    int thick;                         // +0x13c
    short knobpos;                     // +0x140
    short knobsize;                    // +0x142
    void* sliderCallback;              // +0x144 (a typed function pointer here takes symbol ids)
    char pad1[0x15b - 0x148];
};

struct Sub5_004aeac0 {
    char link[0x11];                   // +0x136
    char field_147;                    // +0x147
    char pad0[0x15b - 0x148];
};

union Tail_004aeac0 {
    Sub34_004aeac0 s34;
    Sub5_004aeac0 s5;
};

struct Elem_004aeac0 {
    unsigned char type;                // +0x000
    char pad0[0xb6 - 0x001];
    Body_004aeac0 body;                // +0xb6
    Tail_004aeac0 tail;                // +0x136
};
#pragma pack(pop)

// Reads a whole GUI file into the object's records: the COMMON section of
// each record, then the fields of its type, and the count of records into the
// first record's total.
// FUNCTION: 0x4aeac0
int __stdcall ReadGuiFile(Elem_004aeac0* obj, char* name)
{
    TdfFile parser;
    int i;
    int ret = 0;
    char path[256];
    ChangeExtension(name, path, "GUI");
    if ((&parser)->LoadFile(path) == 1) {
        ret = 1;
        i = 0;
        while (1) {
            (&parser)->ResetCurrentRecord();
            if (!(&parser)->SelectRecordAt(i))
                break;
            int cur = (&parser)->GetCurrentRecord();
            Elem_004aeac0* e = obj + i;
            ReadCommonSection((Common_004ad350*)e, (Tree_004ad350*)&parser);
            (&parser)->SetCurrentRecord(cur);
            switch (e->type) {
            case 0:
                ReadPanelFields((Obj_004ad890*)e, (Source_004ad890*)&parser);
                break;
            case 1:
                ReadButtonFields((Obj_004adc70*)e, (Source_004adc70*)&parser);
                break;
            case 2:
                e->body.s2.field_ce = 0;
                e->body.s2.field_d6 = 0;
                e->body.s2.itemheight = (short)parser.current->GetFieldInt("itemheight", 0);
                break;
            case 3:
                e->tail.s34.maxchars = (short)parser.current->GetFieldInt("maxchars", 0);
                if (e->tail.s34.maxchars > 0x80)
                    e->tail.s34.maxchars = 0x80;
                parser.current->GetFieldString(e->body.text, "text", 0x80, DAT_005119b8);
                strcpy(e->body.text, Translate(e->body.text));
                break;
            case 4:
                e->tail.s34.range = (short)parser.current->GetFieldInt("range", 0);
                e->tail.s34.thick = (short)parser.current->GetFieldInt("thick", 0);
                e->tail.s34.knobpos = (short)parser.current->GetFieldInt("knobpos", 0);
                e->tail.s34.knobsize = (short)parser.current->GetFieldInt("knobsize", 0);
                e->tail.s34.sliderCallback = 0;
                parser.current->GetFieldString(e->body.text, "text", 0x80, DAT_005119b8);
                strcpy(e->body.text, Translate(e->body.text));
                break;
            case 5:
                e->tail.s5.link[0] = 0;
                e->tail.s5.field_147 = 0;
                memset(e->body.text, 0, sizeof(e->body.text));
                parser.current->GetFieldString(e->body.text, "text", 0x80, DAT_005119b8);
                strncpy(e->body.text, Translate(e->body.text), 0x7f);
                parser.current->GetFieldString(e->tail.s5.link, "link", 0x10, DAT_005119b8);
                break;
            case 6:
                {
                    // Through an int local: assigning the call result directly changes the code.
                    int value = parser.current->GetFieldInt("hotornot", 0);
                    e->body.s6.hotornot = value;
                }
                break;
            case 7:
                parser.current->GetFieldString(e->body.text, "filename", 0x20, DAT_005119b8);
                break;
            case 8:
                parser.current->GetFieldString(e->body.text, "filename", 0x20, DAT_005119b8);
                break;
            case 10:
                e->body.nuttin = parser.current->GetFieldInt("nuttin", 0);
                break;
            }
            i++;
        }
        obj->body.total = (short)(i - 1);
        (&parser)->Unload();
    }
    return ret;
}

struct Obj_004aeda0 {
    char unknown_0[8];
    int* buffers[3];                   // +0x8
    int field_14;                      // +0x14
};

// Frees one of the three buffers at +0x8 and clears it and field_14.
// FUNCTION: 0x4aeda0
void __stdcall FreeGafSlot(Obj_004aeda0* obj, int i)
{
    if (obj->buffers[i] != 0) {
        GameFreeThunk(obj->buffers[i]);
        obj->buffers[i] = 0;
        obj->field_14 = 0;
    }
}

struct Table_004aedd0 {
    unsigned short count;              // +0x0
    char unknown_2[0x28 - 0x2];
    void* entries[1];                  // +0x28, 8-byte stride
};

struct Object_004aedd0 {
    char unknown_0[8];
    void* items[1];                    // +0x8
    char unknown_c[0x14 - 0xc];
    void* field_14;                    // +0x14
    char unknown_18[0xab6 - 0x18];
    char dir[0x100];                   // +0xab6
};

// Loads the GAF named by `name` (prefixed with obj->dir) and stores the loaded
// GAF in obj->items[index], after shifting every frame by the offset of the
// glyph/entry 0x49. obj->field_14 receives the same GAF pointer. When the file
// does not exist, obj->items[index] is zeroed.
// FUNCTION: 0x4aedd0
void __stdcall LoadGafIntoSlot(Object_004aedd0* obj, char* name, int index)
{
    // Suspected original bug: this local is never assigned, and the original
    // reads it at 0x4aee78 (mov ebx,[esp+0x10]) when the table has no entry
    // 0x49. Reproducing the read is required for a byte match.
    int unknown;
    char path[256];
    strncpy(path, obj->dir, 0x100);
    strcat(path, name);
    ChangeExtension(path, path, "GAF");
    if (HAPI_FileLengthByName(path)) {
        void* gaf = LoadGaf(path);
        obj->items[index] = gaf;
        Table_004aedd0* table = *(Table_004aedd0**)((char*)gaf + 0xc);
        char* e = (char*)GetGafFrame(table, 0x49);
        int d;
        if (e)
            d = *(unsigned short*)(e + 2);
        else
            d = unknown;
        for (int i = 0; i < table->count; i++) {
            char* f = (char*)GetGafFrame(table, i);
            if (f)
                *(short*)(f + 6) -= d;
        }
        obj->field_14 = obj->items[index];
    } else {
        obj->items[index] = 0;
        return;
    }
}

struct Object_004aeee0 {
    int unknown_0;
    void* gaf;                         // +0x4
    char unknown_8[0xab6 - 0x8];
    char dir[0x100];                   // +0xab6
};

// FUNCTION: 0x4aeee0
void __stdcall LoadGafFile(Object_004aeee0* obj, char* name)
{
    char path[256];
    strncpy(path, obj->dir, 0x100);
    strcat(path, name);
    ChangeExtension(path, path, "GAF");
    if (HAPI_FileLengthByName(path)) {
        obj->gaf = LoadGaf(path);
    }
}

struct Obj_004aef80 {
    char unknown_0[4];
    int* field_4;                      // +0x4
};

// FUNCTION: 0x4aef80
void __stdcall FreeCommonGuiGaf(Obj_004aef80* obj)
{
    if (obj->field_4) {
        GameFreeThunk(obj->field_4);
        obj->field_4 = 0;
    }
}
