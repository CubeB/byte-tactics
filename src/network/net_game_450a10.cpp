// Decompiled by DeepSeek V4.1 Flash, finished by Space Bunny Free, retried by deepseek-v4.1-flash, retried by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
#include <string.h>
#include <windows.h>

class Mission;

#pragma pack(push, 1)
struct PlayerData_00450a10 {
    char unknown_0[0x90];
    int id;                            // +0x90
    char unknown_94[0xb9 - 0x94];
};

struct Player_00450a10 {
    int active;                        // +0x00
    int id;                            // +0x04
    char unknown_8[0x1c - 8];
    int field_1c;                      // +0x1c
    char unknown_20[0x22 - 0x20];
    unsigned char field_22;            // +0x22
    char unknown_23[0x27 - 0x23];
    PlayerData_00450a10* data;         // +0x27
    char name[0x49 - 0x2b];            // +0x2b
    char fullName[0x73 - 0x49];        // +0x49
    unsigned char type;                // +0x73
    char unknown_74[0x13f - 0x74];
    unsigned char alliance;            // +0x13f
    char unknown_140[0x14b - 0x140];
};

struct Game {
    char unknown_0[0x14];
    char net[0x1b63 - 0x14];           // +0x14
    Player_00450a10 players[10];       // +0x1b63
    char unknown_2851[0x2a38 - 0x2851];
    unsigned char* buffer;             // +0x2a38
    unsigned short count;              // +0x2a3c
    char unknown_2a3e[0x2a44 - 0x2a3e];
    unsigned short flags_2a44;         // +0x2a44
    char unknown_2a46[0x391e9 - 0x2a46];
    Mission* campaign;                 // +0x391e9
};

struct Packet_00450a10 {
    unsigned char type;                // +0x00
    PlayerData_00450a10 data;          // +0x01
};
#pragma pack(pop)

struct DPNAME {
    unsigned long dwSize;
    unsigned long dwFlags;
    char* lpszShortNameA;
    char* lpszLongNameA;
};

class PacketManager {
public:
    int SendAllQueued(int param_1);
};

class Mission {
public:
    int GetGameType();
};

extern Game* g_game;
extern int g_usePacketManager;
extern PacketManager g_packetManager;

int __stdcall HAPINET_getplayername(void* net, unsigned long id, void* data, unsigned long* size);
int __stdcall BroadcastPacket(int player, void* data, int size);
void __stdcall FUN_00464290(unsigned char player, char type);
void FUN_00450530();
int GetTicks();
void __stdcall ReportGameEvent(int param_1);

static inline unsigned char FindSlot_00450a10(int id)
{
    unsigned char i;
    for (i = 0; i < 10; i++) {
        int v;
        if (i == 10) {
            v = -1;
        } else {
            v = g_game->players[i].type ? g_game->players[i].id : -1;
        }
        if (v == id) {
            return i;
        }
    }
    return 10;
}

// FUNCTION: 0x450a10
int __stdcall AddNetPlayer(int param_1)
{
    unsigned char slot;
    if (param_1 == -1) {
        slot = 10;
    } else {
        slot = FindSlot_00450a10(param_1);
    }
    int flag = 0;
    if (slot != 10) {
        if (g_game->players[slot].type != 1 && g_game->players[slot].type != 2) {
            flag = 1;
        }
        if (g_game->players[slot].active != 0) {
            flag |= 1;
        }
    } else {
        int found = 0;
        int i = 0;
        while (i < 10) {
            Player_00450a10* q = &g_game->players[i];
            if (q->active == 0 && q->type != 4) {
                found = 1;
                break;
            }
            i++;
        }
        int s = i;
        if (!found) {
            s = 10;
        }
        slot = s;
        if (slot == 10) {
            flag = 1;
        } else {
            g_game->players[slot].type = 3;
        }
    }
    Player_00450a10* p = &g_game->players[slot];
    if (flag) {
        return 1;
    }
    unsigned long size;
    Packet_00450a10 packet;
    char buf[0x400];
    int result;
    // Destinations precomputed before the branch: keeps the strcpy source scan canonical.
    char* d_full = p->fullName;
    char* d_name = p->name;
    if (param_1 != -1) {
        size = 0x400;
        result = HAPINET_getplayername(g_game->net, param_1, buf, &size);
        if (result == 0) {
            strcpy(d_full, ((DPNAME*)buf)->lpszShortNameA);
            strcpy(d_name, ((DPNAME*)buf)->lpszLongNameA);
        }
    } else {
        strcpy(d_full, "COMPUTER");
        strcpy(d_name, "COMPUTER");
        result = 0;
    }
    if (result) {
        return 1;
    }
    FUN_00464290(slot, g_game->players[slot].type);
    p->field_22 = 0;
    p->id = param_1;
    p->field_1c = GetTicks();
    g_game->count++;
    if (g_game->flags_2a44 & 1) {
        for (int i = 0; i < 10; i++) {
            Player_00450a10* q = &g_game->players[i];
            if (q->active != 0 && (q->type == 1 || q->type == 2)) {
                packet.data = *q->data;
                packet.data.id = q->id;
                packet.type = 0x20;
                BroadcastPacket(q->id, &packet, sizeof(packet));
                if (q->active != 0 && (q->type == 1 || q->type == 2)) {
                    unsigned char* msg = g_game->buffer;
                    msg[0] = 0x24;
                    *(int*)(msg + 1) = q->id;
                    msg[5] = q->alliance;
                    BroadcastPacket(q->id, msg, 6);
                    if (g_usePacketManager != 0) {
                        g_packetManager.SendAllQueued(1);
                    }
                }
            }
        }
        FUN_00450530();
        g_packetManager.SendAllQueued(1);
    }
    if (g_game->campaign->GetGameType() == 3 && g_game->count > 1) {
        ReportGameEvent(2);
    }
    return 1;
}
