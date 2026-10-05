// Decompiled by DeepSeek V4.1 Flash. Names are provisional.

#pragma pack(push, 1)
struct Unit {
    char unknown_0[0x6c];
    short field_6c;                     // +0x6c
    char unknown_6e[0x74 - 0x6e];
    short field_74;                     // +0x74
    char unknown_76[0x110 - 0x76];
    unsigned int flags;                 // +0x110
    char unknown_114[0x118 - 0x114];
};

struct Entry_00463f60 {                 // 0x48 bytes
    char unknown_0[0x44];
    unsigned short unit;                // +0x44
    char unknown_46;
    unsigned char flags;                // +0x47
};

struct Game {
    char unknown_0[0x12ef];
    Entry_00463f60 entries[30];         // +0x12ef
    char unknown_1b5f[0x2a3e - 0x1b5f];
    unsigned short tail;                // +0x2a3e
    unsigned short head;                // +0x2a40
    char unknown_2a42[0x14357 - 0x2a42];
    Unit* units;                        // +0x14357
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall FUN_0041c7c0(int a, int b, int c);

// FUNCTION: 0x463f60
int FUN_00463f60(void)
{
    Game* g = g_game;
    int i = g->head;
    int end = g->tail;
    while (end != i) {
        Entry_00463f60* e = &g->entries[i];
        unsigned short id = e->unit;
        if (id != 0 && (e->flags & 0x10) == 0) {
            Unit* u = &g->units[id];
            if (u->flags & 0x10000000) {
                e->flags |= 0x30;
                FUN_0041c7c0(u->field_6c, u->field_74, 1);
                return 1;
            }
        }
        i++;
        if (i == 30)
            i = 0;
    }
    return 0;
}
