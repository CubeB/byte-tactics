// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Builds "<dir>-<side>\<name>" into the caller's buffer when a side prefix is
// available and the file opens, otherwise "<dir>\<name>". An optional
// extension is appended after cutting any existing one at the last '.'.
#include <stdio.h>
#include <string.h>

int GetPreferredLanguage(void);
char* __stdcall StripExtension(char* name);
void* __stdcall HAPI_OpenFileRead(char* path);
int __stdcall HAPI_CloseFile(void* file);

// FUNCTION: 0x4290f0
char* __stdcall BuildDataPath(char* buf, char* dir, char* name, char* ext)
{
    char* side = (char*)GetPreferredLanguage();
    if (side) {
        sprintf(buf, "%s-%s\\%s", dir, side, name);
        if (ext != 0 && strlen(ext) != 0) {
            StripExtension(buf);
            strcat(buf, ".");
            strcat(buf, ext);
        }
        void* file = HAPI_OpenFileRead(buf);
        if (file) {
            HAPI_CloseFile(file);
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
