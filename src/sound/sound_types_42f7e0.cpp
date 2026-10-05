// Decompiled by space-bunny-free. Names are provisional.
// Loads gamedata\allsound.TDF and registers every "sound" entry found in it
// through FUN_00429470, then runs the general sound loader LoadSoundCategories.

class Class_004c2ea0 {
public:
    int field_0;
    int field_4;
    int field_8;

    Class_004c2ea0();
    ~Class_004c2ea0();
};

class Class_004c2f60 {
public:
    int LoadFile(char* path);
};

class Class_004c3490 {
public:
    int SelectRecordAt(int index);
};

class Class_004c3240 {
public:
    void Unload();
};

class Class_004c3e10 {
public:
    char unknown_0[4];
    int field_0x4;

    void ResetCurrentRecord();
};

class Class_004c4420 {
public:
    const char* field_0;

    void CopyRecordName(char* dest, unsigned int count);
};

class TdfRecord {
public:
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
    Class_004c2ea0 obj;
    char name[32];
    char path[256];
    char value[256];

    *(int*)(g_game + 0x33a0f) = 0;
    BuildDataPath(path, "gamedata", "allsound", "TDF");
    if (((Class_004c2f60*)&obj)->LoadFile(path)) {
        int i = 0;
        int more = ((Class_004c3490*)&obj)->SelectRecordAt(i);
        while (more) {
            ((Class_004c4420*)obj.field_4)->CopyRecordName(name, 0x20);
            if (((TdfRecord*)obj.field_4)->GetFieldString(value, "sound", 0x100, DAT_005119b8))
                FUN_00429470(name, value);
            i++;
            ((Class_004c3e10*)&obj)->ResetCurrentRecord();
            more = ((Class_004c3490*)&obj)->SelectRecordAt(i);
        }
        ((Class_004c3240*)&obj)->Unload();
    }
    LoadSoundCategories();
}
