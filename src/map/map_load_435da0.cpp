// Decompiled by Claude Opus 5.5, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by deepseek-v4.1-flash, finished by opus. Names are provisional.
// MATCH. Loads the current mission: resets the mission state, finds the
// mission's OTA file (from the campaign list entry MISSION<n> for type 1, or
// from the map name for types 2 and 3), reads its GlobalHeader block into the
// fields and the name slots (0x435430.cpp), and passes the schema to
// 0x436c30. Returns 1 on success, 0 after reporting an error. 0x435320
// (LoadBriefing) and 0x4356c0 (GetName) are inlined.
//
// What made it match (it sat at 89.5% for many passes):
//  * The constant registers (0 in ebx, -1 in esi; with them name/size in
//    esi/edi and the old object in esi) came from case 2/3: the found-at-once
//    arm calls FUN_00435430 and breaks, and only the fallback reassigns
//    `map`. That splits map into two webs, so its priority drops from 76 to
//    46/44, below LoadBriefing's `name` (56). name is then coloured before
//    map takes ebx and gets esi while no constant piece is pinned there, and
//    the 0 and -1 pieces end up in ebx and esi as in the original. Found with
//    tools/c2prio.py --trace (which prints the -1 constant as "const 0").
//    The old 95% lever (extra field_c1c = -1 / field_c20 = 0 stores) only
//    tipped the same race from the other side.
//  * The late x87 stores after the float getters: every float field is read
//    through an inline GetFloat that converts the double result into a float
//    local and returns it. A plain `(float)` call, a double local alone or a
//    float local alone all keep the fstp right after the call whenever the
//    next call also returns a double.
//  * The 0x48e010 callee is Class_0048ff40::FUN_0048e010 (data/symbols.csv).
// 0x437280 (the buffer reset, no callers) is written out: C1 does not
// auto-inline it out of class (IL 180), while 0x435320 (IL 157) is.
#include <windows.h>
#include <stdio.h>
#include <string.h>

#pragma pack(push, 1)
struct Game {
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

extern Game* g_game;
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

class Class_0048ff40 {
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
void __stdcall OpenMessageBox(char* dest, char* text, int param_3, int param_4, int param_5);
void __stdcall FUN_004b6b80(const char* text, const char* caption);
int __stdcall FUN_004bbc40(char* path);
void __stdcall FUN_004bbd30(char* filename, void* buffer, int offset, int size);
char* __stdcall FUN_004c5740(char* text);
char* __stdcall FUN_004c5840(char* name);
int __stdcall FUN_004c58a0(Class_004c2ea0* obj, char* buf, const char* key, int size, char* def);
void* __cdecl FUN_004d83b0(const char* tag, int size);
void __cdecl FUN_004d85a0(void* p);
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

static inline float GetFloat(Class_004c46c0* section, const char* key)
{
    double value = ((Class_004c4760*)section)->FUN_004c4760(key, 0.0);
    float result = (float)value;
    return result;
}

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
            OpenMessageBox(g_game->messages, msg, 0x1e0, 1, 1);
            return 0;
        }
        FUN_004c58a0(&list, missionName, "missionname", 0x100, 0);
        int found = ((Class_004c48c0*)list.current)->FUN_004c48c0(path, "missionfile", 0x100, DAT_005119b8);
        if (found) {
            char file[0x100];
            FUN_004290f0(file, "Maps", path, "OTA");
            if (!((Class_004c2f60*)&parser)->FUN_004c2f60(file)) {
                char msg[0x100];
                sprintf(msg, "Hey, joker!  There is no mission defintion for this mission: %s", path);
                OpenMessageBox(g_game->messages, msg, 0x1e0, 1, 1);
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
                OpenMessageBox(g_game->messages, msg, 0x1e0, 1, 1);
                return 0;
            }
        } else {
            OpenMessageBox(g_game->messages, "Old TED format no longer supported!", 0x1e0, 1, 1);
            return 0;
        }
        break;
    }
    case 2:
    case 3:
        exists = 0;
        strcpy(missionName, map);
        FUN_004290f0(path, "Maps", map, "OTA");
        if (((Class_004c2f60*)&parser)->FUN_004c2f60(path)) {
            FUN_00435430(1, "Maps", map, "TNT");
            break;
        }
        map = FUN_004c5840(map);
        if (map == 0)
            return 0;
        strcpy(missionName, map);
        FUN_004290f0(path, "Maps", map, "OTA");
        if (!((Class_004c2f60*)&parser)->FUN_004c2f60(path))
            return 0;
        FUN_00435430(1, "Maps", map, "TNT");
        break;
    case 0:
        return 0;
    default:
        return 0;
    }

    if (!((Class_004c3410*)&parser)->FUN_004c3410("GlobalHeader")) {
        OpenMessageBox(g_game->messages, "No GlobalHeader block in mission file!", 0x1e0, 1, 1);
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
    tidalStrength = GetFloat(parser.current, "tidalstrength");
    lavaWorld = parser.current->FUN_004c46c0("lavaworld", 0);
    noSeaLevelTrigger = parser.current->FUN_004c46c0("nosealeveltrigger", 0);
    waterDoesDamage = parser.current->FUN_004c46c0("waterdoesdamage", 0);
    waterDamage = parser.current->FUN_004c46c0("waterdamage", 0);
    ((Class_0048ff40*)g_game->field_391ed)->FUN_0048e010(&parser);
    killMul = GetFloat(parser.current, "killmul");
    timeMul = GetFloat(parser.current, "timemul");
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
        meteor.density = GetFloat(parser.current, "MeteorDensity");
        meteor.duration = GetFloat(parser.current, "MeteorDuration");
        meteor.interval = GetFloat(parser.current, "MeteorInterval");
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

