// Decompiled by Opus. Names are provisional.
// Plays a sound by name: through PlayWavFromDisk when g_useWindowsSound is set,
// otherwise through the game's sound object when sound is enabled.
#include <windows.h>
#include <string.h>

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x10];
    void* sound;                       // +0x10
    char unknown_14[0x37f0c - 0x14];
    int volume1;                       // +0x37f0c
    char unknown_37f10[0x37f19 - 0x37f10];
    unsigned char soundFlags;          // +0x37f19
};
#pragma pack(pop)

class Class_004d0640 {
public:
    int PlaySample(const char* param1, int param2, int param3);
};

extern Game* g_game;
extern int g_useWindowsSound;
extern int g_noDirectSound;

BOOL __stdcall PlayWavFromDisk(char* path);

// FUNCTION: 0x47f290
int __stdcall PlaySoundFile(char* name)
{
    if (g_useWindowsSound)
        return PlayWavFromDisk(name);
    if (name && strlen(name) && g_game->volume1 && (g_game->soundFlags & 7) && !g_noDirectSound)
        return ((Class_004d0640*)g_game->sound)->PlaySample(name, -0x249, 0);
    return 0;
}
