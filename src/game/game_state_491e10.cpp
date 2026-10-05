// Decompiled by Opus. Names are provisional.
// Compare 0x491d70.

struct Queue_00491e10 {
    char unknown_0[0x18];
    void* current;                     // +0x18
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x519];
    Queue_00491e10 queue;              // +0x519
    char unknown_535[0x37e9c - 0x535];
    short field_37e9c;                 // +0x37e9c
    char unknown_37e9e[2];
    char name[0x1e];                   // +0x37ea0
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall FUN_004ab060(Queue_00491e10* queue, char* name);
void __stdcall FUN_004a9660(Queue_00491e10* queue);

// FUNCTION: 0x491e10
void FUN_00491e10()
{
    if (!FUN_004ab060(&g_game->queue, g_game->name)) {
        g_game->field_37e9c = 0;
        FUN_004a9660(&g_game->queue);
    }
}
