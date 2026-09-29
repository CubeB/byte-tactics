// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Retried by space-bunny-free. No functional rewrite improved the 84.5% draft.
// A 128-combination header sweep also left the score at 84.5%.
//
// NOT A MATCH: 84.5% (300-byte original, ours 305), re-verified by
// tools/check.py. Everything matches byte for byte except the FIRST arm (the
// one taken when bit 1 of the flag byte at g_game+0x14281 is set, the `seen`
// byte map). The second arm, the prologue, the player-pointer arithmetic, the
// rect, the flags test and the whole tail call are identical.
//
// WHAT IS LEFT IN THAT ARM (ours -> original):
//   - ours materialises the width into edi before `sub eax, edx` and zeroes
//     edx (`mov edi,[esi+0x80]; sub eax, edx; xor edx,edx; cmp ecx,edi`); the
//     original lets edx (which held height>>1) die on the sub and reuses it
//     for the width (`sub eax, edx; mov edx,[esi+0x80]; cmp ecx, edx`).
//   - ours builds the cell address in edi with the map in ebx
//     (`imul edi,eax; add edi,ebx; cmp byte [edi+ecx],0`); the original builds
//     the index in edx and puts the map pointer in the just-dead row register
//     (`imul edx,eax; mov eax,[esi+0x7c]; add edx,ecx; cmp byte [edx+eax],0`).
//   - ours ends the taken path with `mov edx,1; xor eax,eax; test edx,edx;
//     setne al`; the original has a plain `mov eax,1` and a fail block of
//     `xor eax, eax` after the body.
//
// The blocker is the same register-allocation wall the siblings hit (0x473590
// 84.0, 0x474170 85.4, 0x474b80 84.6, 0x4745e0 79.8). This retry measured,
// on top of everything the earlier rounds did:
//   - Dropping the pinning `if (visible) visible = 1; else visible = 0;` from
//     the first arm, with any spelling of the tests, drops the whole function
//     to 67.3% and 296 bytes and rotates the ENTIRE prologue: g_game moves out
//     of ebx into edi and this->x/this->y swap between bp and bx. The pinning
//     tail is the only thing found that keeps the prologue byte-identical, and
//     it is also the direct cause of the 9 extra bytes above (mov edx,1 / xor
//     eax,eax / test edx,edx / setne al) and of the early xor.
//   - With the pinning tail in place, the compare spelling is NOT a lever:
//     `p->size.Contains(col,row)` and a hand-written
//     `(unsigned)col < p->size.width && (unsigned)row < p->size.height` compile
//     to the same 305 bytes at 84.5%, as do `p` and `q` in the index
//     (`p->fogMap[q->size.width*row+col]` == `q->fogMap[q->size.width*row+col]`).
//   - Also measured with the pinning tail in place, all worse: an index local
//     `int cell = q->size.width*row+col` (82.9%, 302), the height test before
//     the width test (80.0%, 299), the cell read through a `static inline`
//     FogCell(p,col,row) helper (79.8%, 294), the sibling 0x474b80 arm locals
//     `unsigned char* seen` plus `unsigned int w` (37.7%, 305), and one
//     `int hit = cond; visible = hit ? 1 : 0;` (65.1%, 318).
//   - So the mask arm wants NO long-lived visible (so edx is free for the width
//     and the index) while the prologue wants one. A stronger model may find
//     the construct that gives both; nothing I tried in this file does.
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
