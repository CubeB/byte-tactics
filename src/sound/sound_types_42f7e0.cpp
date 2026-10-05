// Decompiled by space-bunny-free. Names are provisional.
// Loads gamedata\allsound.TDF and registers every "sound" entry found in it
// through FUN_00429470, then runs the general sound loader LoadSoundCategories.

class TdfFile {
public:
    int field_0;
    int field_4;
    int field_8;

    TdfFile();
    ~TdfFile();
    int LoadFile(char* path);
    int SelectRecordAt(int index);
    void Unload();
    void ResetCurrentRecord();
};

class TdfRecord {
public:
    const char* field_0;

    void CopyRecordName(char* dest, unsigned int count);
    int GetFieldString(char* dst, char* key, unsigned int size, char* def);
};

extern char* g_game;
extern char DAT_005119b8[];

void __stdcall BuildDataPath(char* out, const char* dir, const char* name, const char* ext);
void __stdcall FUN_00429470(char* name, char* value);
void LoadSoundCategories();

// FUNCTION: 0x42f7e0
void LoadAllSound()
{
    TdfFile obj;
    char name[32];
    char path[256];
    char value[256];

    *(int*)(g_game + 0x33a0f) = 0;
    BuildDataPath(path, "gamedata", "allsound", "TDF");
    if (((TdfFile*)&obj)->LoadFile(path)) {
        int i = 0;
        int more = ((TdfFile*)&obj)->SelectRecordAt(i);
        while (more) {
            ((TdfRecord*)obj.field_4)->CopyRecordName(name, 0x20);
            if (((TdfRecord*)obj.field_4)->GetFieldString(value, "sound", 0x100, DAT_005119b8))
                FUN_00429470(name, value);
            i++;
            ((TdfFile*)&obj)->ResetCurrentRecord();
            more = ((TdfFile*)&obj)->SelectRecordAt(i);
        }
        ((TdfFile*)&obj)->Unload();
    }
    LoadSoundCategories();
}
