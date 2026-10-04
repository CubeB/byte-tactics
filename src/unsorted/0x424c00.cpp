// Decompiled by Claude Opus 5.5, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by fledge-alpha-free, finished by Claude Opus 5.5, finished by GPT-6, finished by Claude Opus 5.5. Names are provisional.
// Loads the map's features: the type-name table into a remap vector, then
// the "Normal", "Animating" and "3D" feature records. The save counterpart is
// 0x424890 (matched, same TU).
//
// #5634 Claude Opus 5.5: MATCH with the game types header and the two system
// headers of the DLLs TA imports that it does not already include
// (SHELL32 and IMAGEHLP): include/ta_types.h, <shlobj.h> and <imagehlp.h>.
// - The one byte left at 99.8% was the 3D loop's store, [spots + offset +
//   0x26] in the original. It is a symbol-id window (docs/c2-regalloc.md,
//   "Symbol ids"): the 3D loop's `c` has to be numbered past 65536 while the
//   Animating loop's `c` and `s` stay below it, which is g_game at 64976 to
//   64995. ta_types.h alone puts g_game at 62385; <shlobj.h> and <imagehlp.h>
//   put it at 64979. The same three headers match 0x471de0 (file total
//   65433, window 65257 to 65554).
// - The file uses the header's types where they hold what the function
//   reads: Game, Feature, Class_004b4560 (the file object; FUN_004b4ba0 and
//   FUN_004b4c80 are its members there, so data/aliases.csv names them),
//   Class_004b4800, Class_004b4bf0, Class_004b4c10, Class_004c2ea0,
//   FeatureName_00424c00, Normal_00424890 and Rot16. It keeps its own views
//   of the cell (the header's Cell has two bytes at +0xa), the feature spot
//   (the header's FeatureSpot has a pointer at +0x4 and a bitfield at +0x2e)
//   and the Animating record (the header's Anim_00424890 has no byte view of
//   the animation bits).
// - The 3D record is plain data. The header's Model_00424c00 holds a Vec3,
//   whose declared default constructor adds an /Ob2 call site; that leaves
//   the second resize's erase out of line (89.6%, 94.9% with an inline empty
//   Vec3()).
// - The static's $S suffix is its symbol id (64984 here), so
//   data/aliases.csv names DAT_00511fb4$S64984 too; 0x4224b0, 0x422ea0,
//   0x424050 and 0x4223e0 use the same static.
//
// Earlier findings that still hold:
// - DAT_00511fb4 is a file-scope static in this TU (as in 0x4223e0), and the
//   vectors are the real <vector> with plain resize() calls. No /Gi: the TU's
//   other functions only match without it.
// - The /Ob2 budget (c2prio --inline) decides the second resize's erase: the
//   original inlines it (copy and _Destroy out of line), which needs the
//   function's IL size at 1055 or more with the ternary FeatureIndex (IL 38,
//   free). The `got` and `type` locals add the IL; plain loops without them
//   keep erase out of line (89.6%). Above about 1150 FUN_004223e0's ~vector
//   inlines its _Destroy too (63.7%).
// - Both loops' spot address orders follow bit 14 of g_game's id below the
//   wrap; no declaration count without the wrap gives the original's mix of
//   one store each way (#5201, #5348).
#include <windows.h>
#include <shlobj.h>
#include <imagehlp.h>
#include "ta_types.h"
#include <string.h>

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
static FeatureList* DAT_00511fb4;

void __stdcall FUN_004222e0();
unsigned short __stdcall FUN_004224b0(char* name);
void __stdcall FUN_00422ea0();
void __stdcall FUN_004233a0(int x, int y, int flag);
void __stdcall FUN_00423550(int x, int y, int flag);
Cell_00424c00* __stdcall FUN_00481550(int x, int y);
void* __stdcall FUN_00423c50(Cell_00424c00* cell, unsigned short feature, void* pos, void* rot, unsigned char owner);

// FUN_00422e40, inlined
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
    return i != 0xffff ? i : FUN_004224b0(name);
}

// FUN_004223e0, inlined
static inline void FreeFeatureList()
{
    for (Class_004c2ea0** p = DAT_00511fb4->begin(); p < DAT_00511fb4->end(); p++)
        delete *p;
    delete DAT_00511fb4;
    DAT_00511fb4 = 0;
}

// FUNCTION: 0x424c00
void __stdcall FUN_00424c00(Class_004b4560* file)
{
    int j;
    file->FUN_004b4560("Features");
    std::vector<unsigned short> remap;
    FUN_004222e0();
    if (file->FUN_004b4ba0("Feature Type Names")) {
        int count = ((Class_004b4bf0*)file)->FUN_004b4bf0() / sizeof(FeatureName_00424c00);
        remap.resize(count);
        std::vector<FeatureName_00424c00> names(count);
        file->FUN_004b4c80(names.begin(), count * (int)sizeof(FeatureName_00424c00));
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
    FUN_00422ea0();
    FreeFeatureList();

    int k, n;
    n = ((Class_004b4800*)file)->FUN_004b4800("Number of Normal Features", 0);
    file->FUN_004b4ba0("Normal Features");
    for (k = 0; k < n; k++) {
        Normal_00424890 rec;
        ((Class_004b4c10*)file)->FUN_004b4c10(k * sizeof(Normal_00424890));
        int got = file->FUN_004b4c80(&rec, sizeof(Normal_00424890));
        if (got >= sizeof(Normal_00424890)) {
            Cell_00424c00* c = FUN_00481550(rec.x, rec.y);
            unsigned short type = remap[rec.feature];
            FUN_00423c50(c, type, 0, 0, 10);
            c->spot = rec.spot;
        }
    }

    n = ((Class_004b4800*)file)->FUN_004b4800("Number of Animating Features", 0);
    file->FUN_004b4ba0("Animating Features");
    for (k = 0; k < n; k++) {
        Anim_00424c00 rec;
        ((Class_004b4c10*)file)->FUN_004b4c10(k * sizeof(Anim_00424c00));
        int got = file->FUN_004b4c80(&rec, sizeof(Anim_00424c00));
        if (got >= sizeof(Anim_00424c00)) {
            Cell_00424c00* c = FUN_00481550(rec.x, rec.y);
            unsigned short type = remap[rec.feature];
            FUN_00423c50(c, type, 0, 0, 10);
            switch (rec.anim) {
            case 0:
                FUN_004233a0(rec.x, rec.y, 0);
                break;
            case 1:
                FUN_00423550(rec.x, rec.y, 0);
                break;
            case 2:
                FUN_00423550(rec.x, rec.y, 1);
                break;
            }
            Spot_00424c00* s = (Spot_00424c00*)&g_game->spots[c->spot];

            s->damage = rec.damage;
            s->frame = rec.frame;
            s->animBits = rec.bits & 0xf0;
        }
    }

    n = ((Class_004b4800*)file)->FUN_004b4800("Number of 3D Features", 0);
    file->FUN_004b4ba0("3D Features");
    for (k = 0; k < n; k++) {
        Model3D_00424c00 rec;
        ((Class_004b4c10*)file)->FUN_004b4c10(k * sizeof(Model3D_00424c00));
        int got = file->FUN_004b4c80(&rec, sizeof(Model3D_00424c00));
        if (got >= sizeof(Model3D_00424c00)) {
            Cell_00424c00* c = FUN_00481550(rec.x, rec.y);
            unsigned short type = remap[rec.feature];
            FUN_00423c50(c, type, &rec.pos, &rec.rot, 10);
            g_game->spots[c->spot].damage = rec.damage;
        }
    }
}
