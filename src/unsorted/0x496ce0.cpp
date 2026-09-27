// Decompiled by deepseek-v4.1-flash. Names are provisional.
//
// PARTIAL (79.2%). Everything matches except register allocation:
// - Original loads the 0x2b4c bitfield into `al` (so `test al,1` is 2 bytes);
//   ours loads it into `cl` (`test cl,1`, 3 bytes), which shifts every later
//   offset by one. The flag test itself (`mov; shr 4; test 1`) matches, so the
//   `unsigned short` bitfield is right. MSVC frees `eax` on the else path in
//   the original but keeps `p`'s player pointer live there in ours.
// - The two duplicated tail copies pick different scratch registers: the
//   original uses ecx then edx (tail A) and edx then eax (tail B); ours uses
//   edx then eax and edx then ecx.
// Tried and ruled out: the tail as a static inline helper, a local Game*
// pointer, reference/const player pointer, direct re-indexing, different msg
// declaration order and scope, `int`/`unsigned char` msg, and all 128
// header sets from tools/headers.py. The store order only matched once `msg`
// was declared at function scope and assigned inside the branch.

#pragma pack(push, 1)
struct PlayerInfo_00496ce0 {
    char unknown_0[0x97];
    unsigned char flags;               // +0x97, bit 0 tested
};

struct Player_00496ce0 {
    int active;                        // +0x00
    int dpid;                          // +0x04
    char unknown_8[0x27 - 0x8];
    PlayerInfo_00496ce0* info;         // +0x27
    char unknown_2b[0x14b - 0x2b];
};

struct Flags_00496ce0 {
    unsigned short low : 4;            // +0x2b4c, bits 0-3
    unsigned short flag : 1;           // bit 4
    unsigned short rest : 11;
};

struct Game_00496ce0 {
    char unknown_0[0x1b63];
    Player_00496ce0 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;         // +0x2a42
    char unknown_2a43[0x2b4c - 0x2a43];
    Flags_00496ce0 flags_2b4c;         // +0x2b4c
    char unknown_2b4e[0x391f1 - 0x2b4e];
    int mode;                          // +0x391f1
    void (*handler)();                 // +0x391f5
};
#pragma pack(pop)

extern Game_00496ce0* g_game;
extern int DAT_0051f304;

int __stdcall FUN_00451df0(int player, void* data, int size);
unsigned int FUN_004b6340();
void FUN_00456310();
void FUN_00497f40();
void __stdcall FUN_004b4fd0(void (*callback)(int), int param);
void FUN_004578f0(int param);

// FUNCTION: 0x496ce0
void FUN_00496ce0()
{
    char msg;
    Player_00496ce0* p = &g_game->players[g_game->localPlayer];
    if (p->info->flags & 1) {
        msg = 8;
        FUN_00451df0(p->dpid, &msg, 1);
    }
    else if (g_game->flags_2b4c.flag) {
        if (DAT_0051f304 < FUN_004b6340()) {
            DAT_0051f304 = FUN_004b6340() + 0x3c;
            FUN_00456310();
        }
    }
    else {
        g_game->mode = 5;
        g_game->handler = FUN_00497f40;
        FUN_004b4fd0(FUN_004578f0, 0);
        return;
    }
    g_game->mode = 5;
    g_game->handler = FUN_00497f40;
    FUN_004b4fd0(FUN_004578f0, 0);
}
