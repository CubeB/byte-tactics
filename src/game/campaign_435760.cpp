// Decompiled by Space Bunny Free. Names are provisional.
// Fills *param_1 with the mission list and returns how many missions there
// are. Neighbours 0x4356f0 and 0x435980 share the same layout. The mission
// count is built by a separate counter `m` and copied into `n` after the
// loop; writing the loop directly on `n` makes MSVC 5 spill the zero to the
// stack at the loop head as well as at the loop exit.
#include <stdio.h>
#include <string.h>

class Class_004c3e10 {
public:
    char unknown_0[4];
    int field_0x4;

    void FUN_004c3e10();
};

class Class_004c3410 {
public:
    int FUN_004c3410(char* name);
};

char* __stdcall FUN_004b6af0(int list, int index);
int __stdcall FUN_004c58a0(Class_004c3e10* obj, char* buf, const char* key, int size, int def);
void* __cdecl FUN_004d83b0(const char* name, int size);

class Class_00435760 {
public:
    int unknown_0;
    char name[0xa08 - 4];              // +0x4
    Class_004c3e10 list;               // +0xa08

    int BuildMissionList(int* param_1);
};

// FUNCTION: 0x435760
int Class_00435760::BuildMissionList(int* param_1)
{
    char buf[128];
    char temp[256];
    if (strlen(name) == 0)
        return 0;
    int n;
    if (strlen(&name[0]) == 0) {
        n = 0;
    } else {
        int m = 0;
        while (1) {
            sprintf(buf, "MISSION%d", m);
            list.FUN_004c3e10();
            if (((Class_004c3410*)&list)->FUN_004c3410(buf) == 0)
                break;
            m++;
        }
        n = m;
    }
    if (n != 0) {
        char* p = (char*)FUN_004d83b0("MissionList", n << 8);
        *param_1 = (int)p;
        *p = 0;
        for (int i = 0; i < n; i++) {
            sprintf(buf, "MISSION%d", i);
            list.FUN_004c3e10();
            if (((Class_004c3410*)&list)->FUN_004c3410(buf) == 0)
                return 0;
            if (FUN_004c58a0(&list, temp, "missionname", 0x100, 0) != 0)
                strcpy(FUN_004b6af0(*param_1, i), temp);
            else
                strcpy(FUN_004b6af0(*param_1, i), "Error -- Unnamed Mission");
        }
    }
    return n;
}
