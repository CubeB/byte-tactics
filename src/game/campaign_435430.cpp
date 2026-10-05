// Decompiled by Claude Opus 5.5. Names are provisional.
// Builds the path of a campaign file and stores it in name slot `index`
// (an inlined copy of 0x4353b0.cpp). With a side prefix (FUN_0049f580) it
// first tries "<dir>-<side>\<name>.<ext>" and keeps it when that file opens;
// otherwise it uses "<dir>\<name>.<ext>". An empty name stores the blank
// string DAT_005119b8.
#include <stdio.h>
#include <string.h>

extern char DAT_005119b8[];

int FUN_0049f580(void);
char* __stdcall StripExtension(char* name);
void* __stdcall FUN_004bb5b0(char* path);
int __stdcall FUN_004bb5d0(void* file);
int __stdcall FUN_004bbc40(char* path);

class Class_00435c00 {
public:
    char unknown_0[0x104];
    char names[9][0x100];              // +0x104
    int exists;                        // +0xa04

    // 0x4353b0, inlined
    void SetName(int index, char* text)
    {
        strcpy(names[index], text);
        if (index == 1) {
            if (strlen(text) != 0)
                exists = FUN_004bbc40(text);
            else
                exists = 0;
        }
    }

    void FUN_00435430(int index, char* dir, char* name, char* ext);
};

// FUNCTION: 0x435430
void Class_00435c00::FUN_00435430(int index, char* dir, char* name, char* ext)
{
    char path[256];
    if (strlen(name) == 0) {
        SetName(index, DAT_005119b8);
        return;
    }
    char* side = (char*)FUN_0049f580();
    if (side) {
        sprintf(path, "%s-%s\\%s", dir, side, name);
        StripExtension(path);
        strcat(path, ".");
        strcat(path, ext);
        void* file = FUN_004bb5b0(path);
        if (file) {
            FUN_004bb5d0(file);
            SetName(index, path);
            return;
        }
    }
    sprintf(path, "%s\\%s", dir, name);
    StripExtension(path);
    strcat(path, ".");
    strcat(path, ext);
    SetName(index, path);
}
