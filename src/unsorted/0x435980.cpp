// Decompiled by Opus. Names are provisional.
// Counts the consecutive "MISSION<n>" entries in the embedded list (from
// MISSION0 up) and returns whether there are more than `index` of them. A
// blank name means no missions.
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

class Class_00435980 {
public:
    int unknown_0;
    char name[0xa08 - 4];              // +0x4
    Class_004c3e10 list;               // +0xa08

    int FUN_00435980(int index);
};

// FUNCTION: 0x435980
int Class_00435980::FUN_00435980(int index)
{
    char buf[128];
    int n;
    if (strlen(name) == 0) {
        n = 0;
    } else {
        n = 0;
        sprintf(buf, "MISSION%d", n);
        list.FUN_004c3e10();
        while (((Class_004c3410*)&list)->FUN_004c3410(buf)) {
            n++;
            sprintf(buf, "MISSION%d", n);
            list.FUN_004c3e10();
        }
    }
    return n > index;
}
