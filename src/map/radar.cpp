// Decompiled by space-bunny-free, deepseek-v4.1-flash, Opus, Claude Opus 5.5, GPT-6, GPT-6.1-sol and mimo-v2.6-pro. Names are provisional.
// The radar: builds the minimap picture from the terrain icon map, keeps its
// view and zoom, and draws the units, projectiles and fog over it.
//
// <stdio.h>, <math.h> and <ddraw.h> stay: their symbol ids decide operand order
// in 0x466780, 0x466b70 and 0x466dc0.
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <ddraw.h>

struct Pic_4665d0 {
    unsigned short w;                   // +0x0
    unsigned short h;                   // +0x2
    unsigned short x;                   // +0x4
    unsigned short y;                   // +0x6
};

struct Point_4665d0 {
    int x;
    int y;
};

struct Quad_4665d0 {
    Point_4665d0 p[4];
};

struct Surface_4665d0 {
    int width;                          // +0x0
    int height;                         // +0x4
    int pitch;                          // +0x8
    int bits;                           // +0xc
    int field_10;                       // +0x10
    int field_14;                       // +0x14
    unsigned short x;                   // +0x18
    unsigned short y;                   // +0x1a
    char unknown_1c[0x10];
    unsigned int flag0 : 1;             // +0x2c
    unsigned int flag1 : 1;
};

#pragma pack(push, 1)
struct Frame_004b8ae0 {
    unsigned short a;                   // +0x0
    unsigned short b;                   // +0x2
    unsigned short c;                   // +0x4
    unsigned short d;                   // +0x6
    unsigned char flag8;                // +0x8
    unsigned char flag9;                // +0x9
    unsigned char flaga;                // +0xa
    unsigned char flagb;                // +0xb
    char unknown_c[4];                  // +0xc
    int unknown_10;                     // +0x10
    char unknown_14[4];                 // +0x14
};

struct IconSet_00466780 {
    int count;                      // +0x0
    unsigned char (*cell)[32][32];  // +0x4
};

struct Fx_00466c20 {
    char unknown_0[0xcc];
    unsigned char* colorMap;         // +0xcc
};

struct Shot_00466dc0;

union Flags110_00466dc0 {
    unsigned int all;
    struct {
        unsigned int :4;
        unsigned int bit4 : 1;
        unsigned int :27;
    } bits;
};

struct Flags241_00466dc0 {
    unsigned int :29;
    unsigned int bit29 : 1;
    unsigned int :2;
};

union Flags111_00466dc0 {
    unsigned int all;
    struct {
        unsigned int :30;
        unsigned int bit30 : 1;
        unsigned int :1;
    } bits;
};

union Flags14281_00466dc0 {
    unsigned short all;
    struct {
        unsigned short bit0 : 1;
        unsigned short bit1 : 1;
        unsigned short :7;
        unsigned short bit9 : 1;
        unsigned short :5;
    } bits;
};

struct Player_00466dc0 {
    char unknown_0[0x96];
    unsigned char field_96;              // +0x96
};

struct MapSize_00466dc0 {
    unsigned int width;                  // +0x80
    unsigned int height;                 // +0x84

    int Contains(unsigned int tx, unsigned int ty)
    {
        return tx < width && ty < height;
    }
};

struct ByteMap_00466dc0 {
    unsigned char* data;                 // +0x0
    MapSize_00466dc0 size;               // +0x4

    int Index(int x, int y) { return size.width * y + x; }
    unsigned char Get(int x, int y) { return data[Index(x, y)]; }
};

struct PlayerInfo_00466dc0 {
    char unknown_0[0x27];
    Player_00466dc0* data;               // +0x27
    char unknown_2b[0x7c - 0x2b];
    ByteMap_00466dc0 explored;           // +0x7c
    char unknown_88[0x14b - 0x88];
};

struct Slot_00466dc0 {
    Shot_00466dc0* shot;                 // +0x0
    char unknown_4[0xe - 4];
    unsigned char field_e;               // +0xe
    char unknown_f[0x1c - 0xf];
};

struct Shot_00466dc0 {
    char unknown_0[0xe0];
    int field_e0;                        // +0xe0
    char unknown_e4[0x111 - 0xe4];
    Flags111_00466dc0 flags;             // +0x111
};

struct UnitType_00466dc0 {
    char unknown_0[0x204];
    short radardistance;                 // +0x204
    short sonardistance;                 // +0x206
    char unknown_208[0x20a - 0x208];
    short radardistancejam;              // +0x20a
    short sonardistancejam;              // +0x20c
    char unknown_20e[0x241 - 0x20e];
    Flags241_00466dc0 flags_241;         // +0x241
    unsigned char flags2;                // +0x245
};

struct Unit {
    char unknown_0[0x10];
    Slot_00466dc0 slots[3];              // +0x10
    char unknown_64[0x8];
    short field_6c;                      // +0x6c
    char unknown_6e[2];
    short field_70;                      // +0x70
    char unknown_72[2];
    short field_74;                      // +0x74
    char unknown_76[0x1c];
    UnitType_00466dc0* type;             // +0x92
    char unknown_96[0x10];
    short unitDefIndex;                      // +0xa6
    short id;                      // +0xa8
    char unknown_aa[0x50];
    unsigned char recentlyDamagedTimer;              // +0xfa
    char unknown_fb[0x4];
    unsigned char playerIndex;              // +0xff
    char unknown_100[0xe];
    unsigned char activateFlags;             // +0x10e
    char unknown_10f[0x1];
    Flags110_00466dc0 flags_110;         // +0x110
    char unknown_114[0x4];
};

struct Projectile_00466dc0 {
    Shot_00466dc0* shot;                 // +0x0
    int posx;                            // +0x4
    int posy;                            // +0x8
    int posz;                            // +0xc
    char unknown_10[0x52 - 0x10];
    Unit* owner;                         // +0x52
    char unknown_56[0x66 - 0x56];
    unsigned char player;                // +0x66
    char unknown_67[0x6b - 0x67];
};

struct Tail_00466dc0 {
    char unknown_0[0x48];
    Unit* owner;                         // q+0x48
    char unknown_4c[0x5c - 0x4c];
    unsigned char player;                // q+0x5c
};

struct Blip_00466dc0 {
    short id;                            // +0x0
    int x;                               // +0x2
    int y;                               // +0x6
};

// The four shorts at +0x142e7 are named posX/posY/width/height, originX/
// originY/sizeX/sizeY and the four-element `dim` the rectangle test indexes,
// depending on the function; the union in Game keeps every spelling addressable.

// The radar redraw flag word at +0x142f1. Most functions test and set its bits
// as a 16-bit unit placed after blinkTimer (blinkOn bit 0, pending bit 1,
// mapChanged bit 2); the unit draw reads bit 0 as a byte and bit 1 through the
// word at +0x142f0, so both views must stay.
union RadarTimer_00466dc0 {
    struct {
        short blinkTimer;                // +0x142ef
        struct {
            unsigned short blinkOn : 1;  // +0x142f1 bit 0
            unsigned short pending : 1;  // bit 1
            unsigned short mapChanged : 1; // bit 2
            unsigned short :13;
        } flags;
    } word;
    struct {
        unsigned char unknown_142ef;     // +0x142ef
        union {
            unsigned short all;
            struct {
                unsigned char lo;        // +0x142f0
                unsigned char hi;        // +0x142f1
            } b;
            struct {
                unsigned short :8;
                unsigned short bit0 : 1; // +0x142f1 bit 0
                unsigned short bit1 : 1;
                unsigned short bit2 : 1;
                unsigned short :5;
            } bits;
        } field_142f0;
    } byte;
};

struct Game {
    char unknown_0[0xc];
    Fx_00466c20* fx;                     // +0xc
    char unknown_10[0xdcb - 0x10];
    unsigned char fogColor;              // +0xdcb
    char unknown_dcc[0xdd9 - 0xdcc];
    unsigned char field_dd9;             // +0xdd9
    char unknown_dda[0x1b63 - 0xdda];
    PlayerInfo_00466dc0 players[1];      // +0x1b63, 0x14b bytes each
    char unknown_1cae[0x2a43 - 0x1cae];
    unsigned char currentPlayer;         // +0x2a43
    char unknown_2a44[0x2cba - 0x2a44];
    short hoverUnitId;                   // +0x2cba
    char unknown_2cbc[0x141f3 - 0x2cbc];
    int projectileCount;                 // +0x141f3
    Projectile_00466dc0* projectiles;    // +0x141f7
    char unknown_141fb[0x1422b - 0x141fb];
    int mapWidth;                        // +0x1422b
    int mapHeight;                       // +0x1422f
    int rowWidth;                        // +0x14233
    int mapHeight2;                      // +0x14237
    int zoomX;                           // +0x1423b
    int zoomY;                           // +0x1423f
    char unknown_14243[0x1426b - 0x14243];
    Frame_004b8ae0* radarFrame;          // +0x1426b
    char unknown_1426f[0x14273 - 0x1426f];
    unsigned short* visibilityMask;      // +0x14273
    char unknown_14277[0x14281 - 0x14277];
    Flags14281_00466dc0 field_14281;     // +0x14281
    IconSet_00466780* iconSet;           // +0x14283
    char unknown_14287[0x1428b - 0x14287];
    unsigned short* mapValues;           // +0x1428b
    char unknown_1428f[0x142bb - 0x1428f];
    int viewLeft;                        // +0x142bb
    int viewTop;                         // +0x142bf
    int viewRight;                       // +0x142c3
    int viewBottom;                      // +0x142c7
    char field_142cb[0x142db - 0x142cb]; // +0x142cb
    void* finalSurface;                  // +0x142db
    void* mappedSurface;                 // +0x142df
    void* pictureSurface;                // +0x142e3
    union {                              // +0x142e7
        struct {
            short posX;                      // +0x142e7
            short posY;                      // +0x142e9
            short width;                     // +0x142eb
            short height;                    // +0x142ed
        };
        struct {
            short originX;                   // +0x142e7
            short originY;                   // +0x142e9
            short sizeX;                     // +0x142eb
            short sizeY;                     // +0x142ed
        };
        struct { short v[4]; } dim;
    };
    RadarTimer_00466dc0 timer;           // +0x142ef
    char unknown_142f3[0x1431f - 0x142f3];
    int scaleX;                          // +0x1431f
    int scaleY;                          // +0x14323
    char unknown_14327[0x14357 - 0x14327];
    Unit* units;                         // +0x14357
    Unit* unitsEnd;                      // +0x1435b
    char unknown_1435f[0x14363 - 0x1435f];
    unsigned short* hotRadar;            // +0x14363
    char unknown_14367[0x1436b - 0x14367];
    int hotRadarCount;                   // +0x1436b
    char unknown_1436f[0x147df - 0x1436f];
    void* field_147df;                   // +0x147df
    void* field_147e3;                   // +0x147e3
    void* field_147e7;                   // +0x147e7
    char unknown_147eb[0x37f2f - 0x147eb];
    Flags14281_00466dc0 field_37f2f;     // +0x37f2f
};
#pragma pack(pop)

extern Game* g_game;
extern char g_radarPictureName[];
extern char g_radarPicTempName[];
extern char g_radarFinalName[];
extern char g_radarMappedName[];

void* __stdcall AllocFrame(char* name, int width, int height);
void __stdcall SurfaceFromFrame(Surface_4665d0* surface, void* pic);
void __stdcall DrawFrame(void* surface, void* frame, int x, int y);
void __stdcall FillSurface(Surface_4665d0* surface, int mode);
void __stdcall DrawFrameQuad(Surface_4665d0* surface, void* pic, Quad_4665d0* dst, Quad_4665d0* src);
void __cdecl FUN_004d85a0(void* pic);

void __stdcall FrameFromSurface(Frame_004b8ae0* dst, void* src);
void __stdcall DownsampleFrame(Frame_004b8ae0* dst, Frame_004b8ae0* src);
void __stdcall DrawPixel(void* picture, int x, int y, int pixel);
void* __stdcall AllocSurface(char* name, int width, int height);
void __stdcall FreeSurface(void* picture);

void __stdcall DrawRectangle(void* param_1, void* param_2, int param_3);
void __stdcall DrawSurface(void* dst, void* bmp, int x, int y);

void* __stdcall GetGafFrame(void* a, int index);
void __stdcall DrawCircle(void* surface, int x, int y, int radius, int color);
void __stdcall DrawDashedCircle(void* surface, int x, int y, int radius, int color,
                            int a6, int a7);

// Rescales a radar picture. The picture is first copied into a temp bitmap of
// its own size, then p is given the new size (x, y) and used as the surface to
// draw on, and the copy is blitted into it, scaled to fit (w - 0x20) by
// (h - 0x80) and centred: when the box is wider than tall the height is the
// scaled one and it is centred, otherwise the width is.

// FUNCTION: 0x4665d0
void __stdcall ResizeRadarPicture(Pic_4665d0* pic, int x, int y, int w, int h)
{
    if (pic == 0) {
        return;
    }
    int dwx;
    int dhy;
    int sw;
    int sh;
    Quad_4665d0 src;
    Quad_4665d0 dst;
    Surface_4665d0 surface;
    int dw = w - 0x20;
    int dh = h - 0x80;
    int ox;
    int oy;
    if (dw >= dh) {
        dwx = x;
        dhy = dh * y / dw;
        sw = pic->w;
        sh = dh * pic->h / dw;
        ox = 0;
        oy = (y - dhy) / 2;
    } else {
        dwx = dw * x / dh;
        dhy = y;
        sw = dw * pic->w / dh;
        sh = pic->h;
        ox = (x - dwx) / 2;
        oy = 0;
    }
    void* temp = AllocFrame("TEMP RADAR PIC", pic->w, pic->h);
    SurfaceFromFrame(&surface, temp);
    DrawFrame(&surface, (short*)pic, 0, 0);
    pic->w = x;
    pic->h = y;
    SurfaceFromFrame(&surface, pic);
    FillSurface(&surface, 0);

    src.p[0].x = 0;
    src.p[0].y = 0;
    src.p[1].x = sw - 1;
    src.p[1].y = 0;
    src.p[2].x = sw - 1;
    src.p[2].y = sh - 1;
    src.p[3].x = 0;
    src.p[3].y = sh - 1;

    dst.p[0].x = ox;
    dst.p[0].y = oy;
    dst.p[1].x = ox + dwx;
    dst.p[1].y = oy;
    dst.p[2].x = ox + dwx;
    dst.p[2].y = oy + dhy;
    dst.p[3].x = ox;
    dst.p[3].y = oy + dhy;

    DrawFrameQuad(&surface, temp, &dst, &src);
    FUN_004d85a0(temp);
}

// Rebuilds the radar picture: fits the map onto 126 pixels along its long side,
// then blits it onto the stored radar frame, or, when there is none, scales the
// 8x8 icon map up to twice that size through a temporary picture.
// FUNCTION: 0x466780
void __stdcall BuildRadarPicture()
{
    int mapWidth = g_game->mapWidth;
    int mapHeight = g_game->mapHeight;
    int width;
    int height;
    if (mapWidth >= mapHeight) {
        width = 126;
        height = mapHeight * 126 / mapWidth;
        g_game->posX = 0;
        g_game->posY = (126 - height) / 2;
    } else {
        width = mapWidth * 126 / mapHeight;
        height = 126;
        g_game->posX = (126 - width) / 2;
        g_game->posY = 0;
    }
    g_game->width = width;
    g_game->height = height;
    g_game->pictureSurface = AllocSurface(g_radarPictureName, width, height);
    Frame_004b8ae0 frame;
    FrameFromSurface(&frame, g_game->pictureSurface);
    if (g_game->radarFrame) {
        DownsampleFrame(g_game->radarFrame, &frame);
        return;
    }
    int h2 = height * 2;
    int w2 = width * 2;
    void* temp = AllocSurface(g_radarPicTempName, w2, h2);
    for (int j = 0; j < h2; j++) {
        for (int i = 0; i < w2; i++) {
            int x = g_game->mapWidth * i / w2;
            int y = g_game->mapHeight * j / h2;
            int index = (y / 32) * (g_game->rowWidth / 2) + x / 32;
            unsigned short value = g_game->mapValues[index];
            if (value >= g_game->iconSet->count) {
                value = 0;
            }
            int pixel = g_game->iconSet->cell[value][y % 32][x % 32];
            DrawPixel(temp, i, j, pixel);
        }
    }
    Frame_004b8ae0 tempFrame;
    FrameFromSurface(&tempFrame, temp);
    DownsampleFrame(&tempFrame, &frame);
    FreeSurface(temp);
}

// The two rectangle edges are `size + pos - 1`.
// FUNCTION: 0x4669b0
void InitRadar()
{
    BuildRadarPicture();
    g_game->finalSurface = AllocSurface(g_radarFinalName, g_game->dim.v[2], g_game->dim.v[3]);
    g_game->mappedSurface = AllocSurface(g_radarMappedName, g_game->dim.v[2], g_game->dim.v[3]);
    g_game->viewLeft = g_game->dim.v[0];
    g_game->viewTop = g_game->dim.v[1];
    // viewRight indexes the four radar shorts, viewBottom goes through a char*:
    // the shape of each address decides the load order.
    g_game->viewRight = g_game->dim.v[2] + g_game->dim.v[0] - 1;
    {
        char* c = (char*)g_game;
        g_game->viewBottom = *(short*)(c + 0x142ed) + *(short*)(c + 0x142e9) - 1;
    }
    g_game->timer.word.flags.mapChanged = 1;
    g_game->timer.word.blinkTimer = 7;
    g_game->timer.word.flags.blinkOn = 0;
}

// FUNCTION: 0x466aa0
void FreeRadar()
{
    FreeSurface(*(void**)((char*)g_game + 0x142e3));
    FreeSurface(*(void**)((char*)g_game + 0x142df));
    FreeSurface(*(void**)((char*)g_game + 0x142db));
    *(void**)((char*)g_game + 0x142e3) = 0;
    *(void**)((char*)g_game + 0x142df) = 0;
    *(void**)((char*)g_game + 0x142db) = 0;
}

// FUNCTION: 0x466b00
void __stdcall DrawRadar(void* param_1)
{
    if (g_game->timer.word.flags.pending) {
        g_game->timer.word.flags.pending = 0;
        DrawSurface(param_1, g_game->finalSurface, g_game->posX, g_game->posY);
        DrawRectangle(param_1, g_game->field_142cb, g_game->field_dd9);
    }
}

// FUNCTION: 0x466b70
void __stdcall CalcRadarViewportRect(int* param_1)
{
    param_1[0] = g_game->sizeX * g_game->scaleX / g_game->mapWidth + g_game->originX;
    param_1[1] = g_game->sizeY * g_game->scaleY / g_game->mapHeight + g_game->originY;
    param_1[2] = g_game->sizeX * g_game->zoomX * 16 / g_game->mapWidth + param_1[0] - 1;
    param_1[3] = g_game->sizeY * g_game->zoomY * 16 / g_game->mapHeight + param_1[1] - 1;
}

// FUNCTION: 0x466c20
void UpdateRadarMapped()
{
    if (g_game->timer.word.flags.mapChanged) {
        g_game->timer.word.flags.mapChanged = 0;
        unsigned char fog = g_game->fogColor;
        PlayerInfo_00466dc0* t = &g_game->players[g_game->currentPlayer];
        unsigned int mask = 1 << g_game->currentPlayer;
        unsigned char* dst = *(unsigned char**)((char*)g_game->mappedSurface + 0xc);
        unsigned char* src = *(unsigned char**)((char*)g_game->pictureSurface + 0xc);
        int halfWidth = g_game->rowWidth / 2;
        int halfHeight = g_game->mapHeight2 / 2;
        for (int i = 0; i < g_game->height; i++) {
            int mapY = i * halfHeight;
            int mapX = 0;
            int j = 0;
            // A while loop, tail in this order: a for loop computes dst+1 before the store.
            while (j < g_game->width) {
                int index = (mapY / g_game->height) * halfWidth + mapX / g_game->width;
                // Outputs assigned directly in each branch, no pixel temporary;
                // the unsigned short cast stays.
                if (!(unsigned short)(g_game->visibilityMask[index] & mask)) {
                    *dst = fog;
                } else if (t->explored.data[index]) {
                    *dst = *src;
                } else {
                    *dst = g_game->fx->colorMap[*src];
                }
                j++;
                mapX += halfWidth;
                src++;
                dst++;
            }
        }
        g_game->timer.word.flags.pending = 1;
    }
}

// The unit loop's x multiply (`movsx eax, [ebx+0x6c]; movsx ecx,
// [esi+0x142eb]` in the original) is ordered by a key built from the symbol
// ids of the two bases plus the displacement. <windows.h> supplies the
// declaration count that gives the original's order; reading the type through
// a reference taken at the top of the loop body leaves the x multiply on `u`.
// The radar helpers use the ByteMap Index/Get methods (width materialised,
// `mov ecx, [edx+0x80]; imul ecx, edi`) and the slot loop reads `slot->shot`
// at each use instead of a `shot` local.
static PlayerInfo_00466dc0* PlayerInfo_00466dc0_Get(unsigned char p)
{
    return (PlayerInfo_00466dc0*)((char*)g_game + 0x1b63) + p;
}

// True when (px, py) is inside the current player's visible area. The two
// halves match the uint8 terrain bitmap and the packed 16-bit bitfield variant.
static inline int OnRadarByte_00466dc0(PlayerInfo_00466dc0* pi, int px, int py)
{
    int tx = px >> 5;
    int ty = py >> 5;
    if (pi->explored.size.Contains(tx, ty) && pi->explored.Get(tx, ty))
        return 1;
    return 0;
}

static inline int OnRadarShort_00466dc0(PlayerInfo_00466dc0* pi, int px, int py)
{
    int tx = px >> 5;
    int ty = py >> 5;
    if (!pi->explored.size.Contains(tx, ty)) {
        return 0;
    }
    ByteMap_00466dc0* b = &pi->explored;
    return (g_game->visibilityMask[b->Index(tx, ty)] &
            (1 << g_game->currentPlayer)) != 0;
}

static inline int OnRadar_00466dc0(int px, int py)
{
    PlayerInfo_00466dc0* pi = PlayerInfo_00466dc0_Get(g_game->currentPlayer);
    if ((g_game->field_14281.all & 2) == 2)
        return OnRadarByte_00466dc0(pi, px, py);
    return OnRadarShort_00466dc0(pi, px, py);
}

// A unit's screen y (height folded into z) times the minimap scale.
static inline int ScaleY_00466dc0(Unit* u)
{
    return ((int)u->field_74 - ((int)u->field_70 >> 1)) * (int)g_game->height;
}

// FUNCTION: 0x466dc0
void DrawRadarUnits(void)
{
    unsigned char* base = (unsigned char*)g_game + 0xdcb;
    unsigned short* out = g_game->hotRadar;

    g_game->hotRadarCount = 0;
    void* surface = g_game->finalSurface;
    DrawSurface(surface, g_game->mappedSurface, 0, 0);

    int enabled;
    if (g_game->field_14281.bits.bit0 || g_game->field_14281.bits.bit1)
        enabled = 0;
    else
        enabled = 1;
    if (g_game->field_37f2f.bits.bit9)
        enabled = 1;

    Unit* u = g_game->units;
    Unit* end = g_game->unitsEnd;
    if (u <= end) {
        do {
            UnitType_00466dc0*& type = u->type;
            if (u->unitDefIndex != 0) {
                if (enabled != 0 || (u->flags_110.all & 0x300) != 0 ||
                    u->playerIndex == g_game->currentPlayer) {
                    int x = u->field_6c * g_game->width /
                            g_game->mapWidth;
                    int y = ScaleY_00466dc0(u) / g_game->mapHeight;
                    if (u->recentlyDamagedTimer == 0 ||
                        (g_game->timer.byte.field_142f0.b.hi & 1) != 0) {
                        DrawFrame(surface,
                            GetGafFrame(g_game->field_147df,
                                PlayerInfo_00466dc0_Get(u->playerIndex)->data->field_96),
                            x, y);
                    }
                    if (u->id == g_game->hoverUnitId) {
                        DrawFrame(surface,
                            GetGafFrame(g_game->field_147e3, 0), x, y);
                    }
                    if (u->flags_110.bits.bit4) {
                        if ((u->activateFlags & 1) != 0 ||
                            (type->flags2 & 4) == 0) {
                            if (type->radardistance != 0)
                                DrawCircle(surface, x, y,
                                    (int)g_game->width * type->radardistance /
                                    g_game->mapWidth, base[0xa]);
                            if (type->sonardistance != 0)
                                DrawCircle(surface, x, y,
                                    (int)g_game->width * type->sonardistance /
                                    g_game->mapWidth, base[0xa]);
                            if (type->radardistancejam != 0)
                                DrawCircle(surface, x, y,
                                    (int)g_game->width * type->radardistancejam /
                                    g_game->mapWidth, base[0xc]);
                            if (type->sonardistancejam != 0)
                                DrawCircle(surface, x, y,
                                    (int)g_game->width * type->sonardistancejam /
                                    g_game->mapWidth, base[0xc]);
                        }
                        if (type->flags_241.bit29) {
                            Slot_00466dc0* slot = u->slots;
                            int n = 3;
                            do {
                                if (slot->shot->flags.bits.bit30) {
                                    int r = ((int)g_game->width *
                                             (slot->shot->field_e0 - 0x200)) /
                                            g_game->mapWidth;
                                    if (slot->field_e != 0)
                                        DrawDashedCircle(surface, x, y, r, base[0xf],
                                                     0x20,
                                                     g_game->timer.byte.field_142f0.b.hi & 1);
                                    else
                                        DrawCircle(surface, x, y, r, base[0xf]);
                                }
                                slot++;
                                n--;
                            } while (n != 0);
                        }
                    }
                    out[0] = u->id;
                    *(int*)(out + 1) = g_game->posX + x;
                    *(int*)(out + 3) = g_game->posY + y;
                    out += 5;
                    g_game->hotRadarCount++;
                }
            }
            u = (Unit*)((char*)u + 0x118);
        } while (u <= g_game->unitsEnd);
    }

    Projectile_00466dc0* p = g_game->projectiles;
    int i = 0;
    if (g_game->projectileCount > 0) {
        short* q = (short*)((char*)p + 0xa);
        do {
            int px = q[-2];
            int x = (int)g_game->width * px / g_game->mapWidth;
            int py = q[2] - ((int)q[0] >> 1);
            int y = (int)g_game->height * py / g_game->mapHeight;
            if ((p->shot->flags.all & 0x60000000) == 0) {
                if ((p->shot->flags.all & 0x40) == 0) {
                    if (OnRadar_00466dc0(px, py) ||
                        ((Tail_00466dc0*)q)->player ==
                            g_game->currentPlayer) {
                        DrawPixel(surface, x, y, base[0xe]);
                    }
                }
            } else {
                if (OnRadar_00466dc0(px, py) ||
                    ((Tail_00466dc0*)((char*)q))->owner->playerIndex ==
                        g_game->currentPlayer) {
                    DrawFrame(surface,
                        GetGafFrame(g_game->field_147e7,
                            PlayerInfo_00466dc0_Get(
                                ((Tail_00466dc0*)q)->player)->data->field_96),
                        x, y);
                }
            }
            i++;
            p = (Projectile_00466dc0*)((char*)p + 0x6b);
            q = (short*)((char*)q + 0x6b);
        } while (i < g_game->projectileCount);
    }

    g_game->timer.byte.field_142f0.bits.bit1 = 1;
}
