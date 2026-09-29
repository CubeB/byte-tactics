// Decompiled by deepseek-v4.1-flash. Names are provisional.
// The 0xbc-byte frame is `Msg_004437c0 msg;`: the group of four dwords is
// copied out of g_game+0x2b6e at msg+0x99, so the leading pad is real and the
// trailing pad keeps the struct at 0xbc exactly. The compatible-version test
// needs `int ver` (zero-extended by the `& 0xff`) to stay a signed compare,
// `jg`/`jl` rather than `ja`/`jb`.
// Handler for the SELGAME (multiplayer game list) screen. Processes the
// UPDATE / PREVMENU / WATCH / JOINGAME / STARTNEW buttons and the per-entry
// "compatible version" check. param_1 is &g_game->sub (g_game+0x519); its
// +0x18 field is the widget created by FUN_004aa8f0, whose +4 is the GUI entry
// table and whose +0x60 is the id of the pressed entry.
#include <windows.h>
#include <string.h>

#pragma pack(push, 1)
struct Entry_004437c0 {                // 0x15b bytes
    unsigned char type;                // +0x00
    char unknown_1[0xb6 - 1];
    short count;                       // +0xb6
    char unknown_b8[0xba - 0xb8];
    short selected;                    // +0xba
    char unknown_bc[0xc2 - 0xbc];
    char* text;                        // +0xc2
    char unknown_c6[0x15b - 0xc6];
};

struct Holder_004437c0 {
    int unknown_0;                     // +0x0
    Entry_004437c0* entries;           // +0x4
    int unknown_8;                     // +0x8
    int field_c;                       // +0xc
};

struct PlayerData_004437c0 {
    char unknown_0[0x80];
    char buf[0x1b];                    // +0x80
    unsigned short flags;              // +0x9b
    char unknown_9d[0x40];
};

struct Player_004437c0 {               // 0x14b bytes
    char unknown_0[0x27];
    PlayerData_004437c0* data;         // +0x27
    char unknown_2b[0x14b - 0x2b];
};

struct Desc_004437c0 {                 // 0x54 bytes
    char unknown_0[0x4c];
    int size;                          // +0x4c
    int offset;                        // +0x50
};

struct Sub_004437c0 {
    char unknown_0[0x18];
    Holder_004437c0* holder;           // +0x18
    char unknown_1c[0x60 - 0x1c];
    int field_60;                      // +0x60
};

struct Info_004437c0 {                 // 16 bytes copied from g_game+0x2b6e
    unsigned int a;
    unsigned int b;
    unsigned int c;
    unsigned int d;
};

struct Msg_004437c0 {                  // 0xbc bytes
    char pad_0[0x99];
    Info_004437c0 group;               // +0x99
    char pad_1[0x13];
};

struct Game_004437c0 {
    char unknown_0[1];
    signed char version;               // +0x1
    char unknown_2[0x1b63 - 2];
    Player_004437c0 players[8];        // +0x1b63
    char unknown_25e2[0x2a42 - (0x1b63 + 8 * 0x14b)];
    unsigned char cur_conn;            // +0x2a42
    char unknown_2a43[1];
    unsigned char field_2a44;          // +0x2a44
    char unknown_2a45[0x2a4b - 0x2a45];
    void* data[0xf];                   // +0x2a4b
    char unknown_2a87[0x2aa7 - 0x2a87];
    Desc_004437c0* desc;               // +0x2aa7
    char* shared;                      // +0x2aab
    char unknown_2aaf[0x2b6a - 0x2aaf];
    char game_info[0x54];              // +0x2b6a
    char unknown_2bbe[0x2bc0 - 0x2bbe];
    unsigned char field_2bc0;          // +0x2bc0
    char unknown_2bc1[0x2bd2 - 0x2bc1];
    char nickname[0x11];               // +0x2bd2
    char password[0x40];               // +0x2be3
};
#pragma pack(pop)

// GLOBAL: 0x511de8
extern Game_004437c0* g_game;
extern unsigned char DAT_00512d90;
extern int DAT_00512c84;

int __stdcall FUN_0049fdf0(Entry_004437c0* entries, const char* name, int type);
Entry_004437c0* __stdcall FUN_0049ff90(Entry_004437c0* entries, const char* name);
char* __stdcall FUN_004a0010(Entry_004437c0* entries, const char* name);
void __stdcall FUN_0047f1a0(char* str, int flag);
int __stdcall FUN_00441460(void* holder);
void __stdcall FUN_0049fa90(Sub_004437c0* sub);
void __stdcall FUN_004ab0a0(Sub_004437c0* sub);
void __cdecl FUN_004d85a0(void* p);
void __stdcall FUN_004a0d00(Sub_004437c0* sub, const char* name, char* text);
void __stdcall FUN_00425730(char* msg);
void FUN_004257a0();
void FUN_00441080();
void __stdcall FUN_004a9660(Sub_004437c0* sub);
void __stdcall FUN_004a7190(Sub_004437c0* sub, char* text);
char* __stdcall FUN_004c5740(char* text);
void __stdcall FUN_004abd90(Sub_004437c0* sub, char* text, int a, int b, int c);
unsigned char FUN_00456850();
int __stdcall FUN_0049fd60(Sub_004437c0* sub, const char* name);

// FUNCTION: 0x4437c0
void __stdcall FUN_004437c0(Sub_004437c0* param_1)
{
    Entry_004437c0* entries = param_1->holder->entries;
    int i;
    int id;
    int cur;

    if (DAT_00512d90 != 0) {
        DAT_00512d90 = 0;
        if (DAT_00512c84 != 0)
            goto startnew;
    }

    if (param_1->field_60 == -1) {
        for (i = 0; i < 0xf; i++) {
            FUN_004d85a0(g_game->data[i]);
            g_game->data[i] = 0;
        }
        FUN_004d85a0(g_game->shared);
        FUN_004d85a0(g_game->desc);
        g_game->shared = 0;
        g_game->desc = 0;
        return;
    }

    if (FUN_0049fdf0(entries, "UPDATE", 0xe) == param_1->field_60) {
        char* pass = FUN_004a0010(entries, "PASSWORD");
        if (pass != 0) {
            cur = g_game->cur_conn;
            strcpy((char*)g_game->players[cur].data + 0x80, pass + 0xb6);
        }
        FUN_0047f1a0("Multi", 0);
        FUN_00441460(param_1->holder);
        FUN_0049fa90(param_1);
        FUN_004ab0a0(param_1);
        return;
    }

    if (FUN_0049fdf0(entries, "PREVMENU", 0xe) == param_1->field_60) {
        g_game->field_2bc0 = 3;
        FUN_0047f1a0("Previous", 0);
        return;
    }

    if (FUN_0049fdf0(entries, "WATCH", 0xe) == param_1->field_60 ||
        FUN_0049fdf0(entries, "JOINGAME", 0xe) == param_1->field_60 ||
        entries[param_1->field_60].type == 2) {
        Entry_004437c0* e = FUN_0049ff90(entries, "GAMENAME");
        Msg_004437c0 msg;
        unsigned int flags;
        int ver;
        char* pass;
        unsigned int b;

        *(Desc_004437c0*)g_game->game_info = g_game->desc[e->selected];
        msg.group = *(Info_004437c0*)((char*)g_game + 0x2b6e);
        flags = *(unsigned int*)((char*)&msg.group + 2);
        ver = *(unsigned int*)((char*)&msg.group + 0xe) & 0xff;

        if (ver <= (int)g_game->version && ver >= (int)g_game->version) {
            if ((flags & 0x8000) != 0 || (flags & 0x10) != 0) {
                FUN_0047f1a0("Previous", 0);
                FUN_004ab0a0(param_1);
                return;
            }
            FUN_004a0d00(param_1, "NICKNAME", g_game->nickname);
            if (strlen(g_game->nickname) == 0) {
                FUN_004a7190(param_1, (char*)FUN_0049fdf0(entries, "NICKNAME", 3));
                FUN_004ab0a0(param_1);
                FUN_004abd90(param_1, FUN_004c5740("You must enter your name"), 0xc8, 1, 1);
                return;
            }
            pass = FUN_004a0010(entries, "PASSWORD");
            lstrcpynA(g_game->password, pass + 0xb6, 0xb);
            cur = g_game->cur_conn;
            lstrcpynA((char*)g_game->players[cur].data + 0x80, pass + 0xb6, 0xb);
            b = FUN_00456850();
            if (b != 0xa &&
                (*(unsigned short*)((char*)g_game->players[b].data + 0x9b) & 0x10) == 0x10)
                g_game->field_2a44 |= 4;
            if (FUN_0049fd60(param_1, "WATCH")) {
                cur = g_game->cur_conn;
                *(unsigned short*)((char*)g_game->players[cur].data + 0x9b) |= 0x40;
                FUN_0047f1a0("Multi", 0);
                g_game->field_2bc0 = 0x13;
                return;
            }
            cur = g_game->cur_conn;
            *(unsigned short*)((char*)g_game->players[cur].data + 0x9b) &= 0xffbf;
            FUN_0047f1a0("BigButton", 0);
            g_game->field_2bc0 = 0x12;
            return;
        }
        FUN_00425730("You do not have a compatible version for this game.");
    } else if (FUN_0049fdf0(entries, "STARTNEW", 0xe) == param_1->field_60) {
startnew:
        cur = g_game->cur_conn;
        *(unsigned short*)((char*)g_game->players[cur].data + 0x9b) &= 0xffbf;
        FUN_0047f1a0("BigButton", 0);
        FUN_004a0d00(param_1, "NICKNAME", g_game->nickname);
        FUN_004257a0();
        FUN_004a9660(param_1);
        FUN_00441080();
        FUN_004ab0a0(param_1);
        return;
    }
    FUN_004ab0a0(param_1);
}
