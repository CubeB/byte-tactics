// Decompiled by Opus. Names are provisional.
// Slot 1 of a victory condition (vtable 0x4fd8b0, state saved by 0x48f160):
// called for each player's unit event; counts the named unit type down and
// announces the victory condition when the count runs out.
#include <string.h>

#pragma pack(push, 1)
struct Info_0048f0f0 {
    char unknown_0[0x20];
    char name[0x20];                   // +0x20
};

struct Player_0048f0f0 {
    char unknown_0[0x92];
    Info_0048f0f0* info;               // +0x92
    char unknown_96[0xff - 0x96];
    unsigned char kind;                // +0xff
};
#pragma pack(pop)

void __stdcall FUN_0047f1a0(char* str, int flag);

class Condition_0048f0f0 {
public:
    virtual void Unknown_0();
    virtual void FUN_0048f0f0(Player_0048f0f0* player);
    int done;                          // +0x04
    int announced;                     // +0x08
};

class Class_0048f0f0 : public Condition_0048f0f0 {
public:
    char name[0x20];                   // +0x0c
    int count;                         // +0x2c
    void FUN_0048f0f0(Player_0048f0f0* player);
};

// FUNCTION: 0x48f0f0
void Class_0048f0f0::FUN_0048f0f0(Player_0048f0f0* player)
{
    if (count > 0 && player->kind == 1 && _strcmpi(name, player->info->name) == 0) {
        if (--count <= 0) {
            done = 1;
            if (announced == 0) {
                FUN_0047f1a0("Victory Condition", 0);
                announced = 1;
            }
        }
    }
}
