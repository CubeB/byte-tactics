// Decompiled by deepseek-v4.1-flash. Names are provisional.
//
// Loads gamedata\sound.TDF and fills the global "Sound Categories" table at
// g_game+0x37e13 (count at g_game+0x37e17). Each of the top level .TDF
// sections becomes one 0x160 byte record: its name is copied into the first
// 0x3f bytes and, at +0x4c, 23 groups of three dwords are filled from the
// entries named after the rows of the global table DAT_005086fc ("select",
// "select1", "select2", ...); the second and third dwords of each group are
// allocated by ReadSoundEntry as the values are added.
//
// The table walk compares as signed ints, not pointers: the original keeps
// `jl` instead of the `jb` a pointer compare would give.
#include <stdio.h>
#include <string.h>

class Class_004c2ea0 {
public:
    int field_0;                        // +0x0 (parsed tree root)
    int field_4;                        // +0x4 (current node)
    int field_8;                        // +0x8

    Class_004c2ea0();
    ~Class_004c2ea0();
};

class Class_004c2f60 {
public:
    int FUN_004c2f60(char* path);
};

class Class_004c3240 {
public:
    void FUN_004c3240();
};

class Class_004c3490 {
public:
    int FUN_004c3490(int index);
};

class Class_004c3e10 {
public:
    void FUN_004c3e10();
};

class Class_004c4420 {
public:
    void FUN_004c4420(char* dest, unsigned int count);
};

class Class_004c4450 {
public:
    int FUN_004c4450();
};

struct SoundInfo_005086fc {
    char* name;                         // +0x0
    int unknown_4[5];                   // +0x4
};

extern char* g_game;
extern SoundInfo_005086fc DAT_005086fc[];

void __stdcall BuildDataPath(char* out, const char* dir, const char* name, const char* ext);
void* __cdecl FUN_004d83b0(char* name, unsigned int size);
int __stdcall ReadSoundEntry(void* source, char* key, int* out);

// FUNCTION: 0x42f580
void LoadSoundCategories()
{
    *(int*)(g_game + 0x37e17) = 0;
    *(int*)(g_game + 0x37e13) = 0;
    Class_004c2ea0 obj;
    char name[32];
    char path[256];
    SoundInfo_005086fc* p;

    BuildDataPath(path, "gamedata", "sound", "TDF");
    if (((Class_004c2f60*)&obj)->FUN_004c2f60(path)) {
        *(int*)(g_game + 0x37e17) = ((Class_004c4450*)obj.field_0)->FUN_004c4450();
        int size = *(int*)(g_game + 0x37e17) * 0x160;
        *(int*)(g_game + 0x37e13) = (int)FUN_004d83b0("Sound Categories", size);
        memset((void*)*(int*)(g_game + 0x37e13), 0, size);
        for (int i = 0; i < *(int*)(g_game + 0x37e17); i++) {
            char* rec = (char*)*(int*)(g_game + 0x37e13) + i * 0x160;
            ((Class_004c3e10*)&obj)->FUN_004c3e10();
            if (((Class_004c3490*)&obj)->FUN_004c3490(i)) {
                ((Class_004c4420*)obj.field_4)->FUN_004c4420(rec, 0x3f);
                p = DAT_005086fc;
                int* vals = (int*)(rec + 0x4c);
                while ((int)p < (int)(DAT_005086fc + 23)) {
                    ReadSoundEntry(&obj, p->name, vals);
                    for (int n = 1; ; n++) {
                        sprintf(name, "%s%i", p->name, n);
                        if (!ReadSoundEntry(&obj, name, vals))
                            break;
                    }
                    p++;
                    vals += 3;
                }
            }
        }
        ((Class_004c3240*)&obj)->FUN_004c3240();
    }
}
