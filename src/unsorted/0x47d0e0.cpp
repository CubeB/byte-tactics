// Decompiled by GPT-6-Luna. Names are provisional.
// GAVE UP: Best check was 30.4%. The draw-cell loops are represented, but
// register allocation and row-pointer updates differ across the branches.
extern char *g_game;
extern char DAT_004fd660[];
void __stdcall FUN_00483210(int, int);
void __stdcall FUN_0047e5c0(int, int, void *);
void __stdcall FUN_00440a70(void *);

// FUNCTION: 0x47d0e0
void __stdcall FUN_0047d0e0(char *map)
{
    int flags;
    int x;
    int y;
    int width;
    int height;
    short *cell;
    short *row;
    int j;
    int i;
    short color;
    struct VtableArg { void *vtable; } local;

    if (*(int *)(map + 0x82) != *(int *)(g_game + 0x142b7)) {
        width = (short)*(int *)(map + 0x7e);
        height = (short)(*(unsigned int *)(map + 0x7e) >> 16);
        x = (short)*(short *)(map + 0x76);
        y = (short)*(short *)(map + 0x78);
        color = *(short *)(map + 0xa8);
        cell = (short *)(*(int *)(g_game + 0x14287) + (y * *(int *)(g_game + 0x14233) + x) * 13);
        flags = *(int *)(map + 0x110);

        if (flags & 0x20000000) {
            int index = 0;
            for (j = 0; j < height; ++j) {
                row = cell;
                for (i = 0; i < width; ++i) {
                    unsigned char mask = *(unsigned char *)(*(int *)(*(int *)(map + 0x92) + 0x14e) + index++);
                    if (*row == color) *row = 0;
                    if (mask & 1) *((unsigned char *)row + 0xc) &= 0xfd;
                    row = (short *)((char *)row + 13);
                }
                cell = (short *)((char *)row + (*(int *)(g_game + 0x14233) - width) * 13);
            }
            FUN_00483210(((y - 1) << 16) | (x - 1), ((height + 2) << 16) | (width + 2));
        } else if ((flags & 3) == 1) {
            for (j = 0; j < height; ++j) {
                row = cell;
                for (i = 0; i < width; ++i) {
                    if (*row == color) *row = 0;
                    row = (short *)((char *)row + 13);
                }
                cell = (short *)((char *)row + (*(int *)(g_game + 0x14233) - width) * 13);
            }
        } else if ((flags & 3) == 2) {
            for (j = 0; j < height; ++j) {
                row = cell;
                for (i = 0; i < width; ++i) {
                    if (row[1] == color) row[1] = 0;
                    row = (short *)((char *)row + 13);
                }
                cell = (short *)((char *)row + (*(int *)(g_game + 0x14233) - width) * 13);
            }
        }
    }

    flags = *(int *)(map + 0x110) & 0xf7ffffff;
    *(int *)(map + 0x110) = flags;
    if ((flags & 0x04000000) != 0) {
        *(int *)(map + 0x110) = flags & 0xfbffffff;
        local.vtable = DAT_004fd660;
        FUN_0047e5c0(*(int *)(map + 0x76), *(int *)(map + 0x7e), &local);
    }
    FUN_00440a70(map);
}
