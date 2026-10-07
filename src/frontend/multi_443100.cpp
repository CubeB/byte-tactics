// Decompiled by Space Bunny Free, finished by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
#include <string.h>

// DirectX 5's DPERR_BUFFERTOOSMALL, MAKE_DPHRESULT(30).
#define DPERR_BUFFERTOOSMALL_00443100 0x8877001e

// Packed: otherwise menu lands at g_game+0x51c. menu must be a Game member.
#pragma pack(push, 1)

struct Guid_00443100 {
    unsigned long data1;
    unsigned short data2;
    unsigned short data3;
    unsigned char data4[8];
};

// The DirectPlay interfaces as laid out by the HAPINET_ wrappers.
struct Net_00443100 {
    void* dp;                          // +0x0
    void* dp3;                         // +0x4
    char unknown_8[0x4cd - 8];
    void* lobby;                       // +0x4cd
};

// 0x102-byte records: a name and a number string.
struct Entry_00443100 {
    char name[0x81];                   // +0x00
    char number[0x81];                 // +0x81
};

// GUI layout entry, as returned by FindGadgetChecked.
struct Layout_00443100 {
    char unknown_0[0xba];
    short player;                      // +0xba
    char unknown_bc[0xce - 0xbc];
    void (__stdcall* callback)(void*, Layout_00443100*);   // +0xce
};

// Object with the layout entry array at +4.
struct Table_00443100 {
    int unknown_0;
    Layout_00443100* entries;           // +0x4
};

// The MODEM.GUI gadget created by LoadGuiLayer.
struct Gadget_00443100 {
    int unknown_0;
    Layout_00443100* entries;           // +0x4
    void (__stdcall* handler)(void*);   // +0x8
    void* owner;                        // +0xc
};

// The multiplayer menu; the layout entry array hangs off it at +0x18.
struct Gui_00443100 {
    char unknown_0[0x18];
};

struct Game {
    char unknown_0[0x14];
    Net_00443100 net;                   // +0x14
    char unknown_4e5[0x519 - 0x4e5];
    Gui_00443100 menu;                  // +0x519
    Table_00443100* table;              // +0x531
};
#pragma pack(pop)

extern Game* g_game;
extern Guid_00443100 DAT_004fcdc8;
extern char* DAT_00512980;
extern int DAT_00512984;
extern Entry_00443100* DAT_00512988;
extern char* DAT_0051298c;

Gadget_00443100* __stdcall LoadGuiLayer(void* gui, const char* name, int flags);
void __stdcall HandleModemDialogClick(void*);
void __stdcall LoadPictureCached(const char* name, int a, int b, int c);
int __stdcall HAPINET_initlobbiedconnection(Net_00443100* net);
int __stdcall HAPINET_createdplayinterface(Guid_00443100* sp, Net_00443100* net);
int __stdcall HAPINET_getplayeraddress(Net_00443100* net, unsigned long player, void* data, unsigned long* size);
int __stdcall HAPINET_enumaddress(Net_00443100* net, void* callback, void* address, unsigned long size, void* context);
int __stdcall HAPINET_releasedplayinterface(Net_00443100* net);
void* __cdecl FUN_004d83b0(const char* name, unsigned int size);
void __cdecl FUN_004d85a0(void* p);
int __stdcall ReadGameRegistryValue(const char* key, void* buf, unsigned int* size);
Layout_00443100* __stdcall FindGadgetChecked(Layout_00443100* entries, char* name);
void __stdcall FUN_004a32a0(void* menu, char* name, char* text, int count, int flag);
void __stdcall FUN_004a2e40(void* menu, char* name, int index);
void __stdcall ShowSelectedAccount(void* menu, Layout_00443100* entry);
void __stdcall CloseTopScreen(void* gui);
void __stdcall FUN_0049fb10(void* gui, int value);
void __stdcall RenderLayer(void* gui, int value);
void __stdcall SetFrontendErrorText(char* text);
void __stdcall SetFrontendState(int state, int line, const char* file);
void __stdcall SetFrontendSubState(char state, int line, char* file);
int __stdcall EnumModemAddressCallback(void* guid, unsigned long size, void* data, void* context);


struct Len { unsigned int v; };

// FUNCTION: 0x443100
void __stdcall OpenModemDialog()
{
    // __stdcall keeps the gadget reload after the push. size is initialised, after addr.
    char* addr = 0;
    unsigned long size = 0;
    Len len;
    // Each HRESULT goes through r before it is compared.
    int r;
    Gadget_00443100* gadget;
    // 8-byte local, not the large Mission type.
    struct { void* dp; void* dp3; } net;
    Guid_00443100 iid = DAT_004fcdc8;

    gadget = LoadGuiLayer(&g_game->menu, "MODEM.GUI", 0x800);
    gadget->handler = HandleModemDialogClick;
    gadget->owner = g_game;
    LoadPictureCached(0, 0, 0, 0);
    HAPINET_initlobbiedconnection(&g_game->net);
    r = HAPINET_createdplayinterface(&iid, (Net_00443100*)&net);
    if (r >= 0) {
        r = HAPINET_getplayeraddress((Net_00443100*)&net, 0, 0, &size);
        if (r == DPERR_BUFFERTOOSMALL_00443100) {
            addr = (char*)FUN_004d83b0("MODEMADDR", size);
            if (addr != 0) {
                r = HAPINET_getplayeraddress((Net_00443100*)&net, 0, addr, &size);
                if (r >= 0) {
                    DAT_00512980 = (char*)FUN_004d83b0("MODEMINFO", 0xc8);
                    memset(DAT_00512980, 0, 0xc8);
                    DAT_00512984 = 0;
                    r = HAPINET_enumaddress(&g_game->net, (void*)EnumModemAddressCallback, addr, size, 0);
                    if (DAT_00512984 == 0) {
                        CloseTopScreen(&g_game->menu);
                        SetFrontendErrorText("Unable to find any modems");
                        SetFrontendState(0xf, 0x4e4, "c:\\cavedog\\wargame\\multi.cpp");
                        SetFrontendSubState(0, 0x4e5, "c:\\cavedog\\wargame\\multi.cpp");
                        FUN_004d85a0(DAT_00512980);
                        FUN_004d85a0(addr);
                        HAPINET_releasedplayinterface((Net_00443100*)&net);
                        return;
                    }
                    if (r >= 0) {
                        int i;
                        FUN_004a32a0(&g_game->menu, "MODEMS", DAT_00512980, DAT_00512984, 0);
                        DAT_00512988 = (Entry_00443100*)FUN_004d83b0("MODEMACCOUNTS", 0x1428);
                        len.v = 0x1428;
                        r = ReadGameRegistryValue("MODEMNUMBERS", DAT_00512988, &len.v);
                        if (r == 0) {
                            for (i = 0; i < 20; i++) {
                                strcpy(DAT_00512988[i].name, "UNUSED");
                                DAT_00512988[i].number[0] = 0;
                            }
                        }
                        char* buffer = (char*)FUN_004d83b0("ACCOUNTNAMES", 0xa00);
                        DAT_0051298c = buffer;
                        *buffer = 0;
                        // Separate p runs the copy loop: keeps eax free until loop entry.
                        char* p = buffer;
                        for (i = 0; i < 20; i++) {
                            strcpy(p, DAT_00512988[i].name);
                            p += strlen(DAT_00512988[i].name) + 1;
                        }
                        Layout_00443100* entry = FindGadgetChecked(g_game->table->entries, "ACCOUNTS");
                        int player = entry->player;
                        FUN_004a32a0(&g_game->menu, "ACCOUNTS", DAT_0051298c, 20, 0);
                        FUN_004a2e40(&g_game->menu, "ACCOUNTS", player);
                        entry = FindGadgetChecked(gadget->entries, "ACCOUNTS");
                        entry->callback = ShowSelectedAccount;
                        ShowSelectedAccount(&g_game->menu, entry);
                    }
                }
            }
        }
    }
    FUN_0049fb10(&g_game->menu, 1);
    RenderLayer(&g_game->menu, 0x40);
    HAPINET_releasedplayinterface((Net_00443100*)&net);
    FUN_004d85a0(addr);
}
