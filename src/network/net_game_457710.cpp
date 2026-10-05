// Decompiled by GPT-6-Luna. Names are provisional.
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

extern char* g_game;
extern char DAT_00512ca8[];
extern char DAT_00512c98[];

int __stdcall FUN_004ca490(void* p);
void __stdcall FUN_004c9c20(void* p);
int __stdcall FUN_00461020(int a, int b);
void __stdcall FUN_00434ab0(int a);

// FUNCTION: 0x457710
int FUN_00457710()
{
    if (*(int*)(g_game + 0x4e5) != 0)
        return 1;

    if (FUN_004ca490(g_game + 0x14)) {
        FUN_004c9c20(g_game + 0x14);
        *(int*)(g_game + 0x4f1) = 10;
        if (FUN_00461020(2, 100)) {
            FUN_00434ab0(3);
            char* name = *(char**)(*(char**)(*(char**)(g_game + 0x4e5) + 8) + 0x30);
            if (name != 0) {
                strncpy(g_game + 0x14, name, 0x10);
                return 1;
            }

            char username[16];
            username[0] = 0;
            if (DAT_00512ca8[0] != 0) {
                strncat(username, DAT_00512ca8, 0x10);
            } else if (DAT_00512c98[0] != 0) {
                strncat(username, DAT_00512c98, 0x10);
            } else {
                DWORD size = 0xf;
                if (GetUserNameA(username, &size))
                    username[size] = 0;
                else
                    strcpy(username, "TotalA");
            }

            time_t now = time(0);
            struct tm* local = localtime(&now);
            int len = strlen(username);
            if (15 - len >= 7)
                sprintf(username + len, "_%02d%02d%02d", local->tm_hour, local->tm_min, local->tm_sec);
            strcpy(g_game + 0x14, username);
            return 1;
        }
    }
    return 0;
}
