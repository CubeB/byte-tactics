// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <stdio.h>
#include <string.h>

extern int DAT_0050289c;                     // campaign/multiplayer flag

char* __stdcall FUN_004290f0(char* out, const char* dir, const char* name, const char* ext);
char __stdcall FUN_0041d6a0(int mode);
char* __stdcall StripExtension(char* name);

// FUNCTION: 0x41d7b0
char* __stdcall FUN_0041d7b0(char* out, const char* dir, const char* name, const char* ext)
{
    if (DAT_0050289c != 0) {
        char c = 0;
        int i;
        for (i = 0; i < 2; i++) {
            c = FUN_0041d6a0(i);
            if (c != 0) {
                break;
            }
        }
        if (c == 0) {
            *out = 0;
            return 0;
        }
        sprintf(out, "%c\\\\%s\\%s", c, dir, name);
        StripExtension(out);
        if (strlen(ext) != 0) {
            strcat(out, ".");
            strcat(out, ext);
        }
        return out;
    }
    return FUN_004290f0(out, dir, name, ext);
}
