// Decompiled by deepseek-v4.1-flash. Names are provisional.

#include <vector>
#include <string.h>

class Class_004c9390 {
public:
    char* data;

    void FUN_004c9390();
};

class Class_004c91a0 {
public:
    char* p;

    ~Class_004c91a0() { ((Class_004c9390*)this)->FUN_004c9390(); }
};

#pragma pack(push, 1)
struct Def_0042a610 {
    char unknown_0[0x20];
    char name[0x122];                  // +0x20
    unsigned int field_142;            // +0x142
    unsigned int field_146;            // +0x146
};

struct Game_0042a610 {
    char unknown_0[0x1439b];
    int* field_1439b;                  // +0x1439b
};
#pragma pack(pop)

extern Game_0042a610* g_game;

struct File_0042a610;

void __stdcall FUN_004290f0(char* out, const char* dir, const char* name, const char* ext);
void* __stdcall FUN_004bbe50(char* path, int* outSize);
int __stdcall FUN_004b6ba0(unsigned char* data, int len);
void __cdecl FUN_004d85a0(void* p);
void __stdcall FUN_004bca30(const char* pattern, int flags, std::vector<Class_004c91a0>* out);
File_0042a610* __stdcall FUN_004bb5b0(char* path);
long __stdcall FUN_004bbd00(File_0042a610* f);
void* __stdcall FUN_004bbff0(char* name, File_0042a610* f, unsigned int* outSize);
int __stdcall FUN_004bb5d0(File_0042a610* f);
void __cdecl FUN_004d8780(void* p);
void __cdecl FUN_004d8710(void* p);

// Loads a unit type's script (scripts\NAME.cob), every GUI file matching
// guis\NAME*.gui and its download file (download\NAME.tdf), XORs the 4-byte
// checksum of each file into field_142, and locks the unit type table while
// reading. Does nothing once field_142 is nonzero.

// FUNCTION: 0x42a610
void __stdcall FUN_0042a610(Def_0042a610* def)
{
    if (def->field_142)
        return;

    FUN_004d8780(g_game->field_1439b);

    char path[256];
    int size;
    FUN_004290f0(path, "scripts", def->name, "cob");
    void* data = FUN_004bbe50(path, &size);
    if (data) {
        def->field_142 ^= FUN_004b6ba0((unsigned char*)data, size);
        FUN_004d85a0(data);
    }

    std::vector<Class_004c91a0> files;
    char name[64];
    strcpy(name, def->name);
    strcat(name, "*");

    FUN_004290f0(path, "guis", name, "gui");
    FUN_004bca30(path, 0, &files);

    for (unsigned int i = 0; i < files.size(); i++) {
        FUN_004290f0(path, "guis", files[i].p, "gui");
        void* data2 = FUN_004bbe50(path, &size);
        if (data2) {
            def->field_142 ^= FUN_004b6ba0((unsigned char*)data2, size);
            FUN_004d85a0(data2);
        }
    }

    FUN_004290f0(path, "download", def->name, "tdf");
    File_0042a610* f = FUN_004bb5b0(path);
    if (f) {
        size = FUN_004bbd00(f);
        if (size > 0) {
            void* data3 = FUN_004bbff0(path, f, 0);
            if (data3) {
                def->field_142 ^= FUN_004b6ba0((unsigned char*)data3, size);
                FUN_004d85a0(data3);
            }
        }
        FUN_004bb5d0(f);
    }

    def->field_142 ^= def->field_146;
    FUN_004d8710(g_game->field_1439b);
}
