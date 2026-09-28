// Decompiled by space-bunny-free. Names are provisional.
// PARTIAL (79.9%). What still differs is all one thing, the register
// allocation: the original keeps `this` in edi and gives it a stack home at
// +0x14, so the prologue is
//     mov edi,ecx; xor ebx,ebx; mov [esp+0x14],edi; mov eax,[edi];
//     mov [edi+0xc1c],ebx
// and the loop reloads `this` from +0x14, leaving ebp for `count` and ebx for
// the constant 0 / the loop counter. Here `this` takes ebp, `count` is spilled
// to +0x14 instead, and the constant 0 stays an immediate. That costs: the
// prologue, `lea ebp,[edx+0xb14]` in the type 2/3 branch (here ebp already
// holds `this`, so the +0xb14 address goes through esi and a spill), the
// `mov ebp, eax; cmp ebp, ebx` count test, the loop-head reload of `this` and
// the found-path stores through edi.
// Everything else matches: block order (the type 2/3 branch is laid out first
// and the type 1 branch last, so the source tests `type != 1` and puts the
// mission walk in the else), the 0xd0 frame, `type > 1 && type <= 3` reusing
// the flags of the `cmp eax,1`, the three return points, the loop rotation
// with its extra `test ebp,ebp` guard, and the 200-byte buffer.
// Tried without effect: `if (type == 1) A else if (type > 1 && type <= 3) B`
// (matches the registers but lays A out first), the type 1 branch after the if
// with its own `return 0`, nested ifs instead of `&&`, `0 < count`, p and i at
// function scope, all of the type 1 locals at function scope, a shared vs a
// separate local for the load result and the list head (the shared one is what
// this file uses, it puts `res` at +0x10 as the original does), while loops,
// `!_strcmpi` and `!= 0` spellings.
//
// Loads a mission by name. Type 1 walks the mission list built by 0x435760
// looking for the name and, on a match, resets the mission index and loads
// that mission. Types 2 and 3 load the map and, when the language is not
// "english", put the translated name (0x4c5740) into the name slot at +0xb14,
// keeping the original spelling when the lookup changed nothing. Every other
// type returns 0. The load result and the mission list share one stack slot,
// which is why `res` is passed as the list out-parameter in the type 1 branch.
//
// Suspected original bugs, both kept as the original has them:
// - 0x435ad7: the translation lookup is given the campaign object itself
//   instead of the lowercased copy of the map name, so FUN_004c5740 walks
//   `type` as a string. The lowercased copy is built and then unused.
// - the `field_c1c = 0` store in the mission-loop branch repeats the one at
//   the top of the function.
#include <string.h>

class Class_004c2ea0 {
public:
    int field_0;
    void* current;                      // +4
    int field_8;
};

class Class_004c3e10 {
public:
    void FUN_004c3e10();
};

class Class_00435760 {
public:
    int unknown_0;
    char name[0xa08 - 4];               // +0x4
    Class_004c3e10 list;                // +0xa08

    int FUN_00435760(char** param_1);
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

    int FUN_00435da0(char* map);
};

class Class_00435a20 : public Class_00435c00 {
public:
    int FUN_00435a20(char* map);
};

int FUN_0049f580(void);
char* __stdcall FUN_004c5740(char* text);
void FUN_004d85a0(void* p);

// FUNCTION: 0x435a20
int Class_00435a20::FUN_00435a20(char* map)
{
    int res;

    field_c1c = 0;
    if (type != 1) {
        if (type > 1 && type <= 3) {
            res = FUN_00435da0(map);
            if (res && FUN_0049f580() && _strcmpi((char*)FUN_0049f580(), "english")) {
                char lower[200];
                strcpy(lower, map);
                _strlwr(lower);
                strncpy(text_b14, FUN_004c5740((char*)this), 0xff);
                if (_strcmpi(text_b14, map) == 0)
                    strcpy(text_b14, map);
            } else {
                strcpy(text_b14, map);
            }
            return res;
        }
    } else {
        int count = ((Class_00435760*)this)->FUN_00435760((char**)&res);
        if (count > 0) {
            char* p = (char*)res;
            for (int i = 0; i < count; i++) {
                if (_strcmpi(p, map) == 0) {
                    FUN_004d85a0((void*)res);
                    field_c1c = 0;
                    missionIndex = i;
                    return FUN_00435da0(0);
                }
                p += strlen(p) + 1;
            }
            FUN_004d85a0((void*)res);
        }
    }
    return 0;
}
