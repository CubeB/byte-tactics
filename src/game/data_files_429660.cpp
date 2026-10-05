// Decompiled by Opus. Names are provisional.
// Reads a whole file into a new buffer in ten chunks, updating the loading
// progress (9..90) after each chunk.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x38d70];
    unsigned char loadProgress;        // +0x38d70
};
#pragma pack(pop)

extern Game* g_game;

void* __stdcall FUN_004bb5b0(char* path);
void __stdcall FUN_004b6290(char* path);
int __stdcall FUN_004bbd00(void* file);
void* __cdecl FUN_004d83b0(char* name, int size);
int __stdcall FUN_004bb7c0(void* file, void* buf, int size);
int __stdcall FUN_004bb5d0(void* file);

// FUNCTION: 0x429660
char* __stdcall FUN_00429660(char* path)
{
    void* file = FUN_004bb5b0(path);
    if (file == 0) {
        FUN_004b6290(path);
    }
    int size = FUN_004bbd00(file);
    int chunk = size / 10;
    char* buf = (char*)FUN_004d83b0(path, size);
    int done = 0;
    for (int progress = 9; progress <= 90; progress += 9) {
        done += FUN_004bb7c0(file, buf + done, chunk);
        g_game->loadProgress = progress;
    }
    if (done < size) {
        FUN_004bb7c0(file, buf + done, size - done);
    }
    FUN_004bb5d0(file);
    return buf;
}
