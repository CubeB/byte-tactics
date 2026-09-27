// Decompiled by space-bunny-free. Names are provisional.
// Loads gamedata\allsound.TDF and registers every "sound" entry found in it
// through FUN_00429470, then runs the general sound loader FUN_0042f580.

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
    int FUN_004c2f60(char* path);
};

class Class_004c3490 {
public:
    int FUN_004c3490(int index);
};

class Class_004c3240 {
public:
    void FUN_004c3240();
};

class Class_004c3e10 {
public:
    char unknown_0[4];
    int field_0x4;

    void FUN_004c3e10();
};

class Class_004c4420 {
public:
    const char* field_0;

    void FUN_004c4420(char* dest, unsigned int count);
};

class Class_004c48c0 {
public:
    int FUN_004c48c0(char* dst, char* key, unsigned int size, char* def);
};

extern char* g_game;
extern char DAT_005119b8[];

void __stdcall FUN_004290f0(char* out, const char* dir, const char* name, const char* ext);
void __stdcall FUN_00429470(char* name, char* value);
void FUN_0042f580();

// FUNCTION: 0x42f7e0
void FUN_0042f7e0()
{
    Class_004c2ea0 obj;
    char name[32];
    char path[256];
    char value[256];

    *(int*)(g_game + 0x33a0f) = 0;
    FUN_004290f0(path, "gamedata", "allsound", "TDF");
    if (((Class_004c2f60*)&obj)->FUN_004c2f60(path)) {
        int i = 0;
        int more = ((Class_004c3490*)&obj)->FUN_004c3490(i);
        while (more) {
            ((Class_004c4420*)obj.field_4)->FUN_004c4420(name, 0x20);
            if (((Class_004c48c0*)obj.field_4)->FUN_004c48c0(value, "sound", 0x100, DAT_005119b8))
                FUN_00429470(name, value);
            i++;
            ((Class_004c3e10*)&obj)->FUN_004c3e10();
            more = ((Class_004c3490*)&obj)->FUN_004c3490(i);
        }
        ((Class_004c3240*)&obj)->FUN_004c3240();
    }
    FUN_0042f580();
}
