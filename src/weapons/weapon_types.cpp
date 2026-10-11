// Decompiled by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1, finished by GPT-6.1-sol, finished by deepseek-v4.1-flash, edited by deepseek-v4.1-flash, finished by claude-opus-5.5, space-bunny-free. Names are provisional.

#include <string.h>
#include <vector>

#include "../util/tdf.h"
#include "weapon_def.h"

// 3.14159265358979 / 180 is exactly the exe's 0.017453292519943278.
#define PI 3.14159265358979

class StringRef {
  public:
    char* ptr;
    StringRef(const StringRef& other);
    StringRef(const char* text);
    StringRef(const char* text, int len);
    ~StringRef() { ReleaseRef(); }
    StringRef& operator=(const StringRef& other);
    void ReleaseRef();
    StringRef* Assign(const StringRef* other);
    StringRef* Append(const StringRef& other);
    StringRef* MakeLower();
    StringRef* MakeUpper();
    StringRef* AssignText(const char* text);
    char* GetUnique();
    StringRef SubString(int start, int end) const;
};

static inline bool operator==(const StringRef& a, const StringRef& b) {
    return strcmp(a.ptr, b.ptr) == 0;
}

static inline bool Ne(const StringRef& a, const StringRef& b) {
    return !(a == b);
}

static inline bool Less(const char* a, const char* b) {
    return _strcmpi(a, b) < 0;
}

// One entry of a weapon's damage table: a unit name and its damage.
struct Entry_00432cf0 {
    StringRef name; // +0x0
    int value;           // +0x4

    Entry_00432cf0(const StringRef& n, int v) : name(n), value(v) {}
};

#pragma pack(push, 1)
// The damage table, kept sorted by name.
struct Table_00499cd0 {
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

void __stdcall SetWeaponFireHandler(WeaponDef*);

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x2cf3];
    WeaponDef weaponDefs[0x100];  // +0x2cf3, stride 0x115
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

void __stdcall ListDirectory(const char* pattern, int flags, std::vector<StringRef>* out);
void __stdcall LoadWeaponType(TdfRecord* section);
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
        WeaponDef* p = &g_game->weaponDefs[i];
        p->index = n++;
        p->name[0] = 0;
    }

    char path[256];
    std::vector<StringRef> files;
    ListDirectory("Weapons\\*.tdf", 0, &files);

    for (StringRef* p = files.begin(); p < files.end(); p++) {
        TdfFile parser;
        BuildDataPath(path, "Weapons", p->ptr, "TDF");
        if (parser.LoadFile(path)
            && (parser.flags || FUN_0041d8a0() == 0)) {
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
// a table sorted by unit name (w->table).
// FUNCTION: 0x42e440
void __stdcall LoadWeaponType(TdfRecord* parser) {
    char* id = (char*)parser->GetRecordName();
    WeaponDef* w = &g_game->weaponDefs[parser->GetFieldInt("ID", -1)];
    strcpy(w->name, id);
    parser->GetFieldString(w->description, "name", 0x40, DAT_005119b8);

    w->speed =
        (int)(parser->GetFieldDouble("weaponvelocity", 0.0) * 2184.5333333333333);
    w->startSpeed =
        (int)(parser->GetFieldDouble("startvelocity", 0.0) * 2184.5333333333333);
    w->acceleration =
        (int)(parser->GetFieldDouble("weaponacceleration", 0.0) *
              72.81777777777778);
    // The short fields are unsigned short, so the casts are too: a (short) cast
    // stored to one changes the inlining further down.
    w->range = parser->GetFieldInt("range", 0x7fff);
    w->coverage = parser->GetFieldInt("coverage", 0);
    w->reloadTime = (unsigned short)(parser->GetFieldDouble("reloadtime", 0.0) * 30.0);
    w->energyPerShot = (float)parser->GetFieldDouble("energypershot", 0.0);
    w->metalPerShot = (float)parser->GetFieldDouble("metalpershot", 0.0);
    w->areaOfEffect = (unsigned short)parser->GetFieldInt("areaofeffect", 0);
    w->edgeDamage = (float)parser->GetFieldDouble("edgeeffectiveness", 0.0);
    w->lifetime = (unsigned short)(parser->GetFieldDouble("weapontimer", 0.0) * 30.0);
    w->flags.b.noAutoRange = parser->GetFieldInt("noautorange", 0);
    w->turnRate =
        (unsigned short)(parser->GetFieldDouble("turnrate", 0.0) * 0.03333333333333333);
    w->burst = (unsigned short)parser->GetFieldInt("burst", 0);
    w->burstRate = (unsigned short)(parser->GetFieldDouble("burstrate", 0.0) * 30.0);
    w->spread = (unsigned short)parser->GetFieldInt("sprayangle", 0);
    w->duration = (unsigned short)(parser->GetFieldDouble("duration", 0.0) * 30.0);
    w->lifeRand = (unsigned short)(parser->GetFieldDouble("randomdecay", 0.0) * 30.0);
    w->smokeRate = (unsigned short)(parser->GetFieldDouble("smokedelay", 0.0) * 30.0);
    w->flightTime = (unsigned short)(parser->GetFieldDouble("flighttime", 0.0) * 30.0);
    // +0xfe is the TDF holdtime in ticks; the blast code copies it to cameraSnapTicks.
    w->holdTime = (unsigned short)(parser->GetFieldDouble("holdtime", 0.0) * 30.0);
    // No (float) cast: it costs the scheduler one unit and shifts later bitfield stores.
    w->pitch =
        parser->GetFieldDouble("minbarrelangle", -11.25) * (PI / 180);
    w->fireStarter = (unsigned char)parser->GetFieldInt("firestarter", 0);
    w->shotKind = (unsigned char)parser->GetFieldInt("rendertype", 0);
    w->colour = (unsigned char)parser->GetFieldInt("color", 0);
    w->colour2 = (unsigned char)parser->GetFieldInt("color2", 0);
    w->flags.b.soundTrigger = parser->GetFieldInt("soundtrigger", 0);
    w->flags.b.guidance = parser->GetFieldInt("guidance", 0);
    w->flags.b.tracks = parser->GetFieldInt("tracks", 0);
    w->flags.b.lineOfSight = parser->GetFieldInt("lineofsight", 0);
    // Named local: makes the bitfield `or` take the shifted value as destination.
    int ballistic = parser->GetFieldInt("ballistic", 0);
    w->flags.b.ballistic = ballistic;
    w->flags.b.unitsOnly = parser->GetFieldInt("unitsonly", 0);
    w->flags.b.groundBounce = parser->GetFieldInt("groundbounce", 0);
    w->flags.b.waterWeapon = parser->GetFieldInt("waterweapon", 0);
    w->flags.b.toAirWeapon = parser->GetFieldInt("toairweapon", 0);
    w->flags.b.smokeTrail = parser->GetFieldInt("smoketrail", 0);
    w->flags.b.turret = parser->GetFieldInt("turret", 0);
    w->flags.b.selfProp = parser->GetFieldInt("selfprop", 0);
    w->flags.b.propeller = parser->GetFieldInt("propeller", 0);
    w->flags.b.noExplode = parser->GetFieldInt("noexplode", 0);
    w->flags.b.burnBlow = parser->GetFieldInt("burnblow", 0);
    w->flags.b.twoPhase = parser->GetFieldInt("twophase", 0);
    w->flags.b.cruise = parser->GetFieldInt("cruise", 0);
    w->flags.b.commandFire = parser->GetFieldInt("commandfire", 0);
    w->flags.b.stockpile = parser->GetFieldInt("stockpile", 0);
    w->flags.b.targetable = parser->GetFieldInt("targetable", 0);
    w->flags.b.interceptor = parser->GetFieldInt("interceptor", 0);
    w->flags.b.beamWeapon = parser->GetFieldInt("beamweapon", 0);
    w->flags.b.shellWeapon = parser->GetFieldInt("shellweapon", 0);
    // Named local, as for ballistic.
    int dropped = parser->GetFieldInt("dropped", 0);
    w->flags.b.dropped = dropped;
    w->flags.b.vLaunch = parser->GetFieldInt("vlaunch", 0);
    w->flags.b.meteor = parser->GetFieldInt("meteor", 0);
    w->flags.b.noRadar = parser->GetFieldInt("noradar", 0);
    w->flags.b.paralyzer = parser->GetFieldInt("paralyzer", 0);
    w->flags.b.startSmoke = parser->GetFieldInt("startsmoke", 0);
    w->flags.b.endSmoke = parser->GetFieldInt("endsmoke", 0);
    w->accuracy = (short)parser->GetFieldInt("accuracy", 0);
    w->tolerance = (unsigned short)parser->GetFieldInt("tolerance", 0);
    w->pitchTolerance = (unsigned short)parser->GetFieldInt("pitchtolerance", 0);
    w->shakeMagnitude = parser->GetFieldInt("shakemagnitude", 0);
    w->shakeDuration = (int)(parser->GetFieldDouble("shakeduration", 0.0) * 30.0);

    char model[0x100];
    if (parser->GetFieldString(model, "model", 0x100, DAT_005119b8) != 0) {
        unsigned char i;
        unsigned char count = w->index;
        for (i = 0; i < count; i++) {
            if (_strcmpi(model, g_game->weaponDefs[i].modelName) == 0) {
                g_game->weaponDefs[count].modelName[0] = 0;
                g_game->weaponDefs[count].model = g_game->weaponDefs[i].model;
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
        g_game->weaponDefs[count].model = h;
        strcpy(g_game->weaponDefs[count].modelName, model);
    } else {
        w->model = 0;
    }
model_done:
    w->explosionSeq = 0;
    char gaf[0x100];
    if (parser->GetFieldString(gaf, "explosiongaf", 0x100, DAT_005119b8) != 0 &&
        parser->GetFieldString(model, "explosionart", 0x100, DAT_005119b8) != 0) {
        void* a = LoadAnimGaf(gaf);
        void* r = FindGafEntry(a, model);
        *(unsigned char*)((char*)r + 2) = 0;
        w->explosionSeq = r;
    }
    w->splash = 0;
    if (*(int*)(*(char**)((char*)g_game + 0x391e9) + 0xd44) != 0) {
        if (parser->GetFieldString(gaf, "lavaexplosiongaf", 0x100, DAT_005119b8) != 0 &&
            parser->GetFieldString(model, "lavaexplosionart", 0x100, DAT_005119b8) != 0) {
            void* a = LoadAnimGaf(gaf);
            void* r = FindGafEntry(a, model);
            *(unsigned char*)((char*)r + 2) = 0;
            w->splash = r;
        }
    } else {
        if (parser->GetFieldString(gaf, "waterexplosiongaf", 0x100, DAT_005119b8) != 0 &&
            parser->GetFieldString(model, "waterexplosionart", 0x100, DAT_005119b8) != 0) {
            void* a = LoadAnimGaf(gaf);
            void* r = FindGafEntry(a, model);
            *(unsigned char*)((char*)r + 2) = 0;
            w->splash = r;
        }
    }
    if (parser->GetFieldString(model, "soundstart", 0x100, DAT_005119b8) != 0) {
        w->sound = (unsigned short)LoadSoundByName(0, model);
    } else {
        w->sound = 0xffff;
    }
    if (parser->GetFieldString(model, "soundhit", 0x100, DAT_005119b8) != 0) {
        w->soundHit = (unsigned short)LoadSoundByName(0, model);
    } else {
        w->soundHit = 0xffff;
    }
    if (parser->GetFieldString(model, "soundwater", 0x100, DAT_005119b8) != 0) {
        w->soundWater = (unsigned short)LoadSoundByName(0, model);
    } else {
        w->soundWater = 0xffff;
    }
    TdfRecord* damage = parser->FindSubRecord("DAMAGE");
    if (damage != 0) {
        w->damage = (unsigned short)damage->GetFieldInt("default", 0);
        int index = 0;
        char* key = damage->GetFieldName(index);
        while (key) {
            if (_strcmpi(key, "default") != 0) {
                int value = damage->GetFieldInt(key, 0);
                if (!w->table)
                    w->table = new Table_00499cd0;
                StringRef name(key);
                Table_00499cd0* m = w->table;
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
        WeaponDef* o = &g_game->weaponDefs[i];
        if (strlen(o->modelName) != 0) {
            GameFreeThunk(o->model);
            o->model = 0;
            o->modelName[0] = 0;
        }
        if (o->table) {
            delete o->table;
            o->table = 0;
        }
        i++;
    }
}
