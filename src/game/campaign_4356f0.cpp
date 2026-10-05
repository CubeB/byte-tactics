// Decompiled by Opus. Names are provisional.
// Counts the consecutive "MISSION<n>" entries in the embedded list (from
// MISSION0 up); a blank name means no missions. See 0x435980.cpp.
#include <stdio.h>
#include <string.h>

class Class_004c3e10 {
public:
    char unknown_0[4];
    int field_0x4;

    void ResetCurrentRecord();
};

class Class_004c3410 {
public:
    int SelectRecord(char* name);
};

class Class_004356f0 {
public:
    int unknown_0;
    char name[0xa08 - 4];              // +0x4
    Class_004c3e10 list;               // +0xa08

    int CountMissions();
};

// FUNCTION: 0x4356f0
int Class_004356f0::CountMissions()
{
    char buf[128];
    if (strlen(name) == 0)
        return 0;
    int n = 0;
    while (1) {
        sprintf(buf, "MISSION%d", n);
        list.ResetCurrentRecord();
        if (((Class_004c3410*)&list)->SelectRecord(buf) == 0)
            break;
        n++;
    }
    return n;
}
