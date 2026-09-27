// Decompiled by DeepSeek V4.1 Flash. Names are provisional.

#pragma pack(push, 1)
struct Entry_0041d1f0 {
    int type;                          // +0x0
    int id;                            // +0x4
    short x;                           // +0x8
    short z;                           // +0xa
};

class Class_0041d1f0_net {
public:
    char unknown_0[0xdb4];
    Entry_0041d1f0* entries;           // +0xdb4
    int entry_count;                   // +0xdb8
};

struct Game_41d1f0 {
    char unknown_0[0x14281];
    unsigned short flags_14281;        // +0x14281
    char unknown_14283[0x142f1 - 0x14283];
    unsigned char flags_142f1;         // +0x142f1
    char unknown_142f2[0x1431f - 0x142f2];
    int x;                             // +0x1431f
    int y;                             // +0x14323
    int x2;                            // +0x14327
    int y2;                            // +0x1432b
    char unknown_1432f[0x37e37 - 0x1432f];
    int viewWidth;                     // +0x37e37
    int viewHeight;                    // +0x37e3b
    char unknown_37e3f[0x391e9 - 0x37e3f];
    Class_0041d1f0_net* net;           // +0x391e9
};
#pragma pack(pop)

extern Game_41d1f0* g_game;

void FUN_0041c3c0(void);

static inline void SetPos(int x, int y)
{
    g_game->x = x;
    g_game->y = y;
}

// FUNCTION: 0x41d1f0
void FUN_0041d1f0()
{
    int i = 0;
    Entry_0041d1f0* e = g_game->net->entries;
    int count = g_game->net->entry_count;
    for (; i < count; i++, e++) {
        if (e->type == 1 && e->id == 0) {
            SetPos(e->x - g_game->viewWidth / 2, e->z - g_game->viewHeight / 2);
            g_game->flags_142f1 |= 2;
            FUN_0041c3c0();
            g_game->x2 = g_game->x;
            g_game->y2 = g_game->y;
            g_game->flags_14281 &= 0xfff7;
            return;
        }
    }
}
