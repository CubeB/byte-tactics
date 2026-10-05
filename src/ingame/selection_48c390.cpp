// Decompiled by space-bunny-free. Names are provisional.
// MATCH, 773 of 773 bytes. (Was 96.8%: the size was already exact and the whole
// residual was one group of loads at the head of the function, in a different
// order. It was a source-STATEMENT ordering problem, not a scheduling problem,
// and the previous note here called it unfixable after 647 variants. It was not:
// the previous sweep varied how the two halvings were interleaved with the
// subtractions, and that is the wrong axis. The axis is the order of the four
// corner declarations, and only one interleaving works, `ymin, xmin, ymax, xmax`.)
//
// The lever, precisely. The original emits, for the group:
//   mov eax,[0x1431f] scroll_x      mov edi,[0x2c92] rect_x1
//   mov ebx,[0x2c96] rect_x2        mov ebp,[0x2c9e] rect_y2
//   mov edx,[0x2c9a] rect_y1        mov ecx,[0x14323] scroll_y
// then `sar ebx,1`/`sub edx,ebx` and `sar eax,1`/`sub ebx,eax`. So the second
// corner's fields (0x2c9a and 0x2c96) are hoisted up with the FIRST corner's,
// not the fourth's. `ymin, ymax, xmin, xmax` puts the second corner third and
// MSVC sinks its loads below the two `sub`s, which is the 96.8% version.
// Interleaving them as `ymin, xmin, ymax, xmax` gives the original's grouping,
// the two spills `mov [esp+0x14],edx` / `mov [esp+0x10],ebp` fall into the
// original's order at the same time, and it matches.
//
// The other half of the fix is dropping the two `sx`/`sy` temporaries. With
// them, `scroll_x` and `scroll_y` are named locals, which changes which
// register MSVC gives the second corner's halving; reading `g_game->scroll_x`
// and `g_game->scroll_y` straight out of the two corner expressions matches.
// So: `ymin, ymax, xmin, xmax` WITH the temporaries is 96.8%, and
// `ymin, xmin, ymax, xmax` with them is 88.4%: the two changes are not
// independent, and the earlier sweep never tried this pair at all.
//
// Rubber-band unit selection: converts the drag rectangle in map units to
// screen coordinates, marks every unit of the local player inside it
// selected, then normalises the player's selection state and reports how
// many units ended up selected.
//
// Suspected original bug: none. Every field offset, sign and comparison
// re-derives from the operand bytes, and `0x2c9a`/`0x2c9e` really are `top`
// and `bottom` while `0x2c92`/`0x2c96` are `left` and `right`, so the halving
// is applied to the horizontal edges only, as the code implies.

#pragma pack(push, 1)
struct Unit {
    char unknown_0[0x6c];
    short x;                            // +0x6c
    char unknown_6e[0x70 - 0x6e];
    short z;                            // +0x70
    char unknown_72[0x74 - 0x72];
    short y;                            // +0x74
    char unknown_76[0x86 - 0x76];
    Unit* parent;                       // +0x86
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
    Unit* first;                        // +0x67
    Unit* last;                         // +0x6b
    char unknown_6f[0x14b - 0x6f];
};

struct Game {
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
    Unit* units_begin;                  // +0x14357
    Unit* units_end;                    // +0x1435b
    unsigned short* list;               // +0x1435f
    char unknown_14363[0x14367 - 0x14363];
    int count;                          // +0x14367
    char unknown_1436b[0x37e9c - 0x1436b];
    unsigned short flag_37e9c;          // +0x37e9c
    char unknown_37e9e[0x37ebe - 0x37e9e];
    unsigned char flag_37ebe;           // +0x37ebe
};
#pragma pack(pop)

extern Game* g_game;
extern char s_SelectMultipleUnits_00508d8c[];

int __stdcall FUN_00491d70(int a);
int __stdcall QueueUnitSpeech(Unit* unit, int a, int b);
int __stdcall PlaySoundByName(char* msg, int a);

// FUNCTION: 0x48c390
int __stdcall FUN_0048c390(void* param_1)
{
    int found = 0;
    int ymin = (g_game->rect_x1 - g_game->scroll_x) + 0x80;
    int xmin = ((g_game->rect_y1 - (g_game->rect_x2 >> 1)) - g_game->scroll_y) + 0x20;
    int ymax = (g_game->rect_y2 - g_game->scroll_x) + 0x80;
    int xmax = ((g_game->rect_y3 - (g_game->rect_x3 >> 1)) - g_game->scroll_y) + 0x20;
    int t;
    if (ymin > ymax) { t = ymin; ymin = ymax; ymax = t; }
    if (xmin > xmax) { t = xmin; xmin = xmax; xmax = t; }
    int toggle = (((Select_0048c390*)param_1)->flags >> 2) & 1;
    if (!toggle) {
        for (Unit* v = g_game->units_begin; v <= g_game->units_end; v++)
            v->flags &= 0xffffff2f;
        FUN_00491d70(0);
    }
    Player_0048c390* p = &g_game->players[g_game->player];
    int count = 0;
    Unit* last;
    for (Unit* u = p->first; u <= p->last; u++) {
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
            if (u->selected) { last = u; count++; }
        }
    }
    g_game->flag_37e9c = 0;
    for (Unit* v = g_game->units_begin; v <= g_game->units_end; v++)
        v->flags &= 0xffffff3f;
    unsigned short* list = g_game->list;
    for (int i = 0; i < g_game->count; i++) {
        Unit* v = &g_game->units_begin[list[i]];
        if (v->owner == g_game->player)
            v->state = 1;
    }
    if (found)
        g_game->flag_37ebe |= 0x10;
    if (!count)
        return 0;
    if (count == 1) {
        QueueUnitSpeech(last, 1, 0);
        return 1;
    }
    PlaySoundByName(s_SelectMultipleUnits_00508d8c, 0);
    return 1;
}
