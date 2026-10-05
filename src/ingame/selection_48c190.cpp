// Decompiled by deepseek-v4.1-flash. Names are provisional.

#pragma pack(push, 1)
struct Unit {
    char unknown_0[0xa8];
    unsigned short index;               // +0xa8
    char unknown_aa[0x110 - 0xaa];
    union {
        unsigned int flags;             // +0x110
        struct {
            unsigned int unknown_0 : 4;
            unsigned int selected : 1;
            unsigned int unknown_5 : 27;
        };
    };
    char unknown_114[0x118 - 0x114];
};

struct Player_0048c190 {                // 0x14b bytes
    char unknown_0[0x6f];
    unsigned short firstIndex;           // +0x6f
    unsigned short lastIndex;            // +0x71
    char unknown_73[0x14b - 0x73];
};

struct Game {
    char unknown_0[0x1b63];
    Player_0048c190 players[10];        // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char player;               // +0x2a42
    char unknown_2a43[0x14357 - 0x2a43];
    Unit* units;                        // +0x14357
};
#pragma pack(pop)

extern Game* g_game;

static inline Unit* GetUnit_0048c190(unsigned short i)
{
    return i == 0 ? 0 : g_game->units + i;
}

// FUNCTION: 0x48c190
Unit* __stdcall FindNextSelectedUnit(Unit* unit, int dir)
{
    Player_0048c190* p = &g_game->players[g_game->player];
    unsigned short x = unit == 0 ? 0 : unit->index;
    if (x < p->firstIndex || x > p->lastIndex) {
        x = p->firstIndex;
    }
    if (dir) {
        short j = x;
        while ((unsigned short)j != p->firstIndex) {
            --j;
            Unit* v = GetUnit_0048c190((unsigned short)j);
            if (v->selected) {
                return v;
            }
        }
        j = p->lastIndex + 1;
        while ((unsigned short)j != x) {
            --j;
            Unit* v = GetUnit_0048c190((unsigned short)j);
            if (v->selected) {
                return v;
            }
        }
    } else {
        short j;
        for (j = x; (unsigned short)j != p->lastIndex; j++) {
            Unit* v = GetUnit_0048c190((unsigned short)(j + 1));
            if (v->selected) {
                return v;
            }
        }
        for (j = p->firstIndex - 1; (unsigned short)j != x; j++) {
            Unit* v = GetUnit_0048c190((unsigned short)(j + 1));
            if (v->selected) {
                return v;
            }
        }
    }
    return 0;
}
