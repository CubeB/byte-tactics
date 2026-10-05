// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x38a47];
    int field_38a47;                   // +0x38a47
};
#pragma pack(pop)

struct Pair_00419560 {
    int a;
    int b;
};

struct Option_00419560;

extern Game* g_game;
extern int DAT_00511bc0;
extern int DAT_00511bc4;
extern int DAT_00511bc8;
extern int DAT_00511c20;
extern int DAT_00511c34;
extern int DAT_00511c48;
extern int DAT_00511c50;
extern Pair_00419560 DAT_00511a60[44];
extern Pair_00419560 DAT_00511c60[44];
extern Option_00419560 DAT_00501d38;
extern Option_00419560 DAT_00501f48;
extern Option_00419560 DAT_00501fd0;

void __stdcall FUN_004b7760(Option_00419560* option);
void __stdcall FUN_004b78e0(void (__stdcall* callback)(int), int param_2);
void __stdcall FUN_00417890(int param_1);

// FUNCTION: 0x419560
void FUN_00419560()
{
    DAT_00511c20 = g_game->field_38a47;
    DAT_00511bc0 = 0;
    DAT_00511bc4 = 0;
    // The second field of the second table is cleared through a walking
    // pointer: the original keeps a separate induction pointer for it.
    Pair_00419560* p = DAT_00511c60;
    for (int i = 0; i < 44; i++) {
        DAT_00511a60[i].a = 0;
        DAT_00511a60[i].b = 0;
        DAT_00511c60[i].a = 0;
        p->b = 0;
        p++;
    }
    DAT_00511c34 = 0;
    DAT_00511bc8 = 0;
    DAT_00511c48 = 0;
    DAT_00511c50 = 0;
    FUN_004b7760(&DAT_00501d38);
    FUN_004b7760(&DAT_00501f48);
    FUN_004b7760(&DAT_00501fd0);
    FUN_004b78e0(FUN_00417890, 4);
}
