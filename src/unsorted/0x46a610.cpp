// Decompiled by space-bunny-free. Names are provisional.

struct Point16_0046a610 {
    short x;
    short z;
};

struct Vec3_0046a610 {
    int x, y, z;
};

struct Rot16_0046a610 {
    short x, y, z;
};

struct AnimRef_0046a610 {
    unsigned short index;              // +0x0
    unsigned short value;              // +0x2
    char unknown_4[0x8 - 0x4];
    void* src;                         // +0x8
};

struct SpotState_0046a610 {
    char unknown_0[0xc];
    void* owner;                       // +0xc
};

#pragma pack(push, 1)

struct Cell_0046a610 {
    char unknown_0[0x4];
    unsigned char shade;               // +0x4
    char unknown_5[0x8 - 0x5];
    unsigned short feature;            // +0x8
    unsigned short spot;               // +0xa
    unsigned char flags;               // +0xc
};

struct Feature_0046a610 {
    char unknown_0[0x94];
    Point16_0046a610 footprint;        // +0x94
    char unknown_98[0xac - 0x98];
    unsigned short* animTable;         // +0xac
    unsigned short* shadowTable;       // +0xb0
    char unknown_b4[0xcc - 0xb4];
    AnimRef_0046a610 anim;             // +0xcc
    AnimRef_0046a610 shadowAnim;       // +0xd8
    char unknown_e4[0xfe - 0xe4];
    unsigned short drawn : 1;          // +0xfe
    unsigned short over : 1;           // +0xff
    unsigned short hasShadow : 1;
    unsigned short flipped : 1;
    unsigned short unknown_bits4 : 12;
};

struct SpotShadow_0046a610 {
    int unknown_8;
    int unknown_c;
    AnimRef_0046a610 shadow;           // +0x10
};

struct Spot_0046a610 {
    char unknown_0[4];
    SpotState_0046a610* state;         // +0x4
    union {
        Vec3_0046a610 pos;             // +0x8
        SpotShadow_0046a610 alt;       // shadow anim at +0x10
    };
    char unknown_1c[0x20 - 0x1c];
    Rot16_0046a610 rot;                // +0x20
    char unknown_26[0x28 - 0x26];
    Point16_0046a610 cell;             // +0x28
    unsigned short feature;            // +0x2c
    char unknown_2e;
    unsigned char flag0 : 1;           // +0x2f
    unsigned char unknown_30 : 1;
    unsigned char hasShadow : 1;
    unsigned char unknown_32 : 5;
};

struct Unit_0046a610 {
    char unknown_0[0x64];
    Rot16_0046a610 rot;                // +0x64
    Vec3_0046a610 pos;                 // +0x6a
    char unknown_76[0x9e - 0x76];
    SpotState_0046a610* state;         // +0x9e
};

struct Game_0046a610 {
    char unknown_0[0x1420b];
    Spot_0046a610* spots;              // +0x1420b
    Unit_0046a610* unit;               // +0x1420f
    char unknown_14213[0x14233 - 0x14213];
    int width;                         // +0x14233
    char unknown_14237[0x1426f - 0x14237];
    Feature_0046a610* features;        // +0x1426f
    char unknown_14273[0x1431f - 0x14273];
    int scroll_x;                      // +0x1431f
    int scroll_y;                      // +0x14323
    char unknown_14327[0x37f06 - 0x14327];
    unsigned char drawFlags;           // +0x37f06
};

#pragma pack(pop)

extern Game_0046a610* g_game;

void __stdcall FUN_004b7f90(void* dest, void* frame, int x, int y);
void __stdcall FUN_004b8500(void* dest, void* frame, int x, int y);
void* __stdcall FUN_004b7ee0(void* ref);
void* __stdcall FUN_004b7f30(unsigned short* table, int index);
void __stdcall FUN_0045ac20(Unit_0046a610* unit, int shade);

// The object's own anim is drawn mirrored when the feature says so.
static void DrawAnim(Feature_0046a610* f, void* dest, void* frame, int x, int y)
{
    if (f->flipped)
        FUN_004b8500(dest, frame, x, y);
    else
        FUN_004b7f90(dest, frame, x, y);
}

// The extra overlay has its own mirror flag.
static void DrawShadow(Feature_0046a610* f, void* dest, void* frame, int x, int y)
{
    if (f->hasShadow)
        FUN_004b8500(dest, frame, x, y);
    else
        FUN_004b7f90(dest, frame, x, y);
}

// FUNCTION: 0x46a610
void __stdcall FUN_0046a610(void* dest, Cell_0046a610* cell, int ix, int iy)
{
    Feature_0046a610* f = &g_game->features[cell->feature];
    int x = f->footprint.x * 16 / 2;
    x += (ix + 8) * 16;
    x -= g_game->scroll_x;
    Cell_0046a610* next = &cell[g_game->width];
    int shade = (cell->shade + cell[1].shade + next->shade + next[1].shade) >> 3;
    int y = f->footprint.z * 16 / 2 - shade + (iy + 2) * 16 - g_game->scroll_y;
    if (cell->flags & 1) {
        Spot_0046a610* spot = &g_game->spots[cell->spot];
        if (f->drawn) {
            if (spot->hasShadow && (g_game->drawFlags & 0x10))
                DrawAnim(f, dest, FUN_004b7ee0(&spot->alt.shadow), x, y);
            DrawAnim(f, dest, FUN_004b7ee0(&spot->state), x, y);
        } else {
            Unit_0046a610* u = g_game->unit;
            SpotState_0046a610* st = spot->state;
            u->state = st;
            st->owner = u;
            u->rot = spot->rot;
            u->pos = spot->pos;
            FUN_0045ac20(u, shade);
        }
    } else {
        if (f->over) {
            if (f->shadowTable && (g_game->drawFlags & 0x10))
                DrawAnim(f, dest, FUN_004b7ee0(&f->shadowAnim), x, y);
            if (f->animTable)
                DrawShadow(f, dest, FUN_004b7ee0(&f->anim), x, y);
        } else {
            if (f->shadowTable && (g_game->drawFlags & 0x10))
                DrawAnim(f, dest, FUN_004b7f30(f->shadowTable, 0), x, y);
            if (f->animTable)
                DrawShadow(f, dest, FUN_004b7f30(f->animTable, 0), x, y);
        }
    }
}
