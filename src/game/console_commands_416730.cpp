// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x14223];
    int baseX;      // +0x14223
    int baseY;      // +0x14227
    int offsetX;    // +0x1422b
    int offsetY;    // +0x1422f
};
#pragma pack(pop)

extern Game* g_game;

// Command arguments.
class Class_004b73e0 {
public:
    int FUN_004b73e0(int index, int fallback);
};

// FUNCTION: 0x416730
void __stdcall CmdEdge(Class_004b73e0* args)
{
    g_game->offsetX = g_game->baseX - args->FUN_004b73e0(1, 0x20);
    g_game->offsetY = g_game->baseY - args->FUN_004b73e0(2, 0x80);
}
