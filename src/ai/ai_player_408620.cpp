// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
#include <windows.h>

#pragma pack(push, 1)
struct Item_00408620 {                 // 0x249 bytes
    char unknown_0[0x20];
    char name[0x249 - 0x20];           // +0x20
};

struct Game {
    char unknown_0[0x1439b];
    Item_00408620* unitDefs;           // +0x1439b
};
#pragma pack(pop)

struct Obj_00408620 {
    char unknown_0[0x5c];
    int orders;                        // +0x5c
};

extern Game* g_game;

unsigned short __stdcall ChooseBuildOption(int player, Obj_00408620* unit);
void __stdcall QueueBuildOrder(char* name, Obj_00408620* unit, int count);

class BuildTimer {
public:
    char unknown_0[0x10];
    int player;                        // +0x10

    void TryIssueIdleFactoryBuildOrder(Obj_00408620* unit);
};

// FUNCTION: 0x408620
void BuildTimer::TryIssueIdleFactoryBuildOrder(Obj_00408620* unit)
{
    if (unit->orders != 0)
        return;
    unsigned short idx = ChooseBuildOption(player, unit);
    if (idx != 0)
        QueueBuildOrder(g_game->unitDefs[idx].name, unit, 1);
}
