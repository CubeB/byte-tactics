// Decompiled by space-bunny-free. Names are provisional.
//
// PARTIAL: 87.0% (208 bytes against our 210). The 12.4 fixed point extraction,
// the cell layout, the four height loads and the sign-corrected /16 are right;
// the return expression is deliberately written out twice, duplicated from the
// first attempt, because that is what puts the twenty first instructions in the
// original's registers.
//
// Two hunks are left:
//  - the second row's address. The original computes cells[idx] + 12*w and then
//    adds w (`lea ecx,[ecx+edi*4]`, `lea edi,[ecx+edx]`, `mov bl,[edi+4]`);
//    here w is added first and 12*w is folded into the addressing mode. Every
//    spelling tried (c[w], c + w, a separate pointer, byte pointers with
//    13*w, 12*w + w, two explicit indices) compiles the same way.
//  - the final interpolation. The original keeps the first row's lerp whole in
//    ecx (`sar ecx,4; add ecx,esi`, then `sub eax,ecx` and `add eax,ecx`),
//    which needs that value used twice; naming it a local gives the right tail
//    but puts x >> 4 in edx instead of ecx and the first twenty instructions
//    stop matching. Roughly 200 combinations of the two were tried; this is one
//    register allocation puzzle, not a structural one.
// Neither every header set nor the compiler-state probe (0 to 82 unused extern
// declarations) changes anything here.

#pragma pack(push, 1)
struct Cell_00485070 {
    char unknown_0[0x4];
    unsigned char height;              // +0x4
    char unknown_5[0xd - 0x5];
};

struct Game_00485070 {
    char unknown_0[0x14233];
    int width;                         // +0x14233
    int height;                        // +0x14237
    char unknown_1423b[0x14287 - 0x1423b];
    Cell_00485070* cells;              // +0x14287
};
#pragma pack(pop)

struct Fixed_00485070 {
    unsigned short frac;
    short whole;
};

struct Pos_00485070 {
    Fixed_00485070 x;                  // +0x0
    Fixed_00485070 y;                  // +0x4
    Fixed_00485070 z;                  // +0x8
};

extern Game_00485070* g_game;

// FUNCTION: 0x485070
int __stdcall FUN_00485070(Pos_00485070* p)
{
    int x = p->x.whole;
    int z = p->z.whole;
    int xc = x >> 4;
    int zc = z >> 4;
    int xf = x & 15;
    int zf = z & 15;
    if (xc < 0 || xc + 1 >= g_game->width || zc < 0 || zc + 1 >= g_game->height)
        return -1;
    Cell_00485070* c = &g_game->cells[zc * g_game->width + xc];
    int a = c->height;
    int b = c[1].height;
    int e = c[g_game->width].height;
    int f = c[g_game->width + 1].height;
    return a + (b - a) * xf / 16 +
        ((e + (f - e) * xf / 16) - (a + (b - a) * xf / 16)) * zf / 16;
}
