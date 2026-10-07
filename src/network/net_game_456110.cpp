// Decompiled by Opus, DeepSeek V4.1 Flash, space-bunny-free, Claude Opus 5.5, Haiku, deepseek-v4.1-flash, deepseek-v4.1, claude-opus-5-5, GPT-6 and GPT-6-Luna. Names are provisional.
// The network game's script-call packets, heartbeat, ping, load progress,
// player counts, resource sharing and lobby connection: the files of the
// module's third part (0x456110 to 0x457d30) gathered in address order.
// The headers are load-bearing: <windows.h> gives 0x457d30's third loop its
// base/index order, <stdio.h> 0x456de0's [ecx + esi] order, <string.h> the
// loop index orders of 0x4572a0, 0x4573d0 and 0x4578f0, and <stdlib.h> the
// first loop's address sums in 0x457540.
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <algorithm>
#include <time.h>

#pragma pack(push, 1)

class CobScript {
public:
    int FindScript(char* name);
};

class MissionConditions {
public:
    int CheckVictory();
};

class PacketManager {
public:
    int SendAllQueued(int param_1);
    void SetDefaultSendPacing(int param);
};

class Class_00456030 {
  public:
    int field_0;
    char unknown_4[0x73 - 0x4];
    char field_73;
    int FUN_00456030();
};

struct Net_4c9f90 {
    char unknown_0[0x4b5];
    int lobby2;                        // +0x4b5 (g_game + 0x4c9)
    int lobby1;                        // +0x4b9 (g_game + 0x4cd)
    char unknown_4bd[0x4c9 - 0x4bd];
};

// The info block a player record points at (+0x27). One type for every view:
// the union members are the names the files give the same bytes.
struct PlayerInfo {
    char unknown_0[0x90];
    int field_90;                      // +0x90
    unsigned char field_94;            // +0x94
    char unknown_95;
    unsigned char field_96;            // +0x96
    union {
        unsigned char flags_97;        // +0x97
        struct {
            unsigned short b0 : 1;
            unsigned short b1 : 1;
            unsigned short b2 : 1;
            unsigned short b3 : 1;
            unsigned short b4 : 1;
            unsigned short b5 : 1;
            unsigned short b6 : 1;
            unsigned short b7 : 1;
            unsigned short b8 : 1;
            unsigned short b9 : 1;
            unsigned short b10 : 1;
            unsigned short b11 : 1;
            unsigned short b12 : 1;
            unsigned short b13 : 1;
            unsigned short b14 : 1;
            unsigned short b15 : 1;
        } bits_97;
    };
    char unknown_99[0x9b - 0x99];
    union {
        unsigned char flags_9b;        // +0x9b
        struct {
            unsigned short : 6;
            unsigned short flag6 : 1;
            unsigned short : 7;
            unsigned short flag14 : 1;
        } bits_9b;
    };
    char unknown_9d[0xb9 - 0x9d];
};

// A player record at g_game+0x1b63 (0x14b bytes). One type for every view:
// where two views name the same bytes differently the union carries both
// names, and where a view reads them as another type the union carries that.
struct Player {
    int active;                        // +0x00
    union {
        int id;                        // +0x04
        int dpid;                      // +0x04
    };
    char unknown_8[0x14 - 8];
    int field_14;                      // +0x14
    char unknown_18[0x20 - 0x18];
    unsigned char progress;            // +0x20
    char unknown_21;
    union {
        unsigned char field_22;        // +0x22
        unsigned char rejectReason;    // +0x22
    };
    char unknown_23[0x27 - 0x23];
    union {
        PlayerInfo* info;              // +0x27
        PlayerInfo* data;              // +0x27
    };
    char unknown_2b[0x73 - 0x2b];
    union {
        unsigned char type;            // +0x73
        char state;                    // +0x73
    };
    char unknown_74[0x8c - 0x74];
    union {
        int field_8c;                  // +0x8c
        float metal;                   // +0x8c
    };
    char unknown_90[0x98 - 0x90];
    union {
        int field_98;                  // +0x98
        float energy;                  // +0x98
    };
    char unknown_9c[0xa4 - 0x9c];
    union {
        int field_a4;                  // +0xa4
        float field_a4f;               // +0xa4
    };
    union {
        int field_a8;                  // +0xa8
        float field_a8f;               // +0xa8
    };
    double field_ac;                   // +0xac
    double field_b4;                   // +0xb4
    double field_bc;                   // +0xbc
    double field_c4;                   // +0xc4
    double field_cc;                   // +0xcc
    double field_d4;                   // +0xd4
    char unknown_dc[0xe4 - 0xdc];
    float field_e4;                    // +0xe4
    float field_e8;                    // +0xe8
    char unknown_ec[0xfc - 0xec];
    short field_fc;                    // +0xfc
    short field_fe;                    // +0xfe
    char unknown_100[0x104 - 0x100];
    short field_104;                   // +0x104
    short field_106;                   // +0x106
    unsigned char field_108[0x16];     // +0x108
    unsigned char t0[11];              // +0x11e
    unsigned char t1[11];              // +0x129
    unsigned char t2[11];              // +0x134
    unsigned char field_13f;           // +0x13f
    int field_140;                     // +0x140
    short field_144;                   // +0x144
    unsigned char field_146;           // +0x146
    unsigned char field_147;           // +0x147
    char unknown_148[0x14b - 0x148];
};

// A unit (0xaa bytes): the player it belongs to and its script names.
struct Unit {
    char unknown_0[0x96];
    Player* player;                    // +0x96
    CobScript* names;                  // +0x9a
    char unknown_9e[0xa8 - 0x9e];
    short field_a8;                    // +0xa8
};

struct Game {
    char unknown_0[0x14];
    Net_4c9f90 net;                    // +0x14
    char unknown_4dd[0x1b5f - 0x4dd];
    int field_1b5f;                    // +0x1b5f
    Player players[10];                // +0x1b63
    char unknown_2851[0x29a4 - 0x2851];
    int field_29a4[10];                // +0x29a4
    char unknown_29cc[0x29d0 - 0x29cc];
    int field_29d0[10];                // +0x29d0
    char unknown_29f8[0x29fc - 0x29f8];
    int field_29fc[10];                // +0x29fc
    char unknown_2a24[0x2a28 - 0x2a24];
    int field_2a28;                    // +0x2a28
    char unknown_2a2c[0x2a38 - 0x2a2c];
    unsigned char* buffer;             // +0x2a38
    short field_2a3c;                  // +0x2a3c
    char unknown_2a3e[0x2a42 - 0x2a3e];
    unsigned char localPlayer;         // +0x2a42
    char unknown_2a43;
    union {
        unsigned char flags_2a44;      // +0x2a44
        struct {
            unsigned char leaving : 1;
            unsigned char rest_2a44 : 7;
        } bits_2a44;
    };
    char unknown_2a45[0x2bee - 0x2a45];
    unsigned short bit0_2bee : 1;      // +0x2bee
    unsigned short rest_2bee : 15;
    char unknown_2bf0[0x2c28 - 0x2bf0];
    int table_2c28[10];                // +0x2c28
    char unknown_2c50[0x38a47 - 0x2c50];
    unsigned int ticks;                // +0x38a47
    char unknown_38a4b[0x38d6f - 0x38a4b];
    // volatile: the byte loads need the explicit widening mask.
    volatile unsigned char stages[6];  // +0x38d6f
    char unknown_38d75[0x391ed - 0x38d75];
    MissionConditions* conditions;     // +0x391ed
    char unknown_391f1[0x3923b - 0x391f1];
    unsigned short bits_3923b : 2;     // +0x3923b
    unsigned short flag2 : 1;
    unsigned short bit3 : 1;
    unsigned short flag4 : 1;
    unsigned short flag5 : 1;
    unsigned short flag6 : 1;
    unsigned short rest_3923b : 9;
};

struct Message_004565a0 {
    char unknown_0[1];
    int start_tick;                    // +0x01
    int sent_tick;                     // +0x05
    int id;                            // +0x09
};

struct Packet_00456110 {
    unsigned char type;                // +0x0
    short id;                          // +0x1
    short index;                       // +0x3
    char field_5;                      // +0x5
    int field_6;                       // +0x6
    int field_a;                       // +0xa
    int field_e;                       // +0xe
    int field_12;                      // +0x12
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
    PlayerInfo info;                   // +0x01
};

struct Packet_00456de0 {
    unsigned char type;                 // +0x0
    unsigned char progress;             // +0x1
};

struct Packet_00456ee0 {
    unsigned char type;                // +0x0
    int subtype;                       // +0x1
    int from;                          // +0x5
    int to;                            // +0x9
    int extra;                         // +0xd
};

struct Packet_00457050 {
    unsigned char type;                // +0x0
    int subtype;                       // +0x1
    int from;                          // +0x5
    int to;                            // +0x9
    int value;                         // +0xd
};

struct Packet_004571c0 {
    unsigned char type;                // +0x0
    int subtype;                       // +0x1
    int from;                          // +0x5
    int to;                            // +0x9
    int zero;                          // +0xd
};

struct Packet_004573d0 {              // 0x3a bytes
    unsigned char type;                // +0x0
    unsigned char flag;                // +0x1
    int field_2;                       // +0x2
    int field_6;                       // +0x6
    int field_a;                       // +0xa
    int field_e;                       // +0xe
    int field_12;                      // +0x12
    int field_16;                      // +0x16
    int field_1a;                      // +0x1a
    int field_1e;                      // +0x1e
    float field_22;                    // +0x22
    float field_26;                    // +0x26
    float field_2a;                    // +0x2a
    float field_2e;                    // +0x2e
    float field_32;                    // +0x32
    float field_36;                    // +0x36
};

struct Packet_00457540 {              // 0x3a bytes
    unsigned char type;                // +0x0
    unsigned char flag;                // +0x1
    short field_2;                     // +0x2
    char unknown_4[0x6 - 0x4];
    short field_6;                     // +0x6
    char unknown_8[0xa - 0x8];
    short field_a;                     // +0xa
    char unknown_c[0xe - 0xc];
    short field_e;                     // +0xe
    char unknown_10[0x12 - 0x10];
    int field_12;                      // +0x12
    int field_16;                      // +0x16
    int field_1a;                      // +0x1a
    int field_1e;                      // +0x1e
    float field_22;                    // +0x22
    float field_26;                    // +0x26
    float field_2a;                    // +0x2a
    float field_2e;                    // +0x2e
    float field_32;                    // +0x32
    float field_36;                    // +0x36
};

struct TeamPacket_00457540 {           // 3 bytes
    unsigned char type;                // +0x0
    unsigned char flag;                // +0x1
    unsigned char flag2;               // +0x2
};

struct Packet_00457d30 {
    unsigned char type;                // +0x0
    int subtype;                       // +0x1
    int from;                          // +0x5
    int to;                            // +0x9
    int zero;                          // +0xd
};

#pragma pack(pop)

extern Game* g_game;
extern int g_usePacketManager;
extern PacketManager g_packetManager;
extern char DAT_00512ca8[];
extern char DAT_00512c98[];

unsigned int GetTicks();
int __stdcall BroadcastPacket(int player, void* data, int size);
int __stdcall SendPacketToPlayer(int from, int to, void* packet, int size);
int __stdcall HAPINET_guaranteepackets(int param_1);
int __stdcall RequestPlayerColor(int param_1);
void FUN_00450530();
void __stdcall SleepMilliseconds(unsigned int param_1);
void __stdcall SendPlayerEconomy(Player* from, Player* to, unsigned char param_3);
void __stdcall TransferEnergy(unsigned char from, unsigned char to, float amount, int flag);
void __stdcall TransferMetal(unsigned char from, unsigned char to, float amount, int flag);
int __stdcall HAPINET_initlobbiedconnection(void* p);
void __stdcall HAPINET_initmultiplaydefaults(void* p);
int __stdcall InitPacketManager(int a, int b);
void __stdcall SetMissionType(int a);
void __stdcall RemovePlayer(int dpid);
void ShutdownScoreTables();
int __stdcall HAPINET_quitgame(Net_4c9f90* net);
void __stdcall SetCloseHandler(void (__cdecl *callback)(int), int param);
void __stdcall QuitApp(char* message);

// FUNCTION: 0x456110
int __stdcall SendScriptCallNoArgsByName(Unit* obj, char* name)
{
    Packet_00456110 packet;
    int index = obj->names->FindScript(name);
    if (!(g_game->flags_2a44 & 1)) {
        return 0;
    }
    packet.type = 0x10;
    packet.id = obj->field_a8;
    packet.index = index;
    packet.field_5 = 0;
    packet.field_6 = 0;
    packet.field_a = 0;
    packet.field_e = 0;
    packet.field_12 = 0;
    return BroadcastPacket(obj->player->id, &packet, 0x16);
}

// FUNCTION: 0x456190
int __stdcall SendScriptCallNoArgs(Unit* obj, short index)
{
    Packet_00456110 packet;
    if (!(g_game->flags_2a44 & 1)) {
        return 0;
    }
    packet.type = 0x10;
    packet.id = obj->field_a8;
    packet.index = index;
    packet.field_5 = 0;
    packet.field_6 = 0;
    packet.field_a = 0;
    packet.field_e = 0;
    packet.field_12 = 0;
    return BroadcastPacket(obj->player->id, &packet, 0x16);
}

// Sends a 0x16-byte type 0x10 packet for the object, like 0x456290, with the
// index looked up by name in the object's name table (+0x9a) first.
// FUNCTION: 0x456200
int __stdcall SendScriptCallByName(Unit* obj, char* name, char field_5,
                           int field_6, int field_a, int field_e, int field_12)
{
    Packet_00456110 packet;
    short index = obj->names->FindScript(name);
    if (!(g_game->flags_2a44 & 1)) {
        return 0;
    }
    packet.type = 0x10;
    packet.id = obj->field_a8;
    packet.index = index;
    packet.field_5 = field_5;
    packet.field_6 = field_6;
    packet.field_a = field_a;
    packet.field_e = field_e;
    packet.field_12 = field_12;
    return BroadcastPacket(obj->player->id, &packet, 0x16);
}

// Sends a 0x16-byte type 0x10 packet for the object (the sibling 0x456110
// sends the same packet with only the index filled in).
// FUNCTION: 0x456290
int __stdcall SendScriptCall(Unit* obj, short index, char field_5,
                           int field_6, int field_a, int field_e, int field_12)
{
    Packet_00456110 packet;
    if (!(g_game->flags_2a44 & 1)) {
        return 0;
    }
    packet.type = 0x10;
    packet.id = obj->field_a8;
    packet.index = index;
    packet.field_5 = field_5;
    packet.field_6 = field_6;
    packet.field_a = field_a;
    packet.field_e = field_e;
    packet.field_12 = field_12;
    return BroadcastPacket(obj->player->id, &packet, 0x16);
}

// Declaring the Msg20 message at function scope (not inside the loop) is what
// puts its address in eax and fixes the whole send block; the trailing
// g_game+0x2bee flag is a 1-bit unsigned short bitfield, which is what makes
// the original emit `or byte ptr [eax+0x2bee], 1` in one instruction.
// FUNCTION: 0x456310
void SendNetHeartbeat()
{
    unsigned int now = GetTicks();
    if ((int)(now - g_game->field_1b5f) <= 0x3c)
        return;
    g_game->field_1b5f += 0x3c;

    for (int i = 0; i < 10; i++) {
        Player* p = &g_game->players[i];
        if (p->active != 0 && (p->type == 1 || p->type == 2)) {
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

            if (p->info->bits_97.b0 & 1) {
                for (int k = 0; k < 10; k++) {
                    unsigned char st = g_game->players[k].type;
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
            Player* p = &g_game->players[i];
            if (p->active != 0 && (p->type == 1 || p->type == 2)) {
                msg.info = *p->info;
                msg.info.field_90 = p->id;
                msg.type = 0x20;
                BroadcastPacket(p->id, (unsigned char*)&msg, 0xba);

                if (p->active != 0 && (p->type == 1 || p->type == 2)) {
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

// Two halves, keyed on the message's own flag at +0x05. When it is clear this
// is the first sighting: stamp the message with the current tick count, send
// it to the player id in the header (the first argument is the local lobby
// dpid from g_game + 0x4cd), and give the packets back to the network layer.
// When it is already set the message is a later update: find the sending
// player in the table, and if they are an active client (state 1 or 2) record
// the round trip into the slot of the player in g_game + 0x4c9.
static inline int GetPlayerId(unsigned char i)
{
    if (i != 10 && g_game->players[i].type)
        return g_game->players[i].id;
    return -1;
}

static inline unsigned char FindPlayerIndex(int id)
{
    // Early return 10 for -1, not a guarded loop.
    if (id == -1)
        return 10;
    for (unsigned char i = 0; i < 10; i++) {
        if (GetPlayerId(i) == id)
            return i;
    }
    return 10;
}

// FUNCTION: 0x4565a0
void __stdcall HandlePing(Message_004565a0* p)
{
    if (p->sent_tick == 0) {                       // not sent yet
        if (g_usePacketManager != 0) {
            g_packetManager.SendAllQueued(1);
        }
        p->sent_tick = GetTickCount();
        int packets_were_guaranteed = HAPINET_guaranteepackets(0);
        SendPacketToPlayer(g_game->net.lobby1, p->id, p, 0xd);
        if (g_usePacketManager != 0) {
            g_packetManager.SendAllQueued(1);
        }
        if (packets_were_guaranteed != 0) {          // hand them back
            HAPINET_guaranteepackets(1);
        }
        return;
    }

    unsigned char index = FindPlayerIndex(p->id);
    // Table lookup goes through the pl pointer local.
    Player* pl = &g_game->players[index];
    if (pl->active == 0) {
        return;
    }
    if (pl->state != 1 && pl->state != 2) {
        return;
    }
    unsigned char other = FindPlayerIndex(g_game->net.lobby2);
    // No bound check on either index: an id of -1 makes FindPlayerIndex
    // return 10, the spare eleventh slot (the table has 11, see docs/bugs.md).
    g_game->players[other].field_14 = GetTickCount() - p->start_tick;
}

// Returns 0 when no player is in state 3. Otherwise every active player of
// type 1 or 2, or in state 3, must have bit 0x20 set in its data flags
// (+0x9b), else it returns 0. Returns 1 when some slot is inactive or lacks
// bit 0x40, unless field_2a3c is 1 (then 0).
static inline int IsPlaying(unsigned char i)
{
    return g_game->players[i].active != 0 && g_game->players[i].type == 3;
}

// The mix of `p->` and `g_game->players[j].` in the second loop is needed:
// it keeps the later reads of active and data from reusing the first loads,
// which frees dl for j and bl for the 0x40 mask as in the original.
// FUNCTION: 0x456760
int FUN_00456760()
{
    int count = 0;
    for (int i = 0; i < 10; i++) {
        Player* p = &g_game->players[i];
        if (p->active != 0 && p->type == 3)
            count++;
    }
    if (count == 0)
        return 0;

    int all = 1;
    for (unsigned char j = 0; j < 10; j++) {
        Player* p = &g_game->players[j];
        if (p->active != 0 && (p->type == 1 || p->type == 2 || IsPlaying(j))) {
            if (!(g_game->players[j].data->flags_9b & 0x20))
                return 0;
        }
        if (g_game->players[j].active == 0 || !(g_game->players[j].data->flags_9b & 0x40))
            all = 0;
    }
    if (g_game->field_2a3c == 1)
        return 0;
    return all == 0;
}

// FUNCTION: 0x456850
unsigned char FindHostSlot()
{
    unsigned char i;
    for (i = 0; i < 10; i++) {
        if (g_game->players[i].type != 0 && (g_game->players[i].data->flags_97 & 1))
            return i;
    }
    return 10;
}

// FUNCTION: 0x4568b0
void __stdcall ReportPacketGap(int, int, int)
{
}

static inline unsigned char FindOccupied_004568c0() {
    for (unsigned char i = 0; i < 10; i++) {
        if (g_game->players[i].type != 0 && g_game->players[i].info->bits_97.b0)
            return i;
    }
    return 10;
}

static inline int FirstJoinedId_004568c0() {
    for (int j = 0; j < 10; j++) {
        if (g_game->players[j].type == 1)
            return g_game->players[j].id;
    }
    return -1;
}

static inline int IsConnected_004568c0(Player* p) {
    return p->active != 0 && (p->type == 1 || p->type == 2);
}

static inline int PlayerId_004568c0(unsigned char pi) {
    // Separate pi == 10 test, then a widened copy; no Player* local, no combined test.
    if (pi == 10)
        return -1;
    int i = pi;
    if (g_game->players[i].type != 0)
        return g_game->players[i].id;
    return -1;
}

// FUNCTION: 0x4568c0
int FUN_004568c0() {
    unsigned char idx = FindOccupied_004568c0();
    int res = ((Class_00456030*)&g_game->players[idx])->FUN_00456030();
    if (res != 0 && g_game->field_2a28 == 0) {
        int* out = g_game->field_29fc;
        if (g_game->players[g_game->localPlayer].info->bits_9b.flag14) {
            int n = 0;
            for (int k0 = 0; k0 < 10; k0++) {
                Player* q = &g_game->players[k0];
                if (q->active != 0 && (q->type == 1 || q->type == 2 || q->type == 3) &&
                    q->field_146 != 10 && (q->info->bits_9b.flag6) == 0)
                    out[k0] = n++;
                else
                    out[k0] = -1;
            }
        } else {
            int cand[10];
            for (int z = 0; z < 10; z++)
                cand[z] = -1;
            int n = 0;
            for (int k1 = 0; k1 < 10; k1++) {
                Player* p = &g_game->players[k1];
                if (p->active != 0 && (p->type == 1 || p->type == 2 || p->type == 3) &&
                    p->field_146 != 10 && (p->info->bits_9b.flag6) == 0) {
                    cand[n] = n;
                    n++;
                }
            }
            if (n > 2 || (int)((__int64)rand() * 2 / 0x8000) != 0)
                std::random_shuffle(cand, cand + n);
            int j = 0;
            for (int k2 = 0; k2 < 10; k2++) {
                Player* q2 = &g_game->players[k2];
                if (q2->active != 0 && (q2->type == 1 || q2->type == 2 || q2->type == 3) &&
                    q2->field_146 != 10) {
                    if (g_game->players[k2].active != 0 && (q2->info->bits_9b.flag6))
                        out[k2] = -1;
                    else
                        // Candidates read by index, cand[j++].
                        out[k2] = cand[j++];
                } else {
                    out[k2] = -1;
                }
            }
        }
        g_game->field_2a28 = 1;
    }
    int ok = 1;
    for (int k3 = 0; k3 < 10; k3++) {
        Player* q = &g_game->players[k3];
        if (q->active != 0 && q->type == 3) {
            if (res != 0) {
                if (g_game->field_29a4[k3] == 0 || g_game->field_29d0[k3] == 0) {
                    ok = 0;
                    break;
                }
            }
            if (res == 0) {
                if (g_game->field_29a4[k3] == 0) {
                    ok = 0;
                    break;
                }
            }
        }
    }
    if (res != 0) {
        for (int k4 = 0; k4 < 10; k4++) {
            // Skip with continue; the send stays a plain call.
            if (g_game->field_29d0[k4] != 0)
                continue;
            unsigned char packet[2];
            packet[0] = 0x1e;
            packet[1] = (unsigned char)g_game->field_29fc[k4];
            if (g_game->players[k4].active != 0) {
                if (g_game->players[k4].type == 3) {
                    SendPacketToPlayer(FirstJoinedId_004568c0(), PlayerId_004568c0(k4), packet, 2);
                } else if (IsConnected_004568c0(&g_game->players[k4])) {
                    g_game->players[k4].field_147 = packet[1];
                    g_game->field_29d0[k4] = 1;
                }
            }
        }
    }
    unsigned char pkt = 0x15;
    // Tail shape: if (res == 0) {...} else if (ok) {...}, then return ok.
    if (res == 0) {
        for (int k6 = 0; k6 < 10; k6++) {
            if (IsConnected_004568c0(&g_game->players[k6]))
                BroadcastPacket(PlayerId_004568c0(k6), &pkt, 1);
        }
    } else if (ok) {
        for (int k5 = 0; k5 < 10; k5++) {
            if (IsConnected_004568c0(&g_game->players[k5]))
                BroadcastPacket(PlayerId_004568c0(k5), &pkt, 1);
        }
    }
    return ok;
}

static inline int PlayerId(unsigned char i)
{
    if (i != 10 && g_game->players[i].type)
        return g_game->players[i].id;
    return -1;
}

// Sends the average of the six load-stage percentages (g_game+0x38d6f..74)
// as a two-byte packet (type 0x2a) to every active player of type 1 or 2,
// and stores it in that player's +0x20.
// FUNCTION: 0x456de0
void __stdcall SendLoadProgress()
{
    Packet_00456de0 packet;
    // The type store must be the first statement, ahead of the prologue stores.
    packet.type = 0x2a;
    // Own int total, summed as a += chain in this order: sets the load order.
    int total = g_game->stages[0];
    total += g_game->stages[5];
    total += g_game->stages[4];
    total += g_game->stages[3];
    total += g_game->stages[2];
    total += g_game->stages[1];

    packet.progress = total / 6;
    for (int i = 0; i < 10; i++) {
        if (g_game->players[i].active != 0
            && (g_game->players[i].type == 1 || g_game->players[i].type == 2)) {
            g_game->players[i].progress = packet.progress;
            BroadcastPacket(PlayerId(i), &packet, 2);
        }
    }
}

// Sends a 17-byte packet (type 0x16, subtype 1) from player `from` to player
// `to`, carrying both players' DirectPlay ids plus a dword argument. Slot 10
// means "no player".
//
// Same player layout and packet as 0x4571c0 (type 0x16, subtype 3); see
// 0x4515d0 for the PlayerDpid helper spelling.
static inline int PlayerDpid(unsigned char i)
{
    if (i != 10 && g_game->players[i].type != 0)
        return g_game->players[i].dpid;
    return -1;
}

static inline int PlayerReady(Player* p)
{
    return p->active != 0
        && (p->type == 1 || p->type == 2 || p->type == 3)
        && p->field_146 != 10
        && (p->field_144 != 0 || p->field_140 == 0);
}

// FUNCTION: 0x456ee0
void __stdcall SendShareMetal(unsigned char from, unsigned char to, int param_3)
{
    Player* a = &g_game->players[from];
    Player* b = &g_game->players[to];

    if (PlayerReady(a) && PlayerReady(b)) {
        Packet_00456ee0 packet;
        packet.type = 0x16;
        packet.subtype = 1;
        packet.from = PlayerDpid(from);
        packet.to = PlayerDpid(to);
        packet.extra = param_3;
        SendPacketToPlayer(PlayerDpid(from), PlayerDpid(to), &packet, sizeof(packet));
    }
}

// Sends a 17-byte packet (type 0x16, subtype 2) from player `from` to player
// `to` carrying both players' DirectPlay ids plus a dword, but only when both
// slots hold a real player of type 1, 2 or 3 (compare 0x4571c0, which is the
// same packet with subtype 3 and a trailing zero).
// FUNCTION: 0x457050
void __stdcall SendShareEnergy(unsigned char from, unsigned char to, int value)
{
    Player* first = &g_game->players[from];
    Player* second = &g_game->players[to];
    if (first->active != 0
        && (first->type == 1 || first->type == 2 || first->type == 3)
        && first->field_146 != 10
        && (first->field_144 != 0 || first->field_140 == 0)
        && second->active != 0
        && (second->type == 1 || second->type == 2 || second->type == 3)
        && second->field_146 != 10
        && (second->field_144 != 0 || second->field_140 == 0)) {
        Packet_00457050 packet;
        packet.type = 0x16;
        packet.subtype = 2;
        packet.from = PlayerDpid(from);
        packet.to = PlayerDpid(to);
        packet.value = value;
        SendPacketToPlayer(PlayerDpid(from), PlayerDpid(to), &packet, sizeof(packet));
    }
}

// Sends a 17-byte packet (type 0x16, subtype 3) from player `from` to
// player `to`, carrying both players' DirectPlay ids. Slot 10 means "no
// player".
static inline int PlayerDpid_00457d30(unsigned char i)
{
    if (g_game->players[i].type != 0)
        return g_game->players[i].dpid;
    return -1;
}

// FUNCTION: 0x4571c0
void __stdcall SendShareMapInfo(unsigned char from, unsigned char to)
{
    if (from != 10 && to != 10) {
        Packet_004571c0 packet;
        packet.type = 0x16;
        packet.subtype = 3;
        packet.from = PlayerDpid_00457d30(from);
        packet.to = PlayerDpid_00457d30(to);
        packet.zero = 0;
        SendPacketToPlayer(PlayerDpid_00457d30(from), PlayerDpid_00457d30(to), &packet, sizeof(packet));
    }
}

// Scans the ten player slots twice. The outer pass picks every slot that looks
// like a local player (active, type 1 or 2, field_140 set, field_22 clear); the
// inner pass then looks for a network slot (active, type 3) whose data->field_94
// is 1 and whose team (field_146) is still clear in the target's three per-team
// byte tables, and hands the pair to SendPlayerEconomy, returning 0 in that case.
// The three tables live at +0x11e, +0x129 and +0x134, eleven bytes each.
// FUNCTION: 0x4572a0
int FUN_004572a0()
{
    int result = 1;
    for (int i = 0; i < 10; i++) {
        Player* pi = &g_game->players[i];
        if (pi->active == 0)
            continue;
        if (pi->type != 1 && pi->type != 2)
            continue;
        if (pi->field_140 == 0)
            continue;
        if (pi->field_22 != 0)
            continue;
        for (int j = 0; j < 10; j++) {
            Player* pj = &g_game->players[j];
            if (pj->active != 0 && pj->type == 3 || pj->field_140 == 0
                || pj->field_22 != 0) {
                // Two sibling ifs, each with its own call: the jump layout and
                // registers follow the original.
                if (pj->active != 0 && pj->type == 3) {
                    if (pj->data->field_94 == 1
                        && (pi->t0[pj->field_146] == 0
                            || pi->t2[pj->field_146] == 0
                            || pi->t1[pj->field_146] == 0))
                        goto send;
                }
                if (pj->active != 0 && pj->type == 3) {
                    if (pi->t1[pj->field_146] == 0) {
                        SendPlayerEconomy(pi, pj, 1);
                        result = 0;
                    }
                }
                continue;
            send:
                SendPlayerEconomy(pi, pj, 1);
                result = 0;
            }
        }
    }
    SleepMilliseconds(0xfa);
    return result;
}

// Sends a 0x3a-byte packet (type 0x28) carrying a player's four short angles
// (+0xfc, +0xfe, +0x104, +0x106), four dwords (+0x98, +0x8c, +0xa8, +0xa4) and
// six doubles (+0xac, +0xbc, +0xcc, +0xb4, +0xc4, +0xd4) as floats, plus the
// caller's flag byte. It goes to `target` when there is one, otherwise to
// every occupied player slot of type 3 whose data->field_94 is 1.
// FUNCTION: 0x4573d0
void __stdcall SendPlayerEconomy(Player* player, Player* target,
                            unsigned char flag)
{
    if (player->active == 0)
        return;
    if (player->type != 1 && player->type != 2)
        return;
    if (player->field_22 != 0)
        return;

    Packet_004573d0 packet;
    packet.type = 0x28;
    packet.flag = flag;
    packet.field_2 = player->field_fc;
    packet.field_6 = player->field_fe;
    packet.field_a = player->field_104;
    packet.field_e = player->field_106;
    packet.field_12 = player->field_98;
    packet.field_16 = player->field_8c;
    packet.field_1a = player->field_a8;
    packet.field_1e = player->field_a4;
    packet.field_22 = (float)player->field_ac;
    packet.field_26 = (float)player->field_bc;
    packet.field_2a = (float)player->field_cc;
    packet.field_2e = (float)player->field_b4;
    packet.field_32 = (float)player->field_c4;
    packet.field_36 = (float)player->field_d4;

    if (target != 0) {
        if (target->field_22 == 0)
            SendPacketToPlayer(player->dpid, target->dpid, &packet, 0x3a);
        return;
    }

    for (int i = 0; i < 10; i++) {
        if (g_game->players[i].active == 0)
            continue;
        if (g_game->players[i].type != 3)
            continue;
        if (g_game->players[i].data->field_94 != 1)
            continue;
        if (g_game->players[i].field_22 != 0)
            continue;
        SendPacketToPlayer(player->dpid, g_game->players[i].dpid, &packet, 0x3a);
    }
}

// A received 0x3a-byte team packet (the same shape 0x4573d0 sends, the four
// angles as shorts in the packet dwords, the six doubles as floats) updates one
// player record, and then, when the packet's flag byte is set, tells the other
// local players about that player's team.
//
// The first pass looks for a local player (active, type 1 or 2) that already
// has the target player's team in its +0x129 byte table. If there is none, the
// packet's fields are copied into the player; the +0x146 byte is that player's
// team number, the two tables at +0x11e and +0x129 are eleven bytes each, one
// entry per team.
// FUNCTION: 0x457540
void __stdcall HandlePlayerEconomy(Packet_00457540* packet, Player* player)
{
    if (player == 0)
        return;
    if (player->field_22 != 0)
        return;

    int found = 0;
    for (int i = 0; i < 10; i++) {
        if (g_game->players[i].active != 0
            && (g_game->players[i].type == 1 || g_game->players[i].type == 2)
            && g_game->players[i].t1[player->field_146] != 0)
            found = 1;
    }

    if (found == 0) {
        player->field_fc = packet->field_2;
        player->field_fe = packet->field_6;
        player->field_104 = packet->field_a;
        player->field_106 = packet->field_e;
        player->field_98 = packet->field_12;
        player->field_8c = packet->field_16;
        player->field_a8 = packet->field_1a;
        player->field_a4 = packet->field_1e;
        player->field_ac = packet->field_22;
        player->field_bc = packet->field_26;
        player->field_cc = packet->field_2a;
        player->field_b4 = packet->field_2e;
        player->field_c4 = packet->field_32;
        player->field_d4 = packet->field_36;
    }

    if (packet->flag == 0)
        return;

    TeamPacket_00457540 team;
    team.type = 0x29;
    team.flag = 1;

    {
    for (int i = 0; i < 10; i++) {
        Player* p = &g_game->players[i];
        if (p->active == 0)
            continue;
        if (p->type != 1 && p->type != 2)
            continue;
        if (p->field_22 != 0)
            continue;
        p->t1[player->field_146] = 1;
        // Must be an if/else of constant stores, not a bool expression, which
        // adds a 32 bit temporary.
        if (p->t0[player->field_146] != 0)
            team.flag2 = 1;
        else
            team.flag2 = 0;
        SendPacketToPlayer(p->dpid, player->dpid, &team, 3);
        if (g_game->conditions->CheckVictory() != 0)
            continue;
        if (p->t0[player->field_146] != 0)
            continue;
        SendPlayerEconomy(p, player, 1);
    }
    }
}

// FUNCTION: 0x457710
int InitLobbiedConnection()
{
    if (*(int*)((char*)g_game + 0x4e5) != 0)
        return 1;

    if (HAPINET_initlobbiedconnection((char*)g_game + 0x14)) {
        HAPINET_initmultiplaydefaults((char*)g_game + 0x14);
        *(int*)((char*)g_game + 0x4f1) = 10;
        if (InitPacketManager(2, 100)) {
            SetMissionType(3);
            char* name = *(char**)(*(char**)(*(char**)((char*)g_game + 0x4e5) + 8) + 0x30);
            if (name != 0) {
                strncpy((char*)g_game + 0x14, name, 0x10);
                return 1;
            }

            char username[16];
            username[0] = 0;
            if (DAT_00512ca8[0] != 0) {
                strncat(username, DAT_00512ca8, 0x10);
            } else if (DAT_00512c98[0] != 0) {
                strncat(username, DAT_00512c98, 0x10);
            } else {
                DWORD size = 0xf;
                if (GetUserNameA(username, &size))
                    username[size] = 0;
                else
                    strcpy(username, "TotalA");
            }

            time_t now = time(0);
            struct tm* local = localtime(&now);
            int len = strlen(username);
            if (15 - len >= 7)
                sprintf(username + len, "_%02d%02d%02d", local->tm_hour, local->tm_min, local->tm_sec);
            strcpy((char*)g_game + 0x14, username);
            return 1;
        }
    }
    return 0;
}

// FUNCTION: 0x4578d0
void __stdcall SetPacketRate(int param_1)
{
    g_packetManager.SetDefaultSendPacing(param_1);
}

// Leaves the game: if the "leaving game" flag at +0x2a44 is set, drops every
// connected human or computer player (type 1 or 2) with RemovePlayer and
// calls ShutdownScoreTables. Then closes the network, installs a null callback, sets
// the game flag at +0x3923b and quits with the text for the local player's
// rejection reason (+0x22), the same text GetRejectReasonText returns, and no text at
// all when the reason is 0.
// FUNCTION: 0x4578f0
void __cdecl LeaveNetGameCallback(int)
{
    if (g_game->bits_2a44.leaving) {
        for (int i = 0; i < 10; i++) {
            if (g_game->players[i].active != 0
                && (g_game->players[i].type == 1 || g_game->players[i].type == 2))
                RemovePlayer(g_game->players[i].dpid);
        }
        ShutdownScoreTables();
    }
    HAPINET_quitgame(&g_game->net);
    SetCloseHandler(0, 0);
    g_game->flag2 = 1;
    int reason = g_game->players[g_game->localPlayer].rejectReason;
    char* text;
    if (reason) {
        switch (reason) {
        case 4:
            text = "You did not have the correct password";
            break;
        case 3:
            text = "The game is closed";
            break;
        case 5:
            text = "The game is full";
            break;
        case 6:
            text = "You have lost connection with the game";
            break;
        case 7:
            text = "You need a unit you don't have for this game";
            break;
        case 8:
            text = "You need a newer version of the game to enter";
            break;
        case 9:
            text = "No watching is allowed for this game";
            break;
        case 10:
            text = "The creator has left the game";
            break;
        default:
            text = "You were rejected from the game";
            break;
        }
    }
    else {
        text = 0;
    }
    QuitApp(text);
}

// Finds the first player (of 10) with a type set and bit 0 of its info flags
// (index 10 if none), then returns 1 if that player slot is active with type
// 1 or 2. Types and layout as in 0x457af0.
static inline unsigned char FindPlayer()
{
    for (unsigned char i = 0; i < 10; i++) {
        if (g_game->players[i].type != 0 && (g_game->players[i].info->flags_97 & 1))
            return i;
    }
    return 10;
}

static inline int IsHuman(Player* p)
{
    if (p->active != 0 && (p->type == 1 || p->type == 2))
        return 1;
    return 0;
}

// FUNCTION: 0x457a50
int IsHostLocal()
{
    return IsHuman(&g_game->players[FindPlayer()]);
}

// FUNCTION: 0x457af0
int CountHumanPlayers()
{
    int count = 0;
    for (int i = 0; i < 10; i++) {
        if ((g_game->players[i].active != 0 && g_game->players[i].type == 1)
            || (g_game->players[i].active != 0 && g_game->players[i].type == 3
                && g_game->players[i].data->field_94 == 1))
            count++;
    }
    return count;
}

// FUNCTION: 0x457b40
int CountComputerPlayers()
{
    int count = 0;
    for (int i = 0; i < 10; i++) {
        if ((g_game->players[i].active != 0 && g_game->players[i].type == 2)
            || (g_game->players[i].active != 0 && g_game->players[i].type == 3
                && g_game->players[i].data->field_94 == 2))
            count++;
    }
    return count;
}

// Counts the active players of type 2 (compare 0x457b40). The pointer local
// per iteration keeps the walk at the start of each entry; indexing twice
// makes MSVC walk the type field instead.
// FUNCTION: 0x457b90
int CountLocalComputerPlayers()
{
    int count = 0;
    for (int i = 0; i < 10; i++) {
        Player* p = &g_game->players[i];
        if (p->active != 0 && p->type == 2)
            count++;
    }
    return count;
}

// Counts active players of type 2 whose field_146 is not 10 (compare
// 0x457c10).
// FUNCTION: 0x457bc0
int FUN_00457bc0()
{
    int count = 0;
    for (int i = 0; i < 10; i++) {
        if (g_game->players[i].active != 0
            && g_game->players[i].type == 2
            && g_game->players[i].field_146 != 10
            && (g_game->players[i].field_144 != 0 || g_game->players[i].field_140 == 0))
            count++;
    }
    return count;
}

// FUNCTION: 0x457c10
int FUN_00457c10()
{
    int count = 0;
    for (int i = 0; i < 10; i++) {
        if (g_game->players[i].active != 0
            && (g_game->players[i].type == 1 || g_game->players[i].type == 2)
            && (g_game->players[i].type == 1 || g_game->players[i].type == 2 || g_game->players[i].type == 3)
            && g_game->players[i].field_146 != 10
            && (g_game->players[i].field_144 != 0 || g_game->players[i].field_140 == 0))
            count++;
    }
    return count;
}

// Counts the active players of type 3 (compare 0x457b90, type 2).
// FUNCTION: 0x457c80
int CountRemotePlayers()
{
    int count = 0;
    for (int i = 0; i < 10; i++) {
        Player* p = &g_game->players[i];
        if (p->active != 0 && p->type == 3)
            count++;
    }
    return count;
}

// FUNCTION: 0x457cb0
int FUN_00457cb0()
{
    int count = 0;
    for (int i = 0; i < 10; i++) {
        if (g_game->players[i].active != 0
            && (g_game->players[i].type == 1 || g_game->players[i].type == 2 || g_game->players[i].type == 3)
            && g_game->players[i].field_146 != 10
            && (g_game->players[i].field_144 != 0 || g_game->players[i].field_140 == 0)
            && (g_game->players[i].type == 1
                || (g_game->players[i].type == 3 && g_game->players[i].data->field_94 == 1))
            && !(g_game->players[i].data->flags_9b & 0x40))
            count++;
    }
    return count;
}

// Shared-resource tick: every 60 game ticks, if the local player is over its
// energy (or metal) reserve and has the matching flag, give up to a third (a
// half for metal) of the surplus to the neediest ally that shows up in the
// per-player array. Every 450 ticks it also sends the subtype-3 share packet
// (the same one 0x4571c0 sends) to every allied player.
// FUNCTION: 0x457d30
void __stdcall UpdateResourceSharing(Player* player)
{
    if ((g_game->flags_2a44 & 1) == 0)
        return;

    if (g_game->ticks % 60 == 0) {
        Player* found = player;
        if (player->info->bits_97.b1 && player->energy > player->field_e4) {
            for (int i = 0; i < 10; i++) {
                Player* p = &g_game->players[i];
                if (p->active != 0
                    && (p->type == 1 || p->type == 2 || p->type == 3)
                    && p->field_146 != 10
                    && (p->type == 1 || p->type == 2 || p->type == 3)
                    && (p->field_144 != 0 || p->field_140 == 0)
                    && p->type == 3
                    && p->info->field_94 == 1
                    && player->field_108[i] != 0
                    && p->energy < player->energy)
                    found = p;
            }
        }
        if (found != player && player->energy > player->field_e4) {
            float amount = (player->energy - player->field_e4) * 0.33333334f;
            float limit = found->field_a8f - found->energy;
            // The min() must be a ternary into a fresh local, not an if.
            float result = amount < limit ? amount : limit;
            amount = result;
            TransferEnergy(player->field_146, found->field_146, amount, 1);
        }

        found = player;
        if (player->info->bits_97.b2 && player->metal > player->field_e8) {
            for (int i = 0; i < 10; i++) {
                Player* p = &g_game->players[i];
                if (p->active != 0
                    && (p->type == 1 || p->type == 2 || p->type == 3)
                    && p->field_146 != 10
                    && (p->type == 1 || p->type == 2 || p->type == 3)
                    && (p->field_144 != 0 || p->field_140 == 0)
                    && p->type == 3
                    && p->info->field_94 == 1
                    && player->field_108[i] != 0
                    && p->metal < player->metal)
                    found = p;
            }
        }
        if (found != player && player->metal > player->field_e8) {
            float amount = (player->metal - player->field_e8) * 0.5f;
            float limit = found->field_a4f - found->metal;
            // The min() must be a ternary into a fresh local, not an if.
            float result = amount < limit ? amount : limit;
            amount = result;
            TransferMetal(player->field_146, found->field_146, amount, 1);
        }
    }

    if (g_game->ticks % 450 == 0) {
        // Must stay a standalone bitfield test, not a mask or shift.
        if (player->info->bits_97.b5) {
        for (int i = 0; i < 10; i++) {
            Player* p = &g_game->players[i];
            if (p->active != 0
                && (p->type == 1 || p->type == 2 || p->type == 3)
                && p->field_146 != 10
                && (p->type == 1 || p->type == 2 || p->type == 3)
                && (p->field_144 != 0 || p->field_140 == 0)
                && p->type == 3
                && p->info->field_94 == 1
                && player->field_108[i] != 0) {
                unsigned char a = player->field_146;
                unsigned char b = p->field_146;
                if (a != 10 && b != 10) {
                    Packet_00457d30 packet;
                    packet.type = 0x16;
                    packet.subtype = 3;
                    packet.from = PlayerDpid_00457d30(a);
                    packet.to = PlayerDpid_00457d30(b);
                    packet.zero = 0;
                    SendPacketToPlayer(PlayerDpid_00457d30(a), PlayerDpid_00457d30(b),
                                 &packet, sizeof(packet));
                }
            }
        }
    }
    }
}
