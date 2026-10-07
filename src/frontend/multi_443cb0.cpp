// Decompiled by Space Bunny Free, finished by deepseek-v4.1-flash. Names are provisional.
// Opens the network game selection dialog (SELGAME.GUI) and sets up the
// buffers it needs: a 0x690 byte "GAME DESCRIPTIONS" block, a 0xe74 byte
// "PLAYER SHARED" block and 15 blocks of 0xa00 bytes named "DATA0" .. "DATA14",
// then a table of (size, offset) pairs (0xb9 bytes each) in the descriptions
// block pointing into the shared block. Every GUI entry from 1 up whose type
// byte is 2 gets FUN_00441220 as its handler and the descriptions block as its
// data. ConnectToGame then connects; on failure an "Invalid TCP/IP Address"
// message box is shown, g_game->field_2bc0 is set to 3 and the function
// returns. On success the game name is put on the menu, and a connection that
// came back with an error status (neither 0 nor 2) is reported and cleared.
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

struct Game {
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
extern Game* g_game;
extern unsigned char DAT_00512d90;
extern int DAT_00512c84;

void BlankScreen();
void __stdcall LoadPictureCached(const char* name, int a, int b, int c);
void FUN_00428b60();
int __stdcall ConnectToGame(Gadget_00443cb0* gadget);
void __stdcall HandleSelectGameClick(Sub_00443cb0* sub);
char* __stdcall GetRejectReasonText(int value);
void __stdcall FUN_0049fa90(Sub_00443cb0* sub);
void __stdcall FUN_0049fad0(Sub_00443cb0* sub);
void __stdcall FUN_0049fb10(Sub_00443cb0* sub, int value);
char* __stdcall FindGadgetIndex(Entry_00443cb0* entries, const char* name, int type);
void __stdcall FUN_004a0bf0(Sub_00443cb0* sub, const char* name, const char* text, int len);
void __stdcall FUN_004a1250(Sub_00443cb0* sub, const char* name, int value);
void __stdcall SelectGadgetByIndex(Sub_00443cb0* sub, const char* text);
void __stdcall RenderLayer(Sub_00443cb0* sub, int value);
void __stdcall CloseTopScreen(Sub_00443cb0* sub);
Gadget_00443cb0* __stdcall LoadGuiLayer(Sub_00443cb0* sub, const char* name, int flags);
void __stdcall OpenMessageBox(Sub_00443cb0* sub, const char* text, int a, int b, int c);
char* __stdcall Translate(const char* text);
void* __cdecl FUN_004d83b0(const char* name, unsigned int size);
void __stdcall FUN_00441220(void* gadget);

// FUNCTION: 0x443cb0
void OpenSelectGameDialog()
{
    char name[0x14];
    int i;
    int j;

    BlankScreen();
    Gadget_00443cb0* gadget = LoadGuiLayer(&g_game->sub, "SELGAME.GUI", 0x80);
    gadget->handler = HandleSelectGameClick;
    gadget->field_c = (int)g_game;
    LoadPictureCached("selectgame2x", 0, 0, 0);
    g_game->desc = (Desc_00443cb0*)FUN_004d83b0("GAME DESCRIPTIONS", 0x690);
    g_game->shared = (char*)FUN_004d83b0("PLAYER SHARED", 0xe74);
    for (i = 0; i < 0xf; i++) {
        sprintf(name, "DATA%d", i);
        g_game->data[i] = FUN_004d83b0(name, 0xa00);
    }
    // Offset is j * 0xb9, no separate off counter.
    for (j = 0; j < 20; j++) {
        g_game->desc[j].size = 0xb9;
        g_game->desc[j].offset = (int)(g_game->shared + j * 0xb9);
    }
    FUN_004a0bf0(&g_game->sub, "PASSWORD", g_game->password, 10);
    FUN_004a0bf0(&g_game->sub, "NICKNAME", g_game->nickname, 10);
    FUN_004a1250(&g_game->sub, "JOIN", 1);
    FUN_004a1250(&g_game->sub, "WATCH", 1);
    for (i = 1; i < gadget->entries->count; i++) {
        if (gadget->entries[i].type == 2) {
            // Indexed inline and bound by reference: no named entries pointer local.
            Entry_00443cb0& e = gadget->entries[i];
            e.handler = FUN_00441220;
            e.data = (int)g_game->desc;
        }
    }
    RenderLayer(&g_game->sub, 0x40);
    if (!ConnectToGame(gadget)) {
        CloseTopScreen(&g_game->sub);
        OpenMessageBox(&g_game->sub, Translate("Invalid TCP/IP Address"), 0xc8, 1, 1);
        g_game->field_2bc0 = 3;
        return;
    }
    SelectGadgetByIndex(&g_game->sub, FindGadgetIndex(gadget->entries, "GAMENAME", 2));
    FUN_00428b60();
    FUN_0049fb10(&g_game->sub, 1);
    RenderLayer(&g_game->sub, 0x40);
    Conn_00443cb0* conn = &g_game->conns[g_game->cur_conn];
    if (conn->status != 0 && conn->status != 2) {
        RenderLayer(&g_game->sub, 0x40);
        char* msg = GetRejectReasonText(conn->status);
        OpenMessageBox(&g_game->sub, Translate(msg), 0x140, 1, 1);
        FUN_0049fa90(&g_game->sub);
        FUN_0049fad0(&g_game->sub);
        conn->status = 0;
    }
    if (DAT_00512d90 != 0 && DAT_00512c84 != 0) {
        if (strlen(g_game->nickname) != 0)
            HandleSelectGameClick(&g_game->sub);
    }
}
