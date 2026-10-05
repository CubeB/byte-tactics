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

// Chunked file writer.
class HapiBank {
public:
    int OpenAccount(char* name);
    int OpenNamedBox(char* name);
    int WriteBox(void* src, int len);
};

// Writes the "Mapping" "Data" chunk from the map's mapping buffer; the
// reading counterpart is 0x484fa0.
// FUNCTION: 0x484f50
void __stdcall SaveMappingData(HapiBank* file)
{
    file->OpenAccount("Mapping");
    unsigned int size = g_game->width * g_game->height * sizeof(short) / 4;
    ((HapiBank*)file)->OpenNamedBox("Data");
    ((HapiBank*)file)->WriteBox(g_game->mapping, size);
}
