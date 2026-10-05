// Decompiled by Opus. Names are provisional.
// Counts the consecutive "MISSION<n>" entries in the embedded list (from
// MISSION0 up); a blank name means no missions. See 0x435980.cpp.
#include <stdio.h>
#include <string.h>

class TdfFile {
public:
    char unknown_0[4];
    int field_0x4;

    void ResetCurrentRecord();
    int SelectRecord(char* name);
};

class Mission {
public:
    int unknown_0;
    char name[0xa08 - 4];              // +0x4
    TdfFile list;                      // +0xa08

    int CountMissions();
};

// FUNCTION: 0x4356f0
int Mission::CountMissions()
{
    char buf[128];
    if (strlen(name) == 0)
        return 0;
    int n = 0;
    while (1) {
        sprintf(buf, "MISSION%d", n);
        list.ResetCurrentRecord();
        if (((TdfFile*)&list)->SelectRecord(buf) == 0)
            break;
        n++;
    }
    return n;
}
