// Decompiled by Opus. Names are provisional.
#include <string.h>

#pragma pack(push, 1)
struct Game_00484fa0 {
    char unknown_0[0x14233];
    int width;                          // +0x14233
    int height;                         // +0x14237
    char unknown_1423b[0x14273 - 0x1423b];
    void* mapping;                      // +0x14273
};
#pragma pack(pop)

extern Game_00484fa0* g_game;

// Chunked file reader.
class Class_004b4560 {
public:
    int FUN_004b4560(char* name);
};

class Class_004b4ba0 {
public:
    int FUN_004b4ba0(char* name);
};

class Class_004b4bf0 {
public:
    int FUN_004b4bf0();
};

class Class_004b4c80 {
public:
    unsigned int FUN_004b4c80(void* buf, int size);
};

// Reads the "Mapping" "Data" chunk into the map's mapping buffer.
// <string.h> decides which of width and height is loaded before the imul.
// FUNCTION: 0x484fa0
void __stdcall FUN_00484fa0(Class_004b4560* file)
{
    if (file->FUN_004b4560("Mapping") && ((Class_004b4ba0*)file)->FUN_004b4ba0("Data")) {
        unsigned int size = g_game->width * g_game->height * sizeof(short) / 4;
        if (((Class_004b4bf0*)file)->FUN_004b4bf0() == size)
            ((Class_004b4c80*)file)->FUN_004b4c80(g_game->mapping, size);
    }
}
