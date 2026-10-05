// Decompiled by space-bunny-free. Names are provisional.
// Reads the "PlayerFeatures" "Plotmap" chunk into the feature field of every
// map cell, two cells per byte (the reading counterpart of 0x484df0).
// Without a header such as <windows.h>, MSVC loads height before width.
#include <windows.h>

#pragma pack(push, 1)
struct Cell_00484e80 {
    char unknown_0[0xc];
    unsigned char low : 3;              // +0xc
    unsigned char feature : 4;
};

struct Game {
    char unknown_0[0x14233];
    int width;                          // +0x14233
    int height;                         // +0x14237
    char unknown_1423b[0x14287 - 0x1423b];
    Cell_00484e80* cells;               // +0x14287
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

// FUNCTION: 0x484e80
void __stdcall FUN_00484e80(Class_004b4560* file)
{
    if (file->FUN_004b4560("PlayerFeatures") && ((Class_004b4ba0*)file)->FUN_004b4ba0("Plotmap")) {
        int size = g_game->width * g_game->height / 2;
        if (((Class_004b4bf0*)file)->FUN_004b4bf0() == size) {
            unsigned char* buf = new unsigned char[size];
            if (buf) {
                Cell_00484e80* cells = g_game->cells;
                if (((Class_004b4c80*)file)->FUN_004b4c80(buf, size) >= size) {
                    Cell_00484e80* c = cells;
                    for (int i = 0; i < size; i++, c += 2) {
                        c->feature = buf[i] >> 4;
                        c[1].feature = buf[i] & 0xf;
                    }
                    delete[] buf;
                }
            }
        }
    }
}
