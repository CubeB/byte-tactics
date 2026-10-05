// Decompiled by Opus. Names are provisional.
// Console command callback: "move <dx> <dz>" shifts a game position at
// +0x2caa/+0x2cb2 by whole units (<< 20). Each call result goes through a
// local; `g_game->x += f() << 20` loads g_game before the call.
#include <string.h>

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x2caa];
    int x;                             // +0x2caa
    int y;                             // +0x2cae
    int z;                             // +0x2cb2
};
#pragma pack(pop)

extern Game* g_game;
extern char DAT_005119b8[];

// Command arguments.
class CommandArgs {
public:
    int GetIntArg(int index, int fallback);
};

class Class_004b73c0 {
public:
    char* GetArg(int index, char* fallback);
};

// FUNCTION: 0x416860
void __stdcall CmdMove(CommandArgs* args)
{
    if (_strcmpi(((Class_004b73c0*)args)->GetArg(0, DAT_005119b8), "move") == 0) {
        int dx = args->GetIntArg(1, 0);
        g_game->x += dx << 20;
        int dz = args->GetIntArg(2, 0);
        g_game->z += dz << 20;
    }
}
