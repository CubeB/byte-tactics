// Decompiled by space-bunny-free, deepseek-v4.1-flash, Opus and Haiku. Names are provisional.

#include <stdio.h>
#include <string.h>

class TdfRecord {
public:
    const char* field_0;
    void CopyRecordName(char* dest, unsigned int count);
    int GetFieldString(char* dst, char* key, unsigned int size, char* def);
    int GetSubRecordCount();
};

class TdfFile {
public:
    int field_0;                        // +0x0 (parsed tree root)
    int field_4;                        // +0x4 (current node)
    int field_8;                        // +0x8

    TdfFile();
    ~TdfFile();
    int LoadFile(char* path);
    void Unload();
    int SelectRecordAt(int index);
    void ResetCurrentRecord();
};

struct Source_0042f450 {
    char unknown_0[4];
    TdfRecord* tdf;                    // +0x4
};

struct SoundInfo_005086fc {
    char* name;                         // +0x0
    int unknown_4[5];                   // +0x4
};

struct Slot_0042f740 {
    int count;                         // +0x00
    int* a;                            // +0x04
    int* b;                            // +0x08
};

struct Entry_0042f740 {
    char unknown_0[0x40];
    Slot_0042f740 slots[0x18];         // +0x40
};

struct IDirectSoundBuffer;

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x33a0f];
    int soundCount;                    // +0x33a0f
    IDirectSoundBuffer** sounds[1];    // +0x33a13
    char unknown_33a17[0x37e13 - 0x33a17];
    Entry_0042f740* entries;           // +0x37e13
    int entry_count;                   // +0x37e17
};
#pragma pack(pop)

extern Game* g_game;
extern char DAT_005119b8[];
extern char g_sayChoiceArrayName[];
extern SoundInfo_005086fc g_speechCategories[];

int __cdecl GameReallocTagged(int param_1, char* param_2, int param_3);
void __cdecl GameFreeThunk(int* param_1);
void* __cdecl GameAllocIgnoreTag(char* name, unsigned int size);
void __stdcall BuildDataPath(char* out, const char* dir, const char* name, const char* ext);
int __stdcall ReadSoundEntry(Source_0042f450* source, char* key, int* out);
void __stdcall LoadSoundByName(char* name, char* value);
void LoadSoundCategories();
void FreeSoundCategories();
void __stdcall FreeSoundSet(IDirectSoundBuffer** set);

// FUNCTION: 0x42f450
int __stdcall ReadSoundEntry(Source_0042f450* param_1, char* param_2, int* param_3)
{
    char local_180[0x40];
    char local_140[0x40];
    char local_100[0x100];

    if (param_1->tdf->GetFieldString(local_140, param_2, 0x40, DAT_005119b8) != 0) {
        sprintf(local_100, "%s%s", param_2, "text");
        if (param_1->tdf->GetFieldString(local_180, local_100, 0x40, DAT_005119b8) == 0)
            local_180[0] = 0;
        param_3[1] = GameReallocTagged(param_3[1], g_sayChoiceArrayName, (param_3[0] + 1) * 0x40);
        param_3[2] = GameReallocTagged(param_3[2], g_sayChoiceArrayName, (param_3[0] + 1) * 0x40);
        strcpy((char*)(param_3[1] + param_3[0] * 0x40), local_140);
        strcpy((char*)(param_3[2] + param_3[0] * 0x40), local_180);
        param_3[0]++;
        return 1;
    }
    return 0;
}

// Loads gamedata\sound.TDF and fills the global "Sound Categories" table at
// g_game+0x37e13 (count at g_game+0x37e17). Each of the top level .TDF
// sections becomes one 0x160 byte record: its name is copied into the first
// 0x3f bytes and, at +0x4c, 23 groups of three dwords are filled from the
// entries named after the rows of the global table g_speechCategories ("select",
// "select1", "select2", ...); the second and third dwords of each group are
// allocated by ReadSoundEntry as the values are added.
// FUNCTION: 0x42f580
void LoadSoundCategories()
{
    g_game->entry_count = 0;
    g_game->entries = 0;
    TdfFile obj;
    char name[32];
    char path[256];
    SoundInfo_005086fc* p;

    BuildDataPath(path, "gamedata", "sound", "TDF");
    if (obj.LoadFile(path)) {
        g_game->entry_count = ((TdfRecord*)obj.field_0)->GetSubRecordCount();
        int size = g_game->entry_count * 0x160;
        g_game->entries = (Entry_0042f740*)GameAllocIgnoreTag("Sound Categories", size);
        memset(g_game->entries, 0, size);
        for (int i = 0; i < g_game->entry_count; i++) {
            char* rec = (char*)g_game->entries + i * 0x160;
            obj.ResetCurrentRecord();
            if (obj.SelectRecordAt(i)) {
                ((TdfRecord*)obj.field_4)->CopyRecordName(rec, 0x3f);
                p = g_speechCategories;
                int* vals = (int*)(rec + 0x4c);
                // Compared as signed ints, not pointers: keeps jl instead of jb.
                while ((int)p < (int)(g_speechCategories + 23)) {
                    ReadSoundEntry((Source_0042f450*)&obj, p->name, vals);
                    for (int n = 1; ; n++) {
                        sprintf(name, "%s%i", p->name, n);
                        if (!ReadSoundEntry((Source_0042f450*)&obj, name, vals))
                            break;
                    }
                    p++;
                    vals += 3;
                }
            }
        }
        obj.Unload();
    }
}

// FUNCTION: 0x42f740
void FreeSoundCategories()
{
    if (g_game->entry_count > 0) {
        for (int i = 0; i < g_game->entry_count; i++) {
            Entry_0042f740* e = &g_game->entries[i];
            for (int j = 0; j < 0x18; j++) {
                if (e->slots[j].count > 0) {
                    GameFreeThunk(e->slots[j].a);
                    GameFreeThunk(e->slots[j].b);
                }
            }
        }
        GameFreeThunk((int*)g_game->entries);
    }
    g_game->entry_count = 0;
    g_game->entries = 0;
}

// Loads gamedata\allsound.TDF and registers every "sound" entry found in it
// through LoadSoundByName, then runs the general sound loader LoadSoundCategories.
// FUNCTION: 0x42f7e0
void LoadAllSound()
{
    TdfFile obj;
    char name[32];
    char path[256];
    char value[256];

    g_game->soundCount = 0;
    BuildDataPath(path, "gamedata", "allsound", "TDF");
    if (obj.LoadFile(path)) {
        int i = 0;
        int more = obj.SelectRecordAt(i);
        while (more) {
            ((TdfRecord*)obj.field_4)->CopyRecordName(name, 0x20);
            if (((TdfRecord*)obj.field_4)->GetFieldString(value, "sound", 0x100, DAT_005119b8))
                LoadSoundByName(name, value);
            i++;
            obj.ResetCurrentRecord();
            more = obj.SelectRecordAt(i);
        }
        obj.Unload();
    }
    LoadSoundCategories();
}

// Releases every loaded sound set, then empties the list.
// FUNCTION: 0x42f8c0
void FreeSounds()
{
    FreeSoundCategories();
    for (int i = 0; i < g_game->soundCount; i++)
        FreeSoundSet(g_game->sounds[i]);
    g_game->soundCount = 0;
}

// FUNCTION: 0x42f900
void __stdcall FUN_0042f900(int)
{
}
