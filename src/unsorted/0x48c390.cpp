// Decompiled by space-bunny-free. Names are provisional.
// Rubber-band unit selection: converts the drag rectangle in map units to
// screen coordinates, marks every unit of the local player inside it
// selected, then normalises the player's selection state and reports how
// many units ended up selected.
// Still differs from the original in 8.5% (size is exact at 773 bytes):
//  * the six loads of the drag-rectangle corner fields and of scroll_y are
//    still scheduled slightly differently (the original hoists 0x2c96 and
//    0x14323 above the two `sub`s, this version hoists 0x2c96 below them),
//    and the two spills `mov [esp+0x10],ebp` / `mov [esp+0x14],edx` come out
//    in the opposite order;
//  * three locals land in the wrong frame slots: the original has
//    toggle at +0x20, the player pointer at +0x24 and `last` at +0x28, this
//    version has last at +0x20, toggle at +0x24 and the player at +0x28.
//    Re-declaring them (uninitialised declarations moved above the min/max
//    swaps, or `last` left unassigned) was tried and scores 87.0 or worse.
//  * `mov ecx, [esp+0x30]` vs `mov eax, [esp+0x30]` for the parameter load.

#pragma pack(push, 1)
struct Unit_0048c390 {
    char unknown_0[0x6c];
    short x;                            // +0x6c
    char unknown_6e[0x70 - 0x6e];
    short z;                            // +0x70
    char unknown_72[0x74 - 0x72];
    short y;                            // +0x74
    char unknown_76[0x86 - 0x76];
    Unit_0048c390* parent;              // +0x86
    char unknown_8a[0xfb - 0x8a];
    int unknown_fb;                     // +0xfb
    unsigned char owner;               // +0xff
    char unknown_100[0x104 - 0x100];
    float unknown_104;                  // +0x104
    char unknown_108[0x110 - 0x108];
    union {
        unsigned int flags;             // +0x110
        struct {
            unsigned int unknown_0 : 4;
            unsigned int selected : 1;   // bit 4
            unsigned int unknown_5 : 1;  // bit 5
            unsigned int state : 2;      // bits 6-7
            unsigned int unknown_8 : 22;
            unsigned int unknown_30 : 1; // bit 30
            unsigned int unknown_31 : 1;
        };
    };
    char unknown_114[0x118 - 0x114];
};

struct Select_0048c390 {
    char unknown_0[8];
    unsigned int flags;                 // +8
};

struct Player_0048c390 {
    char unknown_0[0x67];
    Unit_0048c390* first;               // +0x67
    Unit_0048c390* last;                // +0x6b
    char unknown_6f[0x14b - 0x6f];
};

struct Game_0048c390 {
    char unknown_0[0x1b63];
    Player_0048c390 players[10];        // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char player;               // +0x2a42
    char unknown_2a43[0x2c92 - 0x2a43];
    int rect_x1;                        // +0x2c92
    int rect_x2;                        // +0x2c96
    int rect_y1;                        // +0x2c9a
    int rect_y2;                        // +0x2c9e
    int rect_x3;                        // +0x2ca2
    int rect_y3;                        // +0x2ca6
    char unknown_2caa[0x1431f - 0x2caa];
    int scroll_x;                       // +0x1431f
    int scroll_y;                       // +0x14323
    char unknown_14327[0x14357 - 0x14327];
    Unit_0048c390* units_begin;         // +0x14357
    Unit_0048c390* units_end;           // +0x1435b
    unsigned short* list;               // +0x1435f
    char unknown_14363[0x14367 - 0x14363];
    int count;                          // +0x14367
    char unknown_1436b[0x37e9c - 0x1436b];
    unsigned short flag_37e9c;          // +0x37e9c
    char unknown_37e9e[0x37ebe - 0x37e9e];
    unsigned char flag_37ebe;           // +0x37ebe
};
#pragma pack(pop)

extern Game_0048c390* g_game;
extern char s_SelectMultipleUnits_00508d8c[];

int __stdcall FUN_00491d70(int a);
int __stdcall FUN_0047f780(Unit_0048c390* unit, int a, int b);
int __stdcall FUN_0047f1a0(char* msg, int a);

// FUNCTION: 0x48c390
int __stdcall FUN_0048c390(void* param_1)
{
    int found = 0;
    int sx = g_game->scroll_x;
    int sy = g_game->scroll_y;
    int ymin = (g_game->rect_x1 - sx) + 0x80;
    int ymax = (g_game->rect_y2 - sx) + 0x80;
    int xmin = ((g_game->rect_y1 - (g_game->rect_x2 >> 1)) - sy) + 0x20;
    int xmax = ((g_game->rect_y3 - (g_game->rect_x3 >> 1)) - sy) + 0x20;
    int t;
    if (ymin > ymax) { t = ymin; ymin = ymax; ymax = t; }
    if (xmin > xmax) { t = xmin; xmin = xmax; xmax = t; }
    int toggle = (((Select_0048c390*)param_1)->flags >> 2) & 1;
    if (!toggle) {
        for (Unit_0048c390* v = g_game->units_begin; v <= g_game->units_end; v++)
            v->flags &= 0xffffff2f;
        FUN_00491d70(0);
    }
    Player_0048c390* p = &g_game->players[g_game->player];
    int count = 0;
    Unit_0048c390* last = 0;
    for (Unit_0048c390* u = p->first; u <= p->last; u++) {
        if ((u->flags & 0x20) && u->unknown_104 == 0.0f && !u->unknown_fb &&
            (!u->parent || (u->parent->flags & 0x40000000))) {
            int sy = (u->x - g_game->scroll_x) + 0x80;
            int sx = (u->y - g_game->scroll_y - (u->z >> 1)) + 0x20;
            if (sy >= ymin && sy <= ymax && sx >= xmin && sx <= xmax) {
                if (!toggle)
                    u->selected = 1;
                else
                    u->selected = !u->selected;
                found = 1;
            }
        }
        if (u->selected) { last = u; count++; }
    }
    g_game->flag_37e9c = 0;
    for (Unit_0048c390* v = g_game->units_begin; v <= g_game->units_end; v++)
        v->flags &= 0xffffff3f;
    unsigned short* list = g_game->list;
    for (int i = 0; i < g_game->count; i++) {
        Unit_0048c390* v = &g_game->units_begin[list[i]];
        if (v->owner == g_game->player)
            v->state = 1;
    }
    if (found)
        g_game->flag_37ebe |= 0x10;
    if (!count)
        return 0;
    if (count == 1) {
        FUN_0047f780(last, 1, 0);
        return 1;
    }
    FUN_0047f1a0(s_SelectMultipleUnits_00508d8c, 0);
    return 1;
}
