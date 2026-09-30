// Decompiled by Claude Opus 5.5. Names are provisional.
// Retry #1764: GPT-6.1-sol confirmed 89.5% after three worker checks; no MATCH. Constant-register selection, delayed x87 stores and later register ordering remain different.
// Finished by GPT-6.1-sol.
// GPT-6 retry: chained/reset-helper field initialization and copying unset
// values through the reset fields did not improve 89.5%. Preserve this version;
// the zero/minus-one register choices and delayed x87 stores still differ.
// Loads the current mission: resets the mission state, finds the mission's
// OTA file (from the campaign list entry MISSION<n> for type 1, or from the
// map name for types 2 and 3), reads its GlobalHeader block into the fields
// and the name slots (0x435430.cpp), and passes the schema to 0x436c30.
// Returns 1 on success, 0 after reporting an error. 0x435320 and 0x4356c0
// are inlined.
//
// Partial (89.5%). Still differs:
// - Constant registers: the original keeps 0 in ebx and -1 in esi (which
//   first holds the old g_game+0x391ed object); here 0 lands in esi and -1
//   is not kept in a register. A scratch copy with one extra `= 0` and one
//   extra `= -1` field store gets exactly the original's assignment, so the
//   original has a little more weight on both constants than this source;
//   the N-declarations sweep and every header set leave it unchanged.
// - With that, `name`/`size` in the inlined 0x435320 come out in edi/ebx
//   instead of esi/edi.
// - x87 scheduling: after the killmul, timemul, MeteorDensity and
//   MeteorDuration calls the original delays the fstp until after the next
//   call's `mov ecx; push 0`; here it follows the call directly. This
//   compiler does that whenever the next call also returns a double
//   (a double call after tidalstrength moves its fstp up too), and the
//   matched 0x438320 shows the same early fstp for the same calls.
// Tried without effect: casts replaced by typed members and base classes,
// inline setters for the reset, int or float spellings of the -1 and 0.0
// constants, a pointer local for the list, inline wrappers for the getters.
//
// Checked by deepseek-v4.1-flash (baseline 89.5%): the whole diff is the
// constant-register choice. The original keeps ebx = 0 and esi = -1 pinned for
// the entire body (`xor ebx, ebx` at the top, `or esi, 0xffffffff` after the
// delete), so `cmp eax, ebx`, `push ebx` and `mov [this+0xa04], ebx` appear
// where this source emits `test eax, eax`, `push esi` and a later store via
// esi. Every remaining hunk is that register rename plus the shortened
// near-jump offsets it causes. Changing case 0/default from `return 0` to
// `break` (to force a 4-entry table) scored 84.7% and grew the body to 2808
// bytes, so keep `return 0`.
#include <windows.h>
#include <stdio.h>
#include <string.h>

#pragma pack(push, 1)
struct Game_00435da0 {
    char unknown_0[0x519];
    char messages[0x37ee6 - 0x519];    // +0x519
    short maxUnits;                    // +0x37ee6
    char unknown_37ee8[0x39073 - 0x37ee8];
    int noMovie;                       // +0x39073
    char unknown_39077[0x391ed - 0x39077];
    struct Class_0048df90* field_391ed; // +0x391ed
    char unknown_391f1[0x39219 - 0x391f1];
    int field_39219;                   // +0x39219
    int mapping;                       // +0x3921d
    int lineOfSight;                   // +0x39221
    int field_39225;                   // +0x39225
};
#pragma pack(pop)

extern Game_00435da0* g_game;
extern char DAT_005119b8[];

class Class_004c46c0 {
public:
    int FUN_004c46c0(const char* name, int def);
};

class Class_004c48c0 {
public:
    int FUN_004c48c0(char* dst, char* key, size_t size, char* def);
};

class Class_004c4760 {
public:
    double FUN_004c4760(const char* name, double def);
};

class Class_004c2f60 {
public:
    int FUN_004c2f60(char* file);
};

class Class_004c3410 {
public:
    int FUN_004c3410(char* name);
};

class Class_004c3e10 {
public:
    void FUN_004c3e10();
};

class Class_004c2ea0 {
public:
    int field_0;
    Class_004c46c0* current;            // +0x4
    int field_8;
    Class_004c2ea0();
    ~Class_004c2ea0();
};

class Class_0048dfb0 {
public:
    void FUN_0048dfb0();
};

struct Class_0048df90 {
    char unknown_0[0x8c];

    Class_0048df90();
    ~Class_0048df90() { ((Class_0048dfb0*)this)->FUN_0048dfb0(); }
};

class Class_0048e010 {
public:
    void FUN_0048e010(Class_004c2ea0* parser);
};

class Class_00438320 {
public:
    char name[0x20];                    // +0x0
    int radius;                         // +0x20
    float density;                      // +0x24
    float duration;                     // +0x28
    float interval;                     // +0x2c
    void FUN_00438320();
};

class Class_00436c30 {
public:
    void FUN_00436c30(char* schema, Class_004c2ea0* parser);
};

void __stdcall FUN_004290f0(char* out, const char* dir, const char* name, const char* ext);
void __stdcall FUN_004abd90(char* dest, char* text, int param_3, int param_4, int param_5);
void __stdcall FUN_004b6b80(const char* text, const char* caption);
int __stdcall FUN_004bbc40(char* path);
void __stdcall FUN_004bbd30(char* filename, void* buffer, int offset, int size);
char* __stdcall FUN_004c5740(char* text);
char* __stdcall FUN_004c5840(char* name);
int __stdcall FUN_004c58a0(Class_004c2ea0* obj, char* buf, const char* key, int size, char* def);
void* FUN_004d83b0(const char* tag, int size);
void FUN_004d85a0(void* p);
void FUN_00437d40();
void FUN_00437d50();
void __stdcall FUN_00437d60(Class_00438320* p);

struct Buffer_00435da0 {
    int* data;                         // +0x0
    int size;                          // +0x4
};

class Class_00435c00 {
public:
    int type;                          // +0x0
    char campaign[0x100];              // +0x4
    char names[9][0x100];              // +0x104
    int exists;                        // +0xa04
    Class_004c2ea0 list;               // +0xa08
    char missionName[0x100];           // +0xa14
    char text_b14[0x100];              // +0xb14
    char* briefing;                    // +0xc14
    int missionIndex;                  // +0xc18
    int field_c1c;                     // +0xc1c
    int field_c20;                     // +0xc20
    char description[0x80];            // +0xc24
    char planet[0x80];                 // +0xca4
    char unknown_d24[0xd30 - 0xd24];
    int surfaceMetal;                  // +0xd30
    int minWindSpeed;                  // +0xd34
    int maxWindSpeed;                  // +0xd38
    int gravity;                       // +0xd3c
    float tidalStrength;               // +0xd40
    int lavaWorld;                     // +0xd44
    int noSeaLevelTrigger;             // +0xd48
    int waterDoesDamage;               // +0xd4c
    int waterDamage;                   // +0xd50
    float killMul;                     // +0xd54
    float timeMul;                     // +0xd58
    float humanMetal;                  // +0xd5c
    float computerMetal;               // +0xd60
    char unknown_d64[0xd84 - 0xd64];
    float humanEnergy;                 // +0xd84
    float computerEnergy;              // +0xd88
    char unknown_d8c[0xdac - 0xd8c];
    Buffer_00435da0 buffer0;           // +0xdac
    Buffer_00435da0 buffer1;           // +0xdb4
    Buffer_00435da0 buffer2;           // +0xdbc
    char memory[0x80];                 // +0xdc4
    char numPlayers[0x80];             // +0xe44

    // 0x4356c0, inlined (the if/return form keeps the and/test the original has)
    char* GetName(int index)
    {
        char* ptr = (char*)this + index * 0x100 + 0x104;
        if (strlen(ptr) > 0)
            return ptr;
        return 0;
    }

    // 0x435320, inlined
    void LoadBriefing()
    {
        if (briefing)
            FUN_004d85a0(briefing);
        char* name = strlen(names[2]) > 0 ? names[2] : 0;
        if (name == 0) {
            briefing = 0;
            return;
        }
        int size = FUN_004bbc40(name);
        if (size != 0) {
            briefing = (char*)FUN_004d83b0("Briefing", size + 1);
            FUN_004bbd30(name, briefing, 0, size);
            briefing[size] = 0;
        }
    }

    void FUN_00435430(int index, char* dir, char* name, char* ext);
    int FUN_00436860(int type, Class_004c2ea0* parser, char* schema);
    int FUN_00435da0(char* map);
};

// FUNCTION: 0x435da0
int Class_00435c00::FUN_00435da0(char* map)
{
    Class_004c2ea0 parser;
    char schema[0x20];
    char value[0x100];
    char path[0x100];
    Class_00438320 meteor;
    char desc[0x80];
    char lower[0x80];

    delete g_game->field_391ed;
    g_game->field_391ed = new Class_0048df90;
    surfaceMetal = -1;
    minWindSpeed = -1;
    maxWindSpeed = -1;
    gravity = -1;
    tidalStrength = -1.0f;
    lavaWorld = 0;
    noSeaLevelTrigger = 0;
    planet[0] = 0;
    description[0] = 0;
    if (buffer0.data)
        FUN_004d85a0(buffer0.data);
    buffer0.data = 0;
    buffer0.size = 0;
    if (buffer1.data)
        FUN_004d85a0(buffer1.data);
    buffer1.data = 0;
    buffer1.size = 0;
    if (buffer2.data)
        FUN_004d85a0(buffer2.data);
    buffer2.data = 0;
    buffer2.size = 0;
    if (briefing) {
        FUN_004d85a0(briefing);
        briefing = 0;
    }

    switch (type) {
    case 1: {
        char key[0x100];
        sprintf(key, "MISSION%d", missionIndex);
        ((Class_004c3e10*)&list)->FUN_004c3e10();
        if (!((Class_004c3410*)&list)->FUN_004c3410(key)) {
            char msg[0x100];
            wsprintfA(msg, "The requested mission file, %s, does not exist.", key);
            FUN_004abd90(g_game->messages, msg, 0x1e0, 1, 1);
            return 0;
        }
        FUN_004c58a0(&list, missionName, "missionname", 0x100, 0);
        if (((Class_004c48c0*)list.current)->FUN_004c48c0(path, "missionfile", 0x100, DAT_005119b8)) {
            char file[0x100];
            FUN_004290f0(file, "Maps", path, "OTA");
            if (!((Class_004c2f60*)&parser)->FUN_004c2f60(file)) {
                char msg[0x100];
                sprintf(msg, "Hey, joker!  There is no mission defintion for this mission: %s", path);
                FUN_004abd90(g_game->messages, msg, 0x1e0, 1, 1);
                return 0;
            }
            ((Class_004c3e10*)&parser)->FUN_004c3e10();
            if (((Class_004c3410*)&parser)->FUN_004c3410("GlobalHeader")) {
                g_game->maxUnits = parser.current->FUN_004c46c0("maxunits", 200);
                ((Class_004c3e10*)&parser)->FUN_004c3e10();
                FUN_00435430(1, "Maps", path, "TNT");
            } else {
                char msg[0x100];
                sprintf(msg, "Hey, joker!  Mission file %s is corrupt (no header found).", path);
                FUN_004abd90(g_game->messages, msg, 0x1e0, 1, 1);
                return 0;
            }
        } else {
            FUN_004abd90(g_game->messages, "Old TED format no longer supported!", 0x1e0, 1, 1);
            return 0;
        }
        break;
    }
    case 2:
    case 3:
        exists = 0;
        strcpy(missionName, map);
        FUN_004290f0(path, "Maps", map, "OTA");
        if (!((Class_004c2f60*)&parser)->FUN_004c2f60(path)) {
            map = FUN_004c5840(map);
            if (map == 0)
                return 0;
            strcpy(missionName, map);
            FUN_004290f0(path, "Maps", map, "OTA");
            if (!((Class_004c2f60*)&parser)->FUN_004c2f60(path))
                return 0;
        }
        FUN_00435430(1, "Maps", map, "TNT");
        break;
    case 0:
        return 0;
    default:
        return 0;
    }

    if (!((Class_004c3410*)&parser)->FUN_004c3410("GlobalHeader")) {
        FUN_004abd90(g_game->messages, "No GlobalHeader block in mission file!", 0x1e0, 1, 1);
        return 0;
    }
    field_c20 = *(int*)((char*)parser.current + 0x25);
    FUN_004c58a0(&parser, value, "brief", 0x100, DAT_005119b8);
    FUN_00435430(2, "camps\\briefs", value, "TXT");
    LoadBriefing();
    FUN_004c58a0(&parser, value, "narration", 0x100, DAT_005119b8);
    FUN_00435430(3, "camps\\briefs", value, "WAV");
    FUN_004c58a0(&parser, value, "missionhint", 0x100, DAT_005119b8);
    FUN_00435430(4, "camps\\hints", value, "TXT");
    ((Class_004c48c0*)parser.current)->FUN_004c48c0(value, "glamour", 0x100, DAT_005119b8);
    FUN_00435430(5, DAT_005119b8, value, "PCX");
    ((Class_004c48c0*)parser.current)->FUN_004c48c0(value, "glamoursound", 0x100, DAT_005119b8);
    FUN_00435430(8, "camps\\briefs", value, "WAV");
    ((Class_004c48c0*)parser.current)->FUN_004c48c0(value, "UseOnlyUnits", 0x100, DAT_005119b8);
    FUN_00435430(6, "camps\\useonly", value, "TDF");
    g_game->mapping = parser.current->FUN_004c46c0("mapping", 0);
    g_game->lineOfSight = parser.current->FUN_004c46c0("lineofsight", 0);
    g_game->field_39225 = 1;
    g_game->field_39219 = 0;
    ((Class_004c48c0*)parser.current)->FUN_004c48c0(memory, "memory", 0x80, DAT_005119b8);
    ((Class_004c48c0*)parser.current)->FUN_004c48c0(numPlayers, "numplayers", 0x80, DAT_005119b8);
    ((Class_004c48c0*)parser.current)->FUN_004c48c0(planet, "Planet", 0x80, DAT_005119b8);
    g_game->noMovie = parser.current->FUN_004c46c0("nomovie", 0);
    ((Class_004c48c0*)parser.current)->FUN_004c48c0(desc, "missiondescription", 0x80, "No description available");
    strcpy(lower, desc);
    _strlwr(lower);
    strcpy(description, FUN_004c5740(lower));
    if (_strcmpi(description, lower) == 0)
        strcpy(description, desc);
    minWindSpeed = parser.current->FUN_004c46c0("minwindspeed", 0);
    maxWindSpeed = parser.current->FUN_004c46c0("maxwindspeed", 0);
    gravity = parser.current->FUN_004c46c0("gravity", 0);
    tidalStrength = (float)((Class_004c4760*)parser.current)->FUN_004c4760("tidalstrength", 0.0);
    lavaWorld = parser.current->FUN_004c46c0("lavaworld", 0);
    noSeaLevelTrigger = parser.current->FUN_004c46c0("nosealeveltrigger", 0);
    waterDoesDamage = parser.current->FUN_004c46c0("waterdoesdamage", 0);
    waterDamage = parser.current->FUN_004c46c0("waterdamage", 0);
    ((Class_0048e010*)g_game->field_391ed)->FUN_0048e010(&parser);
    killMul = (float)((Class_004c4760*)parser.current)->FUN_004c4760("killmul", 0.0);
    timeMul = (float)((Class_004c4760*)parser.current)->FUN_004c4760("timemul", 0.0);
    if (!FUN_00436860(type, &parser, schema)) {
        FUN_004b6b80("No suitable schema type in mission file!", "Map error");
        return 0;
    }
    humanMetal = (float)parser.current->FUN_004c46c0("HumanMetal", 0);
    humanEnergy = (float)parser.current->FUN_004c46c0("HumanEnergy", 0);
    computerMetal = (float)parser.current->FUN_004c46c0("ComputerMetal", 0);
    computerEnergy = (float)parser.current->FUN_004c46c0("ComputerEnergy", 0);
    surfaceMetal = parser.current->FUN_004c46c0("SurfaceMetal", 0);
    ((Class_004c48c0*)parser.current)->FUN_004c48c0(value, "aiprofile", 0x100, DAT_005119b8);
    FUN_00435430(7, "ai", value, "txt");
    if (!GetName(7))
        FUN_00435430(7, "ai", "default", "txt");
    ((Class_004c48c0*)parser.current)->FUN_004c48c0(meteor.name, "MeteorWeapon", 0x20, DAT_005119b8);
    if (strlen(meteor.name) != 0) {
        meteor.radius = parser.current->FUN_004c46c0("MeteorRadius", 0);
        meteor.density = (float)((Class_004c4760*)parser.current)->FUN_004c4760("MeteorDensity", 0.0);
        meteor.duration = (float)((Class_004c4760*)parser.current)->FUN_004c4760("MeteorDuration", 0.0);
        meteor.interval = (float)((Class_004c4760*)parser.current)->FUN_004c4760("MeteorInterval", 0.0);
        if (meteor.radius == 0 || meteor.density == 0.0f || meteor.duration == 0.0f || meteor.interval == 0.0f)
            meteor.FUN_00438320();
        FUN_00437d40();
    } else {
        FUN_00437d50();
        meteor.FUN_00438320();
    }
    FUN_00437d60(&meteor);
    ((Class_00436c30*)this)->FUN_00436c30(schema, &parser);
    return 1;
}
