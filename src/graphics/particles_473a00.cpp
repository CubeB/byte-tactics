// Decompiled by Opus. Names are provisional.
// Stays in its own file: merged with the module's second part, the fog arm's
// cell address picks the other SIB base (particles.cpp).
// The nano spark: moved by Step, drawn as one pixel by DrawParticle when the
// local player can see it, and dropped once IsExpired.

struct Vec3_004739b0 {
    int x, y, z;
    Vec3_004739b0& operator+=(const Vec3_004739b0& o)
    {
        x += o.x;
        y += o.y;
        z += o.z;
        return *this;
    }
};

// The integer halves of the 16.16 position.
struct Pos_004739b0 {
    char unknown_0[0x2];
    short x;                           // +0x2
    char unknown_4[0x2];
    short height;                      // +0x6
    char unknown_8[0x2];
    short y;                           // +0xa
};

#pragma pack(push, 1)

struct Rect_004b0510 {
    int x1;                          // +0x0
    int y1;                          // +0x4
    int x2;                          // +0x8
    int y2;                          // +0xc
};

struct MapSize_00473a00 {
    unsigned int width;              // +0x0
    unsigned int height;             // +0x4

    int Contains(unsigned int tx, unsigned int ty)
    {
        return tx < width && ty < height;
    }
};

struct ByteMap_00473a00 {
    unsigned char* data;             // +0x0
    MapSize_00473a00 size;           // +0x4

    unsigned char Get(int x, int y) { return data[size.width * y + x]; }
};

struct Player_00473a00 {
    char unknown_0[0x7c];
    ByteMap_00473a00 explored;       // +0x7c
    char unknown_88[0x14b - 0x88];   // stride 331
};

struct Game {
    char unknown_0[0x1b63];
    Player_00473a00 players[11];     // +0x1b63, stride 0x14b
    char unknown_299c[0x2a43 - 0x299c];
    unsigned char playerIndex;       // +0x2a43
    char unknown_2a44[0x14273 - 0x2a44];
    unsigned short* visibilityMask;  // +0x14273
    char unknown_14277[0x14281 - 0x14277];
    unsigned char flags;             // +0x14281, bit 1 (mask 2)
};

extern Game* g_game;

static inline int Identity_00473a00(int v) { return v; }

// The second arm, inlined. The two player parameters are not a typo: the
// Contains test through `p` and the width in the index through `q` are what
// stop the index from folding into the imul.
static inline int IsSeen_00473a00(Player_00473a00* p, Player_00473a00* q, int col, int row)
{
    if (!p->explored.size.Contains((unsigned int)col, (unsigned int)row))
        return 0;
    return (g_game->visibilityMask[q->explored.size.width * row + col] &
            (1 << g_game->playerIndex)) != 0;
}

// One nano spark (the element of NanoParticles' vector), 0x30 bytes.
class NanoParticle {
public:
    union {
        Vec3_004739b0 pos;             // +0x0
        Pos_004739b0 posw;
    };
    char unknown_c[0xc];
    Vec3_004739b0 vel;                 // +0x18
    char unknown_24[4];
    int flags;                         // +0x28, low 4 bits: frame; the colour
    int endTime;                       // +0x2c, the tick it expires

    void Step();
    void DrawParticle(int param_1, short x, short y);
    int IsExpired(int value);
};
#pragma pack(pop)

void __stdcall FillRectangle(void* surface, Rect_004b0510* rect, int color);

// FUNCTION: 0x473a00
void NanoParticle::DrawParticle(int param_1, short x, short y)
{
    Rect_004b0510 r;
    r.x1 = (short)(posw.x - x) + 0x80;
    r.y1 = (short)(posw.y - y) - (posw.height >> 1) + 0x20;
    r.x2 = r.x1 + 1;
    r.y2 = r.y1 + 1;

    // Two locals with the same value. The original reads the map width twice
    // per arm, once for the bounds test and once for the index, and a single
    // local makes MSVC 5 fold one of the two away. This spelling keeps both
    // loads and the prologue's register choice.
    Player_00473a00* p = &g_game->players[g_game->playerIndex];
    Player_00473a00* q = &g_game->players[g_game->playerIndex];
    int visible;
    if ((g_game->flags & 2) == 2) {
        int col = posw.x >> 5;
        int row = (posw.y - (posw.height >> 1)) >> 5;
        visible = p->explored.size.Contains((unsigned int)col, (unsigned int)row) &&
                  p->explored.Get(col, row) != 0;
    } else {
        int col = posw.x >> 5;
        int row = (posw.y - (posw.height >> 1)) >> 5;
        visible = Identity_00473a00(IsSeen_00473a00(p, q, col, row));
    }

    if (visible)
        FillRectangle((void*)param_1, &r, flags);
}

