// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Builds "<dir>-<side>\<name>" into the caller's buffer when a side prefix is
// available and the file opens, otherwise "<dir>\<name>". An optional
// extension is appended after cutting any existing one at the last '.'.
#include <stdio.h>
#include <string.h>

int FUN_0049f580(void);
char* __stdcall StripExtension(char* name);
void* __stdcall FUN_004bb5b0(char* path);
int __stdcall FUN_004bb5d0(void* file);

// FUNCTION: 0x4290f0
char* __stdcall BuildDataPath(char* buf, char* dir, char* name, char* ext)
{
    char* side = (char*)FUN_0049f580();
    if (side) {
        sprintf(buf, "%s-%s\\%s", dir, side, name);
        if (ext != 0 && strlen(ext) != 0) {
            StripExtension(buf);
            strcat(buf, ".");
            strcat(buf, ext);
        }
        void* file = FUN_004bb5b0(buf);
        if (file) {
            FUN_004bb5d0(file);
            return buf;
        }
    }
    strcpy(buf, dir);
    strcat(buf, "\\");
    strcat(buf, name);
    if (ext != 0 && strlen(ext) != 0) {
        StripExtension(buf);
        strcat(buf, ".");
        strcat(buf, ext);
    }
    return buf;
}
