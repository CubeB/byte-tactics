// Decompiled by deepseek-v4.1-flash. Names are provisional.
//
// NOT A MATCH: 84.5% (300-byte original, ours 305). Prologue, player-pointer
// arithmetic, the rect, the tail call and, since this retry, the whole mask arm
// match byte for byte. Only the fog arm differs:
//   - ours emits an extra `mov edi, [esi+0x80]` before `sub eax, edx` and an
//     extra `xor edx, edx`; the original loads the width into edx after the
//     subtraction and has no early zeroing.
//   - ours builds the index in edi with fogMap in ebx, so the cell address is
//     [edi+ecx]; the original builds it in edx with fogMap in the just-dead eax,
//     giving `imul edx, eax; mov eax, [esi+0x7c]; add edx, ecx; [edx+eax]`.
//   - ours ends the taken path with `mov edx,1; xor eax,eax; test edx,edx;
//     setne al`; the original has a plain `mov eax,1`.
//
// The mask arm was the lever this round. Writing the fail path as an
// early-return shape (`if (!Contains) visible = 0; else visible = ...;`) is
// what turns the two `jae` of the old spelling into the original's `jae fail;
// jb body` with the fail block before the body; a `Contains` method that
// inlines to `tx < width && ty < height` is enough, the two-arm if/else alone
// is not.
//
// The fog arm is a hard register-allocation knot. Any spelling that produces
// the original fog arm text (the canonical `if (cond) visible = 1; else
// visible = 0;`, a `? 1 : 0` ternary, a goto early-exit, or an inlined
// IsExplored/ArmA helper returning one value per path) also moves g_game out
// of ebx into edi and rotates this->x/this->y through bp/bx, which then breaks
// the whole prologue and the mask arm. The shape kept below (a dead
// `visible = 0` plus a trailing `if (visible) visible = 1; else visible = 0;`)
// is the only one found that pins g_game in ebx; it costs the extra width load,
// the early xor and the setne tail. Every other attempt (helpers as free
// functions or members, with/without a second width pointer, `Contains` vs
// inline comparisons, bool/unsigned locals, pre-initialising visible before
// the outer if, an unused-declaration compiler-state sweep, `#include
// <stddef.h>`) either scored lower or flipped g_game. The original's own
// sibling 0x4745e0 (same draw-if-visible shape, 66.3% stuck on the same
// g_game-in-the-wrong-register wall) suggests this is compiler state that a
// spelling alone may not reach.
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

struct Player_00473a00 {
    char unknown_0[0x7c];
    unsigned char* fogMap;           // +0x7c
    MapSize_00473a00 size;           // +0x80
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

extern Game_00473a00* g_game;

void __stdcall FUN_004bf6f0(void* surface, Rect_004b0510* rect, int color);

// FUNCTION: 0x473a00
void Class_00473a00::FUN_00473a00(int param_1, short x, short y)
{
    Rect_004b0510 r;
    r.x1 = (short)(this->x - x) + 0x80;
    r.y1 = (short)(this->y - y) - (this->height >> 1) + 0x20;
    r.x2 = r.x1 + 1;
    r.y2 = r.y1 + 1;

    Player_00473a00* p = &g_game->players[g_game->playerIndex];
    // The original reads the map width twice per arm, once for the bounds test
    // and once for the index; a second pointer with the same value keeps the
    // two loads apart. A helper taking the pointer does not: MSVC 5 CSEs them.
    Player_00473a00* q = &g_game->players[g_game->playerIndex];
    int visible;
    if ((g_game->flags & 2) == 2) {
        int col = this->x >> 5;
        int row = (this->y - (this->height >> 1)) >> 5;
        visible = 0;
        if (((unsigned int)col < p->size.width && (unsigned int)row < p->size.height) &&
            p->fogMap[q->size.width * row + col] != 0)
            visible = 1;
        if (visible)
            visible = 1;
        else
            visible = 0;
    } else {
        int col = this->x >> 5;
        int row = (this->y - (this->height >> 1)) >> 5;
        if (!p->size.Contains((unsigned int)col, (unsigned int)row))
            visible = 0;
        else
            visible = (g_game->visibilityMask[q->size.width * row + col] &
                       (1 << g_game->playerIndex)) != 0;
    }

    if (visible)
        FUN_004bf6f0((void*)param_1, &r, this->color);
}
