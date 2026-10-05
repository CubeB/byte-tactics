// Decompiled by Opus. Names are provisional.

extern int DAT_0051e690;
extern int DAT_0051e694;

void __stdcall FUN_004290f0(char* out, const char* dir, const char* name, const char* ext);
void* __stdcall FUN_004bbe50(char* path, int flags);

class Class_004d0620 {
public:
    void* FUN_004d0620(char* path);
};

struct Game {
    char unknown_0[0x10];
    Class_004d0620* sound;             // +0x10
};

extern Game* g_game;

// Loads sounds/<name>.WAV, either as a plain file or through the sound system.
// FUNCTION: 0x47efe0
void* __stdcall FUN_0047efe0(const char* name)
{
    char path[256];
    if (DAT_0051e690 != 0 && DAT_0051e694 == 0) {
        return 0;
    }
    FUN_004290f0(path, "sounds", name, "WAV");
    if (DAT_0051e694 != 0) {
        return FUN_004bbe50(path, 0);
    }
    return g_game->sound->FUN_004d0620(path);
}
