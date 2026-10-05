// Decompiled by DeepSeek V4.1 Flash, finished by Claude Opus 5.5. Names are provisional.
// Walks a loaded 3DO model: recurses into its two child models, then for each
// object resolves its texture name against the loaded GAF files and either
// points it at a plain texture or starts an animation sequence.
//
// NOTE: "entry" is deliberately left uninitialised, as in the original (likely
// an original bug). The original has no store before the first lookup loop, so
// on the (never taken in practice) path where g_game->blockCount <= 0 the
// parameter pointer is used as if it were a GAF entry. Initialising it to 0
// changes the generated code.
//
// Statement order in two blocks decides the plain-texture branch: the
// not-found block sets flag 1 before storing the colour 0xd1, and the plain
// branch stores the texture before clearing flag 2. Both blocks then end in
// the same flags store "mov [esi+0x1c], eax"; with the colour stored last in
// the not-found block, MSVC does not tail-merge them (it compares the blocks
// before scheduling), and the scheduler then interleaves the plain branch as
// the original does.

#pragma pack(push, 1)
struct Game_0042a140 {
    char unknown_0[0x148d7];
    void* logos;                    // +0x148d7
    void* logos32;                  // +0x148db
    int blockCount;                 // +0x148df
    void** blocks;                  // +0x148e3
    int count;                      // +0x148e7
    int* data;                      // +0x148eb
};
#pragma pack(pop)

extern Game_0042a140* g_game;

struct Ref_0042a140 {
    unsigned short index;           // +0x0
    unsigned short value;           // +0x2
    unsigned char kind;             // +0x4
    char unknown_5[3];
    void* src;                      // +0x8
};

struct Elem_0042a140 {
    int type;                       // +0x0
    char unknown_4[0xc];
    void* name;                     // +0x10
    char unknown_14[8];
    int flags;                      // +0x1c
};

struct Model_0042a140 {
    char unknown_0[8];
    int count;                      // +0x8
    char unknown_c[0x1c];
    Elem_0042a140* entries;         // +0x28
    Model_0042a140* child1;         // +0x2c
    Model_0042a140* child2;         // +0x30
};

void* __stdcall FUN_004b8d40(void* gaf, const char* name);
void __stdcall FUN_004b8b30(Ref_0042a140* ref, void* src, int index);
void* __cdecl FUN_004d84a0(void* param_1, const char* name, unsigned int param_3);

// FUNCTION: 0x42a140
void __stdcall FUN_0042a140(Model_0042a140* model, const char* name)
{
    if (model->child1)
        FUN_0042a140(model->child1, name);
    if (model->child2)
        FUN_0042a140(model->child2, name);

    Elem_0042a140* elem = model->entries;
    for (int i = 0; i < model->count; i++, elem++) {
        if (elem->name) {
            unsigned short* entry;
            elem->flags &= ~4;
            for (int j = 0; j < g_game->blockCount; j++) {
                entry = (unsigned short*)FUN_004b8d40(g_game->blocks[j], (const char*)elem->name);
                if (entry)
                    break;
            }
            if (entry == 0) {
                entry = (unsigned short*)FUN_004b8d40(g_game->logos, (const char*)elem->name);
                if (entry != 0 && *entry == 10)
                    elem->flags |= 4;
                if (entry == 0) {
                    elem->flags |= 1;
                    elem->type = 0xd1;
                    continue;
                }
            }
            if (*entry > 1) {
                FUN_004b8b30((Ref_0042a140*)&elem->name, entry, 0);
                elem->flags |= 2;
                if (!(elem->flags & 4)) {
                    g_game->data = (int*)FUN_004d84a0(g_game->data, "Animplay Pointers", (g_game->count + 1) * 4);
                    g_game->data[g_game->count] = (int)&elem->name;
                    g_game->count++;
                }
                continue;
            }
            elem->name = *(void**)((char*)entry + 0x28);
            elem->flags &= ~2;
        }
    }
}
