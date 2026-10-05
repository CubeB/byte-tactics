// Decompiled by space-bunny-free. Names are provisional.
// Loads a mission by name. Type 1 walks the mission list built by 0x435760
// looking for the name and, on a match, resets the mission index and loads
// that mission. Types 2 and 3 load the map and, when the language is not
// "english", put the translated name (0x4c5740) into the name slot at +0xb14,
// keeping the original spelling when the lookup changed nothing. Every other
// type returns 0. The load result and the mission list share one stack slot,
// which is why `res` is passed as the list out-parameter in the type 1 branch.
//
// The single thing that decides the register allocation here is what
// 0x4c5740 is given. Handed `(char*)this` the lowercased copy is dead after
// the `_strlwr`, so nothing is live across the call: `this` then takes ebp with
// no stack home, `count` is spilled to +0x14 and the loop is not rotated
// (79.9%). Handed the buffer, `&lower` is live across the call, and the
// allocator then keeps `this` in edi with its home at +0x14, `count` in ebp
// and the 0 in ebx from the prologue, and rotates the mission loop so its head
// is the reload of `this` that the inlined `strlen` clobbers.
//
// Tried without effect: `if (type == 1) A else if (type > 1 && type <= 3) B`
// (matches the registers but lays A out first), the type 1 branch after the if
// with its own `return 0`, nested ifs instead of `&&`, `0 < count`, `p` and `i`
// at function scope, a `self = this` local used for every access, a shared vs
// a separate local for the load result and the list head (the shared one is
// what this file uses, it puts `res` at +0x10 as the original does), while
// loops, `!_strcmpi` and `!= 0` spellings.
//
// The `field_c1c = 0` store in the mission-loop branch repeats the one at the
// top of the function, kept as the original has it.
#include <string.h>

class Class_004c2ea0 {
public:
    int field_0;
    void* current;                      // +4
    int field_8;
};

class Class_004c3e10 {
public:
    void ResetCurrentRecord();
};

class Class_00435760 {
public:
    int unknown_0;
    char name[0xa08 - 4];               // +0x4
    Class_004c3e10 list;                // +0xa08

    int BuildMissionList(char** param_1);
};

class Class_00435c00 {
public:
    int type;                           // +0x0
    char campaign[0x100];               // +0x4
    char names[9][0x100];               // +0x104
    int exists;                         // +0xa04
    Class_004c2ea0 list;                // +0xa08
    char missionName[0x100];            // +0xa14
    char text_b14[0x100];               // +0xb14
    char* briefing;                     // +0xc14
    int missionIndex;                   // +0xc18
    int field_c1c;                      // +0xc1c

    int LoadMission(char* map);
};

class Class_00435a20 : public Class_00435c00 {
public:
    int LoadMissionByName(char* map);
};

int FUN_0049f580(void);
char* __stdcall Translate(char* text);
void __cdecl FUN_004d85a0(void* p);

// FUNCTION: 0x435a20
int Class_00435a20::LoadMissionByName(char* map)
{
    int res;

    field_c1c = 0;
    if (type != 1) {
        if (type > 1 && type <= 3) {
            res = LoadMission(map);
            if (res && FUN_0049f580() && _strcmpi((char*)FUN_0049f580(), "english")) {
                char lower[200];
                strcpy(lower, map);
                _strlwr(lower);
                strncpy(text_b14, Translate(lower), 0xff);
                if (_strcmpi(text_b14, map) == 0)
                    strcpy(text_b14, map);
            } else {
                strcpy(text_b14, map);
            }
            return res;
        }
    } else {
        int count = ((Class_00435760*)this)->BuildMissionList((char**)&res);
        if (count > 0) {
            char* p = (char*)res;
            for (int i = 0; i < count; i++) {
                if (_strcmpi(p, map) == 0) {
                    FUN_004d85a0((void*)res);
                    field_c1c = 0;
                    missionIndex = i;
                    return LoadMission(0);
                }
                p += strlen(p) + 1;
            }
            FUN_004d85a0((void*)res);
        }
    }
    return 0;
}
