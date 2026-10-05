// Decompiled by Opus. Names are provisional.
// Frees the game's block table at +0x148e3 (each of its count entries, then
// the table itself) and the buffer at +0x148eb, clearing the pointers and
// the size at +0x148e7.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x148df];
    int count;                         // +0x148df
    int** blocks;                      // +0x148e3
    int size;                          // +0x148e7
    int* data;                         // +0x148eb
};
#pragma pack(pop)

extern Game* g_game;

void __cdecl FUN_004d85a0(int* param_1);

// FUNCTION: 0x42a570
void FUN_0042a570()
{
    for (int i = 0; i < g_game->count; i++) {
        FUN_004d85a0(g_game->blocks[i]);
        g_game->blocks[i] = 0;
    }
    FUN_004d85a0((int*)g_game->blocks);
    g_game->blocks = 0;
    if (g_game->data) {
        FUN_004d85a0(g_game->data);
        g_game->data = 0;
        g_game->size = 0;
    }
}
