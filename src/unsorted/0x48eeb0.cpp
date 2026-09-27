// Decompiled by Opus. Names are provisional.
#include <string.h>

#pragma pack(push, 1)
struct Info_0048eeb0 {
    char unknown_0[0x20];
    char name[0x20];                 // +0x20
};

struct Player_0048eeb0 {
    char unknown_0[0x92];
    Info_0048eeb0* info;             // +0x92
    char unknown_96[0xff - 0x96];
    unsigned char kind;              // +0xff
};
#pragma pack(pop)

void __stdcall FUN_0047f1a0(char* str, int flag);

// Same family as the victory conditions in 0x48ed50.cpp and 0x48efb0.cpp.
class Class_0048eeb0 {
public:
    virtual void FUN_0048eeb0(Player_0048eeb0* player);
    int done;                        // +0x04
    int announced;                   // +0x08
    char name[0x20];                 // +0x0c
};

// FUNCTION: 0x48eeb0
void Class_0048eeb0::FUN_0048eeb0(Player_0048eeb0* player)
{
    if (player->kind == 1 && _strcmpi(name, player->info->name) == 0) {
        done = 1;
        if (announced == 0) {
            FUN_0047f1a0("Victory Condition", 0);
            announced = 1;
        }
    }
}
