// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Declaring the Msg20 message at function scope (not inside the loop) is what
// puts its address in eax and fixes the whole send block; the trailing
// g_game+0x2bee flag is a 1-bit unsigned short bitfield, which is what makes
// the original emit `or byte ptr [eax+0x2bee], 1` in one instruction.
#include <windows.h>
#include <string.h>

#pragma pack(push, 1)
struct PlayerInfo_00456310 {
    char unknown_0[0x90];
    int id;                            // +0x90
    char unknown_94[0x96 - 0x94];
    unsigned char field_96;            // +0x96
    unsigned short bit_97 : 1;         // +0x97
    unsigned short rest_97 : 15;
    char unknown_99[0xb9 - 0x99];
};

struct Player_00456310 {
    int active;                        // +0x00
    int id;                            // +0x04
    char unknown_8[0x27 - 8];
    PlayerInfo_00456310* info;         // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char state;               // +0x73
    char unknown_74[0x13f - 0x74];
    unsigned char field_13f;           // +0x13f
    char unknown_140[0x14b - 0x140];
};

struct Game {
    char unknown_0[0x1b5f];
    int field_1b5f;                    // +0x1b5f
    Player_00456310 players[10];       // +0x1b63
    char unknown_2851[0x2a38 - 0x2851];
    unsigned char* buffer;             // +0x2a38
    char unknown_2a3c[0x2a44 - 0x2a3c];
    unsigned char flags_2a44;          // +0x2a44
    char unknown_2a45[0x2bee - 0x2a45];
    unsigned short bit0_2bee : 1;      // +0x2bee
    unsigned short rest_2bee : 15;
    char unknown_2bf0[0x2c28 - 0x2bf0];
    int table_2c28[10];                // +0x2c28
};

struct Msg13_00456310 {
    unsigned char type;                // +0x00
    int start_tick;                    // +0x01
    int sent_tick;                     // +0x05
    int id;                            // +0x09
};

struct Msg26_00456310 {
    unsigned char type;                // +0x00
    int table[10];                     // +0x01
};

struct Msg20_00456310 {
    unsigned char type;                // +0x00
    PlayerInfo_00456310 info;          // +0x01
};
#pragma pack(pop)

class PacketManager {
public:
    int SendAllQueued(int param_1);
};

extern Game* g_game;
extern int g_usePacketManager;
extern PacketManager g_packetManager;

unsigned int FUN_004b6340();
int __stdcall BroadcastPacket(int id, unsigned char* packet, int size);
int __stdcall HAPINET_guaranteepackets(int param_1);
int __stdcall RequestPlayerColor(int param_1);
void FUN_00450530();

// FUNCTION: 0x456310
void SendNetHeartbeat()
{
    unsigned int now = FUN_004b6340();
    if ((int)(now - g_game->field_1b5f) <= 0x3c)
        return;
    g_game->field_1b5f += 0x3c;

    for (int i = 0; i < 10; i++) {
        Player_00456310* p = &g_game->players[i];
        if (p->active != 0 && (p->state == 1 || p->state == 2)) {
            if (g_usePacketManager != 0)
                g_packetManager.SendAllQueued(1);

            Msg13_00456310 msg;
            msg.type = 2;
            msg.start_tick = GetTickCount();
            msg.sent_tick = 0;
            msg.id = p->id;

            int was = HAPINET_guaranteepackets(0);
            BroadcastPacket(p->id, (unsigned char*)&msg, 0xd);
            if (g_usePacketManager != 0)
                g_packetManager.SendAllQueued(1);
            if (was != 0)
                HAPINET_guaranteepackets(1);

            int id = p->id;
            unsigned char* buf = g_game->buffer;
            buf[0] = 6;
            BroadcastPacket(id, buf, 1);

            if (p->info->field_96 == 0xff)
                RequestPlayerColor(0);

            if (p->info->bit_97 & 1) {
                for (int k = 0; k < 10; k++) {
                    unsigned char st = g_game->players[k].state;
                    if (st == 4)
                        g_game->table_2c28[k] = -1;
                    else if (st == 0)
                        g_game->table_2c28[k] = 0;
                    else
                        g_game->table_2c28[k] = g_game->players[k].id;
                }

                Msg26_00456310 msg26;
                memcpy(msg26.table, g_game->table_2c28, 0x28);
                msg26.type = 0x26;
                BroadcastPacket(p->id, (unsigned char*)&msg26, 0x29);
            }
        }
    }

    Msg20_00456310 msg;
    if (g_game->flags_2a44 & 1) {
        for (int i = 0; i < 10; i++) {
            Player_00456310* p = &g_game->players[i];
            if (p->active != 0 && (p->state == 1 || p->state == 2)) {
                msg.info = *p->info;
                msg.info.id = p->id;
                msg.type = 0x20;
                BroadcastPacket(p->id, (unsigned char*)&msg, 0xba);

                if (p->active != 0 && (p->state == 1 || p->state == 2)) {
                    unsigned char* buf2 = g_game->buffer;
                    buf2[0] = 0x24;
                    *(int*)(buf2 + 1) = p->id;
                    buf2[5] = p->field_13f;
                    BroadcastPacket(p->id, buf2, 6);
                    if (g_usePacketManager != 0)
                        g_packetManager.SendAllQueued(1);
                }
            }
        }
        FUN_00450530();
        g_packetManager.SendAllQueued(1);
    }

    g_game->bit0_2bee = 1;
}
