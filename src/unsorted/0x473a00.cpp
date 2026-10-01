// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free and GPT-6.1-sol, finished by space-bunny-free. Names are provisional.
// Fourth pass (space-bunny-free): 91.4% as reported by check.py, 94.1% by a
// true instruction LCS (88/102 -> 96/102 of the original's instructions).
// The inherited 85.0% draft was kept for three earlier passes by two models.
//
// WHAT CHANGED AND WHY. The inherited draft put a redundant self-correction
// (`if (visible) visible = 1; else visible = 0;`) at the END of the FIRST arm.
// That self-correction is what kept the prologue byte identical: without any
// self-correction anywhere, MSVC 5 drops to 67.3% and rotates the whole
// prologue (g_game out of ebx into edi, this->x and this->y swap between
// bp/bx and bp/di). But inside the first arm the self-correction compiles to
// `xor eax,eax / cmp / setne al / mov edx,eax`, which forced MSVC to keep the
// arm's boolean in eax and so to pick ebp for the fog-map base pointer. The
// original needs eax free at that point (it reuses the dead row register for
// the base pointer), so the boolean must not live in eax.
//
// Moving the self-correction OUT of the first arm and into the second, and
// spelling it through a `bool` temporary, gives the first arm the branchy
// `&&` shape the original has (three conditional jumps to one shared
// `xor eax,eax` fail block, then `mov eax,1 / jmp`), keeps the prologue byte
// identical, and leaves the second arm byte identical too.
//
//   Measured on top of this version, all worse: the self-correction spelled
//   `if (visible) visible = 1; else visible = 0;` in the second arm (309 bytes,
//   87.5%), `visible = !!visible` (309, 87.5%), `visible = (visible != 0)`
//   (309, 87.5%), `if (!visible) visible = 0` (302, 85.9%), a reversed index
//   `col + width*row` (309, 87.5%), a fog pointer local in the first arm
//   (315, 46.7%), `*(q->fogMap + width*row + col)` (323, 46.9%), the pinning in
//   BOTH arms (320, 37.6%), swapping which of the two player pointers is used
//   for the bounds test and for the index (296, 67.3%), declaring col/row at
//   function scope (296, 67.3%), a `MapSize` struct copy (292, 43.8%), and an
//   index local `int cell`/`unsigned int cell` in the first arm (no change at
//   all: MSVC 5 scalar-replaces it, byte for byte the same output), grouping
//   the fog map pointer and the size into one `Fog { unsigned char* map;
//   MapSize size; }` at Player+0x7c and reading both out of it (314 bytes,
//   91.4%, byte for byte the same output), taking the index from `q` rather
//   than `p` (91.4%, identical output), and moving the self-correction inside
//   the second arm's `else` arm (305 bytes, 61.2%, the prologue rotates).
//
// So none of the ways of re-spelling the first arm's cell address changes its
// four instructions: with `q->size.width * row + col` as the index,
// `col + q->size.width * row` as the index, or `p` instead of `q` for the base,
// MSVC 5 picks the same three instructions every time. Only a construct that
// makes the map pointer and the index two separately live values would change
// it, and none of the index locals I tried is one.
//
// NOT A MATCH: 91.4% as check.py reports it (300-byte original, ours 314),
// re-verified with tools/check.py. Only 6 of the original's 102 instructions
// are still not matched, and they are in exactly two places:
//
//   1. First arm, the cell address (ours -> original):
//        mov edi, [esi+0x7c]        ;  imul edx, eax
//        imul edx, eax              ;  mov eax, [esi+0x7c]
//        add edx, edi               ;  add edx, ecx
//        cmp byte [edx+ecx], 0      ;  cmp byte [edx+eax], 0
//      Ours adds the map POINTER into the index and leaves col as the
//      addressing-mode displacement; the original adds col into the index and
//      puts the map pointer in the register the multiply just freed. The whole
//      first arm above these four instructions, and everything else in the
//      function, is byte identical, so this is a register-choice difference in
//      one four-instruction window, not a structural one.
//
//   2. Second arm, the self-correction tail we still have to keep for the
//      allocation (ours -> original):
//        xor ecx, ecx               ;  (nothing)
//        test eax, eax              ;
//        setne cl                   ;
//        xor eax, eax               ;
//        test cl, cl                ;
//        setne al                   ;
//      The original has no instructions there at all: its second arm's value
//      arrives in eax already normalised by `neg eax / sbb eax,eax / neg eax`.
//      The self-correction is only in the source because removing it rotates
//      the prologue; every cheaper spelling tried above either rotates the
//      prologue too or lands at 87.5%.
//
// The blocker is the same register-allocation wall the siblings hit (0x473590
// 84.0, 0x474170 85.4, 0x474b80 84.6, 0x4745e0 79.8), but it is now only 6
// instructions wide and both remaining spots are named above.
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
        visible = ((unsigned int)col < p->size.width &&
                   (unsigned int)row < p->size.height) &&
                  p->fogMap[q->size.width * row + col] != 0;
    } else {
        int col = this->x >> 5;
        int row = (this->y - (this->height >> 1)) >> 5;
        if (!p->size.Contains((unsigned int)col, (unsigned int)row))
            visible = 0;
        else
            visible = (g_game->visibilityMask[q->size.width * row + col] &
                       (1 << g_game->playerIndex)) != 0;
        // A redundant self-correction, and a real one: the original's boolean
        // here is a `mov reg,1` / `xor reg,reg` pair rather than a `setcc`, and
        // this is what pins the register allocation of the whole function.
        // Remove it and the prologue rotates (67.3%). Every cheaper spelling
        // of it costs bytes in the second arm's tail.
        {
            bool b = visible;
            if (b)
                visible = 1;
            else
                visible = 0;
        }
    }

    if (visible)
        FUN_004bf6f0((void*)param_1, &r, this->color);
}
