// Decompiled by GPT-6. Names are provisional.
#include <string.h>
struct Class_004b73c0 {
    char pad[0xd0]; int count;
    const char* GetArg(int, const char*);
};
extern int g_aiCommandsEnabled;
extern char DAT_005119b8[];
extern char* g_game;
// The original checks argument 1 for "any" on every iteration.
// FUNCTION: 0x406c90
void __stdcall CmdPlan(Class_004b73c0* args)
{
    g_aiCommandsEnabled = 0;
    for (int i = 1; i < args->count; ++i) {
        if (!_strcmpi(args->GetArg(1, DAT_005119b8), "any")) g_aiCommandsEnabled = 1;
        if (*(int*)(g_game + 0x37eee) == 0 && !_strcmpi(args->GetArg(i, DAT_005119b8), "easy")) g_aiCommandsEnabled = 1;
        if (*(int*)(g_game + 0x37eee) == 1 && !_strcmpi(args->GetArg(i, DAT_005119b8), "medium")) g_aiCommandsEnabled = 1;
        if (*(int*)(g_game + 0x37eee) == 2 && !_strcmpi(args->GetArg(i, DAT_005119b8), "hard")) g_aiCommandsEnabled = 1;
    }
}
