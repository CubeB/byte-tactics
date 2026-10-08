// Decompiled by Opus and space-bunny-free. Names are provisional.
// The map's saved plot maps: the metal byte, the player-feature nibble and the
// visibility-mask buffer, read from and written to a chunked HapiBank file.

// Both includes stay: they decide whether width or height is loaded first in
// each multiply.
#include <windows.h>
#include <string.h>

#pragma pack(push, 1)
struct Cell {
    char unknown_0[0x7];
    unsigned char metal;                // +0x7
    char unknown_8[0x4];                // +0x8

    // The save and load views read the feature nibble with different widths
    // (five bits against four), so each keeps its own bitfield at +0xc.
    union {
        struct {
            unsigned char low : 3;
            unsigned char feature : 5;
        };
        struct {
            unsigned char low4 : 3;
            unsigned char feature4 : 4;
        };
    };
};

struct Game {
    char unknown_0[0x14233];
    int width;                          // +0x14233
    int height;                         // +0x14237
    char unknown_1423b[0x14273 - 0x1423b];
    void* visibilityMask;               // +0x14273
    char unknown_14277[0x14287 - 0x14277];
    Cell* cells;                        // +0x14287
};
#pragma pack(pop)

extern Game* g_game;

// Chunked file reader/writer.
#include "../util/hapi_bank.h"

// Writes the "Metal" "Plotmap" chunk: the metal byte of every map cell
// (the save counterpart of 0x484d60).
// FUNCTION: 0x484ce0
void __stdcall SaveMetalPlotmap(HapiBank* file)
{
    file->OpenAccount("Metal");
    int size = g_game->width * g_game->height;
    unsigned char* buf = new unsigned char[size];
    Cell* cells = g_game->cells;
    for (int i = 0; i < size; i++) {
        buf[i] = cells[i].metal;
    }
    file->OpenNamedBox("Plotmap");
    file->WriteBox(buf, size);
    delete[] buf;
}

// Writes the "PlayerFeatures" "Plotmap" chunk: the low four bits of the
// feature field of every map cell, two cells packed per byte (the even cell
// in the high nibble). Compare 0x484d60, which reads the "Metal" plot map.
// FUNCTION: 0x484df0
void __stdcall SavePlayerFeaturesPlotmap(HapiBank* file)
{
    file->OpenAccount("PlayerFeatures");
    Cell* cells = g_game->cells;
    int size = g_game->width * g_game->height / 2;
    unsigned char* buf = new unsigned char[size];
    if (buf) {
        for (int i = 0; i < size; i++) {
            buf[i] = (cells[i * 2].feature << 4) | (cells[i * 2 + 1].feature & 0xf);
        }
        file->OpenNamedBox("Plotmap");
        file->WriteBox(buf, size);
        delete buf;
    }
}

// Reads the "Metal" "Plotmap" chunk into the metal byte of every map cell.
// Defined after SavePlayerFeaturesPlotmap, not in address order: the order
// decides the base of this cell loop's induction variable.
// FUNCTION: 0x484d60
void __stdcall LoadMetalPlotmap(HapiBank* file)
{
    if (file->OpenAccount("Metal") && file->OpenNamedBox("Plotmap")) {
        int size = g_game->width * g_game->height;
        if (file->GetBoxSize() == size) {
            unsigned char* buf = new unsigned char[size];
            Cell* cells = g_game->cells;
            // The (unsigned int) cast keeps the count comparison unsigned, as
            // in the original.
            if ((unsigned int)file->ReadBox(buf, size) >= size) {
                for (int i = 0; i < size; i++) {
                    cells[i].metal = buf[i];
                }
                delete[] buf;
            }
        }
    }
}

// Reads the "PlayerFeatures" "Plotmap" chunk into the feature field of every
// map cell, two cells per byte (the reading counterpart of 0x484df0).
// FUNCTION: 0x484e80
void __stdcall LoadPlayerFeaturesPlotmap(HapiBank* file)
{
    if (file->OpenAccount("PlayerFeatures") && file->OpenNamedBox("Plotmap")) {
        int size = g_game->width * g_game->height / 2;
        if (file->GetBoxSize() == size) {
            unsigned char* buf = new unsigned char[size];
            if (buf) {
                Cell* cells = g_game->cells;
                // The (unsigned int) cast keeps the count comparison unsigned,
                // as in the original.
                if ((unsigned int)file->ReadBox(buf, size) >= size) {
                    Cell* c = cells;
                    for (int i = 0; i < size; i++, c += 2) {
                        c->feature4 = buf[i] >> 4;
                        c[1].feature4 = buf[i] & 0xf;
                    }
                    delete[] buf;
                }
            }
        }
    }
}

// Writes the "Mapping" "Data" chunk from the map's visibility-mask buffer; the
// reading counterpart is 0x484fa0.
// FUNCTION: 0x484f50
void __stdcall SaveMappingData(HapiBank* file)
{
    file->OpenAccount("Mapping");
    unsigned int size = g_game->width * g_game->height * sizeof(short) / 4;
    file->OpenNamedBox("Data");
    file->WriteBox(g_game->visibilityMask, size);
}

// Reads the "Mapping" "Data" chunk into the map's visibility-mask buffer.
// FUNCTION: 0x484fa0
void __stdcall LoadMappingData(HapiBank* file)
{
    if (file->OpenAccount("Mapping") && file->OpenNamedBox("Data")) {
        unsigned int size = g_game->width * g_game->height * sizeof(short) / 4;
        if (file->GetBoxSize() == size)
            file->ReadBox(g_game->visibilityMask, size);
    }
}
