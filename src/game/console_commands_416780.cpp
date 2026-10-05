// Decompiled by Opus. Names are provisional.
// Console command: sets two fields of a game sub-object, the second from a
// 16.16 fixed-point number.
#include <stdlib.h>

struct Obj_00416780 {
    char unknown_0[0x48];
    int value;                         // +0x48
    char unknown_4c[0x54 - 0x4c];
    int fixed;                         // +0x54
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x14207];
    Obj_00416780* obj;                 // +0x14207
};
#pragma pack(pop)

extern Game* g_game;
extern char DAT_005119b8[];

// Command arguments.
class Class_004b73e0 {
public:
    char* args[0x34];                  // +0x00
    int count;                         // +0xd0
    int FUN_004b73e0(int index, int fallback);
};

class Class_004b73c0 {
public:
    char* FUN_004b73c0(int index, char* fallback);
};

// FUNCTION: 0x416780
void __stdcall CmdSearch(Class_004b73e0* args)
{
    if (args->FUN_004b73e0(1, 0)) {
        g_game->obj->value = args->FUN_004b73e0(1, 0);
    }
    if (args->count == 3) {
        g_game->obj->fixed = (int)(atof(((Class_004b73c0*)args)->FUN_004b73c0(2, DAT_005119b8)) * 65536.0);
    }
}
