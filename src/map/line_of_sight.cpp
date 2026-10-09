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
    unsigned short unit;               // +0x0
    char unknown_2[2];
    unsigned char height;              // +0x4
    unsigned char high;                // +0x5
    unsigned char low;                 // +0x6
    unsigned char metal;               // +0x7
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
    int widthFixed;                    // +0x10
    int heightFixed;                   // +0x14
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
    int size;                          // +0xc
    unsigned char& at(int x, int y) { return cells[y * width + x]; }
};

// Unused here: the symbol ids of these declarations (a forward declaration and
// real functions) keep 0x4825b0 and 0x483210 matching (docs/c2-regalloc.md).
struct Sound;
void RegisterUnitOrders();
void RegisterGroundOrders();
void EnableAICommands();
void RegisterAICommands();
void FUN_00406f40();
void ResetAIPlayers();
void RegisterVtolOrders();
void StepAllGafSequences();
void ResetNetStats();
void FUN_004161f0();
void InitCommands();
int UpdatePlacementGhostValidity();
void RefreshSelectionOrders();
#include "../network/player.h"

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
    short losCacheCellX;               // +0x7a
    short losCacheCellZ;               // +0x7c
    char unknown_7e[0x92 - 0x7e];
    UnitDef* def;                      // +0x92
    Player* player;                    // +0x96
    char unknown_9a[0xa6 - 0x9a];
    unsigned short unitDefIndex;       // +0xa6
    char unknown_a8[0xf8 - 0xa8];
    unsigned char losSightFrameIdx[4];         // +0xf8
    char unknown_fc[0x118 - 0xfc];     // stride 0x118
};

// A GAF frame (32x32 tile bitmap); GetGafFrame returns one of these.
struct GafFrame {
    unsigned short width;              // +0x0
    unsigned short height;             // +0x2
    short xOffset;                     // +0x4
    short yOffset;                     // +0x6
    unsigned char mask;                // +0x8
    unsigned char flag9;               // +0x9
    unsigned char count;               // +0xa
    unsigned char kind;                // +0xb
    int reserved;                      // +0xc
    unsigned char* data;               // +0x10
    int scratch;                       // +0x14
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

// One unit's sight query (Thaldren's LosSightQuery): the player, the unit's cached sight
// cell, its sight distance and eye height, and the byte that holds its sight frame.
struct SightQuery {
    void* player;                      // +0x00
    short* cacheCell;                  // +0x04
    short sightDistance;               // +0x08
    unsigned char eyeHeight;           // +0x0a
    char unknown_b;                    // +0x0b
    unsigned char* frameIdx;           // +0x0c
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

// Unused here: real functions declared to keep the file's symbol count.
void __stdcall SetCameraPosition(int x, int y, int z);
void __stdcall RecalculateLineOfSight(int param);
void __stdcall CollectVisibleUnitIds();

union Slot {
    int n;
    Point p;
};

class Mission;

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
    unsigned char debugMode;
    Flags_14281 mapFlags;              // +0x14281
    IconSet* iconSet;                  // +0x14283
    Cell* cells;                       // +0x14287
    unsigned short* mapValues;         // +0x1428b
    Grid grid1;                        // +0x1428f
    Grid2 grid2;                       // +0x1429f
    Rec* overflowBucket;               // +0x142b7
    char unknown_142bb[0x142f1 - 0x142bb];
    Flags_142f1 viewDirtyFlags;        // +0x142f1
    char unknown_142f3[0x1431f - 0x142f3];
    int scrollX;                       // +0x1431f
    int scrollY;                       // +0x14323
    char unknown_14327[0x14357 - 0x14327];
    Unit* units;                       // +0x14357
    Unit* unitsEnd;                    // +0x1435b
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
    char unknown_38a4b[0x38d6b - 0x38a4b];
    int pendingSaveStore;              // +0x38d6b
    char unknown_38d6f;
    unsigned char loadProgress;        // +0x38d70
    char unknown_38d71[0x391e9 - 0x38d71];
    Mission* net;                      // +0x391e9
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
int ScanDirectory(char*, char*, char*, int, int, int);

#include "../map/mission.h"

void* __cdecl GameAllocIgnoreTag(const char* name, unsigned int size);
void __cdecl GameFreeThunk(void* p);
void __stdcall FatalError(char* message);
int* __stdcall LoadFileWithProgress(int* file);
GafFrame* __stdcall GetGafFrame(void* table, int index);
void __stdcall UpdateLineOfSight(SightQuery* params);
void __stdcall AddLineOfSight(SightQuery* params);
void __stdcall RemoveLineOfSight(SightQuery* params);
void __stdcall RevealAroundUnit(SightQuery* params);
void __stdcall UpdateCellHeightRange(Point pos, Point size);
void UpdateRadarMapped();
void DrawRadarUnits();
void FreeFeaturePool();
void BuildFogTiles();
void* __stdcall AllocFrame(const char* name, int width, int height);
void __stdcall SurfaceFromFrame(void* dst, void* src);
void __stdcall DrawFrame(void* surface, void* header, int x, int y);
void __stdcall DrawFrameOpaque(void* dst, GafFrame* bmp, int x, int y);
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

inline int LodRaw_00481930(SightQuery* params)
{
    return params->sightDistance / 32;
}

inline int Lod_00481930(SightQuery* params)
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
    g_game->eyes = (Eye*)GameAllocIgnoreTag("EYEBALL MEMORY", 0x2d0);
}

// FUNCTION: 0x481530
void FreeEyeballs()
{
    int temp = (int)g_game->eyes;
    GameFreeThunk((void*)temp);
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
    SightQuery p;
    p.player = unit->player;
    p.cacheCell = &unit->losCacheCellX;
    p.sightDistance = unit->def->range;
    p.frameIdx = unit->losSightFrameIdx;
    p.pos = unit->pos;
    p.eyeHeight = unit->def->field_170;
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
            RemoveLineOfSight((SightQuery*)p);
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
void __stdcall UpdateLineOfSight(SightQuery* params)
{
    if ((g_game->mapFlags.raw & 4) == 4) {
        int x = ((short*)&params->pos.x)[1] >> 5;
        int v = params->eyeHeight + ((short*)&params->pos.y)[1];
        if (v < 0) {
            v = 0;
        }
        if (v > 0xff) {
            v = 0xff;
        }
        int y = (((short*)&params->pos.z)[1] - (v >> 1)) >> 5;
        int diff = abs((int)*params->frameIdx - v);
        unsigned char c = *params->frameIdx;
        if (params->cacheCell[0] != x || params->cacheCell[1] != y || diff > 5) {
            if (c != 0 && (g_game->mapFlags.raw & 2)) {
                RemoveLineOfSight(params);
            }
            params->cacheCell[0] = (short)x;
            params->cacheCell[1] = (short)y;
            if ((unsigned)x >= g_game->grid1.width || (unsigned)y >= g_game->grid1.height) {
                *params->frameIdx = 0;
                return;
            }
            *params->frameIdx = (unsigned char)v;
            if ((unsigned char)(g_game->mapFlags.raw >> 1) & 1) {
                AddLineOfSight(params);
            }
            if (g_game->mapFlags.raw & 1) {
                RevealAroundUnit(params);
            }
        }
    }
    else {
        int i = (short)params->sightDistance / 32 - 5;
        if (i < 0) {
            i = 0;
        } else if (i >= g_game->losTable->count) {
            i = g_game->losTable->count - 1;
        }
        // cx before cy: puts its magic multiply ahead of the subtraction.
        int cx = params->pos.x / 0x200000;
        int cy = params->pos.z / 0x200000 - ((short*)&params->pos.y)[1] / 64;
        GafFrame* e = GetGafFrame(g_game->losTable, i);
        cx -= e->xOffset;
        cy -= e->yOffset;
        if (params->cacheCell[0] != cx || params->cacheCell[1] != cy || *params->frameIdx != i) {
            if ((g_game->mapFlags.raw & 2) == 2) {
                RemoveLineOfSight(params);
                params->cacheCell[0] = (short)cx;
                params->cacheCell[1] = (short)cy;
                *params->frameIdx = (unsigned char)i;
                AddLineOfSight(params);
            } else {
                params->cacheCell[0] = (short)cx;
                params->cacheCell[1] = (short)cy;
                *params->frameIdx = (unsigned char)i;
            }
            if (g_game->mapFlags.raw & 1) {
                RevealAroundUnit(params);
            }
        }
    }
}

// FUNCTION: 0x4827b0
void __stdcall UpdateUnitLineOfSight(Unit* unit)
{
    SightQuery p;
    p.player = unit->player;
    p.cacheCell = &unit->losCacheCellX;
    p.sightDistance = unit->def->range;
    p.frameIdx = unit->losSightFrameIdx;
    p.pos = unit->pos;
    p.eyeHeight = unit->def->field_170;
    int minY = (g_game->seaLevel + 1) << 16;
    if (p.pos.y < minY) {
        p.pos.y = minY;
    }
    UpdateLineOfSight(&p);
}

// FUNCTION: 0x482830
void __stdcall InitUnitSightCircleReveal(SightQuery* params)
{
    if ((g_game->mapFlags.rawByte & 2) != 2) {
        return;
    }
    *params->frameIdx = 0;
    if ((g_game->mapFlags.rawByte & 4) == 4) {
        UpdateLineOfSight(params);
        return;
    }
    // Clamp reads g_game->losTable->count twice, not through a local table pointer.
    int lod = params->sightDistance / 32 - 5;
    if (lod < 0) {
        lod = 0;
    } else {
        if (lod >= g_game->losTable->count) {
            lod = g_game->losTable->count - 1;
        }
    }
    int x = params->pos.x / 0x200000;
    int y = params->pos.z / 0x200000 - params->pos.y_hi / 64;
    GafFrame* entry = GetGafFrame(g_game->losTable, lod);
    x -= entry->xOffset;
    y -= entry->yOffset;
    params->cacheCell[0] = (short)x;
    params->cacheCell[1] = (short)y;
    *params->frameIdx = (char)lod;
    AddLineOfSight(params);
    RevealAroundUnit(params);
}

// FUNCTION: 0x482c20
void BuildDerivedLayers(void)
{
    Rec* rec = new Rec;
    g_game->overflowBucket = rec;
    g_game->overflowBucket->flags = 0x1f;

    Grid2* grid2 = &g_game->grid2;
    int b = g_game->baseY * 0x10000;
    int a = g_game->baseX * 0x10000;
    grid2->heightFixed = b;
    grid2->widthFixed = a;
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

    if (g_game->net->lavaWorld != 0) {
        Cell* c = g_game->cells;
        Cell* end = c + g_game->width * g_game->height;
        while (c < end) {
            if (c->low <= g_game->seaLevel)
                ClearFeature(c);
            c++;
        }
    }
}

// The map block of Game from +0x141fb, which LoadTntMap fills through one base pointer.
struct MapSettings {
    int* sortUnits;                    // +0x00
    int* sortIndices;                  // +0x04
    int* sortLineCount;                // +0x08
    char unknown_c[0x24 - 0xc];
    Grid* grid;                        // +0x24
    int baseX;                         // +0x28
    int baseY;                         // +0x2c
    char unknown_30[0x38 - 0x30];
    int width;                         // +0x38
    int height;                        // +0x3c
    int screenTilesX;                  // +0x40
    int screenTilesY;                  // +0x44
    int blocksX;                       // +0x48, map size / 32
    int blocksY;                       // +0x4c
    int sortRowWidth;                  // +0x50
    int sortRowCount;                  // +0x54
    char unknown_58[0x5c - 0x58];
    int featureReproduceCursor;        // +0x5c
    int windSpeedMin;                  // +0x60
    int windSpeedMax;                  // +0x64
    int rise;                          // +0x68
    int tidal;                         // +0x6c, a float copied as bits
    char unknown_70[0x78 - 0x70];
    unsigned short* visibilityMask;    // +0x78
    char unknown_7c[0x84 - 0x7c];
    unsigned char seaLevel;            // +0x84
    IconSet* iconSet;                  // +0x88
    Cell* cells;                       // +0x8c
    unsigned short* mapValues;         // +0x90
};

// FUNCTION: 0x483610
void LoadTntMap()
{
    MapSettings* mapSettings = (MapSettings*)((char*)g_game + 0x141fb);
    TntInfo info;
    GafFrame pic;
    char text[64];
    Slot a;
    Slot b;
    int* tnt;

    // REGION r1 begin
    tnt = (int*)(g_game->net)->GetNameSlot(1);
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
    a.n = g_game->net->minWindSpeed;
    if (a.n >= 0 && info.version >= 0x2000)
        mapSettings->windSpeedMin = a.n;
    else
        mapSettings->windSpeedMin = info.sea_a;
    a.n = g_game->net->maxWindSpeed;
    if (a.n >= 0 && info.version >= 0x2000)
        mapSettings->windSpeedMax = a.n;
    else
        mapSettings->windSpeedMax = info.sea_b;
    a.n = g_game->net->gravity;
    // Parenthesised so the two constant multiplies are not folded into one.
    if (a.n >= 0 && info.version >= 0x2000)
        mapSettings->rise = (int)((a.n * 65536.0) * 0.0011111111111111111);
    else if (info.sea_d != 0)
        mapSettings->rise = (int)((info.sea_d * 65536.0) * 0.0011111111111111111);
    else
        mapSettings->rise = 0x1fdb;
    if (g_game->net->tidalStrength >= 0.0f)
        mapSettings->tidal = *(int*)&g_game->net->tidalStrength;
    else
        mapSettings->tidal = 0x3f000000;
    mapSettings->seaLevel = (unsigned char)info.flag;
    mapSettings->width = info.width;
    mapSettings->height = info.height;
    mapSettings->baseX = mapSettings->width << 4;
    mapSettings->baseY = mapSettings->height << 4;
    if (info.feature_flags & 1) {
        pic.width = *info.feature_data;
        pic.height = info.feature_data[2];
        pic.xOffset = 0;
        pic.yOffset = 0;
        pic.mask = 0;
        pic.flag9 = 0;
        pic.count = 0;
        pic.kind = 0;
        pic.reserved = 0;
        pic.data = (unsigned char*)(info.feature_data + 4);
        pic.scratch = 0;
        g_game->radarFrame = AllocFrame("TED GENERATED PIC", *(int*)info.feature_data, *(int*)(info.feature_data + 2));
        SurfaceFromFrame(text, g_game->radarFrame);
        DrawFrame(text, &pic, 0, 0);
    } else {
        g_game->radarFrame = 0;
    }
    // REGION r2 end

    // REGION r3 begin
    a.n = (mapSettings->baseX / 32) * (mapSettings->baseY / 32);
    int* dst = (int*)GameAllocIgnoreTag("TILE MAP", a.n * 2);
    mapSettings->mapValues = (unsigned short*)dst;
    memcpy(dst, info.tile_map_src, a.n * 2);
    a.n = mapSettings->width * mapSettings->height;
    unsigned char* plot = (unsigned char*)GameAllocIgnoreTag("PLOT MEMORY", a.n * 0xd);
    mapSettings->cells = (Cell*)plot;
    int fill = g_game->net->surfaceMetal;
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
        unsigned char* q = (unsigned char*)mapSettings->cells;
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
        if (g_game->pendingSaveStore == 0) {
            q = (unsigned char*)mapSettings->cells;
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
            unsigned char* q = (unsigned char*)mapSettings->cells;
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
            if (g_game->pendingSaveStore == 0) {
                q = (unsigned char*)mapSettings->cells;
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
    unsigned int* set = (unsigned int*)GameAllocIgnoreTag("TILE SET", info.tile_set_count * 0x400 + 8);
    mapSettings->iconSet = (IconSet*)set;
    *set = info.tile_set_count;
    mapSettings->iconSet->data = (unsigned char*)mapSettings->iconSet + 8;
    memcpy(mapSettings->iconSet->data, info.tile_set_src, info.tile_set_count * 0x400);
    GameFreeThunk(tnt);
    g_losTables.LoadLosTables();
    int mw = g_game->viewW;
    int mh = g_game->viewH;
    mapSettings->screenTilesX = mw / 16;
    mapSettings->screenTilesY = mh / 16;
    mapSettings->blocksX = mw / 32;
    mapSettings->blocksY = mh / 32;
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
    mapSettings->grid = (Grid*)obj;
    a.n = 2;
    if (mw % 32 != 0)
        a.n = 3;
    if (mh % 32 != 0)
        rb = 3;
    int cols, rows;
    rows = mapSettings->screenTilesY / 2 + rb;
    cols = mapSettings->screenTilesX / 2 + a.n;
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
    g_game->mapFlags.raw &= 0xfff7;
    b.p.x = 0;
    b.p.y = 0;
    a.p.x = (short)g_game->width;
    a.p.y = (short)g_game->height;
    UpdateCellHeightRange(b.p, a.p);
    // REGION r4 end

    // REGION r5 begin
    BuildDerivedLayers();
    ClearBorderFeatures();
    unsigned int total2 = (unsigned int)(mapSettings->width * mapSettings->height) * 2;
    unsigned int half = total2 / 4;
    int* mapped = (int*)GameAllocIgnoreTag("MAPPED MEMORY", half);
    mapSettings->visibilityMask = (unsigned short*)mapped;
    memset(mapped, 0, half);
    int sx = mapSettings->screenTilesX + 0xc;
    int sy = mapSettings->screenTilesY + 0x20;
    mapSettings->sortRowCount = sy;
    mapSettings->sortRowWidth = sx;
    mapSettings->sortUnits = (int*)GameAllocIgnoreTag("SORT UNIT LIST", sy * sx * 4);
    mapSettings->sortIndices = (int*)GameAllocIgnoreTag("SORT INDICES", mapSettings->sortRowCount << 2);
    mapSettings->sortLineCount = (int*)GameAllocIgnoreTag("SORT LINE COUNT", mapSettings->sortRowCount << 1);
    StampFeatureMetal();
    g_game->count = 0;
    g_game->eyes = (Eye*)GameAllocIgnoreTag("EYEBALL MEMORY", 0x2d0);
    mapSettings->featureReproduceCursor = 0;
    g_game->loadProgress = 100;
    // REGION r5 end
}

// FUNCTION: 0x483dd0
void FreeMapResources()
{
    GameFreeThunk(g_game->eyes);
    FreeFeaturePool();
    if (g_game->radarFrame) {
        GameFreeThunk(g_game->radarFrame);
        g_game->radarFrame = 0;
    }
    GameFreeThunk(g_game->sortLineCount);
    GameFreeThunk(g_game->sortIndices);
    GameFreeThunk(g_game->sortUnits);
    GameFreeThunk(g_game->iconSet);
    GameFreeThunk(g_game->visibilityMask);
    GameFreeThunk(g_game->cells);
    GameFreeThunk(g_game->mapValues);
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

    operator delete(g_game->overflowBucket);
    g_game->overflowBucket = 0;
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
    GafFrame bmp;
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
    bmp.xOffset = 0;
    bmp.yOffset = 0;
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
    if (!(g_game->mapFlags.raw & 8)) {
        BuildFogTiles();
        g_game->mapFlags.raw |= 8;
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

