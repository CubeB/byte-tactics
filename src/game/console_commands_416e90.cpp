// Decompiled by GPT-6 Astra. Names are provisional.
#include <string.h>
class Class_004b73c0 {
public:
    char data[0xd0]; int count;
    const char* FUN_004b73c0(int,const char*);
};
#pragma pack(push,1)
struct Game { char pad0[0x37f2f]; unsigned short flags; };
#pragma pack(pop)
extern Game* g_game;
extern char DAT_005119b8[];
// FUNCTION: 0x416e90
void __stdcall FUN_00416e90(Class_004b73c0* args)
{
    if (args->count==6 && !strcmp(args->FUN_004b73c0(1,DAT_005119b8),"Film") &&
        !strcmp(args->FUN_004b73c0(2,DAT_005119b8),"Chris") &&
        !strcmp(args->FUN_004b73c0(3,DAT_005119b8),"Include") &&
        !strcmp(args->FUN_004b73c0(4,DAT_005119b8),"Reload") &&
        !strcmp(args->FUN_004b73c0(5,DAT_005119b8),"Assert"))
        g_game->flags|=2;
    else g_game->flags&=~2;
}
