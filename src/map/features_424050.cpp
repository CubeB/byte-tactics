// Decompiled by Claude Opus 5.5. Names are provisional.
#include <windows.h>
// Needed for the width * height operand order in the scan reset.
#include <ddraw.h>
#include <stdlib.h>

struct Point16_00424050 {
    short x;
    short z;
};

struct Fixed_00424050 {
    unsigned short frac;
    short whole;
};

union Coord_00424050 {
    int value;
    Fixed_00424050 f;
};

struct Vec3_00424050 {
    int x, y, z;

    Vec3_00424050() {}
    Vec3_00424050(int ax, int ay, int az) : x(ax), y(ay), z(az) {}
    Vec3_00424050& operator+=(const Vec3_00424050& o)
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

struct SmokePos_00424050 {
    Coord_00424050 x, y, z;
};

// A GAF frame header: size and origin of the picture.
struct Frame_00424050 {
    unsigned short width;              // +0x0
    unsigned short height;             // +0x2
    unsigned short originX;            // +0x4
    unsigned short originY;            // +0x6
};

struct Anim_00424050 {
    unsigned short index;              // +0x0
    unsigned short value;              // +0x2
    unsigned char kind;                // +0x4
    char unknown_5[3];
    void* src;                         // +0x8
};

#pragma pack(push, 1)
struct Cell_00424050 {
    short unknown_0;                   // +0x0
    char unknown_2[6];
    unsigned short feature;            // +0x8
    unsigned short spot;               // +0xa
    unsigned char flags;               // +0xc
};

struct Feature_00424050 {
    char unknown_0[0x94];
    Point16_00424050 footprint;        // +0x94
    char unknown_98[0xcc - 0x98];
    Anim_00424050 anim;                // +0xcc
    Anim_00424050 anim2;               // +0xd8
    char unknown_e4[0xf6 - 0xe4];
    unsigned short burnt;              // +0xf6
    char unknown_f8[0xfc - 0xf8];
    unsigned char seedChance;          // +0xfc
    unsigned char seedSpread;          // +0xfd
    unsigned short flag0 : 1;          // +0xfe
    unsigned short animated : 1;
    unsigned short bits2 : 14;
};

struct Spot_00424050 {
    short next;                        // +0x0
    short prev;                        // +0x2
    union {
        struct {
            Anim_00424050 anim;        // +0x4
            Anim_00424050 anim2;       // +0x10
        };
        struct {
            void* state;               // +0x4
            Vec3_00424050 pos;         // +0x8
            Vec3_00424050 vel;         // +0x14
        };
    };
    char unknown_20[0x28 - 0x20];
    Point16_00424050 cell;             // +0x28
    unsigned short feature;            // +0x2c
    unsigned char timer;               // +0x2e
    unsigned char flags;               // +0x2f
};

struct Pool_00424050 {
    Spot_00424050* entries;            // +0x0
    char unknown_4[4];
    int usedHead;                      // +0x8
    int restHead;                      // +0xc
    int freeHead;                      // +0x10
};

struct Game {
    char unknown_0[0x1420b];
    Pool_00424050 pool;                // +0x1420b
    char unknown_1421f[0x14233 - 0x1421f];
    int width;                         // +0x14233
    int height;                        // +0x14237
    char unknown_1423b[0x14253 - 0x1423b];
    int featureCount;                  // +0x14253
    int scanIndex;                     // +0x14257
    char unknown_1425b[0x14263 - 0x1425b];
    int gravity;                       // +0x14263
    char unknown_14267[0x1426f - 0x14267];
    Feature_00424050* features;        // +0x1426f
    char unknown_14273[0x1427f - 0x14273];
    unsigned char seaLevel;            // +0x1427f
    char unknown_14280[0x14287 - 0x14280];
    Cell_00424050* cells;              // +0x14287
    char unknown_1428b[0x38a47 - 0x1428b];
    unsigned int ticks;                // +0x38a47
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall StepGafSequence(Anim_00424050* anim);
int __stdcall RandomInt(int range);
Cell_00424050* __stdcall GetMapCell(int x, int y);
void* __stdcall PlaceFeature(Cell_00424050* cell, unsigned short feature, void* pos, void* rot,
                             unsigned char owner);
void __stdcall MoveFeatureSpot(int index, int* head);
int __stdcall GetCellMeanHeight(Vec3_00424050* pos);
int __stdcall GetGroundHeight(Vec3_00424050* pos);
Frame_00424050* __stdcall GetGafSequenceFrame(Anim_00424050* anim);
void __stdcall EmitWhiteSmoke(SmokePos_00424050* pos, short index);
int __stdcall RemoveFeature(Cell_00424050* cell, int flag);
void __stdcall ReplaceFeatureWithDead(int x, int z, int flag);
void __stdcall SpreadFire(Feature_00424050* f, Point16_00424050* cell);

// Inlined copy of GetFootprintCentre: the 16.16 world position of the centre of a
// feature footprint whose corner is at map cell `cell`.
static inline SmokePos_00424050 FootprintCentre_00421eb0(Point16_00424050* cell, Feature_00424050* def)
{
    Point16_00424050 f = def->footprint;
    Point16_00424050 c = *cell;
    SmokePos_00424050 p;
    p.x.value = (f.x + c.x * 2) << 19;
    p.z.value = (f.z + c.z * 2) << 19;
    p.y.value = GetGroundHeight((Vec3_00424050*)&p) << 16;
    return p;
}

// Inlined copy of ReplaceFeatureWithBurnt: replaces a burnt-out feature with its remains.
static inline void BurnOut_00423bf0(Spot_00424050* spot)
{
    Cell_00424050* target = GetMapCell(spot->cell.x, spot->cell.z);
    if (target != 0) {
        unsigned short id = g_game->features[spot->feature].burnt;
        RemoveFeature(target, 0);
        if (id != 0xffff) {
            PlaceFeature(target, id, 0, 0, 10);
        }
    }
}

static inline int Rand_00424050(unsigned short n)
{
    return (int)((__int64)rand() * n / 0x8000);
}

static inline SmokePos_00424050 SmokeAt_00424050(Spot_00424050* spot, Feature_00424050* f)
{
    SmokePos_00424050 p = FootprintCentre_00421eb0(&spot->cell, f);
    Frame_00424050* frame = GetGafSequenceFrame(&spot->anim);
    unsigned short w = frame->width;
    // The narrowed (unsigned short)(w >> 2) keeps `r - originX` before the add.
    p.x.f.whole += Rand_00424050(w >> 1) - frame->originX + (unsigned short)(w >> 2);
    unsigned short h = frame->height;
    // The second term is a shift of a narrowed value: a linear form gets factored.
    p.y.f.whole += (frame->originY - Rand_00424050(h >> 1)) * 2 - ((unsigned short)(h >> 2) << 1);
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
    Feature_00424050* types = g_game->features;
    for (int k = 0; k < g_game->featureCount; k++) {
        if (types[k].animated) {
            StepGafSequence(&types[k].anim);
            StepGafSequence(&types[k].anim2);
        }
    }
    if (--g_game->scanIndex < 0) {
        g_game->scanIndex = g_game->width * g_game->height - 1;
    } else {
        Cell_00424050* c = &g_game->cells[g_game->scanIndex];
        if (c->feature < 0xfffb && !(c->flags & 1)) {
            Feature_00424050* f = &g_game->features[c->feature];
            if (RandomInt(100) < f->seedChance) {
                int x = g_game->scanIndex % g_game->width;
                int z = g_game->scanIndex / g_game->height;
                x += RandomInt(f->seedSpread) - f->seedSpread / 2;
                z += RandomInt(f->seedSpread) - f->seedSpread / 2;
                Cell_00424050* t = GetMapCell(x, z);
                if (t && c->unknown_0 == 0 && t->feature == 0xffff)
                    PlaceFeature(t, c->feature, 0, 0, 10);
            }
        }
    }
    int smoke = g_game->ticks % 3 == 0;
    int i = g_game->pool.usedHead;
    while (i != -1) {
        Spot_00424050* spot = &g_game->pool.entries[i];
        Feature_00424050* f = &g_game->features[spot->feature];
        int next = spot->next;
        if (!f->flag0) {
            if (spot->vel.NonZero()) {
                spot->pos += spot->vel;
                int ground = GetCellMeanHeight(&spot->pos) << 16;
                if (spot->pos.y <= ground) {
                    spot->pos.y = ground;
                    spot->vel = Vec3_00424050(0, 0, 0);
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
                SmokePos_00424050 q = SmokeAt_00424050(spot, f);
                EmitWhiteSmoke(&q, 5);
            }
            StepGafSequence(&spot->anim);
            if (spot->flags & 4)
                StepGafSequence(&spot->anim2);
            if (spot->anim.src == 0) {
                BurnOut_00423bf0(spot);
            } else if (spot->timer > 0 && !(spot->flags & 8)) {
                if (--spot->timer == 0)
                    SpreadFire(f, &spot->cell);
            }
        } else {
            StepGafSequence(&spot->anim);
            if (spot->flags & 4)
                StepGafSequence(&spot->anim2);
            if (spot->anim.src == 0)
                ReplaceFeatureWithDead(spot->cell.x, spot->cell.z, 0);
        }
        i = next;
    }
}
