// Decompiled by deepseek-v4.1-flash, Opus, space-bunny-free, GPT-6, Sonnet and Claude Opus 5.5, finished by deepseek-v4.1-flash. Names are provisional.
// The unit type registry: one UnitDef per unit, its FBI / COB data, the
// movement classes, and the downloadable build menu tables. UnitDef, TdfFile,
// TdfRecord, MovementClass and Game each have one definition; the views the
// functions used sit in unions where they disagree.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <list>
// Included only for its symbols: it puts LoadUnitFbi's yardmap stores back
// in the original's base/index order (docs/c2-regalloc.md).
#include <malloc.h>

#pragma pack(push, 1)

struct Vec3 {
    int x, y, z;
};

// The game's 16.16 fixed-point value. GetFieldFixed returns it by value (through
// a hidden pointer, since it has a constructor) and takes the default by value.
class Fixed {
public:
    int value;
    Fixed(int v) { value = v; }
};

// One unit type, 0x249 bytes. The field names are 0x42bf40's; the two flag
// words keep 0x42b370's bit groupings so its hand-written operator= still sees
// them (bit 5 downloadable, bit 6 canbuild, bit 31 gui of flags1; bits 20..22
// selfdestructcountdown of flags2).
class UnitDef {
public:
    char name[0x20];                    // +0x000
    char unitname[0x20];                // +0x020
    char description[0x40];             // +0x040
    char objectname[0x20];              // +0x080
    char f0a0[0x1e];                    // +0x0a0
    char f0be[0x40];                    // +0x0be
    char f0fe[0x40];                    // +0x0fe
    int f13e;                           // +0x13e
    int f142;                           // +0x142
    int f146;                           // +0x146
    union {
        int f14a;                       // +0x14a
        struct {
            short footprintx;           // +0x14a
            short footprintz;           // +0x14c
        };
    };
    union {
        int f14e;                       // +0x14e
        char* yardmap;                  // +0x14e
    };
    int field_152;                      // +0x152
    void* field_156;                    // +0x156
    int f15a;                           // +0x15a
    Vec3 extentmin;                     // +0x15e
    Vec3 extentmax;                     // +0x16a
    Vec3 extentsize;                    // +0x176
    int radius;                         // +0x182
    float buildcostenergy;              // +0x186
    float buildcostmetal;               // +0x18a
    void* field_18e;                    // +0x18e
    int maxvelocity;                    // +0x192
    int maxslopevelocity;               // +0x196
    int brakerate;                      // +0x19a
    int acceleration;                   // +0x19e
    int bankscale;                      // +0x1a2
    int pitchscale;                     // +0x1a6
    int damagemodifier;                 // +0x1aa
    int moverate1;                      // +0x1ae
    int moverate2;                      // +0x1b2
    void* movementclass;                // +0x1b6
    short turnrate;                     // +0x1ba
    short corpse;                       // +0x1bc
    short maxwaterdepth;                // +0x1be
    short minwaterdepth;                // +0x1c0
    float energymake;                   // +0x1c2
    float energyuse;                    // +0x1c6
    float metalmake;                    // +0x1ca
    float extractsmetal;                // +0x1ce
    float windgenerator;                // +0x1d2
    float tidalgenerator;               // +0x1d6
    float cloakcost;                    // +0x1da
    float cloakcostmoving;              // +0x1de
    float energystorage;                // +0x1e2
    float metalstorage;                 // +0x1e6
    int buildtime;                      // +0x1ea
    char* weapons[3];                   // +0x1ee
    int maxdamage;                      // +0x1fa
    short workertime;                   // +0x1fe
    short healtime;                     // +0x200
    short sightdistance;                // +0x202
    short radardistance;                // +0x204
    short sonardistance;                // +0x206
    short mincloakdistance;             // +0x208
    short radardistancejam;             // +0x20a
    short sonardistancejam;             // +0x20c
    short soundcategory;                // +0x20e
    short buildangle;                   // +0x210
    short builddistance;                // +0x212
    short maneuverleashlength;          // +0x214
    short attackrunlength;              // +0x216
    short kamikazedistance;             // +0x218
    short sortbias;                     // +0x21a
    short cruisealt;                    // +0x21c
    unsigned short id;                  // +0x21e
    char* explodeas;                    // +0x220
    char* selfdestructas;               // +0x224
    unsigned char maxslope;             // +0x228
    unsigned char maxwaterslope;        // +0x229
    char transportsize;                 // +0x22a
    char transportcapacity;             // +0x22b
    char waterline;                     // +0x22c
    char makesmetal;                    // +0x22d
    unsigned char field_22e;            // +0x22e
    char bmcode;                        // +0x22f
    char defaultmissiontype;            // +0x230
    void* weaponCategories[3];          // +0x231
    void* nochasecategory;              // +0x23d
    union {
        unsigned int flags1;            // +0x241
        struct {
            unsigned int b00 : 2;
            unsigned int b02 : 2;
            unsigned int b04 : 1;
            unsigned int downloadable : 1;
            unsigned int canbuild : 1;
            unsigned int b07 : 1;
            unsigned int b08 : 1;
            unsigned int b09 : 1;
            unsigned int b10 : 1;
            unsigned int b11 : 1;
            unsigned int b12 : 1;
            unsigned int b13 : 1;
            unsigned int b14 : 1;
            unsigned int b15 : 1;
            unsigned int b16 : 1;
            unsigned int b17 : 1;
            unsigned int b18 : 1;
            unsigned int b19 : 1;
            unsigned int b20 : 1;
            unsigned int b21 : 1;
            unsigned int b22 : 1;
            unsigned int b23 : 1;
            unsigned int b24 : 1;
            unsigned int b25 : 1;
            unsigned int b26 : 1;
            unsigned int b27 : 1;
            unsigned int b28 : 1;
            unsigned int b29 : 1;
            unsigned int b30 : 1;
            unsigned int gui : 1;
        };
    };
    union {
        unsigned int flags2;            // +0x245
        struct {
            unsigned int c00 : 1;
            unsigned int c01 : 1;
            unsigned int c02 : 1;
            unsigned int c03 : 1;
            unsigned int c04 : 1;
            unsigned int c05 : 1;
            unsigned int c06 : 1;
            unsigned int c07 : 1;
            unsigned int c08 : 1;
            unsigned int c09 : 1;
            unsigned int c10 : 1;
            unsigned int c11 : 1;
            unsigned int c12 : 1;
            unsigned int c13 : 1;
            unsigned int c14 : 1;
            unsigned int c15 : 1;
            unsigned int c16 : 1;
            unsigned int c17 : 1;
            unsigned int c18 : 1;
            unsigned int c19 : 1;
            unsigned int selfdestructcountdown : 3;
        };
    };

    UnitDef& operator=(const UnitDef& src);
    void AddToCategories(char* category);
};

// One 0xbd-byte downloadable build list: a count and five entries.
struct BuildEntry_0042dcf0 {
    unsigned short typeId;             // +0x00
    unsigned char page;                // +0x02
    unsigned char slot;                // +0x03
    char name[0x21];                   // +0x04
};

struct BuildList_0042dcf0 {
    int count;                         // +0x00
    BuildEntry_0042dcf0 entries[5];    // +0x04
};

struct Game {
    char unknown_0[0xc];               // +0x0
    void* field_c;                     // +0xc
    char unknown_10[0x14377 - 0x10];
    void** field_14377;                // +0x14377
    void* field_1437b;                 // +0x1437b
    char unknown_1437f[0x1438f - 0x1437f];
    int field_1438f;                   // +0x1438f
    int field_14393;                   // +0x14393
    int field_14397;                   // +0x14397
    UnitDef* field_1439b;              // +0x1439b
    char unknown_1439f[0x37e13 - 0x1439f];
    char* field_37e13;                 // +0x37e13
    int field_37e17;                   // +0x37e17
    char unknown_37e1b[0x37e1f - 0x37e1b];
    int field_37e1f;                   // +0x37e1f
    int field_37e23;                   // +0x37e23
    char unknown_37e27[0x38d71 - 0x37e27];
    unsigned char field_38d71;         // +0x38d71
    char unknown_38d72[0x391c7 - 0x38d72];
    int field_391c7;                   // +0x391c7
    BuildList_0042dcf0* field_391cb;   // +0x391cb
};

// The game's movement classes: 0x20-byte entries in a 32-entry table.
class MovementClass {
public:
    int* field_0;                      // +0x0
    short field_4;                     // +0x4
    short field_6;                     // +0x6
    short field_8;                     // +0x8
    short field_a;                     // +0xa
    unsigned char field_c;             // +0xc
    unsigned char field_d;             // +0xd
    unsigned char field_e;             // +0xe
    unsigned char field_f;             // +0xf
    int field_10;                      // +0x10
    int field_14;                      // +0x14
    void* field_18;                    // +0x18
    int field_1c;                      // +0x1c

    MovementClass();
    ~MovementClass();
    void ReadMoveInfo(void* parser);
};

struct MovementClassTable {
    MovementClass entries[32];

    static MovementClassTable g_movementClasses;
};

// The memory cache object at g_game+0x1437b. Its three methods are separate
// classes in data/symbols.csv (Construct, Initialize, Destroy), so the names
// stay apart for the calls.
class Class_00458160 {
public:
    char unknown_0[0x10];
    int field_10;

    Class_00458160* Construct(void);
};

class Class_00458180 {
public:
    void Initialize(int size);
};

class Class_004581c0 {
public:
    char unknown_0[0x10];
    void* ptr;                         // +0x10

    void Destroy();
};

// A parsed TDF file; the getters read the current section.
class TdfRecord;

class Class_004c4630 {
public:
    char* FindFieldValue(char* key);
};

class Class_004c46c0 {
public:
    int GetFieldInt(char* key, int def);
};

class Class_004c4760 {
public:
    double GetFieldDouble(char* key, double def);
};

class TdfRecord {
public:
    char* FindFieldValue(char* key);
    int GetFieldString(char* dst, char* key, int size, char* def);
    int GetFieldInt(char* key, int def);
    Fixed GetFieldFixed(char* key, Fixed def);
    double GetFieldDouble(const char* key, double def);
};

class TdfFile {
public:
    int field_0;                       // +0x0
    TdfRecord* current;                // +0x4
    int field_8;                       // +0x8

    TdfFile();
    ~TdfFile();
    int GetString(char* dst, char* key, int size, char* def)
    {
        return current->GetFieldString(dst, key, size, def);
    }
    int GetInt(char* key, int def) { return current->GetFieldInt(key, def); }
    double GetDouble(char* key, double def) { return ((Class_004c4760*)current)->GetFieldDouble(key, def); }
    char* GetValue(char* key) { return ((Class_004c4630*)current)->FindFieldValue(key); }
    int LoadFile(char* file);
    void LoadBuffer(char* data, int size, int flag, char* name);
    int SelectRecord(char* name);
    void ResetCurrentRecord();
    void Unload();
    int GetCurrentRecord();
    void StripComments(char* text);
    int SelectRecordAt(int index);
    void SetCurrentRecord(int record);
};

class Class_004c9390 {
public:
    char* data;

    void ReleaseRef();
};

class Class_004c91a0 {
public:
    char* p;

    ~Class_004c91a0() { ((Class_004c9390*)this)->ReleaseRef(); }
};

class Class_00438760 {
public:
    char value;                        // +0x0
    Class_00438760(char* text);
};

// Puts the one-byte Class_00438760 temporary at [esp+0x23], the top byte of
// its slot, where the original builds it; a plain named local lands at the
// bottom of the slot.
struct MissionHolder {
    char pad[3];
    Class_00438760 mission;
    MissionHolder(char* text) : mission(text) {}
};

#pragma pack(pop)

extern Game* g_game;
extern char DAT_005119b8[];
extern char DAT_00503ea0[];

void* __stdcall GetCategoryMask(char* name);
void __stdcall GetLocalizedString(void* parser, char* dst, char* key, int size, char* def);
void __cdecl ProtectBlockReadWrite(void* param_1);
void __cdecl ProtectBlockReadOnly(void* param_1);
void __cdecl FUN_004d85a0(void* param_1);
void __stdcall FreeCobScript(void* param_1);
void __stdcall BuildDataPath(char* out, const char* dir, const char* name, const char* ext);
void* __cdecl FUN_004d83b0(const char* name, int size);
unsigned short __stdcall FindUnitTypeId(char* name);
void __stdcall LoadUnitFbi(char* path, UnitDef* type);
void AddDownloadBuildOptions();
void __stdcall ListDirectory(const char* pattern, int flags, std::vector<Class_004c91a0>* out);
void __stdcall FUN_00432fb0(void* start, void* end, void* cmp, int param);
void __stdcall FUN_00432d40(void* start, void* end, void* cmp, int param);
int __stdcall CompareUnitTypeNames(const char* a, const char* b);
int __cdecl GameStrdup(char* name);
void __stdcall FatalError(const char* msg);
int __stdcall HAPI_FileLengthByName(char* path);
void* __stdcall Load3do(char* path);
void __stdcall MirrorObject(void* obj);
int __stdcall GetObjectHeight(void* obj);
void* __stdcall LoadCobScript(char* path);
void __stdcall StripExtension(char* text);
void __stdcall FUN_0042a140(void* obj, char* name);
short __stdcall FindOrLoadFeatureType(char* name);
void* __stdcall FindMovementClass(char* name);
char* __stdcall FindWeaponByName(char* name);
void LoadUnitInfo();

// The float fields' getter. The outer parentheses matter (note 7 of 0x42bf40).
#define GETFLOAT(section, key) ((float)(section)->GetFieldDouble(key, 0.0))

static inline Vec3 operator-(const Vec3& a, const Vec3& b) {
    Vec3 r;
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    r.z = a.z - b.z;
    return r;
}

// UnitDef::operator= for the 0x249-byte UnitDef element. Hand-written:
// char arrays are copied with byte loops, plain ints/shorts/structs member by
// member, and the two flag words at +0x241/+0x245 bit by bit.
// FUNCTION: 0x42b370 ??4UnitDef@@QAEAAV0@ABV0@@Z
UnitDef& UnitDef::operator=(const UnitDef& src)
{
    unsigned int i;

    for (i = 0; i < 0x20; i++) name[i] = src.name[i];
    for (i = 0; i < 0x20; i++) unitname[i] = src.unitname[i];
    for (i = 0; i < 0x40; i++) description[i] = src.description[i];
    for (i = 0; i < 0x20; i++) objectname[i] = src.objectname[i];
    for (i = 0; i < 0x1e; i++) f0a0[i] = src.f0a0[i];
    for (i = 0; i < 0x40; i++) f0be[i] = src.f0be[i];
    for (i = 0; i < 0x40; i++) f0fe[i] = src.f0fe[i];

    f13e = src.f13e; f142 = src.f142; f146 = src.f146; f14a = src.f14a;
    f14e = src.f14e; field_152 = src.field_152; field_156 = src.field_156;
    f15a = src.f15a;
    extentmin = src.extentmin; extentmax = src.extentmax; extentsize = src.extentsize;
    radius = src.radius; buildcostenergy = src.buildcostenergy;
    buildcostmetal = src.buildcostmetal; field_18e = src.field_18e;
    maxvelocity = src.maxvelocity; maxslopevelocity = src.maxslopevelocity;
    brakerate = src.brakerate; acceleration = src.acceleration;
    bankscale = src.bankscale; pitchscale = src.pitchscale;
    damagemodifier = src.damagemodifier; moverate1 = src.moverate1;
    moverate2 = src.moverate2; movementclass = src.movementclass;
    turnrate = src.turnrate; corpse = src.corpse;
    maxwaterdepth = src.maxwaterdepth; minwaterdepth = src.minwaterdepth;
    energymake = src.energymake; energyuse = src.energyuse;
    metalmake = src.metalmake; extractsmetal = src.extractsmetal;
    windgenerator = src.windgenerator; tidalgenerator = src.tidalgenerator;
    cloakcost = src.cloakcost; cloakcostmoving = src.cloakcostmoving;
    energystorage = src.energystorage; metalstorage = src.metalstorage;
    buildtime = src.buildtime;
    for (i = 0; i < 3; i++) weapons[i] = src.weapons[i];
    maxdamage = src.maxdamage;
    workertime = src.workertime; healtime = src.healtime;
    sightdistance = src.sightdistance; radardistance = src.radardistance;
    sonardistance = src.sonardistance; mincloakdistance = src.mincloakdistance;
    radardistancejam = src.radardistancejam; sonardistancejam = src.sonardistancejam;
    soundcategory = src.soundcategory; buildangle = src.buildangle;
    builddistance = src.builddistance; maneuverleashlength = src.maneuverleashlength;
    attackrunlength = src.attackrunlength; kamikazedistance = src.kamikazedistance;
    sortbias = src.sortbias; cruisealt = src.cruisealt;
    id = src.id;
    explodeas = src.explodeas; selfdestructas = src.selfdestructas;
    maxslope = src.maxslope; maxwaterslope = src.maxwaterslope;
    transportsize = src.transportsize; transportcapacity = src.transportcapacity;
    waterline = src.waterline; makesmetal = src.makesmetal;
    field_22e = src.field_22e; bmcode = src.bmcode;
    defaultmissiontype = src.defaultmissiontype;
    for (i = 0; i < 3; i++) weaponCategories[i] = src.weaponCategories[i];
    nochasecategory = src.nochasecategory;

    b00 = src.b00; b02 = src.b02;
    b04 = src.b04; downloadable = src.downloadable;
    canbuild = src.canbuild; b07 = src.b07;
    b08 = src.b08; b09 = src.b09;
    b10 = src.b10; b11 = src.b11;
    b12 = src.b12; b13 = src.b13;
    b14 = src.b14; b15 = src.b15;
    b16 = src.b16; b17 = src.b17;
    b18 = src.b18; b19 = src.b19;
    b20 = src.b20; b21 = src.b21;
    b22 = src.b22; b23 = src.b23;
    b24 = src.b24; b25 = src.b25;
    b26 = src.b26; b27 = src.b27;
    b28 = src.b28; b29 = src.b29;
    b30 = src.b30; gui = src.gui;

    c00 = src.c00; c01 = src.c01;
    c02 = src.c02; c03 = src.c03;
    c04 = src.c04; c05 = src.c05;
    c06 = src.c06; c07 = src.c07;
    c08 = src.c08; c09 = src.c09;
    c10 = src.c10; c11 = src.c11;
    c12 = src.c12; c13 = src.c13;
    c14 = src.c14; c15 = src.c15;
    c16 = src.c16; c17 = src.c17;
    c18 = src.c18; c19 = src.c19;
    selfdestructcountdown = src.selfdestructcountdown;

    return *this;
}

// FUNCTION: 0x42bcc0
void FreeUnitInfo()
{
    if (g_game->field_1439b != 0) {
        ProtectBlockReadWrite(g_game->field_1439b);
        FUN_004d85a0(g_game->field_1439b);
        g_game->field_1439b = 0;
        g_game->field_1438f = 0;
    }
}

// FUNCTION: 0x42bd10
void RefreshUnitInfo()
{
    if (g_game->field_1438f == 0 || g_game->field_14397 != 0) {
        LoadUnitInfo();
        g_game->field_14397 = 0;
    }
}

// Walks the unit definition table at g_game+0x1439b (0x249-byte entries) and
// the table at g_game+0x391cb (0xbd-byte entries). For every pair that shares a
// name, if the def's bit 5 at +0x241 is clear it prints a warning and sets the
// "downloadable" bit (the table's lock is taken around the update).
//
// Best so far 100%: the loop must be `for (i = 0; i < count; i++, def += 0x249)`
// with the whole thing a single for statement. That is what puts `inc i` before
// `add esi, 0x249` and the bitfield access at +0x241 makes MSVC pick the
// bitfield storage word as the element base (name is then esi-0x221).
// The initial `i = 0` test must be MSVC's own rotation: no source guard.
// FUNCTION: 0x42bd40
void CheckDownloadableFlags()
{
    UnitDef* def = g_game->field_1439b;
    for (int i = 0; i < g_game->field_1438f;
         i++, def = (UnitDef*)((char*)def + 0x249)) {
        for (int j = 0; j < g_game->field_391c7; j++) {
            if (_strcmpi(g_game->field_391cb[j].entries[0].name, def->unitname) == 0
                && !def->downloadable) {
                char message[128];
                sprintf(message,
                        "Hey!  Somebody forgot to set downloadable=1 for %s",
                        def->unitname);
                ProtectBlockReadWrite(g_game->field_1439b);
                def->downloadable = 1;
                ProtectBlockReadOnly(g_game->field_1439b);
            }
        }
    }
}

// Makes the game's entry table writable, then fills each entry's short list
// (max 30) with the type ids whose element rows match the entry index, and
// finally restores the table to read-only.
// FUNCTION: 0x42be30
void AddDownloadBuildOptions()
{
    ProtectBlockReadWrite(g_game->field_1439b);
    UnitDef* e = g_game->field_1439b;
    for (int a = 0; a < g_game->field_1438f; a++, e++) {
        if (e->field_156 != 0) {
            for (int b = 0; b < g_game->field_391c7; b++) {
                for (int c = 0; c < g_game->field_391cb[b].count; c++) {
                    if (a == g_game->field_391cb[b].entries[c].typeId && e->field_152 <= 0x1e) {
                        unsigned short id = FindUnitTypeId(g_game->field_391cb[b].entries[c].name);
                        if (id != 0) {
                            ((unsigned short*)e->field_156)[e->field_152] = id;
                            e->field_152++;
                        }
                    }
                }
            }
        }
    }
    ProtectBlockReadOnly(g_game->field_1439b);
}

// Reads a unit's FBI file (the UNITINFO section of a TDF) into its 0x249-byte
// unit definition: names, costs, movement, energy, the two flag words, the
// self-destruct countdown, the sound category, corpse, movement class,
// weapons, the yard map and the footprint extents.
//
// Claude Opus 5.5 (#4745): 81.0% -> 94.5%, same size as the original (4772
// bytes). The earlier notes (passes 3 to 16, in git history) chased one
// shared-zero register; these were the real causes:
//   1. Every default argument is a literal 0, and the sound category loop has
//      its own counter (`for (sound = 0; ...)` inside the if, with the store
//      and `goto` on a hit and `= 0` in the else). Then the constant 0 and
//      the counter share esi, as in the original (81.0 -> 89.9).
//   2. The YardMap pointer is cleared twice: once before `if (bmcode == 0)`
//      and again in its else branch (the original has both stores).
//   3. GetFieldFixed returns its 16.16 value by value through a hidden
//      pointer (`Fixed` has constructors), so its result temporaries, the
//      fild temporaries and the yard loop's y all share the frame slot at
//      [esp+0x1c], as in the original; no `scratch` local is needed
//      (89.2 -> 92.7).
//   4. The extents tail is `size = max - min` with an inline Vec3
//      operator- returning by value (92.7 -> 94.0).
//   5. The self-destruct countdown is a 3-bit bitfield store (`= atoi(...)`
//      or `= 5`), and the sound category test goes through an int local
//      (`cmp eax, esi` instead of `test eax, eax`).
//   6. The minimum and maximum extents read the footprint fields directly,
//      with no w/h locals.
// Claude Opus 5.5 (#5476): 94.5% -> MATCH (the bytes and every name match;
// the checker still needs a data/constants.csv row for the immediate
// 0x500000, the countdown's `5 << 20`, which lies in the image's range).
//   7. The x87 stores: each float field is read through GETFLOAT, whose body
//      is a parenthesised cast, `((float)call)`. With the parentheses the
//      fstp lands after the next call's pushes and `this` load, as in the
//      original; `(float)call` alone stores right after the call. The same
//      parentheses inside an inline helper (`return ((float)value);`) work
//      too, and GetFieldDouble is declared with its real double return. This
//      also fixed the six flag-word statements, which no longer differ.
//   8. The YardMap stores read the map through `char*& map =
//      unitdef->yardmap;` declared after the loop locals, so map has the
//      larger symbol id and is the base of `[map + cell]`.
//   9. `w * h` for the yard allocation loads footprintz first only with the
//      parentheses of 7; without them every spelling of the multiply, its
//      locals and the loops kept footprintx first (it moved only when
//      distinct pointer-based memory expressions were added or removed
//      before the footprint stores).
//  10. The `(TdfRecord*)` casts left on parser.current change nothing in
//      type, but each takes a symbol id; dropping any of them moves the
//      register allocation.
// FUNCTION: 0x42bf40
void __stdcall LoadUnitFbi(char* fbi_file, UnitDef* unitdef) {
    TdfFile parser;
    char buf[100];
    char weapon[128];
    char yard[1024];

    if (parser.LoadFile(fbi_file)) {
        if (!parser.SelectRecord("UNITINFO")) {
            parser.Unload();
            goto FINISH;
        }
        {
            parser.current->GetFieldString(unitdef->unitname, "unitname", 0x20, DAT_005119b8);
            GetLocalizedString(&parser, unitdef->name, "name", 0x20, 0);
            GetLocalizedString(&parser, unitdef->description, "description", 0x40, 0);
            parser.current->GetFieldString(buf, "defaultmissiontype", 100, DAT_005119b8);
            MissionHolder m(buf);
            unitdef->defaultmissiontype = m.mission.value;
            parser.current->GetFieldString(buf, "wpri_badTargetCategory", 100, DAT_00503ea0);
            unitdef->weaponCategories[0] = GetCategoryMask(buf);
            parser.current->GetFieldString(buf, "wsec_badTargetCategory", 100, DAT_00503ea0);
            unitdef->weaponCategories[1] = GetCategoryMask(buf);
            parser.current->GetFieldString(buf, "wspe_badTargetCategory", 100, DAT_00503ea0);
            unitdef->weaponCategories[2] = GetCategoryMask(buf);
            parser.current->GetFieldString(buf, "noChaseCategory", 100, DAT_00503ea0);
            unitdef->nochasecategory = GetCategoryMask(buf);
            if (parser.current
                    ->GetFieldString(unitdef->objectname, "objectname", 0x20, DAT_005119b8) == 0) {
                strcpy(unitdef->objectname, unitdef->unitname);
            }
            unitdef->buildcostenergy = (float)parser.current->GetFieldInt("buildcostenergy", 0);
            unitdef->buildcostmetal = (float)parser.current->GetFieldInt("buildcostmetal", 0);
            unitdef->maxvelocity =
                parser.current->GetFieldFixed("maxvelocity", Fixed(0)).value;
            unitdef->brakerate =
                parser.current->GetFieldFixed("brakerate", Fixed(0)).value;
            unitdef->acceleration =
                parser.current->GetFieldFixed("acceleration", Fixed(0)).value;
            unitdef->bankscale =
                parser.current->GetFieldFixed("bankscale", Fixed(0x10000)).value;
            unitdef->pitchscale =
                parser.current->GetFieldFixed("pitchscale", Fixed(0)).value;
            unitdef->damagemodifier =
                parser.current->GetFieldFixed("damagemodifier", Fixed(0x10000)).value;
            unitdef->moverate1 =
                parser.current->GetFieldFixed("moverate1", Fixed(unitdef->maxvelocity * 2)).value;
            unitdef->moverate2 =
                parser.current->GetFieldFixed("moverate2", Fixed(unitdef->maxvelocity * 2)).value;
            unitdef->turnrate =
                (short)parser.current->GetFieldInt("turnrate", 0);
            unitdef->waterline = (char)((TdfRecord*)parser.current)->GetFieldInt("waterline", 0);
            unitdef->transportsize =
                (char)((TdfRecord*)parser.current)->GetFieldInt("transportsize", 0);
            unitdef->transportcapacity =
                (char)((TdfRecord*)parser.current)->GetFieldInt("transportcapacity", 0);
            unitdef->energymake = GETFLOAT(parser.current, "energymake");
            unitdef->energyuse = GETFLOAT(parser.current, "energyuse");
            unitdef->metalmake = GETFLOAT(parser.current, "metalmake");
            unitdef->extractsmetal = GETFLOAT(parser.current, "extractsmetal");
            unitdef->makesmetal = (char)((TdfRecord*)parser.current)->GetFieldInt("makesmetal", 0);
            unitdef->windgenerator = GETFLOAT(parser.current, "windgenerator");
            unitdef->tidalgenerator = GETFLOAT(parser.current, "tidalgenerator");
            unitdef->energystorage = GETFLOAT(parser.current, "energystorage");
            unitdef->metalstorage = GETFLOAT(parser.current, "metalstorage");
            unitdef->buildtime =
                ((TdfRecord*)parser.current)->GetFieldInt("buildtime", 0);
            unitdef->workertime =
                (short)((TdfRecord*)parser.current)->GetFieldInt("workertime", 0);
            unitdef->healtime =
                (short)((TdfRecord*)parser.current)->GetFieldInt("healtime", 0);
            unitdef->maxdamage =
                ((TdfRecord*)parser.current)->GetFieldInt("maxdamage", 0);
            unitdef->sightdistance =
                (short)((TdfRecord*)parser.current)->GetFieldInt("sightdistance", 0);
            unitdef->radardistance =
                (short)((TdfRecord*)parser.current)->GetFieldInt("radardistance", 0);
            unitdef->sonardistance =
                (short)((TdfRecord*)parser.current)->GetFieldInt("sonardistance", 0);
            unitdef->radardistancejam =
                (short)((TdfRecord*)parser.current)->GetFieldInt("radardistancejam", 0);
            unitdef->sonardistancejam =
                (short)((TdfRecord*)parser.current)->GetFieldInt("sonardistancejam", 0);
            unitdef->bmcode = (char)((TdfRecord*)parser.current)->GetFieldInt("bmcode", 0);
            unsigned int value2;
            unsigned int value;
            value = ((TdfRecord*)parser.current)->GetFieldInt("standingmoveorder", 2);
            unitdef->flags1 =
                (value ^ unitdef->flags1) & 3 ^ unitdef->flags1;
            value = ((TdfRecord*)parser.current)->GetFieldInt("standingfireorder", 2);
            unitdef->flags1 =
                (value & 3) << 2 | unitdef->flags1 & 0xfffffff3;
            value = ((TdfRecord*)parser.current)->GetFieldInt("init_cloaked", 0);
            unitdef->flags1 =
                (value & 1) << 4 | unitdef->flags1 & 0xffffffef;
            value = ((TdfRecord*)parser.current)->GetFieldInt("downloadable", 0);
            unitdef->flags1 =
                (value & 1) << 5 | unitdef->flags1 & 0xffffffdf;
            value = ((TdfRecord*)parser.current)->GetFieldInt("builder", 0);
            unitdef->flags1 =
                (value & 1) << 6 | unitdef->flags1 & 0xffffffbf;
            value = ((TdfRecord*)parser.current)->GetFieldInt("stealth", 0);
            unitdef->flags1 =
                (value & 1) << 8 | unitdef->flags1 & 0xfffffeff;
            unitdef->cloakcost = (float)((TdfRecord*)parser.current)->GetFieldInt("cloakcost", 0);
            unitdef->cloakcostmoving = (float)((TdfRecord*)parser.current)->GetFieldInt("cloakcostmoving", (int)unitdef->cloakcost);
            unitdef->mincloakdistance =
                (short)((TdfRecord*)parser.current)->GetFieldInt("mincloakdistance", 0);
            unitdef->buildangle =
                (short)((TdfRecord*)parser.current)->GetFieldInt("buildangle", 0);
            unitdef->builddistance =
                (short)((TdfRecord*)parser.current)->GetFieldInt("builddistance", 0);
            unitdef->sortbias =
                (short)((TdfRecord*)parser.current)->GetFieldInt("sortbias", 0);
            unitdef->cruisealt =
                (short)((TdfRecord*)parser.current)->GetFieldInt("cruisealt", 0);
            value = ((TdfRecord*)parser.current)->GetFieldInt("zbuffer", 0);
            unitdef->flags1 =
                (value & 1) << 7 | unitdef->flags1 & 0xffffff7f;
            value = ((TdfRecord*)parser.current)->GetFieldInt("isairbase", 0);
            unitdef->flags1 =
                (value & 1) << 9 | unitdef->flags1 & 0xfffffdff;
            value = ((TdfRecord*)parser.current)->GetFieldInt("istargetingupgrade", 0);
            unitdef->flags1 =
                (value & 1) << 10 | unitdef->flags1 & 0xfffffbff;
            value = ((TdfRecord*)parser.current)->GetFieldInt("teleporter", 0);
            unitdef->flags1 =
                unitdef->flags1 & 0xffffdfff | (value & 1) << 0xd;
            value = ((TdfRecord*)parser.current)->GetFieldInt("hidedamage", 0);
            unitdef->flags1 =
                unitdef->flags1 & 0xffffbfff | (value & 1) << 0xe;
            value = ((TdfRecord*)parser.current)->GetFieldInt("shootme", 0);
            unitdef->flags1 =
                unitdef->flags1 & 0xffff7fff | (value & 1) << 0xf;
            value = ((TdfRecord*)parser.current)->GetFieldInt("armoredstate", 0);
            unitdef->flags1 =
                (value & 1) << 0x11 | unitdef->flags1 & 0xfffdffff;
            value = ((TdfRecord*)parser.current)->GetFieldInt("activatewhenbuilt", 0);
            unitdef->flags1 =
                (value & 1) << 0x12 | unitdef->flags1 & 0xfffbffff;
            value = ((TdfRecord*)parser.current)->GetFieldInt("canfly", 0);
            unitdef->flags1 =
                (value & 1) << 0xb | unitdef->flags1 & 0xfffff7ff;
            value = ((TdfRecord*)parser.current)->GetFieldInt("canhover", 0);
            unitdef->flags1 =
                unitdef->flags1 & 0xffffefff | (value & 1) << 0xc;
            value = ((TdfRecord*)parser.current)->GetFieldInt("upright", 0);
            unitdef->flags1 =
                (value & 1) << 0x14 | unitdef->flags1 & 0xffefffff;
            value = ((TdfRecord*)parser.current)->GetFieldInt("floater", 0);
            unitdef->flags1 =
                (value & 1) << 0x13 | unitdef->flags1 & 0xfff7ffff;
            value = ((TdfRecord*)parser.current)->GetFieldInt("amphibious", 0);
            unitdef->flags1 =
                (value & 1) << 0x15 | unitdef->flags1 & 0xffdfffff;
            value = ((TdfRecord*)parser.current)->GetFieldInt("isfeature", 0);
            unitdef->flags1 =
                (value & 1) << 0x18 | unitdef->flags1 & 0xfeffffff;
            value = ((TdfRecord*)parser.current)->GetFieldInt("noshadow", 0);
            unitdef->flags1 =
                (value & 1) << 0x19 | unitdef->flags1 & 0xfdffffff;
            value = ((TdfRecord*)parser.current)->GetFieldInt("immunetoparalyzer", 0);
            unitdef->flags1 =
                (value & 1) << 0x1a | unitdef->flags1 & 0xfbffffff;
            value = ((TdfRecord*)parser.current)->GetFieldInt("hoverattack", 0);
            unitdef->flags1 =
                (value & 1) << 0x1b | unitdef->flags1 & 0xf7ffffff;
            value = ((TdfRecord*)parser.current)->GetFieldInt("antiweapons", 0);
            unitdef->flags1 =
                unitdef->flags1 & 0xdfffffff | (value & 1) << 0x1d;
            value = ((TdfRecord*)parser.current)->GetFieldInt("digger", 0);
            unitdef->flags1 =
                unitdef->flags1 & 0xbfffffff | (value & 1) << 0x1e;
            value = ((TdfRecord*)parser.current)->GetFieldInt("onoffable", 0);
            unitdef->flags2 =
                (value & 1) << 2 | unitdef->flags2 & 0xfffffffb;
            value = ((TdfRecord*)parser.current)->GetFieldInt("mobilestandorders", 0);
            unitdef->flags2 =
                (value ^ unitdef->flags2) & 1 ^ unitdef->flags2;
            value = ((TdfRecord*)parser.current)->GetFieldInt("firestandorders", 0);
            unitdef->flags2 =
                (value & 1) << 1 | unitdef->flags2 & 0xfffffffd;
            value = ((TdfRecord*)parser.current)->GetFieldInt("canstop", 0);
            unitdef->flags2 =
                (value & 1) << 3 | unitdef->flags2 & 0xfffffff7;
            value = ((TdfRecord*)parser.current)->GetFieldInt("canattack", 0);
            unitdef->flags2 =
                (value & 1) << 4 | unitdef->flags2 & 0xffffffef;
            value = ((TdfRecord*)parser.current)->GetFieldInt("canguard", 0);
            unitdef->flags2 =
                (value & 1) << 5 | unitdef->flags2 & 0xffffffdf;
            value = ((TdfRecord*)parser.current)->GetFieldInt("canpatrol", 0);
            unitdef->flags2 =
                (value & 1) << 6 | unitdef->flags2 & 0xffffffbf;
            value = ((TdfRecord*)parser.current)->GetFieldInt("canmove", 0);
            unitdef->flags2 =
                (value & 1) << 7 | unitdef->flags2 & 0xffffff7f;
            value = ((TdfRecord*)parser.current)->GetFieldInt("canload", 0);
            unitdef->flags2 =
                (value & 1) << 8 | unitdef->flags2 & 0xfffffeff;
            value = ((TdfRecord*)parser.current)->GetFieldInt("canreclamate", 0);
            unitdef->flags2 =
                (value & 1) << 10 | unitdef->flags2 & 0xfffffbff;
            value = ((TdfRecord*)parser.current)->GetFieldInt("canresurrect", 0);
            value2 = (value & 1) << 0xb | unitdef->flags2 & 0xfffff7ff;
            unitdef->flags2 = value2 & 0xfffffdff | (value2 & 0x400) >> 1;
            value2 = ((TdfRecord*)parser.current)->GetFieldInt("cancapture", 0);
            value = unitdef->flags2;
            value2 = (value2 & 1) << 0xc;
            value = value & 0xffffefff | value2;
            unitdef->flags2 = value;
            value2 = (unsigned int)(unitdef->cloakcost > 0.0f);
            unitdef->flags2 = value & 0xffffdfff | (value2 & 1) << 0xd;
            value = ((TdfRecord*)parser.current)->GetFieldInt("candgun", 0);
            unitdef->flags2 =
                unitdef->flags2 & 0xffffbfff | (value & 1) << 0xe;
            unitdef->maneuverleashlength =
                (short)((TdfRecord*)parser.current)->GetFieldInt("maneuverleashlength", 0);
            unitdef->attackrunlength =
                (short)((TdfRecord*)parser.current)->GetFieldInt("attackrunlength", 0);
            value = ((TdfRecord*)parser.current)->GetFieldInt("kamikaze", 0);
            unitdef->flags1 =
                unitdef->flags1 & 0xefffffff | (value & 1) << 0x1c;
            unitdef->kamikazedistance =
                (short)((TdfRecord*)parser.current)->GetFieldInt("kamikazedistance", 0);
            value = ((TdfRecord*)parser.current)->GetFieldInt("norestrict", 0);
            unitdef->flags2 =
                unitdef->flags2 & 0xffff7fff | (value & 1) << 0xf;
            value = ((TdfRecord*)parser.current)->GetFieldInt("showplayername", 0);
            unitdef->flags2 =
                (value & 1) << 0x11 | unitdef->flags2 & 0xfffdffff;
            value = ((TdfRecord*)parser.current)->GetFieldInt("commander", 0);
            unitdef->flags2 =
                (value & 1) << 0x12 | unitdef->flags2 & 0xfffbffff;
            value = ((TdfRecord*)parser.current)->GetFieldInt("cantbetransported", 0);
            unitdef->flags2 =
                (value & 1) << 0x13 | unitdef->flags2 & 0xfff7ffff;

            char* countdown =
                ((TdfRecord*)parser.current)->FindFieldValue("selfdestructcountdown");
            if (countdown != (char*)0)
                unitdef->selfdestructcountdown = atoi(countdown);
            else
                unitdef->selfdestructcountdown = 5;
            ((TdfRecord*)parser.current)->GetFieldString(buf, "category", 100, DAT_005119b8);
            unitdef->AddToCategories(buf);
            int found = ((TdfRecord*)parser.current)
                    ->GetFieldString(buf, "soundcategory", 100, DAT_005119b8);
            if (found) {
                int sound;
                for (sound = 0; sound < g_game->field_37e17; sound++) {
                    if (_strcmpi(g_game->field_37e13 + sound * 0x160, buf) == 0) {
                        unitdef->soundcategory = (short)sound;
                        goto SOUND_DONE;
                    }
                }
                unitdef->soundcategory = (short)atoi(buf);
            } else {
                unitdef->soundcategory = 0;
            }
        SOUND_DONE:
            unitdef->corpse = -1;
            if (((TdfRecord*)parser.current)->GetFieldString(buf, "corpse", 100, DAT_005119b8))
                unitdef->corpse = FindOrLoadFeatureType(buf);
            unitdef->movementclass = 0;
            if (((TdfRecord*)parser.current)
                    ->GetFieldString(buf, "movementclass", 100, DAT_005119b8))
                unitdef->movementclass = FindMovementClass(buf);
            MovementClass movement;
            MovementClass* move = (MovementClass*)unitdef->movementclass;
            if (move == 0) {
                ((MovementClass*)&movement)->ReadMoveInfo(&parser);
                move = &movement;
            }
            unitdef->footprintx = move->field_4;
            unitdef->footprintz = move->field_6;
            unitdef->maxwaterdepth = move->field_8;
            unitdef->minwaterdepth = move->field_a;
            unitdef->maxslope = move->field_c;
            unitdef->maxwaterslope = move->field_e;
            unitdef->maxslopevelocity = (int)(((__int64)unitdef->maxvelocity << 16) /
                                             ((unitdef->maxslope + 1) * 0x10000));
            char* defaultWeapon = (char*)g_game + 0x2cf3;
            ((TdfRecord*)parser.current)->GetFieldString(weapon, "weapon1", 128, DAT_005119b8);
            char* weapon1 = FindWeaponByName(weapon);
            unitdef->weapons[0] = weapon1 ? weapon1 : defaultWeapon;
            ((TdfRecord*)parser.current)->GetFieldString(weapon, "weapon2", 128, DAT_005119b8);
            char* weapon2 = FindWeaponByName(weapon);
            unitdef->weapons[1] = weapon2 ? weapon2 : defaultWeapon;
            ((TdfRecord*)parser.current)->GetFieldString(weapon, "weapon3", 128, DAT_005119b8);
            char* weapon3 = FindWeaponByName(weapon);
            unitdef->weapons[2] = weapon3 ? weapon3 : defaultWeapon;
            ((TdfRecord*)parser.current)->GetFieldString(weapon, "explodeas", 128, DAT_005119b8);
            char* explodeas = FindWeaponByName(weapon);
            unitdef->explodeas = explodeas ? explodeas : defaultWeapon;
            ((TdfRecord*)parser.current)
                ->GetFieldString(weapon, "selfdestructas", 128, DAT_005119b8);
            char* selfdestructas = FindWeaponByName(weapon);
            unitdef->selfdestructas = selfdestructas ? selfdestructas : defaultWeapon;
            if (unitdef->weapons[0] == defaultWeapon &&
                unitdef->weapons[1] == defaultWeapon &&
                unitdef->weapons[2] == defaultWeapon)
                unitdef->flags1 &= ~0x10000;
            else
                unitdef->flags1 |= 0x10000;
            unitdef->yardmap = 0;
            if (unitdef->bmcode == 0) {
                ((TdfRecord*)parser.current)
                    ->GetFieldString(yard, "YardMap", 1024, DAT_005119b8);
                unitdef->yardmap = (char*)FUN_004d83b0(
                    "BUILDING YARD", unitdef->footprintx * unitdef->footprintz);
                int cell = 0;
                char* cursor = yard;
                int y = 0;
                char*& map = unitdef->yardmap;
                while (y < unitdef->footprintz) {
                    for (int x = 0; x < unitdef->footprintx;) {
                        switch (*cursor) {
                        case '.':
                            map[cell] = 0x0;
                            break;
                        case 'f':
                            map[cell] = 0x6f;
                            break;
                        case 'o':
                            map[cell] = 0x2f;
                            break;
                        case 'c':
                            map[cell] = 0x2d;
                            break;
                        case 'O':
                            map[cell] = 0x2b;
                            break;
                        case 'w':
                            map[cell] = 0x37;
                            break;
                        case 'C':
                            map[cell] = 0x35;
                            break;
                        case 'y':
                            map[cell] = 0x29;
                            break;
                        case 'Y':
                            map[cell] = 0x31;
                            break;
                        case 'G':
                            map[cell] = 0x8f;
                            break;
                        default:
                            cursor++;
                            continue;
                        }
                        x++;
                        cell++;
                        if (cursor[1] != 0)
                            cursor++;
                    }
                    y++;
                }
            } else {
                unitdef->yardmap = 0;
            }
            unitdef->extentmin.x = (unitdef->footprintx * -0x100000) / 2;
            unitdef->extentmin.z = (unitdef->footprintz * -0x100000) / 2;
            unitdef->extentmax.x = (unitdef->footprintx << 20) / 2;
            unitdef->extentmax.z = (unitdef->footprintz << 20) / 2;
            unitdef->extentsize = unitdef->extentmax - unitdef->extentmin;
            unitdef->radius = (unitdef->extentsize.z + unitdef->extentsize.x) / 3;
            parser.Unload();
            // cancloak with no mincloakdistance: default to 80
            if ((unitdef->flags2 & 0x2000) && unitdef->mincloakdistance == 0)
                unitdef->mincloakdistance = 80;
        }
    }
FINISH:;
}

// Reads the unit definition and script of a unit type from disk, both under
// "units/<TypeName>/", and stores the script at UnitDef+0x18e: first the
// definition file ("FBI") when it exists, then the script file ("COB").
// The whole table at g_game+0x1439b (0x249-byte entries) is locked while the
// files are read.
// FUNCTION: 0x42d1f0
void __stdcall ReloadUnitType(unsigned short index)
{
    if (index == 0)
        return;
    UnitDef* type = &g_game->field_1439b[index];
    if ((type->flags1 & 0x800000) == 0)
        return;
    ProtectBlockReadWrite(g_game->field_1439b);
    char path[256];
    BuildDataPath(path, "units", type->unitname, "FBI");
    if (HAPI_FileLengthByName(path)) {
        LoadUnitFbi(path, type);
        FreeCobScript(type->field_18e);
        BuildDataPath(path, "scripts", type->unitname, "COB");
        void* cob = LoadCobScript(path);
        type->field_18e = cob;
        ProtectBlockReadOnly(g_game->field_1439b);
    } else {
        ProtectBlockReadOnly(g_game->field_1439b);
    }
}

// MATCH (deepseek-v4.1-flash). The 96.8 residual was one loop-carried CSE: the units loop's
// latch loaded g_game->field_1438f into ecx and the idiv reused it (`idiv ecx`) with g_game
// parked in edi. Reading the divisor through a body-local pointer (`Game* gp = g_game;`
// then `gp->field_1438f`) makes that load a different value, so it rematerialises as
// `idiv [ecx+0x1438f]`, g_game takes ecx, and the mov cx placement and lea order follow.
// `else break;` in the GUI suffix do-while fixes the tail (`jmp`, not test/jne) and is exactly
// the 2 bytes the count-into-memory adds. The last hunk (the canbuild `je` skipping the list
// reload on the zero-iteration edge) needs the index store form `list[count] = val; count++;`
// instead of the walking `*out = val; out++;`. Finally `new Class_00458160` had to become
// `operator new(0x14)` + `obj = obj ? obj->Construct() : 0;` because data/symbols.csv names
// that address Class_00458160::Construct, not the constructor.
// Pass 15 (deepseek-v4.1-flash): shape unchanged, re-confirmed 2173/2173 at 96.8. The whole
// residual diff is three adjacent spots from one allocator decision: (1) the units-loop entry
// guard, ours materialises the count (`mov ecx,[edi+0x1438f]; cmp ecx,esi`) where the original
// folds it (`cmp [ecx+0x1438f],esi`); (2) the divide, ours `idiv ecx` reusing that materialised
// count across the `jle`, the original `idiv [ecx+0x1438f]`; (3) the latch, ours reloads g_game
// into edi and then the count into ecx, the original reloads g_game into ecx and the count into
// eax. `mov edi,[0x511de8]` and `mov ecx,[0x511de8]` are both 6 bytes, so that register pick is
// not a size effect: with the reloaded g_game parked in edi our count takes ecx and survives to
// the divide, while with g_game in ecx the count cannot live across the pushes/cdq and both uses
// fold into memory operands. The remaining GUI suffix tail (`test eax,eax; jne` here against the
// original `jmp`) is the `else break;` form, which alone gives 2171 bytes / 94.4.
// Pass 14 (deepseek-v4.1-flash): byte accounting confirms the pair exactly. Against 2173/96.8
// the original hunk1 (entry test, `cmp [ecx+0x1438f],esi`) is 2 bytes shorter than ours and the
// original hunk3 (`idiv [ecx+0x1438f]`) is 4 bytes longer, while the GUI tail `jmp` is 3 shorter
// than our `test/jne`: -4 +3 = -1, which the jump-displacement byte restores, so count-into-memory
// (+2) and `else break;` (-2) really are the whole gap. Tried this pass: a union alias
// (field_1438f vs field_1438f_alt at the same offset, used for the divisor) compiles to exactly the
// same 2173 bytes, MSVC treats same-offset union members as one location; moving `type->field_21e
// = u;` in front of the percent store inserts the store between the latch count load and the idiv
// but grows the file to 2181 (95.2), it re-schedules the whole body top. Restored 96.8. The div
// reuses the latch value only because the allocator parks that load in ecx (caller-saved, survives
// to the idiv); the original parks it in eax, which cdq kills, so the divisor stays a memory
// operand and ecx stays free for g_game. That pick is not source-spellable with the forms tried.
// Pass 13 (deepseek-v4.1-flash): kept 96.8% (2173 bytes against 2173). Re-read the original:
// at the units-loop entry 0x42d6b0..0x42d6e3 it holds g_game in ecx (`mov ecx,[0x511de8]`),
// folds the count into `cmp dword ptr [ecx+0x1438f],esi`, and divides with
// `idiv dword ptr [ecx+0x1438f]`; the back edge reloads g_game into ecx and the count into eax
// (`cmp esi,eax; jl`). Ours holds g_game in edi and CSEs the count into ecx across the guard
// and the div (`mov ecx,[edi+0x1438f]; cmp ecx,esi; idiv ecx`), which also forces the extra
// `mov cx,[esp+0x14]` and shifts u's home 0x10 -> 0x14; the original's `mov cx,[esp+0x20]`
// (base+0x10 after 16 bytes of pushes) and `lea edx,[esp+0x74]`/`[esp+0x78]` both resolve to
// base+0x70, so only u's slot differs. The CSE is the whole wall: every structural form that
// would separate the guard from the div either duplicates the tail test or breaks the frame,
// so the ecx/edi pick is not source-spellable with the forms tried.
// Pass 12 (deepseek-v4.1-flash): the do-while respelling of the units loop
// (`u = 1; if (1 < g_game->field_1438f) { do { ... u++; } while ((int)u < g_game->field_1438f); }`)
// plus `else break;` in the GUI suffix loop lands exactly on the original tail (no duplicated
// test) at 2171 bytes / 94.4%: the guard/idiv count-in-register CSE
// (`mov ecx,[edi+0x1438f]; cmp ecx,esi; idiv ecx`) is unchanged, so the allocator pick is not
// structure-spelled. Restored the 96.8% for-loop version (which keeps the wrong tail but the
// right 2173-byte length). Both fixes are complementary: count into memory (+2) + else break (-2).
// Pass 10 (deepseek-v4.1-flash): tried the GUI suffix loop again; back to 96.8%. Appending
// `else break;` to the if inside the do-while (keeping `} while (more);`) DOES produce the
// original tail exactly (`test eax,eax; je exit; inc ebx; mov esi,1; jmp head`, no duplicated
// test), but the build is then 2171 bytes, 2 short, so every later jump displacement is off and
// difflib drops the score to 94.4%; the units-loop hunks are unchanged, so the missing 2 bytes
// are in the count load/compare shape there (ours loads field_1438f into ecx and uses `idiv ecx`
// plus a near jle, the original uses two memory operands and a short jle). Fix that pair together
// and the function should land. for(;;) and do-while(1) with break both re-emit the redundant
// test or peel the first iteration (2230 bytes / 90.3%).
// Pass 11 (deepseek-v4.1-flash): the loop/tail pair is one-way so far. Adding `else break;`
// inside the GUI do-while (keeping `} while (more);`) reproduces the original tail exactly at
// 2171 bytes / 94.4% for every divisor spelling tried: `(int)g_game->field_1438f` and
// `*(int*)((char*)g_game + 0x1438f)` compile byte-identically to the plain form, so the
// hoisted count (`mov ecx,[edi+0x1438f]; cmp ecx,esi; idiv ecx`) does not turn into the
// original's two memory operands (`cmp [ecx+0x1438f],esi`, `idiv [ecx+0x1438f]`) from the
// divisor expression. `int u` instead of `unsigned short u` in the units loop breaks the
// whole loop (2198 bytes / 71.6%). Hoisting the `type` declaration out of the units loop and
// declaring `u` outside it are byte-identical at 96.8% (2173), so the ecx/edi flip for
// g_game is still the only thing left; nothing tried this pass moved it.
// Pass 9 (deepseek-v4.1-flash): 96.8% (2173 bytes against 2173, sizes equal). The cursor swap
// that held this at 91.4% is gone: the copy loop must be `*w++ = *s` (not `*w = *s; w++;`), so
// MSVC emits `mov ecx,w; push s; add w,0x249; call` like the original and keeps the cursor in
// edi / the end pointer in ebp. What still differs is one allocator pick, twice: the original
// holds g_game in ecx and re-reads [ecx+0x1438f] for the loop test and the idiv, ours holds the
// count in ecx and g_game in edi (mov ecx,[edi+0x1438f]; idiv ecx). Units-loop for/do-while
// shapes, the unsigned char cast on the quotient and swapping the test operands are all
// byte-neutral at 96.8%; the GUI suffix loop must stay the condition-tested do-while (`for(;;)`
// plus `if (!more) break;` and `do ... while (1);` both duplicate the body, 2230 bytes / 90.3).
// Everything else, including the frame and the jump offsets, matches.
// Older notes (passes 1-8), kept for context. Best was 91.4% (2175 bytes against 2173), deepseek-v4.1-flash. The compaction copy loop
// takes a separate write cursor (`w = p; ... *w = *s; w++;` then `d = w;` after the loop): that
// removes the [esp+0x10] spill of d (the single-variable form scores 91.2 with d reloaded and
// stored around every operator= call). What still differs is only the cursor/end register swap:
// ours keeps the cursor in ebp and `end` in edi, the original has d in edi and `end` in ebp,
// which cascades into the unit-loop hunks. Rejected this pass: `for(;;) { ...; if
// (!HAPI_FileLengthByName(path)) break; ... }` for the suffix loop (rotated, 2240 bytes, 87.2), inert
// file-scope extern declarations (16 / 48 / 80 -> 91.2 / 90.8 / 91.2), moving the w or end
// declaration, `end` declared last (87.7), making w span both branches with `d = w` after the
// if/else (91.2), `d += 1`, `d = 0` initialiser, `last` alias removed, while-copy with s
// declared outside.
// Best 91.2% (2183 bytes against 2173) shape: the GUI suffix loop must be written as a do-while
// whose condition re-reads the HAPI_FileLengthByName result from a local:
//   more = HAPI_FileLengthByName(path); if (more) { suffix++; found = 1; } while (more);
// That stops /O2 from peeling the first iteration; for(;;), while(1) and a goto loop all score
// 86.2 (2240 bytes) because MSVC duplicates the loop body ahead of a rotated loop.
// The compaction keeps the earlier winning shape: keep bit-23-set elements, scan loop and copy
// loop separate, `*d = *s` (not `*d++`, which reshuffles the callee-saved registers and 89.2).
// Restructuring the guard as `if (p != end) { while(...) }` followed by
// `if (p == end) { d = p; } else { d = p; for(...) }` fixed the inverted first guard,
// 90.2 -> 91.2: the original emits `cmp; je Ld` (scan is the fall-through) then after the loop
// `cmp; jne Lelse`, and this shape reproduces both branch layouts.
// Remaining (as of the 91.2 shape; the spill below was later fixed by the write cursor,
// everything after it is an offset cascade):
// the compaction copy loop spills d to [esp+0x10] and reloads/gathers it around every
// operator= call; the original emits `lea esi,[eax+0x249]; mov edi,eax` and keeps d in edi for
// the whole loop, storing it once after (about 6 bytes). Tried and rejected: `*d++ = *s` (89.2,
// moves `end` out of ebp), reusing p as the write pointer (77.7, drops `xor ebx,ebx` early),
// init d=p before the guard (84.1), while-loop copy, swapped declaration order, moving the
// count/last computation (all still 91.2). The unit-loop idiv difference noted by the previous
// worker disappears once the compaction size matches.
// FUNCTION: 0x42d2e0
void LoadUnitTypes() {
    char namebuf[32];
    char section[32];
    char path[256];
    char classbuf[100];
    char objpath[256];
    char valbuf[256];

    {
        TdfFile parser;
        BuildDataPath(path, "gamedata", "moveinfo", "TDF");
        if (!parser.LoadFile(path))
            FatalError("Can't load MOVEINFO.TDF");

        int i = 0;
        MovementClass* cls = MovementClassTable::g_movementClasses.entries;
        MovementClass* cls_end = &MovementClassTable::g_movementClasses.entries[32];
        do {
            sprintf(classbuf, "CLASS%d", i);
            parser.ResetCurrentRecord();
            if (parser.SelectRecord(classbuf)) {
                parser.current->GetFieldString(classbuf, "name", 100, DAT_005119b8);
                cls->field_0 = (int*)GameStrdup(classbuf);
                cls->ReadMoveInfo(&parser);
            }
            cls++;
            i++;
        } while ((int)cls < (int)cls_end);
        parser.Unload();
    }

    Class_00458160* obj = (Class_00458160*)operator new(0x14);
    obj = obj ? obj->Construct() : 0;
    g_game->field_1437b = obj;

    int t = g_game->field_37e23 * g_game->field_37e1f * 2;
    int v = (int)(t * 1.3);

    int n = *(int*)((char*)g_game->field_c + 0x620) / 0x100000 + 1;
    float scale = 1.0f;
    if (n > 0x10) {
        double d = (double)n * 0.0625;
        if (d > 5.0)
            scale = 5.0f;
        else
            scale = (float)d;
    }
    int size = (int)(v * scale);
    ((Class_00458180*)g_game->field_1437b)->Initialize((size + 0xfff) & 0xfffff000);

    ProtectBlockReadWrite(g_game->field_1439b);

    UnitDef* end = g_game->field_1439b + g_game->field_1438f;
    UnitDef* start = g_game->field_1439b + 1;

    UnitDef* p = start;
    UnitDef* d;
    if (p != end) {
        while (p != end && !(~(p->flags1) & 0x800000))
            p++;
    }
    if (p == end) {
        d = p;
    } else {
        UnitDef* w = p;
        for (UnitDef* s = p + 1; s != end; s++) {
            if (!(~(s->flags1) & 0x800000)) {
                *w++ = *s;
            }
        }
        d = w;
    }
    g_game->field_1438f = (int)(d - g_game->field_1439b);

    start = g_game->field_1439b + 1;
    UnitDef* last = d;
    if (last - start <= 0x10) {
        FUN_00432fb0(start, last, (void*)CompareUnitTypeNames, 0);
    } else {
        FUN_00432d40(start, last, (void*)CompareUnitTypeNames, 0);
        UnitDef* q = start + 0x10;
        FUN_00432fb0(start, q, (void*)CompareUnitTypeNames, 0);
        for (; q != last; q++) {
            UnitDef tmp = *q;
            UnitDef* r = q - 1;
            UnitDef* w = q;
            while (_strcmpi(tmp.unitname, r->unitname) < 0) {
                *w = *r;
                w = r;
                r--;
            }
            *w = tmp;
        }
    }

    {
        unsigned short index = 0;
        if (g_game->field_1438f > 0) {
            do {
                g_game->field_1439b[index].id = index;
                index++;
            } while ((int)index < g_game->field_1438f);
        }
    }
    int c = g_game->field_1438f;
    g_game->field_14393 = 0;
    if (c) {
        do {
            c >>= 1;
            g_game->field_14393++;
        } while (c);
    }

    g_game->field_14377 = (void**)FUN_004d83b0("MODEL PTRS", g_game->field_1438f * 4);

    for (unsigned short u = 1; u < g_game->field_1438f; u++) {
        UnitDef* type = &g_game->field_1439b[u];
        Game* gp = g_game;
        g_game->field_38d71 = (unsigned char)((u * 100) / gp->field_1438f);
        type->id = u;
        BuildDataPath(path, "units", type->unitname, "FBI");
        if (HAPI_FileLengthByName(path))
            LoadUnitFbi(path, type);

        strncpy(namebuf, type->objectname, 0x20);
        namebuf[0x1f] = 0;
        BuildDataPath(objpath, "objects3d", namebuf, "3DO");
        void* model = Load3do(objpath);
        if (model == 0)
            FatalError(objpath);
        MirrorObject(model);
        FUN_0042a140(model, namebuf);
        g_game->field_14377[u] = model;
        type->extentmin.y = 0;
        type->extentmax.y = GetObjectHeight(g_game->field_14377[u]);
        type->extentsize.y = type->extentmax.y - type->extentmin.y;

        strcpy(namebuf, type->unitname);
        StripExtension(namebuf);
        sprintf(section, "%s0", namebuf);
        BuildDataPath(path, "guis", section, "GUI");
        if (HAPI_FileLengthByName(path))
            type->gui = 1;
        else
            type->gui = 0;

        int suffix = 1;
        int found = 0;
        int more;
        do {
            sprintf(section, "%s%d", namebuf, suffix);
            BuildDataPath(path, "guis", section, "GUI");
            more = HAPI_FileLengthByName(path);
            if (more) {
                suffix++;
                found = 1;
            } else
                break;
        } while (more);
        if (found)
            type->field_22e = suffix;
        else if (type->gui)
            type->field_22e = 1;
        else
            type->field_22e = 0;

        BuildDataPath(path, "scripts", type->unitname, "COB");
        type->field_18e = LoadCobScript(path);
    }

    ProtectBlockReadOnly(g_game->field_14377);

    TdfFile parser2;
    BuildDataPath(path, "gamedata", "sidedata", "TDF");
    if (!parser2.LoadFile(path)) {
        FatalError("Can't load GAMEDATA.TDF");
    } else {
        short* list = (short*)FUN_004d83b0("TEMP UTYPE LIST", 0x3c);
        for (unsigned short s = 1; s < g_game->field_1438f; s++) {
            UnitDef* type = &g_game->field_1439b[s];
            type->field_152 = 0;
            type->field_156 = 0;
            if (type->canbuild) {
                parser2.ResetCurrentRecord();
                if (parser2.SelectRecord("CANBUILD") &&
                    parser2.SelectRecord(type->unitname)) {
                    int count = 0;
                    int k = 1;
                    sprintf(objpath, "canbuild%d", k);
                    while (parser2.current->GetFieldString(valbuf, objpath, 0x20, DAT_005119b8)) {
                        short val = FindUnitTypeId(valbuf);
                        if (val != 0) {
                            list[count] = val;
                            count++;
                        }
                        k++;
                        sprintf(objpath, "canbuild%d", k);
                    }
                    type->field_152 = count;
                }
                sprintf(objpath, "CANBUILD %s", type->unitname);
                type->field_156 = FUN_004d83b0(objpath, 0x3c);
                memcpy(type->field_156, list, 0x3c);
            }
        }
        FUN_004d85a0(list);
        parser2.Unload();
    }

    g_game->field_38d71 = 100;
    g_game->field_14397 = 1;
    ProtectBlockReadOnly(g_game->field_1439b);
}

// FUNCTION: 0x42db60
int __stdcall CompareUnitTypeNames(const char* param_1, const char* param_2)
{
    return _strcmpi(param_1 + 0x20, param_2 + 0x20) < 0;
}

// Releases the per-unit-type data tables: the array at g_game+0x14377, the
// 0x249-byte entries of g_game+0x1439b (with their script and file members),
// and the object at g_game+0x1437b. The loop starts at index 1 because entry 0
// is not a real unit type.
// FUNCTION: 0x42db90
void FreeUnitTypes()
{
    ProtectBlockReadWrite(g_game->field_1439b);
    ProtectBlockReadWrite(g_game->field_14377);

    for (unsigned short i = 1; i < g_game->field_1438f; i++) {
        UnitDef* type = &g_game->field_1439b[i];
        void* p = g_game->field_14377[i];
        if (p != 0) {
            FUN_004d85a0(p);
            g_game->field_14377[i] = 0;
        }
        if (type->yardmap != 0) {
            FUN_004d85a0(type->yardmap);
            type->yardmap = 0;
        }
        if (type->field_18e != 0) {
            FreeCobScript(type->field_18e);
            type->field_18e = 0;
        }
        if (type->field_156 != 0) {
            FUN_004d85a0(type->field_156);
            type->field_152 = 0;
            type->field_156 = 0;
        }
    }

    Class_004581c0* obj = (Class_004581c0*)g_game->field_1437b;
    if (obj != 0) {
        obj->Destroy();
        delete obj;
    }
    g_game->field_1437b = 0;

    FUN_004d85a0(g_game->field_14377);
    FUN_004d85a0(g_game->field_1439b);

    g_game->field_14377 = 0;
    g_game->field_1439b = 0;
}

// Edited by deepseek-v4.1: MATCH. Fixes this round:
// 1. `int c; int i;` are declared at the top of the body, before `int n = files.size();`,
//    and the last two loops reuse that one counter `c`. A group of scalar locals gets
//    its esp slots in reverse declaration order, so with the counters declared after n
//    the file count n landed in [esp+0x18]/EBP instead of the original [esp+0x10]/EBX;
//    declaring them first moved n to 0x10/EBX and cascaded the whole group (the max-page
//    scan and the downloadable scan, g_game into EDI, the lea esi,[ebx+eax] addressing),
//    lifting 77.6 -> 97.3.
// 2. The downloadable scan (0x42e037-0x42e0bf) kept coming out with the build-list
//    counter in EBX and the i*0xbd byte offset in EBP, the exact inverse of the
//    original (counter EBP, offset EBX, `inc ebp`; `add ebx,0xbd`; `cmp ebp,[eax+..]`).
//    The register assignment follows the order the loop variables are created, mapped
//    onto ESI, EDI, EBX, EBP (the four callee-saved registers live across the _strcmpi
//    call here), so the offset has to be a real source variable created before the
//    counter. Declaring `int off = 0;` and walking with it (`i++, off += 0xbd`,
//    `((char*)g_game->buildLists + off)->entries[0].name`)
//    makes the strength-reduction temp disappear and gives offset EBX / counter EBP.
// 3. `defs[c].name` is used directly at both call sites instead of a `char* name`
//    local: that drops one live-across-call variable, so the hoisted name temp lands
//    in EDI (the `lea edi,[esi-0x221]` in the preheader) instead of competing for EBX.
// Note 0x4c48c0 and 0x4c46c0 are two different classes in data/symbols.csv
// (TdfRecord::GetFieldString and TdfRecord::GetFieldInt), so `current`
// is a TdfRecord* and the int-arg calls cast it to TdfRecord*.
// FUNCTION: 0x42dcf0
void LoadDownloadMenus()
{
    int c;
    int i;
    char path[256];
    char unitbuf[256];
    std::vector<Class_004c91a0> files;
    BuildDataPath(path, "download", "*", "TDF");
    ListDirectory(path, 0, &files);

    int n = files.size();
    g_game->field_391c7 = n;
    g_game->field_391cb = (BuildList_0042dcf0*)FUN_004d83b0("DOWNLOADMENU", n * 0xbd);

    for (i = 0; i < n; i++) {
        TdfFile parser;
        BuildDataPath(path, "download", files[i].p, "TDF");
        if (parser.LoadFile(path)) {
            int j = 0;
            while (1) {
                parser.ResetCurrentRecord();
                if (!parser.SelectRecordAt(j))
                    break;
                g_game->field_391cb[i].count = j + 1;
                char* buf = unitbuf;
                if (parser.current->GetFieldString(unitbuf, "UNITMENU", 0x20, DAT_005119b8)) {
                    for (unsigned short u = 0; u < g_game->field_1438f; u++) {
                        if (_strcmpi(g_game->field_1439b[u].unitname, buf) == 0) {
                            g_game->field_391cb[i].entries[j].typeId = u;
                            g_game->field_391cb[i].entries[j].page = (unsigned char)parser.current->GetFieldInt("MENU", 0);
                            g_game->field_391cb[i].entries[j].slot = (unsigned char)parser.current->GetFieldInt("BUTTON", 0);
                            parser.current->GetFieldString(g_game->field_391cb[i].entries[j].name, "UNITNAME", 0x20, DAT_005119b8);
                            break;
                        }
                    }
                }
                j++;
            }
        }
    }

    ProtectBlockReadWrite(g_game->field_1439b);
    for (unsigned short u = 0; u < g_game->field_1438f; u++) {
        for (c = 0; c < n; c++) {
            for (int d = 0; d < g_game->field_391cb[c].count; d++) {
                if (g_game->field_391cb[c].entries[d].typeId == u) {
                    if (g_game->field_1439b[u].field_22e < g_game->field_391cb[c].entries[d].page)
                        g_game->field_1439b[u].field_22e = g_game->field_391cb[c].entries[d].page;
                }
            }
        }
    }
    ProtectBlockReadOnly(g_game->field_1439b);

    UnitDef* defs = g_game->field_1439b;
    for (c = 0; c < g_game->field_1438f; c++) {
        for (int i = 0; i < g_game->field_391c7; i++) {
            if (_strcmpi(g_game->field_391cb[i].entries[0].name, defs[c].unitname) == 0
                && !defs[c].downloadable) {
                char buf[128];
                sprintf(buf, "Hey!  Somebody forgot to set downloadable=1 for %s", defs[c].unitname);
                ProtectBlockReadWrite(g_game->field_1439b);
                defs[c].downloadable = 1;
                ProtectBlockReadOnly(g_game->field_1439b);
            }
        }
    }

    AddDownloadBuildOptions();
}

// FUNCTION: 0x42e120
void FreeDownloadMenus()
{
    FUN_004d85a0(g_game->field_391cb);
}

// Walks the whitespace separated names in the argument string. Each name is
// looked up in the name to mask table (GetCategoryMask) and this object's team bit
// is set in the mask that name maps to, then the same is done for "ALL", so the
// team always ends up in the ALL mask.
// The first test sits outside the loop (a do/while): that is what puts the
// loop's register save between the test and the body, and it is also what wins
// ebx for `this` instead of edi.
// The original translation unit of LoadUnitFbi saw only this declaration; in
// the merged file /Ob2 would inline the body into it, so keep it out of line.
#pragma auto_inline(off)
// FUNCTION: 0x488e70
void UnitDef::AddToCategories(char* names)
{
    int n;
    char buf[256];
    if (sscanf(names, " %s %n", buf, &n) == 1) {
        do {
            names += n;
            unsigned short team = this->id;
            unsigned int* mask = (unsigned int*)GetCategoryMask(buf);
            mask[team >> 5] |= 1 << (team & 0x1f);
        } while (sscanf(names, " %s %n", buf, &n) == 1);
    }
    unsigned short team = this->id;
    unsigned int* all = (unsigned int*)GetCategoryMask("ALL");
    all[team >> 5] |= 1 << (team & 0x1f);
}
#pragma auto_inline(on)
