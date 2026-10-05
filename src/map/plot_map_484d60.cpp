// Decompiled by Opus. Names are provisional.
#include <windows.h>

#pragma pack(push, 1)
struct Cell_00484d60 {
    char unknown_0[0x7];
    unsigned char metal;                // +0x7
    char unknown_8[0xd - 0x8];
};

struct Game {
    char unknown_0[0x14233];
    int width;                          // +0x14233
    int height;                         // +0x14237
    char unknown_1423b[0x14287 - 0x1423b];
    Cell_00484d60* cells;               // +0x14287
};
#pragma pack(pop)

extern Game* g_game;

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

// Reads the "Metal" "Plotmap" chunk into the metal byte of every map cell.
// FUNCTION: 0x484d60
void __stdcall LoadMetalPlotmap(Class_004b4560* file)
{
    if (file->FUN_004b4560("Metal") && ((Class_004b4ba0*)file)->FUN_004b4ba0("Plotmap")) {
        int size = g_game->width * g_game->height;
        if (((Class_004b4bf0*)file)->FUN_004b4bf0() == size) {
            unsigned char* buf = new unsigned char[size];
            Cell_00484d60* cells = g_game->cells;
            if (((Class_004b4c80*)file)->FUN_004b4c80(buf, size) >= size) {
                for (int i = 0; i < size; i++) {
                    cells[i].metal = buf[i];
                }
                delete[] buf;
            }
        }
    }
}
