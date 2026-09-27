// Decompiled by deepseek-v4.1-flash. Names are provisional.
// PARTIAL, 84.1%. Every call, argument and the shared third branch are in the
// right place; the two remaining differences are inside the state == 0x11 /
// state == 0x10 condition:
//   - the 0x2a44 bit 2 test emits "jne <body>; jmp <tail>" where the original
//     emits "je <tail>; jmp <body>" (same targets, opposite polarity). The
//     original is the nested if/else-if with the body placed after the
//     state == 0x10 block (MSVC merged the two identical bodies there);
//     writing the nested form here does not merge and the body's registers
//     move from edx/eax to eax/ecx.
//   - the 0x2b4c bit 4 test folds to "test byte [mem], 0x10" where the
//     original loads it and shifts ("mov cl, [mem]; shr cl, 4; test cl, 1").
//     A nested "if (bit4)" reaches the shift form but picks dl, which clobbers
//     edx and again moves the shared body's registers.
// The nested form of the 0x2a44 read (unsigned short bitfield) is needed for
// the shift encoding; a plain mask folds to "test byte ..., 4".

struct Struct_4ab170 {
    char unknown_0[0x18];
    void* field_18;
};

struct Bits_2a44 {
    unsigned short b0 : 1;
    unsigned short b1 : 1;
    unsigned short b2 : 1;
    unsigned short b3 : 1;
    unsigned short b4 : 1;
    unsigned short rest : 11;
};

struct Bits_2b4c {
    unsigned short b0 : 1;
    unsigned short b1 : 1;
    unsigned short b2 : 1;
    unsigned short b3 : 1;
    unsigned short b4 : 1;
    unsigned short rest : 11;
};

extern char* g_game;

class Class_00435100 {
public:
    int FUN_00435100();
};

void FUN_0042bd10();
void FUN_00426e80();
void FUN_004c2470();
void FUN_004c2870();
void FUN_00496ce0();
void FUN_00496db0();
void FUN_00497f40();
void FUN_004578f0(int param);
void __stdcall FUN_004b4fd0(void (*callback)(int), int param);
void __stdcall FUN_004ab170(Struct_4ab170* param_1, unsigned int* param_2, int* param_3);

// FUNCTION: 0x496bb0
void FUN_00496bb0()
{
    FUN_0042bd10();
    FUN_00426e80();

    if ((*(unsigned char*)(g_game + 0x2a44) & 4) != 0
        && (*(Class_00435100**)(g_game + 0x391e9))->FUN_00435100() == 1) {
        FUN_004c2470();
        *(int*)(g_game + 0x391f1) = 4;
        *(void (**)())(g_game + 0x391f5) = FUN_00496db0;
        goto l_b4fd0;
    }
    else if ((*(unsigned char*)(g_game + 0x2a44) & 4) != 0
             && (*(Class_00435100**)(g_game + 0x391e9))->FUN_00435100() == 2) {
        FUN_004c2470();
        *(int*)(g_game + 0x391f1) = 5;
        *(void (**)())(g_game + 0x391f5) = FUN_00497f40;
        goto l_b4fd0;
    }

    if (*(unsigned char*)(g_game + 0x2bbe) == 0x11) {
        if (((Bits_2a44*)(g_game + 0x2a44))->b2)
            goto l_branch3;
    }
    else if (*(unsigned char*)(g_game + 0x2bbe) == 0x10) {
        if (((Bits_2b4c*)(g_game + 0x2b4c))->b4
            && (*(unsigned char*)(g_game + 0x2bbf) == 0x12
                || *(unsigned char*)(g_game + 0x2bbf) == 0x13))
            goto l_branch3;
    }
    goto l_tail;

l_branch3:
    FUN_004c2470();
    *(int*)(g_game + 0x391f1) = 3;
    *(void (**)())(g_game + 0x391f5) = FUN_00496ce0;
l_b4fd0:
    FUN_004b4fd0(FUN_004578f0, 0);
l_tail:
    FUN_004c2470();
    FUN_004ab170((Struct_4ab170*)(g_game + 0x519), 0, 0);
    FUN_004c2870();
}
