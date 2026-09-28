// Decompiled by Space Bunny Free. Names are provisional.
// Opens the network game selection dialog (SELGAME.GUI) and sets up the
// buffers it needs: a 0x690 byte "GAME DESCRIPTIONS" block, a 0xe74 byte
// "PLAYER SHARED" block and 15 blocks of 0xa00 bytes named "DATA0" .. "DATA14",
// then a table of (size, offset) pairs (0xb9 bytes each) in the descriptions
// block pointing into the shared block. Every GUI entry from 1 up whose type
// byte is 2 gets FUN_00441220 as its handler and the descriptions block as its
// data. FUN_00441460 then connects; on failure an "Invalid TCP/IP Address"
// message box is shown, g_game->field_2bc0 is set to 3 and the function
// returns. On success the game name is put on the menu, and a connection that
// came back with an error status (neither 0 nor 2) is reported and cleared.
//
// The local buffer is 0x14 bytes, not the 8 the "DATA%d" text needs: the
// frame is 0x14 bytes and both `lea`es of the buffer are computed from that
// one array (MSVC 5 emits the second one 8 bytes higher than the first, so at
// run time FUN_004d83b0 is handed a pointer 8 bytes above the text sprintf
// just wrote; harmless, because FUN_004d83b0 ignores its name argument and
// passes only the size on to FUN_004d83c0).
//
// Not a MATCH yet: 98.2% (809 of 824 bytes, same length, every reference
// resolves). What still differs is the SIB base/index order of the three
// entry accesses in the gadget loop (the original has `cmp byte [ecx + eax]`
// where we emit `[eax + ecx]`, with eax the entry table and ecx the index
// times 0x15b) and the order of the two zeroing instructions before the
// description table loop (the original has `xor ecx,ecx; xor eax,eax`, we emit
// them the other way round). The SIB order did not move for: the pointer in a
// local with a second local per iteration, `&e[i]`, `e + i`, a static inline
// helper computing `(unsigned char*)e + i * 0x15b`, a char* base with the
// fields reached through casts, swapping the two stores, and for any
// declaration or initialisation order of the loop's index and offset. The
// zeroing order did not move for any of the same changes either. Both are the
// scheduler tie-break the guide calls compiler state (0x4c1480 against
// 0x4c1760 is the recorded case).
#include <stdio.h>
#include <string.h>

#pragma pack(push, 1)
struct Sub_00443cb0 {
    char unknown_0[0x10];
};

struct Entry_00443cb0 {                // 0x15b bytes
    unsigned char type;                // +0x00
    char unknown_1[0xb6 - 0x1];
    short count;                       // +0xb6 (entry 0 only)
    char unknown_b8[0xce - 0xb8];
    void (__stdcall* handler)(void*);  // +0xce
    int data;                          // +0xd2
    char unknown_d6[0x15b - 0xd6];
};

struct Gadget_00443cb0 {
    int unknown_0;
    Entry_00443cb0* entries;           // +0x4
    void (__stdcall* handler)(Sub_00443cb0*);  // +0x8
    int field_c;                       // +0xc
};

struct Desc_00443cb0 {                 // 0x54 bytes
    char unknown_0[0x4c];
    int size;                          // +0x4c
    int offset;                        // +0x50
};

struct Conn_00443cb0 {                 // 0x14b bytes
    char unknown_0[0x22];
    unsigned char status;              // +0x22
    char unknown_23[0x14b - 0x23];
};

struct Game_00443cb0 {
    char unknown_0[0x519];
    Sub_00443cb0 sub;                  // +0x519
    char unknown_529[0x1b63 - 0x529];
    Conn_00443cb0 conns[8];           // +0x1b63
    char unknown_25bb[0x2a42 - 0x25bb];
    unsigned char cur_conn;            // +0x2a42
    char unknown_2a43[0x2a4b - 0x2a43];
    void* data[0xf];                   // +0x2a4b
    char unknown_2a87[0x2aa7 - 0x2a87];
    Desc_00443cb0* desc;               // +0x2aa7
    char* shared;                      // +0x2aab
    char unknown_2aaf[0x2bc0 - 0x2aaf];
    unsigned char field_2bc0;          // +0x2bc0
    char unknown_2bc1[0x2bd2 - 0x2bc1];
    char nickname[0x11];               // +0x2bd2
    char password[0x40];               // +0x2be3
};
#pragma pack(pop)

// GLOBAL: 0x511de8
extern Game_00443cb0* g_game;
extern unsigned char DAT_00512d90;
extern int DAT_00512c84;

void FUN_004257a0();
void __stdcall FUN_004288d0(const char* name, int a, int b, int c);
void FUN_00428b60();
int __stdcall FUN_00441460(Gadget_00443cb0* gadget);
void __stdcall FUN_004437c0(Sub_00443cb0* sub);
char* __stdcall FUN_00452c40(int value);
void __stdcall FUN_0049fa90(Sub_00443cb0* sub);
void __stdcall FUN_0049fad0(Sub_00443cb0* sub);
void __stdcall FUN_0049fb10(Sub_00443cb0* sub, int value);
char* __stdcall FUN_0049fdf0(Entry_00443cb0* entries, const char* name, int type);
void __stdcall FUN_004a0bf0(Sub_00443cb0* sub, const char* name, const char* text, int len);
void __stdcall FUN_004a1250(Sub_00443cb0* sub, const char* name, int value);
void __stdcall FUN_004a7830(Sub_00443cb0* sub, const char* text);
void __stdcall FUN_004a81e0(Sub_00443cb0* sub, int value);
void __stdcall FUN_004a9660(Sub_00443cb0* sub);
Gadget_00443cb0* __stdcall FUN_004aa8f0(Sub_00443cb0* sub, const char* name, int flags);
void __stdcall FUN_004abd90(Sub_00443cb0* sub, const char* text, int a, int b, int c);
char* __stdcall FUN_004c5740(const char* text);
void* __cdecl FUN_004d83b0(const char* name, unsigned int size);
void __stdcall FUN_00441220(void* gadget);

// FUNCTION: 0x443cb0
void FUN_00443cb0()
{
    char name[0x14];
    int i;
    int j;
    int off;

    FUN_004257a0();
    Gadget_00443cb0* gadget = FUN_004aa8f0(&g_game->sub, "SELGAME.GUI", 0x80);
    gadget->handler = FUN_004437c0;
    gadget->field_c = (int)g_game;
    FUN_004288d0("selectgame2x", 0, 0, 0);
    g_game->desc = (Desc_00443cb0*)FUN_004d83b0("GAME DESCRIPTIONS", 0x690);
    g_game->shared = (char*)FUN_004d83b0("PLAYER SHARED", 0xe74);
    for (i = 0; i < 0xf; i++) {
        sprintf(name, "DATA%d", i);
        g_game->data[i] = FUN_004d83b0(name, 0xa00);
    }
    for (j = 0, off = 0; off < 0xe74; j++) {
        g_game->desc[j].size = 0xb9;
        g_game->desc[j].offset = (int)(g_game->shared + off);
        off += 0xb9;
    }
    FUN_004a0bf0(&g_game->sub, "PASSWORD", g_game->password, 10);
    FUN_004a0bf0(&g_game->sub, "NICKNAME", g_game->nickname, 10);
    FUN_004a1250(&g_game->sub, "JOIN", 1);
    FUN_004a1250(&g_game->sub, "WATCH", 1);
    Entry_00443cb0* e = gadget->entries;
    for (i = 1, e = gadget->entries; i < e->count; e = gadget->entries, i++) {
        if (e[i].type == 2) {
            e[i].handler = FUN_00441220;
            e[i].data = (int)g_game->desc;
        }
    }
    FUN_004a81e0(&g_game->sub, 0x40);
    if (!FUN_00441460(gadget)) {
        FUN_004a9660(&g_game->sub);
        FUN_004abd90(&g_game->sub, FUN_004c5740("Invalid TCP/IP Address"), 0xc8, 1, 1);
        g_game->field_2bc0 = 3;
        return;
    }
    FUN_004a7830(&g_game->sub, FUN_0049fdf0(gadget->entries, "GAMENAME", 2));
    FUN_00428b60();
    FUN_0049fb10(&g_game->sub, 1);
    FUN_004a81e0(&g_game->sub, 0x40);
    Conn_00443cb0* conn = &g_game->conns[g_game->cur_conn];
    if (conn->status != 0 && conn->status != 2) {
        FUN_004a81e0(&g_game->sub, 0x40);
        char* msg = FUN_00452c40(conn->status);
        FUN_004abd90(&g_game->sub, FUN_004c5740(msg), 0x140, 1, 1);
        FUN_0049fa90(&g_game->sub);
        FUN_0049fad0(&g_game->sub);
        conn->status = 0;
    }
    if (DAT_00512d90 != 0 && DAT_00512c84 != 0) {
        if (strlen(g_game->nickname) != 0)
            FUN_004437c0(&g_game->sub);
    }
}
