// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Game_00421e60 {
    char unknown_0[0x14233];
    int mapWidth;                    // +0x14233
};

// A 13-byte map cell; 0xfffe marks a cell that belongs to a larger feature
// whose origin cell lies (offsetY, offsetX) cells back.
struct Cell_00421e60 {
    char unknown_0[8];
    unsigned short feature;          // +0x8
    unsigned char offsetY;           // +0xa
    unsigned char offsetX;           // +0xb
    char unknown_c;
};
#pragma pack(pop)

extern Game_00421e60* g_game;

// Reading the field directly each time (no local) gives `or ax, 0xffff` for
// the 0xffff return; a local gives `mov eax, 0xffff`.
// FUNCTION: 0x421e60
unsigned short __stdcall FUN_00421e60(Cell_00421e60* cell)
{
    if (cell->feature >= 0xfffb) {
        if (cell->feature == 0xfffe)
            return (cell - (cell->offsetY * g_game->mapWidth + cell->offsetX))->feature;
        return 0xffff;
    }
    return cell->feature;
}
