// Decompiled by Opus. Names are provisional.
// When the game is networked (flag 1 at +0x2a44), sends every playing
// player's 0xb9-byte data block (type 0x20) and then its 6-byte message
// (type 0x24, the same code as FUN_00452bd0).
// (windows.h only for its effect on base/index order; see tools/headers.py)
#include <windows.h>

#pragma pack(push, 1)
struct PlayerData_00450f90 {
    char unknown_0[0x90];
    int id;                            // +0x90
    char unknown_94[0xb9 - 0x94];
};

struct Player_00450f90 {
    int active;                        // +0x0
    int id;                            // +0x4
    char unknown_8[0x27 - 0x8];
    PlayerData_00450f90* data;         // +0x27
    char unknown_2b[0x73 - 0x2b];
    char state;                        // +0x73
    char unknown_74[0x13f - 0x74];
    char field_13f;                    // +0x13f
    char unknown_140[0x14b - 0x140];
};

struct Game {
    char unknown_0[0x1b63];
    Player_00450f90 players[10];       // +0x1b63
    char unknown_2851[0x2a38 - 0x2851];
    unsigned char* buffer;             // +0x2a38
    char unknown_2a3c[0x2a44 - 0x2a3c];
    unsigned short flags_2a44;         // +0x2a44
};

struct Packet_00450f90 {
    unsigned char type;                // +0x0
    PlayerData_00450f90 data;          // +0x1
};
#pragma pack(pop)

class PacketManager {
public:
    int SendAllQueued(int param_1);
};

extern Game* g_game;
extern int g_usePacketManager;
extern PacketManager g_packetManager;

int __stdcall BroadcastPacket(int player, void* data, int size);
void FUN_00450530();

static inline int IsPlaying(Player_00450f90* player)
{
    if (player->active == 0)
        return 0;
    if (player->state == 1 || player->state == 2)
        return 1;
    return 0;
}

// The packet is declared at function scope: its address escapes to
// BroadcastPacket in one iteration, so MSVC re-reads player->id around the
// stores into it in the next, as the original does.
// FUNCTION: 0x450f90
void BroadcastPlayerInfo()
{
    Packet_00450f90 packet;
    if (g_game->flags_2a44 & 1) {
        for (int i = 0; i < 10; i++) {
            Player_00450f90* player = &g_game->players[i];
            if (IsPlaying(player)) {
                packet.data = *player->data;
                packet.data.id = player->id;
                packet.type = 0x20;
                BroadcastPacket(player->id, &packet, sizeof(packet));
                if (IsPlaying(player)) {
                    unsigned char* msg = g_game->buffer;
                    msg[0] = 0x24;
                    *(int*)(msg + 1) = player->id;
                    msg[5] = player->field_13f;
                    BroadcastPacket(player->id, msg, 6);
                    if (g_usePacketManager != 0) {
                        g_packetManager.SendAllQueued(1);
                    }
                }
            }
        }
        FUN_00450530();
        g_packetManager.SendAllQueued(1);
    }
}
