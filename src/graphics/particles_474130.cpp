// Decompiled by Opus, space-bunny-free, deepseek-v4.1-flash and GPT-6.1-sol. Names are provisional.
// The exhaust puff: moved and animated by Step, drawn by DrawParticle when
// the local player can see it, and dropped once IsExpired.
#include <stddef.h>

void* __stdcall GetGafFrame(void* a, int b);
void __stdcall DrawFrameBlended(void* dest, void* src, int x, int y);

struct Vec3i_00474130 {
    int x;
    int y;
    int z;
    Vec3i_00474130& operator+=(const Vec3i_00474130& o)
    {
        x += o.x;
        y += o.y;
        z += o.z;
        return *this;
    }
};

#pragma pack(push, 1)
struct Pos_00473590 {
    short x;                       // +0x00 (record +0x06)
    char unknown_2[0x4 - 0x2];
    short h;                       // +0x04 (record +0x0a)
    char unknown_6[0x8 - 0x6];
    short y;                       // +0x08 (record +0x0e)
};

struct MapSize_00473590 {
    unsigned int width;            // +0x80
    unsigned int height;           // +0x84

    int Contains(int col, int row)
    {
        return (unsigned int)col < width && (unsigned int)row < height;
    }
};

struct Player_00473590 {
    char unknown_0[0x7c];
    unsigned char* seen;           // +0x7c
    MapSize_00473590 size;         // +0x80
    char unknown_88[0x14b - 0x88];
};

struct Game {
    char unknown_0[0x1b63];
    Player_00473590 players[10];   // +0x1b63, stride 0x14b
    char unknown_2851[0x2a43 - 0x2851];
    unsigned char playerIndex;     // +0x2a43
    char unknown_2a44[0x14273 - 0x2a44];
    unsigned short* visibilityMask;// +0x14273
    char unknown_14277[0x14281 - 0x14277];
    unsigned char fogFlags;        // +0x14281
};

// One exhaust puff (the element of ThrustParticles' vector), 0x3c bytes.
class Class_00474130 {
public:
    void* data;                    // +0x00, the animation
    union {
        struct {
            Vec3i_00474130 pos;    // +0x04
            Vec3i_00474130 pos1;   // +0x10
            Vec3i_00474130 vel;    // +0x1c
        };
        struct {
            char unknown_4[0x6 - 0x4];
            Pos_00473590 posw;     // +0x06
        };
    };
    int mod2_28;                   // +0x28, the frame count
    int field_2c;                  // +0x2c, the frame
    int val_30;                    // +0x30
    int mod_34;                    // +0x34
    int field_38;                  // +0x38, the tick it expires

    void Step();
    void DrawParticle(void* dest, short px, short py);
    int IsExpired(int value);
};
#pragma pack(pop)

extern Game* g_game;

static inline int Identity_00474170(int v) { return v; }

static inline int IsSeen_00474170(Player_00473590* p, Player_00473590* q, int col, int row)
{
    if (!p->size.Contains(col, row))
        return 0;
    return (g_game->visibilityMask[q->size.width * row + col] &
            (1 << g_game->playerIndex)) != 0;
}

// The three adds are an inlined vector operator+=.
// FUNCTION: 0x474130
void Class_00474130::Step()
{
    pos += vel;
    val_30 = (val_30 + 1) % mod_34;
    if (val_30 == 0) {
        field_2c = (field_2c + 1) % mod2_28;
    }
}

// FUNCTION: 0x474170
void Class_00474130::DrawParticle(void* dest, short px, short py)
{
    Pos_00473590* q = &posw;
    short sx = q->x - px + 0x80;
    short sy = q->y - (q->h >> 1) - py + 0x20;
    Player_00473590* p = &g_game->players[g_game->playerIndex];
    Player_00473590* p2 = &g_game->players[g_game->playerIndex];
    int visible;
    if ((g_game->fogFlags & 2) == 2) {
        int col = posw.x >> 5;
        int row = (posw.y - (posw.h >> 1)) >> 5;
        if (p->size.Contains(col, row) &&
            p->seen[p2->size.width * row + col] != 0)
            visible = 1;
        else
            visible = 0;
    } else {
        int col = posw.x >> 5;
        int row = (posw.y - (posw.h >> 1)) >> 5;
        visible = Identity_00474170(IsSeen_00474170(p, p2, col, row));
    }
    if (visible)
        DrawFrameBlended(dest, GetGafFrame(data, field_2c), sx, sy);
}

// Whether the tick has passed the puff's expiry.
// FUNCTION: 0x4742a0
int Class_00474130::IsExpired(int value)
{
    return value > field_38;
}
