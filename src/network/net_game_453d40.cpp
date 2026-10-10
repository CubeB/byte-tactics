// Decompiled by deepseek-v4.1-flash, finished by claude-opus-5-5, finished by DeepSeek V4.1 Flash,
// checked by GPT-6, finished by claude-opus-5-5. Names are provisional.
// Headers and declaration count are load-bearing: windows.h sets the base/index
// order of address operands, memory.h the form of the case 35 allies store.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#pragma pack(push, 1)

#include "player_info.h"

class Player {
public:
    int active;                        // +0x00
    int id;                            // +0x04
    int joinTime;                      // +0x08
    int lobbyDataSynced;               // +0x0c
    int messages;                      // +0x10
    char unknown_14[8];
    int last_time;                     // +0x1c
    unsigned char progress;            // +0x20
    unsigned char flags_21;            // +0x21
    char unknown_22[5];
    PlayerInfo* info;                  // +0x27
    char name[0x1e];                   // +0x2b
    char fullName[0x1e];               // +0x49
    char unknown_67[0x73 - 0x67];
    unsigned char state;               // +0x73
    char unknown_74[0x108 - 0x74];
    unsigned char allies[0x16];        // +0x108
    unsigned char shareLos[11];        // +0x11e
    unsigned char shareVision[11];     // +0x129
    unsigned char shareMapping[11];    // +0x134
    unsigned char alliance;            // +0x13f
    char unknown_140[6];
    unsigned char index;               // +0x146
    unsigned char startPos;            // +0x147
    char unknown_148[3];
    void SetType(int);
    int IsPlayableSlot();
    Player();
    void FreeSideDataAndFogSightCounts();
};

struct WeaponDef {
    char data[0x115];
};

#include "../units/cob_script.h"

class Unit {
public:
    char unknown_0[0x9a];
    CobScript* script;               // +0x9a
    char unknown_9e[0x110 - 0x9e];
    unsigned int flags;                // +0x110
    char unknown_114[4];
    void SetStateBits(unsigned char, int);
};

class UnitSync {
public:
    void ReceiveSyncPacket(void*, unsigned char);
};

#include "settings.h"

struct Game {
    char unknown_0[0x14];
    char session[0x471 - 0x14];        // +0x14
    Settings settings;                 // +0x471
    char unknown_4c1[0x4c9 - 0x4c1];
    int from_id;                       // +0x4c9
    int local_id;                      // +0x4cd
    char unknown_4d1[0x1b63 - 0x4d1];
    Player players[10];                // +0x1b63
    char unknown_2851[0x29a4 - 0x2851];
    int shareVisionReady[11];          // +0x29a4
    int startPosAssignAck[11];         // +0x29d0
    char unknown_29fc[0x2a30 - 0x29fc];
    UnitSync* sync;                    // +0x2a30
    char unknown_2a34[4];
    unsigned char* recvPacketPtr;      // +0x2a38
    char unknown_2a3c[6];
    unsigned char localPlayer;         // +0x2a42
    char playerIndex;
    unsigned char flags_2a44;          // +0x2a44
    char unknown_2a45[0x2be3 - 0x2a45];
    char password[0x2bee - 0x2be3];    // +0x2be3
    unsigned short dirty : 1;          // +0x2bee
    unsigned short : 15;
    char unknown_2bf0[0x2c28 - 0x2bf0];
    int playerIds[11];                 // +0x2c28
    char unknown_2c54[0x2cf3 - 0x2c54];
    WeaponDef weaponDefs[256];         // +0x2cf3
    char unknown_after_weapons[0x14357 - (0x2cf3 + 0x115 * 256)];
    Unit* units;                       // +0x14357
    char unknown_1435b[0x38a51 - 0x1435b];
    unsigned short bit_38a51 : 1;      // +0x38a51
    unsigned short : 15;
    char unknown_38a53[0x38d75 - 0x38a53];
    union {
        volatile unsigned short net_flags; // +0x38d75
        struct {
            unsigned short : 2;
            unsigned short net_bit2 : 1;
            unsigned short : 13;
        } net_bits;
    };
    char unknown_38d77[0x391f1 - 0x38d77];
    int frontendState;                 // +0x391f1
    char unknown_391f5[0x3923b - 0x391f5];
    unsigned short : 2;                // +0x3923b
    unsigned short bit2_3923b : 1;
    unsigned short : 1;
    unsigned short bit4_3923b : 1;
    unsigned short : 11;
};

// The DirectPlay system message HandleNetPackets reads when the sender is 0:
// +0x00 is its DPSYS_ type (3 create, 5 destroy, 0x102 set data, 0x103 set
// name, 0x104 set session description). The fields are named for the
// create-player layout and the set-data layout, which overlay: a create has
// its data at createData/dataSize, a set-data has it at data, a set-name has
// its short name at +0x14 and the long name at longName, and a set-session
// description starts at +0x04.
struct Packet_00453d40 {
    unsigned int type;                 // +0x00
    int playerType;                    // +0x04, 1 is a player
    int id;                            // +0x08
    char* data;                        // +0x0c
    char* createData;                  // +0x10
    int dataSize;                      // +0x14
    char* longName;                    // +0x18
};

// The player-info broadcast packet (type 0x20): a type byte then the sender's
// PlayerInfo, whose id sits at +0x91. net_game.cpp has the same record.
struct SideDataPacket {
    unsigned char type;                // +0x0
    PlayerInfo data;                   // +0x1
};

// The packets whose payload is a player id dword at +0x1: reject (0x1b), drop
// (0x1c), integrity (0x1d), start-position ack (0x1f), team assignment (0x22),
// alliance (0x24) and breach (0x27) all start with it, and nothing reads a byte
// before it. The integrity packet's second dword follows at +0x5.
struct PlayerIdPacket {
    unsigned char type;                // +0x0
    int id;                            // +0x1
    int field_5;                       // +0x5
};

// A feature's damage (0x0f): the sub-command (a weapon index or 0xfd to
// 0xff), then the cell. map/features.cpp and orders/unit_orders.cpp name it;
// the fields are unsigned here because this file loads them zero-extended.
struct FeatureDamagePacket {
    unsigned char type;                // +0x0
    unsigned char sub;                 // +0x1
    unsigned short x;                  // +0x2
    unsigned short z;                  // +0x4
};

// A unit script call (0x10): the unit, the script index, the argument count
// and four arguments. net_game.cpp has the same record, but this file's unit id
// and argument count are unsigned (zero-extended loads).
struct UnitScriptCallPacket {
    unsigned char type;                // +0x0
    unsigned short id;                 // +0x1
    short index;                       // +0x3
    unsigned char argCount;            // +0x5
    int arg0;                          // +0x6
    int arg1;                          // +0xa
    int arg2;                          // +0xe
    int arg3;                          // +0x12
};

// A builder link (0x12): the constructed unit and the builder. net_game.cpp
// has the same record.
struct BuilderLinkPacket {
    unsigned char type;                // +0x0
    short constructedUnitId;           // +0x1
    short builderUnitId;               // +0x3
};

// A sound (0x13): the positional flag, the sound index and the sound's
// position. sound/sound_47ed40.cpp names the same record.
struct Packet_0047f0c0 {
    unsigned char type;                // +0x0
    unsigned char flag;                // +0x1
    int index;                         // +0x2
    int pos[3];                        // +0x6, x, y and z in 16.16
};

// A unit ownership transfer (0x14): the unit, the new owner and the rest of
// the unit's state. units/unit_commands.cpp names the same record.
struct OwnershipTransferPacket {
    unsigned char type;                // +0x0
    short unitId;                      // +0x1
    int newOwnerNetId;                 // +0x3
    char unknown_7[0x18 - 7];          // +0x7
};

// A metal, energy or map share (0x16): the subtype, the two players and the
// amount. net_game.cpp has the same record.
struct ResourceSharePacket {
    unsigned char type;                // +0x0
    int subtype;                       // +0x1
    int fromNetId;                     // +0x5
    int toNetId;                       // +0x9
    int amount;                        // +0xd
};

// A lobby sync request (0x21): the flag, the player asked about and the
// caller. net_game_450530.cpp names the same record.
struct Msg_00450530 {
    unsigned char type;                // +0x0
    unsigned char flag;                // +0x1
    int id;                            // +0x2
    int arg;                           // +0x6
};

// An alliance flag change (0x23): the two players, the flag and the mode.
// net_game.cpp and net_game_452960.cpp name the same record.
struct AllyFlagsPacket {
    unsigned char type;                // +0x0
    int fromNetId;                     // +0x1
    int toNetId;                       // +0x5
    char allied;                       // +0x9
    int force;                         // +0xa
};

#pragma pack(pop)

class PacketManager {
public:
    void SendAllQueued(int);
    void HandleIntegrityNop(int, int, int);
};

// Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
int __stdcall CountLocalComputerPlayers();
int __stdcall CountActiveAIPlayers();
int __stdcall CountActiveHumanOrAiPlayers();
int __stdcall CountRemotePlayers();
int __stdcall CountCombatPlayers();

extern Game* g_game;
extern char DAT_005119b8[];
extern int g_packetModes[];
extern int g_usePacketManager;
extern PacketManager g_packetManager;
extern char g_allySoundName[];
extern char g_playerDisconnectedText[];
extern char g_integrityBreachText[];
extern char g_playerMessageFormat[];

int ReceiveNetPacket();
void CheckDuplicatePlayerIds();
void CheckPlayerTimeouts();
int GetTicks();
void SendLobbySyncRequests();
void RebuildAllyList();
int __stdcall GetSlotDpid(unsigned char);
unsigned char __stdcall FindSlotByDpid(int);
int __stdcall AddNetPlayer(int);
// Second parameter must be an int.
int __stdcall RejectPlayer(int, int);
void __stdcall BuildGameInfo(char*, int*, int*, int*, int*);
void __stdcall HAPINET_updategameinfo(void*, char*, char*, int, int, int, int);
void __stdcall HandlePing(void*);
void __stdcall AddMessage(void*, int, int, unsigned char);
int __stdcall SendPacketToPlayer(int, int, void*, int);
int __stdcall BroadcastPacket(int, void*, int);
void __stdcall BroadcastAllyTeam(Player*);
void __stdcall RemovePlayer(int);
int __stdcall IsColorFree(int, int);
void __stdcall AssignPlayerColor(int, int, int);
void __stdcall SetAlliance(int, int, unsigned char, int);
char* __stdcall Translate(char*);
void __stdcall PlaySoundByName(char*, int);
void __stdcall CreateUnitFromPacket(unsigned char, void*);
void __stdcall ReceiveUnitStates(Player*, void*);
void __stdcall ApplyAttachUnit(void*);
void __stdcall ApplyUnitDamage(void*);
void __stdcall ApplyUnitDeath(void*, int);
void __stdcall ApplyWeaponFirePacket(Player*, void*);
void __stdcall ApplyProjectileHitPacket(Player*, void*);
void __stdcall KillFeature(int, int, int);
void __stdcall StartFeatureBurning(int, int, int);
int __stdcall GetMapCell(int, int);
void __stdcall DamageFeature(int, int, int, WeaponDef*);
void __stdcall FinishConstruction(Unit*, Unit*);
void __stdcall PlaySoundByIndex(int, int);
void __stdcall PlaySoundAt(int, void*, int);
void __stdcall GiveUnitToPlayer(Unit*, Player*, void*);
void __stdcall TransferMetal(unsigned char, unsigned char, int, int);
void __stdcall TransferEnergy(unsigned char, unsigned char, int, int);
void __stdcall ShareMapInfo(unsigned char, unsigned char);
void __stdcall HandlePlayerEconomy(void*, Player*);
void __stdcall SetGameSpeed(int, int);

// The lookups below stay spelled out as inlined, half inlined or called: the
// inline budget makes each variant differ.
static inline int GetPlayerId(unsigned char i)
{
    if (i != 10 && g_game->players[i].state)
        return g_game->players[i].id;
    return -1;
}

// FindPlayerIndex and FindPlayerSlot test `id == -1` first, not a wrapped loop:
// adds a reference per inlined result, which orders the frame slots.
static inline unsigned char FindPlayerIndex(int id)
{
    if (id == -1)
        return 10;
    for (unsigned char i = 0; i < 10; i++) {
        if (GetPlayerId(i) == id)
            return i;
    }
    return 10;
}

static inline unsigned char FindPlayerSlot(int id)
{
    if (id == -1)
        return 10;
    for (unsigned char i = 0; i < 10; i++) {
        if (GetSlotDpid(i) == id)
            return i;
    }
    return 10;
}

static inline Player* PlayerById(int id)
{
    if (FindPlayerSlot(id) == 10)
        return 0;
    return &g_game->players[FindSlotByDpid(id)];
}

static inline Player* PlayerBySlot(int id)
{
    if (FindPlayerSlot(id) == 10)
        return 0;
    return &g_game->players[FindPlayerSlot(id)];
}

static inline Player* PlayerByIndex(int id)
{
    if (FindPlayerIndex(id) == 10)
        return 0;
    return &g_game->players[FindPlayerIndex(id)];
}

static inline unsigned char FindHost()
{
    for (unsigned char i = 0; i < 10; i++) {
        if (g_game->players[i].state != 0) {
            if (g_game->players[i].info->flags_97 & 1)
                return i;
        }
    }
    return 10;
}

// A real function, not inline.
int GetHostDpid()
{
    int i;
    for (i = 0; i < 10; i++) {
        if (g_game->players[i].info->flags_97 & 1)
            return GetPlayerId(i);
    }
    return -1;
}

static inline int FirstJoinedId()
{
    for (int i = 0; i < 10; i++) {
        if (g_game->players[i].state == 1)
            return g_game->players[i].id;
    }
    return -1;
}

static inline int FirstConnectedId()
{
    int i = 0;
    while (1) {
        if (g_game->players[i].active
            && (g_game->players[i].state == 1 || g_game->players[i].state == 2))
            return g_game->players[i].id;
        i++;
        if (i >= 10)
            return -1;
    }
}

static inline unsigned char* InfoPacket(unsigned char* buf, Player* p)
{
    SideDataPacket* pkt = (SideDataPacket*)buf;
    memcpy(&pkt->data, p->info, 0xb9);
    pkt->data.id = p->id;
    pkt->type = 0x20;
    return buf;
}

static inline int IsConnected(Player* p)
{
    return p->active && (p->state == 1 || p->state == 2);
}

static inline int IsValid(Player* p)
{
    return p->active && (p->state == 1 || p->state == 2 || p->state == 3);
}

// One helper, not split.
static inline int InGame(Player* p)
{
    return IsValid(p) && p->index != 10;
}

static inline Player* LocalPlayer()
{
    return &g_game->players[g_game->localPlayer];
}

static inline Unit* UnitAt(unsigned short index)
{
    if (index == 0)
        return 0;
    return &g_game->units[index];
}

static inline unsigned char FreeTeam()
{
    for (int team = 1; team <= 10; team++) {
        int used = 0;
        for (int i = 0; i < 10; i++) {
            Player* p = &g_game->players[i];
            if (IsValid(p) && p->index != 10 && p->lobbyDataSynced == team)
                used = 1;
        }
        if (!used)
            return team;
    }
    return 0;
}

static inline void DropPlayer(int id)
{
    if (FindPlayerSlot(id) == 10)
        return;
    PlayerIdPacket* out = (PlayerIdPacket*)g_game->recvPacketPtr;
    out->type = 0x1c;
    out->id = id;
    Player* p = &g_game->players[FindPlayerSlot(id)];
    if ((p->active && p->state == 3) || !(g_game->net_flags & 1) || (g_game->net_flags & 2))
        RemovePlayer(id);
    BroadcastPacket(LocalPlayer()->id, out, 5);
}

static inline int CommandAllowed(unsigned char* bytes)
{
    int mode = g_game->frontendState;
    if (mode == 5 && (g_packetModes[bytes[0]] & 2))
        return 1;
    if (mode == 6 && (g_packetModes[bytes[0]] & 4))
        return 1;
    if (mode != 5 && mode != 6 && (g_packetModes[bytes[0]] & 1))
        return 1;
    return 0;
}

// Stays in its own file: the dispatch loop's register allocation follows the
// original translation unit's symbol ids, which the gathered file changes.
// FUNCTION: 0x453d40
int HandleNetPackets()
{
    if (!(g_game->flags_2a44 & 1))
        return 0;
    int messages = 0;
    for (unsigned char i = 0; i < 10; i++)
        g_game->players[i].messages = 0;
    unsigned char* packet = g_game->recvPacketPtr;
    int more = 1;
    while (more) {
        more = ReceiveNetPacket();
        if (!more)
            continue;
        int sender = g_game->from_id;
        unsigned char from = FindPlayerIndex(sender);
        unsigned char to = FindPlayerIndex(g_game->local_id);
        Player* player = &g_game->players[from];
        Player* recipient = &g_game->players[to];
        messages++;
        if (sender == 0) {
            if (!IsConnected(recipient))
                continue;
            if (recipient->state != 1)
                continue;
            Packet_00453d40* msg = (Packet_00453d40*)packet;
            switch (msg->type) {
            case 5: {
                if (msg->playerType != 1)
                    break;
                Player* p = PlayerById(msg->id);
                if (!p)
                    break;
                if (!InGame(p))
                    break;
                if (!(g_game->flags_2a44 & 4) && (p->info->flags_97 & 1) && p->state == 3) {
                    RejectPlayer(p->id, 1);
                    RejectPlayer(LocalPlayer()->id, 10);
                    p->SetType(0);
                    LocalPlayer()->SetType(0);
                } else {
                    RejectPlayer(p->id, 1);
                    p->SetType(0);
                }
                g_game->dirty = 1;
                if (LocalPlayer()->info->flags_97 & 1) {
                    char name[32];
                    int a, b, c, d;
                    BuildGameInfo(name, &d, &c, &b, &a);
                    if (LocalPlayer()->info->started)
                        g_game->settings.flags_475 |= 0x20;
                    HAPINET_updategameinfo(g_game->session, name, DAT_005119b8, d, c, b, a);
                }
                break;
            }
            case 3: {
                if (!AddNetPlayer(msg->id))
                    break;
                PlayerById(msg->id);
                // int, not unsigned char.
                int target = FindPlayerIndex(msg->id);
                if (!g_game->players[FindHost()].IsPlayableSlot())
                    break;
                // info is read before payload: on equal priority the register goes to the first written.
                PlayerInfo* info = LocalPlayer()->info;
                char* payload = msg->createData;
                if (info->closed) {
                    RejectPlayer(GetPlayerId(target), 3);
                    break;
                }
                if (msg->dataSize != 0x15) {
                    RejectPlayer(GetPlayerId(target), 8);
                    break;
                }
                if (*(short*)(payload + 0x11) != 0 || *(short*)(payload + 0x13) != 0x50) {
                    RejectPlayer(GetPlayerId(target), 8);
                    break;
                }
                if (!(info->flags_9d & 1))
                    break;
                if (payload && !_strcmpi(g_game->password, payload))
                    break;
                RejectPlayer(GetPlayerId(target), 4);
                break;
            }
            case 0x102: {
                if (msg->playerType != 1)
                    break;
                Player temp;
                Player* p = PlayerById(msg->id);
                if (p) {
                    int target = FindPlayerIndex(msg->id);
                    if (target == 10) {
                        temp.FreeSideDataAndFogSightCounts();
                        continue;
                    }
                    char* payload = msg->data;
                    memcpy(g_game->players[target].info, payload, 0xb9);
                    if (FindHost() == g_game->localPlayer && !(LocalPlayer()->info->flags_9b & 0x80)
                        && (payload[0x9b] & 0x40))
                        RejectPlayer(p->id, 9);
                }
                temp.FreeSideDataAndFogSightCounts();
                break;
            }
            case 0x104:
                if (FindHost() != g_game->localPlayer)
                    memcpy(&g_game->settings, &msg->playerType, sizeof(Settings));
                break;
            case 0x103: {
                if (msg->playerType != 1)
                    break;
                Player* p = PlayerById(msg->id);
                if (p) {
                    strncpy(p->name, msg->longName, 0x1e);
                    strncpy(p->fullName, (char*)msg->dataSize, 0x1e);
                }
                break;
            }
            }
            continue;
        }
        if (!CommandAllowed(packet))
            continue;
        if (IsConnected(player))
            continue;
        if (!InGame(player)) {
            RejectPlayer(sender, 6);
            continue;
        }
        // Original bug: a command byte is never <= 1 and >= 0x2d at once, so
        // this error reply is dead (`||` was meant; docs/bugs.md).
        if (packet[0] <= 1 && packet[0] >= 0x2d) {
            RejectPlayer(sender, 6);
            continue;
        }
        if (player->state != 1 && player->state != 2 && player->state != 3)
            continue;
        if (!InGame(recipient))
            continue;
        player->last_time = GetTicks();
        player->messages++;
        switch (packet[0]) {
        case 32: {
            SideDataPacket* infoPacket = (SideDataPacket*)packet;
            int target = FindPlayerIndex(infoPacket->data.id);
            if (target == 10)
                break;
            if (g_game->players[target].active && g_game->players[target].state == 3) {
                memcpy(g_game->players[target].info, &infoPacket->data, 0xb9);
                CheckDuplicatePlayerIds();
            }
            break;
        }
        case 23:
            if (LocalPlayer()->info->flags_97 & 1) {
                if (!IsColorFree(g_game->from_id, (signed char)packet[1])) {
                    AssignPlayerColor(FirstJoinedId(), g_game->from_id, (signed char)packet[1]);
                } else {
                    unsigned char reply[2];
                    reply[0] = 0x18;
                    reply[1] = packet[1];
                    SendPacketToPlayer(FirstJoinedId(), g_game->from_id, reply, 2);
                    if (g_usePacketManager)
                        g_packetManager.SendAllQueued(1);
                }
            }
            break;
        case 24:
            g_game->players[to].info->color = packet[1];
            if (to == g_game->localPlayer && (g_game->flags_2a44 & 1)) {
                for (int i = 0; i < 10; i++) {
                    Player* p = &g_game->players[i];
                    // Declared here, not inside the IsConnected block: keeps the pushes after the stores.
                    unsigned char reply[0xba];
                    if (IsConnected(p)) {
                        BroadcastPacket(p->id, InfoPacket(reply, p), 0xba);
                        BroadcastAllyTeam(p);
                    }
                }
                SendLobbySyncRequests();
                g_packetManager.SendAllQueued(1);
            }
            g_game->dirty = 1;
            break;
        case 2:
            HandlePing(packet);
            break;
        case 38:
            memcpy(g_game->playerIds, packet + 1, 40);
            g_game->dirty = 1;
            break;
        case 35: {
            AllyFlagsPacket* ally = (AllyFlagsPacket*)packet;
            Player* a = PlayerBySlot(ally->fromNetId);
            Player* b = PlayerBySlot(ally->toNetId);
            if (!a || !b)
                break;
            if (ally->allied)
                PlaySoundByName(g_allySoundName, 0);
            if (IsConnected(b)) {
                SetAlliance(ally->fromNetId, ally->toNetId, ally->allied,
                             ally->force);
                if (!(g_game->flags_2a44 & 4))
                    g_game->dirty = 1;
                else
                    RebuildAllyList();
            }
            g_game->players[a->index].allies[b->index] = ally->allied;
            break;
        }
        case 36: {
            PlayerIdPacket* allyTeam = (PlayerIdPacket*)packet;
            Player* p = PlayerBySlot(allyTeam->id);
            if (p)
                p->alliance = packet[5];
            if (!(g_game->flags_2a44 & 4))
                g_game->dirty = 1;
            break;
        }
        case 27: {
            PlayerIdPacket* reject = (PlayerIdPacket*)packet;
            Player* p = PlayerBySlot(reject->id);
            if (p)
                RejectPlayer(p->id, packet[5]);
            break;
        }
        case 28: {
            PlayerIdPacket* drop = (PlayerIdPacket*)packet;
            int id = drop->id;
            if (FindPlayerIndex(id) == 10)
                break;
            char text[200];
            sprintf(text, Translate(g_playerDisconnectedText), PlayerBySlot(id)->name);
            AddMessage(text, 4, 0, from);
            DropPlayer(drop->id);
            if (drop->id == FirstJoinedId()) {
                g_game->bit2_3923b = 1;
                g_game->bit4_3923b = 0;
            }
            break;
        }
        case 30: {
            unsigned char reply[5];
            PlayerIdPacket* replyPacket = (PlayerIdPacket*)reply;
            replyPacket->type = 0x1f;
            recipient->startPos = packet[1];
            replyPacket->id = recipient->id;
            SendPacketToPlayer(recipient->id, player->id, reply, 5);
            break;
        }
        case 31: {
            PlayerIdPacket* ack = (PlayerIdPacket*)packet;
            int target = FindPlayerIndex(ack->id);
            if (target != 10)
                g_game->startPosAssignAck[target] = 1;
            break;
        }
        case 5:
            if (recipient->active && recipient->state == 1)
                AddMessage(packet + 1, 8, 0, from);
            break;
        case 39: {
            PlayerIdPacket* breach = (PlayerIdPacket*)packet;
            Player* p = PlayerBySlot(breach->id);
            if (!p)
                break;
            from = p->index;
            char text[256];
            sprintf(text, g_playerMessageFormat, p->name, Translate(g_integrityBreachText));
            for (int i = 0; i < 12; i++)
                AddMessage(text, 8, 0, from);
            break;
        }
        case 6: {
            unsigned char reply = 7;
            SendPacketToPlayer(FirstConnectedId(), g_game->from_id, &reply, 1);
            break;
        }
        case 7:
            player->flags_21 |= 1;
            break;
        case 8:
            more = 0;
            g_game->flags_2a44 |= 4;
            break;
        case 9:
            CreateUnitFromPacket(from, packet);
            break;
        case 44:
            ReceiveUnitStates(player, packet);
            break;
        case 10:
            ApplyAttachUnit(packet);
            break;
        case 11:
            ApplyUnitDamage(packet);
            break;
        case 12:
            ApplyUnitDeath(packet, 0);
            break;
        case 13:
            ApplyWeaponFirePacket(player, packet);
            break;
        case 14:
            ApplyProjectileHitPacket(player, packet);
            break;
        case 15: {
            FeatureDamagePacket* damage = (FeatureDamagePacket*)packet;
            switch (damage->sub) {
            case 0xfd:
                KillFeature(damage->x, damage->z, 0);
                break;
            case 0xfe:
                StartFeatureBurning(damage->x, damage->z, 1);
                break;
            case 0xff:
                KillFeature(damage->x, damage->z, 1);
                break;
            default:
                DamageFeature(GetMapCell(damage->x, damage->z),
                             damage->x, damage->z,
                             &g_game->weaponDefs[damage->sub]);
                break;
            }
            break;
        }
        case 16: {
            UnitScriptCallPacket* call = (UnitScriptCallPacket*)packet;
            Unit* unit = UnitAt(call->id);
            if (unit->flags & 0x10000000)
                unit->script->StartScriptWithArgsByIndex(call->index, 0, 0, call->argCount,
                                             call->arg0, call->arg1, call->arg2, call->arg3);
            break;
        }
        case 17: {
            Unit* unit = UnitAt(*(unsigned short*)(packet + 1));
            if (unit->flags & 0x10000000) {
                unit->SetStateBits(packet[3], 1);
                unit->SetStateBits(~packet[3], 0);
            }
            break;
        }
        case 18: {
            BuilderLinkPacket* link = (BuilderLinkPacket*)packet;
            Unit* a = UnitAt(link->constructedUnitId);
            FinishConstruction(UnitAt(link->builderUnitId), a);
            break;
        }
        case 19: {
            Packet_0047f0c0* sound = (Packet_0047f0c0*)packet;
            if (sound->flag)
                PlaySoundByIndex(sound->index, 0);
            else
                PlaySoundAt(sound->index, packet + 6, 0);
            break;
        }
        case 20: {
            OwnershipTransferPacket* transfer = (OwnershipTransferPacket*)packet;
            Unit* unit = UnitAt(transfer->unitId);
            if (!unit || !(unit->flags & 0x10000000))
                break;
            int id = transfer->newOwnerNetId;
            Player* p;
            if (FindPlayerIndex(id) == 10)
                p = 0;
            else
                p = &g_game->players[FindPlayerSlot(id)];
            // Original bug: p is 0 when the named player has left, and
            // IsConnected reads through it (docs/bugs.md).
            if (IsConnected(p))
                GiveUnitToPlayer(unit, p, packet);
            break;
        }
        case 21:
            if (g_game->net_bits.net_bit2)
                g_game->shareVisionReady[from] = 1;
            break;
        case 22: {
            ResourceSharePacket* share = (ResourceSharePacket*)packet;
            unsigned char a = FindPlayerIndex(share->fromNetId);
            unsigned char b = FindPlayerIndex(share->toNetId);
            if (a == 10 || b == 10)
                break;
            switch (share->subtype) {
            case 1:
                TransferMetal(a, b, share->amount, 0);
                break;
            case 2:
                TransferEnergy(a, b, share->amount, 0);
                break;
            case 3:
                ShareMapInfo(a, b);
                break;
            }
            break;
        }
        case 40:
            HandlePlayerEconomy(packet, player);
            break;
        case 41:
            if (packet[1]) {
                recipient->shareLos[from] = 1;
                if (packet[2])
                    recipient->shareMapping[from] = 1;
            }
            break;
        case 25:
            if (packet[1])
                SetGameSpeed(packet[2], 0);
            else
                g_game->bit_38a51 = packet[2];
            break;
        case 26:
            if (g_game->sync && recipient->active && recipient->state == 1)
                g_game->sync->ReceiveSyncPacket(packet, from);
            break;
        case 29: {
            PlayerIdPacket* integrity = (PlayerIdPacket*)packet;
            if (g_usePacketManager)
                g_packetManager.HandleIntegrityNop(g_game->from_id, integrity->id,
                                                   integrity->field_5);
            break;
        }
        case 33: {
            Msg_00450530* request = (Msg_00450530*)packet;
            Player* a = PlayerByIndex(request->id);
            Player* b = PlayerByIndex(request->arg);
            if (!IsConnected(&g_game->players[FindHost()]))
                break;
            if (!a)
                break;
            unsigned char reply[6];
            PlayerIdPacket* replyPacket = (PlayerIdPacket*)reply;
            replyPacket->type = 0x22;
            replyPacket->id = -1;
            reply[5] = 0;
            if (!request->flag) {
                replyPacket->id = a->id;
                reply[5] = a->lobbyDataSynced;
                if (!reply[5])
                    break;
                BroadcastPacket(GetHostDpid(), reply, 6);
            } else {
                if (b) {
                    replyPacket->id = a->id;
                    if (InGame(b))
                        reply[5] = b->lobbyDataSynced;
                } else {
                    replyPacket->id = a->id;
                    if (!a->lobbyDataSynced)
                        reply[5] = FreeTeam();
                }
                if (!reply[5])
                    break;
                a->lobbyDataSynced = reply[5];
                BroadcastPacket(GetHostDpid(), reply, 6);
            }
            break;
        }
        case 34: {
            PlayerIdPacket* team = (PlayerIdPacket*)packet;
            Player* p = PlayerByIndex(team->id);
            if (p)
                p->lobbyDataSynced = packet[5];
            break;
        }
        case 42:
            player->progress = packet[1];
            break;
        }
    }
    CheckDuplicatePlayerIds();
    CheckPlayerTimeouts();
    return messages;
}
