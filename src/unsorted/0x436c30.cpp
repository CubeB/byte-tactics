// Decompiled by Claude Opus 5.5. Names are provisional.
// Loads a mission's units, special rules and features from the parsed TDF:
// selects [globalheader] and then the mission's own section, and copies the
// "units", "specials" and "features" subsections into three tables. The
// unit strings are packed after the unit records in the same allocation.
// The unit table's byte size is its own local, computed before unitCount is
// stored: that keeps the count in ecx at the end of the first loop.
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

class Section_00436c30;

class Class_004c3e10 {
public:
    void FUN_004c3e10();
};

class Class_004c3410 {
public:
    int FUN_004c3410(char* name);
};

class Class_004c4470 {
public:
    Section_00436c30* FUN_004c4470(const char* name);
};

class Class_004c4450 {
public:
    int FUN_004c4450();
};

class Class_004c44c0 {
public:
    Section_00436c30* FUN_004c44c0(int index);
};

class Class_004c48c0 {
public:
    int FUN_004c48c0(char* dst, char* key, size_t size, char* def);
};

class Class_004c46c0 {
public:
    int FUN_004c46c0(const char* name, int def);
};

class Section_00436c30 {
public:
    int unknown_0;
};

struct Parser_00436c30 {
    int unknown_0;
    Section_00436c30* current;         // +0x4
};

struct MissionUnit_00436c30 {
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

struct MissionRule_00436c30 {
    int type;                          // +0x0
    int id;                            // +0x4
    short x;                           // +0x8
    short z;                           // +0xa
};

struct MissionFeature_00436c30 {
    char name[0x80];                   // +0x0
    int x;                             // +0x80
    int z;                             // +0x84
};

extern char DAT_005119b8[];

void* __cdecl FUN_004d83b0(char* name, unsigned int size);
void __cdecl FUN_004d85a0(void* p);

class Class_00436c30 {
public:
    char unknown_0[0xdac];
    MissionUnit_00436c30* units;       // +0xdac
    int unitCount;                     // +0xdb0
    MissionRule_00436c30* rules;       // +0xdb4
    int ruleCount;                     // +0xdb8
    MissionFeature_00436c30* features; // +0xdbc
    int featureCount;                  // +0xdc0

    void FUN_00436c30(char* name, Parser_00436c30* parser);
};

// FUNCTION: 0x436c30
void Class_00436c30::FUN_00436c30(char* name, Parser_00436c30* parser)
{
    char text[0x100];
    char buf[0x400];
    int i;
    int count;
    int total = 0;

    ((Class_004c3e10*)parser)->FUN_004c3e10();
    if (!((Class_004c3410*)parser)->FUN_004c3410("globalheader"))
        return;
    if (!((Class_004c3410*)parser)->FUN_004c3410(name))
        return;
    Section_00436c30* root = parser->current;

    Section_00436c30* list = ((Class_004c4470*)root)->FUN_004c4470("units");
    if (list)
        count = ((Class_004c4450*)list)->FUN_004c4450();
    else
        count = 0;
    for (i = 0; i < count; i++) {
        Section_00436c30* s = ((Class_004c44c0*)list)->FUN_004c44c0(i);
        if (((Class_004c48c0*)s)->FUN_004c48c0(buf, "Unitname", 0x400, DAT_005119b8))
            total += strlen(buf) + 1;
        if (((Class_004c48c0*)s)->FUN_004c48c0(buf, "Ident", 0x400, DAT_005119b8))
            total += strlen(buf) + 1;
        if (((Class_004c48c0*)s)->FUN_004c48c0(buf, "InitialMission", 0x400, DAT_005119b8))
            total += strlen(buf) + 1;
    }
    int unitBytes = count * sizeof(MissionUnit_00436c30);
    unitCount = count;
    units = (MissionUnit_00436c30*)FUN_004d83b0("MISSIONUNIT DATA",
        (total / 0x400 + 2) * 0x400 + unitBytes);
    char* strings = (char*)units + unitBytes;
    for (i = 0; i < count; i++) {
        MissionUnit_00436c30* u = &units[i];
        Section_00436c30* s = ((Class_004c44c0*)list)->FUN_004c44c0(i);
        if (((Class_004c48c0*)s)->FUN_004c48c0(strings, "Unitname", 0x400, DAT_005119b8)) {
            u->name = strings;
            strings += strlen(strings) + 1;
        } else {
            u->name = 0;
        }
        if (((Class_004c48c0*)s)->FUN_004c48c0(strings, "Ident", 0x400, DAT_005119b8)) {
            u->ident = strings;
            strings += strlen(strings) + 1;
        } else {
            u->ident = 0;
        }
        if (((Class_004c48c0*)s)->FUN_004c48c0(strings, "InitialMission", 0x400, DAT_005119b8)) {
            u->initialMission = strings;
            strings += strlen(strings) + 1;
        } else {
            u->initialMission = 0;
        }
        u->x = ((Class_004c46c0*)s)->FUN_004c46c0("XPos", 0) << 16;
        u->y = ((Class_004c46c0*)s)->FUN_004c46c0("YPos", 0) << 16;
        u->z = ((Class_004c46c0*)s)->FUN_004c46c0("ZPos", 0) << 16;
        u->angle = (((Class_004c46c0*)s)->FUN_004c46c0("Angle", 0) << 16) / 360;
        u->player = ((Class_004c46c0*)s)->FUN_004c46c0("Player", 0);
        if (!u->player)
            u->player = 1;
        u->health = ((Class_004c46c0*)s)->FUN_004c46c0("HealthPercentage", 100);
        u->buildPriority = ((Class_004c46c0*)s)->FUN_004c46c0("BuildPriority", 0);
        u->creationCountdown = ((Class_004c46c0*)s)->FUN_004c46c0("CreationCountdown", 0);
        u->missionCritical = ((Class_004c46c0*)s)->FUN_004c46c0("MissionCriticalUnit", 0);
        u->aiIgnore = ((Class_004c46c0*)s)->FUN_004c46c0("AiIgnore", 0);
        u->aiPriorityTarget = ((Class_004c46c0*)s)->FUN_004c46c0("AiPriorityTarget", 0);
        u->initialGroup = ((Class_004c46c0*)s)->FUN_004c46c0("InitialGroup", 0);
        u->immunity = ((Class_004c46c0*)s)->FUN_004c46c0("Immunity", 0);
    }

    list = ((Class_004c4470*)root)->FUN_004c4470("specials");
    if (list)
        count = ((Class_004c4450*)list)->FUN_004c4450();
    else
        count = 0;
    ruleCount = count;
    rules = (MissionRule_00436c30*)FUN_004d83b0("MISSIONRULE DATA", count * sizeof(MissionRule_00436c30));
    int startPos = 0;
    for (i = 0; i < count; i++) {
        MissionRule_00436c30* r = &rules[i];
        r->type = 0;
        Section_00436c30* s = ((Class_004c44c0*)list)->FUN_004c44c0(i);
        if (((Class_004c48c0*)s)->FUN_004c48c0(text, "specialwhat", 0x100, DAT_005119b8)) {
            static int len = strlen("StartPos");
            if (_strnicmp(text, "StartPos", len) == 0) {
                r->type = 1;
                r->x = ((Class_004c46c0*)s)->FUN_004c46c0("XPos", 0);
                r->z = ((Class_004c46c0*)s)->FUN_004c46c0("ZPos", 0);
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

    list = ((Class_004c4470*)root)->FUN_004c4470("features");
    if (list)
        count = ((Class_004c4450*)list)->FUN_004c4450();
    else
        count = 0;
    featureCount = count;
    if (features)
        FUN_004d85a0(features);
    features = (MissionFeature_00436c30*)FUN_004d83b0("MISSIONFEATURE DATA", count * sizeof(MissionFeature_00436c30));
    for (i = 0; i < count; i++) {
        MissionFeature_00436c30* f = &features[i];
        Section_00436c30* s = ((Class_004c44c0*)list)->FUN_004c44c0(i);
        if (!((Class_004c48c0*)s)->FUN_004c48c0(f->name, "Featurename", 0x80, DAT_005119b8))
            f->name[0] = 0;
        f->x = ((Class_004c46c0*)s)->FUN_004c46c0("XPos", -1);
        f->z = ((Class_004c46c0*)s)->FUN_004c46c0("ZPos", -1);
        if (f->x < 0 || f->z < 0)
            f->name[0] = 0;
    }
}
