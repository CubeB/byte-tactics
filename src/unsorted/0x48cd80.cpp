// Decompiled by mimo-v2.6-flash. Names are provisional.

#pragma pack(push, 1)
struct Point_0048cd80 {
    int x;                             // +0x0
    int y;                             // +0x4
};

struct Rect_0048cd80 {
    int left;                          // +0x0
    int top;                           // +0x4
    int right;                         // +0x8
    int bottom;                        // +0xc
};

struct UnitDef_0048cd80 {
    char unknown_0[0x176];
    int field_176;                     // +0x176
    int field_17a;                     // +0x17a
    int field_17e;                     // +0x17e
};

struct Unit_0048cd80 {
    char unknown_0[0x92];
    UnitDef_0048cd80* def;             // +0x92
    char unknown_96[0xa6 - 0x96];
    unsigned short field_a6;           // +0xa6
    unsigned short field_a8;           // +0xa8
    char unknown_aa[0x118 - 0xaa];
};

struct Slot_0048cd80 {
    unsigned short field_0;            // +0x0
    int x;                             // +0x2
    int y;                             // +0x6
};

struct Game_0048cd80 {
    char unknown_0[0x2c76];
    Point_0048cd80 view;               // +0x2c76
    char unknown_2c7e[0x142bb - 0x2c7e];
    Rect_0048cd80 rect_142bb;          // +0x142bb
    char unknown_142cb[0x14357 - 0x142cb];
    Unit_0048cd80* units;              // +0x14357
    Unit_0048cd80* units_end;          // +0x1435b
    unsigned short* list;              // +0x1435f
    Slot_0048cd80* list2;              // +0x14363
    int count;                         // +0x14367
    int count2;                        // +0x1436b
    char unknown_1436f[0x37e27 - 0x1436f];
    Rect_0048cd80 rect_37e27;          // +0x37e27
};
#pragma pack(pop)

extern Game_0048cd80* g_game;

int __stdcall FUN_004b6720(Rect_0048cd80* rect, int x, int y);
int __stdcall FUN_0048c6a0(Unit_0048cd80* unit, Point_0048cd80* p);

static inline int FixMul(int a, int b)
{
    return (int)(((__int64)a * b) >> 16);
}

// FUNCTION: 0x48cd80
unsigned short __stdcall FUN_0048cd80(void)
{
    Point_0048cd80* p = &g_game->view;
    unsigned short result = 0;
    if (FUN_004b6720(&g_game->rect_37e27, p->x, p->y)) {
        int best = 0x7fff0000;
        unsigned short* ids = g_game->list;
        if (ids == 0)
            return 0;
        for (int i = 0; i < g_game->count; i++) {
            Unit_0048cd80* u = &g_game->units[ids[i]];
            if (u->field_a6 != 0) {
                if (FUN_0048c6a0(u, p)) {
                    UnitDef_0048cd80* def = u->def;
                    int v = FixMul(def->field_17a, 0x8000) + def->field_17e;
                    v = FixMul(v, def->field_176);
                    if (v < best && p->x >= 0) {
                        result = u->field_a8;
                        best = v;
                    }
                }
            }
        }
    } else if (FUN_004b6720(&g_game->rect_142bb, p->x, p->y)) {
        Slot_0048cd80* s = g_game->list2;
        int n = g_game->count2;
        int best = 99999;
        int py = p->y;
        int px = p->x;
        for (int i = n; i > 0; i--) {
            int dy = s->y - py;
            int dx = s->x - px;
            int d = dx * dx + dy * dy;
            if (d < 4 && d < best) {
                best = d;
                result = s->field_0;
            }
            s++;
        }
    }
    return result;
}
