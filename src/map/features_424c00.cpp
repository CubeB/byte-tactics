// Decompiled by Claude Opus 5.5, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by fledge-alpha-free, finished by Claude Opus 5.5, finished by GPT-6, finished by Claude Opus 5.5. Names are provisional.
// Loads the map's features: the type-name table into a remap vector, then
// the "Normal", "Animating" and "3D" feature records. The save counterpart is
// 0x424890.
// These headers set g_game's symbol id: the 3D loop's `c` must be numbered
// past 65536, the Animating loop's `c` and `s` below it.
#include <windows.h>
#include <shlobj.h>
#include <imagehlp.h>
#include "ta_types.h"
#include <string.h>

// Own views of the cell, spot and Animating record: the header's differ.
struct Spot_00424c00 {
    char unknown_0[4];
    unsigned short frame;              // +0x4
    char unknown_6[0x26 - 6];
    unsigned short damage;             // +0x26
    char unknown_28[0x2e - 0x28];
    unsigned char animBits;            // +0x2e
    char unknown_2f;
};

struct Cell_00424c00 {
    char unknown_0[8];
    unsigned short feature;            // +0x8
    unsigned short spot;               // +0xa
    unsigned char flags;               // +0xc
};

struct Vec3_00424c00 {
    int x, y, z;
};

#pragma pack(push, 1)
struct Anim_00424c00 {
    unsigned short x;
    unsigned short y;
    unsigned short feature;
    unsigned short damage;
    unsigned char frame;
    union {
        unsigned char bits;
        struct {
            unsigned char anim : 4;
            unsigned char animHi : 4;
        };
    };
};

// Plain data: a Vec3 with a declared constructor changes the inlining.
struct Model3D_00424c00 {
    unsigned short x;
    unsigned short y;
    unsigned short feature;
    unsigned short damage;
    Vec3_00424c00 pos;
    Rot16 rot;
};
#pragma pack(pop)

extern Game* g_game;
typedef std::vector<Class_004c2ea0*> FeatureList;
// File-scope static, as in 0x4223e0.
static FeatureList* DAT_00511fb4;

void __stdcall LoadFeatureFileList();
unsigned short __stdcall LoadFeatureType(char* name);
void __stdcall ResolveFeatureLinks();
void __stdcall StartFeatureBurning(int x, int y, int flag);
void __stdcall KillFeature(int x, int y, int flag);
Cell_00424c00* __stdcall GetMapCell(int x, int y);
void* __stdcall PlaceFeature(Cell_00424c00* cell, unsigned short feature, void* pos, void* rot, unsigned char owner);

// FindOrLoadFeatureType, inlined
static inline unsigned short FindName(char* name)
{
    for (int i = 0; i < g_game->featureCount; i++) {
        if (_strcmpi(name, g_game->features[i].name) == 0) return (unsigned short)i;
    }
    return 0xffff;
}

static inline unsigned short FeatureIndex(char* name)
{
    unsigned short i = FindName(name);
    return i != 0xffff ? i : LoadFeatureType(name);
}

// FreeFeatureFileList, inlined
static inline void FreeFeatureList()
{
    for (Class_004c2ea0** p = DAT_00511fb4->begin(); p < DAT_00511fb4->end(); p++)
        delete *p;
    delete DAT_00511fb4;
    DAT_00511fb4 = 0;
}

// FUNCTION: 0x424c00
void __stdcall LoadFeatures(HapiBank* file)
{
    int j;
    file->OpenAccount("Features");
    std::vector<unsigned short> remap;
    LoadFeatureFileList();
    if (file->OpenNamedBox("Feature Type Names")) {
        int count = ((Class_004b4bf0*)file)->GetBoxSize() / sizeof(FeatureName_00424c00);
        remap.resize(count);
        std::vector<FeatureName_00424c00> names(count);
        file->ReadBox(names.begin(), count * (int)sizeof(FeatureName_00424c00));
        for (int i = 0; i < count; i++) {
            if (i < g_game->featureCount && _strcmpi(names[i].name, g_game->features[i].name) == 0) {
                remap[i] = i;
                continue;
            }
            for (j = 0; j < g_game->featureCount; j++) {
                if (_strcmpi(names[i].name, g_game->features[j].name) == 0) {
                    remap[i] = j;
                    goto next;
                }
            }
            remap[i] = FeatureIndex(names[i].name);
        next:;
        }
    } else {
        remap.resize(g_game->featureCount);
        int i = 0;
        for (; i < g_game->featureCount; i++) remap[i] = i;
    }
    ResolveFeatureLinks();
    FreeFeatureList();

    int k, n;
    n = ((Class_004b4800*)file)->GetIntegerItem("Number of Normal Features", 0);
    file->OpenNamedBox("Normal Features");
    for (k = 0; k < n; k++) {
        Normal_00424890 rec;
        ((Class_004b4c10*)file)->SeekBox(k * sizeof(Normal_00424890));
        // The `got` and `type` locals add IL size that the inlining depends on.
        int got = file->ReadBox(&rec, sizeof(Normal_00424890));
        if (got >= sizeof(Normal_00424890)) {
            Cell_00424c00* c = GetMapCell(rec.x, rec.y);
            unsigned short type = remap[rec.feature];
            PlaceFeature(c, type, 0, 0, 10);
            c->spot = rec.spot;
        }
    }

    n = ((Class_004b4800*)file)->GetIntegerItem("Number of Animating Features", 0);
    file->OpenNamedBox("Animating Features");
    for (k = 0; k < n; k++) {
        Anim_00424c00 rec;
        ((Class_004b4c10*)file)->SeekBox(k * sizeof(Anim_00424c00));
        int got = file->ReadBox(&rec, sizeof(Anim_00424c00));
        if (got >= sizeof(Anim_00424c00)) {
            Cell_00424c00* c = GetMapCell(rec.x, rec.y);
            unsigned short type = remap[rec.feature];
            PlaceFeature(c, type, 0, 0, 10);
            switch (rec.anim) {
            case 0:
                StartFeatureBurning(rec.x, rec.y, 0);
                break;
            case 1:
                KillFeature(rec.x, rec.y, 0);
                break;
            case 2:
                KillFeature(rec.x, rec.y, 1);
                break;
            }
            Spot_00424c00* s = (Spot_00424c00*)&g_game->spots[c->spot];

            s->damage = rec.damage;
            s->frame = rec.frame;
            s->animBits = rec.bits & 0xf0;
        }
    }

    n = ((Class_004b4800*)file)->GetIntegerItem("Number of 3D Features", 0);
    file->OpenNamedBox("3D Features");
    for (k = 0; k < n; k++) {
        Model3D_00424c00 rec;
        ((Class_004b4c10*)file)->SeekBox(k * sizeof(Model3D_00424c00));
        int got = file->ReadBox(&rec, sizeof(Model3D_00424c00));
        if (got >= sizeof(Model3D_00424c00)) {
            Cell_00424c00* c = GetMapCell(rec.x, rec.y);
            unsigned short type = remap[rec.feature];
            PlaceFeature(c, type, &rec.pos, &rec.rot, 10);
            g_game->spots[c->spot].damage = rec.damage;
        }
    }
}
