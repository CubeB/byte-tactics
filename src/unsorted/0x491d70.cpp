// Decompiled by Opus. Names are provisional.

struct Queue_491d70 {
    char unknown_0[0x18];
    void* current;                     // +0x18
};

#pragma pack(push, 1)
struct Game_491d70 {
    char unknown_0[0x519];
    Queue_491d70 queue;                // +0x519
    char unknown_535[0x2bee - 0x535];
    unsigned char field_2bee;          // +0x2bee
    char unknown_2bef[0x37e9c - 0x2bef];
    short field_37e9c;                 // +0x37e9c
    char unknown_37e9e[2];
    char name[0x1e];                   // +0x37ea0
    unsigned short flags;              // +0x37ebe
};
#pragma pack(pop)

extern Game_491d70* g_game;

int __stdcall FUN_004ab060(Queue_491d70* queue, char* name);
void __stdcall FUN_004a9660(Queue_491d70* queue);

// FUNCTION: 0x491d70
int __stdcall FUN_00491d70(int force)
{
    unsigned short flags = g_game->flags;
    if (((flags & 0x800) || (flags & 0x65) || (g_game->field_2bee & 0xe0)) && force == 0) {
        g_game->flags = flags | 0x10;
        return 0;
    }
    g_game->field_37e9c = 0;
    while (g_game->queue.current) {
        if (FUN_004ab060(&g_game->queue, g_game->name))
            return 1;
        FUN_004a9660(&g_game->queue);
    }
    return 0;
}
