// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// This file of the original was built with /Gz, so the function is __stdcall;
// found by the orchestrator's calling-convention sweep of every partial.
// Rebuilds the radar picture: fits the map onto 126 pixels along its long side,
// then blits it onto the stored radar frame, or, when there is none, scales the
// 8x8 icon map up to twice that size through a temporary picture.
//
// PARTIAL: 98.4%, same 548 byte length. Only the four-byte hunk at the top of
// the fill loop and the six-byte hunk at its bottom differ; every instruction
// is present and in the same registers, only two independent operations are
// scheduled on opposite sides of each other. What still differs:
//   1. At 0x466888 the original computes `test ebp, ebp` before it stores the
//      temp pointer and j = 0: `mov esi, eax; test ebp, ebp; mov [temp], esi;
//      mov [j], 0; jle`. Ours stores j = 0 first:
//      `mov esi, eax; mov [j], 0; test ebp, ebp; mov [temp], esi; jle`.
//   2. At 0x466954 the original does `inc edi` first, then reloads w2/h2, then
//      compares and only then restores ebx: `inc edi; mov eax, [w2]; mov ebp,
//      [h2]; cmp edi, eax; mov ebx, eax; jl`. Ours reloads w2/h2, then
//      `inc edi; mov ebx, eax; cmp edi, eax; jl`.
// Both are scheduler tie-breaks: over 50 loop shapes (for/while/do-while,
// guarded and unguarded, pre/post increment, reversed bounds, function-scope
// counters, countdown forms, inlined index/pixel/body helpers) and all 768
// header sets headers.py tries, the compiler emits the same two orders. Adding
// real neighbours (0x4665d0) in the file changes nothing either. The body
// itself (x, y, index, value, pixel) matches instruction for instruction.
// <windows.h> is required: without it the `(y / 32) * (rowWidth / 2)`
// multiply evaluates its operands in the opposite order.
#include <windows.h>

#pragma pack(push, 1)
struct Frame_004b8ae0 {
    unsigned short a;               // +0x0
    unsigned short b;               // +0x2
    unsigned short c;               // +0x4
    unsigned short d;               // +0x6
    unsigned char flag8;            // +0x8
    unsigned char flag9;            // +0x9
    unsigned char flaga;            // +0xa
    unsigned char flagb;            // +0xb
    char unknown_c[4];              // +0xc
    int unknown_10;                 // +0x10
    char unknown_14[4];             // +0x14
};
#pragma pack(pop)

struct IconSet_00466780 {
    int count;                      // +0x0
    unsigned char (*cell)[32][32];  // +0x4
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x1422b];
    int mapWidth;                   // +0x1422b
    int mapHeight;                  // +0x1422f
    int rowWidth;                   // +0x14233
    char unknown_14237[0x1426b - 0x14237];
    Frame_004b8ae0* radarFrame;      // +0x1426b
    char unknown_1426f[0x14283 - 0x1426f];
    IconSet_00466780* iconSet;       // +0x14283
    char unknown_14287[0x1428b - 0x14287];
    unsigned short* mapValues;       // +0x1428b
    char unknown_1428f[0x142e3 - 0x1428f];
    void* picture;                  // +0x142e3
    short posX;                     // +0x142e7
    short posY;                     // +0x142e9
    short width;                    // +0x142eb
    short height;                   // +0x142ed
};
#pragma pack(pop)

extern Game* g_game;
extern char DAT_005074f8[];
extern char DAT_005074e8[];

void __stdcall FrameFromSurface(Frame_004b8ae0* dst, void* src);
void __stdcall DownsampleFrame(Frame_004b8ae0* dst, Frame_004b8ae0* src);
void __stdcall DrawPixel(void* picture, int x, int y, int pixel);
void* __stdcall AllocSurface(char* name, int width, int height);
void __stdcall FreeSurface(void* picture);

// FUNCTION: 0x466780
void __stdcall BuildRadarPicture()
{
    int mapWidth = g_game->mapWidth;
    int mapHeight = g_game->mapHeight;
    int width;
    int height;
    if (mapWidth >= mapHeight) {
        width = 126;
        height = mapHeight * 126 / mapWidth;
        g_game->posX = 0;
        g_game->posY = (126 - height) / 2;
    } else {
        width = mapWidth * 126 / mapHeight;
        height = 126;
        g_game->posX = (126 - width) / 2;
        g_game->posY = 0;
    }
    g_game->width = width;
    g_game->height = height;
    g_game->picture = AllocSurface(DAT_005074f8, width, height);
    Frame_004b8ae0 frame;
    FrameFromSurface(&frame, g_game->picture);
    if (g_game->radarFrame) {
        DownsampleFrame(g_game->radarFrame, &frame);
        return;
    }
    int h2 = height * 2;
    int w2 = width * 2;
    void* temp = AllocSurface(DAT_005074e8, w2, h2);
    for (int j = 0; j < h2; j++) {
        for (int i = 0; i < w2; i++) {
            int x = g_game->mapWidth * i / w2;
            int y = g_game->mapHeight * j / h2;
            int index = (y / 32) * (g_game->rowWidth / 2) + x / 32;
            unsigned short value = g_game->mapValues[index];
            if (value >= g_game->iconSet->count) {
                value = 0;
            }
            int pixel = g_game->iconSet->cell[value][y % 32][x % 32];
            DrawPixel(temp, i, j, pixel);
        }
    }
    Frame_004b8ae0 tempFrame;
    FrameFromSurface(&tempFrame, temp);
    DownsampleFrame(&tempFrame, &frame);
    FreeSurface(temp);
}
