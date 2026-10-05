// Decompiled by DeepSeek V4.1 Flash, finished by Claude Opus 5.5. Names are provisional.
//
// Finds the first in-use player whose info has the "ready" bit set (or 10
// when there is none). If that is the local player, the group change in
// `param` is applied locally (through AssignPlayerColor when the group is taken);
// otherwise packet 0x17 is sent to that player, or broadcast when there is
// none.
//
// What made it match: the search is an inlined helper returning an
// `unsigned char` that the caller widens into an `int` (the widening is what
// makes MSVC store the result straight into a stack byte on both exits), the
// loop tests the fields directly rather than through a per-index helper
// (which moves the "found" exit block), and the local player index is
// re-read from g_game in the compare instead of being cached in a local.
// The flag at info+0x97 is bit 0 of an `unsigned short` bitfield (0x451220
// writes it as a word); a byte field with `& 1` compiles the same here.
#pragma pack(push, 1)
struct PlayerInfo_004526c0 {
    char unknown_0[0x96];
    unsigned char field_96;            // +0x96
    unsigned short ready : 1;          // +0x97, mask 1
};

struct Player_004526c0 {
    int field_0;                       // +0x0
    int field_4;                       // +0x4
    char unknown_8[0x27 - 8];
    PlayerInfo_004526c0* info;         // +0x27
    char unknown_2b[0x73 - 0x2b];
    char flag_73;                      // +0x73
    char unknown_74[0x14b - 0x74];
};

struct Game {
    char unknown_0[0x1b63];
    Player_004526c0 players[10];       // +0x1b63
    char unknown_2851[0x2a38 - 0x2851];
    unsigned char* buffer;             // +0x2a38
    char unknown_2a3c[0x2a42 - 0x2a3c];
    unsigned char localPlayer;         // +0x2a42
};
#pragma pack(pop)

class PacketManager {
public:
    int SendAllQueued(int param_1);
};

extern Game* g_game;
extern int g_usePacketManager;
extern PacketManager g_packetManager;

int __stdcall SendPacketToPlayer(int from, int to, void* packet, int size);
int __stdcall BroadcastPacket(int player, void* data, int size);
int __stdcall AssignPlayerColor(int a, int b, int c);
int __stdcall IsColorFree(int a, int b);

static inline unsigned char FindReadyPlayer()
{
    for (unsigned char i = 0; i < 10; i++) {
        if (g_game->players[i].flag_73 && g_game->players[i].info->ready)
            return i;
    }
    return 10;
}

// FUNCTION: 0x4526c0
int __stdcall RequestPlayerColor(int param)
{
    Player_004526c0* p = &g_game->players[g_game->localPlayer];
    int i = FindReadyPlayer();
    if (i == g_game->localPlayer) {
        if (IsColorFree(p->field_4, param) == 0) {
            AssignPlayerColor(p->field_4, p->field_4, param);
            return 1;
        }
        p->info->field_96 = param;
        return 1;
    }

    unsigned char* buffer = g_game->buffer;
    buffer[0] = 0x17;
    buffer[1] = (unsigned char)param;

    int result;
    if (i == 10)
        result = BroadcastPacket(p->field_4, buffer, 2);
    else
        result = SendPacketToPlayer(p->field_4, g_game->players[i].field_4, buffer, 2);

    if (g_usePacketManager != 0)
        g_packetManager.SendAllQueued(1);
    return result;
}
