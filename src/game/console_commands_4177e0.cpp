// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Debug-data dump helper (sibling of 0x417430/0x417490): builds
// "debugdat\\<name>.txt", saves the game position at +0x2caa as a Vec3 (the
// x/y/z values survive the calls in registers, only z is spilled), reads the
// file's contents and writes them back out, then restores the position.
#include <stdio.h>

// Three ints at g_game + 0x2caa (unaligned on purpose, accessed as bytes).
struct Vec3_004177e0 {
    int x;
    int y;
    int z;
};

extern char* g_game;                   // 0x511de8
extern char DAT_005119b8[];

// Command arguments.
class Class_004b73c0 {
public:
    char* GetArg(int index, char* fallback);
};

void* __stdcall HAPI_OpenFileRead(char* path);
void* __stdcall HAPI_LoadOpenFile(char* path, void* file, int* out);
unsigned int __stdcall ExecuteCommandText(char* data, int size, int param_3, unsigned int param_4);
void __cdecl FUN_004d85a0(void* data);
int __stdcall HAPI_CloseFile(void* file);

// FUNCTION: 0x4177e0
void __stdcall CmdInclude(Class_004b73c0* args)
{
    Vec3_004177e0 pos;
    int info;
    char path[60];

    sprintf(path, "debugdat\\%s.txt", args->GetArg(0, DAT_005119b8));
    void* file = HAPI_OpenFileRead(path);
    if (file != 0) {
        pos = *(Vec3_004177e0*)(g_game + 0x2caa);
        void* data = HAPI_LoadOpenFile(path, file, &info);
        if (data != 0) {
            ExecuteCommandText((char*)data, info, (int)args, 0xffffffff);
            FUN_004d85a0(data);
        }
        HAPI_CloseFile(file);
        *(Vec3_004177e0*)(g_game + 0x2caa) = pos;
    }
}
