// Decompiled by space-bunny-free. Names are provisional.
//
// 75%: every instruction matches except the four multiplies. The original has,
// in each of the four statements, the sign-extended short in the accumulator
// and the int as imul's memory operand,
//     movsx eax, word ptr [base + 0x142e_b]
//     imul   eax, dword ptr [base + 0x1431_f]   (and the other three pairs)
// while this file compiles to
//     movsx edx, word ptr [base + 0x142e_b]
//     mov    eax, dword ptr [base + 0x1431_f]
//     imul   eax, edx
// i.e. MSVC 5 puts the short in a scratch register and the int in the
// accumulator here. Each pair is 2 bytes too long, so we are 173 bytes to 165.
//
// The cause is the state of the compiler when the function is compiled, not
// the source. With this file alone the plain expressions below always give the
// `imul eax, edx` form; putting 24 unused `static inline` functions (which
// emit no code at all) or 24 small functions above the function in the file
// makes this same source give the original's `imul eax, [mem]` form, and 22
// or fewer do not. The 56 game functions that precede this one in the exe's
// contiguous run (the run starts at 0x462bd0 and has no gap larger than 0x960)
// are most likely one translation unit, so the original file had well over 24
// functions compiled first. Only four of the neighbours are matched
// (0x466580, 0x4669b0, 0x466aa0, 0x466b00) and defining all four above this
// function is not enough. This is the regrouping problem in docs/AGENTS.md, so
// the four statements are left in the plainest form.
//
// Source-level attempts that do not change it (all measured on scratch copies
// of this file): swapping the operands (VC5 canonicalises the pair), (int),
// (long), (unsigned) and narrowing (short) casts, including a (short) cast of
// a 32-bit field, which MSVC folds into the very same `movsx word` load but
// still allocates to a scratch register, an int local for the short, a
// `static inline` helper for the product and for the whole quotient, a
// `short*`, `char*` or `int*` address form, the four shorts as an array, a
// reused local, storing the product through param_1, and the address shape of
// either operand (array index, nested struct member, base + offset cast): the
// multiply's operand order does not follow the address shape here, unlike the
// commutative adds of the sibling 0x4669b0. Dividing by a constant instead of
// by g_game->divX does change it, and so does any real computation on the left
// operand, `(sizeX << 4) * scaleX`, but both emit instructions the original
// does not have. Also tried: union members, 16-bit bitfields, reordering the
// struct members, and every header combination (tools/headers.py: all 128
// give the same bytes).
#pragma pack(push, 1)
struct Game_00466b70 {
    char unknown_0[0x1422b];
    int divX;                          // +0x1422b
    int divY;                          // +0x1422f
    char unknown_14233[0x1423b - 0x14233];
    int zoomX;                         // +0x1423b
    int zoomY;                         // +0x1423f
    char unknown_14243[0x142e7 - 0x14243];
    short originX;                     // +0x142e7
    short originY;                     // +0x142e9
    short sizeX;                       // +0x142eb
    short sizeY;                       // +0x142ed
    char unknown_142ef[0x1431f - 0x142ef];

    int scaleX;                        // +0x1431f
    int scaleY;                        // +0x14323
};
#pragma pack(pop)

extern Game_00466b70* g_game;

// FUNCTION: 0x466b70
void __stdcall FUN_00466b70(int* param_1)
{
    param_1[0] = g_game->sizeX * g_game->scaleX / g_game->divX + g_game->originX;
    param_1[1] = g_game->sizeY * g_game->scaleY / g_game->divY + g_game->originY;
    param_1[2] = g_game->sizeX * g_game->zoomX * 16 / g_game->divX + param_1[0] - 1;
    param_1[3] = g_game->sizeY * g_game->zoomY * 16 / g_game->divY + param_1[1] - 1;
}
