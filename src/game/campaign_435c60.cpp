// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Advances the current mission index when the embedded list holds more than
// "current index + 1" consecutive MISSION<n> entries; resets the list cursor
// first. Same scan and side effects as 0x435980.cpp and 0x435c00.cpp.
//
// check.py reports one BAD reference: the tail call at 0x435d02. The machine
// code is byte-identical (100%), but data/symbols.csv records 0x435da0 as the
// bare name "LoadMission", left there by 0x435c00.cpp, which declared it a
// free __stdcall function. That worked in 0x435c00 only because ecx already
// held `this` there. Here the original explicitly loads ecx = this with
// `mov ecx, ebx` (0x435d00) before the call, so 0x435da0 is a __thiscall method
// of this class; a free declaration drops that instruction and cannot match.
// Fix: keep the existing entry and add
//   0x435da0,Mission::LoadMission
// to data/symbols.csv. That is the name its own author will use, since this
// object's class is already recorded as Mission by 0x435c00.cpp.
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
    int unknown_0;                      // +0x0
    char name[0xa08 - 4];               // +0x4
    TdfFile list;                       // +0xa08
    char unknown_a10[0xc18 - 0xa10];
    int field_c18;                      // +0xc18
    int field_c1c;                      // +0xc1c

    int AdvanceMission();
    void LoadMission(char* param);
};

// FUNCTION: 0x435c60
int Mission::AdvanceMission()
{
    char buf[128];
    int n;
    int index = field_c18 + 1;
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
    if (n > index) {
        field_c1c = 0;
        field_c18++;
        LoadMission(0);
        return 1;
    }
    return 0;
}
