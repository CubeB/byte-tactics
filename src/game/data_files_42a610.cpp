// Decompiled by deepseek-v4.1-flash. Names are provisional.

#include <vector>
#include <string.h>

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

#pragma pack(push, 1)
struct Def_0042a610 {
    char unknown_0[0x20];
    char name[0x122];                  // +0x20
    unsigned int field_142;            // +0x142
    unsigned int field_146;            // +0x146
};

struct Game {
    char unknown_0[0x1439b];
    int* field_1439b;                  // +0x1439b
};
#pragma pack(pop)

extern Game* g_game;

struct File_0042a610;

void __stdcall BuildDataPath(char* out, const char* dir, const char* name, const char* ext);
void* __stdcall HAPI_LoadFile(char* path, int* outSize);
int __stdcall ComputeChecksum(unsigned char* data, int len);
void __cdecl FUN_004d85a0(void* p);
void __stdcall ListDirectory(const char* pattern, int flags, std::vector<Class_004c91a0>* out);
File_0042a610* __stdcall HAPI_OpenFileRead(char* path);
long __stdcall HAPI_FileLength(File_0042a610* f);
void* __stdcall HAPI_LoadOpenFile(char* name, File_0042a610* f, unsigned int* outSize);
int __stdcall HAPI_CloseFile(File_0042a610* f);
void __cdecl ProtectBlockReadWrite(void* p);
void __cdecl ProtectBlockReadOnly(void* p);

// Loads a unit type's script (scripts\NAME.cob), every GUI file matching
// guis\NAME*.gui and its download file (download\NAME.tdf), XORs the 4-byte
// checksum of each file into field_142, and locks the unit type table while
// reading. Does nothing once field_142 is nonzero.

// FUNCTION: 0x42a610
void __stdcall FUN_0042a610(Def_0042a610* def)
{
    if (def->field_142)
        return;

    ProtectBlockReadWrite(g_game->field_1439b);

    char path[256];
    int size;
    BuildDataPath(path, "scripts", def->name, "cob");
    void* data = HAPI_LoadFile(path, &size);
    if (data) {
        def->field_142 ^= ComputeChecksum((unsigned char*)data, size);
        FUN_004d85a0(data);
    }

    std::vector<Class_004c91a0> files;
    char name[64];
    strcpy(name, def->name);
    strcat(name, "*");

    BuildDataPath(path, "guis", name, "gui");
    ListDirectory(path, 0, &files);

    for (unsigned int i = 0; i < files.size(); i++) {
        BuildDataPath(path, "guis", files[i].p, "gui");
        void* data2 = HAPI_LoadFile(path, &size);
        if (data2) {
            def->field_142 ^= ComputeChecksum((unsigned char*)data2, size);
            FUN_004d85a0(data2);
        }
    }

    BuildDataPath(path, "download", def->name, "tdf");
    File_0042a610* f = HAPI_OpenFileRead(path);
    if (f) {
        size = HAPI_FileLength(f);
        if (size > 0) {
            void* data3 = HAPI_LoadOpenFile(path, f, 0);
            if (data3) {
                def->field_142 ^= ComputeChecksum((unsigned char*)data3, size);
                FUN_004d85a0(data3);
            }
        }
        HAPI_CloseFile(f);
    }

    def->field_142 ^= def->field_146;
    ProtectBlockReadOnly(g_game->field_1439b);
}
