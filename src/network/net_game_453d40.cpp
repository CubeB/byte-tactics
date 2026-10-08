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

struct PlayerInfo {
    char unknown_0[0x90];
    int id;                            // +0x90
    char unknown_94[2];
    unsigned char color;               // +0x96
    unsigned char flags_97;            // +0x97
    char unknown_98[3];
    union {
        unsigned char flags_9b;        // +0x9b
        struct {
            unsigned short : 4;
            unsigned short bit4 : 1;
        } b9b;
        struct {
            unsigned short : 15;
            unsigned short bit15 : 1;
        } w9b;
    };
    unsigned char flags_9d;            // +0x9d
    char unknown_9e[0xb9 - 0x9e];
};

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
    char field_49[0x1e];               // +0x49
    char unknown_67[0x73 - 0x67];
    unsigned char state;               // +0x73
    char unknown_74[0x108 - 0x74];
    unsigned char allies[0x16];        // +0x108
    unsigned char field_11e[0x16];     // +0x11e
    unsigned char field_134[0xb];      // +0x134
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
// Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
int __stdcall GetPlayerDpid(Player*);
int __stdcall CountHumanPlayers();
int __stdcall CountComputerPlayers();
int __stdcall IsHostLocal();
int __stdcall FindFreeSlot();


struct Feature {
    char data[0x115];
};

class CobScript {
public:
    int StartScriptWithArgsByIndex(int, int, int, int, int, int, int, int);
};

class Unit {
public:
    char unknown_0[0x9a];
    CobScript* script;               // +0x9a
    char unknown_9e[0x110 - 0x9e];
    unsigned int flags_110;            // +0x110
    char unknown_114[4];
    void SetStateBits(unsigned char, int);
};

class UnitSync {
public:
    void ReceiveSyncPacket(void*, unsigned char);
};

struct Settings {
    int field_471;                     // +0x471
    int flags_475;                     // +0x475
    char unknown_479[0x48];
};

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
    unsigned char* packet;             // +0x2a38
    char unknown_2a3c[6];
    unsigned char local;               // +0x2a42
    char playerIndex;
    unsigned char flags_2a44;          // +0x2a44
    char unknown_2a45[0x2be3 - 0x2a45];
    char password[0x2bee - 0x2be3];    // +0x2be3
    unsigned short dirty : 1;          // +0x2bee
    unsigned short : 15;
    char unknown_2bf0[0x2c28 - 0x2bf0];
    int playerIds[11];                 // +0x2c28
    char unknown_2c54[0x2cf3 - 0x2c54];
    Feature features[256];             // +0x2cf3
    char unknown_after_features[0x14357 - (0x2cf3 + 0x115 * 256)];
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
    int mode;                          // +0x391f1
    char unknown_391f5[0x3923b - 0x391f5];
    unsigned short : 2;                // +0x3923b
    unsigned short bit2_3923b : 1;
    unsigned short : 1;
    unsigned short bit4_3923b : 1;
    unsigned short : 11;
};

struct Packet_00453d40 {
    unsigned int type;                 // +0x00
    int field_4;                       // +0x04
    int id;                            // +0x08
    char* field_c;                     // +0x0c
    char* field_10;                    // +0x10
    int field_14;                      // +0x14
    char* field_18;                    // +0x18
};

#pragma pack(pop)

class PacketManager {
public:
    void SendAllQueued(int);
    void HandleIntegrityNop(int, int, int);
};

// Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
int RIReport(int, int, int, int, int, int, int, int, int, int);
int DrawWrappedText(char*, char*, int, int, int, int, int);
void ParseDownloadableAiWeightScripts(int);

// Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
int __stdcall CountLocalComputerPlayers();
int __stdcall CountActiveAIPlayers();
int __stdcall CountActiveHumanOrAiPlayers();
int __stdcall CountRemotePlayers();
int __stdcall CountCombatPlayers();
int __stdcall AreAllPlayersReady();
int __stdcall BroadcastPendingViewState();
int __stdcall InitLobbiedConnection();
void __stdcall SendNetHeartbeat();
void __stdcall RemoveLocalPlayers();

extern Game* g_game;
extern char DAT_005119b8[];
extern int g_packetModes[];
extern int g_usePacketManager;
extern PacketManager g_packetManager;
extern char g_allySoundName[];
extern char DAT_005065c4[];
extern char DAT_0050658c[];
extern char DAT_00506290[];

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
void __stdcall DamageFeature(int, int, int, Feature*);
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
    memcpy(buf + 1, p->info, 0xb9);
    *(int*)(buf + 0x91) = p->id;
    buf[0] = 0x20;
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
    return &g_game->players[g_game->local];
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
    unsigned char* out = g_game->packet;
    out[0] = 0x1c;
    *(int*)(out + 1) = id;
    Player* p = &g_game->players[FindPlayerSlot(id)];
    if ((p->active && p->state == 3) || !(g_game->net_flags & 1) || (g_game->net_flags & 2))
        RemovePlayer(id);
    BroadcastPacket(LocalPlayer()->id, out, 5);
}

static inline int CommandAllowed(unsigned char* bytes)
{
    int mode = g_game->mode;
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
    unsigned char* packet = g_game->packet;
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
                if (msg->field_4 != 1)
                    break;
                Player* p = PlayerById(msg->id);
                if (!p)
                    break;
                if (!InGame(p))
                    break;
                if (!(g_game->flags_2a44 & 4) && (p->info->flags_97 & 1) && p->state == 3) {
                    RejectPlayer(p->id, 1);
                    RejectPlayer(LocalPlayer()->id, 10);
                    ((Player*)p)->SetType(0);
                    ((Player*)LocalPlayer())->SetType(0);
                } else {
                    RejectPlayer(p->id, 1);
                    ((Player*)p)->SetType(0);
                }
                g_game->dirty = 1;
                if (LocalPlayer()->info->flags_97 & 1) {
                    char name[32];
                    int a, b, c, d;
                    BuildGameInfo(name, &d, &c, &b, &a);
                    if (LocalPlayer()->info->b9b.bit4)
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
                char* payload = msg->field_10;
                if (info->w9b.bit15) {
                    RejectPlayer(GetPlayerId(target), 3);
                    break;
                }
                if (msg->field_14 != 0x15) {
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
                if (msg->field_4 != 1)
                    break;
                Player temp;
                Player* p = PlayerById(msg->id);
                if (p) {
                    int target = FindPlayerIndex(msg->id);
                    if (target == 10) {
                        temp.FreeSideDataAndFogSightCounts();
                        continue;
                    }
                    char* payload = msg->field_c;
                    memcpy(g_game->players[target].info, payload, 0xb9);
                    if (FindHost() == g_game->local && !(LocalPlayer()->info->flags_9b & 0x80)
                        && (payload[0x9b] & 0x40))
                        RejectPlayer(p->id, 9);
                }
                temp.FreeSideDataAndFogSightCounts();
                break;
            }
            case 0x104:
                if (FindHost() != g_game->local)
                    memcpy(&g_game->settings, &msg->field_4, sizeof(Settings));
                break;
            case 0x103: {
                if (msg->field_4 != 1)
                    break;
                Player* p = PlayerById(msg->id);
                if (p) {
                    strncpy(p->name, msg->field_18, 0x1e);
                    strncpy(p->field_49, (char*)msg->field_14, 0x1e);
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
            int target = FindPlayerIndex(*(int*)(packet + 0x91));
            if (target == 10)
                break;
            if (g_game->players[target].active && g_game->players[target].state == 3) {
                memcpy(g_game->players[target].info, packet + 1, 0xb9);
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
            if (to == g_game->local && (g_game->flags_2a44 & 1)) {
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
            Player* a = PlayerBySlot(*(int*)(packet + 1));
            Player* b = PlayerBySlot(*(int*)(packet + 5));
            if (!a || !b)
                break;
            if (packet[9])
                PlaySoundByName(g_allySoundName, 0);
            if (IsConnected(b)) {
                SetAlliance(*(int*)(packet + 1), *(int*)(packet + 5), packet[9],
                             *(int*)(packet + 10));
                if (!(g_game->flags_2a44 & 4))
                    g_game->dirty = 1;
                else
                    RebuildAllyList();
            }
            g_game->players[a->index].allies[b->index] = packet[9];
            break;
        }
        case 36: {
            Player* p = PlayerBySlot(*(int*)(packet + 1));
            if (p)
                p->alliance = packet[5];
            if (!(g_game->flags_2a44 & 4))
                g_game->dirty = 1;
            break;
        }
        case 27: {
            Player* p = PlayerBySlot(*(int*)(packet + 1));
            if (p)
                RejectPlayer(p->id, packet[5]);
            break;
        }
        case 28: {
            int id = *(int*)(packet + 1);
            if (FindPlayerIndex(id) == 10)
                break;
            char text[200];
            sprintf(text, Translate(DAT_005065c4), PlayerBySlot(id)->name);
            AddMessage(text, 4, 0, from);
            DropPlayer(*(int*)(packet + 1));
            if (*(int*)(packet + 1) == FirstJoinedId()) {
                g_game->bit2_3923b = 1;
                g_game->bit4_3923b = 0;
            }
            break;
        }
        case 30: {
            unsigned char reply[5];
            reply[0] = 0x1f;
            recipient->startPos = packet[1];
            *(int*)(reply + 1) = recipient->id;
            SendPacketToPlayer(recipient->id, player->id, reply, 5);
            break;
        }
        case 31: {
            int target = FindPlayerIndex(*(int*)(packet + 1));
            if (target != 10)
                g_game->startPosAssignAck[target] = 1;
            break;
        }
        case 5:
            if (recipient->active && recipient->state == 1)
                AddMessage(packet + 1, 8, 0, from);
            break;
        case 39: {
            Player* p = PlayerBySlot(*(int*)(packet + 1));
            if (!p)
                break;
            from = p->index;
            char text[256];
            sprintf(text, DAT_00506290, p->name, Translate(DAT_0050658c));
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
        case 15:
            switch (packet[1]) {
            case 0xfd:
                KillFeature(*(unsigned short*)(packet + 2), *(unsigned short*)(packet + 4), 0);
                break;
            case 0xfe:
                StartFeatureBurning(*(unsigned short*)(packet + 2), *(unsigned short*)(packet + 4), 1);
                break;
            case 0xff:
                KillFeature(*(unsigned short*)(packet + 2), *(unsigned short*)(packet + 4), 1);
                break;
            default:
                DamageFeature(GetMapCell(*(unsigned short*)(packet + 2), *(unsigned short*)(packet + 4)),
                             *(unsigned short*)(packet + 2), *(unsigned short*)(packet + 4),
                             &g_game->features[packet[1]]);
                break;
            }
            break;
        case 16: {
            Unit* unit = UnitAt(*(unsigned short*)(packet + 1));
            if (unit->flags_110 & 0x10000000)
                unit->script->StartScriptWithArgsByIndex(*(short*)(packet + 3), 0, 0, packet[5],
                                             *(int*)(packet + 6), *(int*)(packet + 10),
                                             *(int*)(packet + 14), *(int*)(packet + 18));
            break;
        }
        case 17: {
            Unit* unit = UnitAt(*(unsigned short*)(packet + 1));
            if (unit->flags_110 & 0x10000000) {
                unit->SetStateBits(packet[3], 1);
                unit->SetStateBits(~packet[3], 0);
            }
            break;
        }
        case 18: {
            Unit* a = UnitAt(*(unsigned short*)(packet + 1));
            FinishConstruction(UnitAt(*(unsigned short*)(packet + 3)), a);
            break;
        }
        case 19:
            if (packet[1])
                PlaySoundByIndex(*(int*)(packet + 2), 0);
            else
                PlaySoundAt(*(int*)(packet + 2), packet + 6, 0);
            break;
        case 20: {
            Unit* unit = UnitAt(*(unsigned short*)(packet + 1));
            if (!unit || !(unit->flags_110 & 0x10000000))
                break;
            int id = *(int*)(packet + 3);
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
            unsigned char a = FindPlayerIndex(*(int*)(packet + 5));
            unsigned char b = FindPlayerIndex(*(int*)(packet + 9));
            if (a == 10 || b == 10)
                break;
            switch (*(int*)(packet + 1)) {
            case 1:
                TransferMetal(a, b, *(int*)(packet + 13), 0);
                break;
            case 2:
                TransferEnergy(a, b, *(int*)(packet + 13), 0);
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
                recipient->field_11e[from] = 1;
                if (packet[2])
                    recipient->field_134[from] = 1;
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
        case 29:
            if (g_usePacketManager)
                g_packetManager.HandleIntegrityNop(g_game->from_id, *(int*)(packet + 1),
                                                   *(int*)(packet + 5));
            break;
        case 33: {
            Player* a = PlayerByIndex(*(int*)(packet + 2));
            Player* b = PlayerByIndex(*(int*)(packet + 6));
            if (!IsConnected(&g_game->players[FindHost()]))
                break;
            if (!a)
                break;
            unsigned char reply[6];
            reply[0] = 0x22;
            *(int*)(reply + 1) = -1;
            reply[5] = 0;
            if (!packet[1]) {
                *(int*)(reply + 1) = a->id;
                reply[5] = a->lobbyDataSynced;
                if (!reply[5])
                    break;
                BroadcastPacket(GetHostDpid(), reply, 6);
            } else {
                if (b) {
                    *(int*)(reply + 1) = a->id;
                    if (InGame(b))
                        reply[5] = b->lobbyDataSynced;
                } else {
                    *(int*)(reply + 1) = a->id;
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
            Player* p = PlayerByIndex(*(int*)(packet + 1));
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
