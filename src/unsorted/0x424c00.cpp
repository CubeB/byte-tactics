// Decompiled by Claude Opus 5.5, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by fledge-alpha-free, finished by Claude Opus 5.5, finished by GPT-6. Names are provisional.
// #5535 Codex recheck: 99.8%; the paired loops still require opposite SIB orders.
// #5429 Codex retry: re-confirmed 99.8%; both loops still require opposite
// SIB orders, unavailable with the real symbol and header state.
// #5441 Codex retry: re-confirmed 99.8%; the same opposite SIB orders remain.
// #5403 Codex retry: 99.8% remains best. The two loops still need opposite
// SIB base orders, while real symbol and header states select the same order.
// #5421 Codex retry: re-confirmed 99.8%; no natural source or header state
// produces the two required SIB orders in their respective loops.
// #5348 Claude Opus 5.5 (99.8% kept; scratch files in build/scratch/0x424c00/):
//  - The 3D store needs the original's large symbol prefix: 32286 to 32305
//    dummy externs before g_game give byte-identical code (only the static's
//    $S suffix differs). That puts the 3D loop's `c` (id 33252 now) past
//    65536 while the Anim loop's `c` and `s` (33228, 33237) stay below it,
//    which is the "one large common header" in docs/c2-regalloc.md. A probe
//    file (build/scratch/0x424c00/t/) shows the rule is not g_game's bit 14
//    alone. With g_game's id G in [0, 16384) or [32768, 49152), the order is
//    spots + offset until the index base `c` passes about G + 0x4000
//    (rounded to a multiple of 0x400), then offset + spots until c wraps
//    past 65536. With bit 14 of G set, it is offset + spots from c = G up to
//    the wrap. Our two loops' `c` ids are 25 apart, so only a wrap between
//    them gives the original's mix of one store each way.
//  - Block-scope `extern g_game` (the #5325 lever) cannot do it in real
//    source. Next to the file-scope declaration it is the same symbol (no
//    change). Without one, every block-scope extern is its own symbol: a
//    copy in FindName (FUN_00422e40) reloads g_game where the original
//    shares the j loop's load (89.9%). A function-scope one has bit 14 clear
//    (id about 32886), so both loops come out spots + offset (97.5%, as with
//    the file-scope declaration moved after FreeFeatureList, id 32868). The
//    only split found: the Anim spot through an inline SpotOf(c) holding its
//    own extern, defined before a late file-scope g_game. That gives both
//    orders right, but the extra /Ob2 call site leaves the second resize's
//    erase and FreeFeatureList's ~vector out of line (60.4%). Fixing that
//    needs the IL size near 1152 (it is 1049 there).
//  - A local `Game_00424c00* game = g_game;` in the 3D block (seven
//    placements) gives the right SIB order but becomes a register candidate
//    in ecx where the original loads g_game into edx (98.3%).
// #5330 Codex retry: re-confirmed 99.8%. An explicit local spots pointer
// leaves the same final SIB order; the existing best is preserved.
// #5300 Codex retry: re-confirmed 99.8%. A local spots pointer produced the
// same final SIB order; the current best is preserved.
// #5258 Codex retry: re-confirmed 99.8%. An explicit local spots pointer
// produced the same final SIB order, so the original remains unchanged.
// Loads the map's features: the type-name table into a remap vector, then
// the "Normal", "Animating" and "3D" feature records. The save counterpart is
// 0x424890 (matched, same TU, includes <windows.h>).
//
// Symbol ids, read with `c2prio.py --symbols g_game` (docs/c2-regalloc.md,
// "Symbol ids"; Claude Opus 5.5): the bit 14 rule below is exact. g_game's id
// is 32690 here; 32767 keeps 99.8% and 32768 gives 97.5% (both stores
// `spots + offset`), 49151 97.5% and 49152 99.8% again. The file total does
// not matter: up to 31000 declarations after g_game, which move it and this
// function's id across 49152, change nothing. So no header set can give the
// original's mix of one store each way; the two loops must differ in source.
// Every plausible header set (docs) keeps g_game's bit 14 set, as now.
//
// #5201 Claude Opus 5.5: 83.6% to 99.8% (1496 bytes, one SIB byte left).
//  - DAT_00511fb4 is a file-scope static in this TU (as in 0x4223e0), and the
//    vectors are the real <vector> with plain resize() calls.
//  - No /Gi: 0x4224b0, 0x422ea0 and 0x424050 use the same static, so they are
//    this TU, and they only match without /Gi. <windows.h> as in 0x424890.
//  - The /Ob2 budget (c2prio --inline) decides the second resize's erase: the
//    original inlines it (copy and _Destroy out of line), which needs the
//    function's IL size at 1077 or more with an if/return FeatureIndex (IL 44,
//    costs budget), or 1055 or more with the ternary one (IL 38, free). This
//    spelling is 1057: the `got` and `type` locals add the IL, plain loops
//    without them are 1027 and keep erase out of line (89.6%). Above about
//    1150 FUN_004223e0's ~vector inlines its _Destroy too (63.7%).
//  - What still differs: the 3D loop's store is [offset + spots + 0x26] where
//    the original has [spots + offset + 0x26]. It is a numbering tie, not a
//    spelling: twelve spellings of that store (locals, casts, char*
//    arithmetic, a pool struct, an inline getter, another Spot or Cell type)
//    are byte-identical. Without <windows.h> the 3D store comes out right but
//    the Animating loop's spot address flips to spots + offset; /Gi behaves
//    like <windows.h>. Extern-int dummies placed before g_game flip both
//    loops together (at 78 with <windows.h>, the same 78 from any position
//    before g_game and in a function cut down to the 3D loop alone), placed
//    after g_game they do nothing (up to 1200); under /Gi no count moves it
//    (0 to 1000). No count gives the original's mix (swept 0 to 400 in
//    steps of 6 past the flip), so the original's two loops differ in
//    something not yet found. Writing the Anim loop's first store through
//    the array and the rest through `s` (CSE-shared address) does move the
//    3D base register, but breaks the Anim block (92.3%).
//    The rule, measured with dummies in front of g_game: both orders follow
//    bit 14 of g_game's symbol id taken mod 65536 (offset + spots when it is
//    set: dummies 0..77 and 16462..32767-ish and 49230..65613 with
//    <windows.h>; the same at +65536). A base that is a local instead,
//    `Game_00424c00* game = g_game; game->spots[c->spot].damage = ...` in
//    the 3D loop, gives the original's spots + offset order there, but the
//    local is a register candidate and takes ecx where the original loads
//    g_game into edx (98.3%); an inline method or helper taking the Game
//    pointer does the same. Two identical stores in the 3D block make the
//    first one exactly the original's (98.1%, the second store is extra).
//  - The real FUN_00422e40 (0x422e40.cpp's == spelling) inlined here gives
//    86%; its if/return spelling matches 0x422e40 too but costs 44 of budget.
//    Defining FUN_004223e0 by its real name instead of FreeFeatureList gives
//    the same code.
// Earlier history: #5167 logged the inline budget and the real <vector>
// spelling (51.5% then, before the static); #5134 found /Gi (wrong, see
// above) and the `got` locals; earlier passes moved it from 69.1%.
#include <windows.h>
#include <string.h>
#include <vector>

class Class_004b4560 {
public:
    void FUN_004b4560(char* name);
};

class Class_004b4800 {
public:
    int FUN_004b4800(char* name, int def);
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

class Class_004b4c80 {
public:
    int FUN_004b4c80(void* dst, int len);
};

class Class_004c2ea0 {
public:
    void* data;                        // +0x0
    int field_4;                       // +0x4
    int field_8;                       // +0x8

    Class_004c2ea0();
    ~Class_004c2ea0();
};

struct Vec3_00424c00 {
    int x, y, z;
};

#pragma pack(push, 1)
struct Rot16_00424c00 {
    short x, y, z;
};

struct FeatureName_00424c00 {
    char name[0x80];
};

struct Feature_00424c00 {
    char name[0x100];
};

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

struct Game_00424c00 {
    char unknown_0[0x1420b];
    Spot_00424c00* spots;              // +0x1420b
    char unknown_1420f[0x14253 - 0x1420f];
    int featureCount;                  // +0x14253
    char unknown_14257[0x1426f - 0x14257];
    Feature_00424c00* features;        // +0x1426f
};

struct Normal_00424c00 {
    unsigned short x;
    unsigned short y;
    unsigned short feature;
    unsigned short spot;
};

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

struct Model_00424c00 {
    unsigned short x;
    unsigned short y;
    unsigned short feature;
    unsigned short damage;
    Vec3_00424c00 pos;
    Rot16_00424c00 rot;
};
#pragma pack(pop)

extern Game_00424c00* g_game;
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
void __stdcall FUN_00424c00(Class_004b4ba0* file)
{
    int j;
    ((Class_004b4560*)file)->FUN_004b4560("Features");
    std::vector<unsigned short> remap;
    FUN_004222e0();
    if (file->FUN_004b4ba0("Feature Type Names")) {
        int count = ((Class_004b4bf0*)file)->FUN_004b4bf0() / sizeof(FeatureName_00424c00);
        remap.resize(count);
        std::vector<FeatureName_00424c00> names(count);
        ((Class_004b4c80*)file)->FUN_004b4c80(names.begin(), count * (int)sizeof(FeatureName_00424c00));
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
        Normal_00424c00 rec;
        ((Class_004b4c10*)file)->FUN_004b4c10(k * sizeof(Normal_00424c00));
        int got = ((Class_004b4c80*)file)->FUN_004b4c80(&rec, sizeof(Normal_00424c00));
        if (got >= sizeof(Normal_00424c00)) {
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
        int got = ((Class_004b4c80*)file)->FUN_004b4c80(&rec, sizeof(Anim_00424c00));
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
            Spot_00424c00* s = &g_game->spots[c->spot];

            s->damage = rec.damage;
            s->frame = rec.frame;
            s->animBits = rec.bits & 0xf0;
        }
    }

    n = ((Class_004b4800*)file)->FUN_004b4800("Number of 3D Features", 0);
    file->FUN_004b4ba0("3D Features");
    for (k = 0; k < n; k++) {
        Model_00424c00 rec;
        ((Class_004b4c10*)file)->FUN_004b4c10(k * sizeof(Model_00424c00));
        int got = ((Class_004b4c80*)file)->FUN_004b4c80(&rec, sizeof(Model_00424c00));
        if (got >= sizeof(Model_00424c00)) {
            Cell_00424c00* c = FUN_00481550(rec.x, rec.y);
            unsigned short type = remap[rec.feature];
            FUN_00423c50(c, type, &rec.pos, &rec.rot, 10);
            g_game->spots[c->spot].damage = rec.damage;
        }
    }
}
