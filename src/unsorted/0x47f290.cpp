// Decompiled by Opus. Names are provisional.
// Plays a sound by name: through FUN_0049f6c0 when DAT_0051e694 is set,
// otherwise through the game's sound object when sound is enabled.
#include <windows.h>
#include <string.h>

#pragma pack(push, 1)
struct Game_0047f290 {
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
    int FUN_004d0640(const char* param1, int param2, int param3);
};

extern Game_0047f290* g_game;
extern int DAT_0051e694;
extern int DAT_0051e690;

BOOL __stdcall FUN_0049f6c0(char* path);

// FUNCTION: 0x47f290
int __stdcall FUN_0047f290(char* name)
{
    if (DAT_0051e694)
        return FUN_0049f6c0(name);
    if (name && strlen(name) && g_game->volume1 && (g_game->soundFlags & 7) && !DAT_0051e690)
        return ((Class_004d0640*)g_game->sound)->FUN_004d0640(name, -0x249, 0);
    return 0;
}
