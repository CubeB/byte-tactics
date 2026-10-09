// Decompiled by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1, finished by GPT-6.1-sol, finished by deepseek-v4.1-flash, edited by deepseek-v4.1-flash, finished by claude-opus-5.5, space-bunny-free. Names are provisional.

#include <string.h>
#include <vector>

// 3.14159265358979 / 180 is exactly the exe's 0.017453292519943278.
#define PI 3.14159265358979

class TdfRecord {
  public:
    char* GetRecordName();
    int GetFieldString(char* dst, const char* key, int size, char* def);
    int GetFieldInt(const char* key, int def);
    double GetFieldDouble(const char* key, double def);
    TdfRecord* FindSubRecord(const char* key);
    char* GetFieldName(int index);
};

class Class_004c9390 {
  public:
    char* ptr;
    void ReleaseRef();
};

// A reference-counted string handle (0x4c91a0 copies, 0x4c9390 releases).
class Class_004c91a0 {
  public:
    char* ptr;
    Class_004c91a0(const Class_004c91a0& other);
    ~Class_004c91a0() { ((Class_004c9390*)this)->ReleaseRef(); }
    Class_004c91a0& operator=(const Class_004c91a0& other);
};

class Class_004c91b0 : public Class_004c91a0 {
  public:
    Class_004c91b0(const char* text);
};

static inline bool operator==(const Class_004c91a0& a, const Class_004c91a0& b) {
    return strcmp(a.ptr, b.ptr) == 0;
}

static inline bool Ne(const Class_004c91a0& a, const Class_004c91a0& b) {
    return !(a == b);
}

static inline bool Less(const char* a, const char* b) {
    return _strcmpi(a, b) < 0;
}

// One entry of a weapon's damage table: a unit name and its damage.
struct Entry_00432cf0 {
    Class_004c91a0 name; // +0x0
    int value;           // +0x4

    Entry_00432cf0(const Class_004c91a0& n, int v) : name(n), value(v) {}
};

#pragma pack(push, 1)
// The damage table, kept sorted by name.
class Map_0042e440 {
  public:
    char less;                     // +0x0
    std::vector<Entry_00432cf0> v; // +0x1

    Entry_00432cf0* LowerBound(const char* key) {
        Entry_00432cf0* first = v.begin();
        Entry_00432cf0* last = v.end();
        while (first != last) {
            Entry_00432cf0* mid = first + (last - first) / 2;
            if (Less(mid->name.ptr, key))
                first = mid + 1;
            else
                last = mid;
        }
        return first;
    }
};
#pragma pack(pop)

void __stdcall SetWeaponFireHandler(void*);


#pragma pack(push, 1)
struct Weapon_0042e440 {
    char name[0x20];               // +0x000
    char name2[0x40];              // +0x020
    char pad_60[4];                // +0x060
    Map_0042e440* sub;            // +0x064
    int weaponvelocity;            // +0x068
    int startvelocity;             // +0x06c
    int weaponacceleration;        // +0x070
    void* text;                    // +0x074
    void* anim1;                   // +0x078
    void* anim2;                   // +0x07c
    char model[0x40];              // +0x080
    float energypershot;           // +0x0c0
    float metalpershot;            // +0x0c4
    float minbarrelangle;          // +0x0c8
    int shakemagnitude;            // +0x0cc
    int shakeduration;             // +0x0d0
    short damage;                  // +0x0d4
    short areaofeffect;            // +0x0d6
    float edgeeffectiveness;       // +0x0d8
    int range;                     // +0x0dc
    int coverage;                  // +0x0e0
    short reloadtime;              // +0x0e4
    short weapontimer;             // +0x0e6
    short turnrate;                // +0x0e8
    short burst;                   // +0x0ea
    short burstrate;               // +0x0ec
    short sprayangle;              // +0x0ee
    short duration;                // +0x0f0
    short randomdecay;             // +0x0f2
    unsigned short soundstart;     // +0x0f4
    unsigned short soundhit;       // +0x0f6
    unsigned short soundwater;     // +0x0f8
    short smokedelay;              // +0x0fa
    short flighttime;              // +0x0fc
    short holdtime;                // +0x0fe
    char pad_100[4];               // +0x100
    short accuracy;                // +0x104
    short tolerance;               // +0x106
    short pitchtolerance;          // +0x108
    unsigned char id;              // +0x10a
    unsigned char firestarter;     // +0x10b
    unsigned char rendertype;      // +0x10c
    unsigned char color;           // +0x10d
    unsigned char color2;          // +0x10e
    unsigned char pad_10f[2];      // +0x10f
    unsigned int lineofsight : 1;  // +0x111 bit 0
    unsigned int ballistic : 1;    // bit 1
    unsigned int shellweapon : 1;  // bit 2
    unsigned int beamweapon : 1;   // bit 3
    unsigned int vlaunch : 1;      // bit 4
    unsigned int meteor : 1;       // bit 5
    unsigned int noradar : 1;      // bit 6
    unsigned int paralyzer : 1;    // bit 7
    unsigned int dropped : 1;      // bit 8
    unsigned int startsmoke : 1;   // bit 9
    unsigned int endsmoke : 1;     // bit 10
    unsigned int soundtrigger : 1; // bit 11
    unsigned int guidance : 1;     // bit 12
    unsigned int tracks : 1;       // bit 13
    unsigned int unitsonly : 1;    // bit 14
    unsigned int groundbounce : 1; // bit 15
    unsigned int waterweapon : 1;  // bit 16
    unsigned int toairweapon : 1;  // bit 17
    unsigned int smoketrail : 1;   // bit 18
    unsigned int turret : 1;       // bit 19
    unsigned int selfprop : 1;     // bit 20
    unsigned int propeller : 1;    // bit 21
    unsigned int noexplode : 1;    // bit 22
    unsigned int burnblow : 1;     // bit 23
    unsigned int twophase : 1;     // bit 24
    unsigned int cruise : 1;       // bit 25
    unsigned int commandfire : 1;  // bit 26
    unsigned int noautorange : 1;  // bit 27
    unsigned int stockpile : 1;    // bit 28
    unsigned int targetable : 1;   // bit 29
    unsigned int interceptor : 1;  // bit 30
    unsigned int : 1;              // bit 31
};

struct Game {
    char unknown_0[0x2cf3];
    Weapon_0042e440 weapons[0x100]; // +0x2cf3, stride 0x115
};
#pragma pack(pop)

extern Game* g_game;
extern char DAT_005119b8[];

void __stdcall BuildDataPath(char* out, const char* dir, const char* name, const char* ext);
void __stdcall FatalError(char* path);
void* __stdcall Load3do(char* path);
void __stdcall MirrorObject(void* p);
void __stdcall BindModelTextures(void* a, char* b);
void* __stdcall LoadAnimGaf(char* name);
void* __stdcall FindGafEntry(void* a, char* b);
int __stdcall LoadSoundByName(void* a, char* b);
class TdfFile {
public:
    int root;                          // +0x0
    int current;                       // +0x4
    int field_8;                       // +0x8

    TdfFile();
    ~TdfFile();
    int LoadFile(char* file);
    void ResetCurrentRecord();
    int SelectRecordAt(int index);
};

void __stdcall ListDirectory(const char* pattern, int flags, std::vector<Class_004c91a0>* out);
void __stdcall LoadWeaponType(int section);
int FUN_0041d8a0(void);
extern void __cdecl operator delete(void*);
void __cdecl GameFreeThunk(void* p);

// Loads every "Weapons\\*.tdf" file, parses each with the TDF parser, and
// calls LoadWeaponType once per top level section found in it.
// FUNCTION: 0x42e310
void LoadWeaponTypes()
{
    int n = 0;
    for (int i = 0; i < 0x100; i++) {
        Weapon_0042e440* p = &g_game->weapons[i];
        p->id = n++;
        p->name[0] = 0;
    }

    char path[256];
    std::vector<Class_004c91a0> files;
    ListDirectory("Weapons\\*.tdf", 0, &files);

    for (Class_004c91a0* p = files.begin(); p < files.end(); p++) {
        TdfFile parser;
        BuildDataPath(path, "Weapons", p->ptr, "TDF");
        if (parser.LoadFile(path)
            && (parser.field_8 || FUN_0041d8a0() == 0)) {
            int i = 0;
            while (1) {
                parser.ResetCurrentRecord();
                if (!parser.SelectRecordAt(i))
                    break;
                LoadWeaponType(parser.current);
                i++;
            }
        }
    }
}

// Loads one weapon from a .TDF section: the numbers and flags, the model, the
// explosion animations, the sounds and the DAMAGE sub-section, which goes into
// a table sorted by unit name (w->sub).
// FUNCTION: 0x42e440
void __stdcall LoadWeaponType(TdfRecord* parser) {
    char* id = parser->GetRecordName();
    Weapon_0042e440* w = &g_game->weapons[parser->GetFieldInt("ID", -1)];
    strcpy(w->name, id);
    parser->GetFieldString(w->name2, "name", 0x40, DAT_005119b8);

    w->weaponvelocity =
        (int)(parser->GetFieldDouble("weaponvelocity", 0.0) * 2184.5333333333333);
    w->startvelocity =
        (int)(parser->GetFieldDouble("startvelocity", 0.0) * 2184.5333333333333);
    w->weaponacceleration =
        (int)(parser->GetFieldDouble("weaponacceleration", 0.0) *
              72.81777777777778);
    w->range = parser->GetFieldInt("range", 0x7fff);
    w->coverage = parser->GetFieldInt("coverage", 0);
    w->reloadtime = (short)(parser->GetFieldDouble("reloadtime", 0.0) * 30.0);
    w->energypershot = (float)parser->GetFieldDouble("energypershot", 0.0);
    w->metalpershot = (float)parser->GetFieldDouble("metalpershot", 0.0);
    w->areaofeffect = (short)parser->GetFieldInt("areaofeffect", 0);
    w->edgeeffectiveness = (float)parser->GetFieldDouble("edgeeffectiveness", 0.0);
    w->weapontimer = (short)(parser->GetFieldDouble("weapontimer", 0.0) * 30.0);
    w->noautorange = parser->GetFieldInt("noautorange", 0);
    w->turnrate =
        (short)(parser->GetFieldDouble("turnrate", 0.0) * 0.03333333333333333);
    w->burst = (short)parser->GetFieldInt("burst", 0);
    w->burstrate = (short)(parser->GetFieldDouble("burstrate", 0.0) * 30.0);
    w->sprayangle = (short)parser->GetFieldInt("sprayangle", 0);
    w->duration = (short)(parser->GetFieldDouble("duration", 0.0) * 30.0);
    w->randomdecay = (short)(parser->GetFieldDouble("randomdecay", 0.0) * 30.0);
    w->smokedelay = (short)(parser->GetFieldDouble("smokedelay", 0.0) * 30.0);
    w->flighttime = (short)(parser->GetFieldDouble("flighttime", 0.0) * 30.0);
    w->holdtime = (short)(parser->GetFieldDouble("holdtime", 0.0) * 30.0);
    // No (float) cast: it costs the scheduler one unit and shifts later bitfield stores.
    w->minbarrelangle =
        parser->GetFieldDouble("minbarrelangle", -11.25) * (PI / 180);
    w->firestarter = (unsigned char)parser->GetFieldInt("firestarter", 0);
    w->rendertype = (unsigned char)parser->GetFieldInt("rendertype", 0);
    w->color = (unsigned char)parser->GetFieldInt("color", 0);
    w->color2 = (unsigned char)parser->GetFieldInt("color2", 0);
    w->soundtrigger = parser->GetFieldInt("soundtrigger", 0);
    w->guidance = parser->GetFieldInt("guidance", 0);
    w->tracks = parser->GetFieldInt("tracks", 0);
    w->lineofsight = parser->GetFieldInt("lineofsight", 0);
    // Named local: makes the bitfield `or` take the shifted value as destination.
    int ballistic = parser->GetFieldInt("ballistic", 0);
    w->ballistic = ballistic;
    w->unitsonly = parser->GetFieldInt("unitsonly", 0);
    w->groundbounce = parser->GetFieldInt("groundbounce", 0);
    w->waterweapon = parser->GetFieldInt("waterweapon", 0);
    w->toairweapon = parser->GetFieldInt("toairweapon", 0);
    w->smoketrail = parser->GetFieldInt("smoketrail", 0);
    w->turret = parser->GetFieldInt("turret", 0);
    w->selfprop = parser->GetFieldInt("selfprop", 0);
    w->propeller = parser->GetFieldInt("propeller", 0);
    w->noexplode = parser->GetFieldInt("noexplode", 0);
    w->burnblow = parser->GetFieldInt("burnblow", 0);
    w->twophase = parser->GetFieldInt("twophase", 0);
    w->cruise = parser->GetFieldInt("cruise", 0);
    w->commandfire = parser->GetFieldInt("commandfire", 0);
    w->stockpile = parser->GetFieldInt("stockpile", 0);
    w->targetable = parser->GetFieldInt("targetable", 0);
    w->interceptor = parser->GetFieldInt("interceptor", 0);
    w->beamweapon = parser->GetFieldInt("beamweapon", 0);
    w->shellweapon = parser->GetFieldInt("shellweapon", 0);
    // Named local, as for ballistic.
    int dropped = parser->GetFieldInt("dropped", 0);
    w->dropped = dropped;
    w->vlaunch = parser->GetFieldInt("vlaunch", 0);
    w->meteor = parser->GetFieldInt("meteor", 0);
    w->noradar = parser->GetFieldInt("noradar", 0);
    w->paralyzer = parser->GetFieldInt("paralyzer", 0);
    w->startsmoke = parser->GetFieldInt("startsmoke", 0);
    w->endsmoke = parser->GetFieldInt("endsmoke", 0);
    w->accuracy = (short)parser->GetFieldInt("accuracy", 0);
    w->tolerance = (short)parser->GetFieldInt("tolerance", 0);
    w->pitchtolerance = (short)parser->GetFieldInt("pitchtolerance", 0);
    w->shakemagnitude = parser->GetFieldInt("shakemagnitude", 0);
    w->shakeduration = (int)(parser->GetFieldDouble("shakeduration", 0.0) * 30.0);

    char model[0x100];
    if (parser->GetFieldString(model, "model", 0x100, DAT_005119b8) != 0) {
        unsigned char i;
        unsigned char count = w->id;
        for (i = 0; i < count; i++) {
            if (_strcmpi(model, g_game->weapons[i].model) == 0) {
                g_game->weapons[count].model[0] = 0;
                g_game->weapons[count].text = g_game->weapons[i].text;
                goto model_done;
            }
        }
        char path[0x100];
        BuildDataPath(path, "objects3d", model, "3DO");
        void* h = Load3do(path);
        if (h == 0)
            FatalError(path);
        MirrorObject(h);
        BindModelTextures(h, model);
        g_game->weapons[count].text = h;
        strcpy(g_game->weapons[count].model, model);
    } else {
        w->text = 0;
    }
model_done:
    w->anim1 = 0;
    char gaf[0x100];
    if (parser->GetFieldString(gaf, "explosiongaf", 0x100, DAT_005119b8) != 0 &&
        parser->GetFieldString(model, "explosionart", 0x100, DAT_005119b8) != 0) {
        void* a = LoadAnimGaf(gaf);
        void* r = FindGafEntry(a, model);
        *(unsigned char*)((char*)r + 2) = 0;
        w->anim1 = r;
    }
    w->anim2 = 0;
    if (*(int*)(*(char**)((char*)g_game + 0x391e9) + 0xd44) != 0) {
        if (parser->GetFieldString(gaf, "lavaexplosiongaf", 0x100, DAT_005119b8) != 0 &&
            parser->GetFieldString(model, "lavaexplosionart", 0x100, DAT_005119b8) != 0) {
            void* a = LoadAnimGaf(gaf);
            void* r = FindGafEntry(a, model);
            *(unsigned char*)((char*)r + 2) = 0;
            w->anim2 = r;
        }
    } else {
        if (parser->GetFieldString(gaf, "waterexplosiongaf", 0x100, DAT_005119b8) != 0 &&
            parser->GetFieldString(model, "waterexplosionart", 0x100, DAT_005119b8) != 0) {
            void* a = LoadAnimGaf(gaf);
            void* r = FindGafEntry(a, model);
            *(unsigned char*)((char*)r + 2) = 0;
            w->anim2 = r;
        }
    }
    if (parser->GetFieldString(model, "soundstart", 0x100, DAT_005119b8) != 0) {
        w->soundstart = (unsigned short)LoadSoundByName(0, model);
    } else {
        w->soundstart = 0xffff;
    }
    if (parser->GetFieldString(model, "soundhit", 0x100, DAT_005119b8) != 0) {
        w->soundhit = (unsigned short)LoadSoundByName(0, model);
    } else {
        w->soundhit = 0xffff;
    }
    if (parser->GetFieldString(model, "soundwater", 0x100, DAT_005119b8) != 0) {
        w->soundwater = (unsigned short)LoadSoundByName(0, model);
    } else {
        w->soundwater = 0xffff;
    }
    TdfRecord* damage = parser->FindSubRecord("DAMAGE");
    if (damage != 0) {
        w->damage = (short)damage->GetFieldInt("default", 0);
        int index = 0;
        char* key = damage->GetFieldName(index);
        while (key) {
            if (_strcmpi(key, "default") != 0) {
                int value = damage->GetFieldInt(key, 0);
                if (!w->sub)
                    w->sub = new Map_0042e440;
                Class_004c91b0 name(key);
                Map_0042e440* m = w->sub;
                Entry_00432cf0* e = m->LowerBound(name.ptr);
                // insert called here, not in a helper (inline depth); e assigned
                // first, then &e->value taken in each arm.
                int* r;
                if (e == m->v.end() || Ne(e->name, name)) {
                    e = m->v.insert(e, Entry_00432cf0(name, 0));
                    r = &e->value;
                } else {
                    r = &e->value;
                }
                *r = value;
            }
            key = damage->GetFieldName(++index);
        }
    } else {
        w->damage = 0;
    }
    SetWeaponFireHandler(w);
}

// FUNCTION: 0x42f3a0
void FreeWeaponTypes()
{
    int i = 0;
    while (i < 256) {
        Weapon_0042e440* o = &g_game->weapons[i];
        if (strlen(o->model) != 0) {
            GameFreeThunk(o->text);
            o->text = 0;
            o->model[0] = 0;
        }
        if (o->sub) {
            delete o->sub;
            o->sub = 0;
        }
        i++;
    }
}
