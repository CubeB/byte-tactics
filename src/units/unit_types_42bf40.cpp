// Decompiled by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by Claude Opus 5.5. Names are provisional.
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
#include <list>
class Class_004c4630 {
  public:
    char* FindFieldValue(char* key);
};
class Class_00488e70 {
  public:
    void AddToCategories(char* category);
};
class Class_004402e0 {
  public:
    char data[32];
    Class_004402e0();
    ~Class_004402e0();
};
class MovementClass {
  public:
    void ReadMoveInfo(void* parser);
};
extern char* g_game;
short __stdcall FindOrLoadFeatureType(char* name);
void* __stdcall FindMovementClass(char* name);
char* __stdcall FindWeaponByName(char* name);
void* __cdecl FUN_004d83b0(char* name, int size);

class Class_004c2ea0 {
  public:
    int field_0;
    void* current; // +0x4
    int field_8;
    Class_004c2ea0();
    ~Class_004c2ea0();
};

class Class_004c2f60 {
  public:
    int LoadFile(char* file);
};

class Class_004c3410 {
  public:
    int SelectRecord(char* name);
};

class Class_004c3240 {
  public:
    void Unload();
};

class TdfRecord {
  public:
    int GetFieldString(char* dst, char* key, int size, char* def);
};

class Class_004c46c0 {
  public:
    int GetFieldInt(char* key, int def);
};

// The game's 16.16 fixed-point value. GetFieldFixed returns it by value (through
// a hidden pointer, since it has a constructor) and takes the default by value.
class Fixed {
  public:
    int value;
    Fixed(int v) { value = v; }
};

class Class_004c4800 {
  public:
    Fixed GetFieldFixed(char* key, Fixed def);
};

class Class_004c4760 {
  public:
    double GetFieldDouble(const char* key, double def);
};

// The float fields' getter. The outer parentheses matter (note 7).
#define GETFLOAT(section, key) ((float)((Class_004c4760*)(section))->GetFieldDouble(key, 0.0))

class Class_00438760 {
  public:
    char value; // +0x0
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

extern char DAT_005119b8[];
extern char DAT_00503ea0[];

void __stdcall GetLocalizedString(void* parser, char* dst, char* key, int size, char* def);
void* __stdcall GetCategoryMask(char* text);

struct Vec3 {
    int x, y, z;
};

static inline Vec3 operator-(const Vec3& a, const Vec3& b) {
    Vec3 r;
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    r.z = a.z - b.z;
    return r;
}

#pragma pack(push, 1)
struct UnitDef {
    char name[0x20];                    // +0x000
    char unitname[0x20];                // +0x020
    char description[0x40];             // +0x040
    char objectname[0x20];              // +0x080
    char unknown_a0[0xaa];
    short footprintx;                   // +0x14a
    short footprintz;                   // +0x14c
    char* yardmap;                      // +0x14e
    char unknown_152[0xc];
    Vec3 extentmin;                     // +0x15e
    Vec3 extentmax;                     // +0x16a
    Vec3 extentsize;                    // +0x176
    int radius;                         // +0x182
    float buildcostenergy;              // +0x186
    float buildcostmetal;               // +0x18a
    int unknown_18e;
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
    char* weapon1;                      // +0x1ee
    char* weapon2;                      // +0x1f2
    char* weapon3;                      // +0x1f6
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
    short unknown_21e;
    char* explodeas;                    // +0x220
    char* selfdestructas;               // +0x224
    unsigned char maxslope;             // +0x228
    unsigned char maxwaterslope;        // +0x229
    char transportsize;                 // +0x22a
    char transportcapacity;             // +0x22b
    char waterline;                     // +0x22c
    char makesmetal;                    // +0x22d
    char unknown_22e;
    char bmcode;                        // +0x22f
    char defaultmissiontype;            // +0x230
    void* wpri_badtargetcategory;       // +0x231
    void* wsec_badtargetcategory;       // +0x235
    void* wspe_badtargetcategory;       // +0x239
    void* nochasecategory;              // +0x23d
    unsigned int flags1;                // +0x241
    union {
        unsigned int flags2;            // +0x245
        struct {
            unsigned int low : 20;
            unsigned int selfdestructcountdown : 3;
            unsigned int high : 9;
        };
    };
};
#pragma pack(pop)


// FUNCTION: 0x42bf40
void __stdcall LoadUnitFbi(char* fbi_file, UnitDef* unitdef) {
    Class_004c2ea0 parser;
    char buf[100];
    char weapon[128];
    char yard[1024];

    if (((Class_004c2f60*)&parser)->LoadFile(fbi_file)) {
        if (!((Class_004c3410*)&parser)->SelectRecord("UNITINFO")) {
            ((Class_004c3240*)&parser)->Unload();
            goto FINISH;
        }
        {
            ((TdfRecord*)parser.current)
                ->GetFieldString(unitdef->unitname, "unitname", 0x20, DAT_005119b8);
            GetLocalizedString(&parser, unitdef->name, "name", 0x20, 0);
            GetLocalizedString(&parser, unitdef->description, "description", 0x40, 0);
            ((TdfRecord*)parser.current)
                ->GetFieldString(buf, "defaultmissiontype", 100, DAT_005119b8);
            MissionHolder m(buf);
            unitdef->defaultmissiontype = m.mission.value;
            ((TdfRecord*)parser.current)
                ->GetFieldString(buf, "wpri_badTargetCategory", 100, DAT_00503ea0);
            unitdef->wpri_badtargetcategory = GetCategoryMask(buf);
            ((TdfRecord*)parser.current)
                ->GetFieldString(buf, "wsec_badTargetCategory", 100, DAT_00503ea0);
            unitdef->wsec_badtargetcategory = GetCategoryMask(buf);
            ((TdfRecord*)parser.current)
                ->GetFieldString(buf, "wspe_badTargetCategory", 100, DAT_00503ea0);
            unitdef->wspe_badtargetcategory = GetCategoryMask(buf);
            ((TdfRecord*)parser.current)
                ->GetFieldString(buf, "noChaseCategory", 100, DAT_00503ea0);
            unitdef->nochasecategory = GetCategoryMask(buf);
            if (((TdfRecord*)parser.current)
                    ->GetFieldString(unitdef->objectname, "objectname", 0x20, DAT_005119b8) == 0) {
                strcpy(unitdef->objectname, unitdef->unitname);
            }
            unitdef->buildcostenergy = (float)((Class_004c46c0*)parser.current)->GetFieldInt("buildcostenergy", 0);
            unitdef->buildcostmetal = (float)((Class_004c46c0*)parser.current)->GetFieldInt("buildcostmetal", 0);
            unitdef->maxvelocity =
                ((Class_004c4800*)parser.current)->GetFieldFixed("maxvelocity", Fixed(0)).value;
            unitdef->brakerate =
                ((Class_004c4800*)parser.current)->GetFieldFixed("brakerate", Fixed(0)).value;
            unitdef->acceleration =
                ((Class_004c4800*)parser.current)->GetFieldFixed("acceleration", Fixed(0)).value;
            unitdef->bankscale =
                ((Class_004c4800*)parser.current)->GetFieldFixed("bankscale", Fixed(0x10000)).value;
            unitdef->pitchscale =
                ((Class_004c4800*)parser.current)->GetFieldFixed("pitchscale", Fixed(0)).value;
            unitdef->damagemodifier = ((Class_004c4800*)parser.current)
                                            ->GetFieldFixed("damagemodifier", Fixed(0x10000)).value;
            unitdef->moverate1 =
                ((Class_004c4800*)parser.current)
                     ->GetFieldFixed("moverate1", Fixed(unitdef->maxvelocity * 2)).value;
            unitdef->moverate2 =
                ((Class_004c4800*)parser.current)
                     ->GetFieldFixed("moverate2", Fixed(unitdef->maxvelocity * 2)).value;
            unitdef->turnrate =
                (short)((Class_004c46c0*)parser.current)->GetFieldInt("turnrate", 0);
            unitdef->waterline = (char)((Class_004c46c0*)parser.current)->GetFieldInt("waterline", 0);
            unitdef->transportsize =
                (char)((Class_004c46c0*)parser.current)->GetFieldInt("transportsize", 0);
            unitdef->transportcapacity =
                (char)((Class_004c46c0*)parser.current)->GetFieldInt("transportcapacity", 0);
            unitdef->energymake = GETFLOAT(parser.current, "energymake");
            unitdef->energyuse = GETFLOAT(parser.current, "energyuse");
            unitdef->metalmake = GETFLOAT(parser.current, "metalmake");
            unitdef->extractsmetal = GETFLOAT(parser.current, "extractsmetal");
            unitdef->makesmetal = (char)((Class_004c46c0*)parser.current)->GetFieldInt("makesmetal", 0);
            unitdef->windgenerator = GETFLOAT(parser.current, "windgenerator");
            unitdef->tidalgenerator = GETFLOAT(parser.current, "tidalgenerator");
            unitdef->energystorage = GETFLOAT(parser.current, "energystorage");
            unitdef->metalstorage = GETFLOAT(parser.current, "metalstorage");
            unitdef->buildtime =
                ((Class_004c46c0*)parser.current)->GetFieldInt("buildtime", 0);
            unitdef->workertime =
                (short)((Class_004c46c0*)parser.current)->GetFieldInt("workertime", 0);
            unitdef->healtime =
                (short)((Class_004c46c0*)parser.current)->GetFieldInt("healtime", 0);
            unitdef->maxdamage =
                ((Class_004c46c0*)parser.current)->GetFieldInt("maxdamage", 0);
            unitdef->sightdistance =
                (short)((Class_004c46c0*)parser.current)->GetFieldInt("sightdistance", 0);
            unitdef->radardistance =
                (short)((Class_004c46c0*)parser.current)->GetFieldInt("radardistance", 0);
            unitdef->sonardistance =
                (short)((Class_004c46c0*)parser.current)->GetFieldInt("sonardistance", 0);
            unitdef->radardistancejam =
                (short)((Class_004c46c0*)parser.current)->GetFieldInt("radardistancejam", 0);
            unitdef->sonardistancejam =
                (short)((Class_004c46c0*)parser.current)->GetFieldInt("sonardistancejam", 0);
            unitdef->bmcode = (char)((Class_004c46c0*)parser.current)->GetFieldInt("bmcode", 0);
            unsigned int value2;
            unsigned int value;
            value = ((Class_004c46c0*)parser.current)->GetFieldInt("standingmoveorder", 2);
            unitdef->flags1 =
                (value ^ unitdef->flags1) & 3 ^ unitdef->flags1;
            value = ((Class_004c46c0*)parser.current)->GetFieldInt("standingfireorder", 2);
            unitdef->flags1 =
                (value & 3) << 2 | unitdef->flags1 & 0xfffffff3;
            value = ((Class_004c46c0*)parser.current)->GetFieldInt("init_cloaked", 0);
            unitdef->flags1 =
                (value & 1) << 4 | unitdef->flags1 & 0xffffffef;
            value = ((Class_004c46c0*)parser.current)->GetFieldInt("downloadable", 0);
            unitdef->flags1 =
                (value & 1) << 5 | unitdef->flags1 & 0xffffffdf;
            value = ((Class_004c46c0*)parser.current)->GetFieldInt("builder", 0);
            unitdef->flags1 =
                (value & 1) << 6 | unitdef->flags1 & 0xffffffbf;
            value = ((Class_004c46c0*)parser.current)->GetFieldInt("stealth", 0);
            unitdef->flags1 =
                (value & 1) << 8 | unitdef->flags1 & 0xfffffeff;
            unitdef->cloakcost = (float)((Class_004c46c0*)parser.current)->GetFieldInt("cloakcost", 0);
            unitdef->cloakcostmoving = (float)((Class_004c46c0*)parser.current)->GetFieldInt("cloakcostmoving", (int)unitdef->cloakcost);
            unitdef->mincloakdistance =
                (short)((Class_004c46c0*)parser.current)->GetFieldInt("mincloakdistance", 0);
            unitdef->buildangle =
                (short)((Class_004c46c0*)parser.current)->GetFieldInt("buildangle", 0);
            unitdef->builddistance =
                (short)((Class_004c46c0*)parser.current)->GetFieldInt("builddistance", 0);
            unitdef->sortbias =
                (short)((Class_004c46c0*)parser.current)->GetFieldInt("sortbias", 0);
            unitdef->cruisealt =
                (short)((Class_004c46c0*)parser.current)->GetFieldInt("cruisealt", 0);
            value = ((Class_004c46c0*)parser.current)->GetFieldInt("zbuffer", 0);
            unitdef->flags1 =
                (value & 1) << 7 | unitdef->flags1 & 0xffffff7f;
            value = ((Class_004c46c0*)parser.current)->GetFieldInt("isairbase", 0);
            unitdef->flags1 =
                (value & 1) << 9 | unitdef->flags1 & 0xfffffdff;
            value = ((Class_004c46c0*)parser.current)->GetFieldInt("istargetingupgrade", 0);
            unitdef->flags1 =
                (value & 1) << 10 | unitdef->flags1 & 0xfffffbff;
            value = ((Class_004c46c0*)parser.current)->GetFieldInt("teleporter", 0);
            unitdef->flags1 =
                unitdef->flags1 & 0xffffdfff | (value & 1) << 0xd;
            value = ((Class_004c46c0*)parser.current)->GetFieldInt("hidedamage", 0);
            unitdef->flags1 =
                unitdef->flags1 & 0xffffbfff | (value & 1) << 0xe;
            value = ((Class_004c46c0*)parser.current)->GetFieldInt("shootme", 0);
            unitdef->flags1 =
                unitdef->flags1 & 0xffff7fff | (value & 1) << 0xf;
            value = ((Class_004c46c0*)parser.current)->GetFieldInt("armoredstate", 0);
            unitdef->flags1 =
                (value & 1) << 0x11 | unitdef->flags1 & 0xfffdffff;
            value = ((Class_004c46c0*)parser.current)->GetFieldInt("activatewhenbuilt", 0);
            unitdef->flags1 =
                (value & 1) << 0x12 | unitdef->flags1 & 0xfffbffff;
            value = ((Class_004c46c0*)parser.current)->GetFieldInt("canfly", 0);
            unitdef->flags1 =
                (value & 1) << 0xb | unitdef->flags1 & 0xfffff7ff;
            value = ((Class_004c46c0*)parser.current)->GetFieldInt("canhover", 0);
            unitdef->flags1 =
                unitdef->flags1 & 0xffffefff | (value & 1) << 0xc;
            value = ((Class_004c46c0*)parser.current)->GetFieldInt("upright", 0);
            unitdef->flags1 =
                (value & 1) << 0x14 | unitdef->flags1 & 0xffefffff;
            value = ((Class_004c46c0*)parser.current)->GetFieldInt("floater", 0);
            unitdef->flags1 =
                (value & 1) << 0x13 | unitdef->flags1 & 0xfff7ffff;
            value = ((Class_004c46c0*)parser.current)->GetFieldInt("amphibious", 0);
            unitdef->flags1 =
                (value & 1) << 0x15 | unitdef->flags1 & 0xffdfffff;
            value = ((Class_004c46c0*)parser.current)->GetFieldInt("isfeature", 0);
            unitdef->flags1 =
                (value & 1) << 0x18 | unitdef->flags1 & 0xfeffffff;
            value = ((Class_004c46c0*)parser.current)->GetFieldInt("noshadow", 0);
            unitdef->flags1 =
                (value & 1) << 0x19 | unitdef->flags1 & 0xfdffffff;
            value = ((Class_004c46c0*)parser.current)->GetFieldInt("immunetoparalyzer", 0);
            unitdef->flags1 =
                (value & 1) << 0x1a | unitdef->flags1 & 0xfbffffff;
            value = ((Class_004c46c0*)parser.current)->GetFieldInt("hoverattack", 0);
            unitdef->flags1 =
                (value & 1) << 0x1b | unitdef->flags1 & 0xf7ffffff;
            value = ((Class_004c46c0*)parser.current)->GetFieldInt("antiweapons", 0);
            unitdef->flags1 =
                unitdef->flags1 & 0xdfffffff | (value & 1) << 0x1d;
            value = ((Class_004c46c0*)parser.current)->GetFieldInt("digger", 0);
            unitdef->flags1 =
                unitdef->flags1 & 0xbfffffff | (value & 1) << 0x1e;
            value = ((Class_004c46c0*)parser.current)->GetFieldInt("onoffable", 0);
            unitdef->flags2 =
                (value & 1) << 2 | unitdef->flags2 & 0xfffffffb;
            value = ((Class_004c46c0*)parser.current)->GetFieldInt("mobilestandorders", 0);
            unitdef->flags2 =
                (value ^ unitdef->flags2) & 1 ^ unitdef->flags2;
            value = ((Class_004c46c0*)parser.current)->GetFieldInt("firestandorders", 0);
            unitdef->flags2 =
                (value & 1) << 1 | unitdef->flags2 & 0xfffffffd;
            value = ((Class_004c46c0*)parser.current)->GetFieldInt("canstop", 0);
            unitdef->flags2 =
                (value & 1) << 3 | unitdef->flags2 & 0xfffffff7;
            value = ((Class_004c46c0*)parser.current)->GetFieldInt("canattack", 0);
            unitdef->flags2 =
                (value & 1) << 4 | unitdef->flags2 & 0xffffffef;
            value = ((Class_004c46c0*)parser.current)->GetFieldInt("canguard", 0);
            unitdef->flags2 =
                (value & 1) << 5 | unitdef->flags2 & 0xffffffdf;
            value = ((Class_004c46c0*)parser.current)->GetFieldInt("canpatrol", 0);
            unitdef->flags2 =
                (value & 1) << 6 | unitdef->flags2 & 0xffffffbf;
            value = ((Class_004c46c0*)parser.current)->GetFieldInt("canmove", 0);
            unitdef->flags2 =
                (value & 1) << 7 | unitdef->flags2 & 0xffffff7f;
            value = ((Class_004c46c0*)parser.current)->GetFieldInt("canload", 0);
            unitdef->flags2 =
                (value & 1) << 8 | unitdef->flags2 & 0xfffffeff;
            value = ((Class_004c46c0*)parser.current)->GetFieldInt("canreclamate", 0);
            unitdef->flags2 =
                (value & 1) << 10 | unitdef->flags2 & 0xfffffbff;
            value = ((Class_004c46c0*)parser.current)->GetFieldInt("canresurrect", 0);
            value2 = (value & 1) << 0xb | unitdef->flags2 & 0xfffff7ff;
            unitdef->flags2 = value2 & 0xfffffdff | (value2 & 0x400) >> 1;
            value2 = ((Class_004c46c0*)parser.current)->GetFieldInt("cancapture", 0);
            value = unitdef->flags2;
            value2 = (value2 & 1) << 0xc;
            value = value & 0xffffefff | value2;
            unitdef->flags2 = value;
            value2 = (unsigned int)(unitdef->cloakcost > 0.0f);
            unitdef->flags2 = value & 0xffffdfff | (value2 & 1) << 0xd;
            value = ((Class_004c46c0*)parser.current)->GetFieldInt("candgun", 0);
            unitdef->flags2 =
                unitdef->flags2 & 0xffffbfff | (value & 1) << 0xe;
            unitdef->maneuverleashlength =
                (short)((Class_004c46c0*)parser.current)->GetFieldInt("maneuverleashlength", 0);
            unitdef->attackrunlength =
                (short)((Class_004c46c0*)parser.current)->GetFieldInt("attackrunlength", 0);
            value = ((Class_004c46c0*)parser.current)->GetFieldInt("kamikaze", 0);
            unitdef->flags1 =
                unitdef->flags1 & 0xefffffff | (value & 1) << 0x1c;
            unitdef->kamikazedistance =
                (short)((Class_004c46c0*)parser.current)->GetFieldInt("kamikazedistance", 0);
            value = ((Class_004c46c0*)parser.current)->GetFieldInt("norestrict", 0);
            unitdef->flags2 =
                unitdef->flags2 & 0xffff7fff | (value & 1) << 0xf;
            value = ((Class_004c46c0*)parser.current)->GetFieldInt("showplayername", 0);
            unitdef->flags2 =
                (value & 1) << 0x11 | unitdef->flags2 & 0xfffdffff;
            value = ((Class_004c46c0*)parser.current)->GetFieldInt("commander", 0);
            unitdef->flags2 =
                (value & 1) << 0x12 | unitdef->flags2 & 0xfffbffff;
            value = ((Class_004c46c0*)parser.current)->GetFieldInt("cantbetransported", 0);
            unitdef->flags2 =
                (value & 1) << 0x13 | unitdef->flags2 & 0xfff7ffff;

            char* countdown =
                ((Class_004c4630*)parser.current)->FindFieldValue("selfdestructcountdown");
            if (countdown != (char*)0)
                unitdef->selfdestructcountdown = atoi(countdown);
            else
                unitdef->selfdestructcountdown = 5;
            ((TdfRecord*)parser.current)->GetFieldString(buf, "category", 100, DAT_005119b8);
            ((Class_00488e70*)unitdef)->AddToCategories(buf);
            int found = ((TdfRecord*)parser.current)
                    ->GetFieldString(buf, "soundcategory", 100, DAT_005119b8);
            if (found) {
                int sound;
                for (sound = 0; sound < *(int*)(g_game + 0x37e17); sound++) {
                    if (_strcmpi(*(char**)(g_game + 0x37e13) + sound * 0x160, buf) == 0) {
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
            Class_004402e0 movement;
            char* move = (char*)unitdef->movementclass;
            if (move == 0) {
                ((MovementClass*)&movement)->ReadMoveInfo(&parser);
                move = (char*)&movement;
            }
            unitdef->footprintx = *(short*)(move + 4);
            unitdef->footprintz = *(short*)(move + 6);
            unitdef->maxwaterdepth = *(short*)(move + 8);
            unitdef->minwaterdepth = *(short*)(move + 10);
            unitdef->maxslope = move[12];
            unitdef->maxwaterslope = move[14];
            unitdef->maxslopevelocity = (int)(((__int64)unitdef->maxvelocity << 16) /
                                             ((unitdef->maxslope + 1) * 0x10000));
            char* defaultWeapon = g_game + 0x2cf3;
            ((TdfRecord*)parser.current)->GetFieldString(weapon, "weapon1", 128, DAT_005119b8);
            char* weapon1 = FindWeaponByName(weapon);
            unitdef->weapon1 = weapon1 ? weapon1 : defaultWeapon;
            ((TdfRecord*)parser.current)->GetFieldString(weapon, "weapon2", 128, DAT_005119b8);
            char* weapon2 = FindWeaponByName(weapon);
            unitdef->weapon2 = weapon2 ? weapon2 : defaultWeapon;
            ((TdfRecord*)parser.current)->GetFieldString(weapon, "weapon3", 128, DAT_005119b8);
            char* weapon3 = FindWeaponByName(weapon);
            unitdef->weapon3 = weapon3 ? weapon3 : defaultWeapon;
            ((TdfRecord*)parser.current)->GetFieldString(weapon, "explodeas", 128, DAT_005119b8);
            char* explodeas = FindWeaponByName(weapon);
            unitdef->explodeas = explodeas ? explodeas : defaultWeapon;
            ((TdfRecord*)parser.current)
                ->GetFieldString(weapon, "selfdestructas", 128, DAT_005119b8);
            char* selfdestructas = FindWeaponByName(weapon);
            unitdef->selfdestructas = selfdestructas ? selfdestructas : defaultWeapon;
            if (unitdef->weapon1 == defaultWeapon &&
                unitdef->weapon2 == defaultWeapon &&
                unitdef->weapon3 == defaultWeapon)
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
            ((Class_004c3240*)&parser)->Unload();
            // cancloak with no mincloakdistance: default to 80
            if ((unitdef->flags2 & 0x2000) && unitdef->mincloakdistance == 0)
                unitdef->mincloakdistance = 80;
        }
    }
FINISH:;
}
