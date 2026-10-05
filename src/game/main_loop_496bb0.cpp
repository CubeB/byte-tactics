// Decompiled by DeepSeek V4.1 Flash, finished by Claude Opus 5.5. Names are provisional.
// Picks the next game-setup state: 4 or 5 from what FUN_00435100 returns when
// bit 2 of +0x2a44 is set, otherwise 3 for state 0x11 with that bit, or for
// state 0x10 with bit 4 of +0x2b4c and a sub-state of 0x12 or 0x13.
//
// The last two branches are written as nested ifs, each with its own copy of
// the state change; MSVC merges the copies (the 0x11 case ends in
// "je <tail>; jmp <body>"). A standalone "if (bitfield)" on an unsigned short
// bitfield compiles to "mov cl, [m]; shr cl, N; test cl, 1", while the same
// test inside an && chain folds to "test byte ptr [m], mask".

struct Sub_00496bb0 {
    char unknown_0[0xa6];
};

class Class_00435100 {
public:
    int FUN_00435100();
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x519];
    Sub_00496bb0 sub;                  // +0x519
    char unknown_5bf[0x2a44 - 0x519 - sizeof(Sub_00496bb0)];
    unsigned short bit0 : 1;           // +0x2a44
    unsigned short bit1 : 1;
    unsigned short bit2 : 1;
    unsigned short bit3 : 1;
    unsigned short rest_2a44 : 12;
    char unknown_2a46[0x2b4c - 0x2a46];
    unsigned short low_2b4c : 4;       // +0x2b4c
    unsigned short flag_2b4c : 1;      // bit 4
    unsigned short rest_2b4c : 11;
    char unknown_2b4e[0x2bbe - 0x2b4e];
    unsigned char state_2bbe;          // +0x2bbe
    unsigned char state_2bbf;          // +0x2bbf
    char unknown_2bc0[0x391e9 - 0x2bc0];
    Class_00435100* obj_391e9;         // +0x391e9
    char unknown_391ed[0x391f1 - 0x391ed];
    int mode;                          // +0x391f1
    void (*handler)();                 // +0x391f5
};
#pragma pack(pop)

extern Game* g_game;

void RefreshUnitInfo();
void FUN_00426e80();
void FUN_004c2470();
void FUN_004c2870();
void FUN_00496ce0();
void FUN_00496db0();
void FUN_00497f40();
void __cdecl LeaveNetGameCallback(int param);
void __stdcall FUN_004b4fd0(void (__cdecl *callback)(int), int param);
void __stdcall FUN_004ab170(Sub_00496bb0* sub, unsigned int* param_2, int* param_3);

// FUNCTION: 0x496bb0
void FUN_00496bb0()
{
    RefreshUnitInfo();
    FUN_00426e80();
    if (g_game->bit2 && g_game->obj_391e9->FUN_00435100() == 1) {
        FUN_004c2470();
        g_game->mode = 4;
        g_game->handler = FUN_00496db0;
        FUN_004b4fd0(LeaveNetGameCallback, 0);
    } else if (g_game->bit2 && g_game->obj_391e9->FUN_00435100() == 2) {
        FUN_004c2470();
        g_game->mode = 5;
        g_game->handler = FUN_00497f40;
        FUN_004b4fd0(LeaveNetGameCallback, 0);
    } else if (g_game->state_2bbe == 0x11) {
        if (g_game->bit2) {
            FUN_004c2470();
            g_game->mode = 3;
            g_game->handler = FUN_00496ce0;
            FUN_004b4fd0(LeaveNetGameCallback, 0);
        }
    } else if (g_game->state_2bbe == 0x10) {
        if (g_game->flag_2b4c) {
            if (g_game->state_2bbf == 0x12 || g_game->state_2bbf == 0x13) {
                FUN_004c2470();
                g_game->mode = 3;
                g_game->handler = FUN_00496ce0;
                FUN_004b4fd0(LeaveNetGameCallback, 0);
            }
        }
    }
    FUN_004c2470();
    FUN_004ab170(&g_game->sub, 0, 0);
    FUN_004c2870();
}
