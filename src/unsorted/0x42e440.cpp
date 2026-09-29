// Decompiled by deepseek-v4.1-flash. Names are provisional.
// PARTIAL 68.1%. The linear TDF field load (name/ID through the flags, model,
// environment explosion, sound fields) is transcribed and the 0x330 frame and
// prologue match. Remaining differences: (a) many flag/store/call schedules are
// swapped by one instruction (the store to [ebp+0x111] lands before vs after the
// next call's pushes), (b) the model block's local buffer offsets differ, and
// (c) the DAMAGE subtable (std::vector<UEntry> insertion, from 0x42ef7f onward)
// is not implemented, so the function returns early and the last ~0x380 bytes
// differ. Frame 0x330 = 0x30 scalars + model[0x100] at +0x40 + gaf[0x100] at
// +0x140 + path[0x128] at +0x240.

#include <string.h>

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

#pragma pack(push, 1)
struct Weapon_0042e440 {
    char name[0x20];                    // +0x000
    char name2[0x40];                   // +0x020
    char pad_60[4];                     // +0x060
    void* sub;                          // +0x064
    int weaponvelocity;                 // +0x068
    int startvelocity;                  // +0x06c
    int weaponacceleration;             // +0x070
    void* text;                         // +0x074
    void* anim1;                        // +0x078
    void* anim2;                        // +0x07c
    char model[0x40];                   // +0x080
    float energypershot;                // +0x0c0
    float metalpershot;                 // +0x0c4
    float minbarrelangle;               // +0x0c8
    int shakemagnitude;                 // +0x0cc
    int shakeduration;                  // +0x0d0
    short damage;                       // +0x0d4
    short areaofeffect;                 // +0x0d6
    float edgeeffectiveness;            // +0x0d8
    int range;                          // +0x0dc
    int coverage;                       // +0x0e0
    short reloadtime;                   // +0x0e4
    short weapontimer;                  // +0x0e6
    short turnrate;                     // +0x0e8
    short burst;                        // +0x0ea
    short burstrate;                    // +0x0ec
    short sprayangle;                   // +0x0ee
    short duration;                     // +0x0f0
    short randomdecay;                  // +0x0f2
    short soundstart;                   // +0x0f4
    short soundhit;                     // +0x0f6
    short soundwater;                   // +0x0f8
    short smokedelay;                   // +0x0fa
    short flighttime;                   // +0x0fc
    short holdtime;                     // +0x0fe
    char pad_100[4];                    // +0x100
    short accuracy;                     // +0x104
    short tolerance;                    // +0x106
    short pitchtolerance;               // +0x108
    unsigned char id;                   // +0x10a
    unsigned char firestarter;          // +0x10b
    unsigned char rendertype;           // +0x10c
    unsigned char color;                // +0x10d
    unsigned char color2;               // +0x10e
    unsigned char pad_10f[2];           // +0x10f
    unsigned int lineofsight : 1;       // +0x111 bit 0
    unsigned int ballistic : 1;         // bit 1
    unsigned int shellweapon : 1;       // bit 2
    unsigned int beamweapon : 1;        // bit 3
    unsigned int vlaunch : 1;           // bit 4
    unsigned int meteor : 1;            // bit 5
    unsigned int noradar : 1;           // bit 6
    unsigned int paralyzer : 1;         // bit 7
    unsigned int dropped : 1;           // bit 8
    unsigned int startsmoke : 1;        // bit 9
    unsigned int endsmoke : 1;          // bit 10
    unsigned int soundtrigger : 1;      // bit 11
    unsigned int guidance : 1;          // bit 12
    unsigned int tracks : 1;            // bit 13
    unsigned int unitsonly : 1;         // bit 14
    unsigned int groundbounce : 1;      // bit 15
    unsigned int waterweapon : 1;       // bit 16
    unsigned int toairweapon : 1;       // bit 17
    unsigned int smoketrail : 1;        // bit 18
    unsigned int turret : 1;            // bit 19
    unsigned int selfprop : 1;          // bit 20
    unsigned int propeller : 1;         // bit 21
    unsigned int noexplode : 1;         // bit 22
    unsigned int burnblow : 1;          // bit 23
    unsigned int twophase : 1;          // bit 24
    unsigned int cruise : 1;            // bit 25
    unsigned int commandfire : 1;       // bit 26
    unsigned int noautorange : 1;       // bit 27
    unsigned int stockpile : 1;         // bit 28
    unsigned int targetable : 1;        // bit 29
    unsigned int interceptor : 1;       // bit 30
    unsigned int : 1;                   // bit 31
};

struct Game_0042e440 {
    char unknown_0[0x2cf3];
    Weapon_0042e440 weapons[0x100];     // +0x2cf3, stride 0x115
};
#pragma pack(pop)

extern Game_0042e440* g_game;
extern char DAT_005119b8[];

void __stdcall FUN_004290f0(char* out, const char* dir, const char* name, const char* ext);
void __stdcall FUN_004b6290(char* path);
void* __stdcall FUN_004cb560(char* path);
void __stdcall FUN_004cb590(void* p);
void __stdcall FUN_0042a140(void* a, char* b);
void* __stdcall FUN_00429700(char* name);
void* __stdcall FUN_004b8d40(void* a, char* b);
int __stdcall FUN_00429470(void* a, char* b);

// FUNCTION: 0x42e440
void __stdcall FUN_0042e440(Class_004c4440* parser)
{
    char* id = parser->FUN_004c4440();
    Weapon_0042e440* w = &g_game->weapons[((Class_004c46c0*)parser)->FUN_004c46c0("ID", -1)];
    strcpy(w->name, id);
    ((Class_004c48c0*)parser)->FUN_004c48c0(w->name2, "name", 0x40, DAT_005119b8);

    w->weaponvelocity =
        (int)(((Class_004c4760*)parser)->FUN_004c4760("weaponvelocity", 0.0) * 2184.5333333333333);
    w->startvelocity =
        (int)(((Class_004c4760*)parser)->FUN_004c4760("startvelocity", 0.0) * 2184.5333333333333);
    w->weaponacceleration =
        (int)(((Class_004c4760*)parser)->FUN_004c4760("weaponacceleration", 0.0) * 72.81777777777778);
    w->range = ((Class_004c46c0*)parser)->FUN_004c46c0("range", 0x7fff);
    w->coverage = ((Class_004c46c0*)parser)->FUN_004c46c0("coverage", 0);
    w->reloadtime =
        (short)(((Class_004c4760*)parser)->FUN_004c4760("reloadtime", 0.0) * 30.0);
    w->energypershot =
        (float)((Class_004c4760*)parser)->FUN_004c4760("energypershot", 0.0);
    w->metalpershot =
        (float)((Class_004c4760*)parser)->FUN_004c4760("metalpershot", 0.0);
    w->areaofeffect = (short)((Class_004c46c0*)parser)->FUN_004c46c0("areaofeffect", 0);
    w->edgeeffectiveness =
        (float)((Class_004c4760*)parser)->FUN_004c4760("edgeeffectiveness", 0.0);
    w->weapontimer =
        (short)(((Class_004c4760*)parser)->FUN_004c4760("weapontimer", 0.0) * 30.0);
    w->noautorange = ((Class_004c46c0*)parser)->FUN_004c46c0("noautorange", 0);
    w->turnrate =
        (short)(((Class_004c4760*)parser)->FUN_004c4760("turnrate", 0.0) * 0.03333333333333333);
    w->burst = (short)((Class_004c46c0*)parser)->FUN_004c46c0("burst", 0);
    w->burstrate =
        (short)(((Class_004c4760*)parser)->FUN_004c4760("burstrate", 0.0) * 30.0);
    w->sprayangle = (short)((Class_004c46c0*)parser)->FUN_004c46c0("sprayangle", 0);
    w->duration = (short)(((Class_004c4760*)parser)->FUN_004c4760("duration", 0.0) * 30.0);
    w->randomdecay =
        (short)(((Class_004c4760*)parser)->FUN_004c4760("randomdecay", 0.0) * 30.0);
    w->smokedelay =
        (short)(((Class_004c4760*)parser)->FUN_004c4760("smokedelay", 0.0) * 30.0);
    w->flighttime =
        (short)(((Class_004c4760*)parser)->FUN_004c4760("flighttime", 0.0) * 30.0);
    w->holdtime =
        (short)(((Class_004c4760*)parser)->FUN_004c4760("holdtime", 0.0) * 30.0);
    w->minbarrelangle = (float)(((Class_004c4760*)parser)->FUN_004c4760("minbarrelangle", -11.25)
                                * 0.017453292519943278);
    w->firestarter = (unsigned char)((Class_004c46c0*)parser)->FUN_004c46c0("firestarter", 0);
    w->rendertype = (unsigned char)((Class_004c46c0*)parser)->FUN_004c46c0("rendertype", 0);
    w->color = (unsigned char)((Class_004c46c0*)parser)->FUN_004c46c0("color", 0);
    w->color2 = (unsigned char)((Class_004c46c0*)parser)->FUN_004c46c0("color2", 0);
    w->soundtrigger = ((Class_004c46c0*)parser)->FUN_004c46c0("soundtrigger", 0);
    w->guidance = ((Class_004c46c0*)parser)->FUN_004c46c0("guidance", 0);
    w->tracks = ((Class_004c46c0*)parser)->FUN_004c46c0("tracks", 0);
    w->lineofsight = ((Class_004c46c0*)parser)->FUN_004c46c0("lineofsight", 0);
    w->ballistic = ((Class_004c46c0*)parser)->FUN_004c46c0("ballistic", 0);
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
    w->dropped = ((Class_004c46c0*)parser)->FUN_004c46c0("dropped", 0);
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
    w->shakeduration =
        (int)(((Class_004c4760*)parser)->FUN_004c4760("shakeduration", 0.0) * 30.0);

    char model[0x100];
    if (((Class_004c48c0*)parser)->FUN_004c48c0(model, "model", 0x100, DAT_005119b8) == 0) {
        w->text = 0;
    } else {
        unsigned char count = w->id;
        unsigned char i = 0;
        if (count > 0) {
            do {
                if (_strcmpi(model, g_game->weapons[i].model) == 0) {
                    g_game->weapons[w->id].model[0] = 0;
                    g_game->weapons[w->id].text = g_game->weapons[i].text;
                    goto model_done;
                }
                i++;
            } while (i < count);
        }
        char path[0x128];
        FUN_004290f0(path, "objects3d", model, "3DO");
        void* h = FUN_004cb560(path);
        if (h == 0)
            FUN_004b6290(path);
        FUN_004cb590(h);
        FUN_0042a140(h, model);
        g_game->weapons[w->id].text = h;
        strcpy(g_game->weapons[w->id].model, model);
    }
model_done:
    w->anim1 = 0;
    char gaf[0x100];
    if (((Class_004c48c0*)parser)->FUN_004c48c0(gaf, "explosiongaf", 0x100, DAT_005119b8) != 0
        && ((Class_004c48c0*)parser)->FUN_004c48c0(model, "explosionart", 0x100, DAT_005119b8) != 0) {
        void* a = FUN_00429700(gaf);
        void* r = FUN_004b8d40(a, model);
        *(unsigned char*)((char*)r + 2) = 0;
        w->anim1 = r;
    }
    w->anim2 = 0;
    if (*(int*)((char*)g_game + 0x391e9) != 0) {
        if (((Class_004c48c0*)parser)->FUN_004c48c0(gaf, "lavaexplosiongaf", 0x100, DAT_005119b8) != 0
            && ((Class_004c48c0*)parser)->FUN_004c48c0(model, "lavaexplosionart", 0x100, DAT_005119b8) != 0) {
            void* a = FUN_00429700(gaf);
            void* r = FUN_004b8d40(a, model);
            *(unsigned char*)((char*)r + 2) = 0;
            w->anim2 = r;
        }
    } else {
        if (((Class_004c48c0*)parser)->FUN_004c48c0(gaf, "waterexplosiongaf", 0x100, DAT_005119b8) != 0
            && ((Class_004c48c0*)parser)->FUN_004c48c0(model, "waterexplosionart", 0x100, DAT_005119b8) != 0) {
            void* a = FUN_00429700(gaf);
            void* r = FUN_004b8d40(a, model);
            *(unsigned char*)((char*)r + 2) = 0;
            w->anim2 = r;
        }
    }
    if (((Class_004c48c0*)parser)->FUN_004c48c0(model, "soundstart", 0x100, DAT_005119b8) == 0) {
        w->soundstart = (short)0xffff;
    } else {
        w->soundstart = (short)FUN_00429470(0, model);
    }
    if (((Class_004c48c0*)parser)->FUN_004c48c0(model, "soundhit", 0x100, DAT_005119b8) == 0) {
        w->soundhit = (short)0xffff;
    } else {
        w->soundhit = (short)FUN_00429470(0, model);
    }
    if (((Class_004c48c0*)parser)->FUN_004c48c0(model, "soundwater", 0x100, DAT_005119b8) == 0) {
        w->soundwater = (short)0xffff;
    } else {
        w->soundwater = (short)FUN_00429470(0, model);
    }
    return;
}
