// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <windows.h>
#include <string.h>

#pragma pack(push, 1)
struct Entry_441080 {
    char unknown_0[0x138];
    unsigned short field_138;          // +0x138
};

struct Layer_441080 {
    int unknown_0;                     // +0x0
    Entry_441080* entries;             // +0x4
    void (__stdcall* handler)(void*);  // +0x8
    void* data;                        // +0xc
};

struct Menu_441080 {
    char unknown_0[0x18];
    Layer_441080* layer;               // +0x18
};

struct PlayerInfo_441080 {
    char unknown_0[0x80];
    char name[1];                      // +0x80
};

struct Player_441080 {                 // 0x14b bytes
    char unknown_0[0x27];
    PlayerInfo_441080* info;           // +0x27
    char unknown_2b[0x14b - 0x2b];
};

struct Game {
    char unknown_0[0x519];
    Menu_441080 menu;                  // +0x519
    char unknown_535[0x1b63 - 0x535];
    Player_441080 players[10];         // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;         // +0x2a42
    char unknown_2a43[0x2bc1 - 0x2a43];
    char gameName[0x11];               // +0x2bc1
    char nickName[0x11];               // +0x2bd2
    char passWord[0x11];               // +0x2be3
};
#pragma pack(pop)

extern Game* g_game;
extern char DAT_00512d48;

void __stdcall HandleNewMultiClick(void* gadget);
Layer_441080* __stdcall LoadGuiLayer(Menu_441080* menu, const char* name, int flags);
void __stdcall LoadPictureCached(const char* name, int a, int b, int c);
int IsOnlineConfigLoaded(void);
Entry_441080* __stdcall FUN_004a0010(Entry_441080* entries, char* name);
void __stdcall FUN_004a0bf0(Menu_441080* menu, char* name, char* text, int param_4);
void FUN_00428b60(void);
void __stdcall FUN_0049fb10(Menu_441080* menu, int value);
void __stdcall RenderLayer(Menu_441080* menu, int value);

// FUNCTION: 0x441080
void OpenNewMultiDialog()
{
    DWORD size;
    Layer_441080* layer = LoadGuiLayer(&g_game->menu, "NEWMULTI.GUI", 0x80);
    layer->handler = HandleNewMultiClick;
    layer->data = g_game;
    LoadPictureCached("createnew", 0, 0, 0);
    Entry_441080* entries = layer->entries;
    if (IsOnlineConfigLoaded() && DAT_00512d48 != 0) {
        g_game->nickName[0] = 0;
        strncat(g_game->nickName, &DAT_00512d48, 0x10);
    }
    if (strlen(g_game->nickName) == 0) {
        size = 0x11;
        GetUserNameA(g_game->nickName, &size);
    }
    Entry_441080* gname = FUN_004a0010(entries, "GAMENAME");
    FUN_004a0bf0(&g_game->menu, "GAMENAME", g_game->gameName, 0);
    gname->field_138 = 0x10;
    Entry_441080* nname = FUN_004a0010(entries, "NICKNAME");
    FUN_004a0bf0(&g_game->menu, "NICKNAME", g_game->nickName, 0);
    nname->field_138 = 0x10;
    char* pw = g_game->players[g_game->localPlayer].info->name;
    if (strlen(pw) == 0)
        pw = g_game->passWord;
    FUN_004a0bf0(&g_game->menu, "PASSWORD", pw, 0xa);
    FUN_00428b60();
    FUN_0049fb10(&g_game->menu, 1);
    RenderLayer(&g_game->menu, 0x40);
}
