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
class HapiBank {
public:
    int OpenAccount(char* name);
    int OpenNamedBox(char* name);
    int GetBoxSize();
    unsigned int ReadBox(void* buf, int size);
};

// Reads the "Metal" "Plotmap" chunk into the metal byte of every map cell.
// FUNCTION: 0x484d60
void __stdcall LoadMetalPlotmap(HapiBank* file)
{
    if (file->OpenAccount("Metal") && ((HapiBank*)file)->OpenNamedBox("Plotmap")) {
        int size = g_game->width * g_game->height;
        if (((HapiBank*)file)->GetBoxSize() == size) {
            unsigned char* buf = new unsigned char[size];
            Cell_00484d60* cells = g_game->cells;
            if (((HapiBank*)file)->ReadBox(buf, size) >= size) {
                for (int i = 0; i < size; i++) {
                    cells[i].metal = buf[i];
                }
                delete[] buf;
            }
        }
    }
}
