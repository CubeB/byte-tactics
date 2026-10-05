// Decompiled by Opus. Names are provisional.
#include <string.h>

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x14253];
    int nameCount;              // +0x14253
    char unknown_14257[0x1426f - 0x14257];
    char (*names)[0x100];       // +0x1426f
};
#pragma pack(pop)

extern Game* g_game;

unsigned short __stdcall LoadFeatureType(char* name);

static inline unsigned short FindName(char* name)
{
    for (int i = 0; i < g_game->nameCount; i++) {
        if (_strcmpi(name, g_game->names[i]) == 0) {
            return (unsigned short)i;
        }
    }
    return 0xffff;
}

// FUNCTION: 0x422e40
unsigned short __stdcall FindOrLoadFeatureType(char* name)
{
    unsigned short i = FindName(name);
    if (i == 0xffff)
        i = LoadFeatureType(name);
    return i;
}
