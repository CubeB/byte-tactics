// Decompiled by Space Bunny Free, finished by space-bunny-free. Names are provisional.
// Maps a map pixel (x, y) onto the terrain: walks z down from just above row y
// 16 cells at a time until the projected row (z - ground / 2) reaches y, then
// interpolates between that row and the next one.
//
// PARTIAL: 96.9%, the first 7 instructions of the loop preamble are out.
//
//   original                      ours
//   shl eax, 0x10                 mov edi, ebp
//   mov [esp + 0x10], eax         mov ebx, 0x80
//   mov eax, ebp                  and edi, 0xf0
//   and al, 0xf0                  shl eax, 0x10
//   mov ebx, 0x80                 add edi, ebx
//   lea edi, [eax + 0x80]         mov [esp + 0x10], eax
//   shl edi, 0x10                 shl edi, 0x10
//
// The original stores p.x first and then builds the starting z with a 3-address
// lea, so the two uses of the constant 0x80 (the loop counter's start and the
// z offset) are not shared; here MSVC materialises 0x80 in ebx once and reuses
// it. Ruled out: masking y as a byte or a char (the original's `and al, 0xf0`),
// splitting the mask into its own statement, the order of the three statements,
// declaring the counter inside the for, a while loop with the increments in the
// body, spelling 0x80 as 128, and every header set (headers.py) plus the
// compiler-state probe (0 to 82 unused extern declarations, all 96.9%).
// Claude Sonnet 5.5 pass (#591), and a warning about the code below: the
// original's mask is NOT `y & 0xf0`. `mov eax, ebp; and al, 0xf0` keeps the
// upper 24 bits of y, so it computes `y & ~0xf` (round down to a 16-cell block),
// and `y & 0xf0` below is only what happens to score 96.9 percent. MSVC 5 emits
// the short `and al, -16` for `y & ~0xf` only when the value is computed in eax
// (tiny test: `return (y & ~0xf) + 0x80;` gives `and al,-16; add eax,128`), so the
// original has the mask as a temporary in eax followed by a 3-address
// `lea edi,[eax+0x80]`, while ours computes it in place in edi, where there is no
// byte register, and shares the 0x80 held in ebx (`add edi,ebx`). With `~0xf`
// (also `-16`, `0xfffffff0`) the file is 388 bytes and scores 86.2 because of
// `and edi,0xfffffff0`. Scored, all identical to that: a named temporary, `int
// i` before or after, `z = (t + 0x80) << 16` and the two-statement form, a
// helper for the mask, a helper for the whole start z, `* 0x10000`. The right
// answer is the `~0xf` form plus whatever makes the mask temporary and the
// counter's 0x80 separate values in eax and ebx; whoever finds it should keep
// `~0xf`.
#pragma pack(push, 1)
struct Game_00484b50 {
    char unknown_0[0x14223];
    int baseX;                         // +0x14223
    int baseY;                         // +0x14227
    char unknown_1422b[0x1427f - 0x1422b];
    unsigned char seaLevel;            // +0x1427f
};
#pragma pack(pop)

extern Game_00484b50* g_game;

struct Fixed_00484b50 {
    unsigned short frac;               // +0x0
    short whole;                       // +0x2
};

union Coord_00484b50 {
    int value;
    Fixed_00484b50 parts;
};

struct Pos_00484b50 {
    Coord_00484b50 x;                  // +0x0
    Coord_00484b50 y;                  // +0x4
    Coord_00484b50 z;                  // +0x8
};

int __stdcall FUN_00485070(Pos_00484b50* pos);

#define max(a, b) (((a) > (b)) ? (a) : (b))

// FUNCTION: 0x484b50
void __stdcall FUN_00484b50(int x, int y, Pos_00484b50* out)
{
    if (x < 0)
        x = 0;
    if (x >= g_game->baseX)
        x = g_game->baseX - 1;
    if (y < 0)
        y = 0;
    if (y >= g_game->baseY)
        y = g_game->baseY - 1;

    Pos_00484b50 p;
    int s1;
    int s2;
    p.x.value = x << 16;
    int i = 0x80;
    int z = ((y & 0xf0) + 0x80) << 16;
    for (; i >= 0; i -= 0x10, z -= 0x100000) {
        p.z.value = z;
        p.y.value = max(FUN_00485070(&p), g_game->seaLevel) << 16;
        s1 = p.z.parts.whole - (p.y.parts.whole >> 1);
        if (s1 <= y)
            goto found;
    }
    goto done;

found:
    {
        Pos_00484b50 q = p;
        q.z.value = p.z.value + 0x100000;
        q.y.value = max(FUN_00485070(&q), g_game->seaLevel) << 16;
        s2 = q.z.parts.whole - (q.y.parts.whole >> 1);
        if ((s1 < s2 && y >= s1) || y <= s2) {
            p.z.value = p.z.value + ((y - s1) << 20) / (s2 - s1);
            p.y.value = max(FUN_00485070(&p), g_game->seaLevel) << 16;
        }
    }
done:
    *out = p;
}
