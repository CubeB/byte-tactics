// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct FindData_00495930 {
    char unknown_0[0x14];
    char name[260];                    // +0x14
};

extern char DAT_00503374[];
extern char DAT_005119b8[];

int __stdcall FUN_004bc4b0(const char* path, FindData_00495930* fd, int a, int b);
int __stdcall FUN_004bc640(int handle, FindData_00495930* fd);
void __stdcall FUN_004bc8d0(int handle);

// FUNCTION: 0x495930
void __stdcall BuildScreenshotPath(char* out, const char* dir, const char* name, const char* ext)
{
    bool needSep = false;
    FindData_00495930 fd;
    int max = 0;

    if (*dir != 0) {
        if (dir[strlen(dir) - 1] != '\\') {
            needSep = true;
        }
    }
    sprintf(out, "%s%s%s*.%s", dir, needSep ? DAT_00503374 : DAT_005119b8, name, ext);
    int handle = FUN_004bc4b0(out, &fd, -1, 1);
    if (handle >= 0) {
        do {
            int val = atoi(fd.name + strlen(name));
            if (val > max) {
                max = val;
            }
        } while (FUN_004bc640(handle, &fd) == 0);
        FUN_004bc8d0(handle);
    }
    sprintf(out, "%s%s%s%04i.%s", dir, needSep ? DAT_00503374 : DAT_005119b8, name, max + 1, ext);
}
