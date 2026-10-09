// Grid: one visibility grid (Thaldren's Grid), 0x10 bytes: the cell array, its
// width and height in cells and the cell count. It is the +0x7c of a player
// record and the two at +0x1428f and +0x1429f of Game, the fog and line-of-
// sight grid the map code reads and walks. The one declaration of the type;
// the cells behind the pointer are two bytes each and stay private to the
// files that walk them.
#ifndef GRID_H
#define GRID_H

struct Grid {
    unsigned char* cells;              // +0x0
    unsigned int width;                // +0x4
    unsigned int height;               // +0x8
    int count;                         // +0xc
    Grid(void);
};

#endif
