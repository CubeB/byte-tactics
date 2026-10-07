// Decompiled by Opus, Space Bunny Free, deepseek-v4.1-flash, deepseek-v4.1, Haiku, DeepSeek V4.1 Flash, Claude Opus 5.5, Sonnet and GPT-6.1-sol. Names are provisional.

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <vector>

class TdfRecord;
class Mission;
class Class_004c9390;
class Class_004c91a0;
class Class_004c91b0;

class TdfFile {
public:
    TdfRecord* root;               // +0x0
    TdfRecord* current;            // +0x4
    int field_8;                   // +0x8
    TdfFile();
    ~TdfFile();
    int LoadFile(char* file);
    int SelectRecord(char* name);
    void ResetCurrentRecord();
    void Unload();
};

extern char DAT_005119b8[];

#pragma pack(push, 1)
struct Slot_00436860 {
    int active;                        // +0x0
    char unknown_4[0x14];
};

struct Game {
    char unknown_0[0x519];
    char messages[0x29a0 - 0x519];     // +0x519
    Slot_00436860* slots;              // +0x29a0
    char unknown_29a4[0x2a3c - 0x29a4];
    unsigned short numPlayers;         // +0x2a3c
    char unknown_2a3e[0x2c28 - 0x2a3e];
    int playerIds[10];                 // +0x2c28
    char unknown_2c50[0x37ee6 - 0x2c50];
    short maxUnits;                    // +0x37ee6
    char unknown_37ee8[0x37eee - 0x37ee8];
    int difficulty;                    // +0x37eee
    char unknown_37ef2[0x39073 - 0x37ef2];
    int noMovie;                       // +0x39073
    char unknown_39077[0x391e9 - 0x39077];
    Mission* field_391e9;              // +0x391e9
    struct Class_0048df90* field_391ed; // +0x391ed
    char unknown_391f1[0x39219 - 0x391f1];
    int field_39219;                   // +0x39219
    int mapping;                       // +0x3921d
    int lineOfSight;                   // +0x39221
    int field_39225;                   // +0x39225
};
#pragma pack(pop)

extern Game* g_game;

class PacketManager {
public:
    void SendAllQueued(int param);
};

extern char* g_otaEnumFileList;
extern int g_otaEnumCacheComplete;
extern int g_otaEnumFileListBytes;
extern int g_otaEnumFileCount;
extern int g_usePacketManager;
extern PacketManager g_packetManager;

class Class_0048dfb0 {
public:
    void FUN_0048dfb0();
};

int __stdcall HAPI_FileLengthByName(char* path);
void __stdcall OpenMessageBox(char* dest, char* text, int param_3, int param_4, int param_5);

void __stdcall HAPI_ReadFileAt(char* filename, void* buffer, int offset, int size);
void __cdecl FUN_004d85a0(void* p);

int GetPreferredLanguage(void);
char* __stdcall StripExtension(char* name);
void* __stdcall HAPI_OpenFileRead(char* path);
int __stdcall HAPI_CloseFile(void* file);

char* __stdcall SkipTextLines(char* list, int index);
int __stdcall GetLocalizedString(TdfFile* obj, char* buf, const char* key, int size, int def);

char* __stdcall Translate(char* text);

int __stdcall LoadMapList(char** out, int param_2, int param_3);

class TdfRecord {
public:
    int GetFieldString(char* dst, char* key, size_t size, char* def);
    int GetFieldInt(const char* name, int def);
    double GetFieldDouble(const char* name, double def);
    TdfRecord* FindSubRecord(const char* name);
};

struct Class_0048df90 {
    char unknown_0[0x8c];

    Class_0048df90();
    ~Class_0048df90() { ((Class_0048dfb0*)this)->FUN_0048dfb0(); }
};

class MissionConditions {
public:
    void RegisterConditions(TdfFile* parser);
};

class MeteorParams {
public:
    char name[0x20];                    // +0x0
    int radius;                         // +0x20
    float density;                      // +0x24
    float duration;                     // +0x28
    float interval;                     // +0x2c
    void LoadMeteorDefaults();
};

void __stdcall BuildDataPath(char* out, const char* dir, const char* name, const char* ext);
void __stdcall ShowErrorBox(const char* text, const char* caption);
char* __stdcall FindTranslation(char* name);
int __stdcall GetLocalizedString(TdfFile* obj, char* buf, const char* key, int size, char* def);
void EnableMeteors();
void DisableMeteors();
void __stdcall SetMeteorParams(MeteorParams* p);
void* __cdecl FUN_004d84a0(void* p, const char* name, unsigned int size);
void __stdcall FUN_00491c80(int n);
void __stdcall ListDirectory(const char* pattern, int flags, std::vector<Class_004c91a0>* out);
void HandleNetPackets();

// Every float field is read through this: the double result goes into a float
// local and is returned, which delays the x87 store.
static inline float GetFloat(TdfRecord* section, const char* key)
{
    double value = section->GetFieldDouble(key, 0.0);
    float result = (float)value;
    return result;
}

void __stdcall FatalError(char* text);

class Class_004c44c0 {
public:
    TdfRecord* GetSubRecord(int index);
};

class Class_004c4450 {
public:
    int GetSubRecordCount();
};

struct MissionUnit {
    char* name;                        // +0x0
    char* ident;                       // +0x4
    char* initialMission;              // +0x8
    int x;                             // +0xc
    int y;                             // +0x10
    int z;                             // +0x14
    short angle;                       // +0x18
    short health;                      // +0x1a
    int creationCountdown;             // +0x1c
    short buildPriority;               // +0x20
    unsigned char player;              // +0x22
    unsigned char initialGroup : 4;    // +0x23
    unsigned char missionCritical : 1;
    unsigned char aiIgnore : 1;
    unsigned char aiPriorityTarget : 1;
    unsigned char immunity : 1;
};

struct MissionRule {
    int type;                          // +0x0
    int id;                            // +0x4
    short x;                           // +0x8
    short z;                           // +0xa
};

struct MissionFeature {
    char name[0x80];                   // +0x0
    int x;                             // +0x80
    int z;                             // +0x84
};

void* __cdecl FUN_004d83b0(char* name, unsigned int size);

struct Vec3_00437320 {
    int x;                             // +0x0
    int y;                             // +0x4
    int z;                             // +0x8
};

class Class_004c9390 {
public:
    char* data;                        // +0x0
    void ReleaseRef();
};

// The reference-counted string handle: 0x4c91a0 is its copy constructor and
// 0x4c9390 its release.
class Class_004c91a0 : public Class_004c9390 {
public:
    Class_004c91a0(const Class_004c91a0& other);
    ~Class_004c91a0() { ReleaseRef(); }
};

// 0x4c91b0 builds one of those handles from a C string. In the game this is a
// second constructor of the same class, which data/symbols.csv cannot hold
// twice, so it is a derived class here.
class Class_004c91b0 : public Class_004c91a0 {
public:
    Class_004c91b0(const char* text);
};

struct Header_004373a0 {
    int magic;                         // +0x00
    int width;                         // +0x04
    int height;                        // +0x08
    int unknown_0c;                    // +0x0c
    int plotOffset;                    // +0x10
    int unknown_14;                    // +0x14
    int unknown_18;                    // +0x18
    int features;                      // +0x1c
    int featureOffset;                 // +0x20
    char unknown_24[0x40 - 0x24];
};

int __stdcall HAPI_readfromfile(void* file, void* buf, int size);
int __stdcall HAPI_SeekFile(void* file, int pos);
int __stdcall ComputeChecksum(unsigned char* data, int len);

class Mission {
public:
    int type;                          // +0x000
    char campaign[0x100];              // +0x004
    char names[9][0x100];              // +0x104
    int exists;                        // +0xa04
    TdfFile list;                      // +0xa08, the campaign file
    char missionName[0x100];           // +0xa14
    char text_b14[0x100];              // +0xb14
    char* briefing;                    // +0xc14
    int missionIndex;                  // +0xc18
    int field_c1c;                     // +0xc1c
    int field_c20;                     // +0xc20
    char description[0x80];            // +0xc24
    char planet[0x80];                 // +0xca4
    char* mapList;                     // +0xd24
    int mapCount;                      // +0xd28
    int multi;                         // +0xd2c
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
    MissionUnit* units;                // +0xdac
    int unitCount;                     // +0xdb0
    MissionRule* rules;                // +0xdb4
    int ruleCount;                     // +0xdb8
    MissionFeature* features;          // +0xdbc
    int featureCount;                  // +0xdc0
    char memory[0x80];                 // +0xdc4
    char numPlayers[0x80];             // +0xe44

    Mission(int owner_);
    ~Mission();
    int GetGameType();
    void LoadCampaign(char* file);
    char* GetCampaignName();
    void LoadBriefing();
    char* GetBriefing();
    void SetNameSlot(int index, char* text);
    void BuildCampaignFilePath(int index, char* dir, char* name, char* ext);
    char* GetNameSlot(int index);
    int CountMissions();
    int BuildMissionList(char** out);
    int GetTerrainLength();
    int GetDescription();
    int GetPlanet();
    int GetTerrainSizeTier();
    int MissionExists(int index);
    int LoadMissionByName(char* map);
    void SelectMission(int param_1);
    int GetTranslatedName();
    int GetMissionName();
    bool HasMissionName();
    int GetMissionIndex();
    int AdvanceMission();
    void RefreshMapList(int param_1);
    int LoadMission(char* map);
    int SelectSchema(int type, TdfFile* parser, char* schema);
    void LoadMissionData(char* name, TdfFile* parser);
    void FreeMissionData();
    int CountStartPositions();
    int GetStartPosition(Vec3_00437320* out, int id);
    int ComputeMapChecksum();
};

class MapCacheEntry {
public:
    Class_004c91a0 handle;             // +0x0
    int field_4;                       // +0x4

    MapCacheEntry(const Class_004c91a0& other) : handle(other) {}

    MapCacheEntry& SetChecksum(Mission* self)
    {
        field_4 = self->field_c1c;
        return *this;
    }
};

// The vector at 0x5122c0: MSVC 5 puts the empty allocator byte at +0, so
// _First is g_mapCacheBegin and _Last is g_mapCacheEnd.
extern MapCacheEntry* g_mapCacheBegin;
extern MapCacheEntry* g_mapCacheEnd;

class MapCache : public std::vector<MapCacheEntry> {
public:
    void FUN_00437580(MapCacheEntry* pos, int count, const MapCacheEntry& val);
};


// A file-local global std::vector: the compiler generates its initialiser
// (0x434a30) and the destructor it registers with atexit (0x434a60).
//
// The element is 8 bytes and its destructor releases the reference-counted
// string at +0 through ReleaseRef. Must stay file-static: an external global
// reloads _First after the destroy loop.
// FUNCTION: 0x434a30 _$E5
// FUNCTION: 0x434a60 _$E3
static MapCache s_mapCache;


// Replaces the singleton at g_game+0x391e9 with a fresh Mission when the
// current one belongs to a different owner, then stores it back (NULL when the
// allocation failed). The constructor body is inlined here.
// FUNCTION: 0x434ab0
void __stdcall SetMissionType(int owner)
{
    if (g_game->field_391e9 != 0) {
        if (g_game->field_391e9->type == owner) {
            return;
        }
        // Re-read the global here, not through a local: keeps the delete's null check.
        delete g_game->field_391e9;
    }
    g_game->field_391e9 = new Mission(owner);
}

// Frees a global buffer, clears three globals and deletes the object at
// g_game+0x391e9.

// FUNCTION: 0x434b90
void FreeOtaEnumCacheAndMission()
{
    if (g_otaEnumFileList) {
        FUN_004d85a0(g_otaEnumFileList);
        g_otaEnumFileList = 0;
    }
    g_otaEnumCacheComplete = 0;
    g_otaEnumFileListBytes = 0;
    g_otaEnumFileCount = 0;
    delete g_game->field_391e9;
    g_game->field_391e9 = 0;
}

// FUNCTION: 0x434bf0
int __stdcall LoadMapList(void** param_1, int param_2, int param_3)
{
    if (g_otaEnumFileList != 0) {
        if (param_1 != 0) {
            if (param_3 != 0 && g_otaEnumCacheComplete == 0) {
                *param_1 = g_otaEnumFileList;
                g_otaEnumFileList = 0;
            } else {
                char* p = (char*)FUN_004d83b0("MULTI MAPS", g_otaEnumFileListBytes);
                *param_1 = p;
                memcpy(p, g_otaEnumFileList, g_otaEnumFileListBytes);
            }
        }
        return g_otaEnumFileCount;
    }

    FUN_00491c80(0x14);
    // The scalar locals are members of one struct: frame offsets follow member order.
    struct S { int count; int bFlag; int i; } s;
    
    if (param_2 == 0) {
        s.bFlag = 1;
        if (g_game->field_391e9->type != 3)
            s.bFlag = 0;
    } else {
        s.bFlag = 0;
    }
    int offset = 0;
    g_otaEnumFileListBytes = 1;
    g_otaEnumFileList = (char*)FUN_004d83b0("MULTI MAPS", 1);
    *(char*)g_otaEnumFileList = 0;

    std::vector<Class_004c91a0> files;
    ListDirectory("Maps\\*.ota", 0, &files);
    g_otaEnumFileCount = 0;

    s.count = files.size();
    for (s.i = 0; s.i < s.count; s.i++) {
        // The buffers are members of one struct, name before lower: same reason.
        struct A { char name[256]; char lower[256]; char path[256]; } a;
        BuildDataPath(a.path, "Maps", files[s.i].data, "OTA");
        TdfFile parser;
        if (parser.LoadFile(a.path) != 0
            && g_game->field_391e9->SelectSchema(3, &parser, 0) != 0) {
            strcpy(a.name, files[s.i].data);
            StripExtension(a.name);
            strcpy(a.lower, a.name);
            _strlwr(a.lower);
            char* src = Translate(a.lower);
            if (_strcmpi(src, a.lower) == 0)
                src = a.name;
            int len = strlen(src) + 1;
            g_otaEnumFileList = (char*)FUN_004d84a0(g_otaEnumFileList, "MULTI MAPS",
                                               len + g_otaEnumFileListBytes);
            strcpy(g_otaEnumFileList + offset, src);
            g_otaEnumFileList[offset + len] = 0;
            offset += len;
            g_otaEnumFileListBytes += len;
            g_otaEnumFileCount++;
            if (param_2 != 0)
                break;
        }
        if (s.bFlag != 0) {
            HandleNetPackets();
            if (g_usePacketManager != 0)
                g_packetManager.SendAllQueued(0);
        }
    }
    FUN_00491c80(0x13);
    if (g_otaEnumCacheComplete == 0)
        g_otaEnumCacheComplete = (param_2 == 0);
    int result = LoadMapList(param_1, param_2, param_3);
    return result;
}

// Constructor: clears the state, stores the type and resets the object with
// an empty name through LoadCampaign (which copies the name to +0x4).
// FUNCTION: 0x434f70
Mission::Mission(int owner_)
{
    ruleCount = 0;
    unitCount = 0;
    featureCount = 0;
    rules = 0;
    units = 0;
    features = 0;
    exists = 0;
    mapList = 0;
    mapCount = 0;
    multi = 0;
    briefing = 0;
    missionName[0] = 0;
    text_b14[0] = 0;
    type = owner_;
    LoadCampaign(DAT_005119b8);
}

// Destructor of Mission (constructor 0x434f70, sibling 0x437280).
// Deletes the object at g_game+0x391ed, frees the two-dword buffers at
// +0xdac/+0xdb4/+0xdbc and the pointers at +0xc14 and +0xd24, then destroys
// the sub-object at +0xa08. The second round of buffer frees is dead (the
// first round already zeroed the pointers) but the original emits it anyway,
// so keep it.
// FUNCTION: 0x434ff0
Mission::~Mission()
{
    Class_0048dfb0* obj = (Class_0048dfb0*)g_game->field_391ed;
    if (obj) {
        obj->FUN_0048dfb0();
        operator delete(obj);
        g_game->field_391ed = 0;
    }
    if (units)
        FUN_004d85a0(units);
    units = 0;
    unitCount = 0;
    if (rules)
        FUN_004d85a0(rules);
    rules = 0;
    ruleCount = 0;
    if (features)
        FUN_004d85a0(features);
    features = 0;
    featureCount = 0;
    if (briefing) {
        FUN_004d85a0(briefing);
        briefing = 0;
    }
    if (mapList)
        FUN_004d85a0(mapList);
    if (features)
        FUN_004d85a0(features);
    if (rules)
        FUN_004d85a0(rules);
    if (units)
        FUN_004d85a0(units);

}

// FUNCTION: 0x435100
int Mission::GetGameType()
{
    return *(int*)this;
}

// Loads a campaign (the file name of a .cpf campaign script) into the
// campaign object: clears the script list, copies the file name into
// `campaign`, empties all nine name slots (0x4353b0, inlined), builds the
// path of the campaign file itself into name slot 0 (0x435430), loads the
// script into the list at +0xa08, and on failure reports it in the message
// box and retries with the blank name. Then it clears the mission index and
// loads mission 0, but only when a campaign name was given at all: an empty
// `file` returns here without touching the state.
//
// Two things look like Cavedog's own slips, kept as the original has them:
// - the second `strlen(file) != 0` test is dead: 0x435430 cannot change the
//   caller's pointer, and the first test already passed;
// - the message buffer is 0x80 bytes at the top of a 0x80 frame, while the
//   name printed into it can be 0xff bytes, so a long campaign path overruns
//   the frame. 0x435da0.cpp uses 0x100 for the same message.
// FUNCTION: 0x435110
void Mission::LoadCampaign(char* file)
{
    char msg[0x80];

    list.Unload();
    strcpy(campaign, file);
    for (int i = 0; i < 9; i++)
        SetNameSlot(i, DAT_005119b8);
    if (strlen(file) != 0) {
        BuildCampaignFilePath(0, "camps", campaign, "TDF");
        if (strlen(file) != 0) {
            if (!list.LoadFile(GetNameSlot(0))) {
                wsprintfA(msg, "The requested campaign file, %s, does not exist.", GetNameSlot(0));
                OpenMessageBox(g_game->messages, msg, 0x1e0, 1, 1);
                LoadCampaign(DAT_005119b8);
                return;
            }
        }
        field_c1c = 0;
        missionIndex = 0;
        LoadMission(0);
    }
}

// FUNCTION: 0x4352b0
char* Mission::GetCampaignName()
{
    char* ptr = campaign;
    if (strlen(ptr) > 0) {
        return ptr;
    }
    return 0;
}

// Reloads the buffer at +0xc14 from the file named by the text at +0x304:
// frees the old buffer, sizes the file, allocates size+1 bytes and loads it.
// When the text is empty the buffer is cleared instead.
// FUNCTION: 0x435320
void Mission::LoadBriefing()
{
    if (briefing)
        FUN_004d85a0(briefing);
    char* name = strlen(names[2]) > 0 ? names[2] : 0;
    if (name == 0) {
        briefing = 0;
        return;
    }
    int size = HAPI_FileLengthByName(name);
    if (size != 0) {
        briefing = (char*)FUN_004d83b0("Briefing", size + 1);
        HAPI_ReadFileAt(name, briefing, 0, size);
        briefing[size] = 0;
    }
}

// FUNCTION: 0x4353a0
char* Mission::GetBriefing()
{
    return briefing;
}

// Stores a name in slot `index`; for slot 1 also records whether a file of
// that name exists (HAPI_FileLengthByName), or 0 when the name is empty.
// FUNCTION: 0x4353b0
void Mission::SetNameSlot(int index, char* text)
{
    strcpy(names[index], text);
    if (index == 1) {
        if (strlen(text) != 0)
            exists = HAPI_FileLengthByName(text);
        else
            exists = 0;
    }
}

// Builds the path of a campaign file and stores it in name slot `index`
// (an inlined copy of 0x4353b0.cpp). With a side prefix (GetPreferredLanguage) it
// first tries "<dir>-<side>\<name>.<ext>" and keeps it when that file opens;
// otherwise it uses "<dir>\<name>.<ext>". An empty name stores the blank
// string DAT_005119b8.
// FUNCTION: 0x435430
void Mission::BuildCampaignFilePath(int index, char* dir, char* name, char* ext)
{
    char path[256];
    if (strlen(name) == 0) {
        SetNameSlot(index, DAT_005119b8);
        return;
    }
    char* side = (char*)GetPreferredLanguage();
    if (side) {
        sprintf(path, "%s-%s\\%s", dir, side, name);
        StripExtension(path);
        strcat(path, ".");
        strcat(path, ext);
        void* file = HAPI_OpenFileRead(path);
        if (file) {
            HAPI_CloseFile(file);
            SetNameSlot(index, path);
            return;
        }
    }
    sprintf(path, "%s\\%s", dir, name);
    StripExtension(path);
    strcat(path, ".");
    strcat(path, ext);
    SetNameSlot(index, path);
}

// FUNCTION: 0x4356c0
char* Mission::GetNameSlot(int index)
{
    char* ptr = (char*)this + index * 0x100 + 0x104;
    if (strlen(ptr) > 0)
        return ptr;
    return 0;
}

// Counts the consecutive "MISSION<n>" rules in the embedded list (from
// MISSION0 up); a blank name means no missions. See 0x435980.cpp.
// FUNCTION: 0x4356f0
int Mission::CountMissions()
{
    char buf[128];
    if (strlen(campaign) == 0)
        return 0;
    int n = 0;
    while (1) {
        sprintf(buf, "MISSION%d", n);
        list.ResetCurrentRecord();
        if (list.SelectRecord(buf) == 0)
            break;
        n++;
    }
    return n;
}

// Fills *param_1 with the mission list and returns how many missions there
// are. Neighbours 0x4356f0 and 0x435980 share the same layout.
// FUNCTION: 0x435760
int Mission::BuildMissionList(char** out)
{
    char buf[128];
    char temp[256];
    if (strlen(campaign) == 0)
        return 0;
    int n;
    if (strlen(&campaign[0]) == 0) {
        n = 0;
    } else {
        // Counted in `m` and copied to `n` after the loop, not counted on `n`.
        int m = 0;
        while (1) {
            sprintf(buf, "MISSION%d", m);
            list.ResetCurrentRecord();
            if (list.SelectRecord(buf) == 0)
                break;
            m++;
        }
        n = m;
    }
    if (n != 0) {
        char* p = (char*)FUN_004d83b0("MissionList", n << 8);
        *out = p;
        *p = 0;
        for (int i = 0; i < n; i++) {
            sprintf(buf, "MISSION%d", i);
            list.ResetCurrentRecord();
            if (list.SelectRecord(buf) == 0)
                return 0;
            if (GetLocalizedString(&list, temp, "missionname", 0x100, 0) != 0)
                strcpy(SkipTextLines(*out, i), temp);
            else
                strcpy(SkipTextLines(*out, i), "Error -- Unnamed Mission");
        }
    }
    return n;
}

// FUNCTION: 0x4358f0
int Mission::GetTerrainLength()
{
    return *(int*)((char*)this + 0xa04);
}

// FUNCTION: 0x435900
int Mission::GetDescription()
{
    return (int)((char*)this + 0xc24);
}

// FUNCTION: 0x435910
int Mission::GetPlanet()
{
    return (int)((char*)this + 0xca4);
}

// FUNCTION: 0x435920
int Mission::GetTerrainSizeTier()
{
    int v = exists;
    if (v < 0x3e6666)
        return 0x10;
    if (v < 0x600000)
        return 0x18;
    if (v < 0x800000)
        return 0x20;
    if (v < 0xa00000)
        return 0x30;
    if (v < 0xc00000)
        return 0x40;
    return 0x80;
}

// Counts the consecutive "MISSION<n>" rules in the embedded list (from
// MISSION0 up) and returns whether there are more than `index` of them. A
// blank name means no missions.
// FUNCTION: 0x435980
int Mission::MissionExists(int index)
{
    char buf[128];
    int n;
    if (strlen(campaign) == 0) {
        n = 0;
    } else {
        n = 0;
        sprintf(buf, "MISSION%d", n);
        list.ResetCurrentRecord();
        while (list.SelectRecord(buf)) {
            n++;
            sprintf(buf, "MISSION%d", n);
            list.ResetCurrentRecord();
        }
    }
    return n > index;
}

// Loads a mission by name. Type 1 walks the mission list built by 0x435760
// looking for the name and, on a match, resets the mission index and loads
// that mission. Types 2 and 3 load the map and, when the language is not
// "english", put the translated name (0x4c5740) into the name slot at +0xb14,
// keeping the original spelling when the lookup changed nothing. Every other
// type returns 0.
//
// The `field_c1c = 0` store in the mission-loop branch repeats the one at the
// top of the function, kept as the original has it.
// FUNCTION: 0x435a20
int Mission::LoadMissionByName(char* map)
{
    // One local for the load result and the mission list out-parameter.
    int res;

    field_c1c = 0;
    if (type != 1) {
        if (type > 1 && type <= 3) {
            res = LoadMission(map);
            if (res && GetPreferredLanguage() && _strcmpi((char*)GetPreferredLanguage(), "english")) {
                char lower[200];
                strcpy(lower, map);
                _strlwr(lower);
                // Translate gets the buffer, not (char*)this: keeps `lower` live across the call.
                strncpy(text_b14, Translate(lower), 0xff);
                if (_strcmpi(text_b14, map) == 0)
                    strcpy(text_b14, map);
            } else {
                strcpy(text_b14, map);
            }
            return res;
        }
    } else {
        int count = BuildMissionList((char**)&res);
        if (count > 0) {
            char* p = (char*)res;
            for (int i = 0; i < count; i++) {
                if (_strcmpi(p, map) == 0) {
                    FUN_004d85a0((void*)res);
                    field_c1c = 0;
                    missionIndex = i;
                    return LoadMission(0);
                }
                p += strlen(p) + 1;
            }
            FUN_004d85a0((void*)res);
        }
    }
    return 0;
}

// FUNCTION: 0x435c00
void Mission::SelectMission(int param_1)
{
    field_c1c = 0;
    missionIndex = param_1;
    LoadMission(0);
}

// FUNCTION: 0x435c20
int Mission::GetTranslatedName()
{
    return (int)((char*)this + 0xb14);
}

// FUNCTION: 0x435c30
int Mission::GetMissionName()
{
    return (int)((char*)this + 0xa14);
}

// FUNCTION: 0x435c40
bool Mission::HasMissionName()
{
    return missionName[0] != 0;
}

// FUNCTION: 0x435c50
int Mission::GetMissionIndex()
{
    return *(int*)((char*)this + 0xc18);
}

// Advances the current mission index when the embedded list holds more than
// "current index + 1" consecutive MISSION<n> rules; resets the list cursor
// first. Same scan and side effects as 0x435980.cpp and 0x435c00.cpp.
// LoadMission (0x435da0) is a __thiscall method of this class: the original
// loads ecx = this before the call, which a free declaration would drop.
// FUNCTION: 0x435c60
int Mission::AdvanceMission()
{
    char buf[128];
    int n;
    int index = missionIndex + 1;
    if (strlen(campaign) == 0) {
        n = 0;
    } else {
        n = 0;
        sprintf(buf, "MISSION%d", n);
        list.ResetCurrentRecord();
        while (list.SelectRecord(buf)) {
            n++;
            sprintf(buf, "MISSION%d", n);
            list.ResetCurrentRecord();
        }
    }
    if (n > index) {
        field_c1c = 0;
        missionIndex++;
        LoadMission(0);
        return 1;
    }
    return 0;
}

// Makes sure the map list at +0xd24 is loaded (dropping it first when the
// multiplayer flag changes back to 0), then passes it to LoadMissionByName.
// FUNCTION: 0x435d30
void Mission::RefreshMapList(int param_1)
{
    if (multi != 0 && param_1 == 0 && mapList != 0) {
        FUN_004d85a0((int*)mapList);
        mapList = 0;
    }
    if (mapList == 0) {
        mapCount = LoadMapList(&mapList, param_1, param_1);
    }
    LoadMissionByName(mapList);
    multi = param_1;
}

// Loads the current mission: resets the mission state, finds the
// mission's OTA file (from the campaign list entry MISSION<n> for type 1, or
// from the map name for types 2 and 3), reads its GlobalHeader block into the
// fields and the name slots (0x435430.cpp), and passes the schema to
// 0x436c30. Returns 1 on success, 0 after reporting an error. 0x435320
// (LoadBriefing) and 0x4356c0 (GetName) are inlined.
// The 0x48e010 callee is MissionConditions::RegisterConditions
// (data/symbols.csv).
// FUNCTION: 0x435da0
int Mission::LoadMission(char* map)
{
    TdfFile parser;
    char schema[0x20];
    char value[0x100];
    char path[0x100];
    MeteorParams meteor;
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
    // 0x437280 (the buffer reset) is written out here, not called: it is not auto-inlined.
    if (units)
        FUN_004d85a0(units);
    units = 0;
    unitCount = 0;
    if (rules)
        FUN_004d85a0(rules);
    rules = 0;
    ruleCount = 0;
    if (features)
        FUN_004d85a0(features);
    features = 0;
    featureCount = 0;
    if (briefing) {
        FUN_004d85a0(briefing);
        briefing = 0;
    }

    switch (type) {
    case 1: {
        char key[0x100];
        sprintf(key, "MISSION%d", missionIndex);
        list.ResetCurrentRecord();
        if (!list.SelectRecord(key)) {
            char msg[0x100];
            wsprintfA(msg, "The requested mission file, %s, does not exist.", key);
            OpenMessageBox(g_game->messages, msg, 0x1e0, 1, 1);
            return 0;
        }
        GetLocalizedString(&list, missionName, "missionname", 0x100, 0);
        int found = list.current->GetFieldString(path, "missionfile", 0x100, DAT_005119b8);
        if (found) {
            char file[0x100];
            BuildDataPath(file, "Maps", path, "OTA");
            if (!parser.LoadFile(file)) {
                char msg[0x100];
                sprintf(msg, "Hey, joker!  There is no mission defintion for this mission: %s", path);
                OpenMessageBox(g_game->messages, msg, 0x1e0, 1, 1);
                return 0;
            }
            parser.ResetCurrentRecord();
            if (parser.SelectRecord("GlobalHeader")) {
                g_game->maxUnits = parser.current->GetFieldInt("maxunits", 200);
                parser.ResetCurrentRecord();
                BuildCampaignFilePath(1, "Maps", path, "TNT");
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
        BuildDataPath(path, "Maps", map, "OTA");
        // The found-at-once arm breaks; only the fallback reassigns `map`.
        if (parser.LoadFile(path)) {
            BuildCampaignFilePath(1, "Maps", map, "TNT");
            break;
        }
        map = FindTranslation(map);
        if (map == 0)
            return 0;
        strcpy(missionName, map);
        BuildDataPath(path, "Maps", map, "OTA");
        if (!parser.LoadFile(path))
            return 0;
        BuildCampaignFilePath(1, "Maps", map, "TNT");
        break;
    case 0:
        return 0;
    default:
        return 0;
    }

    if (!parser.SelectRecord("GlobalHeader")) {
        OpenMessageBox(g_game->messages, "No GlobalHeader block in mission file!", 0x1e0, 1, 1);
        return 0;
    }
    field_c20 = *(int*)((char*)parser.current + 0x25);
    GetLocalizedString(&parser, value, "brief", 0x100, DAT_005119b8);
    BuildCampaignFilePath(2, "camps\\briefs", value, "TXT");
    LoadBriefing();
    GetLocalizedString(&parser, value, "narration", 0x100, DAT_005119b8);
    BuildCampaignFilePath(3, "camps\\briefs", value, "WAV");
    GetLocalizedString(&parser, value, "missionhint", 0x100, DAT_005119b8);
    BuildCampaignFilePath(4, "camps\\hints", value, "TXT");
    parser.current->GetFieldString(value, "glamour", 0x100, DAT_005119b8);
    BuildCampaignFilePath(5, DAT_005119b8, value, "PCX");
    parser.current->GetFieldString(value, "glamoursound", 0x100, DAT_005119b8);
    BuildCampaignFilePath(8, "camps\\briefs", value, "WAV");
    parser.current->GetFieldString(value, "UseOnlyUnits", 0x100, DAT_005119b8);
    BuildCampaignFilePath(6, "camps\\useonly", value, "TDF");
    g_game->mapping = parser.current->GetFieldInt("mapping", 0);
    g_game->lineOfSight = parser.current->GetFieldInt("lineofsight", 0);
    g_game->field_39225 = 1;
    g_game->field_39219 = 0;
    parser.current->GetFieldString(memory, "memory", 0x80, DAT_005119b8);
    parser.current->GetFieldString(numPlayers, "numplayers", 0x80, DAT_005119b8);
    parser.current->GetFieldString(planet, "Planet", 0x80, DAT_005119b8);
    g_game->noMovie = parser.current->GetFieldInt("nomovie", 0);
    parser.current->GetFieldString(desc, "missiondescription", 0x80, "No description available");
    strcpy(lower, desc);
    _strlwr(lower);
    strcpy(description, Translate(lower));
    if (_strcmpi(description, lower) == 0)
        strcpy(description, desc);
    minWindSpeed = parser.current->GetFieldInt("minwindspeed", 0);
    maxWindSpeed = parser.current->GetFieldInt("maxwindspeed", 0);
    gravity = parser.current->GetFieldInt("gravity", 0);
    tidalStrength = GetFloat(parser.current, "tidalstrength");
    lavaWorld = parser.current->GetFieldInt("lavaworld", 0);
    noSeaLevelTrigger = parser.current->GetFieldInt("nosealeveltrigger", 0);
    waterDoesDamage = parser.current->GetFieldInt("waterdoesdamage", 0);
    waterDamage = parser.current->GetFieldInt("waterdamage", 0);
    ((MissionConditions*)g_game->field_391ed)->RegisterConditions(&parser);
    killMul = GetFloat(parser.current, "killmul");
    timeMul = GetFloat(parser.current, "timemul");
    if (!SelectSchema(type, &parser, schema)) {
        ShowErrorBox("No suitable schema type in mission file!", "Map error");
        return 0;
    }
    humanMetal = (float)parser.current->GetFieldInt("HumanMetal", 0);
    humanEnergy = (float)parser.current->GetFieldInt("HumanEnergy", 0);
    computerMetal = (float)parser.current->GetFieldInt("ComputerMetal", 0);
    computerEnergy = (float)parser.current->GetFieldInt("ComputerEnergy", 0);
    surfaceMetal = parser.current->GetFieldInt("SurfaceMetal", 0);
    parser.current->GetFieldString(value, "aiprofile", 0x100, DAT_005119b8);
    BuildCampaignFilePath(7, "ai", value, "txt");
    if (!GetNameSlot(7))
        BuildCampaignFilePath(7, "ai", "default", "txt");
    parser.current->GetFieldString(meteor.name, "MeteorWeapon", 0x20, DAT_005119b8);
    if (strlen(meteor.name) != 0) {
        meteor.radius = parser.current->GetFieldInt("MeteorRadius", 0);
        meteor.density = GetFloat(parser.current, "MeteorDensity");
        meteor.duration = GetFloat(parser.current, "MeteorDuration");
        meteor.interval = GetFloat(parser.current, "MeteorInterval");
        if (meteor.radius == 0 || meteor.density == 0.0f || meteor.duration == 0.0f || meteor.interval == 0.0f)
            meteor.LoadMeteorDefaults();
        EnableMeteors();
    } else {
        DisableMeteors();
        meteor.LoadMeteorDefaults();
    }
    SetMeteorParams(&meteor);
    LoadMissionData(schema, &parser);
    return 1;
}

// Picks the mission file's "Schema <n>" block for a game type. Type 1 (a
// campaign mission) takes the first schema whose type is the current
// difficulty (then the others); types 2 and 3 (multiplayer) try the network
// schemas and keep the one whose count of StartPos specials fits the number
// of players best. The chosen block's name goes to `schema` and its section
// becomes the parser's current section. Returns 1 when one was found.
// FUNCTION: 0x436860
int Mission::SelectSchema(int type, TdfFile* parser, char* schema)
{
    int order[4];
    char name[0x10];
    char what[0x10];
    char kind[0x20];
    char* names[7];
    memset(order, -1, sizeof(order));
    names[0] = "Easy";
    names[1] = "Medium";
    names[2] = "Hard";
    names[3] = "Network 1";
    names[4] = "Network 2";
    names[5] = "Network 3";
    names[6] = "Network 4";
    int players = g_game->numPlayers;
    switch (type) {
    case 1:
        switch (g_game->difficulty) {
        case 0:
            order[0] = 0;
            order[1] = 1;
            order[2] = 2;
            break;
        case 1:
            order[0] = 1;
            order[1] = 0;
            order[2] = 2;
            break;
        case 2:
            order[0] = 2;
            order[1] = 1;
            order[2] = 0;
            break;
        default:
            return 0;
        }
        break;
    case 3: {
        for (int i = 0; i < 10; i++) {
            if (g_game->playerIds[i] != -1 && g_game->playerIds[i] != 0)
                players = i + 1;
        }
        order[0] = 3;
        order[1] = 4;
        order[2] = 5;
        order[3] = 6;
        break;
    }
    case 2: {
        for (int i = 0; i < 10; i++) {
            if (g_game->slots[i].active != 0)
                players = i + 1;
        }
        order[0] = 3;
        order[1] = 4;
        order[2] = 5;
        order[3] = 6;
        break;
    }
    case 0:
        return 0;
    default:
        return 0;
    }

    int bestCount = 0;
    int found = 0;
    TdfRecord* best = 0;
    for (int k = 0; k < 4; k++) {
        int d = order[k];
        if (d == -1)
            break;
        for (int n = 0; ; n++) {
            parser->ResetCurrentRecord();
            if (!parser->SelectRecord("GlobalHeader"))
                FatalError("Very bad news!  No MSG!");
            sprintf(name, "Schema %i", n);
            if (!parser->SelectRecord(name))
                break;
            if (!parser->current->GetFieldString(kind, "type", 0x20, DAT_005119b8))
                continue;
            if (_strcmpi(kind, names[d]) != 0)
                continue;
            if (type != 3 && type != 2 || schema == 0) {
                if (schema)
                    strcpy(schema, name);
                return 1;
            }
            TdfRecord* section = parser->current;
            int count = 0;
            if (parser->SelectRecord("specials")) {
                Class_004c44c0* specials = (Class_004c44c0*)parser->current;
                TdfRecord* s;
                for (int i = 0; (s = specials->GetSubRecord(i)) != 0; i++) {
                    if (s->GetFieldString(what, "specialwhat", 0x10, DAT_005119b8)) {
                        static int len = strlen("StartPos");
                        if (_strnicmp(what, "StartPos", len) == 0)
                            count++;
                    }
                }
            }
            if (count != 0 && (count == players || players == 0 || (count > bestCount && bestCount != players))) {
                best = section;
                bestCount = count;
                found = 1;
                strcpy(schema, name);
            }
        }
    }
    if (best)
        parser->current = best;
    return found;
}

// Loads a mission's units, special rules and features from the parsed TDF:
// selects [globalheader] and then the mission's own section, and copies the
// "units", "specials" and "features" subsections into three tables. The
// unit strings are packed after the unit records in the same allocation.
// FUNCTION: 0x436c30
void Mission::LoadMissionData(char* name, TdfFile* parser)
{
    char text[0x100];
    char buf[0x400];
    int i;
    int count;
    int total = 0;

    parser->ResetCurrentRecord();
    if (!parser->SelectRecord("globalheader"))
        return;
    if (!parser->SelectRecord(name))
        return;
    TdfRecord* root = parser->current;

    TdfRecord* list = root->FindSubRecord("units");
    if (list)
        count = ((Class_004c4450*)list)->GetSubRecordCount();
    else
        count = 0;
    for (i = 0; i < count; i++) {
        TdfRecord* s = ((Class_004c44c0*)list)->GetSubRecord(i);
        if (s->GetFieldString(buf, "Unitname", 0x400, DAT_005119b8))
            total += strlen(buf) + 1;
        if (s->GetFieldString(buf, "Ident", 0x400, DAT_005119b8))
            total += strlen(buf) + 1;
        if (s->GetFieldString(buf, "InitialMission", 0x400, DAT_005119b8))
            total += strlen(buf) + 1;
    }
    // Own local, computed before unitCount is stored: keeps the count in ecx.
    int unitBytes = count * sizeof(MissionUnit);
    unitCount = count;
    units = (MissionUnit*)FUN_004d83b0("MISSIONUNIT DATA",
        (total / 0x400 + 2) * 0x400 + unitBytes);
    char* strings = (char*)units + unitBytes;
    for (i = 0; i < count; i++) {
        MissionUnit* u = &units[i];
        TdfRecord* s = ((Class_004c44c0*)list)->GetSubRecord(i);
        if (s->GetFieldString(strings, "Unitname", 0x400, DAT_005119b8)) {
            u->name = strings;
            strings += strlen(strings) + 1;
        } else {
            u->name = 0;
        }
        if (s->GetFieldString(strings, "Ident", 0x400, DAT_005119b8)) {
            u->ident = strings;
            strings += strlen(strings) + 1;
        } else {
            u->ident = 0;
        }
        if (s->GetFieldString(strings, "InitialMission", 0x400, DAT_005119b8)) {
            u->initialMission = strings;
            strings += strlen(strings) + 1;
        } else {
            u->initialMission = 0;
        }
        u->x = s->GetFieldInt("XPos", 0) << 16;
        u->y = s->GetFieldInt("YPos", 0) << 16;
        u->z = s->GetFieldInt("ZPos", 0) << 16;
        u->angle = (s->GetFieldInt("Angle", 0) << 16) / 360;
        u->player = s->GetFieldInt("Player", 0);
        if (!u->player)
            u->player = 1;
        u->health = s->GetFieldInt("HealthPercentage", 100);
        u->buildPriority = s->GetFieldInt("BuildPriority", 0);
        u->creationCountdown = s->GetFieldInt("CreationCountdown", 0);
        u->missionCritical = s->GetFieldInt("MissionCriticalUnit", 0);
        u->aiIgnore = s->GetFieldInt("AiIgnore", 0);
        u->aiPriorityTarget = s->GetFieldInt("AiPriorityTarget", 0);
        u->initialGroup = s->GetFieldInt("InitialGroup", 0);
        u->immunity = s->GetFieldInt("Immunity", 0);
    }

    list = root->FindSubRecord("specials");
    if (list)
        count = ((Class_004c4450*)list)->GetSubRecordCount();
    else
        count = 0;
    ruleCount = count;
    rules = (MissionRule*)FUN_004d83b0("MISSIONRULE DATA", count * sizeof(MissionRule));
    int startPos = 0;
    for (i = 0; i < count; i++) {
        MissionRule* r = &rules[i];
        r->type = 0;
        TdfRecord* s = ((Class_004c44c0*)list)->GetSubRecord(i);
        if (s->GetFieldString(text, "specialwhat", 0x100, DAT_005119b8)) {
            static int len = strlen("StartPos");
            if (_strnicmp(text, "StartPos", len) == 0) {
                r->type = 1;
                r->x = s->GetFieldInt("XPos", 0);
                r->z = s->GetFieldInt("ZPos", 0);
                int id;
                if (!isdigit(text[len]))
                    id = ++startPos;
                else
                    id = atoi(text + len);
                r->id = id;
                if (id > 0)
                    r->id = id - 1;
            }
        }
    }

    list = root->FindSubRecord("features");
    if (list)
        count = ((Class_004c4450*)list)->GetSubRecordCount();
    else
        count = 0;
    featureCount = count;
    if (features)
        FUN_004d85a0(features);
    features = (MissionFeature*)FUN_004d83b0("MISSIONFEATURE DATA", count * sizeof(MissionFeature));
    for (i = 0; i < count; i++) {
        MissionFeature* f = &features[i];
        TdfRecord* s = ((Class_004c44c0*)list)->GetSubRecord(i);
        if (!s->GetFieldString(f->name, "Featurename", 0x80, DAT_005119b8))
            f->name[0] = 0;
        f->x = s->GetFieldInt("XPos", -1);
        f->z = s->GetFieldInt("ZPos", -1);
        if (f->x < 0 || f->z < 0)
            f->name[0] = 0;
    }
}

// Frees three buffers (clearing each pointer and its size) and a fourth
// buffer at +0xc14.
// FUNCTION: 0x437280
void Mission::FreeMissionData()
{
    // Written out in full, no Free() helper: the two clearing stores stay together.
    if (units)
        FUN_004d85a0(units);
    units = 0;
    unitCount = 0;
    if (rules)
        FUN_004d85a0(rules);
    rules = 0;
    ruleCount = 0;
    if (features)
        FUN_004d85a0(features);
    features = 0;
    featureCount = 0;
    if (briefing) {
        FUN_004d85a0(briefing);
        briefing = 0;
    }
}

// Counts the type-1 rules (see GetStartPosition).
// FUNCTION: 0x437300
int Mission::CountStartPositions()
{
    int n = 0;
    for (int i = 0; i < ruleCount; i++) {
        if (rules[i].type == 1) {
            n++;
        }
    }
    return n;
}

// Finds the type-1 entry with the given id and returns its position (16.16
// fixed point, y = 0) in `out`. Returns 0 if there is none.
// FUNCTION: 0x437320
int Mission::GetStartPosition(Vec3_00437320* out, int id)
{
    for (int i = 0; i < ruleCount; i++) {
        if (rules[i].type == 1 && rules[i].id == id) {
            out->x = rules[i].x << 16;
            out->z = rules[i].z << 16;
            out->y = 0;
            return 1;
        }
    }
    return 0;
}

// Map cache lookup for the campaign object (`this` is the campaign at
// g_game+0x391e9, the class 0x435c00.cpp calls Mission). +0xc1c is
// both the running checksum and the "already loaded" flag, so a second call
// returns straight away. Otherwise name slot 1 is searched in the file-local
// vector of {name, checksum} pairs at 0x5122c0 (its initialiser and atexit
// destructor are map_list.cpp, its out-of-line insert is map_load.cpp): a hit
// takes the stored checksum, a miss opens the file, checks the 0x2000 magic of
// its 0x40-byte header and folds the checksums (0x4b6ba0) of the header, of
// the plot data (width * height * 4 bytes at the offset in +0x10) and of the
// feature data (features * 0x84 bytes at the offset in +0x20) into +0xc1c,
// then appends the new pair. The result is +0xc20 xor +0xc1c throughout.
//
// SetChecksum is not a real method: it is how the new entry gets +0xc1c.
// FUNCTION: 0x4373a0
int Mission::ComputeMapChecksum()
{
    if (field_c1c != 0) {
        return field_c20 ^ field_c1c;
    }
    char* name = GetNameSlot(1);
    MapCacheEntry* it;
    // The two pointers stay separate globals: a single std::vector would
    // reference s_mapCache with a displacement, the wrong address.
    for (it = g_mapCacheBegin; it != g_mapCacheEnd; it++) {
        if (_strcmpi(it->handle.data, name) == 0) {
            field_c1c = it->field_4;
            return field_c20 ^ field_c1c;
        }
    }
    void* file = HAPI_OpenFileRead(name);
    if (file == 0) {
        return 0;
    }
    Header_004373a0 header;
    HAPI_readfromfile(file, &header, 0x40);
    if (header.magic != 0x2000) {
        return 0;
    }
    field_c1c = field_c1c ^ ComputeChecksum((unsigned char*)&header, 0x40);
    int size = header.width * header.height * 4;
    void* data = FUN_004d83b0("Raw Plot Data", size);
    HAPI_SeekFile(file, header.plotOffset);
    HAPI_readfromfile(file, data, size);
    field_c1c = field_c1c ^ ComputeChecksum((unsigned char*)data, size);
    FUN_004d85a0(data);
    size = header.features * 0x84;
    if (size > 0) {
        data = FUN_004d83b0("Raw Feature Data", size);
        HAPI_SeekFile(file, header.featureOffset);
        HAPI_readfromfile(file, data, size);
        field_c1c = field_c1c ^ ComputeChecksum((unsigned char*)data, size);
        FUN_004d85a0(data);
    }
    HAPI_CloseFile(file);
    // The checksum is stored after the copy constructor, not inside it:
    // a two-argument constructor hoists the +0xc1c load above the call.
    s_mapCache.FUN_00437580(g_mapCacheEnd, 1, MapCacheEntry(Class_004c91b0(name)).SetChecksum(this));
    return field_c20 ^ field_c1c;
}
