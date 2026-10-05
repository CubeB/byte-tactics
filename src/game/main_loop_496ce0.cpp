// Decompiled by DeepSeek V4.1 Flash, finished by Claude Opus 5.5. Names are provisional.
// If the local player's info has bit 0 of +0x97 set, sends a one-byte
// message 8 to its id; otherwise, with bit 4 of +0x2b4c set, calls
// SendNetHeartbeat once FUN_004b6340() passes DAT_0051f304 (then 0x3c later).
// Every path then switches to state 5 (FUN_00497f40).
//
// Each branch has its own copy of the state change; MSVC merges the first two
// and keeps the third, which reuses the g_game pointer still in edx. `msg`
// declared at function scope keeps its store before the pushes.

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

struct Game {
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

extern Game* g_game;
extern int DAT_0051f304;

int __stdcall BroadcastPacket(int player, void* data, int size);
unsigned int FUN_004b6340();
void SendNetHeartbeat();
void FUN_00497f40();
void __stdcall FUN_004b4fd0(void (__cdecl *callback)(int), int param);
void __cdecl LeaveNetGameCallback(int param);

// FUNCTION: 0x496ce0
void FUN_00496ce0()
{
    char msg;
    Player_00496ce0* p = &g_game->players[g_game->localPlayer];
    if (p->info->flags & 1) {
        msg = 8;
        BroadcastPacket(p->dpid, &msg, 1);
        g_game->mode = 5;
        g_game->handler = FUN_00497f40;
        FUN_004b4fd0(LeaveNetGameCallback, 0);
    } else if (g_game->flags_2b4c.flag) {
        if (DAT_0051f304 < FUN_004b6340()) {
            DAT_0051f304 = FUN_004b6340() + 0x3c;
            SendNetHeartbeat();
        }
        g_game->mode = 5;
        g_game->handler = FUN_00497f40;
        FUN_004b4fd0(LeaveNetGameCallback, 0);
    } else {
        g_game->mode = 5;
        g_game->handler = FUN_00497f40;
        FUN_004b4fd0(LeaveNetGameCallback, 0);
    }
}
