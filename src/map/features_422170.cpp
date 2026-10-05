// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <windows.h>

#pragma pack(push, 1)
struct Entry_00422170 {
    short next;                        // +0x0
    short prev;                        // +0x2
    void* obj;                         // +0x4
    char unknown_8[0x2c - 0x8];
    unsigned short type;               // +0x2c
    char unknown_2e[0x30 - 0x2e];
};

struct Type_00422170 {
    char unknown_0[0x98];
    void* field_98;                    // +0x98
    char unknown_9c[0xa8 - 0x9c];
    void* field_a8;                    // +0xa8
    char unknown_ac[0xfe - 0xac];
    unsigned char flags;               // +0xfe
    char unknown_ff[0x100 - 0xff];
};

struct Pool_00422170 {
    Entry_00422170* entries;           // +0x0
    void* field_4;                     // +0x4
    int head8;                         // +0x8
    int headC;                         // +0xc
};

struct Game {
    char unknown_0[0x1420b];
    Pool_00422170 pool;                // +0x1420b
    char unknown_1421b[0x14253 - 0x1421b];
    int typeCount;                     // +0x14253
    char unknown_14257[0x1426f - 0x14257];
    Type_00422170* types;              // +0x1426f
};
#pragma pack(pop)

extern Game* g_game;

void __cdecl FUN_004d85a0(void* p);
void __stdcall FreeObjectState(void* obj);

// FUNCTION: 0x422170
void FreeFeaturePool()
{
    int i;
    for (i = 0; i <= 1; i++) {
        int idx = i ? g_game->pool.head8 : g_game->pool.headC;
        while (idx != -1) {
            Entry_00422170* e = &g_game->pool.entries[idx];
            if (!(g_game->types[e->type].flags & 1))
                FreeObjectState(e->obj);
            idx = e->next;
        }
    }

    FUN_004d85a0(g_game->pool.field_4);
    FUN_004d85a0(g_game->pool.entries);
    g_game->pool.field_4 = 0;
    g_game->pool.entries = 0;

    Type_00422170* t = g_game->types;
    for (i = 0; i < g_game->typeCount; i++, t++) {
        if (t->flags & 1) {
            if (t->field_a8) {
                FUN_004d85a0(t->field_a8);
                t->field_a8 = 0;
            }
        } else {
            if (t->field_98) {
                FUN_004d85a0(t->field_98);
                t->field_98 = 0;
            }
        }
    }

    FUN_004d85a0(g_game->types);
    g_game->types = 0;
    g_game->typeCount = 0;
}
