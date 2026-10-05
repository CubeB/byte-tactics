// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free and GPT-6.1-sol, finished by space-bunny-free. Names are provisional.
// MATCH (deepseek-v4.1-flash, #4241). 300 bytes, byte for byte the original.
//
// The inherited version was 91.4% with six instructions off in two places:
// the fog arm's cell address picked edi for the map pointer (the original
// reuses the dead row register for it), and the second arm carried a redundant
// `bool` self-correction whose only purpose was to pin the register
// allocation. Two independent changes fixed both, and they are the answer to
// the "allocator-only" wall this family (0x473590, 0x474170, 0x4745e0,
// 0x474b80) has been stuck on.
//
// 1. The fog arm is a ByteMap::Get inline. `{data at +0x7c, size at +0x80}`
//    with `unsigned char Get(int x, int y) { return data[size.width * y + x]; }`
//    (the same inline as 0x407f74, 0x465b6a and 0x475470) compiles to the
//    original's four instructions exactly: the width load, the imul into edx,
//    the data pointer loaded into the register the multiply just freed, then
//    the col add with col as the index. Spelling the same lookup as
//    `p->fogMap[q->size.width * row + col]` always folds the data pointer into
//    the add instead, which is what the 91.4% draft showed.
//
// 2. The mask arm is an inline `IsSeen(p, q, col, row)` helper wrapped in a
//    trivial inline `Identity(v) { return v; }`, and the wrap is the
//    allocation lever: it pins the prologue, the pre-branch block and the call
//    block without emitting a single extra instruction, so the second arm
//    keeps the original's `neg eax / sbb eax,eax / neg eax` tail instead of
//    the self-correction's six extra instructions. Two details are load
//    bearing:
//      * the Contains test reads through `p` and the index through `q`, two
//        separate player pointers. With one pointer the width load folds into
//        the imul (`imul eax, [esi+0x80]`) and the arm loses the original's
//        `mov edx, [esi+0x80] / imul edx, eax` pair; two parameters give the
//        compiler two address nodes and it materialises the load.
//      * the helper must contain the whole arm (Contains test included) and be
//        called through Identity. IsSeen alone, Identity around an inline
//        expression, or Identity around only the mask test all rotate the
//        prologue (67.7 to 70.3 percent).
//
// Measured and rejected on the way: `visible &= 1` as a second-arm tail
// (96.6 percent, one extra `and eax,1`), the same self-correction spelled as
// `!!visible`, `visible ? 1 : 0`, `(bool)visible` or an if/else (87 to 90),
// `add_self`/`++visible` (96.6 but semantically wrong), 128 header sets on
// both the plain and the helper body (flat, so this is not compiler state),
// w/m locals as in 0x4745e0 (45 to 68), a Visible() method with early returns
// (67.7), Identity's parameter type, the rect statement order, and every
// no-op unary spelling of the tail (all folded, all left the prologue
// rotated).
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

struct Game_00473a00 {
    char unknown_0[0x1b63];
    Player_00473a00 players[11];     // +0x1b63, stride 0x14b
    char unknown_299c[0x2a43 - 0x299c];
    unsigned char playerIndex;       // +0x2a43
    char unknown_2a44[0x14273 - 0x2a44];
    unsigned short* visibilityMask;  // +0x14273
    char unknown_14277[0x14281 - 0x14277];
    unsigned char flags;             // +0x14281, bit 1 (mask 2)
};

extern Game_00473a00* g_game;

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

class Class_00473a00 {
public:
    char unknown_0[0x2];
    short x;                         // +0x2
    char unknown_4[0x2];
    short height;                    // +0x6
    char unknown_8[0x2];
    short y;                         // +0xa
    char unknown_c[0x28 - 0xc];
    int color;                       // +0x28
    char unknown_2c[0x30 - 0x2c];

    void FUN_00473a00(int param_1, short x, short y);
};
#pragma pack(pop)

void __stdcall FUN_004bf6f0(void* surface, Rect_004b0510* rect, int color);

// FUNCTION: 0x473a00
void Class_00473a00::FUN_00473a00(int param_1, short x, short y)
{
    Rect_004b0510 r;
    r.x1 = (short)(this->x - x) + 0x80;
    r.y1 = (short)(this->y - y) - (this->height >> 1) + 0x20;
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
        int col = this->x >> 5;
        int row = (this->y - (this->height >> 1)) >> 5;
        visible = p->explored.size.Contains((unsigned int)col, (unsigned int)row) &&
                  p->explored.Get(col, row) != 0;
    } else {
        int col = this->x >> 5;
        int row = (this->y - (this->height >> 1)) >> 5;
        visible = Identity_00473a00(IsSeen_00473a00(p, q, col, row));
    }

    if (visible)
        FUN_004bf6f0((void*)param_1, &r, this->color);
}
