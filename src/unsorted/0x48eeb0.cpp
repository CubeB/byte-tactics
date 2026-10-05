// Decompiled by Opus. Names are provisional.
#include <string.h>

#pragma pack(push, 1)
struct Info_0048eeb0 {
    char unknown_0[0x20];
    char name[0x20];                 // +0x20
};

struct Unit {
    char unknown_0[0x92];
    Info_0048eeb0* info;             // +0x92
    char unknown_96[0xff - 0x96];
    unsigned char kind;              // +0xff
};
#pragma pack(pop)

void __stdcall FUN_0047f1a0(char* str, int flag);

// The "capture unit type" victory condition (vtable 0x4fd8e8, state saved by
// 0x48ef00); this is its slot 2. Same family as the victory conditions in
// 0x48ed50.cpp and 0x48efb0.cpp.
class Class_0048eeb0 {
public:
    virtual void FUN_0048ea20(Unit* unit);
    int done;                        // +0x04
    int announced;                   // +0x08
    char name[0x20];                 // +0x0c
};

// FUNCTION: 0x48eeb0
void Class_0048eeb0::FUN_0048ea20(Unit* unit)
{
    if (unit->kind == 1 && _strcmpi(name, unit->info->name) == 0) {
        done = 1;
        if (announced == 0) {
            FUN_0047f1a0("Victory Condition", 0);
            announced = 1;
        }
    }
}
