// Decompiled by Opus, Sonnet, Haiku, Space Bunny Free, deepseek-v4.1-flash,
// space-bunny-free, GPT-6, deepseek-v4.1, Sonnet 5.5, GPT-6.1-sol,
// mimo-v2.6-pro, claude-sonnet-5-5, Claude Sonnet 5.5, DeepSeek V4.1 Flash and
// opus. Names are provisional.
// The line-of-sight and map module (0x4814c0 to 0x484b50): the eyeball records,
// the map cell lookups by grid and world position, line-of-sight add, remove
// and update, the eye expiry pass, the map grids and fog tiles, the map loader
// and its shutdown, and the cell height range and terrain lookups. The module's
// parts joined in address order; 0x4816a0, 0x481930, 0x481d50, 0x482270,
// 0x482910, 0x482ac0 and 0x4843c0 keep their own files: their register plans
// follow their old files' symbol ids.
#include <memory>
#include <string.h>
#include <math.h>
#include <stddef.h>
#include <algorithm>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#pragma pack(push, 1)

struct Vec3 {
    int x;
    union {
        int y;
        struct {
            short y_lo;
            short y_hi;
        };
    };
    union {
        int z;
        struct {
            short z_lo;
            short z_hi;
        };
    };
};

struct Pos {
    short x;
    short y;
};

struct Point {
    short x;
    short y;
};

struct Rect {
    int left;                          // +0x0
    int top;                           // +0x4
    int right;                         // +0x8
    int bottom;                        // +0xc
};

// A map cell (13 bytes): the feature at +0x8, its footprint offsets at +0xa,
// and the ground heights at +0x4 to +0x6.
struct Cell {
    unsigned short spot;               // +0x0
    char unknown_2[2];
    unsigned char height;              // +0x4
    unsigned char high;                // +0x5
    unsigned char low;                 // +0x6
    unsigned char field_7;             // +0x7
    unsigned short feature;            // +0x8
    union {
        struct {
            unsigned char offsetY;     // +0xa
            unsigned char offsetX;     // +0xb
        };
        unsigned short spotField;      // +0xa
    } sf;
    unsigned char flags;               // +0xc
};

// One cell of the two 2-byte fog grids: its low and high sight levels.
struct FogCell {
    union {
        struct {
            unsigned char lo;          // +0x0
            unsigned char hi;          // +0x1
        };
        struct {
            unsigned char level0;      // +0x0
            unsigned char level1;      // +0x1
        };
    };
};

struct Grid {
    FogCell* cells;                    // +0x0
    unsigned int width;                // +0x4
    unsigned int height;               // +0x8
    int count;                         // +0xc
};

struct Grid2 {
    FogCell* cells;                    // +0x0
    unsigned int width;                // +0x4
    unsigned int height;               // +0x8
    int count;                         // +0xc
    int field_10;                      // +0x10
    int field_14;                      // +0x14
};

// A 10-byte cell of the second fog grid.
struct Rec {
    Rec() { a = 0; b = 0; flags = 0; field_6 = 0; }
    unsigned char a;                   // +0x0
    unsigned char b;                   // +0x1
    int flags;                         // +0x2
    int field_6;                       // +0x6
};

// The explored-terrain grid of one player (at +0x7c).
struct PlayerGrid {
    unsigned char* cells;              // +0x0
    unsigned int width;                // +0x4
    unsigned int height;               // +0x8
    int field_c;                       // +0xc
    unsigned char& at(int x, int y) { return cells[y * width + x]; }
};

struct Player {
    int active;                        // +0x00
    char unknown_4[0x73 - 0x4];
    unsigned char type;                // +0x73
    char unknown_74[0x7c - 0x74];
    PlayerGrid grid;                   // +0x7c
    char unknown_8c[0x146 - 0x8c];
    unsigned char field_146;           // +0x146
    char unknown_147[0x14b - 0x147];   // stride 0x14b
};

struct UnitDef {
    char unknown_0[0x170];
    unsigned char field_170;           // +0x170
    char unknown_171[0x202 - 0x171];
    short range;                       // +0x202
};

struct Unit {
    char unknown_0[0x6a];
    Vec3 pos;                          // +0x6a
    char unknown_76[4];                // +0x76
    short cell[2];                     // +0x7a
    char unknown_7e[0x92 - 0x7e];
    UnitDef* type;                     // +0x92
    Player* owner;                     // +0x96
    char unknown_9a[0xa6 - 0x9a];
    unsigned short id;                 // +0xa6
    char unknown_a8[0xf8 - 0xa8];
    unsigned char losSightFrameIdx[4];         // +0xf8
    char unknown_fc[0x118 - 0xfc];     // stride 0x118
};

// A GAF frame (32x32 tile bitmap); GetGafFrame returns one of these.
struct Bitmap {
    unsigned short width;              // +0x0
    unsigned short height;             // +0x2
    short field_4;                     // +0x4
    short field_6;                     // +0x6
    unsigned char mask;                // +0x8
    unsigned char flag9;               // +0x9
    unsigned char count;               // +0xa
    unsigned char kind;                // +0xb
    int unknown_c;                     // +0xc
    unsigned char* data;               // +0x10
    int unknown_14;                    // +0x14
};

// The table GetGafFrame indexes: a count and 8-byte entries at +0x28.
struct FrameTable {
    unsigned short count;              // +0x0
    char unknown_2[0x28 - 2];
    void* entries;                     // +0x28
};

// One of the "eyeball" records of the array at g_game + 0x1427b. The two
// pointer fields point into the record itself: +4 at its screen position, +0xc
// at the byte flag, so the copy has to re-point them at the destination.
struct Eye {
    void* player;                      // +0x00
    Pos* screen;                       // +0x04, &screenPos
    short x;                           // +0x08
    unsigned char flagA;               // +0x0a
    char flagB;                        // +0x0b
    char* flagPtr;                     // +0x0c, &flagB
    Vec3 pos;                          // +0x10
    unsigned int expires;              // +0x1c
    Pos screenPos;                     // +0x20
    Eye& operator=(const Eye& s) {
        player = s.player;
        screen = &screenPos;
        x = s.x;
        flagPtr = &flagB;
        pos = s.pos;
        flagA = s.flagA;
        expires = s.expires;
        screenPos = s.screenPos;
        flagB = s.flagB;
        return *this;
    }
};

// The parameter block the line-of-sight functions share.
struct Params {
    void* field_0;                     // +0x00
    short* field_4;                    // +0x04
    short field_8;                     // +0x08
    unsigned char field_a;             // +0x0a
    char unknown_b;                    // +0x0b
    unsigned char* field_c;            // +0x0c
    Vec3 pos;                          // +0x10
    int unknown_1c;                    // +0x1c
    int unknown_20;                    // +0x20
};

union Flags_14281 {
    unsigned short raw;                // +0x0
    unsigned char rawByte;
    struct {
        unsigned short bit0 : 1;
        unsigned short bit1 : 1;
        unsigned short flag2 : 1;
        unsigned short flag3 : 1;
        unsigned short rest : 12;
    };
};

union Flags_142f1 {
    unsigned short raw;                // +0x0
    unsigned char rawByte;
    struct {
        unsigned short bit0 : 1;
        unsigned short bit1 : 1;
        unsigned short mapChanged : 1;
        unsigned short rest : 13;
    };
};

struct IconSet {
    char unknown_0[4];
    unsigned char* data;               // +0x4, 32x32 icons, 0x400 bytes each
};

// The map state at g_game + 0x141fb.
struct TntInfo {
    int version;
    int width;
    int height;
    int flag;
    int sea_a;
    int sea_b;
    int sea_d;
    int tile_count;
    int map_size;
    int* tile_set_src;
    unsigned int tile_set_count;
    int* tile_map_src;
    unsigned char* attr_b;
    unsigned char* attr_a;
    int attr_limit;
    unsigned int feature_flags;
    unsigned short* feature_data;
};

struct TntHeader {
    unsigned short width;
    unsigned short height;
    unsigned short pad0;
    unsigned short pad1;
    unsigned char flag[4];
    int zero0;
    unsigned short* data;
    int zero1;
};

union Slot {
    int n;
    Point p;
};

// The object at g_game + 0x391e9 (a mission, 0xd44 bytes).
struct Net {
    char unknown_0[0xd44];
    int field_d44;                     // +0xd44
};

struct Flags_004848e0 {
    unsigned short damagebars : 1;
    unsigned short antiAlias : 1;
    unsigned short shadows : 1;
    unsigned short vehicleShadows : 1;
    unsigned short featureShadows : 1;
    unsigned short shading : 1;
    unsigned short ditheredFog : 1;
    unsigned short unused7 : 1;
    unsigned short switchAlt : 1;
};

struct Game {
    char unknown_0[0xdcb];
    unsigned char colors[16];          // +0xdcb
    char unknown_ddb[0x1b63 - 0xddb];
    Player players[10];                // +0x1b63
    char unknown_2851[0x2a43 - 0x2851];
    unsigned char playerIndex;         // +0x2a43
    char unknown_2a44[0x141fb - 0x2a44];
    int* sortUnits;                    // +0x141fb
    int* sortIndices;                  // +0x141ff
    int* sortLineCount;                // +0x14203
    char unknown_14207[0x1421f - 0x14207];
    Grid* grid;                        // +0x1421f
    int baseX;                         // +0x14223, map size in x
    int baseY;                         // +0x14227, map size in y
    int mapWidth;                      // +0x1422b
    int mapHeight;                     // +0x1422f
    int width;                         // +0x14233
    int height;                        // +0x14237
    char unknown_1423b[0x1426b - 0x1423b];
    void* radarFrame;                  // +0x1426b
    char unknown_1426f[0x14273 - 0x1426f];
    unsigned short* visibilityMask;    // +0x14273
    int count;                         // +0x14277
    Eye* eyes;                         // +0x1427b
    unsigned char seaLevel;            // +0x1427f
    unsigned char unknown_14280;
    Flags_14281 flags;                 // +0x14281
    IconSet* iconSet;                  // +0x14283
    Cell* cells;                       // +0x14287
    unsigned short* mapValues;         // +0x1428b
    Grid grid1;                        // +0x1428f
    Grid2 grid2;                       // +0x1429f
    Rec* field_142b7;                  // +0x142b7
    char unknown_142bb[0x142f1 - 0x142bb];
    Flags_142f1 field_142f1;           // +0x142f1
    char unknown_142f3[0x1431f - 0x142f3];
    int scrollX;                       // +0x1431f
    int scrollY;                       // +0x14323
    char unknown_14327[0x14357 - 0x14327];
    Unit* units;                       // +0x14357
    Unit* units_end;                   // +0x1435b
    char unknown_1435f[0x1485b - 0x1435f];
    FrameTable* losTable;              // +0x1485b
    void* black[4];                    // +0x1485f
    void* gray[4];                     // +0x1486f
    char unknown_1487f[0x37e27 - 0x1487f];
    Rect rect;                         // +0x37e27
    int viewW;                         // +0x37e37
    int viewH;                         // +0x37e3b
    char unknown_37e3f[0x37f06 - 0x37e3f];
    Flags_004848e0 flags_37f06;        // +0x37f06
    char unknown_37f08[0x38a47 - 0x37f08];
    unsigned int ticks;                // +0x38a47
    char unknown_38a4b[0x391e9 - 0x38a4b];
    Net* net;                          // +0x391e9
};

#pragma pack(pop)

extern Game* g_game;

// The line-of-sight tables (g_losTables at 0x51e6a0).
class LosTables {
public:
    explicit LosTables(const std::allocator<int>& al = std::allocator<int>())
        : allocator(al), first(0), last(0), end(0) {}
    ~LosTables() { FreeTables(); }

    void FreeTables();
    void* GetLosTable(int n);
    short GetLosTableCount();
    void LoadLosTables();

    std::allocator<int> allocator;     // +0x0
    int* first;                        // +0x4
    int* last;                         // +0x8
    int* end;                          // +0xc
};

class LosTable {
public:
    short GetLosLineCount();
};

class Class_4335e0 {
public:
    void* GetLosLine(short i);
};

class LosLine {
public:
    short GetLosLineStepCount();
    void GetLosLineStep(short i, int* a, int* b);
};

// Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
int RIReport(int, int, int, int, int, int, int, int, int, int);
int DrawWrappedText(char*, char*, int, int, int, int, int);
void ParseDownloadableAiWeightScripts(int);

class Mission {
public:
    int* GetNameSlot(int index);
};

void* __cdecl FUN_004d83b0(const char* name, unsigned int size);
void __cdecl FUN_004d85a0(void* p);
void __stdcall FatalError(char* message);
int* __stdcall LoadFileWithProgress(int* file);
Bitmap* __stdcall GetGafFrame(void* table, int index);
void __stdcall UpdateLineOfSight(Params* params);
void __stdcall AddLineOfSight(Params* params);
void __stdcall RemoveLineOfSight(Params* params);
void __stdcall RevealAroundUnit(Params* params);
void __stdcall UpdateCellHeightRange(Point pos, Point size);
void UpdateRadarMapped();
void DrawRadarUnits();
void FreeFeaturePool();
void BuildFogTiles();
void* __stdcall AllocFrame(const char* name, int width, int height);
void __stdcall SurfaceFromFrame(void* dst, void* src);
void __stdcall DrawFrame(void* surface, void* header, int x, int y);
void __stdcall DrawFrameOpaque(void* dst, Bitmap* bmp, int x, int y);
void __stdcall DrawTile(void* dst, int x, int y, unsigned char* pix);
void __stdcall DrawFrameGray(void* surface, void* bmp, int x, int y);
void __stdcall EraseFrameDithered(void* surface, void* bmp, int x, int y, int color);
void __stdcall FillRectangle(void* surface, Rect* rect, int color);
void __stdcall GrayRectangle(void* surface, Rect* rect);
void __stdcall DitherRectangle(void* surface, Rect* rect, int color);
void __stdcall InitFeatureAnimPool(int* info);
void* __stdcall PlaceFeature(void* target, unsigned short id, void* pos, void* field_64, unsigned char owner);
void __stdcall PlaceMissionFeatures();
void StampFeatureMetal();
int __stdcall GetGroundHeight(Vec3* pos);
void* __cdecl operator new(unsigned int size);
void __cdecl operator delete(void* p);

// Returns int, not bool: a bool return breaks the fused compare.
struct Expired {
    int operator()(const Eye& e) const { return e.expires < g_game->ticks; }
};

static inline int SumX(Point a, Point b) { return a.x + b.x; }
static inline int SumY(Point a, Point b) { return a.y + b.y; }

static inline Point MakePoint(int x, int y)
{
    Point p;
    p.x = x;
    p.y = y;
    return p;
}

static inline Cell* GetCell(int x, int y)
{
    if (x >= 0 && x < g_game->width && y >= 0 && y < g_game->height)
        return &g_game->cells[y * g_game->width + x];
    return 0;
}

// Same lookup, but the row's sign is tested on the 16.16 pixel row `py` that
// the caller keeps in parallel with the cell row `y`.
static inline Cell* GetCellPixel(int x, int y, int py)
{
    if (x >= 0 && x < g_game->width && py >= 0 && y < g_game->height)
        return &g_game->cells[y * g_game->width + x];
    return 0;
}

static inline void ClearFeature(Cell* c)
{
    unsigned short f = c->feature;
    if (f == 0xffff || f == 0xfffe)
        c->feature = 0xfffd;
}

inline int LodRaw_00481930(Params* params)
{
    return params->field_8 / 32;
}

inline int Lod_00481930(Params* params)
{
    int v = LodRaw_00481930(params);
    return v < 0 ? 0 : v;
}
// FUNCTION: 0x4814c0 _$E4
LosTables g_losTables;

// FUNCTION: 0x4814f0
void FreeLosTables()
{
    g_losTables.FreeTables();
}

// FUNCTION: 0x481500
void InitEyeballs()
{
    g_game->count = 0;
    g_game->eyes = (Eye*)FUN_004d83b0("EYEBALL MEMORY", 0x2d0);
}

// FUNCTION: 0x481530
void FreeEyeballs()
{
    int temp = (int)g_game->eyes;
    FUN_004d85a0((void*)temp);
}

// FUNCTION: 0x481550
Cell* __stdcall GetMapCell(int x, int y)
{
    if (x >= 0 && x < g_game->width && y >= 0 && y < g_game->height)
        return &g_game->cells[y * g_game->width + x];
    return 0;
}

// FUNCTION: 0x4815a0
Cell* __stdcall GetMapCellAtPosition(Vec3* pos)
{
    int x = pos->x >> 20;
    int y = pos->z >> 20;
    if (x >= 0 && x < g_game->width && y >= 0 && y < g_game->height)
        return &g_game->cells[y * g_game->width + x];
    return 0;
}

// FUNCTION: 0x4815f0
Cell* __stdcall GetOriginCellAtPosition(Vec3* pos)
{
    int x = pos->x >> 20;
    int z = pos->z >> 20;
    if (x < 0 || x >= g_game->width || z < 0 || z >= g_game->height)
        return 0;
    Cell* cell = &g_game->cells[z * g_game->width + x];
    if (cell->feature == 0xfffe)
    {
        x -= cell->sf.offsetX;
        z -= cell->sf.offsetY;
        if (x < 0 || x >= g_game->width || z < 0 || z >= g_game->height)
            cell = 0;
        else
            cell = &g_game->cells[z * g_game->width + x];
    }
    return cell;
}

// FUNCTION: 0x482090
void __stdcall RemoveUnitLineOfSight(Unit* unit)
{
    Params p;
    p.field_0 = unit->owner;
    p.field_4 = unit->cell;
    p.field_8 = unit->type->range;
    p.field_c = unit->losSightFrameIdx;
    p.pos = unit->pos;
    p.field_a = unit->type->field_170;
    int minY = (g_game->seaLevel + 1) << 16;
    if (p.pos.y < minY) {
        p.pos.y = minY;
    }
    RemoveLineOfSight(&p);
}

// FUNCTION: 0x482110
int __stdcall IsEyeballExpired(Eye* obj)
{
    unsigned int field_val = obj->expires;
    unsigned int cmp_val = g_game->ticks;
    return field_val < cmp_val ? 1 : 0;
}

// FUNCTION: 0x482130
void ExpireEyeballs()
{
    // Left uninitialised, as in the original.
    int changed;
    Eye* p = g_game->eyes;
    for (int i = 0; i < g_game->count; i++, p++) {
        if (p->expires < g_game->ticks) {
            RemoveLineOfSight((Params*)p);
            changed = 1;
        }
    }
    if (!changed)
        return;
    Eye* end = g_game->eyes + g_game->count;
    Eye* n = std::remove_if(g_game->eyes, end, Expired());
    g_game->count = n - g_game->eyes;
}

// Needed: their symbols are what 0x4825b0's, 0x482830's and 0x483fa0's
// register plans match at.
#include <io.h>
#include <float.h>

// FUNCTION: 0x4825b0
void __stdcall UpdateLineOfSight(Params* params)
{
    if ((g_game->flags.raw & 4) == 4) {
        int x = ((short*)&params->pos.x)[1] >> 5;
        int v = params->field_a + ((short*)&params->pos.y)[1];
        if (v < 0) {
            v = 0;
        }
        if (v > 0xff) {
            v = 0xff;
        }
        int y = (((short*)&params->pos.z)[1] - (v >> 1)) >> 5;
        int diff = abs((int)*params->field_c - v);
        unsigned char c = *params->field_c;
        if (params->field_4[0] != x || params->field_4[1] != y || diff > 5) {
            if (c != 0 && (g_game->flags.raw & 2)) {
                RemoveLineOfSight(params);
            }
            params->field_4[0] = (short)x;
            params->field_4[1] = (short)y;
            if ((unsigned)x >= g_game->grid1.width || (unsigned)y >= g_game->grid1.height) {
                *params->field_c = 0;
                return;
            }
            *params->field_c = (unsigned char)v;
            if ((unsigned char)(g_game->flags.raw >> 1) & 1) {
                AddLineOfSight(params);
            }
            if (g_game->flags.raw & 1) {
                RevealAroundUnit(params);
            }
        }
    }
    else {
        int i = (short)params->field_8 / 32 - 5;
        if (i < 0) {
            i = 0;
        } else if (i >= g_game->losTable->count) {
            i = g_game->losTable->count - 1;
        }
        // cx before cy: puts its magic multiply ahead of the subtraction.
        int cx = params->pos.x / 0x200000;
        int cy = params->pos.z / 0x200000 - ((short*)&params->pos.y)[1] / 64;
        Bitmap* e = GetGafFrame(g_game->losTable, i);
        cx -= e->field_4;
        cy -= e->field_6;
        if (params->field_4[0] != cx || params->field_4[1] != cy || *params->field_c != i) {
            if ((g_game->flags.raw & 2) == 2) {
                RemoveLineOfSight(params);
                params->field_4[0] = (short)cx;
                params->field_4[1] = (short)cy;
                *params->field_c = (unsigned char)i;
                AddLineOfSight(params);
            } else {
                params->field_4[0] = (short)cx;
                params->field_4[1] = (short)cy;
                *params->field_c = (unsigned char)i;
            }
            if (g_game->flags.raw & 1) {
                RevealAroundUnit(params);
            }
        }
    }
}

// FUNCTION: 0x4827b0
void __stdcall UpdateUnitLineOfSight(Unit* unit)
{
    Params p;
    p.field_0 = unit->owner;
    p.field_4 = unit->cell;
    p.field_8 = unit->type->range;
    p.field_c = unit->losSightFrameIdx;
    p.pos = unit->pos;
    p.field_a = unit->type->field_170;
    int minY = (g_game->seaLevel + 1) << 16;
    if (p.pos.y < minY) {
        p.pos.y = minY;
    }
    UpdateLineOfSight(&p);
}

// FUNCTION: 0x482830
void __stdcall InitUnitSightCircleReveal(Params* params)
{
    if ((g_game->flags.rawByte & 2) != 2) {
        return;
    }
    *params->field_c = 0;
    if ((g_game->flags.rawByte & 4) == 4) {
        UpdateLineOfSight(params);
        return;
    }
    // Clamp reads g_game->losTable->count twice, not through a local table pointer.
    int lod = params->field_8 / 32 - 5;
    if (lod < 0) {
        lod = 0;
    } else {
        if (lod >= g_game->losTable->count) {
            lod = g_game->losTable->count - 1;
        }
    }
    int x = params->pos.x / 0x200000;
    int y = params->pos.z / 0x200000 - params->pos.y_hi / 64;
    Bitmap* entry = GetGafFrame(g_game->losTable, lod);
    x -= entry->field_4;
    y -= entry->field_6;
    params->field_4[0] = (short)x;
    params->field_4[1] = (short)y;
    *params->field_c = (char)lod;
    AddLineOfSight(params);
    RevealAroundUnit(params);
}

// FUNCTION: 0x482c20
void BuildDerivedLayers(void)
{
    Rec* rec = new Rec;
    g_game->field_142b7 = rec;
    g_game->field_142b7->flags = 0x1f;

    Grid2* grid2 = &g_game->grid2;
    int b = g_game->baseY * 0x10000;
    int a = g_game->baseX * 0x10000;
    grid2->field_14 = b;
    grid2->field_10 = a;
    int w2;
    int h2;
    h2 = (b + 0x7fffff) >> 0x17;
    w2 = (a + 0x7fffff) >> 0x17;
    grid2->width = w2;
    grid2->height = h2;
    operator delete(grid2->cells);
    int count2 = (h2 * w2 + 7) & 0xfffffff8;
    grid2->count = count2;

    grid2->cells = count2 != 0 ? (FogCell*)new Rec[count2] : 0;
    int outer;
    unsigned char* cells2 = (unsigned char*)grid2->cells;
    unsigned char* cellp = (unsigned char*)g_game->cells;
    int inner;
    int accum;

    for (unsigned int lb1 = 0; lb1 < (unsigned int)grid2->width; lb1++)
        ((Rec*)grid2->cells)[lb1].flags |= 1;
    for (unsigned int lb2 = 0; lb2 < (unsigned int)grid2->width; lb2++)
        ((Rec*)grid2->cells)[(grid2->height - 1) * grid2->width + lb2].flags |= 2;
    for (unsigned int lb3 = 0; lb3 < (unsigned int)grid2->height; lb3++)
        ((Rec*)grid2->cells)[lb3 * grid2->width].flags |= 4;
    for (unsigned int i = 0; i < (unsigned int)grid2->height; i++) {
        int* fp = &((Rec*)grid2->cells)[(i + 1) * grid2->width - 1].flags;
        *fp = *fp | 8;
    }

    for (int d = 0; d < grid2->count; d++)
        ((unsigned char*)grid2->cells)[d * 10] = g_game->seaLevel;

    {
        for (int y = 0; y < g_game->height; y++) {
            unsigned char* recp = cells2;
            for (int x = 0; x < g_game->width; x++, cellp += 0xd) {
                if (cellp[5] > recp[0])
                    recp[0] = cellp[5];
                if ((x & 7) == 7)
                    recp += 10;
            }
            if ((y & 7) == 7)
                cells2 += grid2->width * 10;
        }
    }

    for (unsigned int r = 0; r < (unsigned int)grid2->height; r++) {
        unsigned char* p = (unsigned char*)grid2->cells + r * grid2->width * 10;
        unsigned char prev = 0;
        for (unsigned int fr = 1; fr < (unsigned int)grid2->width; fr++, p += 10) {
            unsigned char t = p[10];
            unsigned char last = prev;
            prev = p[0];
            if (prev <= t)
                prev = t;
            // Ternary, not an if with a result local.
            p[1] = last > prev ? last : prev;
        }
        p[1] = prev;
    }

    for (outer = 0; (unsigned int)outer < (unsigned int)grid2->width; outer++) {
        unsigned char* p = (unsigned char*)grid2->cells + outer * 10;
        unsigned char prev = 0;
        for (unsigned int gc = 1; gc < (unsigned int)grid2->height; gc++, p += grid2->width * 10) {
            unsigned char t = p[grid2->width * 10 + 1];
            unsigned char last = prev;
            prev = p[1];
            if (prev <= t)
                prev = t;
            p[1] = last > prev ? last : prev;

        }
        p[1] = prev;
    }

    Grid* grid1 = &g_game->grid1;
    // Declared apart from the assignments: decides which register each takes.
    int w1, h1;
    h1 = g_game->height / 2;
    w1 = g_game->width / 2;

    grid1->width = w1;
    grid1->height = h1;
    operator delete(grid1->cells);
    int count1 = (h1 * w1 + 7) & 0xfffffff8;
    grid1->count = count1;
    grid1->cells = count1 != 0 ? (FogCell*)operator new(count1 * 2) : 0;
    for (int hf = 0; hf < grid1->count; hf++) {
        ((unsigned char*)grid1->cells)[hf * 2] = 0;
        ((unsigned char*)grid1->cells)[hf * 2 + 1] = 0xff;
    }

    for (outer = 0; outer < g_game->width; outer++) {
        int t20 = (outer - 1) >> 1;
        int t24 = outer >> 1;
        unsigned char* p1 = 0;
        unsigned char* p2 = 0;
        inner = 0;
        // Guarded do-while: sinks the accum = 0 store below the guard.
        if (inner < g_game->height) {
            accum = 0;
            do {
                int idx = g_game->width * inner + outer;
                int cellval = ((unsigned char*)g_game->cells)[idx * 0xd + 4];
                int v = accum - (cellval >> 1);
                int block = v >> 5;
                if (block > -1) {
                    int q = ((block * 32 + 31) * cellval) / (v + 31);
                    if (p1) {
                        p1[0] = max(p1[0], q);
                        p1[1] = min(p1[1], q);
                    }
                    if (p2) {
                        p2[0] = max(p2[0], q);
                        p2[1] = min(p2[1], q);
                    }
                    if ((unsigned int)t20 < (unsigned int)grid1->width
                            && (unsigned int)block < (unsigned int)grid1->height) {
                        p1 = (unsigned char*)grid1->cells + (block * grid1->width + t20) * 2;
                        p1[0] = max(p1[0], q);
                        p1[1] = min(p1[1], q);
                    } else {
                        p1 = 0;
                    }
                    if (t20 != t24
                            && (unsigned int)t24 < (unsigned int)grid1->width
                            && (unsigned int)block < (unsigned int)grid1->height) {
                        p2 = (unsigned char*)grid1->cells + (block * grid1->width + t24) * 2;
                        p2[0] = max(p2[0], q);
                        p2[1] = min(p2[1], q);
                    } else {
                        p2 = 0;
                    }
                }
                if (p1) {
                    p1[0] = max(p1[0], cellval);
                    p1[1] = min(p1[1], cellval);
                }
                if (p2) {
                    p2[0] = max(p2[0], cellval);
                    p2[1] = min(p2[1], cellval);
                }
                inner++;
                accum += 0x10;
            } while (inner < g_game->height);
        }
    }

    int def = g_game->seaLevel;
    for (int jf = 0; jf < grid1->count; jf++) {
        unsigned char* p = (unsigned char*)grid1->cells + jf * 2;
        int aa = p[0];
        int bb = p[1];
        int q1 = (aa * 2 + bb) / 3;
        int q2 = (aa + bb * 2) / 3;
        if (q1 <= def)
            q1 = def;
        p[0] = q1;
        if (q2 <= def)
            q2 = def;
        p[1] = q2;
    }
}

// FUNCTION: 0x483210
void __stdcall UpdateCellHeightRange(Point pos, Point size)
{
    int width = g_game->width;
    int height = g_game->height;
    int xmax = SumX(pos, size);
    int ymax = SumY(pos, size);
    if (pos.x < 0)
        pos.x = 0;
    if (pos.y < 0)
        pos.y = 0;
    if (xmax >= width)
        xmax = width - 1;
    if (ymax >= height)
        ymax = height - 1;
    if (pos.x >= xmax)
        return;
    if (pos.y >= ymax)
        return;
    int col, row;
    for (row = pos.y; row < ymax; row++) {
        Cell* c = &g_game->cells[row * width + pos.x];
        for (col = pos.x; col < xmax; col++, c++) {
            unsigned char lo = c->height;
            unsigned char hi = c->height;
            if (col < width - 1) {
                unsigned char v = c[1].height;
                if (v < lo)
                    lo = v;
                if (v > hi)
                    hi = v;
            }
            if (row < height - 1) {
                unsigned char v = c[width].height;
                if (v < lo)
                    lo = v;
                if (v > hi)
                    hi = v;
                if (col < width - 1) {
                    unsigned char v2 = c[width + 1].height;
                    if (v2 < lo)
                        lo = v2;
                    if (v2 > hi)
                        hi = v2;
                }
            }
            c->low = lo;
            c->high = hi;
        }
    }
}

// FUNCTION: 0x483370
void UpdateAllCellHeightRanges()
{
    UpdateCellHeightRange(MakePoint(0, 0), MakePoint(g_game->width, g_game->height));
}

// FUNCTION: 0x4833b0
void ClearBorderFeatures()
{
    g_game->mapWidth = g_game->baseX - 0x20;
    g_game->mapHeight = g_game->baseY - 0x80;

    int col = g_game->width - 2;
    for (int row = 0; row < g_game->height; row++) {
        Cell* c = GetCell(col, row);
        ClearFeature(c);
        ClearFeature(c + 1);
    }

    for (int x = 0; x < g_game->width; x++) {
        int y = 0;
        int y16 = 0;
        for (;; y++, y16 += 16) {
            Cell* c = GetCellPixel(x, y, y16);
            if (y16 - (c->height >> 1) >= 0)
                break;
            ClearFeature(c);
        }
    }

    for (int x2 = 0; x2 < g_game->width; x2++) {
        int y2 = g_game->height - 1;
        int y16b = y2 * 16;
        for (;; y2--, y16b -= 16) {
            Cell* c = GetCell(x2, y2);
            if (y16b - (c->height >> 1) <= g_game->mapHeight)
                break;
            Cell* prev = c - g_game->width;
            ClearFeature(prev);
        }
    }

    if (g_game->net->field_d44 != 0) {
        Cell* c = g_game->cells;
        Cell* end = c + g_game->width * g_game->height;
        while (c < end) {
            if (c->low <= g_game->seaLevel)
                ClearFeature(c);
            c++;
        }
    }
}

// FUNCTION: 0x483610
void LoadTntMap()
{
    int* mapSettings = (int*)((char*)g_game + 0x141fb);
    TntInfo info;
    TntHeader pic;
    char text[64];
    Slot a;
    Slot b;
    int* tnt;

    // REGION r1 begin
    tnt = ((Mission*)*(void**)((char*)g_game + 0x391e9))->GetNameSlot(1);
    tnt = LoadFileWithProgress(tnt);
    info.version = *tnt;
    switch (info.version) {
    case 0x1020:
        info.width = tnt[1];
        info.height = tnt[2];
        info.flag = tnt[9];
        info.sea_d = tnt[0xd];
        info.sea_a = tnt[10];
        info.sea_b = tnt[0xb];
        info.tile_count = tnt[7];
        info.map_size = tnt[8] + (int)tnt;
        info.tile_set_count = tnt[6];
        info.tile_set_src = (int*)(tnt[5] + (int)tnt);
        info.tile_map_src = (int*)(tnt[3] + (int)tnt);
        info.attr_b = (unsigned char*)(tnt[4] + (int)tnt);
        info.attr_a = 0;
        info.attr_limit = 0xfc;
        info.feature_flags = info.feature_flags ^ ((tnt[0xf] ^ info.feature_flags) & 1);
        info.feature_data = (unsigned short*)(tnt[0xe] + (int)tnt);
        break;
    case 0x2000:
        info.width = tnt[1];
        info.height = tnt[2];
        info.flag = tnt[9];
        info.sea_d = 0;
        info.sea_a = 100;
        info.sea_b = 2000;
        info.tile_count = tnt[7];
        info.map_size = tnt[8] + (int)tnt;
        info.tile_set_count = tnt[6];
        info.tile_set_src = (int*)(tnt[5] + (int)tnt);
        info.tile_map_src = (int*)(tnt[3] + (int)tnt);
        info.attr_a = (unsigned char*)(tnt[4] + (int)tnt);
        info.attr_b = 0;
        info.attr_limit = 0xfffb;
        info.feature_flags = info.feature_flags ^ ((tnt[0xb] ^ info.feature_flags) & 1);
        info.feature_data = (unsigned short*)(tnt[10] + (int)tnt);
        break;
    default:
        sprintf(text, "Unknown TNT version:  0x%08x", info.version);
        FatalError(text);
        break;
    }
    // REGION r1 end

    // REGION r2 begin
    a.n = *(int*)(*(int*)((char*)g_game + 0x391e9) + 0xd34);
    if (a.n >= 0 && info.version >= 0x2000)
        *(int*)((char*)mapSettings + 0x60) = a.n;
    else
        *(int*)((char*)mapSettings + 0x60) = info.sea_a;
    a.n = *(int*)(*(int*)((char*)g_game + 0x391e9) + 0xd38);
    if (a.n >= 0 && info.version >= 0x2000)
        *(int*)((char*)mapSettings + 0x64) = a.n;
    else
        *(int*)((char*)mapSettings + 0x64) = info.sea_b;
    a.n = *(int*)(*(int*)((char*)g_game + 0x391e9) + 0xd3c);
    // Parenthesised so the two constant multiplies are not folded into one.
    if (a.n >= 0 && info.version >= 0x2000)
        *(int*)((char*)mapSettings + 0x68) = (int)((a.n * 65536.0) * 0.0011111111111111111);
    else if (info.sea_d != 0)
        *(int*)((char*)mapSettings + 0x68) = (int)((info.sea_d * 65536.0) * 0.0011111111111111111);
    else
        *(int*)((char*)mapSettings + 0x68) = 0x1fdb;
    if (*(float*)(*(int*)((char*)g_game + 0x391e9) + 0xd40) >= 0.0f)
        *(int*)((char*)mapSettings + 0x6c) = *(int*)(*(int*)((char*)g_game + 0x391e9) + 0xd40);
    else
        *(int*)((char*)mapSettings + 0x6c) = 0x3f000000;
    *(unsigned char*)((char*)mapSettings + 0x84) = (unsigned char)info.flag;
    *(int*)((char*)mapSettings + 0x38) = info.width;
    *(int*)((char*)mapSettings + 0x3c) = info.height;
    mapSettings[10] = mapSettings[14] << 4;
    mapSettings[11] = mapSettings[15] << 4;
    if (info.feature_flags & 1) {
        pic.width = *info.feature_data;
        pic.height = info.feature_data[2];
        pic.pad0 = 0;
        pic.pad1 = 0;
        pic.flag[0] = 0;
        pic.flag[1] = 0;
        pic.flag[2] = 0;
        pic.flag[3] = 0;
        pic.zero0 = 0;
        pic.data = info.feature_data + 4;
        pic.zero1 = 0;
        *(void**)((char*)g_game + 0x1426b) = AllocFrame("TED GENERATED PIC", *(int*)info.feature_data, *(int*)(info.feature_data + 2));
        SurfaceFromFrame(text, *(void**)((char*)g_game + 0x1426b));
        DrawFrame(text, &pic, 0, 0);
    } else {
        *(int*)((char*)g_game + 0x1426b) = 0;
    }
    // REGION r2 end

    // REGION r3 begin
    a.n = (mapSettings[10] / 32) * (mapSettings[11] / 32);
    int* dst = (int*)FUN_004d83b0("TILE MAP", a.n * 2);
    mapSettings[36] = (int)dst;
    memcpy(dst, info.tile_map_src, a.n * 2);
    a.n = mapSettings[14] * mapSettings[15];
    unsigned char* plot = (unsigned char*)FUN_004d83b0("PLOT MEMORY", a.n * 0xd);
    mapSettings[35] = (int)plot;
    int fill = *(int*)(*(int*)((char*)g_game + 0x391e9) + 0xd30);
    if (fill < 0 || info.version < 0x2000)
        fill = 0;
    for (int i = a.n; i > 0; i--) {
        plot[0xc] &= 0xfc;
        *(unsigned short*)plot = 0;
        *(unsigned short*)(plot + 2) = 0;
        *(unsigned short*)(plot + 8) = 0xffff;
        plot[7] = (unsigned char)fill;
        plot += 0xd;
    }
    InitFeatureAnimPool(&info.version);
    if (info.attr_b != 0) {
        unsigned char* q = *(unsigned char**)&mapSettings[35];
        if (a.n > 0) {
            unsigned char* src = info.attr_b;
            for (int i = a.n; i > 0; i--) {
                q[4] = *src;
                q[7] = src[6];
                q[0xc] = (q[0xc] & 0xd7) | 0x50;
                q += 0xd;
                src += 8;
            }
        }
        if (*(int*)((char*)g_game + 0x38d6b) == 0) {
            q = *(unsigned char**)&mapSettings[35];
            if (a.n > 0) {
                unsigned char* src = info.attr_b + 2;
                do {
                    if (*src < info.attr_limit)
                        PlaceFeature(q, *src, 0, 0, 10);
                    q += 0xd;
                    src += 8;
                    a.n--;
                } while (a.n != 0);
            }
        }
    } else {
        if (info.attr_a != 0) {
            unsigned char* q = *(unsigned char**)&mapSettings[35];
            unsigned char* src = info.attr_a;
            if (a.n > 0) {
                b.n = a.n;
                do {
                    q[4] = *src;
                    q[0xc] = (q[0xc] & 0xd7) | 0x50;
                    if (*(unsigned short*)(src + 1) == 0xfffc)
                        PlaceFeature(q, 0xfffc, 0, 0, 10);
                    q += 0xd;
                    src += 4;
                    b.n--;
                } while (b.n != 0);
            }
            if (*(int*)((char*)g_game + 0x38d6b) == 0) {
                q = *(unsigned char**)&mapSettings[35];
                if (a.n > 0) {
                    unsigned short* sp = (unsigned short*)(info.attr_a + 1);
                    int n = a.n;
                    do {
                        if ((int)*sp < info.attr_limit)
                            PlaceFeature(q, *sp, 0, 0, 10);
                        q += 0xd;
                        sp += 2;
                        n--;
                    } while (n != 0);
                }
                PlaceMissionFeatures();
            }
        }
    }
    // REGION r3 end

    // REGION r4 begin
    unsigned int* set = (unsigned int*)FUN_004d83b0("TILE SET", info.tile_set_count * 0x400 + 8);
    *(unsigned int**)((char*)mapSettings + 0x88) = set;
    *set = info.tile_set_count;
    *(int*)(*(int*)((char*)mapSettings + 0x88) + 4) = *(int*)((char*)mapSettings + 0x88) + 8;
    memcpy(*(void**)(*(int*)((char*)mapSettings + 0x88) + 4), info.tile_set_src, info.tile_set_count * 0x400);
    FUN_004d85a0(tnt);
    g_losTables.LoadLosTables();
    int mw = *(int*)((char*)g_game + 0x37e37);
    int mh = *(int*)((char*)g_game + 0x37e3b);
    *(int*)((char*)mapSettings + 0x40) = mw / 16;
    *(int*)((char*)mapSettings + 0x44) = mh / 16;
    *(int*)((char*)mapSettings + 0x48) = mw / 32;
    *(int*)((char*)mapSettings + 0x4c) = mh / 32;
    int* list = (int*)operator new(0x10);
    int* obj = 0;
    if (list != 0) {
        list[1] = 0;
        list[2] = 0;
        list[3] = 0;
        list[0] = 0;
        obj = list;
    }
    int rb = 2;
    *(int**)((char*)mapSettings + 0x24) = obj;
    a.n = 2;
    if (mw % 32 != 0)
        a.n = 3;
    if (mh % 32 != 0)
        rb = 3;
    int cols, rows;
    rows = *(int*)((char*)mapSettings + 0x44) / 2 + rb;
    cols = *(int*)((char*)mapSettings + 0x40) / 2 + a.n;
    obj[1] = cols;
    obj[2] = rows;
    operator delete((void*)obj[0]);
    unsigned int total = (rows * cols + 7U) & 0xfffffff8;
    obj[3] = total;
    int buf;
    if (total != 0)
        buf = (int)operator new(total * 2);
    else
        buf = 0;
    obj[0] = buf;
    *(unsigned short*)((char*)g_game + 0x14281) &= 0xfff7;
    b.p.x = 0;
    b.p.y = 0;
    a.p.x = *(short*)((char*)g_game + 0x14233);
    a.p.y = *(short*)((char*)g_game + 0x14237);
    UpdateCellHeightRange(b.p, a.p);
    // REGION r4 end

    // REGION r5 begin
    BuildDerivedLayers();
    ClearBorderFeatures();
    unsigned int total2 = (unsigned int)(mapSettings[14] * mapSettings[15]) * 2;
    unsigned int half = total2 / 4;
    int* mapped = (int*)FUN_004d83b0("MAPPED MEMORY", half);
    mapSettings[30] = (int)mapped;
    memset(mapped, 0, half);
    int sx = mapSettings[16] + 0xc;
    int sy = mapSettings[17] + 0x20;
    mapSettings[21] = sy;
    mapSettings[20] = sx;
    *mapSettings = (int)FUN_004d83b0("SORT UNIT LIST", sy * sx * 4);
    mapSettings[1] = (int)FUN_004d83b0("SORT INDICES", mapSettings[21] << 2);
    mapSettings[2] = (int)FUN_004d83b0("SORT LINE COUNT", mapSettings[21] << 1);
    StampFeatureMetal();
    *(int*)((char*)g_game + 0x14277) = 0;
    *(int*)((char*)g_game + 0x1427b) = (int)FUN_004d83b0("EYEBALL MEMORY", 0x2d0);
    mapSettings[23] = 0;
    *(unsigned char*)((char*)g_game + 0x38d70) = 100;
    // REGION r5 end
}

// FUNCTION: 0x483dd0
void FreeMapResources()
{
    FUN_004d85a0(g_game->eyes);
    FreeFeaturePool();
    if (g_game->radarFrame) {
        FUN_004d85a0(g_game->radarFrame);
        g_game->radarFrame = 0;
    }
    FUN_004d85a0(g_game->sortLineCount);
    FUN_004d85a0(g_game->sortIndices);
    FUN_004d85a0(g_game->sortUnits);
    FUN_004d85a0(g_game->iconSet);
    FUN_004d85a0(g_game->visibilityMask);
    FUN_004d85a0(g_game->cells);
    FUN_004d85a0(g_game->mapValues);
    g_game->sortLineCount = 0;
    g_game->sortIndices = 0;
    g_game->sortUnits = 0;
    g_game->iconSet = 0;
    g_game->visibilityMask = 0;
    g_game->cells = 0;
    g_game->mapValues = 0;

    void** p = (void**)g_game->grid;
    if (p) {
        operator delete(p[0]);
        operator delete(p);
    }
    g_game->grid = 0;

    Grid* g1 = &g_game->grid1;
    g1->width = 0;
    g1->height = 0;
    operator delete(g1->cells);
    g1->count = 0;
    g1->cells = 0;

    Grid2* g2 = &g_game->grid2;
    g2->width = 0;
    g2->height = 0;
    operator delete(g2->cells);
    g2->count = 0;
    g2->cells = 0;

    operator delete(g_game->field_142b7);
    g_game->field_142b7 = 0;
}

// FUNCTION: 0x483fa0
void __stdcall DrawMapTiles(void* surface)
{
    // Declared together: tilesX/tilesY before tileX/tileY, offX/offY before
    // viewW/viewH.
    int tilesX, tilesY;
    int tileX, tileY;
    int screenX, screenY;
    int offX, offY;
    int edgeX, edgeY;
    int scrollX, scrollY, viewW, viewH;
    Bitmap bmp;
    screenX = g_game->rect.left;
    screenY = g_game->rect.top;
    scrollX = g_game->scrollX;
    scrollY = g_game->scrollY;
    // View size is loaded before the divisions.
    viewW = g_game->viewW;
    viewH = g_game->viewH;
    tileX = scrollX / 32;
    tileY = scrollY / 32;
    offX = scrollX - tileX * 32;
    offY = scrollY - tileY * 32;
    tilesX = (viewW + offX) / 32;
    tilesY = (viewH + offY) / 32;
    edgeX = (viewW - tilesX * 32) + offX;
    edgeY = (viewH - tilesY * 32) + offY;
    if (edgeX != 0)
        tilesX++;
    if (edgeY != 0)
        tilesY++;
    int stride = g_game->width / 2;
    bmp.width = 32;
    bmp.height = 32;
    bmp.field_4 = 0;
    bmp.field_6 = 0;
    bmp.flag9 = 0;
    bmp.count = 0;

    // Left and right columns of partial tiles.
    if (offX != 0 || edgeX != 0) {
        unsigned short* left = g_game->mapValues + tileY * stride + tileX;
        unsigned short* right = g_game->mapValues + tileY * stride + tileX + tilesX - 1;
        // Index loops: screen coordinates are written from the index, no counters.
        for (int j = 0; j < tilesY; j++) {
            if (offX != 0) {
                bmp.data = g_game->iconSet->data + *left * 0x400;
                DrawFrameOpaque(surface, &bmp, screenX - offX, (screenY + j * 32) - offY);
            }
            if (edgeX != 0) {
                bmp.data = g_game->iconSet->data + *right * 0x400;
                DrawFrameOpaque(surface, &bmp, tilesX * 32 + screenX - offX - 32, (screenY + j * 32) - offY);
            }
            left += stride;
            right += stride;
        }
    }

    // Top and bottom rows of partial tiles.
    if (offY != 0 || edgeY != 0) {
        unsigned short* top = g_game->mapValues + tileY * stride + tileX;
        unsigned short* bottom = g_game->mapValues + (tileY + tilesY - 1) * stride + tileX;
        for (int i = 0; i < tilesX; i++) {
            if (offY != 0) {
                bmp.data = g_game->iconSet->data + *top * 0x400;
                DrawFrameOpaque(surface, &bmp, (screenX + i * 32) - offX, screenY - offY);
            }
            if (edgeY != 0) {
                bmp.data = g_game->iconSet->data + *bottom * 0x400;
                DrawFrameOpaque(surface, &bmp, (screenX + i * 32) - offX, tilesY * 32 + screenY - offY - 32);
            }
            top++;
            bottom++;
        }
    }

    // Leave only the whole tiles for the interior.
    if (offX != 0) {
        tilesX--;
        screenX += 32 - offX;
        tileX++;
    }
    if (offY != 0) {
        tilesY--;
        screenY += 32 - offY;
        tileY++;
    }
    if (edgeX != 0)
        tilesX--;
    if (edgeY != 0)
        tilesY--;

    for (int j = 0; j < tilesY; j++) {
        unsigned short* p = g_game->mapValues + (tileY + j) * (unsigned short)stride + tileX;
        // Map pointer advanced in the for increment.
        for (int i = 0; i < tilesX; i++, p++) {
            DrawTile(surface, screenX + i * 32, screenY + j * 32, g_game->iconSet->data + *p * 0x400);
        }
    }
}

// FUNCTION: 0x4848e0
void __stdcall DrawFogOfWar(void* surface)
{
    if (!(g_game->flags.raw & 8)) {
        BuildFogTiles();
        g_game->flags.raw |= 8;
    }

    Grid* grid = g_game->grid;
    int scrollY = g_game->scrollY;
    int scrollX = g_game->scrollX;
    int q = (scrollY + 16) / 32 + (scrollX + 16) / 32;
    // One operand read through g_game, not the local: fixes the LEA operand order.
    int parity = (g_game->scrollX + scrollY) & 1;
    int rX = scrollX % 32;
    int rY = scrollY % 32;
    int fx;
    if (rX < 16) fx = -16 - rX;
    else fx = 16 - rX;
    int fy;
    if (rY < 16) fy = -16 - rY;
    else fy = 16 - rY;
    Rect r;

    for (unsigned int j = 0; j < (unsigned int)grid->height; j++) {
        for (unsigned int i = 0; i < (unsigned int)grid->width; i++) {
            FogCell* cell = &grid->cells[j * grid->width + i];
            r.left = g_game->rect.left + fx + (i << 5);
            r.top = g_game->rect.top + fy + (j << 5);
            r.right = r.left + 31;
            r.bottom = r.top + 31;
            if (cell->level0 == 0xf) {
                FillRectangle(surface, &r, g_game->colors[0]);
            } else {
                if (cell->level1 != 0) {
                    if (cell->level1 != 0xf) {
                        void* bmp = GetGafFrame(g_game->gray[(i + j + q) & 3], cell->level1 - 1);
                        if (g_game->flags_37f06.ditheredFog) {
                            EraseFrameDithered(surface, bmp, r.left, r.top, parity);
                        } else {
                            DrawFrameGray(surface, bmp, r.left, r.top);
                        }
                    } else {
                        if (g_game->flags_37f06.ditheredFog) {
                            DitherRectangle(surface, &r, parity);
                        } else {
                            GrayRectangle(surface, &r);
                        }
                    }
                }
                if (cell->level0 > 0) {
                    void* bmp = GetGafFrame(g_game->black[(i + j + q) & 3], cell->level0 - 1);
                    DrawFrame(surface, bmp, r.left, r.top);
                }
            }
        }
    }
}

// FUNCTION: 0x484b50
void __stdcall ClampWorldPosToTerrain(int x, int y, Vec3* out)
{
    if (x < 0)
        x = 0;
    if (x >= g_game->baseX)
        x = g_game->baseX - 1;
    if (y < 0)
        y = 0;
    if (y >= g_game->baseY)
        y = g_game->baseY - 1;

    Vec3 p;
    int s1;
    int s2;
    p.x = x << 16;
    int t = y & ~0xf;
    int i;
    for (i = 0x80; i >= 0; i -= 0x10) {
        // z comes from the counter, not from a variable of its own.
        p.z = (t + i) << 16;
        p.y = max(GetGroundHeight(&p), g_game->seaLevel) << 16;
        s1 = p.z_hi - (p.y_hi >> 1);
        if (s1 <= y)
            goto found;
    }
    goto done;

found:
    {
        Vec3 q = p;
        q.z = p.z + 0x100000;
        q.y = max(GetGroundHeight(&q), g_game->seaLevel) << 16;
        s2 = q.z_hi - (q.y_hi >> 1);
        if ((s1 < s2 && y >= s1) || y <= s2) {
            p.z = p.z + ((y - s1) << 20) / (s2 - s1);
            p.y = max(GetGroundHeight(&p), g_game->seaLevel) << 16;
        }
    }
done:
    *out = p;
}

