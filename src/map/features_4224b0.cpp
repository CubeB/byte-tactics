// Decompiled by Claude Opus 5.5. Names are provisional.
// Loads the feature type `name` from the feature files: appends a 0x100-byte
// record to the feature table in g_game, fills it from the record's TDF
// fields (3D object, or GAF file and sequences, flags, resources, burn
// weapon) and returns the new feature type number.
#include <windows.h>
#include <stdio.h>
#include <string.h>

class Class_004c3e10 {
public:
    char unknown_0[4];
    void* parser;                       // +0x4

    void FUN_004c3e10();
};

class Class_004c3410 {
public:
    int FUN_004c3410(char* name);
};

class Class_004c48c0 {
public:
    int FUN_004c48c0(char* dst, char* key, int size, char* def);
};

class Class_004c46c0 {
public:
    int FUN_004c46c0(const char* name, int def);
};

class Class_004c4760 {
public:
    double FUN_004c4760(const char* name, double def);
};

struct List_004224b0 {
    int unknown_0;
    Class_004c3e10** first;             // +0x4
    Class_004c3e10** last;              // +0x8
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
extern List_004224b0* DAT_00511fb4;
extern char DAT_005119b8[];

void __stdcall BuildDataPath(char* out, const char* dir, const char* name, const char* ext);
void __stdcall FatalError(char* path);
void* __cdecl FUN_004d84a0(void* p, const char* name, unsigned int size);
void* __stdcall LoadObject3d(const char* name);
Gaf_004224b0* __stdcall LoadGaf(char* path);
Seq_004224b0* __stdcall FindGafEntry(Gaf_004224b0* gaf, const char* name);
char* __stdcall FUN_0049e5b0(char* name);
void __stdcall InitGafSequence(Ref_004224b0* ref, Seq_004224b0* src, int index);

// FUN_00422460, inlined
static inline Class_004c3e10* FindEntry(char* name)
{
    for (Class_004c3e10** p = DAT_00511fb4->first; p < DAT_00511fb4->last; p++) {
        (*p)->FUN_004c3e10();
        if (((Class_004c3410*)*p)->FUN_004c3410(name))
            return *p;
    }
    return 0;
}

// FUN_004222b0, inlined
static inline Seq_004224b0* SeqByName(Gaf_004224b0* gaf, int unused, char* name)
{
    if (strlen(name) == 0) {
        return 0;
    }
    return FindGafEntry(gaf, name);
}

// The TDF lookups that are tested go through an int local (`cmp eax, esi`
// against the zero register rather than `test eax, eax`). The two sequence
// references are cleared field by field; an inline Reset() method or a
// pointer local stores through the `lea` register instead. The return value
// is the full old count (0x422e40.cpp declares it unsigned short).
// <windows.h> before <stdio.h> decides the base and index of
// `features[j].anims`.
// FUNCTION: 0x4224b0
int __stdcall FUN_004224b0(char* name)
{
    Gaf_004224b0* anims;
    int j;
    int ok;
    char seqname[0x100];
    char file[0x100];
    char path[0x100];

    Class_004c3e10* entry = FindEntry(name);
    if (entry == 0) {
        sprintf(path, "Record \"%s\" missing from feature files", name);
        FatalError(path);
    }
    g_game->features = (FeatureDef_004224b0*)FUN_004d84a0(g_game->features, "FEATURES",
                                                          (g_game->featureCount + 1) * 0x100);
    FeatureDef_004224b0* def = &g_game->features[g_game->featureCount];
    strncpy(def->name, name, 0x80);
    ((Class_004c48c0*)entry->parser)->FUN_004c48c0(def->description, "Description", 0x14, DAT_005119b8);
    def->footprintx = ((Class_004c46c0*)entry->parser)->FUN_004c46c0("footprintx", 0);
    def->footprintz = ((Class_004c46c0*)entry->parser)->FUN_004c46c0("footprintz", 0);
    def->height = ((Class_004c46c0*)entry->parser)->FUN_004c46c0("height", 0);
    ok = ((Class_004c48c0*)entry->parser)->FUN_004c48c0(file, "object", 0x100, DAT_005119b8);
    if (ok) {
        def->noobject = 0;
        def->object = LoadObject3d(file);
    } else {
        def->noobject = 1;
        ((Class_004c48c0*)entry->parser)->FUN_004c48c0(file, "filename", 0x100, DAT_005119b8);
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
        ok = ((Class_004c48c0*)entry->parser)->FUN_004c48c0(seqname, "seqname", 0x100, DAT_005119b8);
        if (ok)
            def->seq = SeqByName(anims, 0, seqname);
        else
            def->seq = 0;
        ok = ((Class_004c48c0*)entry->parser)->FUN_004c48c0(seqname, "seqnameshad", 0x100, DAT_005119b8);
        if (ok)
            def->seqshad = SeqByName(anims, 0, seqname);
        else
            def->seqshad = 0;
        ok = ((Class_004c48c0*)entry->parser)->FUN_004c48c0(seqname, "seqnameburn", 0x100, DAT_005119b8);
        if (ok) {
            def->seqburn = SeqByName(anims, 0, seqname);
            if (def->seqburn)
                def->seqburn->kind = 0;
        } else {
            def->seqburn = 0;
        }
        ok = ((Class_004c48c0*)entry->parser)->FUN_004c48c0(seqname, "seqnameburnshad", 0x100, DAT_005119b8);
        if (ok) {
            def->seqburnshad = SeqByName(anims, 0, seqname);
            if (def->seqburnshad)
                def->seqburnshad->kind = 0;
        } else {
            def->seqburnshad = 0;
        }
        ok = ((Class_004c48c0*)entry->parser)->FUN_004c48c0(seqname, "seqnamedie", 0x100, DAT_005119b8);
        if (ok) {
            def->seqdie = SeqByName(anims, 0, seqname);
            if (def->seqdie)
                def->seqdie->kind = 0;
        } else {
            def->seqdie = 0;
        }
        ok = ((Class_004c48c0*)entry->parser)->FUN_004c48c0(seqname, "seqnamedieshad", 0x100, DAT_005119b8);
        if (ok) {
            def->seqdieshad = SeqByName(anims, 0, seqname);
            if (def->seqdieshad)
                def->seqdieshad->kind = 0;
        } else {
            def->seqdieshad = 0;
        }
        ok = ((Class_004c48c0*)entry->parser)->FUN_004c48c0(seqname, "seqnamereclamate", 0x100, DAT_005119b8);
        if (ok) {
            def->seqreclamate = SeqByName(anims, 0, seqname);
            if (def->seqreclamate)
                def->seqreclamate->kind = 0;
        } else {
            def->seqreclamate = 0;
        }
        ok = ((Class_004c48c0*)entry->parser)->FUN_004c48c0(seqname, "seqnamereclamateshad", 0x100, DAT_005119b8);
        if (ok) {
            def->seqreclamateshad = SeqByName(anims, 0, seqname);
            if (def->seqreclamateshad)
                def->seqreclamateshad->kind = 0;
        } else {
            def->seqreclamateshad = 0;
        }
    }
    def->spreadchance = ((Class_004c46c0*)entry->parser)->FUN_004c46c0("spreadchance", 0);
    def->reproduce = ((Class_004c46c0*)entry->parser)->FUN_004c46c0("reproduce", 0);
    def->reproducearea = ((Class_004c46c0*)entry->parser)->FUN_004c46c0("reproducearea", 0);
    def->metal = (unsigned short)((Class_004c46c0*)entry->parser)->FUN_004c46c0("metal", 0);
    def->energy = (unsigned short)((Class_004c46c0*)entry->parser)->FUN_004c46c0("energy", 0);
    def->damage = ((Class_004c46c0*)entry->parser)->FUN_004c46c0("damage", 0);
    def->animating = ((Class_004c46c0*)entry->parser)->FUN_004c46c0("animating", 0);
    def->animtrans = ((Class_004c46c0*)entry->parser)->FUN_004c46c0("animtrans", 0);
    def->shadtrans = ((Class_004c46c0*)entry->parser)->FUN_004c46c0("shadtrans", 0);
    def->flamable = ((Class_004c46c0*)entry->parser)->FUN_004c46c0("flamable", 0);
    def->geothermal = ((Class_004c46c0*)entry->parser)->FUN_004c46c0("geothermal", 0);
    def->blocking = ((Class_004c46c0*)entry->parser)->FUN_004c46c0("blocking", 0);
    def->reclaimable = ((Class_004c46c0*)entry->parser)->FUN_004c46c0("reclaimable", 0);
    def->autoreclaimable = ((Class_004c46c0*)entry->parser)->FUN_004c46c0("autoreclaimable", 1);
    def->indestructible = ((Class_004c46c0*)entry->parser)->FUN_004c46c0("indestructible", 0);
    def->nodisplayinfo = ((Class_004c46c0*)entry->parser)->FUN_004c46c0("nodisplayinfo", 0);
    def->nodrawundergray = ((Class_004c46c0*)entry->parser)->FUN_004c46c0("nodrawundergray", 0);
    if (_strcmpi(name, "DragonsTeeth") == 0)
        def->nodrawundergray = 1;
    if (_strcmpi(name, "DragonsTeeth_Core") == 0)
        def->nodrawundergray = 1;
    if (_strcmpi(name, "Fortification") == 0)
        def->nodrawundergray = 1;
    if (_strcmpi(name, "Fortification_Core") == 0)
        def->nodrawundergray = 1;
    def->sparktime = (short)(((Class_004c4760*)entry->parser)->FUN_004c4760("sparktime", 0.0) * 30.0);
    ((Class_004c48c0*)entry->parser)->FUN_004c48c0(seqname, "burnweapon", 0x100, DAT_005119b8);
    def->burnweapon = FUN_0049e5b0(seqname);
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
