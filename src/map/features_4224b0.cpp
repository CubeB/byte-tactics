// Decompiled by Claude Opus 5.5. Names are provisional.
// Stays in its own file: in features.cpp the merged file's symbol ids schedule
// the `name` load after the strncpy setup instead of before it
// (docs/c2-regalloc.md, "Symbol ids").
// Loads the feature type `name` from the feature files: appends a 0x100-byte
// record to the feature table in g_game, fills it from the record's TDF
// fields (3D object, or GAF file and sequences, flags, resources, burn
// weapon) and returns the new feature type number.
// <windows.h> must come before <stdio.h>: it decides the registers of
// `features[j].anims`.
#include <windows.h>
#include <stdio.h>
#include <string.h>

class TdfRecord {
public:
    int GetFieldString(char* dst, char* key, int size, char* def);
    int GetFieldInt(const char* name, int def);
    double GetFieldDouble(const char* name, double def);
};

class TdfFile {
public:
    char unknown_0[4];
    TdfRecord* parser;                  // +0x4

    void ResetCurrentRecord();
    int SelectRecord(char* name);
};

struct List_004224b0 {
    int unknown_0;
    TdfFile** first;                    // +0x4
    TdfFile** last;                     // +0x8
};

struct Seq_004224b0 {
    unsigned short count;               // +0x0
    unsigned char kind;                 // +0x2
};

struct Gaf_004224b0;

struct Ref_004224b0 {
    unsigned short index;               // +0x0
    unsigned short value;               // +0x2
    unsigned char kind;                 // +0x4
    char unknown_5[3];
    Seq_004224b0* src;                  // +0x8
};

struct FeatureDef_004224b0 {
    char name[0x80];                    // +0x00
    char description[0x14];             // +0x80
    short footprintx;                   // +0x94
    short footprintz;                   // +0x96
    union {
        void* object;                   // +0x98
        char filename[0x10];            // +0x98
    };
    Gaf_004224b0* anims;                // +0xa8
    Seq_004224b0* seq;                  // +0xac
    Seq_004224b0* seqshad;              // +0xb0
    Seq_004224b0* seqburn;              // +0xb4
    Seq_004224b0* seqburnshad;          // +0xb8
    Seq_004224b0* seqdie;               // +0xbc
    Seq_004224b0* seqdieshad;           // +0xc0
    Seq_004224b0* seqreclamate;         // +0xc4
    Seq_004224b0* seqreclamateshad;     // +0xc8
    Ref_004224b0 ref;                   // +0xcc
    Ref_004224b0 refshad;               // +0xd8
    char* burnweapon;                   // +0xe4
    short sparktime;                    // +0xe8
    short damage;                       // +0xea
    float energy;                       // +0xec
    float metal;                        // +0xf0
    unsigned short dead;                // +0xf4
    unsigned short burnt;               // +0xf6
    unsigned short reclamate;           // +0xf8
    char height;                        // +0xfa
    char spreadchance;                  // +0xfb
    char reproduce;                     // +0xfc
    char reproducearea;                 // +0xfd
    unsigned short noobject : 1;        // +0xfe
    unsigned short animating : 1;
    unsigned short animtrans : 1;
    unsigned short shadtrans : 1;
    unsigned short flamable : 1;
    unsigned short geothermal : 1;
    unsigned short blocking : 1;
    unsigned short reclaimable : 1;
    unsigned short autoreclaimable : 1;
    unsigned short indestructible : 1;
    unsigned short nodisplayinfo : 1;
    unsigned short nodrawundergray : 1;
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x14253];
    int featureCount;                   // +0x14253
    char unknown_14257[0x1426f - 0x14257];
    FeatureDef_004224b0* features;      // +0x1426f
};
#pragma pack(pop)

extern Game* g_game;
extern List_004224b0* s_featureTdfParsers;
extern char DAT_005119b8[];

void __stdcall BuildDataPath(char* out, const char* dir, const char* name, const char* ext);
void __stdcall FatalError(char* path);
void* __cdecl GameReallocTagged(void* p, const char* name, unsigned int size);
void* __stdcall LoadObject3d(const char* name);
Gaf_004224b0* __stdcall LoadGaf(char* path);
Seq_004224b0* __stdcall FindGafEntry(Gaf_004224b0* gaf, const char* name);
char* __stdcall FindWeaponByName(char* name);
void __stdcall InitGafSequence(Ref_004224b0* ref, Seq_004224b0* src, int index);

// FindFeatureFile, inlined
static inline TdfFile* FindEntry(char* name)
{
    for (TdfFile** p = s_featureTdfParsers->first; p < s_featureTdfParsers->last; p++) {
        (*p)->ResetCurrentRecord();
        if ((*p)->SelectRecord(name))
            return *p;
    }
    return 0;
}

// FindOptionalGafEntry, inlined
static inline Seq_004224b0* SeqByName(Gaf_004224b0* gaf, int unused, char* name)
{
    if (strlen(name) == 0) {
        return 0;
    }
    return FindGafEntry(gaf, name);
}

// Reads one seqname-style field into *out: the sequence named by key from the
// feature's GAF, or 0. The tested lookup goes through an int local: it changes
// the zero compare.
static inline void LoadFeatureSeq(TdfRecord* rec, char* key, char* seqname, Gaf_004224b0* anims, Seq_004224b0** out)
{
    int ok = rec->GetFieldString(seqname, key, 0x100, DAT_005119b8);
    if (ok)
        *out = SeqByName(anims, 0, seqname);
    else
        *out = 0;
}

// The same read for the burn, die and reclaim sequences, which play once: the
// sequence's loop flag is cleared.
static inline void LoadFeatureSeqOnce(TdfRecord* rec, char* key, char* seqname, Gaf_004224b0* anims, Seq_004224b0** out)
{
    int ok = rec->GetFieldString(seqname, key, 0x100, DAT_005119b8);
    if (ok) {
        *out = SeqByName(anims, 0, seqname);
        if (*out)
            (*out)->kind = 0;
    } else {
        *out = 0;
    }
}

// The return value is the full old count (0x422e40.cpp declares it unsigned
// short).
// The sequence references must be cleared field by field, not by a Reset()
// method or a pointer local.
// FUNCTION: 0x4224b0
int __stdcall LoadFeatureType(char* name)
{
    Gaf_004224b0* anims;
    int j;
    // Tested TDF lookups go through an int local: it changes the zero compare.
    int ok;
    char seqname[0x100];
    char file[0x100];
    char path[0x100];

    TdfFile* entry = FindEntry(name);
    if (entry == 0) {
        sprintf(path, "Record \"%s\" missing from feature files", name);
        FatalError(path);
    }
    g_game->features = (FeatureDef_004224b0*)GameReallocTagged(g_game->features, "FEATURES",
                                                          (g_game->featureCount + 1) * 0x100);
    FeatureDef_004224b0* def = &g_game->features[g_game->featureCount];
    strncpy(def->name, name, 0x80);
    entry->parser->GetFieldString(def->description, "Description", 0x14, DAT_005119b8);
    def->footprintx = entry->parser->GetFieldInt("footprintx", 0);
    def->footprintz = entry->parser->GetFieldInt("footprintz", 0);
    def->height = entry->parser->GetFieldInt("height", 0);
    ok = entry->parser->GetFieldString(file, "object", 0x100, DAT_005119b8);
    if (ok) {
        def->noobject = 0;
        def->object = LoadObject3d(file);
    } else {
        def->noobject = 1;
        entry->parser->GetFieldString(file, "filename", 0x100, DAT_005119b8);
        for (j = 0; j < g_game->featureCount; j++) {
            if (strncmp(g_game->features[j].filename, file, 0x10) == 0) {
                def->anims = 0;
                anims = g_game->features[j].anims;
                strcpy(def->filename, "REUSE");
                break;
            }
        }
        if (j == g_game->featureCount) {
            BuildDataPath(path, "anims", file, "GAF");
            def->anims = LoadGaf(path);
            strncpy(def->filename, file, 0x10);
            anims = def->anims;
        }
        LoadFeatureSeq(entry->parser, "seqname", seqname, anims, &def->seq);
        LoadFeatureSeq(entry->parser, "seqnameshad", seqname, anims, &def->seqshad);
        LoadFeatureSeqOnce(entry->parser, "seqnameburn", seqname, anims, &def->seqburn);
        LoadFeatureSeqOnce(entry->parser, "seqnameburnshad", seqname, anims, &def->seqburnshad);
        LoadFeatureSeqOnce(entry->parser, "seqnamedie", seqname, anims, &def->seqdie);
        LoadFeatureSeqOnce(entry->parser, "seqnamedieshad", seqname, anims, &def->seqdieshad);
        LoadFeatureSeqOnce(entry->parser, "seqnamereclamate", seqname, anims, &def->seqreclamate);
        LoadFeatureSeqOnce(entry->parser, "seqnamereclamateshad", seqname, anims, &def->seqreclamateshad);
    }
    def->spreadchance = entry->parser->GetFieldInt("spreadchance", 0);
    def->reproduce = entry->parser->GetFieldInt("reproduce", 0);
    def->reproducearea = entry->parser->GetFieldInt("reproducearea", 0);
    def->metal = (unsigned short)entry->parser->GetFieldInt("metal", 0);
    def->energy = (unsigned short)entry->parser->GetFieldInt("energy", 0);
    def->damage = entry->parser->GetFieldInt("damage", 0);
    def->animating = entry->parser->GetFieldInt("animating", 0);
    def->animtrans = entry->parser->GetFieldInt("animtrans", 0);
    def->shadtrans = entry->parser->GetFieldInt("shadtrans", 0);
    def->flamable = entry->parser->GetFieldInt("flamable", 0);
    def->geothermal = entry->parser->GetFieldInt("geothermal", 0);
    def->blocking = entry->parser->GetFieldInt("blocking", 0);
    def->reclaimable = entry->parser->GetFieldInt("reclaimable", 0);
    def->autoreclaimable = entry->parser->GetFieldInt("autoreclaimable", 1);
    def->indestructible = entry->parser->GetFieldInt("indestructible", 0);
    def->nodisplayinfo = entry->parser->GetFieldInt("nodisplayinfo", 0);
    def->nodrawundergray = entry->parser->GetFieldInt("nodrawundergray", 0);
    if (_strcmpi(name, "DragonsTeeth") == 0)
        def->nodrawundergray = 1;
    if (_strcmpi(name, "DragonsTeeth_Core") == 0)
        def->nodrawundergray = 1;
    if (_strcmpi(name, "Fortification") == 0)
        def->nodrawundergray = 1;
    if (_strcmpi(name, "Fortification_Core") == 0)
        def->nodrawundergray = 1;
    def->sparktime = (short)(entry->parser->GetFieldDouble("sparktime", 0.0) * 30.0);
    entry->parser->GetFieldString(seqname, "burnweapon", 0x100, DAT_005119b8);
    def->burnweapon = FindWeaponByName(seqname);
    def->ref.index = 0;
    def->ref.value = 0;
    def->ref.kind = 0;
    def->ref.src = 0;
    def->refshad.index = 0;
    def->refshad.value = 0;
    def->refshad.kind = 0;
    def->refshad.src = 0;
    if (def->animating) {
        if (def->seq)
            InitGafSequence(&def->ref, def->seq, 0);
        if (def->seqshad)
            InitGafSequence(&def->refshad, def->seqshad, 0);
    }
    return g_game->featureCount++;
}
