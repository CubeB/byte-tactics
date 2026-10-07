// Decompiled by Claude Opus 5.5. Names are provisional.
// Resolves every feature type's "featuredead", "featurereclamate" and
// "featureburnt" names to feature type numbers (loading them when needed),
// updating the load progress byte as it goes.
// Must stay the only header.
#include <string.h>

class TdfFile {
public:
    char unknown_0[4];
    void* parser;                       // +0x4

    void ResetCurrentRecord();
    int SelectRecord(char* name);
};

class TdfRecord {
public:
    int GetFieldString(char* dst, char* key, int size, char* def);
};

struct List_00422ea0 {
    int unknown_0;
    TdfFile** first;                    // +0x4
    TdfFile** last;                     // +0x8
};

struct FeatureDef_00422ea0 {
    char name[0xf4];
    unsigned short dead;                // +0xf4
    unsigned short burnt;               // +0xf6
    unsigned short reclamate;           // +0xf8
    char unknown_fa[0x100 - 0xfa];
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x14253];
    int featureCount;                   // +0x14253
    char unknown_14257[0x1426f - 0x14257];
    FeatureDef_00422ea0* features;      // +0x1426f
    char unknown_14273[0x38d72 - 0x14273];
    unsigned char loadPercent;          // +0x38d72
};
#pragma pack(pop)

extern Game* g_game;
extern List_00422ea0* DAT_00511fb4;
extern char DAT_005119b8[];

unsigned short __stdcall LoadFeatureType(char* name);

// FindFeatureFile, inlined
static inline TdfFile* FindEntry(char* name)
{
    for (TdfFile** p = DAT_00511fb4->first; p < DAT_00511fb4->last; p++) {
        (*p)->ResetCurrentRecord();
        if (((TdfFile*)*p)->SelectRecord(name))
            return *p;
    }
    return 0;
}

// FindFeatureType-like search, inlined
static inline unsigned short FindName(char* name)
{
    for (int i = 0; i < g_game->featureCount; i++) {
        if (_strcmpi(name, g_game->features[i].name) == 0) {
            return (unsigned short)i;
        }
    }
    return 0xffff;
}

// FindOrLoadFeatureType, inlined
static inline unsigned short FeatureIndex(char* name)
{
    unsigned short i = FindName(name);
    if (i != 0xffff)
        return i;
    return LoadFeatureType(name);
}

// Do/while behind its own guard, table pointer taken after the guard; i = 0 is
// stored before the count pointer.
// FUNCTION: 0x422ea0
void ResolveFeatureLinks()
{
    char buf[0x100];
    FeatureDef_00422ea0** features;
    int i = 0;
    int* count = &g_game->featureCount;
    if (*count > 0) {
        features = &g_game->features;
        do {
            TdfRecord* parser = (TdfRecord*)FindEntry((*features)[i].name)->parser;
            if (parser->GetFieldString(buf, "featuredead", 0x100, DAT_005119b8))
                (*features)[i].dead = FeatureIndex(buf);
            else
                (*features)[i].dead = 0xffff;
            if (parser->GetFieldString(buf, "featurereclamate", 0x100, DAT_005119b8))
                (*features)[i].reclamate = FeatureIndex(buf);
            else
                (*features)[i].reclamate = 0xffff;
            if (parser->GetFieldString(buf, "featureburnt", 0x100, DAT_005119b8))
                (*features)[i].burnt = FeatureIndex(buf);
            else
                (*features)[i].burnt = 0xffff;
            g_game->loadPercent = (unsigned char)((i + 1) * 100 / *count);
            i++;
        } while (i < *count);
    }
}
