// Cell: one cell of the map's cell grid, 13 bytes, the array at
// g_game+0x14287. The one declaration of the struct for the files that read or
// write a map cell. map/features.cpp keeps its own view, since it spells a
// byte differently; a view that splits the flags byte into bitfields
// (map/plot_map.cpp, ingame/info_panel_46a610.cpp) keeps its own too.
#ifndef CELL_H
#define CELL_H

#pragma pack(push, 1)

struct Cell {                          // 13 bytes per cell
    unsigned short unit;               // +0x0, id of the unit owning the cell
    unsigned short unit2;              // +0x2
    unsigned char height;              // +0x4
    unsigned char high;                // +0x5, highest floor
    unsigned char low;                 // +0x6, lowest floor
    unsigned char metal;               // +0x7
    unsigned short feature;            // +0x8
    unsigned char offsetY;             // +0xa
    unsigned char offsetX;             // +0xb
    unsigned char flags;               // +0xc
};

#pragma pack(pop)

#endif
