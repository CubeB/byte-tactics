// Decompiled by Haiku, Sonnet, Opus, deepseek-v4.1-flash, DeepSeek V4.1 Flash, deepseek-v4.1, gpt-6-luna, GPT-6.1-sol, mimo-v2.6-pro, Claude Opus 5.5, space-bunny-free, claude-sonnet-5-5 and Space Bunny Free. Names are provisional.
// The network game's player lookups, slot handling, player names and info
// packets and the session setup: the files of the module's first part
// (0x44fd40 to 0x451220) gathered in address order.
//
// <stdio.h> and <stdlib.h> carry 0x450380's sprintf and rand, and <string.h>
// the string copies of 0x450090, 0x450140, 0x450980, 0x451090 and 0x451220.
// <windows.h> is deliberately absent: 0x450f90's original file needed it for
// its loop's base/index order, but in this file's declaration context it
// matches without it, and with it 0x451220 stops matching.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// DirectPlay 3 DPNAME (the toolchain's DirectX 3 <dplay.h> lacks it).
struct DPNAME {
    unsigned long dwSize;
    unsigned long dwFlags;
    char* lpszShortNameA;
    char* lpszLongNameA;
};

typedef void (__stdcall *FuncPtr)(void*);
extern FuncPtr g_packetHandlers[];

class Class_0044fda0 {
public:
    void DispatchPacket();
};

class Mission {
public:
    int GetGameType();
    int GetMissionName();
};

class PacketManager {
public:
    int SendAllQueued(int param_1);
};

#pragma pack(push, 1)

// The info block a player record points at (+0x27). One type for every view:
// where two views name the same bytes differently the union carries both
// names.
struct PlayerInfo {
    char unknown_0[0x8b];
    unsigned short field_8b;           // +0x8b
    unsigned short field_8d;           // +0x8d
    char unknown_8f[0x90 - 0x8f];
    int field_90;                      // +0x90
    char unknown_94[0x96 - 0x94];
    unsigned char field_96;            // +0x96
    union {
        unsigned char flags_97;        // +0x97
        struct {
            unsigned short flag_97_0 : 1;
            unsigned short rest_97 : 15;
        } bits_97;
    };
    char unknown_99[0x9b - 0x99];
    union {
        unsigned short field_9b;       // +0x9b
        struct {
            unsigned short : 4;
            unsigned short flag_9b_4 : 1;
            unsigned short : 11;
        } bits_9b;
    };
    unsigned short flag_9d_0 : 1;      // +0x9d
    unsigned short rest_9d : 15;
    char unknown_9f[0xa5 - 0x9f];
    unsigned short field_a5;           // +0xa5
    unsigned char field_a7;            // +0xa7
    unsigned char field_a8;            // +0xa8
    char unknown_a9[0xb9 - 0xa9];
};

// A player record at g_game+0x1b63 (0x14b bytes). One type for every view:
// where two views name the same bytes differently the union carries both
// names.
struct Player {
    int active;                        // +0x00
    union {
        int id;                        // +0x04
        int dpid;                      // +0x04
        int field_4;                   // +0x04
    };
    int field_8;                       // +0x08
    int field_c;                       // +0x0c
    char unknown_10[0x1c - 0x10];
    int field_1c;                      // +0x1c
    char unknown_20[0x21 - 0x20];
    unsigned char field_21;            // +0x21
    union {
        unsigned char field_22;        // +0x22
        unsigned char reason;          // +0x22
    };
    char unknown_23[0x27 - 0x23];
    union {
        PlayerInfo* info;              // +0x27
        PlayerInfo* data;              // +0x27
    };
    char name[0x1e];                   // +0x2b
    char fullName[0x2a];               // +0x49
    union {
        unsigned char type;            // +0x73
        char state;                    // +0x73
    };
    char unknown_74[0x13f - 0x74];
    union {
        char alliance;                 // +0x13f
        char field_13f;                // +0x13f
    };
    char unknown_140[0x146 - 0x140];
    unsigned char field_146;           // +0x146
    char unknown_147[0x14b - 0x147];
    void SetType(int value);
};

struct Game {
    char unknown_0[1];
    unsigned char field_1;             // +0x01
    unsigned char field_2;             // +0x02
    char unknown_3[0x14 - 3];
    char net[0x475 - 0x14];            // +0x14
    unsigned int unknown_475_0 : 5;    // +0x475
    unsigned int flag_475_5 : 1;
    unsigned int unknown_475_6 : 26;
    char unknown_479[0x4f1 - 0x479];
    int netMode;                       // +0x4f1
    char unknown_4f5[0x519 - 0x4f5];
    char menu[0x1b63 - 0x519];         // +0x519
    Player players[10];                // +0x1b63
    char unknown_2851[0x299c - 0x2851];
    int duplicateIds;                  // +0x299c
    char unknown_29a0[0x2a38 - 0x29a0];
    unsigned char* buffer;             // +0x2a38
    union {
        unsigned short field_2a3c;     // +0x2a3c
        short field_2a3c_signed;       // +0x2a3c
    };
    char unknown_2a3e[0x2a42 - 0x2a3e];
    unsigned char localPlayer;         // +0x2a42
    char unknown_2a43;
    unsigned short flags_2a44;         // +0x2a44
    char unknown_2a46[0x2bc1 - 0x2a46];
    char gameName[0x10];               // +0x2bc1
    char unknown_2bd1;
    char nickName[0x11];               // +0x2bd2
    char passWord[0x11];               // +0x2be3
    char unknown_2bf4[0x37f1b - 0x2bf4];
    short field_37f1b;                 // +0x37f1b
    char unknown_37f1d[0x37f1f - 0x37f1d];
    short field_37f1f;                 // +0x37f1f
    char unknown_37f21[0x391e9 - 0x37f21];
    Mission* campaign;                 // +0x391e9
    char unknown_391ed[0x391f1 - 0x391ed];
    int mode;                          // +0x391f1
    char unknown_391f5[0x39211 - 0x391f5];
    char connection[4];                // +0x39211
    char unknown_39215[0x3923b - 0x39215];
    unsigned short field_3923b;        // +0x3923b
};

struct Packet_00450a10 {
    unsigned char type;                // +0x0
    PlayerInfo data;                   // +0x1
};

struct Packet_00450f90 {
    unsigned char type;                // +0x0
    PlayerInfo data;                   // +0x1
};

#pragma pack(pop)

// GLOBAL: 0x511de8
extern Game* g_game;
extern char* g_leftGameTexts[8];
extern int g_usePacketManager;
extern PacketManager g_packetManager;
extern char DAT_005119b8[];

int __stdcall HAPINET_setplayername(void* net, int dpid, DPNAME* name, int flags);
int __stdcall HAPINET_getplayername(void* net, unsigned long id, void* data, unsigned long* size);
int __stdcall HAPINET_addplayer(void* net, unsigned long* id, char* shortName, char* longName,
                                char* name, short field_11, short field_13);
void __stdcall HAPINET_updategameinfo(void* obj, char* name, char* data, int d, int c, int b, int a);
void __stdcall HAPINET_initmultiplaydefaults(void* net);
void __stdcall HAPINET_initconnection(void* net, void* connection);
int __stdcall HAPINET_quitgame(void* net);
void __stdcall HAPINET_uninitmultiplay(void* param_1);
int __stdcall InitPacketManager(int a, int b);
bool IsReporterDllLoaded();
void __stdcall RemovePlayer(int id);
void ShutdownScoreTables();
void __stdcall SetCloseHandler(void (__cdecl *callback)(int), int param);
void __stdcall QuitApp(char* message);
char* __stdcall Translate(char* text);
void __stdcall AddMessage(char* text, int param_2, int param_3, char param_4);
void FUN_00450530();
int __stdcall BroadcastPacket(int player, void* data, int size);
void __stdcall SetupPlayerSlot(unsigned char player, char type);
int GetTicks();
void __stdcall ReportGameEvent(int param_1);
void __stdcall OpenMessageBox(void* menu, const char* text, int a, int b, int c);

// FUNCTION: 0x44fd40
int __stdcall DefaultPacketHandler(int arg1)
{
    return 0xffffffff;
}

// FUNCTION: 0x44fd50
int __stdcall FUN_0044fd50(int arg1)
{
    return 0xffffffff;
}

// FUNCTION: 0x44fd60
int __stdcall FUN_0044fd60(int)
{
    return 0xffffffff;
}

// FUNCTION: 0x44fd70
int __stdcall FUN_0044fd70(int arg1)
{
    return 0xffffffff;
}

// FUNCTION: 0x44fd80
int __stdcall FUN_0044fd80(int arg1)
{
    return 0xffffffff;
}

// FUNCTION: 0x44fd90
int __stdcall FUN_0044fd90(int arg1)
{
    return 0xffffffff;
}

// FUNCTION: 0x44fda0
void Class_0044fda0::DispatchPacket()
{
    unsigned int idx = 0;
    idx = *(unsigned char*)this;
    g_packetHandlers[idx](this);
}

// Returns the id of the first active player whose type is 1 or 2, or -1.
// FUNCTION: 0x44fdb0
int GetLocalDpid()
{
    int i = 0;
    while (!(g_game->players[i].active != 0
             && (g_game->players[i].type == 1 || g_game->players[i].type == 2))) {
        if (++i >= 10)
            return -1;
    }
    return g_game->players[i].id;
}

// FUNCTION: 0x44fe00
int GetLocalHumanDpid()
{
    for (int i = 0; i < 10; i++) {
        if (g_game->players[i].type == 1) {
            return g_game->players[i].id;
        }
    }
    return -1;
}

static inline int PlayerId(unsigned char i)
{
    if (i == 10 || !g_game->players[i].type)
        return -1;
    return g_game->players[i].id;
}

// FUNCTION: 0x44fe40
unsigned char __stdcall FindSlotByDpid(int id)
{
    if (id != -1) {
        for (unsigned char i = 0; i < 10; i++) {
            if (PlayerId(i) == id)
                return i;
        }
    }
    return 10;
}

// FUNCTION: 0x44feb0
unsigned char __stdcall FUN_0044feb0(Player* player)
{
    if (player == 0) {
        return 10;
    }
    return player->field_146;
}

static inline int GetPlayerField_0044fed0(unsigned char i)
{
    // Redundant i != 10 test must stay: 10 is the not-found index.
    if (i != 10 && g_game->players[i].type)
        return g_game->players[i].id;
    return -1;
}

static inline unsigned char FindPlayerIndex_0044fed0(int id)
{
    if (id != -1) {
        for (unsigned char i = 0; i < 10; i++) {
            if (GetPlayerField_0044fed0(i) == id)
                return i;
        }
    }
    return 10;
}

// Finds the player whose field_4 equals the given id and returns a pointer to
// it, or null when there is none.
// FUNCTION: 0x44fed0
Player* __stdcall FindPlayerByDpid(int id)
{
    if (FindPlayerIndex_0044fed0(id) == 10)
        return 0;
    return &g_game->players[FindPlayerIndex_0044fed0(id)];
}

// Returns the id at +4 of a player slot (index 10 means none), or -1 when
// the slot is unused; the pointer version is 0x450010.
// FUNCTION: 0x44ffd0
int __stdcall GetSlotDpid(unsigned char index)
{
    if (index != 10 && g_game->players[index].type != 0)
        return g_game->players[index].id;
    return -1;
}

// FUNCTION: 0x450010
int __stdcall GetPlayerDpid(Player* obj)
{
    if (obj != 0 && obj->type != 0) {
        return (int)obj->field_4;
    }
    return -1;
}

static inline int PlayerField(unsigned char index)
{
    if (index != 10 && g_game->players[index].type != 0)
        return g_game->players[index].id;
    return -1;
}

// FUNCTION: 0x450030
int GetHostDpid()
{
    int i;
    for (i = 0; i < 10; i++) {
        if (g_game->players[i].info->flags_97 & 1)
            return PlayerField(i);
    }
    return -1;
}

static inline int PlayerDpid(Player* p)
{
    if (p != 0 && p->type != 0)
        return p->dpid;
    return -1;
}

// FUNCTION: 0x450090
int __stdcall SetPlayerName(unsigned char index, char* shortName, char* longName)
{
    Player* p = &g_game->players[index];
    strncpy(p->name, longName, 30);
    strncpy(p->fullName, shortName, 30);
    DPNAME name;
    name.dwSize = sizeof(DPNAME);
    name.dwFlags = 0;
    name.lpszShortNameA = shortName;
    name.lpszLongNameA = longName;
    int r = HAPINET_setplayername(g_game->net, PlayerDpid(p), &name, 2);
    return r == 0 ? 1 : 0;
}

// FUNCTION: 0x450140
int __stdcall GetPlayerName(int dpid, char* shortName, char* longName)
{
    int r;
    if (dpid != -1) {
        char buf[0x400];
        unsigned long size = 0x400;
        r = HAPINET_getplayername(g_game->net, dpid, buf, &size);
        if (r == 0) {
            strcpy(shortName, ((DPNAME*)buf)->lpszShortNameA);
            strcpy(longName, ((DPNAME*)buf)->lpszLongNameA);
        }
    } else {
        strcpy(shortName, "COMPUTER");
        strcpy(longName, "COMPUTER");
        r = 0;
    }
    return r == 0;
}

static inline int GetPlayerField(unsigned char i)
{
    if (i != 10 && g_game->players[i].type)
        return g_game->players[i].field_4;
    return -1;
}

static inline unsigned char FindPlayerIndex_00450240(int id)
{
    // Early return, not a loop wrapped in if (id != -1).
    if (id == -1)
        return 10;
    for (unsigned char i = 0; i < 10; i++) {
        if (GetPlayerField(i) == id)
            return i;
    }
    return 10;
}

static inline Player* FindPlayer(int id)
{
    if (FindPlayerIndex_00450240(id) == 10)
        return 0;
    return &g_game->players[FindPlayerIndex_00450240(id)];
}

// Finds the highest field_4 among the active players of type 1 or 3, looks
// that player up by field_4 and sets bit 0 of its info flags. Nothing in the
// exe calls it. It returns the constant 10, the "no player" index.
// FUNCTION: 0x450240
unsigned char PickNewHost()
{
    unsigned int max = 0;
    Player* p = g_game->players;
    int n = 10;
    do {
        if ((p->active != 0 && p->type == 3)
            || (p->active != 0 && p->type == 1)) {
            if (p->field_4 > max)
                max = p->field_4;
        }
        p++;
    } while (--n);

    Player* q = FindPlayer(max);
    if (q)
        q->info->flags_97 |= 1;
    // Must return 10, not void.
    return 10;
}

static inline int GetPlayerField_00450380(unsigned char i)
{
    if (i != 10 && g_game->players[i].type)
        return g_game->players[i].id;
    return -1;
}

static inline unsigned char FindPlayerIndex_00450380(int id)
{
    if (id != -1) {
        for (unsigned char i = 0; i < 10; i++) {
            if (GetPlayerField_00450380(i) == id)
                return i;
        }
    }
    return 10;
}

// Announces that the player with the given id has left, unless the local
// player is flagged. The index search was an inlined helper and appears twice
// in the original (see 0x44fed0, which has the same shape).
// FUNCTION: 0x450380
void __stdcall AnnouncePlayerLeft(int id)
{
    char buf[200];
    if (g_game->players[g_game->localPlayer].field_22 == 1)
        return;
    Player* p;
    if (FindPlayerIndex_00450380(id) == 10)
        p = 0;
    else
        p = &g_game->players[FindPlayerIndex_00450380(id)];
    if (p == 0)
        return;
    sprintf(buf, "%s %s", p->name, Translate(g_leftGameTexts[rand() & 7]));
    AddMessage(buf, 4, 0, p->field_146);
}

// Returns the first player slot that is inactive and not of type 4, or 10
// when there is none.
// FUNCTION: 0x4504f0
int FindFreeSlot()
{
    int found = 0;
    int i;
    for (i = 0; i < 10; i++) {
        Player* p = &g_game->players[i];
        if (p->active == 0 && p->type != 4) {
            found = 1;
            break;
        }
    }
    if (!found)
        i = 10;
    return i;
}

// FUN_00450530 (0x450530) stays in net_game_450530.cpp. Its original
// translation unit saw only a prototype of GetSlotDpid (0x44ffd0), so the
// compiler could not inline it; here the definition is in the same file and
// /Ob2 expands it into the FindTo helper, which then misses the inline budget
// at one IsPlaying site and spills the loop counter, 989 bytes against 977.
// Putting the definition after the caller does not help (MSVC inlines across
// the whole file); #pragma auto_inline(off) does, but the guide allows that
// only in a class's file, so 0x450530 keeps a file of its own.

// FUNCTION: 0x450910
char FUN_00450910(void)
{
    Game* game = g_game;
    for (int id = 1; id <= 10; id++) {
        int used = 0;
        for (int i = 0; i < 10; i++) {
            Player* p = &game->players[i];
            if (p->active != 0
                && (p->type == 1 || p->type == 2 || p->type == 3)
                && p->field_146 != 10
                && p->field_c == id) {
                used = 1;
            }
        }
        if (!used) {
            return id;
        }
    }
    return 0;
}

// Sets the game's flag at +0x299c when two active players (of type 1, 2 or
// 3, and not in state 10) share an id from 1 to 10 (see 0x450910, which
// finds the first unused id).
// Indexing g_game->players[i] in every test (rather than a player pointer
// local) is what makes MSVC walk the array from the type field.
// FUNCTION: 0x450980
void FUN_00450980(void)
{
    int counts[11];
    memset(counts, 0, sizeof(counts));
    int dup = 0;
    for (int i = 0; i < 10; i++) {
        if (g_game->players[i].active != 0
            && (g_game->players[i].type == 1 || g_game->players[i].type == 2 || g_game->players[i].type == 3)
            && g_game->players[i].field_146 != 10
            && g_game->players[i].field_c > 0 && g_game->players[i].field_c <= 10) {
            if (++counts[g_game->players[i].field_c] > 1) {
                dup = 1;
                break;
            }
        }
    }
    g_game->duplicateIds = dup;
}

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
            Player* q = &g_game->players[i];
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
    Player* p = &g_game->players[slot];
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
    SetupPlayerSlot(slot, g_game->players[slot].type);
    p->field_22 = 0;
    p->id = param_1;
    p->field_1c = GetTicks();
    g_game->field_2a3c++;
    if (g_game->flags_2a44 & 1) {
        for (int i = 0; i < 10; i++) {
            Player* q = &g_game->players[i];
            if (q->active != 0 && (q->type == 1 || q->type == 2)) {
                packet.data = *q->data;
                packet.data.field_90 = q->id;
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
    if (g_game->campaign->GetGameType() == 3 && g_game->field_2a3c > 1) {
        ReportGameEvent(2);
    }
    return 1;
}

// FUNCTION: 0x450d80
void InitNetConnection()
{
    HAPINET_initmultiplaydefaults(g_game->net);
    g_game->netMode = 10;
    if (InitPacketManager(2, 100)) {
        HAPINET_initconnection(g_game->net, g_game->connection);
    }
}

// FUNCTION: 0x450dd0
void CloseNetSession()
{
    if (g_game->flags_2a44 & 1) {
        if (g_usePacketManager != 0) {
            g_packetManager.SendAllQueued(1);
        }
        if (!IsReporterDllLoaded()) {
            HAPINET_uninitmultiplay(g_game->net);
        }
        g_game->flags_2a44 &= 0xfffe;
    }
}

static inline int IsPlaying_00450e20(Player* player)
{
    if (player->active == 0)
        return 0;
    if (player->state == 1 || player->state == 2)
        return 1;
    return 0;
}

// FUNCTION: 0x450e20
void LeaveNetGame()
{
    if (g_usePacketManager != 0) {
        g_packetManager.SendAllQueued(1);
    }
    if (g_game->flags_2a44 & 1) {
        for (int i = 0; i < 10; i++) {
            if (IsPlaying_00450e20(&g_game->players[i])) {
                RemovePlayer(g_game->players[i].id);
            }
        }
        ShutdownScoreTables();
    }
    HAPINET_quitgame(g_game->net);
    SetCloseHandler(0, 0);
    g_game->field_3923b |= 4;
    int reason = g_game->players[g_game->localPlayer].reason;
    char* text;
    if (reason != 0) {
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
    } else {
        text = 0;
    }
    QuitApp(text);
}

static inline int IsPlaying_00450f90(Player* player)
{
    if (player->active == 0)
        return 0;
    if (player->state == 1 || player->state == 2)
        return 1;
    return 0;
}

// When the game is networked (flag 1 at +0x2a44), sends every playing
// player's 0xb9-byte data block (type 0x20) and then its 6-byte message
// (type 0x24, the same code as FUN_00452bd0).
// The packet is declared at function scope: its address escapes to
// BroadcastPacket in one iteration, so MSVC re-reads player->id around the
// stores into it in the next, as the original does.
// FUNCTION: 0x450f90
void BroadcastPlayerInfo()
{
    Packet_00450f90 packet;
    if (g_game->flags_2a44 & 1) {
        for (int i = 0; i < 10; i++) {
            Player* player = &g_game->players[i];
            if (IsPlaying_00450f90(player)) {
                packet.data = *player->data;
                packet.data.field_90 = player->id;
                packet.type = 0x20;
                BroadcastPacket(player->id, &packet, sizeof(packet));
                if (IsPlaying_00450f90(player)) {
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

// FUNCTION: 0x451090
void __stdcall BuildGameInfo(char* name, int* d, int* c, int* b, int* a)
{
    PlayerInfo* info = g_game->players[g_game->localPlayer].info;
    char* p = (char*)info;
    p[0xa7] = g_game->field_1;
    p += 0x99;
    p[0xf] = g_game->field_2;
    unsigned short w = *(unsigned short*)(p + 2);
    p += 4;
    *(unsigned short*)(p - 2) = w ^ ((g_game->field_2a3c ^ w) & 0xf);
    p += 4;
    *d = *(int*)(p - 8);
    *c = *(int*)(p - 4);
    *b = *(int*)(p);
    *a = *(int*)(p + 4);
    memset(name, ' ', 0x20);
    name[0x1f] = 0;
    strncpy(name, g_game->gameName, 0x10);
    int src = g_game->campaign->GetMissionName();
    strncpy(name + 0x10, (char*)src, 0xf);
    char* q = name;
    int n = 0x20;
    do {
        if (*q == 0)
            *q = ' ';
        q++;
    } while (--n);
    name[0x1f] = 0;
}

// The flag is a bit of an unsigned short bitfield whose storage starts at the
// odd offset 0x9b (packed struct): that gives the byte load and "shr al, 4;
// test al, 1". An unsigned char bitfield folds to "test byte ptr".
// FUNCTION: 0x451180
void UpdateNetGameInfo(void)
{
    int a;
    int b;
    int c;
    int d;
    char name[32];

    BuildGameInfo(name, &d, &c, &b, &a);
    if (g_game->players[g_game->localPlayer].info->bits_9b.flag_9b_4) {
        g_game->flag_475_5 = 1;
    }
    HAPINET_updategameinfo(g_game->net, name, DAT_005119b8, d, c, b, a);
}

// The index of the first connected player, or 10 when there is none.
// Must stay a static inline helper: return i in the loop, return 10 after it.
static inline unsigned char FindPlayerInUse()
{
    for (unsigned char i = 0; i < 10; i++) {
        if (g_game->players[i].type != 0 && g_game->players[i].info->bits_97.flag_97_0)
            return i;
    }
    return 10;
}

// Sends the "AI:" or nickname name to DirectPlay for one player slot and
// fills in that slot's PlayerInfo. The two "is any player connected" searches
// use the FindPlayerInUse helper above, called once before the name is
// built and once on the failure path; see 0x4515d0, 0x451bc0 and 0x451df0 for
// the same helper.
// FUNCTION: 0x451220
int __stdcall CreateLocalPlayer(unsigned char playerIndex, int flag)
{
    char buf[256];

    Player* player = &g_game->players[playerIndex];
    player->SetType(flag);

    // Parameter on the left of the compare.
    int same = (playerIndex == FindPlayerInUse());

    if (flag == 1) {
        strcpy(buf, g_game->nickName);
    } else {
        sprintf(buf, "AI:%s", g_game->players[g_game->localPlayer].name);
        buf[16] = 0;
    }

    player->info->bits_97.flag_97_0 = same;
    player->info->field_96 = 0xff;
    player->field_21 &= 0xfd;
    player->field_8 = GetTicks();
    PlayerInfo* info = player->info;
    info->field_9b = (info->field_9b ^ ((g_game->field_2a3c_signed ^ info->field_9b) & 0xf)) & 0x7fff;
    info->flag_9d_0 = (strlen(g_game->passWord) != 0);
    info->field_a5 = 0x64;
    info->field_8b = g_game->field_37f1b;
    info->field_8d = g_game->field_37f1f;
    info->field_a7 = g_game->field_1;
    info->field_a8 = g_game->field_2;

    int r = HAPINET_addplayer(g_game->net, (unsigned long*)&player->field_4,
                         buf, buf, g_game->passWord, 0, 0x50);
    if (r == 0) {
        g_game->players[playerIndex].SetType(0);
        unsigned char i = FindPlayerInUse();
        Player* slot = &g_game->players[i];
        if (slot->active != 0 && (slot->type == 1 || slot->type == 2)) {
            OpenMessageBox(g_game->menu,
                Translate("Direct Play failed to add new player.\n\nRecommended you go to previous screen and re-create the game session.\n"),
                500, 1, 1);
        }
        else {
            OpenMessageBox(g_game->menu,
                Translate("Direct Play failed to add new player.\n\nRecommended you go to previous screen and re-join the game session.\n"),
                500, 1, 1);
        }
    }
    return r;
}
