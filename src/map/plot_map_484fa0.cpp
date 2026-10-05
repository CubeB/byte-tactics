// Decompiled by Opus. Names are provisional.
#include <string.h>

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x14233];
    int width;                          // +0x14233
    int height;                         // +0x14237
    char unknown_1423b[0x14273 - 0x1423b];
    void* mapping;                      // +0x14273
};
#pragma pack(pop)

extern Game* g_game;

// Chunked file reader.
class HapiBank {
public:
    int OpenAccount(char* name);
    int OpenNamedBox(char* name);
    int GetBoxSize();
    unsigned int ReadBox(void* buf, int size);
};

// Reads the "Mapping" "Data" chunk into the map's mapping buffer.
// <string.h> decides which of width and height is loaded before the imul.
// FUNCTION: 0x484fa0
void __stdcall LoadMappingData(HapiBank* file)
{
    if (file->OpenAccount("Mapping") && ((HapiBank*)file)->OpenNamedBox("Data")) {
        unsigned int size = g_game->width * g_game->height * sizeof(short) / 4;
        if (((HapiBank*)file)->GetBoxSize() == size)
            ((HapiBank*)file)->ReadBox(g_game->mapping, size);
    }
}
