// Decompiled by Opus, space-bunny-free, LongCat 2.5 Preview Free, GPT-6.1-sol, mimo-v2.6-pro and Sonnet. Names are provisional.
// The wake spark: moved and cycled through its colours by Step, drawn as one
// pixel by DrawParticle when the local player can see it, and dropped once it
// has expired and is not under water.
#include <stddef.h>

struct Vec3_00474580 {
    int x, y, z;
    void operator+=(const Vec3_00474580& o) { x += o.x; y += o.y; z += o.z; }
};

struct Rect_004b0510 {
    int x1;                          // +0x0
    int y1;                          // +0x4
    int x2;                          // +0x8
    int y2;                          // +0xc
};

#pragma pack(push, 1)
struct MapSize_004745e0 {
    unsigned int width;             // +0x80
    unsigned int height;            // +0x84

    int Contains(int col, int row)
    {
        return (unsigned int)col < width && (unsigned int)row < height;
    }
};

struct ByteMap_004745e0 {
    unsigned char* data;            // +0x7c
    MapSize_004745e0 size;          // +0x80

    unsigned char Get(int tx, int ty) { return data[size.width * ty + tx]; }
};

struct Map_004745e0 {               // one entry of g_game->players
    char unknown_0[0x7c];
    ByteMap_004745e0 explored;      // +0x7c
    char unknown_88[0x14b - 0x88];
};

struct Game {
    char unknown_0[0x1b63];
    Map_004745e0 players[1];         // +0x1b63
    char unknown_1[0x2a43 - 0x1b63 - 0x14b];
    unsigned char playerIndex;       // +0x2a43
    char unknown_2a44[0x14273 - 0x2a44];
    unsigned short* visibilityMask;  // +0x14273, one bit per player
    char unknown_14277[0x1427f - 0x14277];
    unsigned char byte_1427f;        // +0x1427f
    char unknown_14280;
    unsigned char flags;             // +0x14281, bit 1 (mask 2)
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall FillRectangle(void* surface, Rect_004b0510* rect, int color);
int __stdcall GetGroundHeight(void* param_1);

// The pin the earlier passes were looking for: a helper that returns its
// argument. It emits no instruction, but MSVC 5 allocates what it returns as a
// fresh live range, which is what holds the frame in place. Remove it and the
// whole frame rotates.
static inline int Identity_004745e0(int v) { return v; }

// The whole mask arm, the Contains test included; the index re-reads the width
// through the second pointer, which keeps both of the original's width loads.
static inline int IsSeen_004745e0(Map_004745e0* p, Map_004745e0* q, int col, int row)
{
    if (!p->explored.size.Contains(col, row))
        return 0;
    return (g_game->visibilityMask[q->explored.size.width * row + col] &
            (1 << g_game->playerIndex)) != 0;
}

#pragma pack(push, 1)
struct Pos_004745e0 {
    short x;                        // +0
    char unknown_2[2];
    short height;                   // +4
    char unknown_6[2];
    short y;                        // +8

    // Is this record's position inside the explored byte map of the local
    // player (flags bit 1 set), or inside their bit of the shared visibility
    // mask (bit clear)? The two arms keep their own fail block, as the original
    // does.
    int Visible()
    {
        Map_004745e0* p = &g_game->players[g_game->playerIndex];
        Map_004745e0* p2 = &g_game->players[g_game->playerIndex];
        int visible;
        if ((g_game->flags & 2) == 2) {
            int col = x >> 5;
            int row = (y - (height >> 1)) >> 5;
            if (p->explored.size.Contains(col, row) &&
                p->explored.Get(col, row) != 0)
                visible = 1;
            else
                visible = 0;
        } else {
            int col = x >> 5;
            int row = (y - (height >> 1)) >> 5;
            visible = Identity_004745e0(IsSeen_004745e0(p, p2, col, row));
        }
        return visible;
    }
};
#pragma pack(pop)

// One wake spark (the element of WakeParticles' vector), 0x44 bytes.
#pragma pack(push, 1)
class Class_00474580 {
public:
    int unknown_0;                     // +0x00
    union {
        struct {
            Vec3_00474580 pos;         // +0x04
            Vec3_00474580 pos2;        // +0x10
            Vec3_00474580 vel;         // +0x1c
        };
        struct {
            char unknown_4[0x6 - 0x4];
            Pos_004745e0 posw;         // +0x06
        };
    };
    int min;                           // +0x28
    int max;                           // +0x2c
    int value;                         // +0x30, the colour
    int step;                          // +0x34
    int tick;                          // +0x38
    int period;                        // +0x3c
    int field_40;                      // +0x40, the tick it expires

    void Step();
    void DrawParticle(void* surface, short px, short py);
    int IsExpired(int param_1);
};
#pragma pack(pop)

// Moves by the velocity, then every `period` ticks steps a value that wraps
// between min and max.
// FUNCTION: 0x474580
void Class_00474580::Step()
{
    pos += vel;
    tick = (tick + 1) % period;
    if (tick == 0) {
        value += step;
        if (value > max) value = min;
        if (value < min) value = max;
    }
}

// FUNCTION: 0x4745e0
void Class_00474580::DrawParticle(void* surface, short px, short py)
{
    Rect_004b0510 r;
    short sx = posw.x - px;
    short sy = posw.y - py;
    r.x1 = sx + 0x80;
    r.y1 = sy - (posw.height >> 1) + 0x20;
    r.x2 = r.x1 + 1;
    r.y2 = r.y1 + 1;
    // Visible() stays a member: inlined here, the arms' pos loads are CSE'd
    // against the header's.
    if (posw.Visible())
        FillRectangle(surface, &r, value);
}

// Expired once the tick has passed field_40 or the spark is above the sea
// (its ground height not below the sea level).
// FUNCTION: 0x474720
int Class_00474580::IsExpired(int param_1)
{
    if (param_1 <= field_40) {
        int r = GetGroundHeight(&pos);
        if (r < g_game->byte_1427f)
            return 0;
    }
    return 1;
}
