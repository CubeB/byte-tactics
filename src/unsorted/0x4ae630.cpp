// Decompiled by GPT-6. Names are provisional.
// Partial (94.8%): the two-tab loop counter setup and tab store are reversed.
// All 1160 bytes are present; the jump table diff is relocation display noise.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Class_004bbbe0;
char* __stdcall FUN_004baff0(char*, char*, const char*);
int __stdcall FUN_004bbc40(char*);
void __stdcall FUN_004bbc30(char*);
void __stdcall FUN_004bbc10(char*, char*);
Class_004bbbe0* __stdcall FUN_004bb6a0(char*);
void __stdcall FUN_004bb5d0(Class_004bbbe0*);
unsigned int __stdcall FUN_004bbbe0(Class_004bbbe0*, void*, unsigned int);
void __stdcall FUN_004accd0(Class_004bbbe0*, int);
void __stdcall FUN_004acde0(Class_004bbbe0*, char*, char*, int);
void __stdcall FUN_004ace50(void*, Class_004bbbe0*, int);
void __stdcall FUN_004ad4f0(void*, Class_004bbbe0*, int);

// FUNCTION: 0x4ae630
void __stdcall FUN_004ae630(char* obj, char* name)
{
    char tab;
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
    FUN_004baff0(name, path, "GUI");
    if (FUN_004bbc40(path)) {
        FUN_004baff0(name, backup, "BGU");
        FUN_004bbc30(backup);
        FUN_004bbc10(path, backup);
    }
    Class_004bbbe0* out = FUN_004bb6a0(path);
    char* p = obj;
    for (index = 0; index < *(short*)(obj + 0xb6) + 1; index++, p += 0x15b) {
        sprintf(gadget, "GADGET%d", index);
        sprintf(header, "[%s]", gadget);
        FUN_004bbbe0(out, header, strlen(header));
        FUN_004bbbe0(out, "\n", 1);
        FUN_004accd0(out, 1);
        FUN_004bbbe0(out, "{\n", 2);
        sprintf(common, "[%s]", "COMMON");
        tab = '\t';
        for (int i = 0; i < 1; i++) FUN_004bbbe0(out, &tab, 1);
        FUN_004bbbe0(out, common, strlen(common));
        FUN_004bbbe0(out, "\n", 1);
        FUN_004accd0(out, 2);
        FUN_004bbbe0(out, "{\n", 2);
        FUN_004ace50(p, out, 2);
        i = 2;
        tab = '\t';
        do { FUN_004bbbe0(out, &tab, 1); } while (--i);
        FUN_004bbbe0(out, "}\n", 2);
        switch (*(unsigned char*)p) {
        case 0:
            FUN_004ad4f0(p, out, 1);
            break;
        case 1:
            FUN_004acde0(out, "status", _itoa(*(short*)(p + 0x138), button, 10), 1);
            FUN_004acde0(out, "text", p + 0xb6, 1);
            FUN_004acde0(out, "quickkey", _itoa(*(signed char*)(p + 0x13a), button, 10), 1);
            FUN_004acde0(out, "grayedout", _itoa(*(unsigned char*)(p + 0x13c) & 1, button, 10), 1);
            FUN_004acde0(out, "stages", _itoa(*(unsigned char*)(p + 0x136), button, 10), 1);
            break;
        case 2:
            FUN_004acde0(out, "itemheight", _itoa(*(short*)(p + 0xda), list, 10), 1);
            break;
        case 3:
            FUN_004acde0(out, "maxchars", _itoa(*(short*)(p + 0x138), edit, 10), 1);
            FUN_004acde0(out, "text", p + 0xb6, 1);
            break;
        case 4:
            FUN_004acde0(out, "range", _itoa(*(short*)(p + 0x136), slider, 10), 1);
            FUN_004acde0(out, "thick", _itoa(*(int*)(p + 0x13c), slider, 10), 1);
            FUN_004acde0(out, "knobpos", _itoa(*(short*)(p + 0x140), slider, 10), 1);
            FUN_004acde0(out, "knobsize", _itoa(*(short*)(p + 0x142), slider, 10), 1);
            break;
        case 5:
            FUN_004acde0(out, "text", p + 0xb6, 1);
            FUN_004acde0(out, "link", p + 0x136, 1);
            break;
        case 6:
            FUN_004acde0(out, "hotornot", _itoa(*(unsigned int*)(p + 0xc8) & 1, hot, 10), 1);
            break;
        case 7:
            FUN_004acde0(out, "filename", p + 0xb6, 1);
            break;
        case 8:
            FUN_004acde0(out, "filename", p + 0xb6, 1);
            break;
        case 10:
            FUN_004acde0(out, "nuttin", _itoa(*(int*)(p + 0xb6), empty, 10), 1);
            break;
        }
        tab = '\t';
        for (i = 0; i < 1; i++) FUN_004bbbe0(out, &tab, 1);
        FUN_004bbbe0(out, "}\n", 2);
    }
    FUN_004bb5d0(out);
}
