// Decompiled by Opus. Names are provisional.
// Counts the consecutive "MISSION<n>" entries in the embedded list (from
// MISSION0 up) and returns whether there are more than `index` of them. A
// blank name means no missions.
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

    int MissionExists(int index);
};

// FUNCTION: 0x435980
int Mission::MissionExists(int index)
{
    char buf[128];
    int n;
    if (strlen(name) == 0) {
        n = 0;
    } else {
        n = 0;
        sprintf(buf, "MISSION%d", n);
        list.ResetCurrentRecord();
        while (((TdfFile*)&list)->SelectRecord(buf)) {
            n++;
            sprintf(buf, "MISSION%d", n);
            list.ResetCurrentRecord();
        }
    }
    return n > index;
}
