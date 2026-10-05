// Decompiled by GPT-6. Names are provisional.
#include <string.h>
struct Class_004b73c0 {
    char pad[0xd0]; int count;
    const char* FUN_004b73c0(int, const char*);
};
extern int DAT_00501774;
extern char DAT_005119b8[];
extern char* g_game;
// The original checks argument 1 for "any" on every iteration.
// FUNCTION: 0x406c90
void __stdcall FUN_00406c90(Class_004b73c0* args)
{
    DAT_00501774 = 0;
    for (int i = 1; i < args->count; ++i) {
        if (!_strcmpi(args->FUN_004b73c0(1, DAT_005119b8), "any")) DAT_00501774 = 1;
        if (*(int*)(g_game + 0x37eee) == 0 && !_strcmpi(args->FUN_004b73c0(i, DAT_005119b8), "easy")) DAT_00501774 = 1;
        if (*(int*)(g_game + 0x37eee) == 1 && !_strcmpi(args->FUN_004b73c0(i, DAT_005119b8), "medium")) DAT_00501774 = 1;
        if (*(int*)(g_game + 0x37eee) == 2 && !_strcmpi(args->FUN_004b73c0(i, DAT_005119b8), "hard")) DAT_00501774 = 1;
    }
}
