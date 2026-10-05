// Decompiled by Opus. Names are provisional.

extern int g_noDirectSound;
extern int g_useWindowsSound;

void __stdcall BuildDataPath(char* out, const char* dir, const char* name, const char* ext);
void* __stdcall HAPI_LoadFile(char* path, int flags);

class Class_004d0620 {
public:
    void* LoadSample(char* path);
};

struct Game {
    char unknown_0[0x10];
    Class_004d0620* sound;             // +0x10
};

extern Game* g_game;

// Loads sounds/<name>.WAV, either as a plain file or through the sound system.
// FUNCTION: 0x47efe0
void* __stdcall LoadSoundFile(const char* name)
{
    char path[256];
    if (g_noDirectSound != 0 && g_useWindowsSound == 0) {
        return 0;
    }
    BuildDataPath(path, "sounds", name, "WAV");
    if (g_useWindowsSound != 0) {
        return HAPI_LoadFile(path, 0);
    }
    return g_game->sound->LoadSample(path);
}
