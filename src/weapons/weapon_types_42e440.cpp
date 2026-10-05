// Decompiled by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1, finished by GPT-6.1-sol, finished by deepseek-v4.1-flash, edited by deepseek-v4.1-flash, finished by claude-opus-5.5. Names are provisional.
// Loads one weapon from a .TDF section: the numbers and flags, the model, the
// explosion animations, the sounds and the DAMAGE sub-section, which goes into
// a table sorted by unit name (w->sub).
//
// The bytes match (claude-opus-5.5, #4194, from 84.7%). Three changes did it:
//
// 1. 84.7% to 99.1%: the DAMAGE table is written on the real <vector>, in
//    the shape of the matched TDF section map in 0x4c3e40 and 0x4c54f0 (a
//    byte, then a std::vector of {name handle, int} at +1, a first != last
//    binary search taking the key's char* by value, and the
//    `e == end || Ne(e->name, key)` test). The old hand-made vector could not
//    get the signed `(last - first) / 2`, the materialised `!(a == b)` or the
//    inline decisions right. The insert must be called in the function body:
//    `m->v.insert(e, Entry(name, 0))` at depth 1 leaves 555 budget for
//    insert(P, 1, X)'s own sites, so the first two arms are inlined and the
//    third arm's _Ucopy, copy_backward and fill stay out of line, as in the
//    original (inside an InsertNew helper it is one level deeper: 71.7%).
//    Assigning `e` first and taking `&e->value` in each arm puts the
//    temporary's destructor before the `lea esi, [ebx+ecx*8+4]` (98.9% with
//    `&insert(...)->value`).
//
// 2. 99.1% to 99.6%: ballistic and dropped are read into named locals. That
//    makes their `or` take the shifted value as its destination (`or eax,
//    ecx`, then the store from eax), as in the original; the permuter found
//    the same. Without them those two statements differ.
//
// 3. 99.6% to 100%: minbarrelangle is assigned without a `(float)` cast. The
//    old `float mba = (float)(...); w->minbarrelangle = mba;` (and a plain
//    `(float)` cast) put the fstp in the same place but cost the scheduler
//    one extra unit, and the scheduler treats every 8th bitfield statement
//    after it differently depending on that count (tracks, turret and
//    stockpile were one step off). Found by deleting each earlier statement
//    and putting back as many one-instruction stores: only this statement
//    did not come back to the same shapes.
//
// Names: the out-of-line callees of the inlined vector::insert are real
// <vector>/<algorithm> instantiations that data/symbols.csv knows by other
// names: 0x432cf0 is std::_Construct<Entry_00432cf0, Entry_00432cf0> (ecx is
// never set at its call sites; symbols.csv says allocator::construct, which
// compiles to the same bytes), 0x432c20 is ??_GEntry_00432cf0 (the scalar
// deleting destructor, called with 0 from _Destroy), 0x432d20 is
// Entry_00432cf0::operator=, 0x432c80 is std::fill and 0x432cb0 is
// std::copy_backward. data/aliases.csv lets these names reach them.
#include <string.h>
#include <vector>

// 3.14159265358979 / 180 is exactly the exe's 0.017453292519943278.
#define PI 3.14159265358979

class Class_004c4440 {
  public:
    char* FUN_004c4440();
};

class Class_004c46c0 {
  public:
    int FUN_004c46c0(const char* key, int def);
};

class Class_004c4760 {
  public:
    double FUN_004c4760(const char* key, double def);
};

class Class_004c48c0 {
  public:
    int FUN_004c48c0(char* dst, const char* key, int size, char* def);
};

class Class_004c4470 {
  public:
    void* FUN_004c4470(const char* key);
};

class Class_004c45e0 {
  public:
    char* FUN_004c45e0(int index);
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

void __stdcall FUN_0049e010(void*);


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

void __stdcall FUN_004290f0(char* out, const char* dir, const char* name, const char* ext);
void __stdcall FatalError(char* path);
void* __stdcall Load3do(char* path);
void __stdcall MirrorObject(void* p);
void __stdcall FUN_0042a140(void* a, char* b);
void* __stdcall FUN_00429700(char* name);
void* __stdcall FindGafEntry(void* a, char* b);
int __stdcall FUN_00429470(void* a, char* b);

// FUNCTION: 0x42e440
void __stdcall FUN_0042e440(Class_004c4440* parser) {
    char* id = parser->FUN_004c4440();
    Weapon_0042e440* w = &g_game->weapons[((Class_004c46c0*)parser)->FUN_004c46c0("ID", -1)];
    strcpy(w->name, id);
    ((Class_004c48c0*)parser)->FUN_004c48c0(w->name2, "name", 0x40, DAT_005119b8);

    w->weaponvelocity =
        (int)(((Class_004c4760*)parser)->FUN_004c4760("weaponvelocity", 0.0) * 2184.5333333333333);
    w->startvelocity =
        (int)(((Class_004c4760*)parser)->FUN_004c4760("startvelocity", 0.0) * 2184.5333333333333);
    w->weaponacceleration =
        (int)(((Class_004c4760*)parser)->FUN_004c4760("weaponacceleration", 0.0) *
              72.81777777777778);
    w->range = ((Class_004c46c0*)parser)->FUN_004c46c0("range", 0x7fff);
    w->coverage = ((Class_004c46c0*)parser)->FUN_004c46c0("coverage", 0);
    w->reloadtime = (short)(((Class_004c4760*)parser)->FUN_004c4760("reloadtime", 0.0) * 30.0);
    w->energypershot = (float)((Class_004c4760*)parser)->FUN_004c4760("energypershot", 0.0);
    w->metalpershot = (float)((Class_004c4760*)parser)->FUN_004c4760("metalpershot", 0.0);
    w->areaofeffect = (short)((Class_004c46c0*)parser)->FUN_004c46c0("areaofeffect", 0);
    w->edgeeffectiveness = (float)((Class_004c4760*)parser)->FUN_004c4760("edgeeffectiveness", 0.0);
    w->weapontimer = (short)(((Class_004c4760*)parser)->FUN_004c4760("weapontimer", 0.0) * 30.0);
    w->noautorange = ((Class_004c46c0*)parser)->FUN_004c46c0("noautorange", 0);
    w->turnrate =
        (short)(((Class_004c4760*)parser)->FUN_004c4760("turnrate", 0.0) * 0.03333333333333333);
    w->burst = (short)((Class_004c46c0*)parser)->FUN_004c46c0("burst", 0);
    w->burstrate = (short)(((Class_004c4760*)parser)->FUN_004c4760("burstrate", 0.0) * 30.0);
    w->sprayangle = (short)((Class_004c46c0*)parser)->FUN_004c46c0("sprayangle", 0);
    w->duration = (short)(((Class_004c4760*)parser)->FUN_004c4760("duration", 0.0) * 30.0);
    w->randomdecay = (short)(((Class_004c4760*)parser)->FUN_004c4760("randomdecay", 0.0) * 30.0);
    w->smokedelay = (short)(((Class_004c4760*)parser)->FUN_004c4760("smokedelay", 0.0) * 30.0);
    w->flighttime = (short)(((Class_004c4760*)parser)->FUN_004c4760("flighttime", 0.0) * 30.0);
    w->holdtime = (short)(((Class_004c4760*)parser)->FUN_004c4760("holdtime", 0.0) * 30.0);
    w->minbarrelangle =
        ((Class_004c4760*)parser)->FUN_004c4760("minbarrelangle", -11.25) * (PI / 180);
    w->firestarter = (unsigned char)((Class_004c46c0*)parser)->FUN_004c46c0("firestarter", 0);
    w->rendertype = (unsigned char)((Class_004c46c0*)parser)->FUN_004c46c0("rendertype", 0);
    w->color = (unsigned char)((Class_004c46c0*)parser)->FUN_004c46c0("color", 0);
    w->color2 = (unsigned char)((Class_004c46c0*)parser)->FUN_004c46c0("color2", 0);
    w->soundtrigger = ((Class_004c46c0*)parser)->FUN_004c46c0("soundtrigger", 0);
    w->guidance = ((Class_004c46c0*)parser)->FUN_004c46c0("guidance", 0);
    w->tracks = ((Class_004c46c0*)parser)->FUN_004c46c0("tracks", 0);
    w->lineofsight = ((Class_004c46c0*)parser)->FUN_004c46c0("lineofsight", 0);
    int ballistic = ((Class_004c46c0*)parser)->FUN_004c46c0("ballistic", 0);
    w->ballistic = ballistic;
    w->unitsonly = ((Class_004c46c0*)parser)->FUN_004c46c0("unitsonly", 0);
    w->groundbounce = ((Class_004c46c0*)parser)->FUN_004c46c0("groundbounce", 0);
    w->waterweapon = ((Class_004c46c0*)parser)->FUN_004c46c0("waterweapon", 0);
    w->toairweapon = ((Class_004c46c0*)parser)->FUN_004c46c0("toairweapon", 0);
    w->smoketrail = ((Class_004c46c0*)parser)->FUN_004c46c0("smoketrail", 0);
    w->turret = ((Class_004c46c0*)parser)->FUN_004c46c0("turret", 0);
    w->selfprop = ((Class_004c46c0*)parser)->FUN_004c46c0("selfprop", 0);
    w->propeller = ((Class_004c46c0*)parser)->FUN_004c46c0("propeller", 0);
    w->noexplode = ((Class_004c46c0*)parser)->FUN_004c46c0("noexplode", 0);
    w->burnblow = ((Class_004c46c0*)parser)->FUN_004c46c0("burnblow", 0);
    w->twophase = ((Class_004c46c0*)parser)->FUN_004c46c0("twophase", 0);
    w->cruise = ((Class_004c46c0*)parser)->FUN_004c46c0("cruise", 0);
    w->commandfire = ((Class_004c46c0*)parser)->FUN_004c46c0("commandfire", 0);
    w->stockpile = ((Class_004c46c0*)parser)->FUN_004c46c0("stockpile", 0);
    w->targetable = ((Class_004c46c0*)parser)->FUN_004c46c0("targetable", 0);
    w->interceptor = ((Class_004c46c0*)parser)->FUN_004c46c0("interceptor", 0);
    w->beamweapon = ((Class_004c46c0*)parser)->FUN_004c46c0("beamweapon", 0);
    w->shellweapon = ((Class_004c46c0*)parser)->FUN_004c46c0("shellweapon", 0);
    int dropped = ((Class_004c46c0*)parser)->FUN_004c46c0("dropped", 0);
    w->dropped = dropped;
    w->vlaunch = ((Class_004c46c0*)parser)->FUN_004c46c0("vlaunch", 0);
    w->meteor = ((Class_004c46c0*)parser)->FUN_004c46c0("meteor", 0);
    w->noradar = ((Class_004c46c0*)parser)->FUN_004c46c0("noradar", 0);
    w->paralyzer = ((Class_004c46c0*)parser)->FUN_004c46c0("paralyzer", 0);
    w->startsmoke = ((Class_004c46c0*)parser)->FUN_004c46c0("startsmoke", 0);
    w->endsmoke = ((Class_004c46c0*)parser)->FUN_004c46c0("endsmoke", 0);
    w->accuracy = (short)((Class_004c46c0*)parser)->FUN_004c46c0("accuracy", 0);
    w->tolerance = (short)((Class_004c46c0*)parser)->FUN_004c46c0("tolerance", 0);
    w->pitchtolerance = (short)((Class_004c46c0*)parser)->FUN_004c46c0("pitchtolerance", 0);
    w->shakemagnitude = ((Class_004c46c0*)parser)->FUN_004c46c0("shakemagnitude", 0);
    w->shakeduration = (int)(((Class_004c4760*)parser)->FUN_004c4760("shakeduration", 0.0) * 30.0);

    char model[0x100];
    if (((Class_004c48c0*)parser)->FUN_004c48c0(model, "model", 0x100, DAT_005119b8) != 0) {
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
        FUN_004290f0(path, "objects3d", model, "3DO");
        void* h = Load3do(path);
        if (h == 0)
            FatalError(path);
        MirrorObject(h);
        FUN_0042a140(h, model);
        g_game->weapons[count].text = h;
        strcpy(g_game->weapons[count].model, model);
    } else {
        w->text = 0;
    }
model_done:
    w->anim1 = 0;
    char gaf[0x100];
    if (((Class_004c48c0*)parser)->FUN_004c48c0(gaf, "explosiongaf", 0x100, DAT_005119b8) != 0 &&
        ((Class_004c48c0*)parser)->FUN_004c48c0(model, "explosionart", 0x100, DAT_005119b8) != 0) {
        void* a = FUN_00429700(gaf);
        void* r = FindGafEntry(a, model);
        *(unsigned char*)((char*)r + 2) = 0;
        w->anim1 = r;
    }
    w->anim2 = 0;
    if (*(int*)(*(char**)((char*)g_game + 0x391e9) + 0xd44) != 0) {
        if (((Class_004c48c0*)parser)->FUN_004c48c0(gaf, "lavaexplosiongaf", 0x100, DAT_005119b8) !=
                0 &&
            ((Class_004c48c0*)parser)
                    ->FUN_004c48c0(model, "lavaexplosionart", 0x100, DAT_005119b8) != 0) {
            void* a = FUN_00429700(gaf);
            void* r = FindGafEntry(a, model);
            *(unsigned char*)((char*)r + 2) = 0;
            w->anim2 = r;
        }
    } else {
        if (((Class_004c48c0*)parser)
                    ->FUN_004c48c0(gaf, "waterexplosiongaf", 0x100, DAT_005119b8) != 0 &&
            ((Class_004c48c0*)parser)
                    ->FUN_004c48c0(model, "waterexplosionart", 0x100, DAT_005119b8) != 0) {
            void* a = FUN_00429700(gaf);
            void* r = FindGafEntry(a, model);
            *(unsigned char*)((char*)r + 2) = 0;
            w->anim2 = r;
        }
    }
    if (((Class_004c48c0*)parser)->FUN_004c48c0(model, "soundstart", 0x100, DAT_005119b8) != 0) {
        w->soundstart = (unsigned short)FUN_00429470(0, model);
    } else {
        w->soundstart = 0xffff;
    }
    if (((Class_004c48c0*)parser)->FUN_004c48c0(model, "soundhit", 0x100, DAT_005119b8) != 0) {
        w->soundhit = (unsigned short)FUN_00429470(0, model);
    } else {
        w->soundhit = 0xffff;
    }
    if (((Class_004c48c0*)parser)->FUN_004c48c0(model, "soundwater", 0x100, DAT_005119b8) != 0) {
        w->soundwater = (unsigned short)FUN_00429470(0, model);
    } else {
        w->soundwater = 0xffff;
    }
    void* damage = ((Class_004c4470*)parser)->FUN_004c4470("DAMAGE");
    if (damage != 0) {
        w->damage = (short)((Class_004c46c0*)damage)->FUN_004c46c0("default", 0);
        int index = 0;
        char* key = ((Class_004c45e0*)damage)->FUN_004c45e0(index);
        while (key) {
            if (_strcmpi(key, "default") != 0) {
                int value = ((Class_004c46c0*)damage)->FUN_004c46c0(key, 0);
                if (!w->sub)
                    w->sub = new Map_0042e440;
                Class_004c91b0 name(key);
                Map_0042e440* m = w->sub;
                Entry_00432cf0* e = m->LowerBound(name.ptr);
                int* r;
                if (e == m->v.end() || Ne(e->name, name)) {
                    e = m->v.insert(e, Entry_00432cf0(name, 0));
                    r = &e->value;
                } else {
                    r = &e->value;
                }
                *r = value;
            }
            key = ((Class_004c45e0*)damage)->FUN_004c45e0(++index);
        }
    } else {
        w->damage = 0;
    }
    FUN_0049e010(w);
}
