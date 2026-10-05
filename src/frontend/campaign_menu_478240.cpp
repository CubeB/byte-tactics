// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Sets up the NEWGAME.GUI menu: picks the campaign list or the play-any-game
// entry, sizes the Campaign / CampaignKnob / Missions gadgets, clears the two
// side GAF lists, selects the difficulty, fills the Campaign and (for a
// campaign) Missions lists, then switches the visible pane.
//
// Suspected original bug, kept exactly: the "Campaign" gadget lookup at
// 0x478514 stores its result in eax, tests eax for null at 0x478519, and then
// dereferences [eax+0xce] on BOTH branches (0x478525 and 0x478531), so a
// null return would still be written through.
#include <string.h>

#pragma pack(push, 1)
struct Unit {
    char unknown_0[0x95];
    unsigned char side;                // +0x95
};

struct PlayerEntry_00478240 {          // 0x14b bytes
    Unit* unit;                        // +0x0
    char unknown_4[0x14b - 4];
};

struct Entry_00478240 {                // 0x15b bytes
    char unknown_0[0x15];
    short y;                           // +0x15
    char unknown_17[2];
    short height;                      // +0x19
    char unknown_1b[0xba - 0x1b];
    short line;                        // +0xba
    char unknown_bc[0xc2 - 0xbc];
    char* text;                        // +0xc2
    char unknown_c6[0xce - 0xc6];
    void (__stdcall* callback)();      // +0xce
    char unknown_d2[0x137 - 0xd2];
    unsigned char difficulty;          // +0x137
    char unknown_138[0x15b - 0x138];
};

// The same record viewed through its entry-0 gadget data, where +0xc0 is the
// GAF handle (the +0xc2 text pointer of the layout above overlaps it).
struct GafEntry_00478240 {
    char unknown_0[0xc0];
    void* gaf;                         // +0xc0
};

struct Layer_00478240 {
    int unknown_0;                     // +0x0
    Entry_00478240* entries;           // +0x4
    void (__stdcall* handler)();       // +0x8
    void* data;                        // +0xc
};

struct Menu_00478240 {
    char unknown_0[0x18];
    Layer_00478240* layer;             // +0x18
};

struct Game {
    char unknown_0[0x519];
    Menu_00478240 menu;                // +0x519
    char unknown_535[0x1b8a - 0x535];
    PlayerEntry_00478240 players[10];  // +0x1b8a
    char unknown_2878[0x2a42 - 0x2878];
    unsigned char localPlayer;         // +0x2a42
    char unknown_2a43[0x37eee - 0x2a43];
    int field_37eee;                   // +0x37eee
    char unknown_37ef2[0x391e9 - 0x37ef2];
    void* net;                         // +0x391e9
};
#pragma pack(pop)

extern Game* g_game;
extern int DAT_0051e668;
extern int* DAT_0051e65c;
extern int* DAT_0051e660;
extern int DAT_00507b6c;

void FUN_004257a0();
Layer_00478240* __stdcall LoadGuiLayer(void* menu, const char* name, int flags);
void __stdcall BuildDataPath(char* out, const char* dir, const char* name,
                            const char* ext);
void __stdcall FUN_004288d0(const char* name, int a, int b, int c);
int __stdcall FUN_004bc930(char* name, int flag);
int __stdcall FindGadgetIndex(Entry_00478240* entries, const char* name, int type);
void FUN_004c2470();
void __stdcall RenderLayer(void* menu, int value);
void FUN_004c2870();
void* __stdcall FindGafEntry(void* gaf, const char* name);
int __stdcall GetGafFrame(unsigned short* p, int index);
void FUN_00477360();
Entry_00478240* __stdcall FindGadgetOrNull(Entry_00478240* entries,
                                       const char* name);
void __stdcall SetGadgetStatusByName(void* menu, const char* name, int value);
void __stdcall FUN_0049fa90(void* menu);
void __stdcall FUN_004a0570(void* menu, const char* name, int value);
Entry_00478240* __stdcall FindGadgetChecked(Entry_00478240* entries,
                                       const char* name);
void __cdecl FUN_004d85a0(int* p);
void __stdcall FUN_0047f1a0(const char* name, int value);
int __stdcall FUN_00476a60(int** p, int value);
void __stdcall FUN_004a32a0(void* menu, const char* name, int* data, int count,
                            int flag);
void __stdcall FUN_004a2be0(void* menu, int index);
char* __stdcall FUN_004b6af0(char* text, int line);

class Class_00435110 {
public:
    void LoadCampaign(char* file);
};

class Class_00435760 {
public:
    int BuildMissionList(int* param_1);
};

void __stdcall FUN_004779e0();
void __stdcall FUN_00477ab0();
void __stdcall SelectGadgetByName(void* menu, const char* name);
void __stdcall FUN_0049fb10(void* menu, int value);
void __stdcall FUN_00491c80(int value);

// FUNCTION: 0x478240
void __stdcall FUN_00478240(int param_1)
{
    char buf[256];

    FUN_004257a0();
    DAT_0051e668 = param_1;
    Layer_00478240* layer =
        LoadGuiLayer(&g_game->menu, "NEWGAME.GUI", 0x400);
    Entry_00478240* entries = layer->entries;
    layer->handler = FUN_00477ab0;
    layer->data = g_game;

    if (param_1 != 0) {
        FUN_004288d0("playanygame4", 0, 0, 0);
        DAT_00507b6c = 0;
    } else {
        BuildDataPath(buf, "camps", "*", "TDF");
        if (FUN_004bc930(buf, 0) <= 2) {
            FUN_004288d0("newcampaign4x", 0, 0, 0);
            DAT_00507b6c = 1;
        } else {
            FUN_004288d0("newcampaign4", 0, 0, 0);
            DAT_00507b6c = 0;
        }
    }

    if (param_1 != 0) {
        int i = FindGadgetIndex(entries, "Campaign", 2);
        if (i != -1) {
            Entry_00478240* e = (Entry_00478240*)((char*)entries + i * 0x15b);
            e->y = 0x134;
            e->height = 0x30;
        }
        i = FindGadgetIndex(entries, "CampaignKnob", 4);
        if (i != -1) {
            Entry_00478240* e = (Entry_00478240*)((char*)entries + i * 0x15b);
            e->y = 0x134;
            e->height = 0x30;
        }
        i = FindGadgetIndex(entries, "Missions", 2);
        if (i != -1)
            ((Entry_00478240*)((char*)entries + i * 0x15b))->height = 0x3e;
    }

    FUN_004c2470();
    RenderLayer(&g_game->menu, 1);
    FUN_004c2870();

    unsigned short* p =
        (unsigned short*)FindGafEntry(((GafEntry_00478240*)entries)->gaf, "Side0");
    int n = 0;
    while (n < *p) {
        char* r = (char*)GetGafFrame(p, 0);
        *(short*)(r + 6) = 0;
        *(short*)(r + 4) = 0;
        n++;
    }
    p = (unsigned short*)FindGafEntry(((GafEntry_00478240*)entries)->gaf, "Side1");
    n = 0;
    while (n < *p) {
        char* r = (char*)GetGafFrame(p, 0);
        *(short*)(r + 6) = 0;
        *(short*)(r + 4) = 0;
        n++;
    }

    FUN_00477360();

    Entry_00478240* diff =
        FindGadgetOrNull(g_game->menu.layer->entries, "Difficulty");
    if (g_game->field_37eee == 0) {
        diff->difficulty = 0;
        SetGadgetStatusByName(&g_game->menu, "Easy", 1);
    }
    if (g_game->field_37eee == 1) {
        diff->difficulty = 1;
        SetGadgetStatusByName(&g_game->menu, "Medium", 1);
    }
    if (g_game->field_37eee == 2) {
        diff->difficulty = 2;
        SetGadgetStatusByName(&g_game->menu, "Hard", 1);
    }

    FUN_0049fa90(&g_game->menu);

    if (DAT_00507b6c == 0 || param_1 != 0) {
        FUN_004a0570(&g_game->menu, "Campaign", 1);
        FUN_004a0570(&g_game->menu, "CampaignKnob", 1);
        Entry_00478240* c = FindGadgetChecked(entries, "Campaign");
        if (c != 0 && DAT_0051e668 != 0)
            c->callback = FUN_004779e0;
        else
            c->callback = 0;

        int side = g_game->players[g_game->localPlayer].unit->side;
        Layer_00478240* cur = g_game->menu.layer;
        if (DAT_0051e65c != 0) {
            FUN_004d85a0(DAT_0051e65c);
            DAT_0051e65c = 0;
        }
        FUN_0047f1a0("smlbutton", 0);
        int count = FUN_00476a60(&DAT_0051e65c, side);
        FUN_004a32a0(&g_game->menu, "Campaign", DAT_0051e65c, count, 0);
        int ci = FindGadgetIndex(cur->entries, "Campaign", 2);
        FUN_004a2be0(&g_game->menu, ci);
        FUN_0049fa90(&g_game->menu);

        if (param_1 != 0) {
            FUN_004a0570(&g_game->menu, "Missions", 1);
            FUN_004a0570(&g_game->menu, "MissionsKnob", 1);
            FindGadgetChecked(entries, "Missions");

            Menu_00478240* menu = &g_game->menu;
            Layer_00478240* mlayer = menu->layer;
            if (DAT_0051e660 != 0) {
                FUN_004d85a0(DAT_0051e660);
                DAT_0051e660 = 0;
            }
            Entry_00478240* m = FindGadgetChecked(g_game->menu.layer->entries,
                                             "Campaign");
            char* text = FUN_004b6af0(m->text, m->line);
            ((Class_00435110*)g_game->net)->LoadCampaign(text);
            int mc = ((Class_00435760*)g_game->net)->BuildMissionList(
                (int*)&DAT_0051e660);
            FUN_004a32a0(menu, "Missions", DAT_0051e660, mc, 0);
            int mi = FindGadgetIndex(mlayer->entries, "Missions", 2);
            FUN_004a2be0(&g_game->menu, mi);
            FUN_0049fa90(&g_game->menu);
        }
    }

    if (param_1 != 0)
        SelectGadgetByName(&g_game->menu, "Missions");
    else if (DAT_00507b6c != 0)
        SelectGadgetByName(&g_game->menu, "Difficulty");
    else
        SelectGadgetByName(&g_game->menu, "Campaign");

    FUN_0049fb10(&g_game->menu, 1);
    RenderLayer(&g_game->menu, 0x40);
    FUN_00491c80(0x13);
}
