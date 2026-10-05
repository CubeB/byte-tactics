// Decompiled by Claude Opus 5.5, finished by deepseek-v4.1-flash, edited by deepseek-v4.1. Names are provisional.
// Saves the map's features: the feature type names, then one record per
// occupied map cell ("3D Features", "Normal Features" or "Animating
// Features") and the three counts. The load counterpart is 0x424c00.
//
// MATCH. Two source details drive the whole loop layout. The feature test
// `if (c->feature < 0xfffb)` must be written before the
// `Feature_00424890* f = &g_game->features[c->feature]` definition (MSVC
// then emits cmp/jae before the address arithmetic), and the cell pointer
// must be declared before `end` and the counters
// (`Cell_00424890* c = g_game->cells;` above `end`, with
// `for (; c < end; c++)`). That declaration order is what keeps the cell
// pointer in esi and the feature pointer in edi. A loop-scope c with the
// same test-first body scores 91.9% (esi/edi swapped), test-last scores
// 99.2% (right registers, cmp after the arithmetic).
// <windows.h> is needed (83% without it).
#include <windows.h>
#include <string.h>
#include <vector>

class Class_004b4560 {
public:
    void FUN_004b4560(char* name);
};

class Class_004b4630 {
public:
    int FUN_004b4630(const char* name, int value);
};

class Class_004b4ba0 {
public:
    int FUN_004b4ba0(char* name);
};

class Class_004b4bf0 {
public:
    int FUN_004b4bf0();
};

class Class_004b4c10 {
public:
    void FUN_004b4c10(int pos);
};

class Class_004b4cf0 {
public:
    int FUN_004b4cf0(void* src, int len);
};

struct Vec3_00424890 {
    int x, y, z;
};

#pragma pack(push, 1)
struct Rot16_00424890 {
    short x, y, z;
};

struct FeatureName_00424890 {
    char name[0x80];
};

struct Feature_00424890 {
    char name[0xb4];                   // +0x0
    int anim0;                         // +0xb4
    char unknown_b8[4];
    int anim1;                         // +0xbc
    char unknown_c0[4];
    int anim2;                         // +0xc4
    char unknown_c8[0xfe - 0xc8];
    unsigned char flags;               // +0xfe
    char unknown_ff[0x100 - 0xff];
};

struct Spot_00424890 {
    char unknown_0[4];
    unsigned char frame;               // +0x4
    char unknown_5[8 - 5];
    union {
        Vec3_00424890 pos;             // +0x8 (3D features)
        struct {
            int unknown_8;
            int anim;                  // +0xc (animating features)
        };
    };
    char unknown_14[0x20 - 0x14];
    Rot16_00424890 rot;                // +0x20
    unsigned short damage;             // +0x26
    char unknown_28[0x2e - 0x28];
    unsigned char animLo : 4;          // +0x2e
    unsigned char animHi : 4;
    char unknown_2f;
};

struct Cell_00424890 {
    char unknown_0[8];
    unsigned short feature;            // +0x8
    unsigned short spot;               // +0xa
    unsigned char flags;               // +0xc
};

struct Game {
    char unknown_0[0x1420b];
    Spot_00424890* spots;              // +0x1420b
    char unknown_1420f[0x14233 - 0x1420f];
    int width;                         // +0x14233
    int height;                        // +0x14237
    char unknown_1423b[0x14253 - 0x1423b];
    int featureCount;                  // +0x14253
    char unknown_14257[0x1426f - 0x14257];
    Feature_00424890* features;        // +0x1426f
    char unknown_14273[0x14287 - 0x14273];
    Cell_00424890* cells;              // +0x14287
};

struct Normal_00424890 {
    short x;
    short y;
    unsigned short feature;
    unsigned short spot;
};

struct Anim_00424890 {
    short x;
    short y;
    unsigned short feature;
    unsigned short damage;
    unsigned char frame;
    unsigned char anim : 4;
    unsigned char animHi : 4;
};

struct Model_00424890 {
    short x;
    short y;
    unsigned short feature;
    unsigned short damage;
    Vec3_00424890 pos;
    Rot16_00424890 rot;
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x424890
void __stdcall FUN_00424890(Class_004b4ba0* file)
{
    ((Class_004b4560*)file)->FUN_004b4560("Features");
    std::vector<FeatureName_00424890> names(g_game->featureCount);
    FeatureName_00424890* dst = names.begin();
    Feature_00424890* src = g_game->features;
    for (int i = 0; i < g_game->featureCount; i++, dst++, src++)
        strncpy(dst->name, src->name, 0x80);
    file->FUN_004b4ba0("Feature Type Names");
    ((Class_004b4cf0*)file)->FUN_004b4cf0(names.begin(), g_game->featureCount * sizeof(FeatureName_00424890));

    Cell_00424890* c = g_game->cells;
    Cell_00424890* end = g_game->cells + g_game->width * g_game->height;
    int normalCount = 0;
    int modelCount = 0;
    int animCount = 0;
    int x = 0;
    int y = 0;
    for (; c < end; c++) {
        if (c->feature < 0xfffb) {
            Feature_00424890* f = &g_game->features[c->feature];
            if (!(f->flags & 1)) {
                Model_00424890 rec;
                Spot_00424890* s = &g_game->spots[c->spot];
                rec.x = x;
                rec.y = y;
                rec.feature = c->feature;
                rec.damage = s->damage;
                rec.pos = s->pos;
                rec.rot = s->rot;
                file->FUN_004b4ba0("3D Features");
                ((Class_004b4c10*)file)->FUN_004b4c10(((Class_004b4bf0*)file)->FUN_004b4bf0());
                ((Class_004b4cf0*)file)->FUN_004b4cf0(&rec, 0x1a);
                modelCount++;
            } else if (c->flags & 1) {
                Anim_00424890 rec;
                Spot_00424890* s = &g_game->spots[c->spot];
                rec.x = x;
                rec.y = y;
                rec.feature = c->feature;
                rec.damage = s->damage;
                rec.frame = s->frame;
                rec.animHi = s->animHi;
                if (s->anim == f->anim0)
                    rec.anim = 0;
                else if (s->anim == f->anim1)
                    rec.anim = 1;
                else if (s->anim == f->anim2)
                    rec.anim = 2;
                else
                    goto next;
                file->FUN_004b4ba0("Animating Features");
                ((Class_004b4c10*)file)->FUN_004b4c10(((Class_004b4bf0*)file)->FUN_004b4bf0());
                ((Class_004b4cf0*)file)->FUN_004b4cf0(&rec, 10);
                animCount++;
            } else {
                Normal_00424890 rec;
                rec.x = x;
                rec.y = y;
                rec.feature = c->feature;
                rec.spot = c->spot;
                file->FUN_004b4ba0("Normal Features");
                ((Class_004b4c10*)file)->FUN_004b4c10(((Class_004b4bf0*)file)->FUN_004b4bf0());
                ((Class_004b4cf0*)file)->FUN_004b4cf0(&rec, 8);
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
    ((Class_004b4630*)file)->FUN_004b4630("Number of Normal Features", normalCount);
    ((Class_004b4630*)file)->FUN_004b4630("Number of 3D Features", modelCount);
    ((Class_004b4630*)file)->FUN_004b4630("Number of Animating Features", animCount);
}
