// Decompiled by Opus. Names are provisional.
// Writes the "Metal" "Plotmap" chunk: the metal byte of every map cell
// (the save counterpart of 0x484d60). Without a header such as <windows.h>,
// MSVC loads height before width for the multiply (as in 0x484df0).
#include <windows.h>

#pragma pack(push, 1)
struct Cell_00484ce0 {
    char unknown_0[0x7];
    unsigned char metal;                // +0x7
    char unknown_8[0xd - 0x8];
};

struct Game_00484ce0 {
    char unknown_0[0x14233];
    int width;                          // +0x14233
    int height;                         // +0x14237
    char unknown_1423b[0x14287 - 0x1423b];
    Cell_00484ce0* cells;               // +0x14287
};
#pragma pack(pop)

extern Game_00484ce0* g_game;

// Chunked file writer.
class Class_004b4560 {
public:
    int FUN_004b4560(char* name);
};

class Class_004b4ba0 {
public:
    int FUN_004b4ba0(char* name);
};

class Class_004b4cf0 {
public:
    int FUN_004b4cf0(void* src, int len);
};

// FUNCTION: 0x484ce0
void __stdcall FUN_00484ce0(Class_004b4560* file)
{
    file->FUN_004b4560("Metal");
    int size = g_game->width * g_game->height;
    unsigned char* buf = new unsigned char[size];
    Cell_00484ce0* cells = g_game->cells;
    for (int i = 0; i < size; i++) {
        buf[i] = cells[i].metal;
    }
    ((Class_004b4ba0*)file)->FUN_004b4ba0("Plotmap");
    ((Class_004b4cf0*)file)->FUN_004b4cf0(buf, size);
    delete[] buf;
}
