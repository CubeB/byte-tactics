// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Stays in its own file: in features.cpp the loop address's base and index
// swap (lea [edx+ebx+4] against the original's [ebx+edx+4]; docs/c2-regalloc.md,
// "Symbol ids").
#include <string.h>

// 0x30-byte element of the feature animation table: a doubly linked list of
// free/used slots, next at +0, prev at +2.
struct AnimEntry_00421f20 {
    short next;                        // +0x0
    short prev;                        // +0x2
    char unknown_4[0x2c];
};

#pragma pack(push, 1)
struct FeatureUnit_00421f20 {
    char unknown_0[0x92];
    int field_92;                      // +0x92
    char unknown_96[0xa6 - 0x96];
    unsigned short field_a6;           // +0xa6
    char unknown_a8[0x110 - 0xa8];
    int flags_110;                     // +0x110
    int flags_114;                     // +0x114
};
#pragma pack(pop)

struct NameEntry_00421f20 {
    char unknown_0[4];
    char name[0x80];                   // +0x4
};

struct List_00421f20 {
    char unknown_0[0x1c];
    int count;                         // +0x1c
    NameEntry_00421f20* entries;       // +0x20
};

struct AnimManager_00421f20 {
    char unknown_0[0x10];
    AnimEntry_00421f20* anim;          // +0x10
    FeatureUnit_00421f20* featureUnit; // +0x14
    int field_18;                      // +0x18
    int field_1c;                      // +0x1c
    int field_20;                      // +0x20
    char unknown_24[0x58 - 0x24];
    int nameCount;                     // +0x58
    char unknown_5c[0x74 - 0x5c];
    char (*names)[0x100];              // +0x74
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x141fb];
    AnimManager_00421f20 anim;         // +0x141fb
    char unknown_14273[0x1439b - 0x14273];
    int unitDefs;                      // +0x1439b
};
#pragma pack(pop)

extern Game* g_game;

void* __cdecl GameAllocIgnoreTag(char* name, unsigned int size);
unsigned short __stdcall LoadFeatureType(char* name);

// FUNCTION: 0x421f20
void __stdcall InitFeatureAnimPool(List_00421f20* list)
{
    AnimManager_00421f20* m = &g_game->anim;

    m->anim = (AnimEntry_00421f20*)GameAllocIgnoreTag("FEATURE ANIM DATA", 0x18000);
    memset(m->anim, 0, 0x18000);
    m->field_18 = -1;
    m->field_1c = -1;
    m->field_20 = 0;
    for (int i = 0; i < 0x800; i++) {
        m->anim[i].next = i + 1;
        m->anim[i].prev = i - 1;
    }
    m->anim[0].prev = -1;
    m->anim[0x7ff].next = -1;

    m->featureUnit = (FeatureUnit_00421f20*)GameAllocIgnoreTag("Feature Unit", 0x118);
    memset(m->featureUnit, 0, 0x118);
    m->featureUnit->flags_110 |= 0x200;
    m->featureUnit->flags_110 |= 0x20000000;
    m->featureUnit->field_a6 = 0;
    m->featureUnit->flags_114 |= 1;
    m->featureUnit->field_92 = g_game->unitDefs;
    m->names = 0;
    m->nameCount = 0;

    // A reference to the field, not a local copy: every use re-reads
    // list->entries, and that is what makes MSVC base the address on the loop
    // offset (lea [ebx+edx+4]) instead of on the loaded pointer.
    NameEntry_00421f20*& entries = list->entries;

    for (int j = 0; j < list->count; j++) {
        LoadFeatureType(entries[j].name);
    }
}
