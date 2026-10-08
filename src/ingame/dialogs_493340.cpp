// Decompiled by Opus, DeepSeek V4.1 Flash, Claude Opus 5.5, mimo-v2.6-flash, space-bunny-free, deepseek-v4.1-flash, deepseek-v4.1, Sonnet 5.5, Space Bunny Free, GPT-6, Haiku and Sonnet. Names are provisional.
// The in-game dialogs: the metal and energy readouts, giving selected units
// away, the resource sharing screen (SHARE.GUI), the chat target screen
// (TALK.GUI), the unit info screen, the tab menu (TABMENU.GUI), the frame
// pacing, the game step loop and the mission-script event dispatcher.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <windows.h>
#include <math.h>
#include <vector>

class Mission {
public:
    int GetGameType();
};

class Class_004ce680 {
public:
    int GetTrackCategory();
};

class Sound {
public:
    void SetTrackCategory(int param_1);
};

unsigned int __cdecl GetMilliseconds(void);

class PacketManager {
public:
    int SendAllQueued(int param_1);
};

class FrameTimers {                    // frame-time profile, embedded at g_game+0x38d85
public:
    int last;                           // +0x00
    int total;                          // +0x04
    int values[9];                      // +0x08
    int acc[9];                         // +0x2c

    void AccumulateProfileTime(int i) {
        unsigned int t = GetMilliseconds();
        int d = t - last;
        int v = acc[i];
        v = v + d;
        acc[i] = v;
        last = t;
    }
};

struct Game;

#pragma pack(push, 1)

struct Rect {
    int left;
    int top;
    int right;
    int bottom;
};

struct Point {
    int x;
    int y;
};

struct Quad {
    Point p[4];
};

struct Menu;

struct Gadget {                        // GUI entry, 0x15b bytes
    unsigned char type;                // +0x00
    unsigned char group;               // +0x01
    char name[0x10];                   // +0x02
    char unknown_12[0x17 - 0x12];
    short field_17;                    // +0x17
    short field_19;                    // +0x19
    int attr;                          // +0x1b
    char unknown_1f[0xb6 - 0x1f];
    union {
        short count;                   // +0xb6 (entry 0 only)
        void* callback;                // +0xb6 (the HOTR entry)
    } u;
    union {
        short selected;                // +0xba
        void* image;                   // +0xba
        struct {
            char unknown_bc[2];
            void* surface;             // +0xbc
        };
    };
    char unknown_c0[0xd2 - 0xc0];
    void* field_d2;                    // +0xd2
    char unknown_d6[0x136 - 0xd6];
    short field_136;                   // +0x136
    char unknown_138[0x13c - 0x138];
    int field_13c;                     // +0x13c
    short field_140;                   // +0x140
    short field_142;                   // +0x142
    void (__stdcall* handler)(Menu*, int); // +0x144
    char unknown_148[0x14a - 0x148];
    void* field_14a;                   // +0x14a
    char unknown_14e[0x15b - 0x14e];
};

struct Layer {
    int unknown_0;
    Gadget* entries;                   // +0x04
    void (__stdcall* handler)(Menu*);  // +0x08
    Game* owner;                       // +0x0c
    char unknown_10[0x20 - 0x10];
    int field_20;                      // +0x20
};

struct Menu {
    char unknown_0[0x18];
    Layer* layer;                      // +0x18
    char unknown_1c[0x60 - 0x1c];
    int current;                       // +0x60
};

struct PlayerInfo {
    char unknown_0[0x96];
    unsigned char field_96;            // +0x96
    char unknown_97[0x9b - 0x97];
    union {
        unsigned char flags;           // +0x9b
        struct {
            unsigned short pad : 6;
            unsigned short bit6 : 1;   // mask 0x40
            unsigned short rest : 9;
        };
    };
};

struct Player {                        // 0x14b bytes
    int active;                        // +0x00
    int field_4;                       // +0x04
    char unknown_8[0x18 - 0x8];
    int tick;                          // +0x18
    char unknown_1c[0x27 - 0x1c];
    PlayerInfo* info;                  // +0x27
    char name[0x48];                   // +0x2b
    unsigned char type;                // +0x73
    char unknown_74[0x8c - 0x74];
    float energy;                      // +0x8c
    char unknown_90[0x98 - 0x90];
    float metal;                       // +0x98
    char unknown_9c[0xa4 - 0x9c];
    float energyCapacity;              // +0xa4
    float metalCapacity;               // +0xa8
    char unknown_ac[0xfc - 0xac];
    short field_fc;                    // +0xfc
    short field_fe;                    // +0xfe
    char unknown_100[0x104 - 0x100];
    short field_104;                   // +0x104
    short field_106;                   // +0x106
    unsigned char allied[10];          // +0x108
    char unknown_112[0x140 - 0x112];
    int field_140;                     // +0x140
    unsigned short field_144;          // +0x144
    unsigned char field_146;           // +0x146
    char unknown_147[0x148 - 0x147];
    unsigned char field_148;           // +0x148
    char unknown_149[0x14b - 0x149];
};

struct Unit {
    char unknown_0[0x86];
    int field_86;                      // +0x86
    int field_8a;                      // +0x8a
    char unknown_8e[0xa6 - 0x8e];
    unsigned short type;               // +0xa6
    char unknown_a8[0xf0 - 0xa8];
    int field_f0;                      // +0xf0
    unsigned char field_f4;            // +0xf4
    char unknown_f5[0x110 - 0xf5];
    union {
        unsigned int flags;            // +0x110
        struct {
            unsigned int bits_110 : 14;
            unsigned int flag_110 : 1;
        };
    };
    char unknown_114[0x118 - 0x114];
};

struct Saved {                         // 10 bytes
    int a;                             // +0x00
    int b;                             // +0x04
    short c;                           // +0x08
};

struct Flags16 {
    unsigned short bits0_7 : 8;
    unsigned short bit8 : 1;
    unsigned short bits9_15 : 7;
};

struct Game {
    char unknown_0[0x10];
    Class_004ce680* field_10;          // +0x10
    char unknown_14[0x511 - 0x14];
    Player* slowest;                   // +0x511
    int lag;                           // +0x515
    Menu menu;                         // +0x519
    int team_index;                    // +0x57d
    int selected;                      // +0x581
    char unknown_585[0x1b63 - 0x585];
    Player players[10];                // +0x1b63
    char unknown_2851[0x2a3c - 0x2851];
    unsigned short numPlayers;         // +0x2a3c
    char unknown_2a3e[0x2a42 - 0x2a3e];
    unsigned char localPlayer;         // +0x2a42
    unsigned char player;              // +0x2a43
    unsigned char flags_2a44;          // +0x2a44
    char unknown_2a45[0x2bee - 0x2a45];
    union {
        unsigned short flags_2bee;     // +0x2bee
        Flags16 bits_2bee;
    };
    unsigned char mode_2bf0;           // +0x2bf0
    unsigned char field_2bf1[11];      // +0x2bf1
    char unknown_2bfc[0x2c74 - 0x2bfc];
    unsigned char field_2c74;          // +0x2c74
    char unknown_2c75[0x2cba - 0x2c75];
    unsigned short field_2cba;         // +0x2cba
    char unknown_2cbc[0x14280 - 0x2cbc];
    unsigned char counter_14280;       // +0x14280
    char unknown_14281[0x14357 - 0x14281];
    Unit* units;                       // +0x14357
    char unknown_1435b[0x1439b - 0x1435b];
    char* unitDefs;                    // +0x1439b
    char unknown_1439f[0x148db - 0x1439f];
    int field_148db;                   // +0x148db
    char unknown_148df[0x37ea0 - 0x148df];
    char guiName[0x1e];                // +0x37ea0
    union {
        unsigned short flags_37ebe;    // +0x37ebe
        unsigned char byte_37ebe;
        struct {
            unsigned short pad_37ebe : 6;
            unsigned short bit6_37ebe : 1;
            unsigned short rest_37ebe : 9;
        };
        struct {
            unsigned short low_37ebe : 11;
            unsigned short bit11_37ebe : 1;
            unsigned short high_37ebe : 4;
        };
    };
    char unknown_37ec0[0x37ef6 - 0x37ec0];
    int field_37ef6;                   // +0x37ef6
    char unknown_37efa[0x37f06 - 0x37efa];
    unsigned char field_37f06;         // +0x37f06
    char unknown_37f07[0x37f2f - 0x37f07];
    unsigned short field_37f2f;        // +0x37f2f, bit 1 is the "verbose" bit
    char unknown_37f31[0x38a37 - 0x37f31];
    unsigned int lastTick;             // +0x38a37
    int steps;                         // +0x38a3b
    int elapsed;                       // +0x38a3f
    float carry;                       // +0x38a43
    int ticks;                         // +0x38a47
    unsigned short maxSpeed;           // +0x38a4b
    unsigned short speed;              // +0x38a4d
    short streak;                      // +0x38a4f
    unsigned short paused : 1;         // +0x38a51
    unsigned short lagging : 1;
    unsigned short faster : 1;
    unsigned short rest : 13;
    char unknown_38a53[0x38d75 - 0x38a53];
    volatile unsigned short netFlags;  // +0x38d75, 16-bit flags word (volatile: see below)
    char unknown_38d77[0x38d85 - 0x38d77];
    FrameTimers prof;                  // +0x38d85
    char unknown_38dd5[0x391e9 - 0x38dd5];
    Mission* net;                      // +0x391e9
    char unknown_391ed[0x3923b - 0x391ed];
    unsigned short bit0_3923b : 1;     // +0x3923b
    unsigned short rest_3923b : 15;
};
#pragma pack(pop)

extern int g_shareDialogPlayerNetIds[10];
extern int DAT_005091cc;
extern char g_chatDraftText[];
extern char g_livePlayerPrefix[];      // "LIVEPLYR"
extern char g_smallButtonSoundName[];  // "SmallButton"
extern char g_sendTypeGadgetName[];    // "SENDTYPE"
extern char g_sendToGadgetName[];      // "SENDTO"
extern char g_talkGadgetName[];        // "TALK"
extern char g_chatTargetSeparators[];  // ",:;"
extern char g_enemiesChatTarget[];     // "Enemies"
extern char g_alliesChatTarget[];      // "Allies"
extern int g_chatDraftInitialized;
extern unsigned char DAT_0051f2c8[10];
extern unsigned char DAT_0051e810[10];
extern int DAT_0051f2d8;
extern int g_scorePanelFlashDecayTick;
extern unsigned int g_cdActivityLastSampleTick;
extern int g_cdActivityStableTicks;
extern int DAT_0051f2dc;
extern int DAT_0051e710[];
extern int g_lastCdActivityMode;
extern int g_usePacketManager;
extern PacketManager g_packetManager;

Gadget* __stdcall FUN_004a0200(Gadget* entries, char* name);
int __stdcall ReadSliderValue(Gadget* entry);
void __stdcall SetTranslatedTextByName(Menu* menu, char* name, char* text, int param_4);
void __stdcall PlaySoundByName(char* name, int flag);
Gadget* __stdcall FindGadgetChecked(Gadget* entries, char* name);
int __stdcall IsCurrentGadgetNamed(Menu* menu, char* name);
void __cdecl FUN_004d85a0(void* p);
void __stdcall MarkChanged(Menu* menu);
void __stdcall ClearSelectedGadget(Menu* menu);
void __stdcall TransferEnergy(unsigned char from, unsigned char to, float amount, int flag);
void __stdcall TransferMetal(unsigned char from, unsigned char to, float amount, int flag);
int __stdcall GetButtonStageByName(Menu* menu, char* name);
unsigned char __stdcall FindSlotByDpid(int id);
void __stdcall ShareMapInfo(unsigned char from, unsigned char to);
void __stdcall SendShareMapInfo(unsigned char from, unsigned char to);
void __stdcall CollectSelectedUnits(std::vector<Unit*>* list);
unsigned int* __stdcall GetCategoryMask(char* name);
void __stdcall GiveUnitToPlayer(Unit* unit, Player* player, int arg);
Layer* __stdcall LoadGuiLayer(Menu* menu, const char* name, int flags);
void __stdcall HandleShareDialogEvent(Menu* gadget);
int __stdcall FindGadgetIndex(Gadget* entries, char* name, int type);
void __stdcall SetSliderFromValue(Gadget* entry, int param_2);
void* __cdecl FUN_004d83b0(char* name, unsigned int size);
void __stdcall CloseTopScreen(Menu* menu);
void __stdcall ConfigureListBoxByName(Menu* menu, char* name, char* text, int count, int flag);
void __stdcall SetKeyboardInput(Menu* menu, int value);
void __stdcall RenderLayer(Menu* menu, int value);
void __stdcall SetGadgetActiveByName(Menu* menu, char* name, int value);
void __stdcall SetButtonStageByName(Menu* menu, char* name, int value);
int __stdcall GetButtonStage(Menu* menu, int index);
Gadget* __stdcall FUN_004a0010(Gadget* entries, char* name);
void __stdcall GetGadgetText(Menu* menu, char* name, char* text);
void ResetPlayerGadgets();
void OpenTalkDialog();
int __stdcall ExecuteCommandLine(char* cmd, int flags);
void __stdcall TrySetFocus(Menu* menu, int index);
void __stdcall SendChatMessage(Player* from, char* text, int param_3, char* to);
void __stdcall HandleTalkDialogEvent(Menu* gadget);
void __stdcall RefreshAlliesScreen(int value);
Gadget* __stdcall FUN_004a0280(Gadget* entries, char* name);
void __stdcall FreeSurface(void* param_1);
void __stdcall GetGadgetRect(Gadget* entry, Rect* rect);
void __stdcall DrawSurface(void* dest, void* image, int x, int y);
void __stdcall HandleUnitInfoDialogEvent(Menu* gadget);
void __stdcall DrawUnitInfoImage(Menu* gadget, Gadget* entry);
void __stdcall BuildDataPath(char* out, const char* dir, const char* name, const char* ext);
void* __stdcall LoadPcx(char* path, int param);
char* __stdcall MakePropList(void* obj);
char* __stdcall Translate(char* text);
void __stdcall AddTextGadget(Layer* obj, char* name, char* text, int x, short y, int w, int flags);
int __stdcall IsUnitVisibleToPlayer(Player* player, Unit* unit);
unsigned short __stdcall FindUnitTypeId(char* name);
void __stdcall DisableKeyCommands(Menu* menu);
void OpenInGameOptions();
void OpenShareDialog();
void OpenControlDialog();
void OpenAlliesDialog();
void __stdcall HandleMain2LayoutEvent(Menu* gadget);
unsigned int GetTicks();
int GetScreenWidth();
int __stdcall IsKeyDown(int key);
void __stdcall FadeRectangle(void* surface, Rect* rect, int level);
void __stdcall DrawTextClipped(void* surface, char* text, int x, int y, int maxw, int style);
int __stdcall GetTextPixelWidth(char* text);
void* __stdcall GetGafFrame(void* glyphs, int c);
void __stdcall DrawFrameQuad(void* surface, void* pic, Quad* dst, Quad* src);
void ReportIntervalTimer();
int __stdcall IsScreenNamed(Menu* menu, const char* name);
void HideSoftwareCursor();
void __stdcall HandleTabMenuEvent(Menu* gadget);
int IsHostLocal();
void __stdcall EnableKeyCommands(Menu* menu);
void ShowSoftwareCursor();
void HandleNetPackets(void);
void UpdateAllUnits(void);
void UpdateProjectiles(void);
void UpdateExplosions(void);
void UpdatePlayers(void);
void UpdateFeatures(void);
void StepAllGafSequences(void);
void UpdateWind(void);
void UpdateMeteors(void);
void UpdateCameraFollow(void);
void UpdateParticles(void);
void UpdateBlink(void);
void EmptyPostSimStepHook(void);
void EmptyPostSimStepHook_B(void);
void EmptyPostSimStepHook_C(void);
void ExpireOldestMessage(void);
void ExpireEyeballs(void);
void __stdcall UpdateResourceSharing(Player* player);
int __stdcall SetPageFlipping(int enable);

// FUNCTION: 0x493340
void __stdcall UpdateMetalReadout(Menu* obj, int unused)
{
    char buf[52];
    Gadget* value = FUN_004a0200(obj->layer->entries, "METAL");
    if (value != 0) {
        sprintf(buf, "%d", ReadSliderValue(value));
        SetTranslatedTextByName(obj, "METAL#", buf, 0);
    }
}

// FUNCTION: 0x493390
void __stdcall UpdateEnergyReadout(Menu* obj, int unused)
{
    char buf[52];
    Gadget* value = FUN_004a0200(obj->layer->entries, "ENERGY");
    if (value != 0) {
        sprintf(buf, "%d", ReadSliderValue(value));
        SetTranslatedTextByName(obj, "ENERGY#", buf, 0);
    }
}

// The same inlined bit-set test as 0x41c310.
static inline int TestBit(unsigned int* set, unsigned short n)
{
    return set[n >> 5] & (1 << (n & 0x1f));
}

// CollectSelectedUnits clears the vector and fills it with the local player's
// units that have bit 4 of +0x110 set (probably the selected units); each one
// whose low two flag bits are not 2, with +0x86 and +0x8a clear and whose type
// is not in the "Commander" set, is handed to the given player with
// GiveUnitToPlayer.
// The list is a real std::vector<Unit*> (0x48ca20 calls the vector's _Ucopy,
// _Ufill and _Destroy).

// FUNCTION: 0x4933e0
void __stdcall GiveSelectedUnitsToPlayer(unsigned char player)
{
    extern Game* g_game;
    // Must be a real std::vector: its constructor leaves the player index in place.
    std::vector<Unit*> list;
    CollectSelectedUnits(&list);
    Player* p = &g_game->players[player];
    unsigned int* set = GetCategoryMask("Commander");
    for (std::vector<Unit*>::iterator it = list.begin(); it != list.end(); it++) {
        Unit* unit = *it;
        if ((unit->flags & 3) != 2 && unit->field_8a == 0 && unit->field_86 == 0
            && !TestBit(set, unit->type)) {
            GiveUnitToPlayer(unit, p, 0);
        }
    }
}

static inline int IsPlaying_4934b0(Player* p)
{
    return p->active != 0 && (p->type == 1 || p->type == 2 || p->type == 3)
        && p->field_146 != 10;
}

static inline int IsCounted_4934b0(Player* p)
{
    return (p->type == 1 || p->type == 2 || p->type == 3)
        && (p->field_144 != 0 || p->field_140 == 0);
}

// FUNCTION: 0x4934b0
void __stdcall HandleShareDialogEvent(Menu* obj)
{
    extern Game* g_game;
    Gadget* data = obj->layer->entries;

    if (obj->current == -1) {
        Gadget* e = FindGadgetChecked(data, "PLYRLIST");
        FUN_004d85a0(e->field_d2);
        g_game->flags_37ebe &= ~0x40;
        return;
    }
    if (IsCurrentGadgetNamed(obj, "MAPINFO")) {
        MarkChanged(obj);
        PlaySoundByName("Options", 0);
        ClearSelectedGadget(obj);
        return;
    }
    if (IsCurrentGadgetNamed(obj, "SHARUNIT")) {
        MarkChanged(obj);
        PlaySoundByName("Options", 0);
        ClearSelectedGadget(obj);
        return;
    }
    if (IsCurrentGadgetNamed(obj, "OK")) {
        PlaySoundByName("Options", 0);
        Gadget* plyr = FindGadgetChecked(data, "PLYRLIST");
        short idx = plyr->selected;
        if (idx < 0)
            return;
        int pi = FindSlotByDpid(g_shareDialogPlayerNetIds[idx]);
        Player* p = &g_game->players[pi];
        if (IsPlaying_4934b0(p) && !(p->info->flags & 0x40) && IsCounted_4934b0(p)) {
            TransferEnergy(g_game->localPlayer, pi,
                         (float)ReadSliderValue(FUN_004a0200(data, "METAL")), 1);
            TransferMetal(g_game->localPlayer, pi,
                         (float)ReadSliderValue(FUN_004a0200(data, "ENERGY")), 1);
            if (GetButtonStageByName(obj, "SHARUNIT"))
                GiveSelectedUnitsToPlayer(pi);
            if (GetButtonStageByName(obj, "MAPINFO")) {
                ShareMapInfo(g_game->localPlayer, pi);
                SendShareMapInfo(g_game->localPlayer, pi);
            }
        }
        return;
    }
    if (IsCurrentGadgetNamed(obj, "CANCEL")) {
        PlaySoundByName("Previous", 0);
        return;
    }
    if (obj->current != -1)
        ClearSelectedGadget(obj);
}

// Opens the resource sharing screen (SHARE.GUI): it walks the ten player
// records, finds the local player's entry (flagged 0x40), points the METAL and
// ENERGY sliders at the local counts, and finishes with the menu setup calls.
// A resource's owner is flagged 0x40 in the player record; the two sliders are
// the "METAL#" and "ENERGY#" texts.

// FUNCTION: 0x4936f0
void OpenShareDialog()
{
    extern Game* g_game;
    if (g_game->players[g_game->localPlayer].info->bit6)
        return;
    Layer* layer = LoadGuiLayer(&g_game->menu, "SHARE.GUI", 0x800);
    g_game->bit6_37ebe = 1;
    Gadget* entries = layer->entries;
    layer->handler = HandleShareDialogEvent;
    layer->owner = g_game;
    int idx = FindGadgetIndex(entries, "METAL", 0xe);
    if (idx != -1) {
        Gadget* e = &entries[idx];
        e->field_142 = layer->entries[idx].field_19;
        e->field_136 = layer->entries[idx].field_17 - e->field_142;
        e->field_13c = (int)g_game->players[g_game->localPlayer].metal;
        e->handler = UpdateMetalReadout;
        e->field_140 = 0;
        SetSliderFromValue(e, 0);
        e->field_14a = g_game;
    }
    idx = FindGadgetIndex(layer->entries, "ENERGY", 0xe);
    if (idx != -1) {
        Gadget* e = FUN_004a0200(layer->entries, "ENERGY");
        e->field_142 = layer->entries[idx].field_19;
        e->field_136 = layer->entries[idx].field_17 - e->field_142;
        e->field_13c = (int)g_game->players[g_game->localPlayer].energy;
        e->handler = UpdateEnergyReadout;
        e->field_140 = 0;
        SetSliderFromValue(e, 0);
        e->field_14a = g_game;
    }

    char* names = (char*)FUN_004d83b0("PLAYERS", g_game->numPlayers * 30);
    char* np = names;
    *np = 0;
    memset(g_shareDialogPlayerNetIds, -1, sizeof(g_shareDialogPlayerNetIds));
    int* ids = g_shareDialogPlayerNetIds;
    int count = 0;
    for (int i = 0; i < 10; i++) {
        Player* p = &g_game->players[i];
        if (p->active && (p->type == 1 || p->type == 2 || p->type == 3) && p->field_146 != 10 &&
            (p->field_144 != 0 || p->field_140 == 0) && p->type != 1 && !p->info->bit6) {
            strcpy(np, p->name);
            np += strlen(p->name) + 1;
            *ids = p->field_4;
            count++;
            ids++;
        }
    }
    if (count == 0) {
        CloseTopScreen(&g_game->menu);
        return;
    }
    ConfigureListBoxByName(&g_game->menu, "PLYRLIST", names, count, 0);
    char text[0x34];
    // Both tail blocks go through the local menu pointer, menu first, with no
    // self-comparison: this fixes their register choice.
    Menu* menu = &g_game->menu;
    Layer* lyr = menu->layer;
    Gadget* ents = lyr->entries;
    Gadget* e = FUN_004a0200(ents, "METAL");
    if (e) {
        sprintf(text, "%d", ReadSliderValue(e));
        SetTranslatedTextByName(menu, "METAL#", text, 0);
    }
    menu = &g_game->menu;
    lyr = menu->layer;
    ents = lyr->entries;
    e = FUN_004a0200(ents, "ENERGY");
    if (e) {
        sprintf(text, "%d", ReadSliderValue(e));
        SetTranslatedTextByName(menu, "ENERGY#", text, 0);
    }
    MarkChanged(&g_game->menu);
    SetKeyboardInput(&g_game->menu, 1);
    RenderLayer(&g_game->menu, 0x40);
}

// Resets every player's "PLAYER%d" gadget and publishes a "LIVEPLYR%d" value
// for each occupied slot except the local player's: 1, whether the local
// player considers that slot an ally, or the flag at g_game+0x2bf1, depending
// on the chat mode at g_game+0x2bf0.

// FUNCTION: 0x493ae0
void ResetPlayerGadgets(void)
{
    extern Game* g_game;
    char buf[52];
    unsigned char* p = g_game->players[g_game->localPlayer].allied;
    for (int i = 0; i < 10; i++, p++) {
        sprintf(buf, "PLAYER%d", i);
        SetGadgetActiveByName(&g_game->menu, buf, 0);
        unsigned char state = g_game->players[i].type;
        if (state != 0 && state != 4 && i != g_game->localPlayer) {
            sprintf(buf, "LIVEPLYR%d", i);
            int value = 0;
            unsigned char* flags = g_game->field_2bf1;
            switch (g_game->mode_2bf0) {
            case 1:
                value = *p;
                break;
            case 2:
                value = (*p == 0);
                break;
            case 0:
                value = 1;
                break;
            case 3:
                value = flags[i];
                break;
            }
            SetButtonStageByName(&g_game->menu, buf, value);
        }
    }
}

// FUNCTION: 0x493bf0
void __stdcall HandleTalkDialogEvent(Menu* gadget)
{
    char buf2[0x12c];
    char buf[0x100];
    // mode and oldmode must both be int.
    int oldmode;
    int mode;
    int n;
    int d;
    // Declared here, after n and d, not at file scope: g_game's symbol id sets
    // the base/index order of the byte stores.
    extern Game* g_game;
    Gadget* entries = gadget->layer->entries;
    // current is read directly, not through an id local.
    if (gadget->current == -1) {
        g_game->flags_37ebe &= ~4;
        return;
    }
    if (_strnicmp(entries[gadget->current].name, g_livePlayerPrefix, 8) == 0) {
        PlaySoundByName(g_smallButtonSoundName, 0);
        g_game->mode_2bf0 = 3;
        SetButtonStageByName(gadget, g_sendTypeGadgetName, g_game->mode_2bf0);
        n = atoi(&entries[gadget->current].name[8]);
        // Kept as the original has it: n is never range checked before it
        // indexes the 11-byte selection mask, so a "LIVEPLYR42" style name
        // writes outside field_2bf1. The neighbouring mode_2bf0 is clamped
        // (`if (g_game->mode_2bf0 >= 4) g_game->mode_2bf0 = 0;`), so the
        // omission looks like an oversight rather than a deliberate choice.
        unsigned char v = (unsigned char)GetButtonStage(gadget, gadget->current);
        g_game->field_2bf1[n] = v;
        MarkChanged(gadget);
        ClearSelectedGadget(gadget);
        goto tail;
    }
    if (IsCurrentGadgetNamed(gadget, g_sendToGadgetName)) {
        PlaySoundByName(g_smallButtonSoundName, 0);
        unsigned char v = (unsigned char)GetButtonStage(gadget, gadget->current);
        g_game->bits_2bee.bit8 = v & 1;
        GetGadgetText(gadget, g_talkGadgetName, g_chatDraftText);
        CloseTopScreen(gadget);
        OpenTalkDialog();
        ClearSelectedGadget(gadget);
        return;
    }
    if (IsCurrentGadgetNamed(gadget, g_sendTypeGadgetName)) {
        PlaySoundByName(g_smallButtonSoundName, 0);
        g_game->mode_2bf0 = (unsigned char)GetButtonStageByName(gadget, g_sendTypeGadgetName);
        if (g_game->mode_2bf0 >= 4)
            g_game->mode_2bf0 = 0;
        ResetPlayerGadgets();
        ClearSelectedGadget(gadget);
        goto tail;
    }
    if (IsCurrentGadgetNamed(gadget, g_talkGadgetName)) {
        Gadget* talk = FUN_004a0010(entries, g_talkGadgetName);
        mode = g_game->mode_2bf0;
        lstrcpynA(buf, (char*)talk + 0xb6, 0x100);
        char* p = buf;
        while (*p && *p == ' ')
            p++;
        if (*p == '+') {
            int flags = 1;
            // The cast keeps the shift a 16-bit one, which is what stops MSVC
            // folding this into `test byte ptr [g_game + 0x37f2f], 2`.
            if (flags & (unsigned char)(g_game->field_37f2f >> 1))
                flags = 7;
            if (DAT_005091cc)
                flags |= 2;
            int r = ExecuteCommandLine(p + 1, flags);
            entries = gadget->layer->entries;
            if (r & 2)
                mode = 0;
        }
        if (strlen(p) != 0) {
            // These four statements stay in this order: oldmode, to, base, saved.
            oldmode = g_game->mode_2bf0;
            char* to = 0;
            Player* base = &g_game->players[g_game->localPlayer];
            Saved saved = *(Saved*)g_game->field_2bf1;
            // Early out rather than a positive `if`, and the empty statement
            // at skip0 below is load bearing: either change costs 9 points.
            if (!(' ' < p[1] && strchr(g_chatTargetSeparators, p[1]) != 0)) goto skip0;
            if (isdigit(p[0])) {
                d = p[0] - '0';
                if (d < 0 || d > 9 || g_game->players[d].field_4 == 0)
                    goto clear;
                p += 2;
                mode = 3;
                // to is computed before the memset.
                to = g_game->players[d].name;
                memset(g_game->field_2bf1, 0, 11);
                g_game->field_2bf1[d] = 1;
            } else {
                int c = tolower(p[0]);
                if (c != 'a') {
                    if (c == 'e') {
                        mode = 2;
                        to = g_enemiesChatTarget;
                    } else {
                        goto after;
                    }
                } else {
                    mode = 1;
                    to = g_alliesChatTarget;
                }
                p += 2;
            }
skip0:;
after:
            g_game->mode_2bf0 = mode;
            memset(buf2, 0, sizeof(buf2));
            SendChatMessage(base, p, 4, to);
            *(Saved*)g_game->field_2bf1 = saved;
            g_game->mode_2bf0 = oldmode;
        }
clear:
        memset(g_chatDraftText, 0, 0x81);
        g_game->bits_2bee.bit8 = 0;
    }
tail:
    int index = FindGadgetIndex(entries, g_talkGadgetName, 3);
    TrySetFocus(&g_game->menu, index);
    g_game->menu.layer->field_20 = FindGadgetIndex(entries, g_talkGadgetName, 3);
}

// The functions before 0x493bf0 declare g_game inside their bodies: its symbol
// id has to come after the locals of 0x493bf0 (the order of its byte stores).
// GLOBAL: 0x511de8
extern Game* g_game;
// FUNCTION: 0x494050
void OpenTalkDialog()
{
    PlayerInfo* info = g_game->players[g_game->localPlayer].info;
    if (info->bit6)
        return;
    if (g_chatDraftInitialized == 0) {
        g_chatDraftInitialized = 1;
        memset(g_chatDraftText, 0, 0x81);
    }
    if (g_game->flags_37ebe & 0x800)
        return;
    int multi = (g_game->flags_2bee & 0x100)
                && g_game->net->GetGameType() == 3;
    Layer* d = LoadGuiLayer(&g_game->menu,
                            multi ? "TALK2.GUI" : "TALK.GUI",
                            multi ? 0x800 : 0x880);
    Gadget* entries = d->entries;
    d->handler = HandleTalkDialogEvent;
    g_game->flags_37ebe |= 4;
    SetTranslatedTextByName(&g_game->menu, "TALK", g_chatDraftText, 0);
    SetButtonStageByName(&g_game->menu, "SENDTO", multi);
    if (g_game->net->GetGameType() != 3) {
        SetGadgetActiveByName(&g_game->menu, "SENDTO", 0);
    } else if (multi) {
        SetButtonStageByName(&g_game->menu, "SENDTYPE", g_game->mode_2bf0);
        RefreshAlliesScreen(1);
        ResetPlayerGadgets();
    }
    TrySetFocus(&g_game->menu, FindGadgetIndex(entries, "TALK", 3));
    d->field_20 = FindGadgetIndex(entries, "TALK", 3);
    d->owner = g_game;
    RenderLayer(&g_game->menu, 0x40 | (multi ? 0 : 0x80));
}

// FUNCTION: 0x494220
void __stdcall HandleUnitInfoDialogEvent(Menu* gadget)
{
    if (gadget->current == -1) {
        Gadget* e = FUN_004a0280(gadget->layer->entries, "HOTR");
        FreeSurface(e->image);
        g_game->flags_37ebe &= ~0x800;
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "DONE")) {
        PlaySoundByName("smlbutton", 0);
        return;
    }
    ClearSelectedGadget(&g_game->menu);
}

// Draws a gadget's image at the gadget's position (GetGadgetRect fills its
// bounding rectangle).

// FUNCTION: 0x494290
void __stdcall DrawUnitInfoImage(Menu* gadget, Gadget* entry)
{
    if (entry->image) {
        Rect r;
        GetGadgetRect(entry, &r);
        DrawSurface(gadget->layer->entries->surface, entry->image, r.left, r.top);
    }
}

// FUNCTION: 0x4942e0
void __stdcall OpenUnitInfoDialog(void)
{
    if (g_game->bit11_37ebe)
        return;

    int y;
    char* stats;
    char name[0x20];
    char buf[0x100];
    unsigned short type = 0;

    if (g_game->selected != -1) {
        strncpy(name, g_game->menu.layer->entries[g_game->selected].name, 0x10);
        name[0x10] = 0;
        type = FindUnitTypeId(name);
    } else {
        unsigned short t = g_game->field_2cba;
        if (t != 0) {
            Unit* unit = &g_game->units[t];
            Player* owner = &g_game->players[g_game->player];
            if (IsUnitVisibleToPlayer(owner, unit) == 0)
                type = 0;
            else
                type = unit->type;
        }
    }

    if (type == 0)
        return;

    Layer* layer = LoadGuiLayer(&g_game->menu, "UNITINFOx.GUI", 0x1000);
    Gadget* entries = layer->entries;
    layer->handler = HandleUnitInfoDialogEvent;
    layer->owner = g_game;
    Gadget* hotr = FUN_004a0280(entries, "HOTR");
    hotr->u.callback = (void*)DrawUnitInfoImage;
    char* def = g_game->unitDefs + 0x249 * (unsigned)type;
    BuildDataPath(buf, "unitpics", def + 0x20, "PCX");
    hotr->image = LoadPcx(buf, 0);
    stats = MakePropList(def);
    int n = entries->u.count;

    AddTextGadget(g_game->menu.layer, "TEXT", Translate("Cost"), 0x82, 0x20, -1, 2);
    n++;
    entries[n].attr = 0x411;
    AddTextGadget(g_game->menu.layer, "TEXT", Translate("Energy"), 0x8c, 0x2f, -1, 2);
    n++;
    entries[n].attr = 0x411;
    AddTextGadget(g_game->menu.layer, "TEXT", Translate("Metal"), 0x8c, 0x3e, -1, 2);
    n++;
    entries[n].attr = 0x411;
    AddTextGadget(g_game->menu.layer, "TEXT", Translate("Build Time"), 0x8c, 0x4d, -1, 2);
    n++;
    entries[n].attr = 0x411;
    AddTextGadget(g_game->menu.layer, "TEXT", Translate("Statistics"), 0x82, 0x5c, -1, 2);
    n++;
    entries[n].attr = 0x411;
    AddTextGadget(g_game->menu.layer, "TEXT", Translate("Max Velocity"), 0x8c, 0x6b, -1, 2);
    n++;
    entries[n].attr = 0x411;
    AddTextGadget(g_game->menu.layer, "TEXT", Translate("Acceleration"), 0x8c, 0x7a, -1, 2);
    n++;
    entries[n].attr = 0x411;
    AddTextGadget(g_game->menu.layer, "TEXT", Translate("Turn Rate"), 0x8c, 0x89, -1, 2);
    n++;
    entries[n].attr = 0x411;

    char* s = stats;
    if (*s != 0) {
        y = 0x20;
        do {
            AddTextGadget(g_game->menu.layer, "TEXT", s, 0xf0, y, -1, 2);
            n++;
            entries[n].attr = 0x411;
            s += strlen(s) + 1;
            y += 0xf;
        } while (*s != 0);
    }

    FUN_004d85a0(stats);
    SetTranslatedTextByName(&g_game->menu, "NAME", def, 0x80);
    MarkChanged(&g_game->menu);
}

// FUNCTION: 0x494740
void __stdcall HandleTabMenuEvent(Menu* gadget)
{
    if (gadget->current == -1) {
        g_game->flags_2bee &= 0xff1f;
        DisableKeyCommands(gadget);
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "OPTIONS")) {
        g_game->byte_37ebe |= 1;
        PlaySoundByName("BigButton", 0);
        OpenInGameOptions();
        ClearSelectedGadget(gadget);
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "SHARE")) {
        PlaySoundByName("BigButton", 0);
        OpenShareDialog();
        ClearSelectedGadget(gadget);
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "CONTROL")) {
        PlaySoundByName("BigButton", 0);
        OpenControlDialog();
        ClearSelectedGadget(gadget);
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "ALLIES")) {
        PlaySoundByName("BigButton", 0);
        OpenAlliesDialog();
        ClearSelectedGadget(gadget);
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "CANCEL")) {
        CloseTopScreen(gadget);
        return;
    }
    ClearSelectedGadget(gadget);
}

// FUNCTION: 0x494840
void __stdcall HandleTabMenuCancel(Menu* gadget)
{
    if (gadget->current == -1) {
        g_game->flags_2bee &= 0xff1f;
        DisableKeyCommands(&g_game->menu);
        return;
    }
    if (!IsCurrentGadgetNamed(gadget, "CANCEL"))
        ClearSelectedGadget(gadget);
}

// FUNCTION: 0x494890
void __stdcall HandleMain2LayoutEvent(Menu* gadget)
{
    if (gadget->current != -1) {
        ClearSelectedGadget(gadget);
    }
}

// FUNCTION: 0x4948b0
void __stdcall FlashScorePanelKillLoss(int param_1, int param_2)
{
    if (param_1 >= 0) {
        DAT_0051f2c8[param_1] = 0x1e;
    }
    if (param_2 >= 0) {
        DAT_0051e810[param_2] = 0x1e;
    }
}

// FUNCTION: 0x4948e0
void __stdcall DrawScorePanel(void* surface)
{
    if (g_scorePanelFlashDecayTick < (int)GetTicks()) {
        g_scorePanelFlashDecayTick = GetTicks() + 1;
        for (int i = 0; i < 10; i++) {
            if (DAT_0051f2c8[i] > 0)
                DAT_0051f2c8[i] -= 2;
            if (DAT_0051e810[i] > 0)
                DAT_0051e810[i] -= 2;
        }
    }

    if (!(g_game->field_37f06 & 0x80)
        && (IsKeyDown(0x20) == 0
            || (g_game->team_index != -1
                && g_game->menu.layer->entries[g_game->team_index].type == 3))) {
        if (DAT_0051f2d8 <= 0)
            return;
        if (DAT_0051f2d8 == 0x7d)
            PlaySoundByName("Panel", 0);
        int q = DAT_0051f2d8 / 4;
        if (q <= 1)
            q = 1;
        DAT_0051f2d8 -= q;
        if (DAT_0051f2d8 <= 0) {
            DAT_0051f2d8 = 0;
            PlaySoundByName("Options", 0);
        }
    } else if (DAT_0051f2d8 < 0x7d) {
        if (DAT_0051f2d8 == 0)
            PlaySoundByName("Panel", DAT_0051f2d8);
        int q = (0x7d - DAT_0051f2d8) / 4;
        if (q <= 1)
            q = 1;
        DAT_0051f2d8 += q;
        if (DAT_0051f2d8 >= 0x7d) {
            DAT_0051f2d8 = 0x7d;
            PlaySoundByName("Options", 0);
        }
    }

    Rect panel;
    panel.left = GetScreenWidth() - DAT_0051f2d8;
    panel.top = 0x20;
    panel.right = panel.left + 0x7d;
    panel.bottom = g_game->numPlayers * 0x28 + 0x2e;
    FadeRectangle(surface, &panel, -0x18);

    panel.right = panel.left + 0x7d;
    int y = panel.top;
    int maxw = panel.right - panel.left - 6;
    char buf[100];
    Quad src;
    src.p[0].x=1; src.p[0].y=1; src.p[3].x=1; src.p[1].y=1;
    strcpy(buf, Translate("Kills"));
    DrawTextClipped(surface, buf, panel.left + 2, y, maxw, 0);
    strcpy(buf, Translate("Losses"));
    DrawTextClipped(surface, buf, panel.right - GetTextPixelWidth(buf) - 2, y, maxw, 0);
    y += 0xf;

    for (int i = 0; i < (int)g_game->numPlayers; i++) {
        Player* p = g_game->players;
        // Chained assignments fix the store order; the right edge is
        // panel.left + maxw.
        Quad dst;
        dst.p[0].x = dst.p[3].x = panel.left + 7;
        dst.p[2].x = dst.p[1].x = panel.left + maxw;
        dst.p[0].y = dst.p[1].y = y + 1;
        dst.p[2].y = dst.p[3].y = y + 0x25;

        // The draw block stays inside the search loop; cleanup follows the loop.
        int n;
        for (n = 0; n < 10; n++, p++) {
            if (p->active == 0)
                continue;
            unsigned char c = p->type;
            if (c != 1 && c != 2 && c != 3)
                continue;
            if (p->field_146 == 0xa)
                continue;
            if (p->field_144 == 0 && p->field_140 != 0)
                continue;
            if (p->info->flags & 0x40)
                continue;
            if (p->field_148 != i)
                continue;
            // Assigned in the order left, right, top, bottom.
            Rect hr;
            hr.left = panel.left + 4;
            hr.right = panel.right - 4;
            hr.top = y - 1;
            hr.bottom = y + 0x26;
            if (n == g_game->localPlayer) {
                FadeRectangle(surface, &hr, 0x1f);
                FadeRectangle(surface, &hr, 0x14);
            }
            unsigned short* frame = (unsigned short*)GetGafFrame(
                (void*)g_game->field_148db, p->info->field_96);

            src.p[1].x = frame[0] - 1;
            src.p[2].x = frame[0] - 1;
            src.p[2].y = frame[1] - 1;
            src.p[3].y = frame[1] - 1;
            DrawFrameQuad(surface, frame, &dst, &src);

            DrawTextClipped(surface, p->name, dst.p[0].x + 2, dst.p[0].y + 5, maxw, 0);
            int kills = g_game->field_37ef6 == 2 ? p->field_104 : p->field_fc;
            sprintf(buf, "%d", kills);
            DrawTextClipped(surface, buf, dst.p[0].x + 2, dst.p[0].y + 0x14, maxw,
                         DAT_0051f2c8[n]);
            int losses = g_game->field_37ef6 == 2 ? p->field_106 : p->field_fe;
            sprintf(buf, "%d", losses);
            DrawTextClipped(surface, buf, dst.p[1].x - GetTextPixelWidth(buf) - 2,
                         dst.p[0].y + 0x14, maxw, DAT_0051e810[n]);
            y += 0x28;
            break;
        }
        if (n == 10) {
            Player* q = g_game->players;
            // Countdown k = n gives the frame its stack slot.
            for (int k = n; k != 0; k--, q++) {
                if (q->active == 0)
                    continue;
                unsigned char c = q->type;
                if (c != 1 && c != 2 && c != 3)
                    continue;
                if (q->field_146 == 0xa)
                    continue;
                if (q->field_144 == 0 && q->field_140 != 0)
                    continue;
                if (q->info->flags & 0x40)
                    continue;
                if (q->field_148 > i)
                    q->field_148--;
            }
        }
    }
}

// Frame-time / desync watchdog: keeps a 30-entry ring of per-frame counters at
// DAT_0051e710, and when the ring total (or the last five entries) grows too
// large it flips the network state g_lastCdActivityMode between 0 and 1 and resets the
// counter g_cdActivityStableTicks.
//
// Matches with netFlags declared `volatile`. Without it, only the two tests
// on g_game->netFlags at 0x494e8b differ. The original emits two memory-operand bit tests
// (`test byte ptr [eax+0x38d75],1` then `...,2`); this source makes MSVC load
// the byte once (`mov al,[eax+0x38d75]; test al,1; test al,2`), 4 bytes
// shorter. The same pair appears in 0x452800 and in 0x453d40 (at 0x4550c2).
//
// Notes from Claude Opus 5.5 (#436): the difference is not compiler state.
// It scores 78.9% for 0 to 400 unused `extern int`s, for 500 to 8000 unused
// prototypes, for every headers.py set and with <windows.h> plus <string>,
// <vector> + <map>, <iostream>, <list> or the DirectX headers. In scratch
// tests MSVC 5 shares the load across the two tests for every non-volatile
// spelling (bitfields, masks, byte/word/dword union views, inline helpers
// taking the object or a reference, ternaries, switch, separate ifs).
// Adding `volatile` to the declaration of netFlags (a 16-bit flags word)
// makes this function MATCH with no other change; adding `volatile` to
// 0x452800's declaration of the field makes that function MATCH as well.
// Evidence that Cavedog really declared this word volatile: every write to
// it in the game (0x496861, 0x4975c0 and
// 0x497c57) is a word read-modify-write through a register
// (`mov cx,[m]; or ecx,4; mov [m],cx`), which MSVC 5 emits only for a
// volatile field; every non-volatile form (`w |= 4`, `w = w & ~4`, a 1-bit
// bitfield store, a struct copy) compiles to `or byte ptr [m],4` or
// `and word ptr [m],0xfffb` straight to memory. Accepted as the original
// declaration (network state, likely also touched from DirectPlay's thread);
// see docs/consolidation.md.

// FUNCTION: 0x494e70
void UpdateCdCategoryByActivity()
{
    if ((g_game->flags_2a44 & 4)
        && (!(g_game->netFlags & 1) || (g_game->netFlags & 2))
        && GetTicks() > g_cdActivityLastSampleTick + 0x1e) {
        int state = g_game->field_10->GetTrackCategory();
        if (++g_cdActivityStableTicks > 10) {
            int recent = 0;
            int n = 5;
            int i = DAT_0051f2dc - 1;
            int total = 0;
            int* p = &DAT_0051e710[i];
            for (;;) {
                if (i < 0) {
                    i += 30;
                    p += 30;
                }
                int v = *p;
                total += v;
                if (n != 0) {
                    n--;
                    recent += v;
                }
                if (i == DAT_0051f2dc)
                    break;
                i--;
                p--;
            }
            int newstate = g_lastCdActivityMode;
            if (state == 0 && (total > 0x32 || recent > 0x1e)
                && g_game->players[g_game->localPlayer].field_144 > 0x1e)
                newstate = 1;
            else if (state == 1 && total < 10 && recent == 0 && g_cdActivityStableTicks > 0x3c)
                newstate = 0;
            if (newstate != g_lastCdActivityMode) {
                ((Sound*)g_game->field_10)->SetTrackCategory(newstate);
                g_cdActivityStableTicks = 0;
                g_lastCdActivityMode = newstate;
            }
        }
        if (++DAT_0051f2dc >= 30)
            DAT_0051f2dc = 0;
        DAT_0051e710[DAT_0051f2dc] = 0;
        ReportIntervalTimer();
        g_cdActivityLastSampleTick = GetTicks();
    }
}

// FUNCTION: 0x494ff0
void __stdcall AddCdActivitySample(int param_1)
{
    DAT_0051e710[DAT_0051f2dc] += param_1;
}

// What the code does: it opens (or closes) the TABMENU.GUI tab menu page. If any
// of bits 5 to 7 of the flags word at +0x2bee is set the page is being closed:
// the bits are cleared and the gui is either hidden, or torn down when
// IsScreenNamed does not find the file. Otherwise the bit 5 "open" flag is set,
// the gui is fetched through LoadGuiLayer and given a click handler and an owner,
// and the three ALLIES / SHARE / CONTROL checkboxes are set. The two first ones
// are enabled when the player count of free, non allied slots is positive and
// the net mode is 3, CONTROL also needs bit 0 of +0x2c74 clear and IsHostLocal
// true.

// FUNCTION: 0x495010
void ToggleTabMenu()
{
    PlaySoundByName("SmallButton", 0);
    unsigned short f = g_game->flags_2bee;
    if (f & 0xe0) {
        g_game->flags_2bee = f & 0xff1f;
        if (IsScreenNamed(&g_game->menu, "TABMENU.GUI"))
            CloseTopScreen(&g_game->menu);
        return;
    }
    g_game->flags_2bee = (f & 0xff3f) | 0x20;
    HideSoftwareCursor();
    Layer* d = LoadGuiLayer(&g_game->menu, "TABMENU.GUI", 0x800);
    d->owner = g_game;
    d->handler = HandleTabMenuEvent;

    // Single count++ body, no extra locals: sets the register split of the scan.
    int count = 0;
    Player* p = g_game->players;
    for (int i = 0; i < 10; i++, p++) {
        if (p->active != 0 && p->type == 1)
            continue;
        if (p->info->bit6)
            continue;
        count++;
    }
    // The players[localPlayer] scale by 0x14b is left to the compiler.
    int mode = g_game->net->GetGameType();
    if (mode == 3 && !g_game->players[g_game->localPlayer].info->bit6) {
        int v = count > 0;
        SetGadgetActiveByName(&g_game->menu, "ALLIES", v);
        SetGadgetActiveByName(&g_game->menu, "SHARE", v);
        int ctl = !(g_game->field_2c74 & 1) && IsHostLocal();
        SetGadgetActiveByName(&g_game->menu, "CONTROL", ctl);
    } else {
        SetGadgetActiveByName(&g_game->menu, "ALLIES", 0);
        SetGadgetActiveByName(&g_game->menu, "SHARE", 0);
        SetGadgetActiveByName(&g_game->menu, "CONTROL", 0);
    }
    EnableKeyCommands(&g_game->menu);
    RenderLayer(&g_game->menu, 0x40);
    ShowSoftwareCursor();
}

// Opens a dialog whose GUI file name is kept in the game object, with
// HandleMain2LayoutEvent as its handler and the game object as its owner.

// FUNCTION: 0x495200
void OpenMain2Layout()
{
    Layer* gadget = LoadGuiLayer(&g_game->menu, g_game->guiName, 0x20);
    gadget->handler = HandleMain2LayoutEvent;
    gadget->owner = g_game;
}

// Frame pacing: measures the time since the last call, finds the slowest
// active player (the one with the lowest tick counter that still has units),
// and turns the lag behind the fastest into a speed factor. That factor scales
// the elapsed time into a whole number of game ticks for this frame (the
// fraction is carried over in a float), which is clamped to 0..5 and to 0
// while paused. Too many capped frames in a row lowers the current speed step,
// a long run of idle ones raises it again.

// FUNCTION: 0x495230
void UpdateFramePacing()
{
    unsigned int now = GetTicks();
    g_game->elapsed = now - g_game->lastTick;
    g_game->lastTick = now;
    unsigned short speed = g_game->speed;
    double rate = speed * 0.1;
    g_game->faster = speed < g_game->maxSpeed;

    int best = g_game->ticks;
    g_game->slowest = 0;
    for (Player* p = g_game->players; p != g_game->players + 10; p++) {
        if (p->active != 0 && p->type == 3 && p->field_144 > 0) {
            int tick = p->tick;
            if (tick < best) {
                g_game->slowest = p;
                best = tick;
            }
        }
    }
    g_game->lag = g_game->ticks - best;

    if (g_game->lag >= 900) {
        int lag = g_game->lag;
        if (lag > 3600)
            lag = 3600;
        double f = (3600 - lag) * 0.00037037037037037035;
        if (f < 0.01)
            f = 0.01;
        g_game->lagging = 1;
        rate *= f;
    } else {
        g_game->lagging = 0;
    }

    double x = g_game->elapsed * rate + g_game->carry;
    double whole = floor(x);
    g_game->steps = (int)whole;
    // No (float) cast: with it the fsub moves after the steps store.
    g_game->carry = x - whole;
    if (g_game->steps < 0)
        g_game->steps = 0;
    if (g_game->paused) {
        g_game->steps = 0;
        return;
    }
    if (g_game->steps > 5) {
        g_game->steps = 5;
        g_game->streak++;
        if (g_game->streak > 10) {
            g_game->streak = 0;
            if (g_game->speed > 1)
                g_game->speed--;
        }
    } else {
        g_game->streak--;
        if (g_game->streak < -100) {
            g_game->streak = 0;
            if (g_game->speed < g_game->maxSpeed)
                g_game->speed++;
        }
    }
}

// Runs one game update per pending time step (g_game->steps), profiling each
// phase into g_game->prof.acc[]; the profile object also lives at +0x38d85 with
// total at +4, the display copy at +8 and the accumulators at +0x2c.

// FUNCTION: 0x495490
void __stdcall RunGameSteps(int showStats)
{
    int n = g_game->steps;

    while (n--) {
        g_game->ticks++;

        if (showStats) {
            HandleNetPackets();
            g_game->prof.AccumulateProfileTime(0);
        }
        UpdateAllUnits();
        g_game->prof.AccumulateProfileTime(1);
        UpdateProjectiles();
        g_game->prof.AccumulateProfileTime(7);
        UpdateExplosions();
        g_game->prof.AccumulateProfileTime(8);
        UpdatePlayers();
        g_game->prof.AccumulateProfileTime(2);

        UpdateFeatures();
        StepAllGafSequences();
        UpdateWind();
        UpdateMeteors();
        UpdateCameraFollow();
        g_game->prof.AccumulateProfileTime(8);

        UpdateParticles();
        g_game->prof.AccumulateProfileTime(6);
        UpdateBlink();
        g_game->prof.AccumulateProfileTime(8);

        if (showStats && g_usePacketManager != 0) {
            UpdateResourceSharing(&g_game->players[g_game->localPlayer]);
            g_packetManager.SendAllQueued(0);
            g_game->prof.AccumulateProfileTime(0);
        }
    }

    EmptyPostSimStepHook();
    EmptyPostSimStepHook_B();
    EmptyPostSimStepHook_C();
    ExpireOldestMessage();
    ExpireEyeballs();
    g_game->prof.AccumulateProfileTime(8);
}

// Mission-script event dispatcher: the argument selects one of six cases out
// of 0x3d..0x70. The 0x3d walk keeps a byte counter in ebx next to a separate
// byte-offset induction variable in eax, and the redundant `i < 10` guard
// inside the do-while is what leaves the preheader test in the binary.

// FUNCTION: 0x4956c0
void __stdcall HandleDebugHotkey(int eventType)
{
    switch (eventType) {
    case 0x69:
        g_game->bit0_3923b = !g_game->bit0_3923b;
        break;
    case 0x6d:
        g_game->counter_14280++;
        if (g_game->counter_14280 == 5)
            g_game->counter_14280 = 0;
        break;
    case 0x50:
        SetPageFlipping(1);
        break;
    case 0x70:
        SetPageFlipping(0);
        break;
    case 0x5d: {
        Unit* u = &g_game->units[g_game->field_2cba];
        if (u->type != 0) {
            u->field_f4 = 10;
            u->flag_110 = 1;
            u->field_f0 = 0;
        }
        break;
    }
    case 0x3d: {
        int k = 0;
        unsigned char i = 0;
        do {
            if (i < 10) {
                if (g_game->players[k].active != 0) {
                    unsigned char kind = g_game->players[k].type;
                    if (kind == 1 || kind == 2 || kind == 3) {
                        if (g_game->players[k].field_146 != 10) {
                            g_game->players[k].energy = g_game->players[k].energyCapacity;
                            g_game->players[k].metal = g_game->players[k].metalCapacity;
                        }
                    }
                }
            }
            i++;
            k++;
        } while (i < 10);
        break;
    }
    default:
        break;
    }
}
