// Decompiled by space-bunny-free. Names are provisional.
// Loads every "Weapons\\*.tdf" file, parses each with the TDF parser, and
// calls LoadWeaponType once per top level section found in it.

#include <vector>

class Class_004c9390 {
public:
    char* data;

    void ReleaseRef();
};

class Class_004c91a0 {
public:
    char* p;

    ~Class_004c91a0() { ((Class_004c9390*)this)->ReleaseRef(); }
};

class Class_004c2ea0 {
public:
    int root;                          // +0x0
    int current;                       // +0x4
    int field_8;                       // +0x8

    Class_004c2ea0();
    ~Class_004c2ea0();
};

class Class_004c2f60 {
public:
    int LoadFile(char* file);
};

class Class_004c3e10 {
public:
    void ResetCurrentRecord();
};

class Class_004c3490 {
public:
    int SelectRecordAt(int index);
};

#pragma pack(push, 1)
struct Weapon_0042e310 {
    unsigned char used;                // +0x0
    char unknown_1[0x109];
    unsigned char id;                  // +0x10a
    char unknown_10b[0x115 - 0x10b];
};

struct Game {
    char unknown_0[0x2cf3];
    Weapon_0042e310 weapons[0x100];    // +0x2cf3
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall BuildDataPath(char* out, const char* dir, const char* name, const char* ext);
void __stdcall ListDirectory(const char* pattern, int flags, std::vector<Class_004c91a0>* out);
void __stdcall LoadWeaponType(int section);
int FUN_0041d8a0(void);

// FUNCTION: 0x42e310
void LoadWeaponTypes()
{
    int n = 0;
    for (int i = 0; i < 0x100; i++) {
        Weapon_0042e310* p = &g_game->weapons[i];
        p->id = n++;
        p->used = 0;
    }

    char path[256];
    std::vector<Class_004c91a0> files;
    ListDirectory("Weapons\\*.tdf", 0, &files);

    for (Class_004c91a0* p = files.begin(); p < files.end(); p++) {
        Class_004c2ea0 parser;
        BuildDataPath(path, "Weapons", p->p, "TDF");
        if (((Class_004c2f60*)&parser)->LoadFile(path)
            && (parser.field_8 || FUN_0041d8a0() == 0)) {
            int i = 0;
            while (1) {
                ((Class_004c3e10*)&parser)->ResetCurrentRecord();
                if (!((Class_004c3490*)&parser)->SelectRecordAt(i))
                    break;
                LoadWeaponType(parser.current);
                i++;
            }
        }
    }
}
