// Decompiled by deepseek-v4.1-flash, finished by claude-opus-5-5, finished by DeepSeek V4.1 Flash,
// checked by GPT-6, finished by claude-opus-5-5. Names are provisional.
//
// MATCH (claude-opus-5-5, #5136), from the #4288 rewrite (63.3%) in four steps:
//  - FindPlayerIndex and FindPlayerSlot test `if (id == -1) return 10;` first
//    instead of wrapping the loop in `if (id != -1)`. The code is the same,
//    but each inlined result now counts one more reference, which puts the
//    16 result slots below the ~40 loop counters as in the original: every
//    scalar frame slot lands right (63.3 -> 79.2). The two copies of the slot
//    lookup (one had the early return already) are one helper now.
//  - case 24's reply buffer is declared in the loop body, not inside the
//    IsConnected block, so the argument pushes stay after the InfoPacket
//    stores and p->id is reloaded for the call (79.2 -> 95.9); the stores
//    are memcpy, the id, then the type byte.
//  - <windows.h> (WIN32_LEAN_AND_MEAN): with that many declarations before
//    the function every two-register address takes the pointer as its base
//    ([edx+eax+K], [ebx+esi+K]); dummy declarations do the same from about
//    3200 on. <memory.h> sets the count to a value where case 35's
//    allies[a->index][b->index] store has the original's form; that store
//    flips with period 16 in the declaration count (see #4992's notes).
//  - case 3 reads info before payload: on equal priority C2 gives esi to the
//    variable written first in the block (tools/c2prio.py).
//
// Shape facts from the earlier rounds that still hold:
//  - The lookups are the real helpers next door (0x44fdb0 .. 0x450910);
//    MSVC 5 inlines a helper only while its size budget lasts, so the same
//    lookup appears inlined, half inlined (FUN_0044ffd0 called) or called
//    (FUN_0044fe40). Each variant is spelled out below.
//  - `int target = FindPlayerIndex(..)` (not unsigned char).
//  - InGame(p) is one helper; FUN_00450030 (HostId) is a real function.
//  - FUN_00453010's second parameter is an int.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#pragma pack(push, 1)

struct PlayerInfo {
    char unknown_0[0x90];
    int field_90;                      // +0x90
    char unknown_94[2];
    unsigned char field_96;            // +0x96
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
    int field_8;                       // +0x08
    int team;                          // +0x0c
    int messages;                      // +0x10
    char unknown_14[8];
    int last_time;                     // +0x1c
    unsigned char field_20;            // +0x20
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
    unsigned char field_13f;           // +0x13f
    char unknown_140[6];
    unsigned char index;               // +0x146
    unsigned char field_147;           // +0x147
    char unknown_148[3];
};

struct Feature {
    char data[0x115];
};

class Class_004b0b00 {
public:
    int FUN_004b0b00(int, int, int, int, int, int, int, int);
};

class Class_0048b090 {
public:
    char unknown_0[0x9a];
    Class_004b0b00* field_9a;          // +0x9a
    char unknown_9e[0x110 - 0x9e];
    unsigned int flags_110;            // +0x110
    char unknown_114[4];
    void FUN_0048b090(unsigned char, int);
};

class Class_0046d500 {
public:
    void FUN_0046d500(void*, unsigned char);
};

struct Settings {
    int field_471;                     // +0x471
    int flags_475;                     // +0x475
    char unknown_479[0x48];
};

struct Game {
    char unknown_0[0x14];
    char field_14[0x471 - 0x14];       // +0x14
    Settings settings;                 // +0x471
    char unknown_4c1[0x4c9 - 0x4c1];
    int from_id;                       // +0x4c9
    int local_id;                      // +0x4cd
    char unknown_4d1[0x1b63 - 0x4d1];
    Player players[10];                // +0x1b63
    char unknown_2851[0x29a4 - 0x2851];
    int field_29a4[11];                // +0x29a4
    int field_29d0[11];                // +0x29d0
    char unknown_29fc[0x2a30 - 0x29fc];
    Class_0046d500* field_2a30;        // +0x2a30
    char unknown_2a34[4];
    unsigned char* packet;             // +0x2a38
    char unknown_2a3c[6];
    unsigned char local;               // +0x2a42
    char unknown_2a43;
    unsigned char flags_2a44;          // +0x2a44
    char unknown_2a45[0x2be3 - 0x2a45];
    char password[0x2bee - 0x2be3];    // +0x2be3
    unsigned short dirty : 1;          // +0x2bee
    unsigned short : 15;
    char unknown_2bf0[0x2c28 - 0x2bf0];
    char field_2c28[40];               // +0x2c28
    char unknown_2c50[0x2cf3 - 0x2c50];
    Feature features[256];             // +0x2cf3
    char unknown_after_features[0x14357 - (0x2cf3 + 0x115 * 256)];
    Class_0048b090* units;             // +0x14357
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

struct Packet {
    unsigned int type;                 // +0x00
    int field_4;                       // +0x04
    int id;                            // +0x08
    char* field_c;                     // +0x0c
    char* field_10;                    // +0x10
    int field_14;                      // +0x14
    char* field_18;                    // +0x18
};

#pragma pack(pop)

class Class_004618a0 {
public:
    void FUN_004618a0(int);
};

class Class_00461620 {
public:
    void FUN_00461620(int, int, int);
};

class Class_00463be0 {
public:
    char data[0x14b];
    Class_00463be0();
};

class Class_00463c40 {
public:
    void FUN_00463c40();
};

class Class_00463c60 {
public:
    void FUN_00463c60(int);
};

class Class_00456030 {
public:
    int FUN_00456030();
};

extern Game* g_game;
extern char DAT_005119b8[];
extern int DAT_00512bc0[];
extern int DAT_00506dbc;
extern Class_004618a0 DAT_00513000;
extern char DAT_00505dc4[];
extern char DAT_005065c4[];
extern char DAT_0050658c[];
extern char DAT_00506290[];

int FUN_004534e0();
void FUN_00450980();
void FUN_00453c20();
int FUN_004b6340();
void FUN_00450530();
void FUN_00446fb0();
int __stdcall FUN_0044ffd0(unsigned char);
unsigned char __stdcall FUN_0044fe40(int);
int __stdcall FUN_00450a10(int);
int __stdcall FUN_00453010(int, int);
void __stdcall FUN_00451090(char*, int*, int*, int*, int*);
void __stdcall FUN_004c9890(void*, char*, char*, int, int, int, int);
void __stdcall FUN_004565a0(void*);
void __stdcall FUN_00463ca0(void*, int, int, unsigned char);
int __stdcall FUN_00451bc0(int, int, void*, int);
int __stdcall FUN_00451df0(int, void*, int);
void __stdcall FUN_00452bd0(Player*);
void __stdcall FUN_00452cc0(int);
int __stdcall FUN_00452570(int, int);
void __stdcall FUN_004523e0(int, int, int);
void __stdcall FUN_00452960(int, int, unsigned char, int);
char* __stdcall FUN_004c5740(char*);
void __stdcall FUN_0047f1a0(char*, int);
void __stdcall FUN_004861d0(unsigned char, void*);
void __stdcall FUN_0048b920(Player*, void*);
void __stdcall FUN_0048ab70(void*);
void __stdcall FUN_00489ce0(void*);
void __stdcall FUN_004866d0(void*, int);
void __stdcall FUN_0049d270(Player*, void*);
void __stdcall FUN_0049af90(Player*, void*);
void __stdcall FUN_00423550(int, int, int);
void __stdcall FUN_004233a0(int, int, int);
int __stdcall FUN_00481550(int, int);
void __stdcall FUN_004244b0(int, int, int, Feature*);
void __stdcall FUN_0041b8d0(Class_0048b090*, Class_0048b090*);
void __stdcall FUN_0047f0c0(int, int);
void __stdcall FUN_0047f300(int, void*, int);
void __stdcall FUN_00488570(Class_0048b090*, Player*, void*);
void __stdcall FUN_00464b30(unsigned char, unsigned char, int, int);
void __stdcall FUN_00464c60(unsigned char, unsigned char, int, int);
void __stdcall FUN_00485420(unsigned char, unsigned char);
void __stdcall FUN_00457540(void*, Player*);
void __stdcall FUN_00490df0(int, int);

static inline int GetPlayerId(unsigned char i)
{
    if (i != 10 && g_game->players[i].state)
        return g_game->players[i].id;
    return -1;
}

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
        if (FUN_0044ffd0(i) == id)
            return i;
    }
    return 10;
}

static inline Player* PlayerById(int id)
{
    if (FindPlayerSlot(id) == 10)
        return 0;
    return &g_game->players[FUN_0044fe40(id)];
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

int FUN_00450030()
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

static inline int InGame(Player* p)
{
    return IsValid(p) && p->index != 10;
}

static inline Player* LocalPlayer()
{
    return &g_game->players[g_game->local];
}

static inline Class_0048b090* UnitAt(unsigned short index)
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
            if (IsValid(p) && p->index != 10 && p->team == team)
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
        FUN_00452cc0(id);
    FUN_00451df0(LocalPlayer()->id, out, 5);
}

static inline int CommandAllowed(unsigned char* bytes)
{
    int mode = g_game->mode;
    if (mode == 5 && (DAT_00512bc0[bytes[0]] & 2))
        return 1;
    if (mode == 6 && (DAT_00512bc0[bytes[0]] & 4))
        return 1;
    if (mode != 5 && mode != 6 && (DAT_00512bc0[bytes[0]] & 1))
        return 1;
    return 0;
}

// FUNCTION: 0x453d40
int FUN_00453d40()
{
    if (!(g_game->flags_2a44 & 1))
        return 0;
    int messages = 0;
    for (unsigned char i = 0; i < 10; i++)
        g_game->players[i].messages = 0;
    unsigned char* packet = g_game->packet;
    int more = 1;
    while (more) {
        more = FUN_004534e0();
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
            Packet* msg = (Packet*)packet;
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
                    FUN_00453010(p->id, 1);
                    FUN_00453010(LocalPlayer()->id, 10);
                    ((Class_00463c60*)p)->FUN_00463c60(0);
                    ((Class_00463c60*)LocalPlayer())->FUN_00463c60(0);
                } else {
                    FUN_00453010(p->id, 1);
                    ((Class_00463c60*)p)->FUN_00463c60(0);
                }
                g_game->dirty = 1;
                if (LocalPlayer()->info->flags_97 & 1) {
                    char name[32];
                    int a, b, c, d;
                    FUN_00451090(name, &d, &c, &b, &a);
                    if (LocalPlayer()->info->b9b.bit4)
                        g_game->settings.flags_475 |= 0x20;
                    FUN_004c9890(g_game->field_14, name, DAT_005119b8, d, c, b, a);
                }
                break;
            }
            case 3: {
                if (!FUN_00450a10(msg->id))
                    break;
                PlayerById(msg->id);
                int target = FindPlayerIndex(msg->id);
                if (!((Class_00456030*)&g_game->players[FindHost()])->FUN_00456030())
                    break;
                PlayerInfo* info = LocalPlayer()->info;
                char* payload = msg->field_10;
                if (info->w9b.bit15) {
                    FUN_00453010(GetPlayerId(target), 3);
                    break;
                }
                if (msg->field_14 != 0x15) {
                    FUN_00453010(GetPlayerId(target), 8);
                    break;
                }
                if (*(short*)(payload + 0x11) != 0 || *(short*)(payload + 0x13) != 0x50) {
                    FUN_00453010(GetPlayerId(target), 8);
                    break;
                }
                if (!(info->flags_9d & 1))
                    break;
                if (payload && !_strcmpi(g_game->password, payload))
                    break;
                FUN_00453010(GetPlayerId(target), 4);
                break;
            }
            case 0x102: {
                if (msg->field_4 != 1)
                    break;
                Class_00463be0 temp;
                Player* p = PlayerById(msg->id);
                if (p) {
                    int target = FindPlayerIndex(msg->id);
                    if (target == 10) {
                        ((Class_00463c40*)&temp)->FUN_00463c40();
                        continue;
                    }
                    char* payload = msg->field_c;
                    memcpy(g_game->players[target].info, payload, 0xb9);
                    if (FindHost() == g_game->local && !(LocalPlayer()->info->flags_9b & 0x80)
                        && (payload[0x9b] & 0x40))
                        FUN_00453010(p->id, 9);
                }
                ((Class_00463c40*)&temp)->FUN_00463c40();
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
            FUN_00453010(sender, 6);
            continue;
        }
        // Original bug: a command byte is never <= 1 and >= 0x2d at once, so
        // this error reply is dead (`||` was meant; docs/bugs.md).
        if (packet[0] <= 1 && packet[0] >= 0x2d) {
            FUN_00453010(sender, 6);
            continue;
        }
        if (player->state != 1 && player->state != 2 && player->state != 3)
            continue;
        if (!InGame(recipient))
            continue;
        player->last_time = FUN_004b6340();
        player->messages++;
        switch (packet[0]) {
        case 32: {
            int target = FindPlayerIndex(*(int*)(packet + 0x91));
            if (target == 10)
                break;
            if (g_game->players[target].active && g_game->players[target].state == 3) {
                memcpy(g_game->players[target].info, packet + 1, 0xb9);
                FUN_00450980();
            }
            break;
        }
        case 23:
            if (LocalPlayer()->info->flags_97 & 1) {
                if (!FUN_00452570(g_game->from_id, (signed char)packet[1])) {
                    FUN_004523e0(FirstJoinedId(), g_game->from_id, (signed char)packet[1]);
                } else {
                    unsigned char reply[2];
                    reply[0] = 0x18;
                    reply[1] = packet[1];
                    FUN_00451bc0(FirstJoinedId(), g_game->from_id, reply, 2);
                    if (DAT_00506dbc)
                        DAT_00513000.FUN_004618a0(1);
                }
            }
            break;
        case 24:
            g_game->players[to].info->field_96 = packet[1];
            if (to == g_game->local && (g_game->flags_2a44 & 1)) {
                for (int i = 0; i < 10; i++) {
                    Player* p = &g_game->players[i];
                    unsigned char reply[0xba];
                    if (IsConnected(p)) {
                        FUN_00451df0(p->id, InfoPacket(reply, p), 0xba);
                        FUN_00452bd0(p);
                    }
                }
                FUN_00450530();
                DAT_00513000.FUN_004618a0(1);
            }
            g_game->dirty = 1;
            break;
        case 2:
            FUN_004565a0(packet);
            break;
        case 38:
            memcpy(g_game->field_2c28, packet + 1, 40);
            g_game->dirty = 1;
            break;
        case 35: {
            Player* a = PlayerBySlot(*(int*)(packet + 1));
            Player* b = PlayerBySlot(*(int*)(packet + 5));
            if (!a || !b)
                break;
            if (packet[9])
                FUN_0047f1a0(DAT_00505dc4, 0);
            if (IsConnected(b)) {
                FUN_00452960(*(int*)(packet + 1), *(int*)(packet + 5), packet[9],
                             *(int*)(packet + 10));
                if (!(g_game->flags_2a44 & 4))
                    g_game->dirty = 1;
                else
                    FUN_00446fb0();
            }
            g_game->players[a->index].allies[b->index] = packet[9];
            break;
        }
        case 36: {
            Player* p = PlayerBySlot(*(int*)(packet + 1));
            if (p)
                p->field_13f = packet[5];
            if (!(g_game->flags_2a44 & 4))
                g_game->dirty = 1;
            break;
        }
        case 27: {
            Player* p = PlayerBySlot(*(int*)(packet + 1));
            if (p)
                FUN_00453010(p->id, packet[5]);
            break;
        }
        case 28: {
            int id = *(int*)(packet + 1);
            if (FindPlayerIndex(id) == 10)
                break;
            char text[200];
            sprintf(text, FUN_004c5740(DAT_005065c4), PlayerBySlot(id)->name);
            FUN_00463ca0(text, 4, 0, from);
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
            recipient->field_147 = packet[1];
            *(int*)(reply + 1) = recipient->id;
            FUN_00451bc0(recipient->id, player->id, reply, 5);
            break;
        }
        case 31: {
            int target = FindPlayerIndex(*(int*)(packet + 1));
            if (target != 10)
                g_game->field_29d0[target] = 1;
            break;
        }
        case 5:
            if (recipient->active && recipient->state == 1)
                FUN_00463ca0(packet + 1, 8, 0, from);
            break;
        case 39: {
            Player* p = PlayerBySlot(*(int*)(packet + 1));
            if (!p)
                break;
            from = p->index;
            char text[256];
            sprintf(text, DAT_00506290, p->name, FUN_004c5740(DAT_0050658c));
            for (int i = 0; i < 12; i++)
                FUN_00463ca0(text, 8, 0, from);
            break;
        }
        case 6: {
            unsigned char reply = 7;
            FUN_00451bc0(FirstConnectedId(), g_game->from_id, &reply, 1);
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
            FUN_004861d0(from, packet);
            break;
        case 44:
            FUN_0048b920(player, packet);
            break;
        case 10:
            FUN_0048ab70(packet);
            break;
        case 11:
            FUN_00489ce0(packet);
            break;
        case 12:
            FUN_004866d0(packet, 0);
            break;
        case 13:
            FUN_0049d270(player, packet);
            break;
        case 14:
            FUN_0049af90(player, packet);
            break;
        case 15:
            switch (packet[1]) {
            case 0xfd:
                FUN_00423550(*(unsigned short*)(packet + 2), *(unsigned short*)(packet + 4), 0);
                break;
            case 0xfe:
                FUN_004233a0(*(unsigned short*)(packet + 2), *(unsigned short*)(packet + 4), 1);
                break;
            case 0xff:
                FUN_00423550(*(unsigned short*)(packet + 2), *(unsigned short*)(packet + 4), 1);
                break;
            default:
                FUN_004244b0(FUN_00481550(*(unsigned short*)(packet + 2), *(unsigned short*)(packet + 4)),
                             *(unsigned short*)(packet + 2), *(unsigned short*)(packet + 4),
                             &g_game->features[packet[1]]);
                break;
            }
            break;
        case 16: {
            Class_0048b090* unit = UnitAt(*(unsigned short*)(packet + 1));
            if (unit->flags_110 & 0x10000000)
                unit->field_9a->FUN_004b0b00(*(short*)(packet + 3), 0, 0, packet[5],
                                             *(int*)(packet + 6), *(int*)(packet + 10),
                                             *(int*)(packet + 14), *(int*)(packet + 18));
            break;
        }
        case 17: {
            Class_0048b090* unit = UnitAt(*(unsigned short*)(packet + 1));
            if (unit->flags_110 & 0x10000000) {
                unit->FUN_0048b090(packet[3], 1);
                unit->FUN_0048b090(~packet[3], 0);
            }
            break;
        }
        case 18: {
            Class_0048b090* a = UnitAt(*(unsigned short*)(packet + 1));
            FUN_0041b8d0(UnitAt(*(unsigned short*)(packet + 3)), a);
            break;
        }
        case 19:
            if (packet[1])
                FUN_0047f0c0(*(int*)(packet + 2), 0);
            else
                FUN_0047f300(*(int*)(packet + 2), packet + 6, 0);
            break;
        case 20: {
            Class_0048b090* unit = UnitAt(*(unsigned short*)(packet + 1));
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
                FUN_00488570(unit, p, packet);
            break;
        }
        case 21:
            if (g_game->net_bits.net_bit2)
                g_game->field_29a4[from] = 1;
            break;
        case 22: {
            unsigned char a = FindPlayerIndex(*(int*)(packet + 5));
            unsigned char b = FindPlayerIndex(*(int*)(packet + 9));
            if (a == 10 || b == 10)
                break;
            switch (*(int*)(packet + 1)) {
            case 1:
                FUN_00464b30(a, b, *(int*)(packet + 13), 0);
                break;
            case 2:
                FUN_00464c60(a, b, *(int*)(packet + 13), 0);
                break;
            case 3:
                FUN_00485420(a, b);
                break;
            }
            break;
        }
        case 40:
            FUN_00457540(packet, player);
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
                FUN_00490df0(packet[2], 0);
            else
                g_game->bit_38a51 = packet[2];
            break;
        case 26:
            if (g_game->field_2a30 && recipient->active && recipient->state == 1)
                g_game->field_2a30->FUN_0046d500(packet, from);
            break;
        case 29:
            if (DAT_00506dbc)
                ((Class_00461620*)&DAT_00513000)
                    ->FUN_00461620(g_game->from_id, *(int*)(packet + 1), *(int*)(packet + 5));
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
                reply[5] = a->team;
                if (!reply[5])
                    break;
                FUN_00451df0(FUN_00450030(), reply, 6);
            } else {
                if (b) {
                    *(int*)(reply + 1) = a->id;
                    if (InGame(b))
                        reply[5] = b->team;
                } else {
                    *(int*)(reply + 1) = a->id;
                    if (!a->team)
                        reply[5] = FreeTeam();
                }
                if (!reply[5])
                    break;
                a->team = reply[5];
                FUN_00451df0(FUN_00450030(), reply, 6);
            }
            break;
        }
        case 34: {
            Player* p = PlayerByIndex(*(int*)(packet + 1));
            if (p)
                p->team = packet[5];
            break;
        }
        case 42:
            player->field_20 = packet[1];
            break;
        }
    }
    FUN_00450980();
    FUN_00453c20();
    return messages;
}
