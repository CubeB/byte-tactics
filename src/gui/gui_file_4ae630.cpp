// Decompiled by GPT-6, finished by space-bunny-free, edited by deepseek-v4.1. Names are provisional.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct FileHandle;
char* __stdcall ChangeExtension(char*, char*, const char*);
int __stdcall HAPI_FileLengthByName(char*);
void __stdcall RemoveFile(char*);
void __stdcall RenameFile(char*, char*);
FileHandle* __stdcall HAPI_CreateFile(char*);
void __stdcall HAPI_CloseFile(FileHandle*);
unsigned int __stdcall HAPI_WriteFile(FileHandle*, void*, unsigned int);
void __stdcall WriteTabs(FileHandle*, int);
void __stdcall WriteKeyValue(FileHandle*, char*, char*, int);
void __stdcall WriteCommonFields(void*, FileHandle*, int);
void __stdcall WritePanelFields(void*, FileHandle*, int);

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
        WriteCommonFields(p, out, 2);
        // Counter and char defined in this block, in this order: emitted in source order.
        {
            int j = 2;
            char t2 = '\t';
            do { HAPI_WriteFile(out, &t2, 1); } while (--j);
        }
        HAPI_WriteFile(out, "}\n", 2);
        switch (*(unsigned char*)p) {
        case 0:
            WritePanelFields(p, out, 1);
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
