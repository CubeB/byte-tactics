// Decompiled by Opus. Names are provisional.
#include <stdio.h>

class Class_00435100 {
public:
    int FUN_00435100();
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x391e9];
    Class_00435100* field_391e9;       // +0x391e9
};
#pragma pack(pop)

struct Player_00463e50 {
    char unknown_0[0x49];
    char name[1];                      // +0x49
};

extern Game* g_game;
extern char DAT_005119b8[];

void __stdcall FUN_00453360(char* param_1);
void __stdcall FUN_0046c810(char* param_1);
void __stdcall FUN_00463ca0(char* param_1, int param_2, int param_3, int param_4);

// FUNCTION: 0x463e50
void __stdcall FUN_00463e50(Player_00463e50* from, char* text, int param_3, char* to)
{
    char buf[200];

    sprintf(buf, "<%s%s%s> %s", from->name, to ? "->" : DAT_005119b8,
            to ? to : DAT_005119b8, text);
    FUN_00453360(buf);
    if (g_game->field_391e9->FUN_00435100() == 3) {
        FUN_0046c810(buf);
    }
    FUN_00463ca0(buf, param_3, 0, 10);
}
