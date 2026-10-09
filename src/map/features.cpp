// Decompiled by Opus, DeepSeek V4.1 Flash, Claude Opus 5.5, deepseek-v4.1-flash, deepseek-v4.1, GPT-6, fledge-alpha-free, Haiku, Space Bunny Free, LongCat 2.5 Preview Free, GPT-6.1-sol, space-bunny-free and mimo-v2.6-pro. Names are provisional.
// The features module's translation unit (0x421e60 to 0x4256a0): the map's
// feature types and the pool of feature spots, placing, burning, killing and
// reclaiming them, the per-tick update, saving and loading, and the
// out-of-line std::vector members they call.
//
// 0x421f20, 0x4224b0, 0x422ea0 and 0x424c00 stay in files of their own:
// their register plans and operand orders follow their old files' symbol ids
// (docs/c2-regalloc.md), and 0x424c00 also matches only with include/ta_types.h
// at its exact size.
// FLAGS: /Gi
#include <windows.h>
#include <ddraw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

#include "../util/tdf.h"
#include "../util/hapi_bank.h"

#pragma pack(push, 1)

struct Gaf;                            // a GAF file's header

struct AnimSrc {                       // a GAF sequence header
    unsigned short count;
    unsigned char kind;
};

struct Anim {                          // an animation reference (0xc bytes)
    unsigned short index;
    unsigned short value;
    unsigned char kind;
    char unknown_5[3];
    AnimSrc* src;
};

struct AnimPair {
    AnimSrc* anim;
    AnimSrc* shadow;
};

struct Vec3 {
    int x, y, z;

    Vec3() {}
    Vec3(int ax, int ay, int az) : x(ax), y(ay), z(az) {}
    Vec3& operator+=(const Vec3& o)
    {
        x += o.x;
        y += o.y;
        z += o.z;
        return *this;
    }
    int NonZero()
    {
        return x || z || y;
    }
};

struct Point16 {
    short x;
    short z;
};

struct Rot16 {
    short x, y, z;
};

struct Fixed {
    unsigned short frac;
    short whole;
};

union Coord {
    int value;
    Fixed f;
};

struct SmokePos {
    Coord x, y, z;
};

struct GafFrame {
    unsigned short width;
    unsigned short height;
    unsigned short originX;
    unsigned short originY;
};

struct FeatureDamagePacket {
    unsigned char type;
    unsigned char sub;
    short x;
    short z;
};

struct FeatureUnit {
    char unknown_0[0x92];
    int field_92;                      // +0x92
    char unknown_96[0xa6 - 0x96];
    unsigned short field_a6;           // +0xa6
    char unknown_a8[0x110 - 0xa8];
    int flags_110;                     // +0x110
    int flags_114;                     // +0x114
};

struct AnimEntry {
    short next;                        // +0x0
    short prev;                        // +0x2
    char unknown_4[0x2c];
};

struct NameEntry {
    char unknown_0[4];
    char name[0x80];                   // +0x4
};

struct AnimManager {
    char unknown_0[0x10];
    AnimEntry* anim;                   // +0x10
    FeatureUnit* featureUnit;          // +0x14
    int activeHead;                    // +0x18
    int idleHead;                      // +0x1c
    int freeHead;                      // +0x20
    char unknown_24[0x58 - 0x24];
    int nameCount;                     // +0x58
    char unknown_5c[0x74 - 0x5c];
    char (*names)[0x100];              // +0x74
};

struct List {                          // 0x421f20's feature name list
    char unknown_0[0x1c];
    int count;                         // +0x1c
    NameEntry* entries;                // +0x20
};

struct Feature;                        // the 0x100-byte feature definition

struct Spot {                          // a feature spot (0x30 bytes)
    short prev;                        // +0x0
    short next;                        // +0x2
    union {
        struct {
            Anim anim;                 // +0x4
            Anim shadow;               // +0x10
        };
        struct {
            void* state;               // +0x4
            Vec3 pos;                  // +0x8
            Vec3 vel;                  // +0x14
        };
        struct {
            unsigned char frame;       // +0x4
            char unknown_5[3];
            union {
                struct {
                    Vec3 pos2;         // +0x8
                };
                struct {
                    int unknown_8;     // +0x8
                    int animIndex;     // +0xc
                };
            };
        };
    };
    Rot16 rot;                         // +0x20
    unsigned short damage;             // +0x26
    union {
        struct {
            short x;                   // +0x28
            short z;                   // +0x2a
        };
        Point16 cell;                  // +0x28
    };
    unsigned short feature;            // +0x2c
    union {
        unsigned char burnTime;        // +0x2e
        unsigned char timer;
        unsigned char animBits;
        struct {
            unsigned char animLo : 4;
            unsigned char animHi : 4;
        };
    };
    union {
        unsigned char flags;           // +0x2f
        struct {
            unsigned char used : 1;
            unsigned char reclaimed : 1;
            unsigned char hasShadow : 1;
            unsigned char noSend : 1;
            unsigned char bit4 : 1;
            unsigned char bits5 : 3;
        };
    };
};

struct Pool {                          // the feature spot pool
    Spot* entries;                     // +0x0
    void* field_4;                     // +0x4
    int usedHead;                      // +0x8
    int restHead;                      // +0xc
    int freeHead;                      // +0x10
};

struct Feature {                       // the feature definition (0x100 bytes)
    char name[0x80];                   // +0x00
    char description[0x14];            // +0x80
    union {
        Point16 footprint;             // +0x94
        struct {
            short footprintx;          // +0x94
            short footprintz;          // +0x96
        };
    };
    union {
        void* object;                  // +0x98
        char filename[0x10];           // +0x98
    };
    Gaf* anims;                        // +0xa8
    AnimSrc* seq;                      // +0xac
    AnimSrc* seqshad;                  // +0xb0
    union {
        struct {
            AnimSrc* seqburn;          // +0xb4
            AnimSrc* seqburnshad;      // +0xb8
            AnimSrc* seqdie;           // +0xbc
            AnimSrc* seqdieshad;       // +0xc0
            AnimSrc* seqreclamate;     // +0xc4
            AnimSrc* seqreclamateshad; // +0xc8
        };
        struct {
            AnimPair burn;             // +0xb4
            AnimPair death[2];         // +0xbc: destroyed, reclaimed
        };
        struct {
            int anim0;                 // +0xb4
            int field_b8;
            int anim1;                 // +0xbc
            int field_c0;
            int anim2;                 // +0xc4
            int field_c8;
        };
    };
    Anim ref;                          // +0xcc
    Anim refshad;                      // +0xd8
    char* burnweapon;                  // +0xe4
    union {
        short sparktime;               // +0xe8
        unsigned short burnTime;
    };
    unsigned short damage;             // +0xea
    float energy;                      // +0xec
    union {
        float metal;                   // +0xf0
        float value;
    };
    unsigned short dead;               // +0xf4
    unsigned short burnt;              // +0xf6
    unsigned short reclamate;          // +0xf8
    char height;                       // +0xfa
    union {
        char spreadchance;             // +0xfb
        unsigned char spreadChance;
    };
    union {
        char reproduce;                // +0xfc
        unsigned char seedChance;
    };
    union {
        char reproducearea;            // +0xfd
        unsigned char seedSpread;
    };
    union {
        unsigned short flags16;        // +0xfe
        unsigned char flags;
        struct {
            unsigned short noobject : 1;
            unsigned short animating : 1;
            unsigned short animtrans : 1;
            unsigned short shadtrans : 1;
            unsigned short flamable : 1;
            unsigned short geothermal : 1;
            unsigned short blocking : 1;
            unsigned short reclaimable : 1;
            unsigned short autoreclaimable : 1;
            unsigned short indestructible : 1;
            unsigned short nodisplayinfo : 1;
            unsigned short nodrawundergray : 1;
            unsigned short bits12 : 4;
        };
    };
};

union SpotField {                      // the cell's spot or footprint offset
    struct {
        unsigned char offsetY;         // +0xa
        unsigned char offsetX;         // +0xb
    };
    unsigned short spot;               // +0xa
};

struct Cell {                          // 13 bytes per cell
    union {
        char unknown_0[8];
        struct {
            short unit;                // +0x0
            char unknown_2[6];
        };
        struct {
            char unknown_0b[7];
            char field7;               // +0x7
        };
    };
    unsigned short feature;            // +0x8
    union {
        struct {
            unsigned char offsetY;     // +0xa
            unsigned char offsetX;     // +0xb
        };
        unsigned short spot;           // +0xa
        SpotField sf;                  // +0xa
    };
    union {
        unsigned char flags;           // +0xc
        struct {
            unsigned char flag0 : 1;
            unsigned char bits1 : 2;
            unsigned char owner : 4;
            unsigned char bit7 : 1;
        };
    };
};

struct PlayerData {
    char unknown_0[0x97];
    unsigned char flags;               // +0x97
};

struct Player {
    int active;                        // +0x0
    char unknown_4[0x23];
    PlayerData* data;                  // +0x27
    char unknown_2b[0x48];
    unsigned char type;                // +0x73
    char unknown_74[0xd7];
};

struct PlayerInfo {
    char unknown_0[4];
    int id;                            // +0x4
};

struct Unit {
    char unknown_0[0x96];
    PlayerInfo* info;                  // +0x96
    char unknown_9a[0xbc - 0x9a];
    float energy;                      // +0xbc
    char unknown_c0[0xd4 - 0xc0];
    float metal;                       // +0xd4
    char unknown_d8[0xec - 0xd8];
    Player* player;                    // +0xec
};

struct Weapon {
    char unknown_0[0xd4];
    unsigned short damage;             // +0xd4
    char unknown_d6[0x10a - 0xd6];
    unsigned char kind;                // +0x10a
    char flag_10b;                     // +0x10b
};

struct Obj {
    char unknown_0[0x28];
    short x;                           // +0x28
    short y;                           // +0x2a
    unsigned short entry;              // +0x2c
};

struct MissionFeature {
    char name[0x80];
    int x;                             // +0x80
    int y;                             // +0x84
};

struct FeatureName {
    char name[0x80];
};

struct Normal {
    short x;
    short y;
    unsigned short feature;
    unsigned short spot;
};

struct AnimRecord {
    short x;
    short y;
    unsigned short feature;
    unsigned short damage;
    unsigned char frame;
    unsigned char anim : 4;
    unsigned char animHi : 4;
};

struct Model {
    short x;
    short y;
    unsigned short feature;
    unsigned short damage;
    Vec3 pos;
    Rot16 rot;
};

#include "../map/mission.h"

struct Game {
    char unknown_0[0x1b63];
    Player players[10];                // +0x1b63
    char unknown_2851[0x2a43 - 0x2851];
    unsigned char playerIndex;         // +0x2a43
    char unknown_2a44[0x141fb - 0x2a44];
    union {
        AnimManager anim;              // +0x141fb
        struct {
            char unknown_141fb[0x10];
            union {
                Pool pool;             // +0x1420b
                Spot* spots;           // +0x1420b
            };
            char unknown_1421f[0x14233 - 0x1421f];
            union {
                int width;             // +0x14233
                int mapWidth;
            };
            int height;                // +0x14237
            char unknown_1423b[0x14253 - 0x1423b];
            union {
                int featureCount;      // +0x14253
                int nameCount;
                int typeCount;
            };
            int scanIndex;             // +0x14257
            char unknown_1425b[0x14263 - 0x1425b];
            int gravity;               // +0x14263
            char unknown_14267[0x1426f - 0x14267];
            Feature* features;         // +0x1426f
        };
    };
    char unknown_14273[0x1427f - 0x14273];
    unsigned char seaLevel;            // +0x1427f
    char unknown_14280[0x14287 - 0x14280];
    Cell* cells;                       // +0x14287
    char unknown_1428b[0x1439b - 0x1428b];
    int unitDefs;                   // +0x1439b
    char unknown_1439f[0x37ecc - 0x1439f];
    int windX;                         // +0x37ecc
    int windY;                         // +0x37ed0
    int windZ;                         // +0x37ed4
    char unknown_37ed8[0x37eee - 0x37ed8];
    int difficulty;                    // +0x37eee
    char unknown_37ef2[0x37f2f - 0x37ef2];
    unsigned char flags_37f2f;         // +0x37f2f
    char unknown_37f30[0x38a47 - 0x37f30];
    unsigned int ticks;                // +0x38a47
    char unknown_38a4b[0x38d72 - 0x38a4b];
    unsigned char loadPercent;         // +0x38d72
    char unknown_38d73[0x391e9 - 0x38d73];
    Mission* net;                      // +0x391e9
};
#pragma pack(pop)

class Class_004c9390 {
public:
    char* data;                        // +0x0

    void ReleaseRef();
};

struct Elem {
    Class_004c9390 name;               // +0x0

    ~Elem() { name.ReleaseRef(); }
};

extern Game* g_game;
extern char DAT_005119b8[];

typedef std::vector<TdfFile*> FeatureList;
// File-scope static: 0x4223e0 needs it static (an extern global changes
// register use in its inlined ~vector), and 0x422460, 0x4224b0 and 0x422ea0
// walk the vector through begin() and end().
static FeatureList* s_featureTdfParsers;

void __cdecl GameFreeThunk(void* p);
void __stdcall FreeObjectState(void* obj);
AnimSrc* __stdcall FindGafEntry(void* list, const char* name);
void __stdcall InitGafSequence(Anim* ref, AnimSrc* src, int index);
int __stdcall LoadFeatureType(char* name);
int __stdcall GetGroundHeight(Vec3* pos);
Cell* __stdcall GetMapCell(int x, int y);
Spot* __stdcall PlaceFeature(Cell* cell, unsigned short feature, Vec3* pos, Rot16* rot,
                             unsigned char owner);
int __stdcall RemoveFeature(Cell* cell, int flag);
void __stdcall MoveFeatureSpot(int index, int* head);
int __stdcall RandomInt(int range);
void __stdcall PlaySoundAtByName(char* name, Vec3* pos, int param_3);
int GetLocalDpid();
int GetHostDpid();
int __stdcall BroadcastPacket(int player, void* data, int size);
int __stdcall SendPacketToPlayer(int from, int to, void* packet, int size);
void __stdcall StartFeatureBurning(int x, int z, int flag);
void __stdcall KillFeature(int x, int z, int flag);
void __stdcall ReplaceFeatureWithDead(int x, int y, int flag);
void __stdcall SpreadFire(Feature* f, Point16* cell);
int __stdcall StepGafSequence(Anim* anim);
int __stdcall GetCellMeanHeight(Vec3* pos);
GafFrame* __stdcall GetGafSequenceFrame(Anim* anim);
void __stdcall EmitWhiteSmoke(SmokePos* pos, short index);
void __stdcall ApplyAreaDamageAt(void* owner, Vec3* pos);
void* __stdcall CreateObjectState(void* obj);
void __stdcall EmitTimedSubParticles(Vec3* p, short index);
void __stdcall RefreshAllPassMaps(Point16 a, Point16 b);
void __stdcall FindFilesRecursive(char* dir, char* pattern, void* list, int flags, char recurse);

// Reading the field directly each time (no local) gives `or ax, 0xffff` for
// the 0xffff return; a local gives `mov eax, 0xffff`.
// FUNCTION: 0x421e60
unsigned short __stdcall GetCellFeature(Cell* cell)
{
    if (cell->feature >= 0xfffb) {
        if (cell->feature == 0xfffe)
            return (cell - (cell->offsetY * g_game->mapWidth + cell->offsetX))->feature;
        return 0xffff;
    }
    return cell->feature;
}

// FUNCTION: 0x421eb0
Vec3 __stdcall GetFootprintCentre(Point16* cell, Feature* def)
{
    Point16 f = def->footprint;
    Point16 c = *cell;
    Vec3 p;
    p.x = (f.x + c.x * 2) << 19;
    p.z = (f.z + c.z * 2) << 19;
    p.y = GetGroundHeight(&p) << 16;
    return p;
}

// FUNCTION: 0x422040
void StampFeatureMetal(void)
{
    Cell* c = g_game->cells;
    for (int i = 0; i < g_game->width * g_game->height; i++, c++) {
        if (c->feature < 0xfffb) {
            Feature* f = &g_game->features[c->feature];
            if (f->value != 0.0f && (f->flags16 & 0x200)) {
                int x0 = i % g_game->width;
                int y0 = i / g_game->width;
                for (int y = y0; y < y0 + f->footprintz; y++) {
                    for (int x = x0; x < x0 + f->footprintx; x++) {
                        Cell* cc = GetMapCell(x, y);
                        if (cc != 0)
                            cc->field7 = (char)f->value;
                    }
                }
            }
        }
    }
}

// FUNCTION: 0x422170
void FreeFeaturePool()
{
    int i;
    for (i = 0; i <= 1; i++) {
        int idx = i ? g_game->pool.usedHead : g_game->pool.restHead;
        while (idx != -1) {
            Spot* e = &g_game->pool.entries[idx];
            if (!(g_game->features[e->feature].flags & 1))
                FreeObjectState(e->state);
            idx = e->prev;
        }
    }

    GameFreeThunk(g_game->pool.field_4);
    GameFreeThunk(g_game->pool.entries);
    g_game->pool.field_4 = 0;
    g_game->pool.entries = 0;

    Feature* t = g_game->features;
    for (i = 0; i < g_game->featureCount; i++, t++) {
        if (t->flags & 1) {
            if (t->anims) {
                GameFreeThunk(t->anims);
                t->anims = 0;
            }
        } else {
            if (t->object) {
                GameFreeThunk(t->object);
                t->object = 0;
            }
        }
    }

    GameFreeThunk(g_game->features);
    g_game->features = 0;
    g_game->featureCount = 0;
}

// FUNCTION: 0x4222b0
void* __stdcall FindOptionalGafEntry(void* list, int unused, char* name)
{
    // Returning a value on both paths keeps them apart: with a void return the
    // early exit is folded into a jump to the shared epilogue.
    if (strlen(name) == 0) {
        return 0;
    }
    return FindGafEntry(list, name);
}

// Must not be __cdecl: the calling convention changes the loop entry order.
// FUNCTION: 0x4222e0
void __stdcall LoadFeatureFileList()
{
    s_featureTdfParsers = new std::vector<TdfFile*>;
    std::vector<Elem> list;
    FindFilesRecursive("features", "*.tdf", &list, -1, 1);
    for (std::vector<Elem>::iterator it = list.begin(); it < list.end(); it++) {
        TdfFile* obj = new TdfFile;
        if (obj->LoadFile(it->name.data)) {
            s_featureTdfParsers->push_back(obj);
        } else {
            delete obj;
        }
    }
}

// FUNCTION: 0x4223e0
void FreeFeatureFileList()
{
    for (TdfFile** p = s_featureTdfParsers->begin(); p < s_featureTdfParsers->end(); p++)
        delete *p;
    delete s_featureTdfParsers;
    s_featureTdfParsers = 0;
}

// FUNCTION: 0x422460
TdfFile* __stdcall FindFeatureFile(char* name)
{
    for (TdfFile** p = s_featureTdfParsers->begin(); p < s_featureTdfParsers->end(); p++) {
        (*p)->ResetCurrentRecord();
        if (((TdfFile*)*p)->SelectRecord(name))
            return *p;
    }
    return 0;
}

// FUNCTION: 0x422dd0
short __stdcall FindFeatureType(char* name)
{
    for (int i = 0; i < g_game->featureCount; i++) {
        if (_strcmpi(name, g_game->features[i].name) == 0) {
            return (short)i;
        }
    }
    return -1;
}

// FindFeatureType-like search, inlined
static inline unsigned short FindName(char* name)
{
    for (int i = 0; i < g_game->featureCount; i++) {
        if (_strcmpi(name, g_game->features[i].name) == 0) {
            return (unsigned short)i;
        }
    }
    return 0xffff;
}
// FUNCTION: 0x422e40
unsigned short __stdcall FindOrLoadFeatureType(char* name)
{
    unsigned short i = FindName(name);
    if (i == 0xffff)
        i = LoadFeatureType(name);
    return i;
}

static inline int FindFeature(char* name)
{
    for (int i = 0; i < g_game->featureCount; i++) {
        if (_strcmpi(name, g_game->features[i].name) == 0)
            return i;
    }
    return 0xffff;
}
// FUNCTION: 0x423160
void PlaceMissionFeatures(void)
{
    for (int i = 0; i < g_game->net->featureCount; i++) {
        MissionFeature* p = &g_game->net->features[i];
        if (p->name[0] == 0)
            continue;
        unsigned short id = (unsigned short)FindFeature(p->name);
        if (id == 0xffff) {
            id = LoadFeatureType(p->name);
            if (id == 0xffff)
                continue;
        }
        Feature* f = &g_game->features[id];
        // A cell local with one GetMapCell call per branch, not x/y locals and one call.
        Cell* cell;
        if (f->flags16 & 1)
            cell = GetMapCell(p->x, p->y);
        else
            cell = GetMapCell(p->x - f->footprint.x / 2, p->y - f->footprint.z / 2);
        PlaceFeature(cell, id, 0, 0, 10);
    }
}

// FUNCTION: 0x4232a0
int AllocFeatureSpot()
{
    Pool* p = &g_game->pool;
    int i = p->freeHead;
    if (i == -1) {
        return 0x800;
    }
    MoveFeatureSpot(i, &p->usedHead);
    p->entries[i].used = 0;
    return i;
}

// FUNCTION: 0x4232f0
void __stdcall MoveFeatureSpot(int index, int* head)
{
    Pool* p = &g_game->pool;
    Spot* e = &p->entries[index];
    if (e->next == -1) {
        if (p->usedHead == index)
            p->usedHead = e->prev;
        else if (p->restHead == index)
            p->restHead = e->prev;
        else if (p->freeHead == index)
            p->freeHead = e->prev;
    } else {
        p->entries[e->next].prev = e->prev;
    }
    if (e->prev != -1)
        p->entries[e->prev].next = e->next;
    short old = *head;
    e->next = -1;
    e->prev = old;
    *head = index;
    if (e->prev != -1)
        p->entries[e->prev].next = index;
}

static inline int AllocSpot()
{
    Pool* p = &g_game->pool;
    int i = p->freeHead;
    if (i == -1) {
        return 0x800;
    }
    MoveFeatureSpot(i, &p->usedHead);
    p->entries[i].used = 0;
    return i;
}
// FUNCTION: 0x4233a0
void __stdcall StartFeatureBurning(int x, int z, int flag)
{
    Cell* cell = GetMapCell(x, z);
    if (cell == 0)
        return;
    if (cell->feature >= 0xfffb)
        return;
    Feature* f = &g_game->features[cell->feature];
    if (f->seqburn == 0)
        return;
    if (cell->flags & 1)
        return;
    int i = AllocSpot();
    if (i >= 0x800)
        return;
    Spot* s = &g_game->pool.entries[i];
    s->feature = cell->feature;
    cell->spot = i;
    cell->flags |= 1;
    InitGafSequence(&s->anim, f->seqburn, 0);
    if (f->seqburnshad != 0) {
        InitGafSequence(&s->shadow, f->seqburnshad, 0);
        s->hasShadow = 1;
    } else {
        s->hasShadow = 0;
    }
    s->used = 1;
    s->x = x;
    s->z = z;
    s->burnTime = RandomInt(f->burnTime >> 1) + (f->burnTime >> 1);
    s->noSend = flag;
    Vec3 pos;
    pos.x = x << 20;
    pos.z = z << 20;
    PlaySoundAtByName("treeburn", &pos, 0);
    if (flag == 0) {
        FeatureDamagePacket packet;
        packet.type = 0xf;
        packet.sub = 0xfe;
        packet.x = x;
        packet.z = z;
        BroadcastPacket(GetLocalDpid(), &packet, 6);
    }
}

// FUNCTION: 0x423550
void __stdcall KillFeature(int x, int z, int flag)
{
    Cell* cell = GetMapCell(x, z);
    if (cell->feature == 0xfffe) {
        x -= cell->offsetX;
        z -= cell->offsetY;
        cell = GetMapCell(x, z);
    }
    if (cell->feature >= 0xfffb)
        return;
    Feature* f = &g_game->features[cell->feature];
    AnimPair pair;
    pair.anim = 0;
    pair.shadow = 0;
    if (f->flags & 1) {
        if (flag != 0)
            pair = f->death[1];
        else
            pair = f->death[0];
    }
    // If/else, not an early return: keeps the ReplaceFeatureWithDead call last.
    if (pair.anim != 0) {
        if (cell->flags & 1)
            return;
        int i = AllocSpot();
        if (i >= 0x800)
            return;
        Spot* s = &g_game->pool.entries[i];
        s->feature = cell->feature;
        s->bit4 = flag != 0;
        cell->spot = i;
        cell->flags |= 1;
        InitGafSequence(&s->anim, pair.anim, 0);
        if (pair.shadow != 0) {
            InitGafSequence(&s->shadow, pair.shadow, 0);
            s->hasShadow = 1;
        } else {
            s->hasShadow = 0;
        }
        s->used = 0;
        s->reclaimed = flag;
        s->x = x;
        s->z = z;
    } else {
        ReplaceFeatureWithDead(x, z, flag);
    }
}

// FUNCTION: 0x423710
void __stdcall ReplaceFeatureWithDead(int x, int y, int flag)
{
    Cell* cell = GetMapCell(x, y);
    if (cell == 0)
        return;
    if (cell->feature >= 0xfffb)
        return;
    Feature* f = &g_game->features[cell->feature];
    unsigned short v;
    if (flag != 0)
        v = f->reclamate;
    else
        v = f->dead;
    if (cell->flags & 1) {
        Spot* spot = g_game->spots + cell->spot;
        if (spot->flags & 2)
            v = f->reclamate;
        RemoveFeature(cell, 0);
        PlaceFeature(cell, v, &spot->pos, &spot->rot, 10);
        return;
    }
    RemoveFeature(cell, 0);
    PlaceFeature(cell, v, 0, 0, 10);
}

static inline Feature* GetFeature(Cell* cell)
{
    if (cell == 0)
        return 0;
    if (cell->feature < 0xfffb) {
        if (cell->feature >= g_game->featureCount)
            return 0;
        return &g_game->features[cell->feature];
    }
    if (cell->feature != 0xfffe)
        return 0;
    // Full expression twice, no locals or `cell -=`; ends with the feature return.
    if ((cell - (cell->offsetY * g_game->width + cell->offsetX))->feature >= 0xfffb)
        return 0;
    return &g_game->features[(cell - (cell->offsetY * g_game->width + cell->offsetX))->feature];
}

// Takes the field by reference and the amount as a double; the metal add stays plain code.
static inline void AddScaled(Unit* unit, float& res, double amount)
{
    if (unit->player->active && unit->player->type == 2) {
        switch (g_game->difficulty) {
        case 1:
            res += amount * 0.7;
            break;
        case 0:
            res += amount * 0.5;
            break;
        default:
            res += amount;
            break;
        }
    } else {
        res += amount;
    }
}
// FUNCTION: 0x4237d0
int __stdcall ReclaimFeature(Unit* unit, Vec3* pos)
{
    int x = pos->x / 0x100000;
    int z = pos->z / 0x100000;
    Cell* cell = GetMapCell(x, z);
    Feature* f = GetFeature(GetMapCell(x, z));
    if (f == 0)
        return 0;
    if ((cell->flags & 1) && (f->flags & 1))
        return 0;
    AddScaled(unit, unit->energy, f->energy);
    float metal = f->metal;
    if (unit->player->active && unit->player->type == 2) {
        switch (g_game->difficulty) {
        case 1:
            unit->metal += metal * 0.7;
            break;
        case 0:
            unit->metal += metal * 0.5;
            break;
        default:
            unit->metal += metal;
            break;
        }
    } else {
        unit->metal += metal;
    }
    KillFeature(x, z, 1);
    if (g_game->net->GetGameType() == 3) {
        FeatureDamagePacket packet;
        packet.type = 0xf;
        packet.sub = 0xff;
        packet.x = x;
        packet.z = z;
        BroadcastPacket(unit->info->id, &packet, 6);
    }
    return 1;
}

// Inlined copy of GetFootprintCentre: the 16.16 world position of the centre of a
// feature footprint whose corner is at map cell `cell`.
static inline Vec3 FootprintCentre(Point16* cell, Feature* def)
{
    Point16 f = def->footprint;
    Point16 c = *cell;
    Vec3 p;
    p.x = (f.x + c.x * 2) << 19;
    p.z = (f.z + c.z * 2) << 19;
    p.y = GetGroundHeight(&p) << 16;
    return p;
}
// A burning feature at `cell` sets fire (StartFeatureBurning) to flammable features
// within 3 cells, then to up to five cells downwind (the wind vector at
// +0x37ecc/+0x37ed4 times 2.0 in 16.16), and leaves its burnt remains
// (ApplyAreaDamageAt) at its centre.
// FUNCTION: 0x4239c0
void __stdcall SpreadFire(Feature* f, Point16* cell)
{
    for (int z = cell->z - 3; z <= cell->z + 3; z++) {
        for (int x = cell->x - 3; x <= cell->x + 3; x++) {
            if (x != cell->x || z != cell->z) {
                Cell* c = GetMapCell(x, z);
                if (c && c->feature < 0xfffb && !(c->flags & 1)) {
                    Feature* g = &g_game->features[c->feature];
                    // Its own `if`, not part of the `&&` chain.
                    if (g->flamable) {
                        if (RandomInt(100) < g->spreadChance)
                            StartFeatureBurning(x, z, 0);
                    }
                }
            }
        }
    }
    Coord px;
    Coord pz;
    px.value = cell->x << 16;
    // Computed from its own read of cell->z, before lastZ copies it.
    pz.value = cell->z << 16;
    int lastX = cell->x;
    int lastZ = cell->z;
    for (int i = 5; i; i--) {
        px.value += (int)(((__int64)g_game->windX * 0x20000) >> 16);
        pz.value += (int)(((__int64)g_game->windZ * 0x20000) >> 16);
        int x = px.f.whole;
        int z = pz.f.whole;
        if (x != lastX || z != lastZ) {
            Cell* c = GetMapCell(x, z);
            if (c && c->feature < 0xfffb && !(c->flags & 1)) {
                Feature* g = &g_game->features[c->feature];
                if (g->flamable) {
                    if (RandomInt(100) < g->spreadChance)
                        StartFeatureBurning(x, z, 0);
                }
            }
            lastX = x;
            lastZ = z;
        }
    }
    if (f->burnweapon) {
        Vec3 pos = FootprintCentre(cell, f);
        ApplyAreaDamageAt(f->burnweapon, &pos);
    }
}

// FUNCTION: 0x423bf0
void __stdcall ReplaceFeatureWithBurnt(Obj* obj)
{
    Cell* target = GetMapCell(obj->x, obj->y);
    if (target != 0) {
        unsigned short id = g_game->features[obj->entry].burnt;
        RemoveFeature(target, 0);
        if (id != 0xffff) {
            PlaceFeature(target, id, 0, 0, 10);
        }
    }
}

// Places feature `feature` on map cell `cell` (its footprint's corner): fails
// when the footprint leaves the map or a feature already there cannot be
// removed (RemoveFeature), takes a spot for features that keep state, marks the
// other footprint cells 0xfffe with their offsets from the corner, and returns
// the spot (0 when none).
// FUNCTION: 0x423c50
Spot* __stdcall PlaceFeature(Cell* cell, unsigned short feature,
                                      Vec3* pos, Rot16* rot,
                                      unsigned char owner)
{
    Spot* spot = 0;
    if (feature == 0xffff)
        return 0;
    if (feature == 0xfffc) {
        cell->feature = feature;
        return 0;
    }
    // Computed before the cell index.
    Feature* f = &g_game->features[feature];
    int n = cell - g_game->cells;
    int w = g_game->width;
    Point16 at;
    at.x = n % w;
    at.z = n / w;
    if (at.x + f->footprint.x > w)
        return 0;
    if (at.z + f->footprint.z > g_game->height)
        return 0;
    // Both footprint loops walk an explicit `c++` pointer, not `row[x]`.
    for (int z = 0; z < f->footprint.z; z++) {
        Cell* c = &cell[z * g_game->width];
        for (int x = 0; x < f->footprint.x; x++, c++) {
            if (c->feature != 0xffff && !RemoveFeature(c, 0))
                return 0;
        }
    }
    if (!f->noobject) {
        int i = AllocSpot();
        if (i >= 0x800)
            return 0;
        spot = &g_game->pool.entries[i];
        spot->feature = feature;
        spot->damage = 0;
        spot->cell = at;
        if (pos)
            spot->pos = *pos;
        else
            spot->pos = FootprintCentre(&at, f);
        if (rot)
            spot->rot = *rot;
        else
            memset(&spot->rot, 0, sizeof(spot->rot));
        spot->state = CreateObjectState(f->object);
        cell->feature = feature;
        cell->spot = i;
        cell->flag0 = 1;
    } else {
        cell->feature = feature;
        cell->spot = 0;
        cell->flag0 = 0;
    }
    cell->owner = owner;
    for (int z2 = 0; z2 < f->footprint.z; z2++) {
        Cell* c = &cell[z2 * g_game->width];
        for (int x = 0; x < f->footprint.x; x++, c++) {
            if (x != 0 || z2 != 0) {
                c->feature = 0xfffe;
                c->offsetX = x;
                c->offsetY = z2;
                c->flag0 = 0;
            }
        }
    }
    if (f->geothermal) {
        if (pos) {
            EmitTimedSubParticles(pos, 4);
        } else {
            Vec3 p = FootprintCentre(&at, f);
            EmitTimedSubParticles(&p, 4);
        }
    }
    int index = cell - g_game->cells;
    Point16 p2;
    p2.x = index % g_game->width;
    p2.z = index / g_game->width;
    RefreshAllPassMaps(p2, f->footprint);
    return spot;
}

// Inlined copy of GetFootprintCentre: the 16.16 world position of the centre of a
// feature footprint whose corner is at map cell `cell`.
static inline SmokePos FootprintCentreSmoke(Point16* cell, Feature* def)
{
    Point16 f = def->footprint;
    Point16 c = *cell;
    SmokePos p;
    p.x.value = (f.x + c.x * 2) << 19;
    p.z.value = (f.z + c.z * 2) << 19;
    p.y.value = GetGroundHeight((Vec3*)&p) << 16;
    return p;
}

// Inlined copy of ReplaceFeatureWithBurnt: replaces a burnt-out feature with its remains.
static inline void BurnOut(Spot* spot)
{
    Cell* target = GetMapCell(spot->cell.x, spot->cell.z);
    if (target != 0) {
        unsigned short id = g_game->features[spot->feature].burnt;
        RemoveFeature(target, 0);
        if (id != 0xffff) {
            PlaceFeature(target, id, 0, 0, 10);
        }
    }
}

static inline int Rand(unsigned short n)
{
    return (int)((__int64)rand() * n / 0x8000);
}

static inline SmokePos SmokeAt(Spot* spot, Feature* f)
{
    SmokePos p = FootprintCentreSmoke(&spot->cell, f);
    GafFrame* frame = GetGafSequenceFrame(&spot->anim);
    unsigned short w = frame->width;
    // The narrowed (unsigned short)(w >> 2) keeps `r - originX` before the add.
    p.x.f.whole += Rand(w >> 1) - frame->originX + (unsigned short)(w >> 2);
    unsigned short h = frame->height;
    // The second term is a shift of a narrowed value: a linear form gets factored.
    p.y.f.whole += (frame->originY - Rand(h >> 1)) * 2 - ((unsigned short)(h >> 2) << 1);
    return p;
}
// Per-tick update of map features: advances the animations of animated
// feature types, lets one scanned map cell per tick seed a copy of its feature
// nearby (chance +0xfc, spread +0xfd), then walks the used spot list: falling
// debris moves under gravity until it lands (or sinks below sea level), burning
// features puff smoke every third tick, burn down and spread fire
// (SpreadFire), and finished animations are replaced by their remains.
// Suspected original bug: the seed position's row is scanIndex / height
// (+0x14237), not scanIndex / width, so on non-square maps the seed lands in
// the wrong row.
// FUNCTION: 0x424050
void __stdcall UpdateFeatures()
{
    Feature* types = g_game->features;
    for (int k = 0; k < g_game->featureCount; k++) {
        if (types[k].animating) {
            StepGafSequence(&types[k].ref);
            StepGafSequence(&types[k].refshad);
        }
    }
    if (--g_game->scanIndex < 0) {
        g_game->scanIndex = g_game->width * g_game->height - 1;
    } else {
        Cell* c = &g_game->cells[g_game->scanIndex];
        if (c->feature < 0xfffb && !(c->flags & 1)) {
            Feature* f = &g_game->features[c->feature];
            if (RandomInt(100) < f->seedChance) {
                int x = g_game->scanIndex % g_game->width;
                int z = g_game->scanIndex / g_game->height;
                x += RandomInt(f->seedSpread) - f->seedSpread / 2;
                z += RandomInt(f->seedSpread) - f->seedSpread / 2;
                Cell* t = GetMapCell(x, z);
                if (t && c->unit == 0 && t->feature == 0xffff)
                    PlaceFeature(t, c->feature, 0, 0, 10);
            }
        }
    }
    int smoke = g_game->ticks % 3 == 0;
    int i = g_game->pool.usedHead;
    while (i != -1) {
        Spot* spot = &g_game->pool.entries[i];
        Feature* f = &g_game->features[spot->feature];
        int next = spot->prev;
        if (!f->noobject) {
            if (spot->vel.NonZero()) {
                spot->pos += spot->vel;
                int ground = GetCellMeanHeight(&spot->pos) << 16;
                if (spot->pos.y <= ground) {
                    spot->pos.y = ground;
                    spot->vel = Vec3(0, 0, 0);
                } else if (spot->pos.y < g_game->seaLevel << 16) {
                    spot->vel.x = 0;
                    spot->vel.y = -0x2ccc;
                    spot->vel.z = 0;
                } else {
                    spot->vel.y -= g_game->gravity;
                }
            } else {
                MoveFeatureSpot(i, &g_game->pool.restHead);
            }
        } else if (spot->flags & 1) {
            if (smoke) {
                SmokePos q = SmokeAt(spot, f);
                EmitWhiteSmoke(&q, 5);
            }
            StepGafSequence(&spot->anim);
            if (spot->flags & 4)
                StepGafSequence(&spot->shadow);
            if (spot->anim.src == 0) {
                BurnOut(spot);
            } else if (spot->timer > 0 && !(spot->flags & 8)) {
                if (--spot->timer == 0)
                    SpreadFire(f, &spot->cell);
            }
        } else {
            StepGafSequence(&spot->anim);
            if (spot->flags & 4)
                StepGafSequence(&spot->shadow);
            if (spot->anim.src == 0)
                ReplaceFeatureWithDead(spot->cell.x, spot->cell.z, 0);
        }
        i = next;
    }
}

// FUNCTION: 0x4244b0
void __stdcall DamageFeature(Cell* cell, int x, int z, Weapon* weapon)
{
    if (!(g_game->flags_37f2f & 8))
        return;
    if (cell->feature >= 0xfffb)
        return;
    Feature* f = &g_game->features[cell->feature];
    if (f->indestructible)
        return;
    FeatureDamagePacket packet;
    int send;
    if (g_game->net->GetGameType() == 3) {
        if (!(g_game->players[g_game->playerIndex].data->flags & 1)) {
            packet.type = 0xf;
            packet.sub = weapon->kind;
            packet.x = x;
            packet.z = z;
            SendPacketToPlayer(GetLocalDpid(), GetHostDpid(), &packet, 6);
            return;
        }
        send = 1;
        packet.sub = 0xfc;
    } else {
        send = 0;
    }
    if (f->flamable && weapon->flag_10b && !(cell->flags & 1)) {
        StartFeatureBurning(x, z, 0);
    } else if (cell->flags & 1) {
        if (!f->noobject) {
            Spot* s = &g_game->spots[cell->spot];
            if (s->x != x)
                return;
            if (s->z != z)
                return;
            s->damage += weapon->damage;
            if (s->damage >= f->damage) {
                KillFeature(s->x, s->z, 0);
                packet.sub = 0xfd;
            }
        }
    } else {
        int d = weapon->damage + cell->spot;
        if (d >= f->damage) {
            KillFeature(x, z, 0);
            packet.sub = 0xfd;
        } else {
            cell->spot = d;
        }
    }
    if (send && packet.sub > 0xfc) {
        packet.type = 0xf;
        packet.x = x;
        packet.z = z;
        BroadcastPacket(GetLocalDpid(), &packet, 6);
    }
}

// Must stay an inline helper, feature then flags: by hand in the loop the store order changes.
static inline void ClearCell(Cell* c)
{
    c->feature = 0xffff;
    c->flags &= 0xfe;
}
// FUNCTION: 0x4246b0
int __stdcall RemoveFeature(Cell* cell, int flag)
{
    if (cell->feature == 0xfffe)
        cell -= cell->sf.offsetY * g_game->width + cell->sf.offsetX;
    if (cell->feature >= 0xfffb)
        return 0;
    Feature* f = &g_game->features[cell->feature];
    if (flag == 0 && (f->flags16 & 0x200))
        return 0;
    if (cell->flags & 1) {
        Spot* spot = &g_game->spots[cell->sf.spot];
        if (!(f->flags16 & 1)) {
            FreeObjectState(spot->state);
            spot->state = 0;
        }
        MoveFeatureSpot(cell->sf.spot, &g_game->pool.freeHead);
    }
    ClearCell(cell);
    for (int y = 0; y < f->footprint.z; y++) {
        Cell* row = &cell[y * g_game->width];
        for (int x = 0; x < f->footprint.x; x++) {
            if (row[x].feature == 0xfffe)
                ClearCell(&row[x]);
        }
    }
    int index = cell - g_game->cells;
    Point16 p;
    p.x = index % g_game->width;
    p.z = index / g_game->width;
    RefreshAllPassMaps(p, f->footprint);
    return 1;
}

// FUNCTION: 0x424840
void RemoveAllFeatures(void)
{
    int n = g_game->width * g_game->height;
    for (int i = 0; i < n; i++) {
        Cell* c = &g_game->cells[i];
        if (c->feature < 0xfffb || c->feature == 0xfffe) {
            RemoveFeature(c, 1);
        }
    }
}

// FUNCTION: 0x424890
void __stdcall SaveFeatures(HapiBank* file)
{
    file->OpenAccount("Features");
    std::vector<FeatureName> names(g_game->featureCount);
    FeatureName* dst = names.begin();
    Feature* src = g_game->features;
    for (int i = 0; i < g_game->featureCount; i++, dst++, src++)
        strncpy(dst->name, src->name, 0x80);
    file->OpenNamedBox("Feature Type Names");
    file->WriteBox(names.begin(), g_game->featureCount * sizeof(FeatureName));

    // Declared before `end` and the counters, and walked by `for (; c < end; c++)`.
    Cell* c = g_game->cells;
    Cell* end = g_game->cells + g_game->width * g_game->height;
    int normalCount = 0;
    int modelCount = 0;
    int animCount = 0;
    int x = 0;
    int y = 0;
    for (; c < end; c++) {
        // The test comes before the definition of `f`.
        if (c->feature < 0xfffb) {
            Feature* f = &g_game->features[c->feature];
            if (!(f->flags & 1)) {
                Model rec;
                Spot* s = &g_game->spots[c->spot];
                rec.x = x;
                rec.y = y;
                rec.feature = c->feature;
                rec.damage = s->damage;
                rec.pos = s->pos2;
                rec.rot = s->rot;
                file->OpenNamedBox("3D Features");
                file->SeekBox(file->GetBoxSize());
                file->WriteBox(&rec, 0x1a);
                modelCount++;
            } else if (c->flags & 1) {
                AnimRecord rec;
                Spot* s = &g_game->spots[c->spot];
                rec.x = x;
                rec.y = y;
                rec.feature = c->feature;
                rec.damage = s->damage;
                rec.frame = s->frame;
                rec.animHi = s->animHi;
                if (s->animIndex == f->anim0)
                    rec.anim = 0;
                else if (s->animIndex == f->anim1)
                    rec.anim = 1;
                else if (s->animIndex == f->anim2)
                    rec.anim = 2;
                else
                    goto next;
                file->OpenNamedBox("Animating Features");
                file->SeekBox(file->GetBoxSize());
                file->WriteBox(&rec, 10);
                animCount++;
            } else {
                Normal rec;
                rec.x = x;
                rec.y = y;
                rec.feature = c->feature;
                rec.spot = c->spot;
                file->OpenNamedBox("Normal Features");
                file->SeekBox(file->GetBoxSize());
                file->WriteBox(&rec, 8);
                normalCount++;
            }
        }
    next:
        x++;
        if (x >= g_game->width) {
            x = 0;
            y++;
        }
    }
    file->SetIntegerItem("Number of Normal Features", normalCount);
    file->SetIntegerItem("Number of 3D Features", modelCount);
    file->SetIntegerItem("Number of Animating Features", animCount);
}

typedef std::vector<TdfFile*> Vec_004251e0;
typedef void (Vec_004251e0::*DestroyFn_004251e0)(Vec_004251e0::iterator, Vec_004251e0::iterator);

// _Destroy is protected: the derived class takes its address to emit it out of line.
struct Access_004251e0 : Vec_004251e0 {
    static DestroyFn_004251e0 fn;
};

// FUNCTION: 0x4251e0 ?_Destroy@?$vector@PAVTdfFile@@V?$allocator@PAVTdfFile@@@std@@@std@@IAEXPAPAVTdfFile@@0@Z
DestroyFn_004251e0 Access_004251e0::fn = &Access_004251e0::_Destroy;

typedef std::vector<unsigned short> Vec_004251f0;
// Taking the member's address is what emits it out of line.
typedef Vec_004251f0::size_type (Vec_004251f0::*SizeFn_004251f0)() const;

// FUNCTION: 0x4251f0 ?size@?$vector@GV?$allocator@G@std@@@std@@QBEIXZ
SizeFn_004251f0 g_size_004251f0 = &Vec_004251f0::size;

typedef std::vector<unsigned short> Vec_00425210;
typedef void (Vec_00425210::*InsertFn_00425210)(
    Vec_00425210::iterator, Vec_00425210::size_type, const unsigned short&);

// A growth step on this vector type, standing in for the original TU's own;
// the insert's bytes need the TU to instantiate reserve (or resize, or the
// copy constructor).
void __stdcall Grow_00425210(Vec_00425210* v, int extra)
{
    v->reserve(extra + v->size());
}

// FUNCTION: 0x425210 ?insert@?$vector@GV?$allocator@G@std@@@std@@QAEXPAGIABG@Z
InsertFn_00425210 g_insert_00425210 = &Vec_00425210::insert;

typedef std::vector<unsigned short> Vec_00425430;
// The element is a scalar, not a 2-byte struct: the resize value temporary in
// 0x424c00 is stored as a dword.
typedef Vec_00425430::iterator (Vec_00425430::*EraseFn_00425430)(
    Vec_00425430::iterator, Vec_00425430::iterator);

// FUNCTION: 0x425430 ?erase@?$vector@GV?$allocator@G@std@@@std@@QAEPAGPAG0@Z
EraseFn_00425430 g_erase_00425430 = &Vec_00425430::erase;

typedef std::vector<unsigned short> Vec_00425470;
typedef void (Vec_00425470::*DestroyFn_00425470)(Vec_00425470::iterator, Vec_00425470::iterator);

// _Destroy is protected: the derived class takes its address to emit it out of line.
struct Access_00425470 : Vec_00425470 {
    static DestroyFn_00425470 fn;
};

// FUNCTION: 0x425470 ?_Destroy@?$vector@GV?$allocator@G@std@@@std@@IAEXPAG0@Z
DestroyFn_00425470 Access_00425470::fn = &Access_00425470::_Destroy;

typedef std::vector<TdfFile*> Vec_00425480;
typedef void (Vec_00425480::*InsertFn_00425480)(
    Vec_00425480::iterator, Vec_00425480::size_type, TdfFile* const&);

// An assignment on this vector type, standing in for the original TU's own;
// the insert's bytes need the TU to instantiate operator=.
void __stdcall Assign_00425480(Vec_00425480* a, const Vec_00425480* b)
{
    *a = *b;
}

// FUNCTION: 0x425480 ?insert@?$vector@PAVTdfFile@@V?$allocator@PAVTdfFile@@@std@@@std@@QAEXPAPAVTdfFile@@IABQAV3@@Z
InsertFn_00425480 g_insert_00425480 = &Vec_00425480::insert;

namespace std {
// FUNCTION: 0x4256a0 ?copy@std@@YGPAGPAG00@Z
// Explicit __stdcall specialization: the original instantiated it under /Gz.
template <>
unsigned short* __stdcall copy(unsigned short* first, unsigned short* last, unsigned short* dest)
{
    for (; first != last; ++dest, ++first)
        *dest = *first;
    return dest;
}
}
