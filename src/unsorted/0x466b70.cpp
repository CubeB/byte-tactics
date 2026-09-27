// Decompiled by space-bunny-free. Names are provisional.
//
// Not a byte match yet: 65%, everything matches except the four multiplies.
// The original has, in each of the four statements,
//     movsx eax, word ptr [base + 0x142e_b]
//     imul   eax, dword ptr [base + 0x1431_f]   (and the other three pairs)
// while this file compiles to
//     movsx edx, word ptr [base + 0x142e_b]
//     mov    eax, dword ptr [base + 0x1431_f]
//     imul   eax, edx
// i.e. MSVC 5 here puts the sign-extended short in a scratch register and the
// int operand in the accumulator, and uses the register form of imul. The
// original has the short in the accumulator and the int as imul's memory
// operand, which is the form VC5 emits when the multiply's left operand needs
// no conversion (checked: a 4-byte enum field, or the same expression divided
// by a constant instead of by g_game->divX, both give the memory-operand form).
// So the original's short field must reach the multiply as a 16-bit load that
// carries no separate conversion node, or the multiply must be a standalone
// expression whose target register is the accumulator. Tried and rejected:
// swapping the operands (identical code, VC5 canonicalises the pair), (int),
// (long) and (unsigned) casts, short locals, block-scoped and comma/assignment
// temporaries, static inline helpers (whole statement, product only, identity
// on the short, product with two args), nested struct and array member access,
// union member access, 16-bit int bitfields, reordering the struct members,
// product-into-a-local then divide in a separate statement, and every header
// set (tools/headers.py: all 128 give the same bytes).
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
