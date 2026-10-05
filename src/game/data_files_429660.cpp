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

void* __stdcall HAPI_OpenFileRead(char* path);
void __stdcall FatalError(char* path);
int __stdcall HAPI_FileLength(void* file);
void* __cdecl FUN_004d83b0(char* name, int size);
int __stdcall HAPI_readfromfile(void* file, void* buf, int size);
int __stdcall HAPI_CloseFile(void* file);

// FUNCTION: 0x429660
char* __stdcall FUN_00429660(char* path)
{
    void* file = HAPI_OpenFileRead(path);
    if (file == 0) {
        FatalError(path);
    }
    int size = HAPI_FileLength(file);
    int chunk = size / 10;
    char* buf = (char*)FUN_004d83b0(path, size);
    int done = 0;
    for (int progress = 9; progress <= 90; progress += 9) {
        done += HAPI_readfromfile(file, buf + done, chunk);
        g_game->loadProgress = progress;
    }
    if (done < size) {
        HAPI_readfromfile(file, buf + done, size - done);
    }
    HAPI_CloseFile(file);
    return buf;
}
