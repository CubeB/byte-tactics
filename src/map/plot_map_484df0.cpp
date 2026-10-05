// Decompiled by Opus. Names are provisional.
// Writes the "PlayerFeatures" "Plotmap" chunk: the low four bits of the
// feature field of every map cell, two cells packed per byte (the even cell
// in the high nibble). Compare 0x484d60, which reads the "Metal" plot map.
// Without a header such as <windows.h>, MSVC loads height before width for
// the multiply.
#include <windows.h>

#pragma pack(push, 1)
struct Cell_00484df0 {
    char unknown_0[0xc];
    unsigned char low : 3;              // +0xc
    unsigned char feature : 5;
};

struct Game {
    char unknown_0[0x14233];
    int width;                          // +0x14233
    int height;                         // +0x14237
    char unknown_1423b[0x14287 - 0x1423b];
    Cell_00484df0* cells;               // +0x14287
};
#pragma pack(pop)

extern Game* g_game;

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

// FUNCTION: 0x484df0
void __stdcall FUN_00484df0(Class_004b4560* file)
{
    file->FUN_004b4560("PlayerFeatures");
    Cell_00484df0* cells = g_game->cells;
    int size = g_game->width * g_game->height / 2;
    unsigned char* buf = new unsigned char[size];
    if (buf) {
        for (int i = 0; i < size; i++) {
            buf[i] = (cells[i * 2].feature << 4) | (cells[i * 2 + 1].feature & 0xf);
        }
        ((Class_004b4ba0*)file)->FUN_004b4ba0("Plotmap");
        ((Class_004b4cf0*)file)->FUN_004b4cf0(buf, size);
        delete buf;
    }
}
