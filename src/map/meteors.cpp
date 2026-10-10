// Decompiled by Opus, Haiku, Claude Opus 5.5 and DeepSeek V4.1 Flash. Names are provisional.

#include <stdlib.h>
#include <string.h>
#include <memory>

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x14233];
    int mapWidthTiles;               // +0x14233
    int mapHeightTiles;              // +0x14237
    char unknown_1423b[0x38a47 - 0x1423b];
    // UpdateMeteors reads this as the tick counter, StartMeteorShower as a
    // plain int.
    union {
        unsigned int gameTick;       // +0x38a47
        int field_38a47;             // +0x38a47
    };
};
#pragma pack(pop)

// The 0x115-byte weapon definition, its flags read as a byte here.
#include "../weapons/weapon_def.h"

#include "../util/vec3.h"

struct Vec3_00437de0 {
    int x;
    int y;
    int z;
};

#include "../util/hapi_bank.h"

#include "../util/tdf.h"


class MeteorParams {
public:
    char name[0x20];                    // +0x0
    int radius;                         // +0x20
    float density;                      // +0x24
    float duration;                     // +0x28
    float interval;                     // +0x2c
    void LoadMeteorDefaults();
};

// Not std::vector: its destructor would add a dead element-destroy store.
template <class T, class A = std::allocator<T> >
class Vector_00438480 {
public:
    explicit Vector_00438480(const A& al = A())
        : allocator(al), first(0), last(0), end(0) {}
    ~Vector_00438480()
    {
        allocator.deallocate(first, end - first);
        first = 0, last = 0, end = 0;
    }

    A allocator;
    T* first;
    T* last;
    T* end;
};

extern char g_extTdf[];
extern char DAT_005119b8[];
extern char g_meteorWeaponName[];
extern int g_meteorSpawnMagnitude;     // strike radius
extern int g_meteorSpawnInterval;      // ticks between meteors
extern int g_meteorShowerDuration;
// Declared before g_meteorScheduleGap on purpose: in the sum the operand with the
// larger symbol id goes first, and the original computes it in edx.
extern int g_meteorStrikeEndTime;      // time the strike ends
extern int g_meteorScheduleGap;
extern int g_meteorActive;             // shower active
extern int g_meteorNextStrikeTime;     // next strike time
extern int g_meteorsEnabled;           // enabled
extern int g_meteorNextHitTime;        // next meteor time
extern WeaponDef* g_meteorWeapon;        // the meteor weapon definition
extern Game* g_game;
extern Point16 g_meteorOrigin;         // origin
extern Point16 g_meteorTarget;         // target

WeaponDef* __stdcall FindWeaponByName(char* name);
int __cdecl FUN_004b70ef(short angle, int scale);
int __cdecl FUN_004b7123(short angle, int scale);
int __stdcall SpawnProjectile(void* player, Vec3_00437de0* pos, Vec3_00437de0* vel, int count);
void __stdcall BuildDataPath(char* out, const char* dir, const char* name, const char* ext);
void __stdcall FatalError(char* text);

// FUNCTION: 0x437cd0
void InitMeteors()
{
    g_meteorActive = 0;
    g_meteorNextStrikeTime = g_meteorSpawnInterval;
    g_meteorWeapon = FindWeaponByName(g_meteorWeaponName);
    if (g_meteorWeapon == 0) {
        g_meteorWeapon = (WeaponDef*)((char*)g_game + 0x2cf3);
        return;
    }
    if (!(g_meteorWeapon->flags.flags8 & 0x20))
        g_meteorWeapon = (WeaponDef*)((char*)g_game + 0x2cf3);
}

// FUNCTION: 0x437d30
void EmptyShutdownPreCleanup(void)
{
}

// FUNCTION: 0x437d40
void EnableMeteors()
{
    g_meteorsEnabled = 1;
}

// FUNCTION: 0x437d50
void DisableMeteors()
{
    g_meteorsEnabled = 0;
}

// FUNCTION: 0x437d60
void __stdcall SetMeteorParams(MeteorParams* p)
{
    strcpy(g_meteorWeaponName, p->name);
    g_meteorSpawnMagnitude = p->radius;
    g_meteorSpawnInterval = (int)(30.0f / p->density);
    g_meteorShowerDuration = (int)(p->duration * 30.0f);
    g_meteorScheduleGap = (int)(p->interval * 30.0f);
}

// C-style helpers returning the struct by value; with a constructor and
// operator+= the offset's x never goes through the stack slot as it does here.
static inline Point16 MakePoint(int x, int y)
{
    Point16 p;
    p.x = x;
    p.y = y;
    return p;
}

static inline Point16 AddPoints(Point16 a, Point16 b)
{
    Point16 r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    return r;
}

// Inline copy of StartMeteorShower (0x438070): starts a shower.
static inline void StartShower()
{
    g_meteorActive = 1;
    g_meteorStrikeEndTime = g_meteorShowerDuration + g_game->gameTick;
    g_meteorNextStrikeTime = g_meteorScheduleGap + g_meteorStrikeEndTime;
    g_meteorNextHitTime = g_game->gameTick;
    g_meteorTarget = MakePoint((int)((__int64)rand() * g_game->mapWidthTiles / 0x8000),
                             (int)((__int64)rand() * g_game->mapHeightTiles / 0x8000));
    g_meteorOrigin = AddPoints(MakePoint((int)((__int64)rand() * 30 / 0x8000) - 15,
                                       (int)((__int64)rand() * 10 / 0x8000) - 15),
                             g_meteorTarget);
}

// Meteor shower update, run once per game tick: starts a new shower when
// its time comes (an inline copy of StartMeteorShower), and while one is active
// drops a meteor every g_meteorSpawnInterval ticks from a random point around the
// origin, high up, with a velocity that carries it to the target in 90
// ticks.
// FUNCTION: 0x437de0
void UpdateMeteors()
{
    if (g_meteorNextStrikeTime <= g_game->gameTick) {
        StartShower();
        if (g_meteorsEnabled == 0)
            g_meteorActive = 0;
    }
    if (g_meteorActive != 0) {
        if (g_meteorNextHitTime <= g_game->gameTick) {
            g_meteorNextHitTime = g_meteorSpawnInterval + g_game->gameTick;
            Vec3_00437de0 vel;
            vel.x = ((g_meteorTarget.x - g_meteorOrigin.x) << 20) / 90;
            vel.y = -15 << 16;
            vel.z = ((g_meteorTarget.y - g_meteorOrigin.y) << 20) / 90;
            int r = (int)((__int64)rand() * g_meteorSpawnMagnitude / 0x8000);
            // The radius shift is its own statement.
            int radius = r << 16;
            int angle = (int)((__int64)rand() * 0x10000 / 0x8000);
            Vec3_00437de0 offset;
            offset.x = -FUN_004b70ef(angle, radius);
            offset.y = 0;
            offset.z = -FUN_004b7123(angle, radius);
            // The offset is copied into pos as a struct: keeps the first stores of pos.x, pos.y.
            Vec3_00437de0 pos = offset;
            pos.x += g_meteorOrigin.x << 20;
            pos.y = -vel.y * 90;
            pos.z += g_meteorOrigin.y << 20;
            SpawnProjectile(g_meteorWeapon, &pos, &vel, 1);
        }
        if (g_meteorStrikeEndTime <= g_game->gameTick)
            g_meteorActive = 0;
    }
}

// FUNCTION: 0x438070
void StartMeteorShower()
{
    g_meteorActive = 1;
    g_meteorStrikeEndTime = g_meteorShowerDuration + g_game->field_38a47;
    g_meteorNextStrikeTime = g_meteorScheduleGap + g_meteorStrikeEndTime;
    g_meteorNextHitTime = g_game->field_38a47;
    g_meteorTarget = MakePoint((int)((__int64)rand() * g_game->mapWidthTiles / 0x8000),
                             (int)((__int64)rand() * g_game->mapHeightTiles / 0x8000));
    g_meteorOrigin = AddPoints(MakePoint((int)((__int64)rand() * 30 / 0x8000) - 15,
                                       (int)((__int64)rand() * 10 / 0x8000) - 15),
                             g_meteorTarget);
}

// Writes the meteor shower state to the "Meteor" section of a parsed text file.
// FUNCTION: 0x438180
void __stdcall SaveMeteors(HapiBank* file)
{
    file->OpenAccount("Meteor");
    file->SetIntegerItem("Enabled", g_meteorsEnabled);
    file->SetIntegerItem("Active", g_meteorActive);
    file->SetIntegerItem("Next Strike Time", g_meteorNextStrikeTime);
    file->SetIntegerItem("Time Strike Ends", g_meteorStrikeEndTime);
    file->SetIntegerItem("Next Hit Time", g_meteorNextHitTime);
    file->SetIntegerItem("Origin X", g_meteorOrigin.x);
    file->SetIntegerItem("Origin Z", g_meteorOrigin.y);
    file->SetIntegerItem("Target X", g_meteorTarget.x);
    file->SetIntegerItem("Target Z", g_meteorTarget.y);
}

// Reads the "Meteor" section into the meteor weapon globals.
// FUNCTION: 0x438250
void __stdcall LoadMeteors(HapiBank* file)
{
    file->OpenAccount("Meteor");
    g_meteorsEnabled = file->GetIntegerItem("Enabled", 0);
    g_meteorActive = file->GetIntegerItem("Active", 0);
    g_meteorNextStrikeTime = file->GetIntegerItem("Next Strike Time", 0);
    g_meteorStrikeEndTime = file->GetIntegerItem("Time Strike Ends", 0);
    g_meteorNextHitTime = file->GetIntegerItem("Next Hit Time", 0);
    g_meteorOrigin.x = file->GetIntegerItem("Origin X", 0);
    g_meteorOrigin.y = file->GetIntegerItem("Origin Z", 0);
    g_meteorTarget.x = file->GetIntegerItem("Target X", 0);
    g_meteorTarget.y = file->GetIntegerItem("Target Z", 0);
}

// FUNCTION: 0x438320
void MeteorParams::LoadMeteorDefaults()
{
    TdfFile parser;
    char path[256];
    BuildDataPath(path, "gamedata", "meteor", g_extTdf);
    if (parser.LoadFile(path)
        && parser.SelectRecord("Default")) {
        if (parser.current->GetFieldString((char*)this, "MeteorWeapon", 0x20, DAT_005119b8)) {
            radius = parser.current->GetFieldInt("MeteorRadius", 0);
            density = (float)parser.current->GetFieldDouble("MeteorDensity", 0.0);
            duration = (float)parser.current->GetFieldDouble("MeteorDuration", 0.0);
            float intervalTime = (float)parser.current->GetFieldDouble("MeteorInterval", 0.0);
            interval = intervalTime;
            if (radius != 0 && density != 0.0f && duration != 0.0f && intervalTime != 0.0f)
                return;
        }
        FatalError("Hey, hoser!  The default meteor shower data was bogus!");
    }
}

// The global at 0x512340, its initialiser (0x438450) and the destructor the
// compiler registers for it with atexit (0x438480). It is laid out like
// std::vector (an empty allocator, then first/last/end), but it is not one.
// This destructor only frees the storage.
// FUNCTION: 0x438450 _$E4
// FUNCTION: 0x438480 _$E2
Vector_00438480<int> g_missionOrderTableVec;
