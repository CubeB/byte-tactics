// Decompiled by Opus, space-bunny-free, LongCat 2.5 Preview Free, deepseek-v4.1-flash, deepseek-v4.1, mimo-v2.6-pro and Sonnet. Names are provisional.
// Stays in its own file: merged with the module's second part, the fog arm's
// cell address picks the other SIB base and keeps the fog pointer in a register
// (particles.cpp).
// The smoke puff: drifted by Step, drawn by DrawParticle when the local player
// can see it, and dropped once IsExpired.
#include <stddef.h>
#include <stdlib.h>
// Only for its symbol ids: DrawParticle matches only in a window of the
// symbol count.
#include <stdio.h>

void* __stdcall GetGafFrame(void* a, int b);
void __stdcall DrawFrameBlended(void* dest, void* src, int x, int y);

#pragma pack(push, 1)
struct Pos_00474b80 {
    short x;                       // +0x00 (record +0x06)
    char unknown_2[0x4 - 0x2];
    short h;                       // +0x04 (record +0x0a)
    char unknown_6[0x8 - 0x6];
    short y;                       // +0x08 (record +0x0e)
};

struct MapSize_00474b80 {
    unsigned int width;            // +0x80
    unsigned int height;           // +0x84

    int Contains(int col, int row)
    {
        return (unsigned int)col < width && (unsigned int)row < height;
    }
};

struct ByteMap_00474b80 {
    unsigned char* data;           // +0x7c
    MapSize_00474b80 size;         // +0x80

    unsigned char Get(int x, int y) { return data[size.width * y + x]; }
};

// The header's type, kept local: its explored pointer cannot spell the
// ByteMap this fog test needs.
struct Player {
    char unknown_0[0x7c];
    ByteMap_00474b80 explored;     // +0x7c
    char unknown_88[0x14b - 0x88];
};

struct Game {
    char unknown_0[0x1b63];
    Player players[10];   // +0x1b63, stride 0x14b
    char unknown_2851[0x2a43 - 0x2851];
    unsigned char playerIndex;     // +0x2a43
    char unknown_2a44[0x14263 - 0x2a44];
    int rise;                      // +0x14263
    char unknown_14267[0x14273 - 0x14267];
    unsigned short* visibilityMask;// +0x14273
    char unknown_14277[0x14281 - 0x14277];
    unsigned short mapFlags;       // +0x14281
    char unknown_14283[0x37ecc - 0x14283];
    int windX;                     // +0x37ecc
    char unknown_37ed0[0x37ed4 - 0x37ed0];
    int windZ;                     // +0x37ed4
};

// One smoke puff (the element of SmokeParticles' vector), 0x20 bytes.
struct SmokeParticle {
    void* data;                    // +0x00, the animation
    union {
        struct {
            int x;                 // +0x04
            int y;                 // +0x08
            int z;                 // +0x0c
        };
        struct {
            char unknown_4[0x6 - 0x4];
            Pos_00474b80 posw;     // +0x06
        };
    };
    int limit;                     // +0x10
    int rounds;                    // +0x14, the frame
    int period;                    // +0x18
    int countdown;                 // +0x1c

    void Step();
    void DrawParticle(void* dest, short px, short py);
    int IsExpired(int unused);
};
#pragma pack(pop)

extern Game* g_game;

// The pin the earlier passes were looking for: a helper that returns its
// argument. It emits no instruction, but MSVC 5 allocates what it returns as a
// fresh live range, which is what holds the player pointer in edx across the
// pre-branch block. Remove it and the whole frame rotates.
static inline int Identity_00474b80(int v) { return v; }

static inline int IsSeen_00474b80(Player* p, Player* q, int col, int row)
{
    if (!p->explored.size.Contains(col, row))
        return 0;
    return (g_game->visibilityMask[q->explored.size.width * row + col] &
            (1 << g_game->playerIndex)) != 0;
}

// FUNCTION: 0x474b80
void SmokeParticle::DrawParticle(void* dest, short px, short py)
{
    Pos_00474b80* q = &posw;
    short sx = q->x - px + 0x80;
    short sy = q->y - (q->h >> 1) - py + 0x20;
    Player* p = &g_game->players[g_game->playerIndex];
    // The second spelling of the same record: the bounds test reads p, the mask
    // index re-reads the width through p2, which keeps both of the original's
    // two width loads.
    Player* p2 = &g_game->players[g_game->playerIndex];
    int visible;
    if ((g_game->mapFlags & 2) == 2) {
        int col = posw.x >> 5;
        int row = (posw.y - (posw.h >> 1)) >> 5;
        // Fog map read through Get(): keeps the fog pointer in a register.
        if (p->explored.size.Contains(col, row) &&
            p->explored.Get(col, row) != 0)
            visible = 1;
        else
            visible = 0;
    } else {
        int col = posw.x >> 5;
        int row = (posw.y - (posw.h >> 1)) >> 5;
        visible = Identity_00474b80(IsSeen_00474b80(p, p2, col, row));
    }
    if (visible)
        DrawFrameBlended(dest, GetGafFrame(data, rounds), sx, sy);
}

